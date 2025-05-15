#include "Poco/Net/HTTPReactorServer.h"
#include "Poco/NObserver.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPSocketAcceptor.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPObserver.h"

namespace Poco {
	namespace Net {


class HTTPReactorServerConnection;


HTTPReactorServer::HTTPReactorServer(int port, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory):_tcpReactorServer(port){
        _pParams = pParams;
		_pFactory = pFactory;
		_logger = &Poco::Logger::root();
		_tcpReactorServer.setRecvMessageCallback([this](const TcpReactorConnectionPtr & conn) {
			// Handle incoming message
			this->onMessage(conn);
		});
    }
	HTTPReactorServer::~HTTPReactorServer(){

    }
	
	void HTTPReactorServer::start() {
		_tcpReactorServer.start();
	}
	void HTTPReactorServer::stop() {
		_tcpReactorServer.stop();
	}

	void HTTPReactorServer::onMessage(const TcpReactorConnectionPtr & conn) {
		_logger->information("onMessage: " + std::to_string(conn->socket().available()));
		// Handle incoming message
		try {
			// Handle read event
			HTTPServerSession session(conn->socket(), _pParams);
			// Create request and response objects
			HTTPServerResponseImpl response(session);
			HTTPServerRequestImpl request(response, session, _pParams);
			// Process request and generate response
            Poco::Timestamp now;
			response.setDate(now);
			response.setVersion(request.getVersion());
			response.setKeepAlive( request.getKeepAlive() && session.canKeepAlive());
            try
				{


					
#ifndef POCO_ENABLE_CPP11
					std::auto_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
#else
					std::unique_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
#endif
					if (pHandler.get())
					{
						if (request.getExpectContinue() && response.getStatus() == HTTPResponse::HTTP_OK)
							response.sendContinue();
					
						pHandler->handleRequest(request, response);
						session.setKeepAlive(_pParams->getKeepAlive() && response.getKeepAlive() && session.canKeepAlive());
					}
					else sendErrorResponse(session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
				}
				catch (Poco::Exception& e)
				{
					_logger->error(e.displayText());
					if (!response.sent())
					{
						try
						{
							sendErrorResponse(session, e.code()==0 ? HTTPResponse::HTTP_INTERNAL_SERVER_ERROR : HTTPResponse::HTTPStatus(e.code()));
						}
						catch (...)
						{
						}
					}
					throw;
				}
		}
		catch (const Poco::Exception& ex) {
			onError(ex);
		}
	}

	void HTTPReactorServer::sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status) {
		HTTPServerResponseImpl response(session);
		response.setVersion(HTTPMessage::HTTP_1_1);
		response.setStatusAndReason(status);
		response.setKeepAlive(false);

		session.setKeepAlive(false);
	}
	void HTTPReactorServer::onError(const Poco::Exception& ex) {
		// Handle error
		_logger->error(ex.displayText());
	}



HTTPReactorServerConnection::HTTPReactorServerConnection(StreamSocket socket, SocketReactor& reactor)
       :_reactor(reactor), _socket(socket){
		_logger = &Poco::Logger::root();
        _reactor.addEventHandler(_socket, HTTPObserver<HTTPReactorServerConnection, Poco::Net::ReadableNotification>(shared_from_this(), &HTTPReactorServerConnection::onRead));
		_reactor.addEventHandler(_socket, HTTPObserver<HTTPReactorServerConnection, Poco::Net::WritableNotification>(shared_from_this(), &HTTPReactorServerConnection::onWrite));
	}
	HTTPReactorServerConnection::~HTTPReactorServerConnection(){};

    void HTTPReactorServerConnection::initialize() {
        // Initialization code if needed
    }

	void HTTPReactorServerConnection::onRead(const AutoPtr<ReadableNotification>& pNf) {
		try {
			// Handle read event
			HTTPServerSession session(_socket, _pParams);
			// Create request and response objects
			HTTPServerResponseImpl response(session);
			HTTPServerRequestImpl request(response, session, _pParams);
			// Process request and generate response
            Poco::Timestamp now;
			response.setDate(now);
			response.setVersion(request.getVersion());
			response.setKeepAlive( request.getKeepAlive() && session.canKeepAlive());
            try
				{


					
#ifndef POCO_ENABLE_CPP11
					std::auto_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
#else
					std::unique_ptr<HTTPRequestHandler> pHandler(_pFactory->createRequestHandler(request));
#endif
					if (pHandler.get())
					{
						if (request.getExpectContinue() && response.getStatus() == HTTPResponse::HTTP_OK)
							response.sendContinue();
					
						pHandler->handleRequest(request, response);
						session.setKeepAlive(_pParams->getKeepAlive() && response.getKeepAlive() && session.canKeepAlive());
					}
					else sendErrorResponse(session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
				}
				catch (Poco::Exception& e)
				{
					_logger->error(e.displayText());
					if (!response.sent())
					{
						try
						{
							sendErrorResponse(session, e.code()==0 ? HTTPResponse::HTTP_INTERNAL_SERVER_ERROR : HTTPResponse::HTTPStatus(e.code()));
						}
						catch (...)
						{
						}
					}
					throw;
				}
		}
		catch (const Poco::Exception& ex) {
			onError(new ErrorNotification(&_reactor));
		}
	}


    void HTTPReactorServerConnection::sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status) {
        HTTPServerResponseImpl response(session);
        response.setVersion(HTTPMessage::HTTP_1_1);
        response.setStatusAndReason(status);
        response.setKeepAlive(false);

        session.setKeepAlive(false);
    }



}
};