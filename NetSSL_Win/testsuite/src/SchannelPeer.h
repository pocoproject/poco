//
// SchannelPeer.h
//
// Definition of the SchannelPeer class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SchannelPeer_INCLUDED
#define SchannelPeer_INCLUDED


#include "Poco/Net/SecureSocketImpl.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/Context.h"
#include <string>


class SchannelPeer
	/// A TLS peer that calls Schannel directly on a plain socket. The tests use it to
	/// send a secure socket byte arrangements that the library does not produce by
	/// itself: records coalesced into one write, a session ticket held back and
	/// written together with application data, close_notify after data, corrupted
	/// and empty records.
{
public:
	enum Role
	{
		CLIENT,
		SERVER
	};

	SchannelPeer(Role role, const Poco::Net::StreamSocket& socket, Poco::Net::Context::Ptr pContext);
		/// Uses the credentials of the Context (its certificate and protocol settings).

	~SchannelPeer();
		/// Deletes the security context; the socket is not closed.

	std::string handshake(bool sendFinalRecords = true);
		/// Runs the TLS handshake with the peer on the socket until it is complete.
		/// The records of the final handshake call (a TLS 1.3 server's session ticket)
		/// are sent unless sendFinalRecords is false; then they are returned so that a
		/// test can write them together with other records. A CLIENT always sends them,
		/// as its Finished message is among them.

	std::string encrypt(const std::string& data);
		/// Returns the record(s) that carry the data; nothing is sent.

	std::string closeNotify();
		/// Returns the close_notify record; nothing is sent.

	void send(const std::string& bytes);
		/// Writes the bytes to the socket in one call.

	std::string receive();
		/// Returns the next decrypted application data. A post-handshake message is handed
		/// to the handshake, which is completed with the peer. Returns an empty string at
		/// close_notify and when the peer closed the connection.

private:
	SchannelPeer(const SchannelPeer&) = delete;
	SchannelPeer& operator = (const SchannelPeer&) = delete;

	SECURITY_STATUS handshakeCall(const std::string& input, std::string& output);
		/// Makes one InitializeSecurityContextW (CLIENT) or AcceptSecurityContext (SERVER)
		/// call with the input, appends its output token to output and puts the input
		/// that Schannel did not consume back in front of the unread input.

	std::string handshakeLoop(bool sendFinal);
		/// Answers the peer's handshake records until the handshake call returns SEC_E_OK.
		/// The output of the final call is sent if sendFinal is true, otherwise returned.

	std::string applyControlToken(DWORD token);
		/// Applies the control token and returns the output of the handshake call that follows.

	bool fill();
		/// Appends what the socket delivers to the unread input. Returns false when the
		/// peer has closed the connection.

	bool inputEndsOnRecordBoundary() const;
		/// Returns true if the unread input is one or more complete TLS records, or if
		/// it holds a header that is no TLS record header, which the handshake call rejects.

	Role _role;
	Poco::Net::StreamSocket _socket;
	Poco::Net::Context::Ptr _pContext;
	PSecurityFunctionTableW _pSecurityFunctions;
	CtxtHandle _hContext;
	std::string _input;
};


#endif // SchannelPeer_INCLUDED
