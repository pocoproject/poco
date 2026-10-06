#ifndef Net_TCPReactorAcceptor_INCLUDED
#define Net_TCPReactorAcceptor_INCLUDED

#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/TCPReactorServerConnection.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"
#include <memory>
#include <vector>

namespace Poco::Net {


class Net_API TCPReactorAcceptor : public Poco::Net::SocketAcceptor<TCPReactorServerConnection>
{
public:
	TCPReactorAcceptor(
		Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor, TCPServerParams::Ptr pParams);

	~TCPReactorAcceptor();
	
	[[nodiscard]] SocketReactor& reactor();
	void stop();

	void onAccept(const AutoPtr<ReadableNotification>& pNf) override;

	void onTimeout(const AutoPtr<TimeoutNotification>& pNf);
		/// Calls the timeout callback. The reactor dispatches the
		/// TimeoutNotification when a poll has found nothing to do.

	void setRecvMessageCallback(const RecvMessageCallback& cb)
	{
		_recvMessageCallback = cb;
	}

	void setCloseCallback(const CloseCallback& cb)
		/// Sets the callback that the connections accepted from now on
		/// call when they close.
	{
		_closeCallback = cb;
	}

	void setAcceptCallback(const AcceptCallback& cb)
		/// Sets the callback that is asked whether a connection that has
		/// just been accepted is taken. A connection that it declines is
		/// closed at once and never served.
	{
		_acceptCallback = cb;
	}

	void setTimeoutCallback(const TimeoutCallback& cb);
		/// Sets the callback that is called, on the thread of the reactor
		/// of the server socket, whenever a poll of that reactor has found
		/// nothing to do: every poll timeout while there is no traffic.
		/// Must be called before the reactor runs.

private:
	TCPReactorServerConnection* createServiceHandler(Poco::Net::StreamSocket& socket) override;

private:
	TCPReactorAcceptor(const TCPReactorAcceptor&) = delete;
	TCPReactorAcceptor&                         operator=(const TCPReactorAcceptor&) = delete;
	std::vector<std::shared_ptr<SocketReactor>> _workerReactors;
	SocketReactor&                              _selfReactor;
	bool                                        _useSelfReactor;
	std::shared_ptr<ThreadPool>                 _threadPool;
	RecvMessageCallback                         _recvMessageCallback;
	CloseCallback                               _closeCallback;
	AcceptCallback                              _acceptCallback;
	TimeoutCallback                             _timeoutCallback;
	TCPServerParams::Ptr                        _pParams;
	ServerSocket                                _serverSocket;
	std::atomic<bool>                           _stopped{false};
};

} // namespace Poco::Net

#endif // Net_TCPReactorAcceptor_INCLUDED

