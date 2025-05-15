#ifndef Net_TCPReactorServer_INCLUDED
#define Net_TCPReactorServer_INCLUDED

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/TCPReactorAcceptor.h"
#include "Poco/Runnable.h"
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
	TCPReactorServer(int port);
	
		/// Creates the TCPReactorServer using the given
		/// stream socket.
	~TCPReactorServer();

	void start();
	void stop();
	
	void setRecvMessageCallback(const RecvMessageCallback & cb) {
		_acceptor->setRecvMessageCallback(cb);
	}
private:


	Thread _thread;
	SocketReactor _reactor;
	std::shared_ptr<TCPReactorAcceptor> _acceptor;
	ServerSocket _socket;
};
} // namespace Net
} // namespace Poco

#endif // Net_TCPReactorServer_INCLUDED