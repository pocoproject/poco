#ifndef Net_TCPReactorAcceptor_INCLUDED
#define Net_TCPReactorAcceptor_INCLUDED

#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/TCPReactorServerConnection.h"
#include "Poco/ThreadPool.h"
#include <atomic>
#include <memory>
#include <vector>
namespace Poco {
	namespace Net{

		class TCPReactorAcceptor: public Poco::Net::SocketAcceptor<TCPReactorServerConnection>
		{
		public:
			TCPReactorAcceptor(Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor):
				Poco::Net::SocketAcceptor<TCPReactorServerConnection>(socket, reactor),
				_threadPool(4)
			{
				for (int i=0;i<4; i++) {
					std::shared_ptr<SocketReactor> workerReactor(std::make_shared<SocketReactor>());
					_reactors.push_back(workerReactor);
					_threadPool.start(*workerReactor);
				}
			}
		
			~TCPReactorAcceptor()
			{
			}

			std::shared_ptr<SocketReactor> reactor() {
				static std::atomic_int index(0);
				return  _reactors[index++ % _reactors.size()];
			}
		private:
			TCPReactorServerConnection* createServiceHandler(Poco::Net::StreamSocket& socket) override
			{
				return new TCPReactorServerConnection(socket, *reactor());
			}
		private:
			TCPReactorAcceptor(const TCPReactorAcceptor&) = delete;
			TCPReactorAcceptor& operator = (const TCPReactorAcceptor&) = delete;
			std::vector<std::shared_ptr<SocketReactor> > _reactors;
			ThreadPool _threadPool;
		};
	}
}

#endif // Net_TCPReactorAcceptor_INCLUDED