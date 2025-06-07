#include "Poco/Logger.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/TCPReactorServer.h"
#include "Poco/ThreadPool.h"
namespace Poco {
namespace Net {


class HTTPReactorServer
{
public:
	HTTPReactorServer(int port, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
	~HTTPReactorServer();
	void start();
	void stop();
	void onMessage(const TcpReactorConnectionPtr& conn);
	void onError(const Poco::Exception& ex);
	void sendErrorResponse(HTTPSession& session, HTTPResponse::HTTPStatus status);

private:
	TCPReactorServer               _tcpReactorServer;
	HTTPServerParams::Ptr          _pParams;
	HTTPRequestHandlerFactory::Ptr _pFactory;

	ThreadPool _threadPool;
};

}} // namespace Poco::Net

