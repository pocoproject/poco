#include "Poco/Net/TCPReactorServerConnection.h"
#include "Poco/Net/HTTPObserver.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"

namespace Poco {
namespace Net {


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
	// ShutdownNotification is dispatched when the reactor stops; without it
	// the connection (and its socket) outlives the reactor's run loop.
	_reactor.addEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ShutdownNotification>(
			shared_from_this(), &TCPReactorServerConnection::onShutdown));
	// Without a registered ErrorNotification observer the reactor's error
	// dispatch is dropped by the NotificationCenter, so socket errors would
	// surface only when a later read fails.
	_reactor.addEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ErrorNotification>(
			shared_from_this(), &TCPReactorServerConnection::onError));
	// Readable must come last: this may run on the acceptor's thread while
	// the reactor polls, and once the socket is readable an immediately
	// disconnecting client could run handleClose() on the reactor thread
	// before registration completes, leaving a half-registered connection.
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
	// connection on error.
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
		// Close FIRST: the leak/hot-spin fix must run even if logging throws.
		// Then log best-effort, guarded so nothing escapes onRead. handleClose()
		// may already have destroyed this, so the block below touches only the
		// exception and free functions, never a member.
		handleClose();
		try
		{
			// Expected client-disconnect exceptions - a peer reset/abort, or a
			// timeout from a stalled peer (including the accepted-socket send
			// timeout) - are routine on a busy server, so log them at debug.
			// Genuinely unexpected failures stay at error so alerting can rely
			// on it.
			Poco::Logger& log = Poco::Logger::get("Poco.Net.TCPReactorServer");
			if (dynamic_cast<const Poco::Net::ConnectionResetException*>(&exc)
				|| dynamic_cast<const Poco::Net::ConnectionAbortedException*>(&exc)
				|| dynamic_cast<const Poco::TimeoutException*>(&exc))
			{
				if (log.debug()) log.debug("connection closed: %s", exc.displayText());
			}
			else
				log.error("connection closed on error: %s", exc.displayText());
		}
		catch (...) {}
	}
	catch (const std::exception& exc)
	{
		handleClose();
		try
		{
			Poco::Logger::get("Poco.Net.TCPReactorServer").error(
				"connection closed on error: %s", std::string(exc.what()));
		}
		catch (...) {}
	}
	catch (...)
	{
		handleClose();
		try
		{
			Poco::Logger::get("Poco.Net.TCPReactorServer").error(
				"connection closed on unknown error");
		}
		catch (...) {}
	}
}

void TCPReactorServerConnection::onError(const AutoPtr<ErrorNotification>& pNf)
{
	// A throw would abort the reactor's notification loop for the remaining
	// connections. The payload is deliberately not reported: the POLL_ERROR
	// dispatch carries none (code 0, empty description), and on the exception
	// paths SocketReactor::run() has already passed the exception itself to
	// the ErrorHandler.
	try
	{
		handleClose();
	}
	catch (...) {}
}

void TCPReactorServerConnection::onShutdown(const AutoPtr<ShutdownNotification>& pNf)
{
	// A throw here would abort the reactor's shutdown broadcast, leaving the
	// remaining connections open.
	try
	{
		handleClose();
	}
	catch (...) {}
}

void TCPReactorServerConnection::handleClose()
{
	// keepSocket delays the socket close until the removals are done
	StreamSocket keepSocket = _socket;
	// The reactor's observers hold the only owning shared_ptrs; removing them
	// ends this object's lifetime (the dispatching NotificationCenter keeps
	// it alive until the current handler returns). Remove Readable last so
	// the intermediate poll-mask updates never pass mode 0, and touch no
	// member after the removals.
	_reactor.removeEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ErrorNotification>(
			shared_from_this(), &TCPReactorServerConnection::onError));
	_reactor.removeEventHandler(
		_socket,
		HTTPObserver<TCPReactorServerConnection, ShutdownNotification>(
			shared_from_this(), &TCPReactorServerConnection::onShutdown));
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

}} // namespace Poco::Net

