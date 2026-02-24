//
// Message.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Message
//
// Definition of the Message class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Message_INCLUDED
#define MongoDB_Message_INCLUDED


#include "Poco/Bugcheck.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/Types.h"


namespace Poco {
namespace MongoDB {


class MongoDB_API Message
	/// Base class for all messages send or retrieved from MongoDB server.
	///
	/// INTERNAL: This is a base class for OpMsgMessage and not intended
	/// for direct use. Use OpMsgMessage instead.
{
public:
	explicit Message(MessageHeader::OpCode opcode);
		/// Creates a Message using the given OpCode.

	virtual ~Message();
		/// Destructor

	[[nodiscard]] MessageHeader& header();
		/// Returns the message header

protected:
	MessageHeader _header;

	void messageLength(Poco::Int32 length);
		/// Sets the message length in the message header
};


//
// inlines
//
inline MessageHeader& Message::header()
{
	return _header;
}


inline void Message::messageLength(Poco::Int32 length)
{
	poco_assert(length > 0);
	_header.setMessageLength(length);
}


} } // namespace Poco::MongoDB


#endif // MongoDB_Message_INCLUDED
