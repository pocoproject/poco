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
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"

namespace Poco {
namespace MongoDB {

// Query and write
const std::string OpMsgMessage::CMD_INSERT { "insert" };
const std::string OpMsgMessage::CMD_DELETE { "delete" };
const std::string OpMsgMessage::CMD_UPDATE { "update" };
const std::string OpMsgMessage::CMD_FIND { "find" };
const std::string OpMsgMessage::CMD_FIND_AND_MODIFY { "findAndModify" };
const std::string OpMsgMessage::CMD_GET_MORE { "getMore" };

// Aggregation
const std::string OpMsgMessage::CMD_AGGREGATE { "aggregate" };
const std::string OpMsgMessage::CMD_COUNT { "count" };
const std::string OpMsgMessage::CMD_DISTINCT { "distinct" };
const std::string OpMsgMessage::CMD_MAP_REDUCE { "mapReduce" };

// Replication and administration 
const std::string OpMsgMessage::CMD_HELLO { "hello" };

const std::string OpMsgMessage::CMD_CREATE { "create" };
const std::string OpMsgMessage::CMD_CREATE_INDEXES { "createIndexes" };
const std::string OpMsgMessage::CMD_DROP { "drop" };
const std::string OpMsgMessage::CMD_DROP_DATABASE { "dropDatabase" };
const std::string OpMsgMessage::CMD_KILL_CURSORS { "killCursors" };
const std::string OpMsgMessage::CMD_LIST_DATABASES { "listDatabases" };
const std::string OpMsgMessage::CMD_LIST_INDEXES { "listIndexes" };

// Diagnostic
const std::string OpMsgMessage::CMD_BUILD_INFO { "buildInfo" };
const std::string OpMsgMessage::CMD_COLL_STATS { "collStats" };
const std::string OpMsgMessage::CMD_DB_STATS { "dbStats" };
const std::string OpMsgMessage::CMD_HOST_INFO { "hostInfo" };


static const std::string& commandIdentifier(const std::string& command);
	/// Commands have different names for the payload that is sent in a separate section

OpMsgMessage::OpMsgMessage() :
	Message(MessageHeader::OP_MSG)
{
}


OpMsgMessage::OpMsgMessage(const std::string& databaseName, const std::string& collectionName) :
	Message(MessageHeader::OP_MSG),
	_databaseName(databaseName),
	_collectionName(collectionName)
{
}


#if false
OpMsgMessage::OpMsgMessage(const Int64& cursorID) :
	Message(MessageHeader::OP_MSG)
{
}
#endif


OpMsgMessage::~OpMsgMessage()
{
}


void OpMsgMessage::setCommandName(const std::string& command)
{
	_commandName = command;
	_body.clear();

	// IMPORTANT: Command name must be first
	_body.add(_commandName, _collectionName);
	_body.add("$db", _databaseName);
}


Document& OpMsgMessage::body()
{
	return _body;
}


Document::Vector& OpMsgMessage::documents()
{
	return _documents;
}


void OpMsgMessage::clear()
{
	_flags = MSG_FLAGS_DEFAULT;
	_commandName.clear();
	_documents.clear();
	_body.clear();
}


void OpMsgMessage::send(std::ostream& ostr)
{

	std::cout << "send body: " << _body.toString() << std::endl;

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
		const Poco::Int32 size = sizeof(size) + identifier.size() + 1 + ssdoc.tellp();
		writer << PAYLOAD_TYPE_1;
		writer << size;
		writer.writeCString(identifier.c_str());
		StreamCopier::copyStream(ssdoc, ss);
	}
	writer.flush();

	const std::string section = ss.str();
	std::string dump;
	Logger::formatDump(dump, section.data(), section.length());

	std::cout << dump << std::endl;

	messageLength(static_cast<Poco::Int32>(ss.tellp()));

	_header.write(socketWriter);
	StreamCopier::copyStream(ss, ostr);

	ostr.flush();
}


void OpMsgMessage::read(std::istream& istr)
{
	clear();

	std::string message;
	{
		BinaryReader reader(istr, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		_header.read(reader);

		poco_assert_dbg(_header.opCode() == _header.OP_MSG);

		const std::streamsize remainingSize {_header.getMessageLength() - _header.MSG_HEADER_SIZE };
		message.reserve(remainingSize);

		reader.readRaw(remainingSize, message);

		std::string dump;
		Logger::formatDump(dump, message.data(), message.length());

		std::cout << dump << std::endl;
	}
	// Read complete message and then interpret it.

	std::istringstream msgss(message);
	BinaryReader reader(msgss, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);

	Poco::UInt32 flags {0xFFFFFFFF};
	Poco::UInt8 payloadType {0xFF};

	reader >> flags;
	_flags = static_cast<Flags>(flags);

	reader >> payloadType;
	poco_assert_dbg(payloadType == PAYLOAD_TYPE_0);

	_body.read(reader);

	// Read next sections from the buffer
	while (msgss.good())
	{
		// NOTE: Not tested yet with database, because it returns everything in the body.
		reader >> payloadType;
		if (!msgss.good())
		{
			break;
		}
		poco_assert_dbg(payloadType == PAYLOAD_TYPE_1);
		std::cout << "section payload: " << payloadType << std::endl;

		Poco::Int32 sectionSize {0};
		reader >> sectionSize;
		poco_assert_dbg(sectionSize > 0);

		std::cout << "section size: " << sectionSize << std::endl;
		std::streamoff offset = sectionSize - sizeof(sectionSize);
		std::streampos endOfSection = msgss.tellg() + offset;

		std::string identifier;
		reader.readCString(identifier);
		std::cout << "section identifier: " << identifier << std::endl;

		// Loop to read documents from this section.
		while (msgss.tellg() < endOfSection)
		{
			std::cout << "section doc: " << msgss.tellg() << " " << endOfSection << std::endl;
			Document::Ptr doc = new Document();
			doc->read(reader);
			_documents.push_back(doc);
			if (msgss.tellg() < 0)
			{
				break;
			}
		}
	}

}

const std::string& commandIdentifier(const std::string& command)
{
	// Names of identifiers for commands that send bulk documents in the request
	// The identifier is set in the section type 1.
	static std::map<std::string, std::string> identifiers {
		{ OpMsgMessage::CMD_INSERT, "documents" },
		{ OpMsgMessage::CMD_DELETE, "deletes" },
		{ OpMsgMessage::CMD_UPDATE, "updates" },
		{ OpMsgMessage::CMD_CREATE_INDEXES, "indexes" }
	};

	static const std::string emptyIdentifier;

	const auto i = identifiers.find(command);
	if (i != identifiers.end())
	{
		return i->second;
	}
	// This likely means that documents are incorrectly set for a command
	// that does not send list of documents.
	return emptyIdentifier;
}


} } // namespace Poco::MongoDB
