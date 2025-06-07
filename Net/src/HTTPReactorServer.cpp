#include "Poco/Net/HTTPReactorServer.h"
#include "Poco/Net/HTTPReactorServerSession.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPSession.h"
#include <cstring>

namespace Poco { namespace Net {

HTTPReactorServer::HTTPReactorServer(int port, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory)
	: _tcpReactorServer(port, pParams)
{
	_pParams = pParams;
	_pFactory = pFactory;
	_tcpReactorServer.setRecvMessageCallback([this](const TcpReactorConnectionPtr& conn) {
		// Handle incoming message
		this->onMessage(conn);
	});
}

HTTPReactorServer::~HTTPReactorServer()
{
}

void HTTPReactorServer::start()
{
	_tcpReactorServer.start();
}

void HTTPReactorServer::stop()
{
	_tcpReactorServer.stop();
}

void HTTPReactorServer::onMessage(const TcpReactorConnectionPtr& conn)
{
	try
	{
		// Handle read event
		HTTPReactorServerSession session(conn->socket(), conn->buffer(), _pParams);
		if (!session.checkRequestComplete())
		{
			return;
		}
		// Create request and response objects
		HTTPServerResponseImpl response(session);
		HTTPServerRequestImpl  request(response, session, _pParams);
		// Process request and generate response
		Poco::Timestamp now;
		response.setDate(now);
		response.setVersion(request.getVersion());
		response.setKeepAlive(request.getKeepAlive());
		std::string server = _pParams->getSoftwareVersion();
		if (!server.empty())
		{
			response.set("Server", server);
		}
		try
		{
			session.requestTrailer().clear();
			session.responseTrailer().clear();
			std::unique_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
			if (pHandler.get())
			{
				if (request.getExpectContinue() && response.getStatus() == HTTPResponse::HTTP_OK)
					response.sendContinue();

				pHandler->handleRequest(request, response);
				session.setKeepAlive(_pParams->getKeepAlive() && response.getKeepAlive());
			}
			else
				sendErrorResponse(session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
		}
		catch (Poco::Exception& e)
		{
			if (!response.sent())
			{
				try
				{
					sendErrorResponse(
						session,
						e.code() == 0 ? HTTPResponse::HTTP_INTERNAL_SERVER_ERROR : HTTPResponse::HTTPStatus(e.code()));
				}
				catch (...)
				{
				}
			}
			throw;
		}
	}
	catch (const Poco::Exception& ex)
	{
		onError(ex);
	}
}

void HTTPReactorServer::sendErrorResponse(HTTPSession& session, HTTPResponse::HTTPStatus status)
{
	HTTPServerResponseImpl response(session);
	response.setVersion(HTTPMessage::HTTP_1_1);
	response.setStatusAndReason(status);
	response.setKeepAlive(false);

	session.setKeepAlive(false);
}

void HTTPReactorServer::onError(const Poco::Exception& ex)
{
	// Handle error
	throw ex;
}

}} // namespace Poco::Net

