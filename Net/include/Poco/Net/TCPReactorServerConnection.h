#ifndef Net_TCPReactorServerConnection_INCLUDED
#define Net_TCPReactorServerConnection_INCLUDED

#include "Poco/Net/Net.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include <atomic>
#include <string>
#include <functional>

namespace Poco::Net {


class TCPReactorServerConnection;
using TcpReactorConnectionPtr = std::shared_ptr<TCPReactorServerConnection>;
using RecvMessageCallback = std::function<void(const TcpReactorConnectionPtr&)>;
using CloseCallback = std::function<void(const TcpReactorConnectionPtr&)>;

class Net_API TCPReactorServerConnection : public std::enable_shared_from_this<TCPReactorServerConnection>
{
public:
	TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor);

	~TCPReactorServerConnection();

	void initialize();

	void onRead(const AutoPtr<ReadableNotification>& pNf);
		/// Reads from the socket and passes what was read to the receive
		/// callback.
		///
		/// A blocking socket is read once, and again for what a TLS socket
		/// holds. A non-blocking socket is read as long as there is
		/// something to read, up to 16 reads of 4 KB for one event; what a
		/// TLS socket holds is read whatever the count.

	void onError(const AutoPtr<ErrorNotification>& pNf);
	void onShutdown(const AutoPtr<ShutdownNotification>& pNf);

	void handleClose();
		/// Closes the connection: calls the close callback and removes the
		/// connection from the reactor. Further calls do nothing.

	[[nodiscard]] const StreamSocket& socket();
	[[nodiscard]] std::string& buffer();

	void setRecvMessageCallback(const RecvMessageCallback& cb);

	void setCloseCallback(const CloseCallback& cb);
		/// Sets the callback that is called once when the connection closes,
		/// whether the peer closed it, an error did, handleClose() was
		/// called or the reactor is stopping. buffer() still holds what was
		/// received and not taken out of it.

	void setMaxPendingRequestSize(std::size_t size);
		/// Sets the maximum number of bytes buffered while the request is
		/// still incomplete. 0 disables the limit.

private:
	enum ReadResult
	{
		READ_DATA,    /// data was read and passed to the receive callback
		READ_NOTHING, /// nothing to read at the moment
		READ_CLOSED   /// the connection is closed
	};

	ReadResult readSome();
		/// Reads from the socket once.

	[[nodiscard]] bool socketHoldsData() const;
		/// Returns true if the socket holds data that it has already taken
		/// from the network, which is therefore not signalled as readable.

	Poco::Net::SocketReactor& _reactor;
	Poco::Net::StreamSocket   _socket;
	RecvMessageCallback       _rcvCallback;
	CloseCallback             _closeCallback;
	std::string               _buf;
	std::size_t               _maxPendingRequestSize {0};
	std::atomic<bool>         _closed {false};
};

} // namespace Poco::Net

#endif // Net_TCPReactorServerConnection_INCLUDED

