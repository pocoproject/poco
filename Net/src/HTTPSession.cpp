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


namespace Poco {
namespace Net {


HTTPSession::HTTPSession(bool extBuf):
	_pBuffer(0),
	_pCurrent(0),
	_pEnd(0),
	_keepAlive(false),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(0),
	_pExtBufIn(extBuf ? new IOBuffer : 0),
	_pExtBufOut(extBuf ? new IOBuffer : 0),
	_hasInData(false),
	_hasOutData(false)
{
}


HTTPSession::HTTPSession(const StreamSocket& socket):
	_socket(socket),
	_pBuffer(0),
	_pCurrent(0),
	_pEnd(0),
	_keepAlive(false),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(0),
	_pExtBufIn(0),
	_pExtBufOut(0),
	_hasInData(false),
	_hasOutData(false)
{
}


HTTPSession::HTTPSession(const StreamSocket& socket, bool keepAlive):
	_socket(socket),
	_pBuffer(0),
	_pCurrent(0),
	_pEnd(0),
	_keepAlive(keepAlive),
	_connectionTimeout(HTTP_DEFAULT_CONNECTION_TIMEOUT),
	_receiveTimeout(HTTP_DEFAULT_TIMEOUT),
	_sendTimeout(HTTP_DEFAULT_TIMEOUT),
	_pException(0),
	_pExtBufIn(0),
	_pExtBufOut(0),
	_hasInData(false),
	_hasOutData(false)
{
}


HTTPSession::~HTTPSession()
{
	try
	{
		if (_pBuffer) HTTPBufferAllocator::deallocate(_pBuffer, HTTPBufferAllocator::BUFFER_SIZE);
		delete _pExtBufIn;
		delete _pExtBufOut;
	}
	catch (...)
	{
		poco_unexpected();
	}
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
	 _sendTimeout = sendTimeout;
	 _receiveTimeout = receiveTimeout;
}


int HTTPSession::get()
{
	if (_pCurrent == _pEnd)
		refill();
	
	if (_pCurrent < _pEnd)
		return *_pCurrent++;
	else
		return std::char_traits<char>::eof();
}

	
int HTTPSession::peek()
{
	if (_pCurrent == _pEnd)
		refill();

	if (_pCurrent < _pEnd)
		return *_pCurrent;
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
		if (!_pExtBufOut)
			return _socket.sendBytes(buffer, (int) length);
		else
		{
			Poco::Mutex::ScopedLock l(_outMutex);
			std::size_t len = _pExtBufOut->size();
			_pExtBufOut->append(buffer, length);
			_hasOutData = (_pExtBufOut->find("\r\n\r\n") != _pExtBufOut->npos);
			return static_cast<int>(length);
		}
	}
	catch (Poco::Exception& exc)
	{
		setException(exc);
		throw;
	}
}


int HTTPSession::getSend(IOBuffer& buf)
{
	Poco::Mutex::ScopedLock l(_outMutex);
	if (_hasOutData)
	{
		buf = std::move(*_pExtBufOut);
		_hasOutData = false;
		_pExtBufOut->clear();
	}
	return buf.size();
}


int HTTPSession::receive(char* buffer, int length)
{
	try
	{
		if (!_pExtBufIn)
			return _socket.receiveBytes(buffer, length);
		else
		{
			Poco::Mutex::ScopedLock l(_inMutex);
			int len = 0;
			if (_hasInData)
			{
				std::string::size_type pos = _pExtBufIn->find("\r\n\r\n");
				std::string::size_type nPos = _pExtBufIn->npos;
				while (pos != nPos && len < length)
				{
					int cpLen = _pExtBufIn->size() > length - len ? length - len : static_cast<int>(_pExtBufIn->size());
					std::memcpy(buffer + len, _pExtBufIn->data() + len, cpLen);
					_pExtBufIn->erase(0, cpLen);
					len += cpLen;
					pos = _pExtBufIn->find("\r\n\r\n");
				}
				_hasInData = false;
			}
			return len;
		}
	}
	catch (Poco::Exception& exc)
	{
		setException(exc);
		throw;
	}
}


int HTTPSession::setReceive(char* buffer, int length)
{
	Poco::Mutex::ScopedLock l(_inMutex);
	_pExtBufIn->append(buffer, length);
	_hasInData = (_pExtBufIn->find("\r\n\r\n") != _pExtBufIn->npos);
	return length;
}


void HTTPSession::refill()
{
	if (!_pBuffer)
	{
		_pBuffer = HTTPBufferAllocator::allocate(HTTPBufferAllocator::BUFFER_SIZE);
	}
	_pCurrent = _pEnd = _pBuffer;
	int n = receive(_pBuffer, HTTPBufferAllocator::BUFFER_SIZE);
	_pEnd += n;
}


bool HTTPSession::connected() const
{
	if (!_pExtBufIn)
		return _socket.impl()->initialized();
	else return true;
}


void HTTPSession::connect(const SocketAddress& address)
{
	if (!_pExtBufIn)
	{
		_socket.connect(address, _connectionTimeout);
		_socket.setReceiveTimeout(_receiveTimeout);
		_socket.setSendTimeout(_sendTimeout);
		_socket.setNoDelay(true);
	}
	// There may be leftover data from a previous (failed) request in the buffer,
	// so we clear it.
	_pCurrent = _pEnd = _pBuffer;
}


void HTTPSession::abort()
{
	if (!_pExtBufIn)
	{
		_socket.shutdown();
		close();
	}
}


void HTTPSession::close()
{
	if (!_pExtBufIn) _socket.close();
}


void HTTPSession::setException(const Poco::Exception& exc)
{
	delete _pException;
	_pException = exc.clone();
}


void HTTPSession::clearException()
{
	delete _pException;
	_pException = 0;
}


StreamSocket HTTPSession::detachSocket()
{
	if (!_pExtBufIn)
	{
		StreamSocket oldSocket(_socket);
		StreamSocket newSocket;
		_socket = newSocket;
		return oldSocket;
	}
	else return StreamSocket();
}


void HTTPSession::attachSocket(const StreamSocket& socket)
{
	if (!_pExtBufIn) _socket = socket;
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


} } // namespace Poco::Net
