#ifndef Net_TCPReactorServer_INCLUDED
#define Net_TCPReactorServer_INCLUDED

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPReactorAcceptor.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"
#include <vector>

namespace Poco {
namespace Net {


	class TCPReactorServer
{
	/// This class implements a TCP server using the Reactor pattern.
	/// It uses a SocketReactor to handle incoming connections and
	/// dispatches them to TCPReactorServerConnection objects.
	///
	/// The TCPReactorServer is designed to be used with a SocketReactor
	/// and a TCPReactorAcceptor. The SocketReactor handles the event
	/// loop, while the TCPReactorAcceptor accepts incoming connections
	/// and creates TCPReactorServerConnection objects to handle them.
public:
	TCPReactorServer(int port, TCPServerParams::Ptr pParams);

	~TCPReactorServer();

	void start(bool acceptorUseSelfReactor = false);
	/// Starts the TCPReactorServer.
	/// The server will listen for incoming connections
	/// on the given port.
	/// The acceptorUseSelfReactor parameter specifies
	/// whether the acceptor should use its own reactor
	void stop();

	void setRecvMessageCallback(const RecvMessageCallback& cb);

private:
	ThreadPool                                       _threadPool;
	std::vector<SocketReactor>                       _reactors;
	std::vector<std::shared_ptr<TCPReactorAcceptor>> _acceptors;
	std::vector<ServerSocket>                        _sockets;
	TCPServerParams::Ptr                             _pParams;
	int                                              _port;
};

}} // namespace Poco::Net

#endif // Net_TCPReactorServer_INCLUDED

