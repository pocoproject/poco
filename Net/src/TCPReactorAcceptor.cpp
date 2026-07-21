#include "Poco/Net/TCPReactorAcceptor.h"
#include <atomic>


namespace Poco::Net {


TCPReactorAcceptor::TCPReactorAcceptor(Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor,
									   TCPServerParams::Ptr pParams)
	: Poco::Net::SocketAcceptor<TCPReactorServerConnection>(socket, reactor),
	  _selfReactor(reactor),
	  _useSelfReactor(pParams->getUseSelfReactor()),
	  _pParams(pParams)
{
	int workerThreads = _useSelfReactor ? 0 : _pParams->getMaxThreads();
	if (workerThreads > 0)
	{
		_threadPool = std::make_shared<Poco::ThreadPool>("TCPRW", workerThreads, workerThreads);
		for (int i = 0; i < workerThreads; i++)
		{
			std::shared_ptr<SocketReactor> workerReactor(std::make_shared<SocketReactor>());
			_workerReactors.push_back(workerReactor);
			_threadPool->start(*workerReactor);
		}
	}
	else
	{
		_useSelfReactor = true;
	}
}

TCPReactorAcceptor::~TCPReactorAcceptor()
{
	stop();
}

void TCPReactorAcceptor::stop()
{
	if (_stopped.exchange(true))
	{
		return;
	}
	for (auto& worker : _workerReactors)
	{
		worker->stop();
	}
	if (_threadPool)
	{
		_threadPool->joinAll();
	}
}

SocketReactor& TCPReactorAcceptor::reactor()
{
	if (_useSelfReactor)
	{
		return _selfReactor;
	}
	static std::atomic_uint index(0);
	return *_workerReactors[index++ % _workerReactors.size()];
}

TCPReactorServerConnection* TCPReactorAcceptor::createServiceHandler(Poco::Net::StreamSocket& socket)
{
	// enable nodelay per default: OSX really needs that
#if defined(POCO_HAS_UNIX_SOCKET)
	if (socket.address().family() != AddressFamily::UNIX_LOCAL)
#endif
	{
		socket.setNoDelay(true);
	}
	// Bound blocking response writes on the reactor thread: without this a
	// client that stops reading wedges the worker reactor (and every
	// connection multiplexed onto it) forever. Zero = disabled (default).
	const Poco::Timespan sendTimeout = _pParams->getSendTimeout();
	if (sendTimeout.totalMicroseconds() > 0)
	{
		socket.setSendTimeout(sendTimeout);
	}
	auto tmpConnPtr = std::make_shared<TCPReactorServerConnection>(socket, reactor());
	tmpConnPtr->setRecvMessageCallback(_recvMessageCallback);
	tmpConnPtr->initialize();
	return tmpConnPtr.get();
}

} // namespace Poco::Net

