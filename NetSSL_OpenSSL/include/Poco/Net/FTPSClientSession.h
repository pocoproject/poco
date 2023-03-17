//
// FTPSClientSession.h
//
// Library: NetSSL_OpenSSL
// Package: FTPS
// Module:  FTPSClientSession
//
// Definition of the FTPSClientSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_FTPSClientSession_INCLUDED
#define NetSSL_FTPSClientSession_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/FTPClientSession.h"


namespace Poco {
namespace Net {


class NetSSL_API FTPSClientSession: public Poco::Net::FTPClientSession
	/// This is an extension of FTPClientSession that supports
	/// FTP over SSL/TLS using the AUTH SSL/AUTH TLS and PBSZ/PROT
	/// commands according to RFC 4217.
{
public:
	FTPSClientSession();
		/// Creates an FTPSClientSession.
		///
		/// Passive mode will be used for data transfers.

	explicit FTPSClientSession(Context::Ptr pContext);
		/// Creates an FTPSClientSession using the given Context.
		///
		/// Passive mode will be used for data transfers.

	FTPSClientSession(const StreamSocket& socket, bool readWelcomeMessage = true, bool enableFTPS = true, Context::Ptr pContext = nullptr);
		/// Creates an FTPSClientSession using the given
		/// connected socket for the control connection.
		///
		/// Passive mode will be used for data transfers.

	FTPSClientSession(const std::string& host, Poco::UInt16 port = FTP_PORT, const std::string& username = "", const std::string& password = "", Context::Ptr pContext = nullptr);
		/// Creates an FTPSClientSession using a socket connected
		/// to the given host and port. If username is supplied,
		/// login is attempted.
		///
		/// Passive mode will be used for data transfers.

	virtual ~FTPSClientSession();

	void enableFTPS(bool enable = true);
		/// Enable or disable FTPS (FTP over SSL/TLS).

	bool isSecure() const;
		/// Returns true if the session is FTPS.

	void forceSessionReuse(bool force = true);
		/// Enable or disable session reusing

protected:
	virtual StreamSocket establishDataConnection(const std::string& command, const std::string& arg);
		/// Create secure data connection

	virtual void receiveServerReadyReply();
		/// Function that read server welcome message after connetion and set and make secure socket

private:
	void beforeCreateDataSocket();
		///Send commands to check if we can encrypt data socket

	void afterCreateControlSocket();
		///Send commands to make SSL negotiating of control channel

	bool _enableFTPS = true;
	bool _secureDataConnection = false;
	bool _forceSessionReuse = false;
	Context::Ptr _pContext;
};


//
// inlines
//
inline bool FTPSClientSession::isSecure() const
{
	if (_pControlSocket != nullptr)
		return _pControlSocket->secure();
	return false;
}


inline void FTPSClientSession::forceSessionReuse(bool force)
{
	_forceSessionReuse = force;
}


} } // namespace Poco::Net


#endif // #define NetSSL_FTPSClientSession_INCLUDED
