#ifndef Net_TCPReactorServerConnection_INCLUDED
#define Net_TCPReactorServerConnection_INCLUDED


#include "Poco/Logger.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include <cstring>
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
			TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor);
	
			~TCPReactorServerConnection();
			
			void initialize();
			
			void onRead(const AutoPtr<ReadableNotification>& pNf);
			void onWrite(const AutoPtr<WritableNotification>& pNf);
			void onError(const AutoPtr<ErrorNotification>& pNf);
			void onShutdown(const AutoPtr<ShutdownNotification>& pNf);
			
			void handleClose();
			const StreamSocket& socket();
			std::string& buffer();

			void setRecvMessageCallback(const RecvMessageCallback & cb);
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
