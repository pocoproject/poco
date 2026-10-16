//
// HTTPServerConnection.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerConnection
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServerConnection.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Timestamp.h"
#include "Poco/Delegate.h"
#include <istream>
#include <memory>
#include <optional>


using namespace std::string_literals;


namespace Poco::Net {


namespace
{
	// A larger unread remainder closes the connection instead of being read.
	constexpr std::streamsize MAX_DRAIN_SIZE = 65536;

	// Bytes of the request body left unread would be parsed as the next request,
	// so the connection can only be kept alive once the body is read to its end.
	bool drainRequestBody(std::istream& body)
	{
		try
		{
			if (body.bad()) return false;
			// A handler that read the body on a non-blocking socket and then made it blocking again can leave
			// eofbit set from a would-block; ignore() would then read nothing and the eof() test would report
			// the body as complete.
			body.clear();
			body.ignore(MAX_DRAIN_SIZE + 1);
			return body.eof() && !body.bad();
		}
		catch (std::exception&)
		{
			return false;
		}
	}
}


HTTPServerConnection::HTTPServerConnection(const StreamSocket& socket, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory):
	TCPServerConnection(socket),
	_pParams(pParams),
	_pFactory(pFactory),
	_stopped(false)
{
	poco_check_ptr (pFactory);

	_pFactory->serverStopped += Poco::delegate(this, &HTTPServerConnection::onServerStopped);
}


HTTPServerConnection::~HTTPServerConnection()
{
	try
	{
		_pFactory->serverStopped -= Poco::delegate(this, &HTTPServerConnection::onServerStopped);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void HTTPServerConnection::run()
{
	std::string server = _pParams->getSoftwareVersion();
	HTTPServerSession session(socket(), _pParams);
	while (!_stopped && session.hasMoreRequests())
	{
		try
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if (!_stopped)
			{
				std::optional<HTTPServerResponseImpl> response(std::in_place, session);
				HTTPServerRequestImpl request(*response, session, _pParams);

				Poco::Timestamp now;
				response->setDate(now);
				response->setVersion(request.getVersion());
				response->setKeepAlive(_pParams->getKeepAlive() && request.getKeepAlive() && session.canKeepAlive());
				if (!server.empty())
					response->set("Server"s, server);
				try
				{
					session.requestTrailer().clear();
					session.responseTrailer().clear();
					std::unique_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
					if (pHandler.get())
					{
						if (request.getExpectContinue() && response->getStatus() == HTTPResponse::HTTP_OK)
							response->sendContinue();

						pHandler->handleRequest(request, *response);
						bool keepAlive = _pParams->getKeepAlive() && response->getKeepAlive() && session.canKeepAlive();
						// Destroying the response sends its buffered rest before the drain waits for the body.
						response.reset();
						// On a socket that the handler made non-blocking, a would-block also ends the body stream,
						// so the drain cannot tell whether the body was read to its end.
						if (keepAlive) keepAlive = session.socket().getBlocking() && drainRequestBody(request.stream());
						session.setKeepAlive(keepAlive && session.networkException() == nullptr);
					}
					else sendErrorResponse(session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
				}
				catch (Poco::Exception&)
				{
					if (response.has_value() && !response->sent())
					{
						try
						{
							sendErrorResponse(session, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
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
			sendErrorResponse(session, HTTPResponse::HTTP_BAD_REQUEST);
		}
		catch (Poco::Exception&)
		{
			if (session.networkException())
			{
				session.networkException()->rethrow();
			}
			else throw;
		}
	}
}


void HTTPServerConnection::sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status)
{
	HTTPServerResponseImpl response(session);
	response.setVersion(HTTPMessage::HTTP_1_1);
	response.setStatusAndReason(status);
	response.setKeepAlive(false);
	response.send();
	session.setKeepAlive(false);
}


void HTTPServerConnection::onServerStopped(const bool& abortCurrent)
{
	_stopped = true;
	if (abortCurrent)
	{
		try
		{
			// Note: On Windows, select() will not return if one of its socket is being
			// shut down. Therefore we have to call close(), which works better.
			// On other platforms, we do the more graceful thing.
#if defined(_WIN32)
			socket().close();
#else
			socket().shutdown();
#endif
		}
		catch (...)
		{
		}
	}
	else
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		try
		{
#if defined(_WIN32)
			socket().close();
#else
			socket().shutdown();
#endif
		}
		catch (...)
		{
		}
	}
}


} // namespace Poco::Net
