#ifndef Net_TCPReactorServerConnection_INCLUDED
#define Net_TCPReactorServerConnection_INCLUDED


#include "Poco/Buffer.h"
#include "Poco/Logger.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include <cstddef>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

namespace Poco {
	namespace Net {
		class TCPReactorServerConnection;
		using TcpReactorConnectionPtr = std::shared_ptr<TCPReactorServerConnection>;
		using RecvMessageCallback =
		std::function<void(const TcpReactorConnectionPtr &)>;
		class TCPReactorServerConnection : public std::enable_shared_from_this<TCPReactorServerConnection>
		{
		public:
		
			
			TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor):_reactor(reactor), _socket(socket)
			{
				_logger = &Poco::Logger::root();
				
			}
			~TCPReactorServerConnection(){}
			
			void initialize() {
				_logger->information("initialize");
				_reactor.addEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));
				_logger->information("initialize end " + std::to_string( weak_from_this().use_count()));

			}
			
			void onRead(const AutoPtr<ReadableNotification>& pNf) {
				size_t n = _socket.available();
				_logger->information("onRead: " + std::to_string(n));

				if (n==0) {
					handleClose();
				} else if (n<0) {
					
					handleClose();
				} else {
					_rcvCallback(shared_from_this());
				}
			}
			void onWrite(const AutoPtr<WritableNotification>& pNf) {}
			void onError(const AutoPtr<ErrorNotification>& pNf) {
				_logger->error("onError: " + std::to_string(pNf->code()) + " " + pNf->description());
				handleClose();
			}
			void onShutdown(const AutoPtr<ShutdownNotification>& pNf) {}
			
			void handleClose() {
				_logger->information("handleClose");
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
