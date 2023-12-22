//
// FTPSClientSession.cpp
//
// Library: NetSSL_OpenSSL
// Package: FTPS
// Module:  FTPSClientSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/FTPSClientSession.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureStreamSocketImpl.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/NetException.h"


namespace Poco {
namespace Net {


FTPSClientSession::FTPSClientSession():
	FTPClientSession()
{
}


FTPSClientSession::FTPSClientSession(Context::Ptr pContext):
	FTPClientSession(),
	_pContext(pContext)
{
}

FTPSClientSession::FTPSClientSession(const StreamSocket& socket, bool readWelcomeMessage, bool enableFTPS, Context::Ptr pContext):
	FTPClientSession(socket, readWelcomeMessage),
	_enableFTPS(enableFTPS),
	_pContext(pContext)
{
}


FTPSClientSession::FTPSClientSession(const std::string& host, Poco::UInt16 port, const std::string& username, const std::string& password, Context::Ptr pContext):
	FTPClientSession(host, port, username, password),
	_pContext(pContext)
{
}


FTPSClientSession::~FTPSClientSession()
{
}


void FTPSClientSession::enableFTPS(bool enable)
{
	_enableFTPS = enable;
}


void FTPSClientSession::beforeCreateDataSocket()
{
	if (_secureDataConnection) return;
	_secureDataConnection = false;
	if (!_pControlSocket->secure()) return;
	std::string sResponse;
	int status = sendCommand("PBSZ 0", sResponse);
	if (isPositiveCompletion(status))
	{
		status = sendCommand("PROT P", sResponse);
		if (isPositiveCompletion(status))
		{
			_secureDataConnection = true;
		}
	}
}


void FTPSClientSession::afterCreateControlSocket()
{
	if (!_enableFTPS) return;
	_pControlSocket->setNoDelay(true);
	if (_pControlSocket->secure()) return;

	std::string sResponse;
	int status = sendCommand("AUTH TLS", sResponse);
	if (!isPositiveCompletion(status))
		status = sendCommand("AUTH SSL", sResponse);

	if (isPositiveCompletion(status))
	{
		// Server support FTPS
		try
		{
			if (!_pContext) _pContext = Poco::Net::SSLManager::instance().defaultClientContext();
			Poco::Net::SecureStreamSocket sss(Poco::Net::SecureStreamSocket::attach(*_pControlSocket, getHost(), _pContext));
			*_pControlSocket = sss;
		}
		catch (Poco::Exception&)
		{
			throw;
		}
	}
	else
	{
		_enableFTPS = false;
	}
}


StreamSocket FTPSClientSession::establishDataConnection(const std::string& command, const std::string& arg)
{
	beforeCreateDataSocket();

	StreamSocket ss = FTPClientSession::establishDataConnection(command, arg);
	ss.setNoDelay(true);

	// SSL nogotiating of data socket
	if (_secureDataConnection && _pControlSocket->secure())
	{
		// We need to reuse the control socket SSL session so the server can ensure that client that opened control socket is the same using data socket
		Poco::Net::SecureStreamSocketImpl* pSecure = dynamic_cast<Poco::Net::SecureStreamSocketImpl*>(_pControlSocket->impl());
		if (pSecure != nullptr)
		{
			Poco::Net::SecureStreamSocket sss(Poco::Net::SecureStreamSocket::attach(ss, getHost(), pSecure->context(), pSecure->currentSession()));
			ss = sss;
			if (_forceSessionReuse)
			{
				sss.setLazyHandshake(false);
				if (sss.completeHandshake() != 1)
					throw Poco::Net::FTPException("SSL Session HANDSHAKE error");
				if (!sss.sessionWasReused())
					throw Poco::Net::FTPException("SSL Session for data connection was not reused");
			}
		}
	}
	return ss;
}


void FTPSClientSession::receiveServerReadyReply()
{
	FTPClientSession::receiveServerReadyReply();
	afterCreateControlSocket();
}


} } // namespace Poco::Net
