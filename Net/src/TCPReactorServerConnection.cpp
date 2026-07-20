#include "Poco/Net/TCPReactorServerConnection.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"

namespace Poco::Net {


const int BUFFER_SIZE = 4096;


TCPReactorServerConnection::TCPReactorServerConnection(StreamSocket socket, SocketReactor& reactor)
	: _reactor(reactor), _socket(socket)
{
	_buf.reserve(BUFFER_SIZE);
}

TCPReactorServerConnection::~TCPReactorServerConnection()
{
}

void TCPReactorServerConnection::initialize()
{
	_reactor.addEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ReadableNotification>(
			shared_from_this(), &TCPReactorServerConnection::onRead));
}

void TCPReactorServerConnection::onRead(const AutoPtr<ReadableNotification>& pNf)
{
	// The accepted socket is blocking, so receiveBytes never returns < 0: on a
	// peer reset it THROWS (ConnectionResetException), and the read callback
	// runs the HTTP handler, which can throw too (a send timeout, a handler
	// error). An exception escaping onRead would be swallowed by the reactor's
	// per-socket catch and route to the ErrorHandler WITHOUT ever running
	// handleClose(): the connection object and its fd leak, and because the
	// dead fd keeps polling readable, the reactor re-dispatches onRead forever
	// and busy-spins. So contain every failure here and always close the
	// connection on error. See issue: HubMonitor dashboard silently unavailable.
	try
	{
		char tmp[BUFFER_SIZE] = {0};
		int  n = _socket.receiveBytes(tmp, sizeof(tmp));
		if (n <= 0)
		{
			// 0 = orderly EOF; blocking receiveBytes does not return < 0.
			handleClose();
		}
		else
		{
			_buf.append(tmp, n);
			_rcvCallback(shared_from_this());
		}
	}
	catch (const Poco::Exception& exc)
	{
		Poco::Logger::get("Poco.Net.TCPReactorServer").error(
			"connection closed on error: %s", exc.displayText());
		handleClose();
	}
	catch (const std::exception& exc)
	{
		Poco::Logger::get("Poco.Net.TCPReactorServer").error(
			"connection closed on error: %s", std::string(exc.what()));
		handleClose();
	}
	catch (...)
	{
		Poco::Logger::get("Poco.Net.TCPReactorServer").error(
			"connection closed on unknown error");
		handleClose();
	}
}

void TCPReactorServerConnection::onError(const AutoPtr<ErrorNotification>& pNf)
{
	handleClose();
}

void TCPReactorServerConnection::onShutdown(const AutoPtr<ShutdownNotification>& pNf)
{
	handleClose();
}

void TCPReactorServerConnection::handleClose()
{
	// here must keep _socket to delay the _socket destrcutor
	StreamSocket keepSocket = _socket;
	// here will delete this, so memberships' destructor will be invoked
	_reactor.removeEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ReadableNotification>(
			shared_from_this(), &TCPReactorServerConnection::onRead));
}

const StreamSocket& TCPReactorServerConnection::socket()
{
	return _socket;
}

std::string& TCPReactorServerConnection::buffer()
{
	return _buf;
}

void TCPReactorServerConnection::setRecvMessageCallback(const RecvMessageCallback& cb)
{
	_rcvCallback = cb;
}

} // namespace Poco::Net

