//
// HTTPServerAsyncConnection.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerAsyncConnection
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServerAsyncConnection.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Timestamp.h"
#include "Poco/Delegate.h"
#include <memory>


namespace Poco {
namespace Net {


//
// HTTPServiceHandler
//

HTTPServiceHandler::HTTPServiceHandler(StreamSocket& socket, SocketReactor& reactor) :
	_socket(socket), _reactor(reactor), _pSession(0), _stopped(false)
{
	_reactor.addEventHandler(_socket, Observer<HTTPServiceHandler, ReadableNotification>(*this, &HTTPServiceHandler::onReadable));
	_reactor.addEventHandler(_socket, Observer<HTTPServiceHandler, WritableNotification>(*this, &HTTPServiceHandler::onWritable));
}

HTTPServiceHandler::~HTTPServiceHandler()
{
	_reactor.removeEventHandler(_socket, Observer<HTTPServiceHandler, ReadableNotification>(*this, &HTTPServiceHandler::onReadable));
	_reactor.removeEventHandler(_socket, Observer<HTTPServiceHandler, WritableNotification>(*this, &HTTPServiceHandler::onWritable));
}

void HTTPServiceHandler::setSession(HTTPServerSession* pSession)
{
	_pSession = dynamic_cast<HTTPServerAsyncSession*>(pSession);
}

void HTTPServiceHandler::onReadable(ReadableNotification* pNf)
{
	pNf->release();
	if (_stopped) return;

	if (_pSession)
	{
		if (SocketAddress() == _pSession->clientAddress())
			_pSession->setClientAddress(_socket.peerAddress());
		if (SocketAddress() == _pSession->serverAddress())
			_pSession->setServerAddress(_socket.address());
		if (_socket.getBlocking()) _socket.setBlocking(false);
		char buffer[8] = { 0 };
		int n = _socket.receiveBytes(buffer, sizeof(buffer));
		while (n > 0)
		{
			_pSession->setReceive(buffer, n);
			n = _socket.receiveBytes(buffer, sizeof(buffer));
		}
	}
}

void HTTPServiceHandler::onWritable(WritableNotification* pNf)
{
	pNf->release();
	if (_stopped) return;

	if (_pSession)
	{
		if (SocketAddress() == _pSession->clientAddress())
			_pSession->setClientAddress(_socket.peerAddress());
		if (SocketAddress() == _pSession->serverAddress())
			_pSession->setServerAddress(_socket.address());
		if (_socket.getBlocking()) _socket.setBlocking(false);
		HTTPSession::IOBuffer buf;
		if (_pSession->getSend(buf)) _socket.sendBytes(buf.data(), buf.size());
	}
}


//
// HandlerWatcher
//

HandlerWatcher::HandlerWatcher(HTTPServiceHandler* pHandler) :
	_pHandler(pHandler)
{
}


HandlerWatcher::~HandlerWatcher()
{
	_pHandler->stop();
}


//
// HTTPServerAsyncConnection
//

HTTPServerAsyncConnection::HTTPServerAsyncConnection(ServerSocket& socket, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory):
	_pParams(pParams),
	_pFactory(pFactory),
	_stopped(true),
	_session(_pParams),
	_acceptor(socket, _reactor),
	_rc(1)
{
	poco_check_ptr (pFactory);
	_pFactory->serverStopped += Poco::delegate(this, &HTTPServerAsyncConnection::onServerStopped);
}


HTTPServerAsyncConnection::~HTTPServerAsyncConnection()
{
	try
	{
		_pFactory->serverStopped -= Poco::delegate(this, &HTTPServerAsyncConnection::onServerStopped);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void HTTPServerAsyncConnection::duplicate()
{
	++_rc;
}


void HTTPServerAsyncConnection::release()
{
	if (--_rc == 0)
	{
		_reactor.stop();
		_thread.join();
		delete this;
	}
}


void HTTPServerAsyncConnection::run()
{
	AutoPtr<HTTPServerAsyncConnection> guard(this, true); // ensure object stays alive

	_thread.start(_reactor);
	HTTPServiceHandler* pH = _acceptor.serviceHandler();
	while (!pH)
	{
		Thread::sleep(10);
		pH = _acceptor.serviceHandler();
	}
	pH->setSession(&_session);
	HandlerWatcher hw(pH);
	_stopped = false;

	std::string server = _pParams->getSoftwareVersion();
	while (!_stopped /*&& _session.hasMoreRequests() FIXME*/)
	{
		try
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if (!_stopped && (_session.hasInData() || _session.hasOutData()))
			{
				HTTPServerResponseImpl response(_session);
				HTTPServerRequestImpl request(response, _session, _pParams);
			
				Poco::Timestamp now;
				response.setDate(now);
				response.setVersion(request.getVersion());
				response.setKeepAlive(_pParams->getKeepAlive() && request.getKeepAlive() && _session.canKeepAlive());
				if (!server.empty()) response.set("Server", server);
				try
				{
					std::unique_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
					if (pHandler.get())
					{
						if (request.getExpectContinue() && response.getStatus() == HTTPResponse::HTTP_OK)
							response.sendContinue();
					
						pHandler->handleRequest(request, response);
						_session.setKeepAlive(_pParams->getKeepAlive() && response.getKeepAlive() && _session.canKeepAlive());
					}
					else sendErrorResponse(_session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
				}
				catch (Poco::Exception&)
				{
					if (!response.sent())
					{
						try
						{
							sendErrorResponse(_session, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
						}
						catch (...)
						{
						}
					}
					throw;
				}
			}
		}
		catch (NoMessageException&)
		{
			break;
		}
		catch (MessageException&)
		{
			sendErrorResponse(_session, HTTPResponse::HTTP_BAD_REQUEST);
		}
		catch (Poco::Exception&)
		{
			if (_session.networkException())
			{
				_session.networkException()->rethrow();
			}
			else throw;
		}
	}
}


void HTTPServerAsyncConnection::sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status)
{
	HTTPServerResponseImpl response(session);
	response.setVersion(HTTPMessage::HTTP_1_1);
	response.setStatusAndReason(status);
	response.setKeepAlive(false);
	response.send();
	session.setKeepAlive(false);
}


void HTTPServerAsyncConnection::onServerStopped(const bool& abortCurrent)
{
	_stopped = true;
}


} } // namespace Poco::Net
