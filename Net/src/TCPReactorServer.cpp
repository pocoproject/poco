#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"


namespace Poco {
	namespace Net {

		TCPReactorServer::TCPReactorServer(int port, TCPServerParams::Ptr pParams, int lisenerNum):	
		_threadPool("TCPR",lisenerNum),_reactors(lisenerNum),_port(port),_pParams(pParams)
		{
			for (auto& reactor: _reactors) {
				ServerSocket socket(_port);
				_socket.push_back(socket);
				auto acceptor = std::make_shared<TCPReactorAcceptor>(socket, reactor,_pParams, true);
				_acceptors.push_back(acceptor);
				
			}
		}
		
		TCPReactorServer::~TCPReactorServer(){
			stop();
		}

		void TCPReactorServer::start(bool acceptorUseSelfReactor){
			for (auto& reactor: _reactors) {
				_threadPool.start(reactor);
			}
		}
		void TCPReactorServer::setRecvMessageCallback(const RecvMessageCallback & cb){
			for (auto& acceptor: _acceptors) {
				acceptor->setRecvMessageCallback(cb);
			}
		}
		void TCPReactorServer::stop(){
			for (auto& reactor: _reactors) {
				reactor.stop();
			
			}
			_threadPool.joinAll();
		}
}
}
