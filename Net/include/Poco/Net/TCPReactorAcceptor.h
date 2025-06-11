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

namespace Poco {
namespace Net {


class Net_API TCPReactorAcceptor : public Poco::Net::SocketAcceptor<TCPReactorServerConnection>
{
public:
	TCPReactorAcceptor(
		Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor, TCPServerParams::Ptr pParams);

	~TCPReactorAcceptor();
	SocketReactor& reactor();

	void setRecvMessageCallback(const RecvMessageCallback& cb)
	{
		_recvMessageCallback = cb;
	}

private:
	TCPReactorServerConnection* createServiceHandler(Poco::Net::StreamSocket& socket) override;

private:
	TCPReactorAcceptor(const TCPReactorAcceptor&) = delete;
	TCPReactorAcceptor&                         operator=(const TCPReactorAcceptor&) = delete;
	std::vector<std::shared_ptr<SocketReactor>> _wokerReactors;
	SocketReactor&                              _selfReactor;
	bool                                        _useSelfReactor;
	std::shared_ptr<ThreadPool>                 _threadPool;
	RecvMessageCallback                         _recvMessageCallback;
	TCPServerParams::Ptr                        _pParams;
};

}} // namespace Poco::Net

#endif // Net_TCPReactorAcceptor_INCLUDED

