#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"

namespace Poco::Net {



TCPReactorServer::TCPReactorServer(int port, TCPServerParams::Ptr pParams)
	: TCPReactorServer(SocketAddress(static_cast<Poco::UInt16>(port)), pParams)
{
}


TCPReactorServer::TCPReactorServer(const SocketAddress& address, TCPServerParams::Ptr pParams)
	: _threadPool("TCPRA", pParams->getAcceptorNum()),
	  _reactors(pParams->getAcceptorNum()),
	  _pParams(pParams),
	  _port(address.port()),
	  _stopped(false)
{
	for (auto& reactor : _reactors)
	{
		ServerSocket socket(address);
		_sockets.push_back(socket);
		if (_sockets.size() == 1)
		{
			_port = socket.address().port();
		}
		auto acceptor = std::make_shared<TCPReactorAcceptor>(socket, reactor, _pParams);
		_acceptors.push_back(acceptor);
	}
}

TCPReactorServer::~TCPReactorServer()
{
	stop();
}

void TCPReactorServer::start()
{
	for (auto& reactor : _reactors)
	{
		_threadPool.start(reactor);
	}
}

void TCPReactorServer::setRecvMessageCallback(const RecvMessageCallback& cb)
{
	for (auto& acceptor : _acceptors)
	{
		acceptor->setRecvMessageCallback(cb);
	}
}

void TCPReactorServer::stop()
{
	if (_stopped.exchange(true))
	{
		return;
	}
	for (auto& acceptor : _acceptors)
	{
		acceptor->stop();
	}
	for (auto& reactor : _reactors)
	{
		reactor.stop();
	}
	_threadPool.joinAll();
}

} // namespace Poco::Net

