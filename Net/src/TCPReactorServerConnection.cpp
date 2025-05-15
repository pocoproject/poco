#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/TCPReactorServerConnection.h"

namespace Poco {
	namespace Net {


			TCPReactorServerConnection::TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor):_reactor(reactor), _socket(socket)
			{
				_logger = &Poco::Logger::root();
				_buf.reserve(4096);
				
			}
			TCPReactorServerConnection::~TCPReactorServerConnection(){}
			
			void TCPReactorServerConnection::initialize() {
				_logger->information("initialize");
				_reactor.addEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));
				_logger->information("initialize end " + std::to_string( weak_from_this().use_count()));

			}
			
			void TCPReactorServerConnection::onRead(const AutoPtr<ReadableNotification>& pNf) {
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
			void TCPReactorServerConnection::onWrite(const AutoPtr<WritableNotification>& pNf) {}
			void TCPReactorServerConnection::onError(const AutoPtr<ErrorNotification>& pNf) {
				_logger->error("onError: " + std::to_string(pNf->code()) + " " + pNf->description());
				handleClose();
			}
			void TCPReactorServerConnection::onShutdown(const AutoPtr<ShutdownNotification>& pNf) {}
			
			void TCPReactorServerConnection::handleClose() {
				_logger->information("handleClose " + _socket.peerAddress().toString());
				_reactor.removeEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));

			}
			const StreamSocket& TCPReactorServerConnection::socket() {
				return _socket;
			}

			const std::string& TCPReactorServerConnection::buffer() {
				return _buf;
			}

			void TCPReactorServerConnection::setRecvMessageCallback(const RecvMessageCallback & cb) {
				_rcvCallback = cb;
			}

		
		
	}
}


