#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/TCPReactorServerConnection.h"

namespace Poco {
	namespace Net {


			TCPReactorServerConnection::TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor):_reactor(reactor), _socket(socket)
			{
				_buf.reserve(BUFFER_SIZE);
				
			}
			TCPReactorServerConnection::~TCPReactorServerConnection(){}
			
			void TCPReactorServerConnection::initialize() {
				_reactor.addEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));
			}
			
			void TCPReactorServerConnection::onRead(const AutoPtr<ReadableNotification>& pNf) {
				char tmp[BUFFER_SIZE]={0};
				int n = _socket.receiveBytes(tmp, sizeof(tmp));
				if (n==0) {
					handleClose();
				} else if (n<0) {
					// TODO
					handleClose();
				} else {
					_buf.append(tmp, n);
					_rcvCallback(shared_from_this());
				}
			}

			void TCPReactorServerConnection::onError(const AutoPtr<ErrorNotification>& pNf) {
				handleClose();
			}

			void TCPReactorServerConnection::onShutdown(const AutoPtr<ShutdownNotification>& pNf) {
				handleClose();
			}
			
			void TCPReactorServerConnection::handleClose() {
				// here must keep _socket to delay the _socket destrcutor
				StreamSocket keepSocket = _socket;
				// here will delete this, so memberships' destructor will be invoked
				_reactor.removeEventHandler(_socket, HTTPObserver<TCPReactorServerConnection, ReadableNotification>(shared_from_this(), &TCPReactorServerConnection::onRead));

			}
			const StreamSocket& TCPReactorServerConnection::socket() {
				return _socket;
			}

			std::string& TCPReactorServerConnection::buffer() {
				return _buf;
			}

			void TCPReactorServerConnection::setRecvMessageCallback(const RecvMessageCallback & cb) {
				_rcvCallback = cb;
			}

		
		
	}
}


