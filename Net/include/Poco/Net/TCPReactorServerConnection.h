#ifndef Net_TCPReactorServerConnection_INCLUDED
#define Net_TCPReactorServerConnection_INCLUDED


#include "Poco/Buffer.h"
#include "Poco/Logger.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include <cstddef>
#include <unistd.h>

namespace Poco {
	namespace Net {
	
		class TCPReactorServerConnection : public std::enable_shared_from_this<TCPReactorServerConnection>
		{
		public:
		using TcpReactorConnectionPtr = std::shared_ptr<TCPReactorServerConnection>;
			using RecvMessageCallback =
		std::function<void(const TcpReactorConnectionPtr &)>;
			TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor):_reactor(reactor), _socket(socket)
			{
				_logger = &Poco::Logger::root();
				_reactor.addEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));
				
			}
			~TCPReactorServerConnection();
			
			void initialize();
			
			void onRead(const AutoPtr<ReadableNotification>& pNf) {
				size_t n = _socket.available();

				if (n==0) {
					handleClose();
				} else if (n<0) {
					
					handleClose();
				} else {
					_rcvCallback(shared_from_this());
				}
			}
			void onWrite(const AutoPtr<WritableNotification>& pNf) {}
			void onError(const AutoPtr<ErrorNotification>& pNf) {}
			void onShutdown(const AutoPtr<ShutdownNotification>& pNf) {}
			
			void handleClose() {
				_reactor.removeEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));

			}
			StreamSocket socket() {
				return _socket;
			}

			void setRecvMessageCallback(const RecvMessageCallback & cb) {
				_rcvCallback = cb;
			}
		private:
			Poco::Net::SocketReactor& _reactor;
			Poco::Net::StreamSocket _socket;
			RecvMessageCallback _rcvCallback;
		
			Logger* _logger;
		
		};
	}
}

#endif // Net_TCPReactorServerConnection_INCLUDED
