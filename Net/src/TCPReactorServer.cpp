#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"

namespace Poco::Net {



TCPReactorServer::TCPReactorServer(int port, TCPServerParams::Ptr pParams)
	: _threadPool("TCPRA", pParams->getAcceptorNum()),
	  _reactors(pParams->getAcceptorNum()),
	  _pParams(pParams),
	  _port(port),
	  _stopped(false)
{
	for (auto& reactor : _reactors)
	{
		ServerSocket socket(_port);
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

