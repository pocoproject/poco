#include "Poco/Net/TCPReactorAcceptor.h"
#include <atomic>
#include <string>


namespace Poco {
	namespace Net{

			TCPReactorAcceptor::TCPReactorAcceptor(Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor, TCPServerParams::Ptr pParams):
				Poco::Net::SocketAcceptor<TCPReactorServerConnection>(socket, reactor),
				_pParams(pParams),_selfReactor(reactor),_useSelfReactor(pParams->getUseSelfReactor())
			{
				int workerThreads = _useSelfReactor? 0 : _pParams->getMaxThreads();
				if (workerThreads > 0) {
					_threadPool = std::make_shared<Poco::ThreadPool>("TCPRA", workerThreads, workerThreads);
				} 
				for (int i=0; i<workerThreads; i++) {
					std::shared_ptr<SocketReactor> workerReactor(std::make_shared<SocketReactor>());
					_wokerReactors.push_back(workerReactor);
					_threadPool->start(*workerReactor);
				}
			}
		
			TCPReactorAcceptor::~TCPReactorAcceptor()
			{
			}

			SocketReactor& TCPReactorAcceptor::reactor() {
				if (_useSelfReactor) {
					return _selfReactor;
				}
				static std::atomic_uint index(0);
				return  *_wokerReactors[index++ % _wokerReactors.size()];
			}

			TCPReactorServerConnection* TCPReactorAcceptor::createServiceHandler(Poco::Net::StreamSocket& socket) 
			{
				// enable nodelay per default: OSX really needs that
#if defined(POCO_HAS_UNIX_SOCKET)
				if (socket.address().family() != AddressFamily::UNIX_LOCAL)
#endif
				{
					socket.setNoDelay(true);
				}
				auto tmpConnPtr =  std::make_shared<TCPReactorServerConnection>(socket, reactor());
				tmpConnPtr->setRecvMessageCallback(_recvMessageCallback);
				tmpConnPtr->initialize();
				return tmpConnPtr.get();
			}

	
	}
}
