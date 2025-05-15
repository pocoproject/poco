#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Thread.h"


namespace Poco {
	namespace Net {

		TCPReactorServer::TCPReactorServer(int port):	_thread("TCPReactorServer")
		{
			_socket.bind(port);
			_socket.listen();
			_acceptor = std::make_shared<TCPReactorAcceptor>(_socket, _reactor);
		};
		TCPReactorServer::~TCPReactorServer(){
			stop();
		}
		void TCPReactorServer::start(){
			_thread.start(_reactor);
		}
		void TCPReactorServer::stop(){
			_reactor.stop();
			_thread.join();
		}
}
}
