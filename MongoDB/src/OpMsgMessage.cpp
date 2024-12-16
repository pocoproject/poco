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
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"

#define POCO_MONGODB_DUMP	false

namespace Poco {
namespace MongoDB {

// Query and write
const std::string OpMsgMessage::CMD_INSERT { "insert"s };
const std::string OpMsgMessage::CMD_DELETE { "delete"s };
const std::string OpMsgMessage::CMD_UPDATE { "update"s };
const std::string OpMsgMessage::CMD_FIND { "find"s };
const std::string OpMsgMessage::CMD_FIND_AND_MODIFY { "findAndModify"s };
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
const std::string OpMsgMessage::CMD_DROP { "drop"s };
const std::string OpMsgMessage::CMD_DROP_DATABASE { "dropDatabase"s };
const std::string OpMsgMessage::CMD_KILL_CURSORS { "killCursors"s };
const std::string OpMsgMessage::CMD_LIST_DATABASES { "listDatabases"s };
const std::string OpMsgMessage::CMD_LIST_INDEXES { "listIndexes"s };

// Diagnostic
const std::string OpMsgMessage::CMD_BUILD_INFO { "buildInfo"s };
const std::string OpMsgMessage::CMD_COLL_STATS { "collStats"s };
const std::string OpMsgMessage::CMD_DB_STATS { "dbStats"s };
const std::string OpMsgMessage::CMD_HOST_INFO { "hostInfo"s };


static const std::string& commandIdentifier(const std::string& command);
	/// Commands have different names for the payload that is sent in a separate section


static const std::string keyCursor		{ "cursor"s };
static const std::string keyFirstBatch	{ "firstBatch"s };
static const std::string keyNextBatch	{ "nextBatch"s };

constexpr static Poco::UInt8 PAYLOAD_TYPE_0 { 0 };
constexpr static Poco::UInt8 PAYLOAD_TYPE_1 { 1 };

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
	_body.add("$db"s, _databaseName);
}


void OpMsgMessage::setCursor(Poco::Int64 cursorID, Poco::Int32 batchSize)
{
	_commandName = OpMsgMessage::CMD_GET_MORE;
	_body.clear();

	// IMPORTANT: Command name must be first
	_body.add(_commandName, cursorID);
	_body.add("$db"s, _databaseName);
	_body.add("collection"s, _collectionName);
	if (batchSize > 0)
	{
		_body.add("batchSize"s, batchSize);
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
	if (_body.exists("ok"s))
	{
		ok = _body.getInteger("ok"s);
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
	BinaryWriter socketWriter(ostr, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);

	// Serialise the body
	std::stringstream ss;
	BinaryWriter writer(ss, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer << _flags;

	writer << PAYLOAD_TYPE_0;
	_body.write(writer);

	if (!_documents.empty())
	{
		// Serialise attached documents

		std::stringstream ssdoc;
		BinaryWriter wdoc(ssdoc, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		for (auto& doc: _documents)
		{
			doc->write(wdoc);
		}
		wdoc.flush();

		const std::string& identifier = commandIdentifier(_commandName);
		const Poco::Int32 size = static_cast<Poco::Int32>(sizeof(size) + identifier.size() + 1 + ssdoc.tellp());
		writer << PAYLOAD_TYPE_1;
		writer << size;
		writer.writeCString(identifier.c_str());
		StreamCopier::copyStream(ssdoc, ss);
	}
	writer.flush();

#if POCO_MONGODB_DUMP
	const std::string section = ss.str();
	std::string dump;
	Logger::formatDump(dump, section.data(), section.length());
	std::cout << dump << std::endl;
#endif

	messageLength(static_cast<Poco::Int32>(ss.tellp()));

	_header.write(socketWriter);
	StreamCopier::copyStream(ss, ostr);

	ostr.flush();
}


void OpMsgMessage::read(std::istream& istr)
{
	std::string message;
	{
		BinaryReader reader(istr, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		_header.read(reader);

		poco_assert_dbg(_header.opCode() == _header.OP_MSG);

		const std::streamsize remainingSize { static_cast<std::streamsize>(_header.getMessageLength() - _header.MSG_HEADER_SIZE) };
		message.reserve(remainingSize);

#if POCO_MONGODB_DUMP
		std::cout
			<< "Message hdr: " << _header.getMessageLength() << " " << remainingSize << " "
			<< _header.opCode() << " " << _header.getRequestID() << " " << _header.responseTo()
			<< std::endl;
#endif

		reader.readRaw(remainingSize, message);

#if POCO_MONGODB_DUMP
		std::string dump;
		Logger::formatDump(dump, message.data(), message.length());
		std::cout << dump << std::endl;
#endif
	}
	// Read complete message and then interpret it.

	std::istringstream msgss(message);
	BinaryReader reader(msgss, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);

	Poco::UInt8 payloadType {0xFF};

	reader >> _flags;
	reader >> payloadType;
	poco_assert_dbg(payloadType == PAYLOAD_TYPE_0);

	_body.read(reader);

	// Read next sections from the buffer
	while (msgss.good())
	{
		// NOTE: Not tested yet with database, because it returns everything in the body.
		// Does MongoDB ever return documents as Payload type 1?
		reader >> payloadType;
		if (!msgss.good())
		{
			break;
		}
		poco_assert_dbg(payloadType == PAYLOAD_TYPE_1);
#if POCO_MONGODB_DUMP
		std::cout << "section payload: " << payloadType << std::endl;
#endif

		Poco::Int32 sectionSize {0};
		reader >> sectionSize;
		poco_assert_dbg(sectionSize > 0);

#if POCO_MONGODB_DUMP
		std::cout << "section size: " << sectionSize << std::endl;
#endif
		std::streamoff offset = sectionSize - sizeof(sectionSize);
		std::streampos endOfSection = msgss.tellg() + offset;

		std::string identifier;
		reader.readCString(identifier);
#if POCO_MONGODB_DUMP
		std::cout << "section identifier: " << identifier << std::endl;
#endif

		// Loop to read documents from this section.
		while (msgss.tellg() < endOfSection)
		{
#if POCO_MONGODB_DUMP
			std::cout << "section doc: " << msgss.tellg() << " " << endOfSection << std::endl;
#endif
			Document::Ptr doc = new Document();
			doc->read(reader);
			_documents.push_back(doc);
			if (msgss.tellg() < 0)
			{
				break;
			}
		}
	}

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


} } // namespace Poco::MongoDB
