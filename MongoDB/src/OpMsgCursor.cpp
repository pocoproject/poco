//
// OpMsgCursor.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  OpMsgCursor
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/Bugcheck.h"
#include "Poco/Exception.h"

//
// NOTE:
//
// MongoDB specification indicates that the flag MSG_EXHAUST_ALLOWED shall be
// used in the request when the receiver is ready to receive multiple messages
// without sending additional requests in between. Sender (MongoDB) indicates
// that more messages follow with flag MSG_MORE_TO_COME.
//
// It seems that this does not work properly. MSG_MORE_TO_COME is set and reading
// next messages sometimes works, however often the data is missing in response
// or the message header contains wrong message length and reading blocks.
// Opcode in the header is correct.
//
// Using MSG_EXHAUST_ALLOWED is therefore currently disabled.
//
// It seems that related JIRA ticket is:
//
// https://jira.mongodb.org/browse/SERVER-57297
//
// https://github.com/mongodb/specifications/blob/master/source/message/OP_MSG.rst
//

#define _MONGODB_EXHAUST_ALLOWED_WORKS	false

using namespace std::string_literals;

namespace Poco {
namespace MongoDB {


static const std::string keyCursor		{"cursor"s};
static const std::string keyCursors		{"cursors"s};
static const std::string keyBatchSize	{"batchSize"s};
static const std::string keyId			{"id"s};
static const std::string keyCursorsKilled {"cursorsKilled"s};

static Poco::Int64 cursorIdFromResponse(const MongoDB::Document& doc);


OpMsgCursor::OpMsgCursor(const std::string& db, const std::string& collection):
#if _MONGODB_EXHAUST_ALLOWED_WORKS
	_query(db, collection, OpMsgMessage::MSG_EXHAUST_ALLOWED)
#else
	_query(db, collection)
#endif
{
}

OpMsgCursor::~OpMsgCursor()
{
	try
	{
		poco_assert_msg_dbg(_cursorID == 0, "OpMsgCursor destroyed with active cursor - call kill() before destruction");
	}
	catch (...)
	{
	}
}


void OpMsgCursor::setEmptyFirstBatch(bool empty) noexcept
{
	_emptyFirstBatch = empty;
}


bool OpMsgCursor::emptyFirstBatch() const noexcept
{
	return _emptyFirstBatch;
}


void OpMsgCursor::setBatchSize(Int32 batchSize) noexcept
{
	_batchSize = batchSize;
}


Int32 OpMsgCursor::batchSize() const noexcept
{
	return _batchSize;
}


bool OpMsgCursor::isActive() const noexcept
{
	const auto& cmd {_query.commandName()};
	return ( _cursorID > 0 || (!cmd.empty() && cmd != OpMsgMessage::CMD_GET_MORE) );
}


template<typename ConnType>
OpMsgMessage& OpMsgCursor::nextImpl(ConnType& connection)
{
	if (_cursorID == 0)
	{
		_response.clear();

		if (!isActive())
		{
			// Cursor reached the end of data. Nothing to provide.
			return _response;
		}

		if (_emptyFirstBatch || _batchSize > 0)
		{
			Int32 bsize = _emptyFirstBatch ? 0 : _batchSize;
			auto& body { _query.body() };
			if (_query.commandName() == OpMsgMessage::CMD_FIND)
			{
				// Prevent duplicated fields if next() fails due to communication
				// issues and is the used again.
				body.remove(keyBatchSize);
				body.add(keyBatchSize, bsize);
			}
			else if (_query.commandName() == OpMsgMessage::CMD_AGGREGATE)
			{
				// Prevent duplicated fields if next() fails due to communication
				// issues and is the used again.
				body.remove(keyCursor);
				auto& cursorDoc = body.addNewDocument(keyCursor);
				cursorDoc.add(keyBatchSize, bsize);
			}
		}

		connection.sendRequest(_query, _response);

		const auto& rdoc = _response.body();
		_cursorID = cursorIdFromResponse(rdoc);
	}
	else
	{
#if _MONGODB_EXHAUST_ALLOWED_WORKS
		std::cout << "Response flags: " << _response.flags() << std::endl;
		if (_response.flags() & OpMsgMessage::MSG_MORE_TO_COME)
		{
			std::cout << "More to come. Reading more response: " << std::endl;
			_response.clear();
			connection.readResponse(_response);
		}
		else
#endif
		{
			_response.clear();
			_query.setCursor(_cursorID, _batchSize);
			connection.sendRequest(_query, _response);
		}
	}

	const auto& rdoc = _response.body();
	_cursorID = cursorIdFromResponse(rdoc);

	return _response;
}


OpMsgMessage& OpMsgCursor::next(Connection& connection)
{
	return nextImpl(connection);
}


OpMsgMessage& OpMsgCursor::next(ReplicaSetConnection& connection)
{
	return nextImpl(connection);
}


template<typename ConnType>
void OpMsgCursor::killImpl(ConnType& connection)
{
	_response.clear();
	if (_cursorID != 0)
	{
		_query.setCommandName(OpMsgMessage::CMD_KILL_CURSORS);

		MongoDB::Array::Ptr cursors = new MongoDB::Array();
		cursors->add<Poco::Int64>(_cursorID);
		_query.body().add(keyCursors, cursors);

		connection.sendRequest(_query, _response);

		const auto killed = _response.body().get<MongoDB::Array::Ptr>(keyCursorsKilled, nullptr);
		if (!killed || killed->size() != 1 || killed->get<Poco::Int64>(0, -1) != _cursorID)
		{
			throw Poco::ProtocolException("Cursor not killed as expected: "s + std::to_string(_cursorID));
		}

		_cursorID = 0;
		_query.clear();
		_response.clear();
	}
}


void OpMsgCursor::kill(Connection& connection)
{
	killImpl(connection);
}


void OpMsgCursor::kill(ReplicaSetConnection& connection)
{
	killImpl(connection);
}


Poco::Int64 cursorIdFromResponse(const MongoDB::Document& doc)
{
	Poco::Int64 id {0};
	auto cursorDoc = doc.get<Document::Ptr>(keyCursor, nullptr);
	if(cursorDoc)
	{
		id = cursorDoc->get<Poco::Int64>(keyId, 0);
	}
	return id;
}


// Explicit template instantiation
template OpMsgMessage& OpMsgCursor::nextImpl<Connection>(Connection& connection);
template OpMsgMessage& OpMsgCursor::nextImpl<ReplicaSetConnection>(ReplicaSetConnection& connection);
template void OpMsgCursor::killImpl<Connection>(Connection& connection);
template void OpMsgCursor::killImpl<ReplicaSetConnection>(ReplicaSetConnection& connection);


} } // Namespace Poco::MongoDB
