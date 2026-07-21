#ifndef Net_HTTPReactorServer_INCLUDED
#define Net_HTTPReactorServer_INCLUDED

#include "Poco/Logger.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/TCPReactorServer.h"
namespace Poco::Net {


class Net_API HTTPReactorServer
{
public:
	HTTPReactorServer(int port, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
		/// Listens on the wildcard address on the given port.

	HTTPReactorServer(const SocketAddress& address, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
		/// Listens on a specific (host, port) so deployments can bind to a
		/// single interface (e.g. SocketAddress("127.0.0.1", 9800) for a
		/// localhost-only service).

	~HTTPReactorServer();
	void start();
	void stop();
	int port() const { return _tcpReactorServer.port(); }
	void onMessage(const TcpReactorConnectionPtr& conn);
	void onError(const Poco::Exception& ex);
		/// Rethrows ex preserving its dynamic type, so callers up the stack can
		/// classify the failure. Handlers run inline on the reactor worker
		/// thread, so this propagates into TCPReactorServerConnection::onRead.

	void sendErrorResponse(HTTPSession& session, HTTPResponse::HTTPStatus status);

private:
	TCPReactorServer               _tcpReactorServer;
	HTTPServerParams::Ptr          _pParams;
	HTTPRequestHandlerFactory::Ptr _pFactory;
};

} // namespace Poco::Net

#endif // Net_HTTPReactorServer_INCLUDED
