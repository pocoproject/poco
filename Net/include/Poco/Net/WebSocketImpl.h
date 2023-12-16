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
	int sendBytes(const void* buffer, int length, int flags) override;
		/// Sends a WebSocket protocol frame.

	int receiveBytes(void* buffer, int length, int flags) override;
		/// Receives a WebSocket protocol frame.

	int receiveBytes(Poco::Buffer<char>& buffer, int flags = 0, const Poco::Timespan& span = 0) override;
		/// Receives a WebSocket protocol frame.

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
	void shutdownSend() override;
	void shutdown() override;
	int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0) override;
	int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0) override;
	void sendUrgent(unsigned char data) override;
	int available() override;
	bool secure() const override;
	void setSendTimeout(const Poco::Timespan& timeout) override;
	Poco::Timespan getSendTimeout() override;
	void setReceiveTimeout(const Poco::Timespan& timeout) override;
	Poco::Timespan getReceiveTimeout() override;

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
		MAX_HEADER_LENGTH = 14
	};

	int receiveHeader(char mask[4], bool& useMask);
	int receivePayload(char *buffer, int payloadLength, char mask[4], bool useMask);
	int receiveNBytes(void* buffer, int bytes);
	int receiveSomeBytes(char* buffer, int bytes);
	~WebSocketImpl() override;

private:
	WebSocketImpl();

	StreamSocketImpl* _pStreamSocketImpl;
	int _maxPayloadSize;
	Poco::Buffer<char> _buffer;
	int _bufferOffset;
	int _frameFlags;
	bool _mustMaskPayload;
	Poco::Random _rnd;
};


//
// inlines
//
inline int WebSocketImpl::frameFlags() const
{
	return _frameFlags;
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
