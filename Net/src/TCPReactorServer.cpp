#include "Poco/Net/TCPReactorServer.h"


namespace Poco {
	namespace Net {

		TCPReactorServer::TCPReactorServer(){
		
		}
		
		TCPReactorServer::~TCPReactorServer(){
			stop();
		}
		void TCPReactorServer::start(){
			for (auto& reactor : _reactors) {
				reactor->start();
			}
		}
		void TCPReactorServer::stop(){
			for (auto& reactor : _reactors) {
				reactor->stop();
			}
			_threadPool.stopAll();
		}
}
}
