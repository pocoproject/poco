//
// HTTPSession.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPBufferAllocator.h"
#include "Poco/Net/NetException.h"
#include <cstring>


using Poco::TimeoutException;


namespace Poco::Net {


HTTPSession::HTTPSession():
	_pCurrent(nullptr),
	_pEnd(nullptr),
	_keepAlive(false),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(nullptr)
{
}


HTTPSession::HTTPSession(const StreamSocket& socket):
	_socket(socket),
	_pCurrent(nullptr),
	_pEnd(nullptr),
	_keepAlive(false),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(nullptr)
{
}


HTTPSession::HTTPSession(const StreamSocket& socket, bool keepAlive):
	_socket(socket),
	_pCurrent(nullptr),
	_pEnd(nullptr),
	_keepAlive(keepAlive),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(nullptr)
{
}


HTTPSession::~HTTPSession()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
	delete _pException;
}


void HTTPSession::setKeepAlive(bool keepAlive)
{
	_keepAlive = keepAlive;
}


void HTTPSession::setTimeout(const Poco::Timespan& timeout)
{
	setTimeout(timeout, timeout, timeout);
}


void HTTPSession::setTimeout(const Poco::Timespan& connectionTimeout, const Poco::Timespan& sendTimeout, const Poco::Timespan& receiveTimeout)
{
	_connectionTimeout = connectionTimeout;
	try
	{
		setSendTimeout(sendTimeout);
	}
	catch (...)
	{
		try
		{
			setReceiveTimeout(receiveTimeout);
		}
		catch (...)
		{
			// The exception from the send timeout is the one reported.
		}
		throw;
	}
	setReceiveTimeout(receiveTimeout);
}


void HTTPSession::setSendTimeout(const Poco::Timespan& timeout)
{
	_sendTimeout = timeout;
	if (connected())
	{
		try
		{
			_socket.setSendTimeout(timeout);
		}
		catch (NetException&)
		{
			// A socket that rejects the option has failed; the next send or receive reports it.
		}
	}
}


void HTTPSession::setReceiveTimeout(const Poco::Timespan& timeout)
{
	_receiveTimeout = timeout;
	if (connected())
	{
		try
		{
			_socket.setReceiveTimeout(timeout);
		}
		catch (NetException&)
		{
			// A socket that rejects the option has failed; the next send or receive reports it.
		}
	}
}


int HTTPSession::get()
{
	if (_pCurrent == _pEnd)
		refill();

	if (_pCurrent < _pEnd)
		return static_cast<unsigned char>(*_pCurrent++);
	else
		return std::char_traits<char>::eof();
}


int HTTPSession::peek()
{
	if (_pCurrent == _pEnd)
		refill();

	if (_pCurrent < _pEnd)
		return static_cast<unsigned char>(*_pCurrent);
	else
		return std::char_traits<char>::eof();
}


int HTTPSession::read(char* buffer, std::streamsize length)
{
	if (_pCurrent < _pEnd)
	{
		int n = (int) (_pEnd - _pCurrent);
		if (n > length) n = (int) length;
		std::memcpy(buffer, _pCurrent, n);
		_pCurrent += n;
		return n;
	}
	else return receive(buffer, (int) length);
}


int HTTPSession::write(const char* buffer, std::streamsize length)
{
	try
	{
		return _socket.sendBytes(buffer, (int) length);
	}
	catch (Poco::Exception& exc)
	{
		setException(exc);
		throw;
	}
}


int HTTPSession::receive(char* buffer, int length)
{
	try
	{
		if (_readDeadline.has_value())
		{
			const Poco::Timespan remaining(*_readDeadline - Poco::Clock());
			// poll() is needed only where the deadline comes before the receive timeout of the socket;
			// available() counts data that TLS has already decrypted, which poll() does not see.
			const bool deadlineFirst = _receiveTimeout == 0 || remaining < _receiveTimeout;
			if (remaining <= 0 || (deadlineFirst && _socket.available() == 0 && !_socket.poll(remaining, Socket::SELECT_READ)))
				throw TimeoutException("HTTP read deadline expired");
		}
		return _socket.receiveBytes(buffer, length);
	}
	catch (Poco::Exception& exc)
	{
		setException(exc);
		throw;
	}
}


bool HTTPSession::dataPending() const
{
	// Only a TLS socket holds data that poll() does not see: bytes decrypted but not yet read.
	return buffered() > 0 || (_socket.secure() && _socket.available() > 0) || _socket.poll(Poco::Timespan(), Socket::SELECT_READ);
}


void HTTPSession::refill()
{
	if (_pBuffer == nullptr)
	{
		_pBuffer.reset(new char[HTTPBufferAllocator::BUFFER_SIZE]);
	}
	_pCurrent = _pEnd = _pBuffer.get();
	int n = receive(_pBuffer.get(), HTTPBufferAllocator::BUFFER_SIZE);
	// A non-blocking socket returns a negative value when no data is available.
	if (n > 0) _pEnd += n;
}


bool HTTPSession::connected() const
{
	return _socket.impl()->initialized();
}


void HTTPSession::connect(const SocketAddress& address)
{
	_socket.connect(address, _connectionTimeout);
	_socket.setReceiveTimeout(_receiveTimeout);
	_socket.setSendTimeout(_sendTimeout);
#if defined(POCO_HAS_UNIX_SOCKET)
	if (address.family() != SocketAddress::UNIX_LOCAL)
#endif
		_socket.setNoDelay(true);
	// There may be leftover data from a previous (failed) request in the buffer,
	// so we clear it.
	_pCurrent = _pEnd = _pBuffer.get();
}


void HTTPSession::connect(const SocketAddress& targetAddress, const SocketAddress& sourceAddress)
{
	_socket.bind(sourceAddress, true);
	connect(targetAddress);
}


void HTTPSession::abort()
{
	_socket.shutdown();
	close();
}


void HTTPSession::close()
{
	_socket.close();
}


void HTTPSession::setException(const Poco::Exception& exc)
{
	delete _pException;
	_pException = exc.clone();
}


void HTTPSession::clearException()
{
	delete _pException;
	_pException = nullptr;
}


void HTTPSession::setReadDeadline(const Poco::Clock& deadline)
{
	_readDeadline = deadline;
}


void HTTPSession::clearReadDeadline()
{
	_readDeadline.reset();
}


StreamSocket HTTPSession::detachSocket()
{
	StreamSocket oldSocket(_socket);
	StreamSocket newSocket;
	_socket = newSocket;
	return oldSocket;
}


void HTTPSession::attachSocket(const StreamSocket& socket)
{
	_socket = socket;
	_pCurrent = _pEnd = _pBuffer.get();
}


void HTTPSession::attachSessionData(const Poco::Any& data)
{
	_data = data;
}


void HTTPSession::drainBuffer(Poco::Buffer<char>& buffer)
{
	buffer.assign(_pCurrent, static_cast<std::size_t>(_pEnd - _pCurrent));
	_pCurrent = _pEnd;
}


} // namespace Poco::Net
