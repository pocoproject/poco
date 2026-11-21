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


namespace Poco::Net {


class HTTPSession;


class Net_API WebSocketImpl: public StreamSocketImpl
	/// This class implements a WebSocket, according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketImpl(StreamSocketImpl* pStreamSocketImpl, HTTPSession& session, bool mustMaskPayload);
		/// Creates a WebSocketImpl.

	// StreamSocketImpl
	int sendBytes(const void* buffer, int length, int flags) override;
		/// Sends a WebSocket protocol frame.
		///
		/// See WebSocket::sendFrame() for more information, including
		/// behavior if set to non-blocking.

	int receiveBytes(void* buffer, int length, int flags) override;
		/// Receives a WebSocket protocol frame.
		///
		/// See WebSocket::receiveFrame() for more information, including
		/// behavior if set to non-blocking.

	int receiveBytes(Poco::Buffer<char>& buffer, int flags = 0, const Poco::Timespan& span = 0) override;
		/// Receives a WebSocket protocol frame.
		///
		/// See WebSocket::receiveFrame() for more information, including
		/// behavior if set to non-blocking.

	int sendBytes(const SocketBufVec& buffers, int flags = 0) override;
		/// Not supported by a WebSocket.
		///
		/// Throws a Poco::InvalidAccessException.

	int receiveBytes(SocketBufVec& buffers, int flags = 0) override;
		/// Not supported by a WebSocket.
		///
		/// Throws a Poco::InvalidAccessException.

	[[nodiscard]] poco_socket_t releaseSocket() override;
		/// Not supported by a WebSocket.
		///
		/// Throws a Poco::InvalidAccessException.

	SocketImpl* acceptConnection(SocketAddress& clientAddr) override;
	void connect(const SocketAddress& address) override;
	void connect(const SocketAddress& address, const Poco::Timespan& timeout) override;
	void connectNB(const SocketAddress& address) override;
	void bind(const SocketAddress& address, bool reuseAddress = false) override;
	void bind(const SocketAddress& address, bool reuseAddress, bool reusePort) override;
	void bind6(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false) override;
	void bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only) override;
	void listen(int backlog = 64) override;
	void close() override;
	void shutdownReceive() override;
	int shutdownSend() override;
	int shutdown() override;
	int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0) override;
	int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0) override;
	void sendUrgent(unsigned char data) override;
	[[nodiscard]] int available() override;
	[[nodiscard]] bool secure() const override;
	void setSendBufferSize(int size) override;
	[[nodiscard]] int getSendBufferSize() override;
	void setReceiveBufferSize(int size) override;
	[[nodiscard]] int getReceiveBufferSize() override;
	void setSendTimeout(const Poco::Timespan& timeout) override;
	[[nodiscard]] Poco::Timespan getSendTimeout() override;
	void setReceiveTimeout(const Poco::Timespan& timeout) override;
	[[nodiscard]] Poco::Timespan getReceiveTimeout() override;
	void setBlocking(bool flag) override;
	[[nodiscard]] bool getBlocking() const override;

	// Internal
	[[nodiscard]] int frameFlags() const;
		/// Returns the frame flags of the most recently received frame.

	[[nodiscard]] bool mustMaskPayload() const;
		/// Returns true if the payload must be masked.

	void setMaxPayloadSize(int maxPayloadSize);
		/// Sets the maximum payload size for receiveFrame().
		///
		/// The default is std::numeric_limits<int>::max().

	[[nodiscard]] int getMaxPayloadSize() const;
		/// Returns the maximum payload size for receiveFrame().
		///
		/// The default is std::numeric_limits<int>::max().

	void setAllowedRSVBits(int allowedRSV);
		/// Sets the RSV bits (FRAME_FLAG_RSV1, FRAME_FLAG_RSV2,
		/// FRAME_FLAG_RSV3) that receiveFrame() accepts on received
		/// frames.
		///
		/// This is for applications that negotiate WebSocket extensions
		/// themselves (for example permessage-deflate) and use the
		/// corresponding RSV bits on both peers.
		///
		/// The default is 0: all RSV bits are rejected.

	[[nodiscard]] int getAllowedRSVBits() const;
		/// Returns the RSV bits that receiveFrame() accepts.
		///
		/// The default is 0: all RSV bits are rejected.

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

	[[nodiscard]] int peekHeader(ReceiveState& receiveState);
	[[nodiscard]] int incompleteHeader(ReceiveState& receiveState);
		/// Called by peekHeader() when the bytes available do not make a
		/// complete frame header. Returns 0 if the peer has closed the
		/// connection, so that the caller reports the close, and -1
		/// otherwise, so that the caller waits for the rest of the header.
		///
		/// Clears the frame flags, which the caller may already have taken
		/// from the part of the header it did receive.
	void skipHeader(int headerLength);
	[[nodiscard]] int receivePayload(char *buffer, int payloadLength, char mask[MASK_LENGTH], bool useMask, int maskOffset);
	[[nodiscard]] int receiveNBytes(void* buffer, int length);
	[[nodiscard]] int receiveSomeBytes(char* buffer, int length);
	[[nodiscard]] int peekSomeBytes(char* buffer, int length);
	~WebSocketImpl() override;

private:
	WebSocketImpl();

	StreamSocketImpl* _pStreamSocketImpl;
	int _maxPayloadSize;
	Poco::Buffer<char> _buffer;
	int _bufferOffset;
	bool _mustMaskPayload;
	int _allowedRSV;
	bool _peerClosed;
		/// Set once a receive on the underlying socket has reported the end
		/// of the connection.
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


inline int WebSocketImpl::getAllowedRSVBits() const
{
	return _allowedRSV;
}


inline int WebSocketImpl::getMaxPayloadSize() const
{
	return _maxPayloadSize;
}


} // namespace Poco::Net


#endif // Net_WebSocketImpl_INCLUDED
