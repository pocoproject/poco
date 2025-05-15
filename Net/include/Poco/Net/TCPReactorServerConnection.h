#ifndef Net_TCPReactorServerConnection_INCLUDED
#define Net_TCPReactorServerConnection_INCLUDED


#include "Poco/Exception.h"
#include "Poco/Logger.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include <cstring>
#include <exception>
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
				_buf.reserve(4096);
				
			}
			~TCPReactorServerConnection(){}
			
			void initialize() {
				_logger->information("initialize");
				_reactor.addEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));
				_logger->information("initialize end " + std::to_string( weak_from_this().use_count()));

			}
			
			void onRead(const AutoPtr<ReadableNotification>& pNf) {
				_logger->information("onRead begin");
				char tmp[4096]={0};
				int n =0;
				try {
				
					n = _socket.receiveBytes(tmp, sizeof(tmp));
				}catch (Poco::Exception &e) {
					_logger->error("exp:"+e.displayText());
				}catch (std::exception &e) {
					_logger->error("exp:"+std::string(e.what()));
				
				} catch (...) {
					_logger->error("unknow exp");
				}
				_logger->information("onRead: " + std::to_string(n));
				
				
				if (n==0) {
					handleClose();
				} else if (n<0) {
					
					handleClose();
				} else {
					_logger->information("rcvmsg call back");
					_buf.assign(tmp, n);
					// char rs[100]="HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nHello, World!";
					// _socket.sendBytes(rs, strlen(rs));
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
				_logger->information("handleClose " + _socket.peerAddress().toString());
				_reactor.removeEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));

			}
			const StreamSocket& socket() {
				return _socket;
			}

			const std::string& buffer() {
				return _buf;
			}

			void setRecvMessageCallback(const RecvMessageCallback & cb) {
				_rcvCallback = cb;
			}
		private:
			Poco::Net::SocketReactor& _reactor;
			Poco::Net::StreamSocket _socket;
			RecvMessageCallback _rcvCallback;
			std::string _buf;
		
			Logger* _logger;
		
		};
	}
}

#endif // Net_TCPReactorServerConnection_INCLUDED
