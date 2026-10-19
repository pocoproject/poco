//
// StreamClient.h
//
// Library: AI/MCP
// Package: Core
// Module:  StreamClient
//
// Definition of the StreamClient class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_StreamClient_INCLUDED
#define AIMCP_StreamClient_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Client.h"
#include <iosfwd>


namespace Poco::AI::MCP {


class AIMCP_API StreamClient: public Client
	/// A client over a pair of streams that carry newline-delimited messages,
	/// e.g. the two ends of an in-process pipe, or of a connection the
	/// application opened. For a server process, use ProcessClient.
	///
	/// After a request, the client reads until the reply arrives: it passes
	/// over notifications, replies to other ids, requests (which a server
	/// must not send on a stream) and lines that are not JSON objects.
	///
	/// The reads block: a stream read cannot be timed out, so the timeout of
	/// the client has no effect here.
{
public:
	StreamClient(std::istream& in, std::ostream& out, std::size_t maxMessageSize = DEFAULT_MAX_MESSAGE_SIZE);
		/// Creates a client that writes its requests to out and reads the
		/// replies from in. A line longer than maxMessageSize bytes fails the
		/// request it arrives during: it may be the reply. Both streams must
		/// outlive the client.

	~StreamClient() override;
		/// Destroys the client.

protected:
	Poco::JSON::Object::Ptr exchange(const Poco::JSON::Object::Ptr& request) override;
		/// Throws Poco::IOException when the stream ends before the reply, and
		/// Poco::ProtocolException for a line longer than the size limit.

private:
	std::istream& _in;
	std::ostream& _out;
	std::size_t _maxMessageSize;
};


} // namespace Poco::AI::MCP


#endif // AIMCP_StreamClient_INCLUDED
