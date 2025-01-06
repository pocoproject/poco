//
// WebSocketImpl.h
//
// Library: Net
// Package: WebSocket
// Module:  WebSocketImpl
//
// Definition of the StreamSocketImpl class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_WebSocketImpl_INCLUDED
#define Net_WebSocketImpl_INCLUDED


#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Buffer.h"
#include "Poco/Random.h"
#include "Poco/Buffer.h"


namespace Poco {
namespace Net {


class HTTPSession;


class Net_API WebSocketImpl: public StreamSocketImpl
	/// This class implements a WebSocket, according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketImpl(StreamSocketImpl* pStreamSocketImpl, HTTPSession& session, bool mustMaskPayload);
		/// Creates a WebSocketImpl.

	// StreamSocketImpl
	virtual int sendBytes(const void* buffer, int length, int flags);
		/// Sends a WebSocket protocol frame.
		///
		/// See WebSocket::sendFrame() for more information, including
		/// behavior if set to non-blocking.

	virtual int receiveBytes(void* buffer, int length, int flags);
		/// Receives a WebSocket protocol frame.
		///
		/// See WebSocket::receiveFrame() for more information, including
		/// behavior if set to non-blocking.

	virtual int receiveBytes(Poco::Buffer<char>& buffer, int flags = 0, const Poco::Timespan& span = 0);
		/// Receives a WebSocket protocol frame.
		///
		/// See WebSocket::receiveFrame() for more information, including
		/// behavior if set to non-blocking.

	virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
	virtual void connect(const SocketAddress& address);
	virtual void connect(const SocketAddress& address, const Poco::Timespan& timeout);
	virtual void connectNB(const SocketAddress& address);
	virtual void bind(const SocketAddress& address, bool reuseAddress = false);
	virtual void bind(const SocketAddress& address, bool reuseAddress, bool reusePort);
	virtual void bind6(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false);
	virtual void bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only);
	virtual void listen(int backlog = 64);
	virtual void close();
	virtual void shutdownReceive();
	virtual int shutdownSend();
	virtual int shutdown();
	virtual int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0);
	virtual int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0);
	virtual void sendUrgent(unsigned char data);
	virtual int available();
	virtual bool secure() const;
	virtual void setSendBufferSize(int size);
	virtual int getSendBufferSize();
	virtual void setReceiveBufferSize(int size);
	virtual int getReceiveBufferSize();
	virtual void setSendTimeout(const Poco::Timespan& timeout);
	virtual Poco::Timespan getSendTimeout();
	virtual void setReceiveTimeout(const Poco::Timespan& timeout);
	virtual Poco::Timespan getReceiveTimeout();
	virtual void setBlocking(bool flag);
	virtual bool getBlocking() const;

	// Internal
	int frameFlags() const;
		/// Returns the frame flags of the most recently received frame.

	bool mustMaskPayload() const;
		/// Returns true if the payload must be masked.

	void setMaxPayloadSize(int maxPayloadSize);
		/// Sets the maximum payload size for receiveFrame().
		///
		/// The default is std::numeric_limits<int>::max().

	int getMaxPayloadSize() const;
		/// Returns the maximum payload size for receiveFrame().
		///
		/// The default is std::numeric_limits<int>::max().

protected:
	enum
	{
		FRAME_FLAG_MASK   = 0x80,
		MAX_HEADER_LENGTH = 14,
		MASK_LENGTH       = 4
	};

	struct ReceiveState
	{
		int frameFlags = 0;
		bool useMask = false;
		char mask[MASK_LENGTH];
		int headerLength = 0;
		int payloadLength = 0;
		int remainingPayloadLength = 0;
		Poco::Buffer<char> payload{0};
		int maskOffset = 0;
	};

	struct SendState
	{
		int length = 0;
		int remainingPayloadOffset = 0;
		int remainingPayloadLength = 0;
		Poco::Buffer<char> payload{0};
	};

	int peekHeader(ReceiveState& receiveState);
	void skipHeader(int headerLength);
	int receivePayload(char *buffer, int payloadLength, char mask[MASK_LENGTH], bool useMask, int maskOffset);
	int receiveNBytes(void* buffer, int length);
	int receiveSomeBytes(char* buffer, int length);
	int peekSomeBytes(char* buffer, int length);
	virtual ~WebSocketImpl();

private:
	WebSocketImpl();

	StreamSocketImpl* _pStreamSocketImpl;
	int _maxPayloadSize;
	Poco::Buffer<char> _buffer;
	int _bufferOffset;
	bool _mustMaskPayload;
	ReceiveState _receiveState;
	SendState _sendState;
	Poco::Random _rnd;
};


//
// inlines
//
inline int WebSocketImpl::frameFlags() const
{
	return _receiveState.frameFlags;
}


inline bool WebSocketImpl::mustMaskPayload() const
{
	return _mustMaskPayload;
}


inline int WebSocketImpl::getMaxPayloadSize() const
{
	return _maxPayloadSize;
}


} } // namespace Poco::Net


#endif // Net_WebSocketImpl_INCLUDED
