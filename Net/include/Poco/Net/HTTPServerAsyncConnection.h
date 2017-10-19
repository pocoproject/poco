//
// HTTPServerAsyncConnection.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerAsyncConnection
//
// Definition of the HTTPServerAsyncConnection class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPServerAsyncConnection_INCLUDED
#define Net_HTTPServerAsyncConnection_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerAsyncSession.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace Net {


class HTTPServerSession;


class HTTPServiceHandler
{
public:
	HTTPServiceHandler(StreamSocket& socket, SocketReactor& reactor);
	~HTTPServiceHandler();

	void setSession(HTTPServerSession* pSession);
	void onReadable(ReadableNotification* pNf);
	void onWritable(WritableNotification* pNf);
	void stop();

private:
	StreamSocket            _socket;
	SocketReactor&          _reactor;
	HTTPServerAsyncSession* _pSession;
	std::atomic<bool>       _stopped;
};


inline void HTTPServiceHandler::stop()
{
	_stopped = true;
}


class HandlerWatcher
	/// Small RAII utility class for ServiceHandler
	/// destruction.
{
public:
	HandlerWatcher(HTTPServiceHandler* pHandler);
	~HandlerWatcher();

private:
	HandlerWatcher();
	HTTPServiceHandler* _pHandler;
};


class Net_API HTTPServerAsyncConnection: public Runnable
{
public:
	HTTPServerAsyncConnection(ServerSocket& socket, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
		/// Creates the HTTPServerAsyncConnection.

	virtual ~HTTPServerAsyncConnection();
		/// Destroys the HTTPServerAsyncConnection.

	void run();
		/// Handles all HTTP requests coming in.

	void stop();
	bool isRunning() const;

	void duplicate();
	void release();

private:
	void sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status);
	void onServerStopped(const bool& abortCurrent);

private:
	HTTPServerParams::Ptr              _pParams;
	HTTPRequestHandlerFactory::Ptr     _pFactory;
	bool                               _stopped;
	Poco::FastMutex                    _mutex;
	HTTPServerAsyncSession             _session;
	SocketReactor                      _reactor;
	Thread                             _thread;
	SocketAcceptor<HTTPServiceHandler> _acceptor;
	std::atomic<int>                   _rc;
};


//
// inlines
//

inline void HTTPServerAsyncConnection::stop()
{
	_stopped = true;
}


inline bool HTTPServerAsyncConnection::isRunning() const
{
	return !_stopped;
}


} } // namespace Poco::Net


#endif // Net_HTTPServerAsyncConnection_INCLUDED
