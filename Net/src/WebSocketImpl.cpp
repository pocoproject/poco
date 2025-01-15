//
// WebSocketImpl.cpp
//
// Library: Net
// Package: WebSocket
// Module:  WebSocketImpl
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef NOMINMAX
	#define NOMINMAX
#endif // NOMINMAX
#include "Poco/Net/WebSocketImpl.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Buffer.h"
#include "Poco/BinaryWriter.h"
#include "Poco/BinaryReader.h"
#include "Poco/MemoryStream.h"
#include "Poco/Format.h"
#include <limits>
#include <cstring>


namespace Poco {
namespace Net {


WebSocketImpl::WebSocketImpl(StreamSocketImpl* pStreamSocketImpl, HTTPSession& session, bool mustMaskPayload):
	StreamSocketImpl(pStreamSocketImpl->sockfd()),
	_pStreamSocketImpl(pStreamSocketImpl),
	_maxPayloadSize(std::numeric_limits<int>::max()),
	_buffer(0),
	_bufferOffset(0),
	_mustMaskPayload(mustMaskPayload)
{
	poco_check_ptr(pStreamSocketImpl);
	_pStreamSocketImpl->duplicate();
	session.drainBuffer(_buffer);
}


WebSocketImpl::~WebSocketImpl()
{
	try
	{
		_pStreamSocketImpl->release();
		reset();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


int WebSocketImpl::sendBytes(const void* buffer, int length, int flags)
{
	if (_sendState.remainingPayloadLength > 0)
	{
		if (length != _sendState.length)
		{
			throw InvalidArgumentException("Pending send buffer length mismatch");
		}
		int sent = _pStreamSocketImpl->sendBytes(_sendState.payload.begin() + _sendState.remainingPayloadOffset, _sendState.remainingPayloadLength);
		if (sent >= 0)
		{
			if (sent < _sendState.remainingPayloadLength)
			{
				_sendState.remainingPayloadOffset += sent;
				_sendState.remainingPayloadLength -= sent;
				return -1;
			}
			else 
			{
				_sendState.length = 0;
				_sendState.remainingPayloadOffset = 0;
				_sendState.remainingPayloadLength = 0;
				return length;
			}
		}
		else return -1;
	}

	Poco::Buffer<char>& frame(_sendState.payload);
	frame.resize(length + MAX_HEADER_LENGTH, false);
	Poco::MemoryOutputStream ostr(frame.begin(), frame.size());
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);

	if (flags == 0) flags = WebSocket::FRAME_BINARY;
	flags &= 0xff;
	writer << static_cast<Poco::UInt8>(flags);
	Poco::UInt8 lengthByte(0);
	if (_mustMaskPayload)
	{
		lengthByte |= FRAME_FLAG_MASK;
	}
	if (length < 126)
	{
		lengthByte |= static_cast<Poco::UInt8>(length);
		writer << lengthByte;
	}
	else if (length < 65536)
	{
		lengthByte |= 126;
		writer << lengthByte << static_cast<Poco::UInt16>(length);
	}
	else
	{
		lengthByte |= 127;
		writer << lengthByte << static_cast<Poco::UInt64>(length);
	}
	if (_mustMaskPayload)
	{
		const Poco::UInt32 mask = _rnd.next();
		const char* m = reinterpret_cast<const char*>(&mask);
		const char* b = reinterpret_cast<const char*>(buffer);
		writer.writeRaw(m, MASK_LENGTH);
		char* p = frame.begin() + ostr.charsWritten();
		for (int i = 0; i < length; i++)
		{
			p[i] = b[i] ^ m[i % MASK_LENGTH];
		}
	}
	else
	{
		std::memcpy(frame.begin() + ostr.charsWritten(), buffer, length);
	}

	int frameLength = length + static_cast<int>(ostr.charsWritten());
	int sent = _pStreamSocketImpl->sendBytes(frame.begin(), frameLength);
	if (sent >= 0)
	{
		if (sent < frameLength)
		{
			_sendState.length = length;
			_sendState.remainingPayloadOffset = sent;
			_sendState.remainingPayloadLength = frameLength - sent;
			return -1;
		}
		else 
		{
			_sendState.length = 0;
			_sendState.remainingPayloadOffset = 0;
			_sendState.remainingPayloadLength = 0;
			return length;
		}
	}
	else
	{
		_sendState.length = length;
		_sendState.remainingPayloadOffset = 0;
		_sendState.remainingPayloadLength = frameLength;
		return -1;
	}	
}


int WebSocketImpl::peekHeader(ReceiveState& receiveState)
{
	char header[MAX_HEADER_LENGTH];
	
	receiveState.frameFlags = 0;
	receiveState.useMask = false;
	receiveState.headerLength = 0;
	receiveState.payloadLength = 0;
	receiveState.remainingPayloadLength = 0;

	int n = peekSomeBytes(header, MAX_HEADER_LENGTH);
	if (n == 0)
		return 0;
	else if (n < 2)
		return -1;

	Poco::UInt8 flags = static_cast<Poco::UInt8>(header[0]);
	receiveState.frameFlags = flags;
	Poco::UInt8 lengthByte = static_cast<Poco::UInt8>(header[1]);
	receiveState.useMask = ((lengthByte & FRAME_FLAG_MASK) != 0);
	int maskOffset = 0;
	lengthByte &= 0x7f;
	if (lengthByte == 127)
	{
		if (n < 10)
		{
			receiveState.frameFlags = 0;
			return -1;
		}
		Poco::MemoryInputStream istr(header + 2, 8);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);
		Poco::UInt64 l;
		reader >> l;
		if (l > _maxPayloadSize) throw WebSocketException("Payload too big", WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		receiveState.payloadLength = static_cast<int>(l);
		maskOffset = 10;
	}
	else if (lengthByte == 126)
	{
		if (n < 4)
		{
			receiveState.frameFlags = 0;
			return -1;
		}
		Poco::MemoryInputStream istr(header + 2, 2);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);
		Poco::UInt16 l;
		reader >> l;
		if (l > _maxPayloadSize) throw WebSocketException("Payload too big", WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		receiveState.payloadLength = static_cast<int>(l);
		maskOffset = 4;
	}
	else
	{
		if (lengthByte > _maxPayloadSize) throw WebSocketException("Payload too big", WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		receiveState.payloadLength = lengthByte;
		maskOffset = 2;
	}

	if (receiveState.useMask)
	{
		if (n < maskOffset + MASK_LENGTH)
		{
			receiveState.frameFlags = 0;
			return -1;
		}
		std::memcpy(receiveState.mask, header + maskOffset, MASK_LENGTH);
		receiveState.headerLength = maskOffset + MASK_LENGTH;
	}
	else
	{
		receiveState.headerLength = maskOffset;
	}

	receiveState.remainingPayloadLength = receiveState.payloadLength;

	return receiveState.payloadLength;
}


void WebSocketImpl::skipHeader(int headerLength)
{
	poco_assert_dbg (headerLength <= MAX_HEADER_LENGTH);

	if (headerLength > 0)
	{
		char header[MAX_HEADER_LENGTH];
		int n = receiveNBytes(header, headerLength);
		poco_assert_dbg (n == headerLength);
	}
}


void WebSocketImpl::setMaxPayloadSize(int maxPayloadSize)
{
	poco_assert (maxPayloadSize > 0);

	_maxPayloadSize = maxPayloadSize;
}


int WebSocketImpl::receivePayload(char *buffer, int payloadLength, char mask[MASK_LENGTH], bool useMask, int maskOffset)
{
	int received = receiveNBytes(reinterpret_cast<char*>(buffer), payloadLength);
	if (received > 0 && useMask)
	{
		for (int i = 0; i < received; i++)
		{
			buffer[i] ^= mask[(i + maskOffset) % MASK_LENGTH];
		}
	}
	return received;
}


int WebSocketImpl::receiveBytes(void* buffer, int length, int)
{
	if (getBlocking())
	{
		int payloadLength = -1;
		while (payloadLength < 0)
		{
			payloadLength = peekHeader(_receiveState);
		}
		if (payloadLength <= 0)
		{
			skipHeader(_receiveState.headerLength);
			return payloadLength;
		}
		else if (payloadLength > length)
		{
			throw WebSocketException(Poco::format("Insufficient buffer for payload size %d", payloadLength), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		}

		skipHeader(_receiveState.headerLength);

		if (receivePayload(reinterpret_cast<char*>(buffer), payloadLength, _receiveState.mask, _receiveState.useMask, 0) != payloadLength)
			throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

		return payloadLength;
	}
	else
	{
		if (_receiveState.remainingPayloadLength == 0)
		{
			int payloadLength = peekHeader(_receiveState);
			if (payloadLength <= 0)
			{
				skipHeader(_receiveState.headerLength);
				return payloadLength;
			}
			else if (payloadLength > length)
			{
				throw WebSocketException(Poco::format("Insufficient buffer for payload size %d", payloadLength), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
			}

			skipHeader(_receiveState.headerLength);

			_receiveState.payload.resize(payloadLength, false);
		}
		else if (_receiveState.payloadLength > length)
		{
			throw WebSocketException(Poco::format("Insufficient buffer for payload size %d", _receiveState.payloadLength), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		}
		int payloadOffset = _receiveState.payloadLength - _receiveState.remainingPayloadLength;
		int n = receivePayload(_receiveState.payload.begin() + payloadOffset, _receiveState.remainingPayloadLength, _receiveState.mask, _receiveState.useMask, _receiveState.maskOffset);
		if (n > 0)
		{
			_receiveState.remainingPayloadLength -= n;
			if (_receiveState.remainingPayloadLength == 0)
			{
				_receiveState.maskOffset = 0;
				std::memcpy(buffer, _receiveState.payload.begin(), _receiveState.payloadLength);
				return _receiveState.payloadLength;
			}
			else
			{
				_receiveState.maskOffset += n;
				return -1;
			}
		}
		else if (n == 0)
		{
			throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);
		}
		else
		{
			return -1;
		}
	}
}


int WebSocketImpl::receiveBytes(Poco::Buffer<char>& buffer, int, const Poco::Timespan&)
{
	if (getBlocking())
	{
		int payloadLength = -1;
		while (payloadLength < 0)
		{
			payloadLength = peekHeader(_receiveState);
		}
		if (payloadLength <= 0)
			return payloadLength;

		skipHeader(_receiveState.headerLength);

		std::size_t oldSize = buffer.size();
		buffer.resize(oldSize + payloadLength);

		if (receivePayload(buffer.begin() + oldSize, payloadLength, _receiveState.mask, _receiveState.useMask, 0) != payloadLength)
			throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

		return payloadLength;
	}
	else
	{
		if (_receiveState.remainingPayloadLength == 0)
		{
			int payloadLength = peekHeader(_receiveState);
			if (payloadLength <= 0)
				return payloadLength;

			skipHeader(_receiveState.headerLength);

			_receiveState.payload.resize(payloadLength, false);
		}
		int payloadOffset = _receiveState.payloadLength - _receiveState.remainingPayloadLength;
		int n = receivePayload(_receiveState.payload.begin() + payloadOffset, _receiveState.remainingPayloadLength, _receiveState.mask, _receiveState.useMask, _receiveState.maskOffset);
		if (n > 0)
		{
			_receiveState.remainingPayloadLength -= n;
			if (_receiveState.remainingPayloadLength == 0)
			{
				_receiveState.maskOffset = 0;
				std::size_t oldSize = buffer.size();
				buffer.resize(oldSize + _receiveState.payloadLength);

				std::memcpy(buffer.begin() + oldSize, _receiveState.payload.begin(), _receiveState.payloadLength);
				return _receiveState.payloadLength;
			}
			else
			{
				_receiveState.maskOffset += n;
				return -1;
			}
		}
		else if (n == 0)
		{
			throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);
		}
		else
		{
			return -1;
		}
	}
}


int WebSocketImpl::receiveNBytes(void* buffer, int length)
{
	int received = receiveSomeBytes(reinterpret_cast<char*>(buffer), length);
	if (received > 0)
	{
		while (received < length)
		{
			int n = receiveSomeBytes(reinterpret_cast<char*>(buffer) + received, length - received);
			if (n > 0)
				received += n;
			else
				break;
		}
	}
	return received;
}


int WebSocketImpl::receiveSomeBytes(char* buffer, int length)
{
	int n = static_cast<int>(_buffer.size()) - _bufferOffset;
	if (n > 0)
	{
		if (length < n) n = length;
		std::memcpy(buffer, _buffer.begin() + _bufferOffset, n);
		_bufferOffset += length;
		return n;
	}
	else
	{
		return _pStreamSocketImpl->receiveBytes(buffer, length);
	}
}


int WebSocketImpl::peekSomeBytes(char* buffer, int length)
{
	int n = static_cast<int>(_buffer.size()) - _bufferOffset;
	if (n > 0)
	{
		if (length < n) n = length;
		std::memcpy(buffer, _buffer.begin() + _bufferOffset, n);
		if (length > n)
		{
			int rc = _pStreamSocketImpl->receiveBytes(buffer + n, length - n);
			if (rc > 0) 
			{
				std::size_t currentSize = _buffer.size();
				_buffer.resize(currentSize + rc);
				std::memcpy(_buffer.begin() + currentSize, buffer + n, rc);
				n += rc;
			}
		}
		return n;
	}
	else
	{
		int rc = _pStreamSocketImpl->receiveBytes(buffer, length);
		if (rc > 0)
		{
			_buffer.resize(rc);
			std::memcpy(_buffer.begin(), buffer, rc);
			_bufferOffset = 0;
		}
		return rc;
	}
}


SocketImpl* WebSocketImpl::acceptConnection(SocketAddress& clientAddr)
{
	throw Poco::InvalidAccessException("Cannot acceptConnection() on a WebSocketImpl");
}


void WebSocketImpl::connect(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketImpl");
}


void WebSocketImpl::connect(const SocketAddress& address, const Poco::Timespan& timeout)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketImpl");
}


void WebSocketImpl::connectNB(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connectNB() a WebSocketImpl");
}


void WebSocketImpl::bind(const SocketAddress& address, bool reuseAddress)
{
	throw Poco::InvalidAccessException("Cannot bind() a WebSocketImpl");
}


void WebSocketImpl::bind(const SocketAddress& address, bool reuseAddress, bool reusePort)
{
	throw Poco::InvalidAccessException("Cannot bind() a WebSocketImpl");
}


void WebSocketImpl::bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	throw Poco::InvalidAccessException("Cannot bind6() a WebSocketImpl");
}


void WebSocketImpl::bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only)
{
	throw Poco::InvalidAccessException("Cannot bind6() a WebSocketImpl");
}


void WebSocketImpl::listen(int backlog)
{
	throw Poco::InvalidAccessException("Cannot listen() on a WebSocketImpl");
}


void WebSocketImpl::close()
{
	_pStreamSocketImpl->close();
	reset();
}


void WebSocketImpl::shutdownReceive()
{
	_pStreamSocketImpl->shutdownReceive();
}


int WebSocketImpl::shutdownSend()
{
	return _pStreamSocketImpl->shutdownSend();
}


int WebSocketImpl::shutdown()
{
	return _pStreamSocketImpl->shutdown();
}


int WebSocketImpl::sendTo(const void* buffer, int length, const SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot sendTo() on a WebSocketImpl");
}


int WebSocketImpl::receiveFrom(void* buffer, int length, SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot receiveFrom() on a WebSocketImpl");
}


void WebSocketImpl::sendUrgent(unsigned char data)
{
	throw Poco::InvalidAccessException("Cannot sendUrgent() on a WebSocketImpl");
}


bool WebSocketImpl::secure() const
{
	return _pStreamSocketImpl->secure();
}


void WebSocketImpl::setSendBufferSize(int size)
{
	_pStreamSocketImpl->setSendBufferSize(size);
}


int WebSocketImpl::getSendBufferSize()
{
	return _pStreamSocketImpl->getSendBufferSize();
}


void WebSocketImpl::setReceiveBufferSize(int size)
{
	_pStreamSocketImpl->setReceiveBufferSize(size);
}


int WebSocketImpl::getReceiveBufferSize()
{
	return _pStreamSocketImpl->getReceiveBufferSize();
}


void WebSocketImpl::setSendTimeout(const Poco::Timespan& timeout)
{
	_pStreamSocketImpl->setSendTimeout(timeout);
}


Poco::Timespan WebSocketImpl::getSendTimeout()
{
	return _pStreamSocketImpl->getSendTimeout();
}


void WebSocketImpl::setReceiveTimeout(const Poco::Timespan& timeout)
{
	_pStreamSocketImpl->setReceiveTimeout(timeout);
}


Poco::Timespan WebSocketImpl::getReceiveTimeout()
{
	return _pStreamSocketImpl->getReceiveTimeout();
}


void WebSocketImpl::setBlocking(bool flag)
{
	_pStreamSocketImpl->setBlocking(flag);
}


bool WebSocketImpl::getBlocking() const
{
	return _pStreamSocketImpl->getBlocking();
}


int WebSocketImpl::available()
{
	int n = static_cast<int>(_buffer.size()) - _bufferOffset;
	if (n > 0)
		return n + _pStreamSocketImpl->available();
	else
		return _pStreamSocketImpl->available();
}


} } // namespace Poco::Net
