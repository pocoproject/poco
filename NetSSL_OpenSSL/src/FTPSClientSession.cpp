//
// FTPSClientSession.cpp
//
// Library: Net
// Package: FTP
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

FTPSClientSession::FTPSClientSession() :
	FTPClientSession()
{
}


FTPSClientSession::~FTPSClientSession()
{
}

FTPSClientSession::FTPSClientSession(const StreamSocket& socket) :
	FTPClientSession(socket)
{
}


FTPSClientSession::FTPSClientSession(const std::string& host,
										Poco::UInt16 port,
										const std::string& username,
										const std::string& password) :
	FTPClientSession(host, port, username, password)
{
}

void FTPSClientSession::tryFTPSmode(bool bTryFTPS)
{
	_bTryFTPS = bTryFTPS;
}

void FTPSClientSession::beforeCreateDataSocket()
{
	if (_bSecureDataConnection)
		return;
	_bSecureDataConnection = false;
	if (!_pControlSocket->secure())
		return;
	std::string sResponse;
	int status = sendCommand("PBSZ 0", sResponse);
	if (isPositiveCompletion(status))
	{
		status = sendCommand("PROT P", sResponse);
		if (isPositiveCompletion(status))
			_bSecureDataConnection = true;
	}
}

void FTPSClientSession::afterCreateControlSocket()
{
	if (!_bTryFTPS)
		return;
	_pControlSocket->setNoDelay(true);
	if (_pControlSocket->secure())
		return;

	std::string sResponse;
	int status = sendCommand("AUTH TLS", sResponse);
	if (!isPositiveCompletion(status))
		status = sendCommand("AUTH SSL", sResponse);

	if (isPositiveCompletion(status))
	{
		//Server support FTPS
		try
		{
			Poco::Net::SecureStreamSocket sss(Poco::Net::SecureStreamSocket::attach(*_pControlSocket, Poco::Net::SSLManager::instance().defaultClientContext()));
			*_pControlSocket = sss;
		}
		catch (Poco::Exception&)
		{
			throw;
		}
	}
	else
	{
		_bTryFTPS = false;
	}
}

StreamSocket FTPSClientSession::establishDataConnection(const std::string& command, const std::string& arg)
{
	beforeCreateDataSocket();

	StreamSocket ss = FTPClientSession::establishDataConnection(command, arg);
	ss.setNoDelay(true);

	//SSL nogotiating of data socket
	if ((_bSecureDataConnection) && (_pControlSocket->secure()))
	{
		//We need to reuse the control socket SSL session so the server can ensure that client that opened control socket is the same using data socket
		Poco::Net::SecureStreamSocketImpl* pSecure = dynamic_cast<Poco::Net::SecureStreamSocketImpl*>(_pControlSocket->impl());
		if (pSecure != nullptr)
		{
			Poco::Net::SecureStreamSocket sss(Poco::Net::SecureStreamSocket::attach(ss, pSecure->context(), pSecure->currentSession()));
			ss = sss;
		}
	}
	return ss;
}

void FTPSClientSession::receiveServerReadyReply()
{
	FTPClientSession::receiveServerReadyReply();
	afterCreateControlSocket();
}

}} // namespace Poco::Net