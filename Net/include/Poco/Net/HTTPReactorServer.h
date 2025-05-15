#include "Poco/Logger.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/ThreadPool.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/ServerSocket.h"
#include <memory>
#include <vector>
namespace Poco {
	namespace Net {


class HTTPReactorServerConnection;

class HTTPReactorServer 
{
public:
	HTTPReactorServer(int port, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
	~HTTPReactorServer();
	void start();
	void stop();

private:
	std::vector<std::shared_ptr<SocketReactor>> _reactors;
	std::vector<ServerSocket> _serverSockets;
	std::vector<std::shared_ptr<SocketAcceptor<HTTPReactorServerConnection>>> _acceptors;
    

    ThreadPool _threadPool;
};

class HTTPReactorServerConnection : public std::enable_shared_from_this<HTTPReactorServerConnection>
{
public:
	HTTPReactorServerConnection(StreamSocket socket, SocketReactor& reactor);
	~HTTPReactorServerConnection();

    void initialize();

	void onRead(const AutoPtr<ReadableNotification>& pNf) ;
	void onWrite(const AutoPtr<WritableNotification>& pNf) {}
	void onError(const AutoPtr<ErrorNotification>& pNf) {}
	void onShutdown(const AutoPtr<ShutdownNotification>& pNf) {}

    void sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status) ;

    void setParams(Poco::Net::HTTPServerParams::Ptr pParams) {
        _pParams = pParams;
    }

    void setFactory(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory) {
        _pFactory = pFactory;
    }
private:
	Poco::Net::SocketReactor& _reactor;
	Poco::Net::StreamSocket _socket;
    Poco::Net::HTTPServerParams::Ptr _pParams;
    Poco::Net::HTTPRequestHandlerFactory::Ptr _pFactory;
    Logger* _logger;

};
}
};