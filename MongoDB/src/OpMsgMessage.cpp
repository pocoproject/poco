//
// OpMsgMessage.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  OpMsgMessage
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/ByteOrder.h"
#include "Poco/MemoryStream.h"
#include "Poco/StreamCopier.h"
#include <algorithm>
#include <cstring>
#include <istream>
#include <map>
#include <ostream>
#include <sstream>

#define POCO_MONGODB_DUMP	false

#if POCO_MONGODB_DUMP
#include "Poco/Logger.h"
#include <iostream>
#endif

namespace Poco::MongoDB {

// Query and write
const std::string OpMsgMessage::CMD_INSERT { "insert"s };
const std::string OpMsgMessage::CMD_DELETE { "delete"s };
const std::string OpMsgMessage::CMD_UPDATE { "update"s };
const std::string OpMsgMessage::CMD_FIND { "find"s };
const std::string OpMsgMessage::CMD_FIND_AND_MODIFY { "findAndModify"s };
const std::string OpMsgMessage::CMD_BULK_WRITE { "bulkWrite"s };
const std::string OpMsgMessage::CMD_GET_MORE { "getMore"s };

// Aggregation
const std::string OpMsgMessage::CMD_AGGREGATE { "aggregate"s };
const std::string OpMsgMessage::CMD_COUNT { "count"s };
const std::string OpMsgMessage::CMD_DISTINCT { "distinct"s };
const std::string OpMsgMessage::CMD_MAP_REDUCE { "mapReduce"s };

// Replication and administration
const std::string OpMsgMessage::CMD_HELLO { "hello"s };
const std::string OpMsgMessage::CMD_REPL_SET_GET_STATUS { "replSetGetStatus"s };
const std::string OpMsgMessage::CMD_REPL_SET_GET_CONFIG { "replSetGetConfig"s };

const std::string OpMsgMessage::CMD_CREATE { "create"s };
const std::string OpMsgMessage::CMD_CREATE_INDEXES { "createIndexes"s };
const std::string OpMsgMessage::CMD_DROP_INDEXES { "dropIndexes"s };
const std::string OpMsgMessage::CMD_DROP { "drop"s };
const std::string OpMsgMessage::CMD_DROP_DATABASE { "dropDatabase"s };
const std::string OpMsgMessage::CMD_KILL_CURSORS { "killCursors"s };
const std::string OpMsgMessage::CMD_LIST_DATABASES { "listDatabases"s };
const std::string OpMsgMessage::CMD_LIST_INDEXES { "listIndexes"s };
const std::string OpMsgMessage::CMD_LIST_COLLECTIONS { "listCollections"s };
const std::string OpMsgMessage::CMD_RENAME_COLLECTION { "renameCollection"s };
const std::string OpMsgMessage::CMD_COLL_MOD { "collMod"s };
const std::string OpMsgMessage::CMD_GET_PARAMETER { "getParameter"s };
const std::string OpMsgMessage::CMD_SET_PARAMETER { "setParameter"s };

// Diagnostic
const std::string OpMsgMessage::CMD_BUILD_INFO { "buildInfo"s };
const std::string OpMsgMessage::CMD_COLL_STATS { "collStats"s };
const std::string OpMsgMessage::CMD_DB_STATS { "dbStats"s };
const std::string OpMsgMessage::CMD_HOST_INFO { "hostInfo"s };
const std::string OpMsgMessage::CMD_PING { "ping"s };
const std::string OpMsgMessage::CMD_SERVER_STATUS { "serverStatus"s };
const std::string OpMsgMessage::CMD_CONNECTION_STATUS { "connectionStatus"s };
const std::string OpMsgMessage::CMD_EXPLAIN { "explain"s };
const std::string OpMsgMessage::CMD_LIST_COMMANDS { "listCommands"s };
const std::string OpMsgMessage::CMD_GET_LOG { "getLog"s };

// Authentication
const std::string OpMsgMessage::CMD_SASL_START { "saslStart"s };
const std::string OpMsgMessage::CMD_SASL_CONTINUE { "saslContinue"s };


static const std::string& commandIdentifier(const std::string& command);
	/// Commands have different names for the payload that is sent in a separate section


static const std::string keyDb			{ "$db"s };
static const std::string keyCollection	{ "collection"s };
static const std::string keyCursor		{ "cursor"s };
static const std::string keyOk			{ "ok"s };
static const std::string keyFirstBatch	{ "firstBatch"s };
static const std::string keyNextBatch	{ "nextBatch"s };
static const std::string keyBatchSize	{ "batchSize"s };

constexpr static Poco::UInt8 PAYLOAD_TYPE_0 { 0 };
constexpr static Poco::UInt8 PAYLOAD_TYPE_1 { 1 };

// Smallest message that can be parsed: the header and the flags.
constexpr static Poco::Int32 OP_MSG_MIN_SIZE { MessageHeader::MSG_HEADER_SIZE + 4 };

// The server rejects a larger document in a document sequence: a write command
// statement may exceed maxBsonObjectSize by 16 KiB.
constexpr static Poco::Int32 MAX_SEQUENCE_DOCUMENT_SIZE { BSON_MAX_DOCUMENT_SIZE + 16 * 1024 };


[[nodiscard]] static Poco::Int32 int32At(const std::string& message, std::size_t pos)
{
	Poco::Int32 value;
	std::memcpy(&value, message.data() + pos, sizeof(value));
	return ByteOrder::fromLittleEndian(value);
}


[[nodiscard]] static std::size_t readSectionDocument(const std::string& message, std::size_t pos, std::size_t end, Document& doc)
	/// Reads the document at pos, which must end before end, and returns its size.
{
	// The size and minimum checks happen once, in Document.
	MemoryInputStream istr(message.data() + pos, static_cast<std::streamsize>(end - pos));
	BinaryReader reader(istr, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	return static_cast<std::size_t>(doc.read(reader,
		static_cast<Poco::Int32>(std::min<std::size_t>(end - pos, MAX_MESSAGE_SIZE_BYTES))));
}

OpMsgMessage::OpMsgMessage() :
	Message(MessageHeader::OP_MSG)
{
}


OpMsgMessage::OpMsgMessage(const std::string& databaseName, const std::string& collectionName, UInt32 flags) :
	Message(MessageHeader::OP_MSG),
	_databaseName(databaseName),
	_collectionName(collectionName),
	_flags(flags)
{
}


OpMsgMessage::~OpMsgMessage()
{
}

const std::string& OpMsgMessage::databaseName() const
{
	return _databaseName;
}


const std::string& OpMsgMessage::collectionName() const
{
	return _collectionName;
}


void OpMsgMessage::setCommandName(const std::string& command)
{
	_commandName = command;
	_body.clear();

	// IMPORTANT: Command name must be first
	if (_collectionName.empty())
	{
		// Collection is not specified. It is assumed that this particular command does
		// not need it.
		_body.add(_commandName, Int32(1));
	}
	else
	{
		_body.add(_commandName, _collectionName);
	}
	_body.add(keyDb, _databaseName);
}


void OpMsgMessage::setCursor(Poco::Int64 cursorID, Poco::Int32 batchSize)
{
	_commandName = OpMsgMessage::CMD_GET_MORE;
	_body.clear();

	// IMPORTANT: Command name must be first
	_body.add(_commandName, cursorID);
	_body.add(keyDb, _databaseName);
	_body.add(keyCollection, _collectionName);
	if (batchSize > 0)
	{
		_body.add(keyBatchSize, batchSize);
	}
}


const std::string& OpMsgMessage::commandName() const
{
	return _commandName;
}


void OpMsgMessage::setAcknowledgedRequest(bool ack)
{
	const auto& id = commandIdentifier(_commandName);
	if (id.empty())
		return;

	_acknowledged = ack;

	auto writeConcern = _body.get<Document::Ptr>("writeConcern"s, nullptr);
	if (writeConcern)
		writeConcern->remove("w");

	if (ack)
	{
		_flags = _flags & (~MSG_MORE_TO_COME);
	}
	else
	{
		_flags = _flags | MSG_MORE_TO_COME;
		if (!writeConcern)
			_body.addNewDocument("writeConcern"s).add("w"s, 0);
		else
			writeConcern->add("w"s, 0);
	}

}


bool OpMsgMessage::acknowledgedRequest() const
{
	return _acknowledged;
}


UInt32 OpMsgMessage::flags() const
{
	return _flags;
}


Document& OpMsgMessage::body()
{
	return _body;
}


const Document& OpMsgMessage::body() const
{
	return _body;
}


Document::Vector& OpMsgMessage::documents()
{
	return _documents;
}


const Document::Vector& OpMsgMessage::documents() const
{
	return _documents;
}


bool OpMsgMessage::responseOk() const
{
	Poco::Int64 ok {false};
	if (_body.exists(keyOk))
	{
		ok = _body.getInteger(keyOk);
	}
	return (ok != 0);
}


void OpMsgMessage::clear()
{
	_flags = MSG_FLAGS_DEFAULT;
	_commandName.clear();
	_body.clear();
	_documents.clear();
}


void OpMsgMessage::send(std::ostream& ostr)
{
	if (_flags & MSG_CHECKSUM_PRESENT)
		throw Poco::InvalidArgumentException("MongoDB message checksums are not supported");

	BinaryWriter socketWriter(ostr, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);

	// One stream for the flags, the body and the document sequence
	std::stringstream ss;
	BinaryWriter writer(ss, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer << _flags;

	writer << PAYLOAD_TYPE_0;
	_body.write(writer);

	auto tooLarge = [](std::streamoff size)
	{
		return Poco::InvalidArgumentException("MongoDB request of " + std::to_string(size) +
			" bytes exceeds the maximum message size of " + std::to_string(MAX_MESSAGE_SIZE_BYTES) + " bytes");
	};

	if (!_documents.empty())
	{
		const std::string& identifier = commandIdentifier(_commandName);

		writer << PAYLOAD_TYPE_1;
		// The size of the section is patched in once its end is known.
		const std::streamoff sizePos = ss.tellp();
		writer << static_cast<Poco::Int32>(0);
		writer.writeCString(identifier.c_str());
		for (auto& doc: _documents)
		{
			const std::streamoff start = ss.tellp();
			doc->write(writer);
			const std::streamoff size = ss.tellp() - start;
			if (size > MAX_SEQUENCE_DOCUMENT_SIZE)
			{
				throw Poco::InvalidArgumentException("MongoDB document of " + std::to_string(size) +
					" bytes exceeds the maximum of " + std::to_string(MAX_SEQUENCE_DOCUMENT_SIZE) + " bytes");
			}
			// Checked per document, so that the stream cannot grow past what an Int32 holds.
			const std::streamoff written = ss.tellp();
			if (written + MessageHeader::MSG_HEADER_SIZE > MAX_MESSAGE_SIZE_BYTES)
				throw tooLarge(written + MessageHeader::MSG_HEADER_SIZE);
		}
		const std::streamoff end = ss.tellp();
		ss.seekp(sizePos, std::ios_base::beg);
		writer << static_cast<Poco::Int32>(end - sizePos);
		ss.seekp(end, std::ios_base::beg);
	}

	// Checked before the serialised data is copied. The server closes the connection
	// instead of replying to a larger message.
	const std::streamoff payloadSize = ss.tellp();
	if (payloadSize + MessageHeader::MSG_HEADER_SIZE > MAX_MESSAGE_SIZE_BYTES)
		throw tooLarge(payloadSize + MessageHeader::MSG_HEADER_SIZE);

#if POCO_MONGODB_DUMP
	const std::string section = ss.str();
	std::string dump;
	Logger::formatDump(dump, section.data(), section.length());
	std::cout << dump << std::endl;
#endif

	messageLength(static_cast<Poco::Int32>(payloadSize));

	_header.write(socketWriter);

	Poco::StreamCopier::copyStream(ss, ostr);
	ostr.flush();
}


void OpMsgMessage::read(std::istream& istr)
{
	clear();

	std::string message;
	{
		BinaryReader reader(istr, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		_header.read(reader);

		// Only bounds are checked (see Document::read). The errors are DataFormatException:
		// an IOException would make ReplicaSetConnection send the request again.
		const Poco::Int32 length = _header.getMessageLength();
		if (length < OP_MSG_MIN_SIZE || length > MAX_MESSAGE_SIZE_BYTES)
			throw Poco::DataFormatException("Invalid MongoDB message length: " + std::to_string(length));
		const auto payloadSize = static_cast<std::size_t>(length - MessageHeader::MSG_HEADER_SIZE);

#if POCO_MONGODB_DUMP
		std::cout
			<< "Message hdr: " << _header.getMessageLength() << ' ' << payloadSize << ' '
			<< _header.opCode() << ' ' << _header.getRequestID() << ' ' << _header.responseTo()
			<< std::endl;
#endif

		reader.readRaw(static_cast<std::streamsize>(payloadSize), message);
		if (message.size() != payloadSize)
		{
			throw Poco::IOException("Incomplete MongoDB message: expected " + std::to_string(payloadSize) +
				" bytes, received " + std::to_string(message.size()));
		}

		// Checked after the payload, so that the stream stays in sync.
		if (_header.opCode() != MessageHeader::OP_MSG)
			throw Poco::DataFormatException("Unexpected MongoDB message opcode: " + std::to_string(static_cast<Poco::Int32>(_header.opCode())));

#if POCO_MONGODB_DUMP
		std::string dump;
		Logger::formatDump(dump, message.data(), message.length());
		std::cout << dump << std::endl;
#endif
	}

	// Each section and document must fit in the bytes left; documents may exceed 16 MiB.
	_flags = static_cast<UInt32>(int32At(message, 0));

	std::size_t end = message.size();
	if (_flags & MSG_CHECKSUM_PRESENT)
	{
		// CRC-32C of the message; not verified.
		end -= sizeof(Poco::UInt32);
	}

	std::size_t pos = sizeof(_flags);
	bool haveBody = false;
	while (pos < end)
	{
		const auto payloadType = static_cast<Poco::UInt8>(message[pos++]);
		if (payloadType == PAYLOAD_TYPE_0)
		{
			if (haveBody)
				throw Poco::DataFormatException("MongoDB message has more than one body section");
			haveBody = true;
			pos += readSectionDocument(message, pos, end, _body);
		}
		else if (payloadType == PAYLOAD_TYPE_1)
		{
			if (end - pos < sizeof(Poco::Int32))
				throw Poco::DataFormatException("Truncated MongoDB message section");
			const Poco::Int32 sectionSize = int32At(message, pos);
			if (sectionSize < static_cast<Poco::Int32>(sizeof(Poco::Int32) + 1) || static_cast<std::size_t>(sectionSize) > end - pos)
				throw Poco::DataFormatException("Invalid MongoDB message section size: " + std::to_string(sectionSize));
			const std::size_t sectionEnd = pos + static_cast<std::size_t>(sectionSize);

			const std::size_t identifierEnd = message.find('\0', pos + sizeof(Poco::Int32));
			if (identifierEnd == std::string::npos || identifierEnd >= sectionEnd)
				throw Poco::DataFormatException("MongoDB message section identifier is not terminated");

			pos = identifierEnd + 1;
			while (pos < sectionEnd)
			{
				Document::Ptr doc = new Document();
				pos += readSectionDocument(message, pos, sectionEnd, *doc);
				_documents.push_back(doc);
			}
		}
		else
		{
			throw Poco::DataFormatException("Unsupported MongoDB message section kind: " + std::to_string(payloadType));
		}
	}

	if (!haveBody)
		throw Poco::DataFormatException("MongoDB message has no body section");

	// Extract documents from the cursor batch if they are there.
	MongoDB::Array::Ptr batch;
	auto curDoc = _body.get<MongoDB::Document::Ptr>(keyCursor, nullptr);
	if (curDoc)
	{
		batch = curDoc->get<MongoDB::Array::Ptr>(keyFirstBatch, nullptr);
		if (!batch)
		{
			batch = curDoc->get<MongoDB::Array::Ptr>(keyNextBatch, nullptr);
		}
	}
	if (batch)
	{
		// Reserve space to avoid reallocations
		_documents.reserve(_documents.size() + batch->size());

		for(std::size_t i = 0; i < batch->size(); i++)
		{
			const auto& d = batch->get<MongoDB::Document::Ptr>(i, nullptr);
			if (d)
			{
				_documents.push_back(d);
			}
		}
	}

}

const std::string& commandIdentifier(const std::string& command)
{
	// Names of identifiers for commands that send bulk documents in the request
	// The identifier is set in the section type 1.
	static const std::map<std::string, std::string> identifiers {
		{ OpMsgMessage::CMD_INSERT, "documents"s },
		{ OpMsgMessage::CMD_DELETE, "deletes"s },
		{ OpMsgMessage::CMD_UPDATE, "updates"s },

		// Not sure if create index can send document section
		{ OpMsgMessage::CMD_CREATE_INDEXES, "indexes"s }
	};

	const auto i = identifiers.find(command);
	if (i != identifiers.end())
	{
		return i->second;
	}

	// This likely means that documents are incorrectly set for a command
	// that does not send list of documents in section type 1.
	static const std::string emptyIdentifier;
	return emptyIdentifier;
}


} // namespace Poco::MongoDB
