//
// HTTPSession.h
//
// Library: Net
// Package: HTTP
// Module:  HTTPSession
//
// Definition of the HTTPSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPSession_INCLUDED
#define Net_HTTPSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Timespan.h"
#include "Poco/Clock.h"
#include "Poco/Exception.h"
#include "Poco/Any.h"
#include "Poco/Buffer.h"
#include <ios>
#include <optional>


namespace Poco::Net {


class Net_API HTTPSession
	/// HTTPSession implements basic HTTP session management
	/// for both HTTP clients and HTTP servers.
	///
	/// HTTPSession implements buffering for HTTP connections, as well
	/// as specific support for the various HTTP stream classes.
	///
	/// This class can not be instantiated. HTTPClientSession or
	/// HTTPServerSession must be used instead.
{
public:
	void setKeepAlive(bool keepAlive);
		/// Sets the keep-alive flag for this session.
		///
		/// If the keep-alive flag is enabled, persistent
		/// HTTP/1.1 connections are supported.

	[[nodiscard]] bool getKeepAlive() const;
		/// Returns the value of the keep-alive flag for
		/// this session.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the timeout for the HTTP session.
		///
		/// If the session is connected, the send and receive
		/// timeouts are also applied to the socket; see
		/// setSendTimeout() for the exceptions.

	void setTimeout(const Poco::Timespan& connectionTimeout, const Poco::Timespan& sendTimeout, const Poco::Timespan& receiveTimeout);
		/// Sets different timeouts for the HTTP session.
		///
		/// If the session is connected, the send and receive
		/// timeouts are also applied to the socket; see
		/// setSendTimeout() for the exceptions. If the send
		/// timeout is rejected, the receive timeout is still
		/// applied and the send timeout's exception is rethrown.

	[[nodiscard]] Poco::Timespan getTimeout() const;
		/// Returns the timeout for the HTTP session.

	void setConnectTimeout(const Poco::Timespan& timeout);
		/// Sets the connect timeout.

	[[nodiscard]] Poco::Timespan getConnectTimeout() const;
		/// Gets the connect timeout.

	void setSendTimeout(const Poco::Timespan& timeout);
		/// Sets the send timeout.
		///
		/// If the session is connected, the timeout is also
		/// applied to the socket. Throws an InvalidArgumentException
		/// or an IOException if the socket rejects the value.
		/// A NetException is ignored: the socket has failed,
		/// and the next send or receive reports it.

	[[nodiscard]] Poco::Timespan getSendTimeout() const;
		/// Gets the send timeout.

	void setReceiveTimeout(const Poco::Timespan& timeout);
		/// Sets the receive timeout.
		///
		/// If the session is connected, the timeout is also
		/// applied to the socket. Throws an InvalidArgumentException
		/// or an IOException if the socket rejects the value.
		/// A NetException is ignored: the socket has failed,
		/// and the next send or receive reports it.

	[[nodiscard]] Poco::Timespan getReceiveTimeout() const;
		/// Gets the receive timeout.

	[[nodiscard]] bool connected() const;
		/// Returns true if the underlying socket is connected.

	virtual void abort();
		/// Aborts a session in progress by shutting down
		/// and closing the underlying socket.

	[[nodiscard]] const Poco::Exception* networkException() const;
		/// If sending or receiving data over the underlying
		/// socket connection resulted in an exception, a
		/// pointer to this exception is returned.
		/// A malformed body is reported as a MessageException; a body
		/// cut short is reported as a MessageException or as the network
		/// error that ended it.
		///
		/// Otherwise, NULL is returned.

	void attachSessionData(const Poco::Any& data);
		/// Allows to attach an application-specific data
		/// item to the session.
		///
		/// On the server side, this can be used to manage
		/// data that must be maintained over the entire
		/// lifetime of a persistent connection (that is,
		/// multiple requests sent over the same connection).

	[[nodiscard]] const Poco::Any& sessionData() const;
		/// Returns the data attached with attachSessionData(),
		/// or an empty Poco::Any if no user data has been
		/// attached.

	enum
	{
		HTTP_PORT = 80
	};

	StreamSocket detachSocket();
		/// Detaches the socket from the session.
		///
		/// The socket is returned, and a new, uninitialized socket is
		/// attached to the session.

	[[nodiscard]] StreamSocket& socket();
		/// Returns a reference to the underlying socket.

	void drainBuffer(Poco::Buffer<char>& buffer);
		/// Copies all bytes remaining in the internal buffer to the
		/// given Poco::Buffer, resizing it as necessary.
		///
		/// This is usually used together with detachSocket() to
		/// obtain any data already read from the socket, but not
		/// yet processed.

	[[nodiscard]] const MessageHeader& requestTrailer() const;
		/// Returns the trailer for a request sent using chunked
		/// transfer encoding. This method must be called after
		/// the entire content of the message has been read.

	[[nodiscard]] MessageHeader& requestTrailer();
		/// Returns the trailer for a request sent using chunked
		/// transfer encoding. The trailer fields to be sent must be set
		/// before the request body has been fully written.

	[[nodiscard]] const MessageHeader& responseTrailer() const;
		/// Returns the trailer for a response sent using chunked
		/// transfer encoding. This method must be called after
		/// the entire content of the message has been read.

	[[nodiscard]] MessageHeader& responseTrailer();
		/// Returns the trailer for a response sent using chunked
		/// transfer encoding. The trailer fields to be sent must be set
		/// before the response body has been fully written.

	[[nodiscard]] virtual SocketAddress clientAddress() = 0;
	[[nodiscard]] virtual SocketAddress serverAddress() = 0;
protected:
	HTTPSession();
		/// Creates a HTTP session using an
		/// unconnected stream socket.

	HTTPSession(const StreamSocket& socket);
		/// Creates a HTTP session using the
		/// given socket. The session takes ownership
		/// of the socket and closes it when it's no
		/// longer used.

	HTTPSession(const StreamSocket& socket, bool keepAlive);
		/// Creates a HTTP session using the
		/// given socket. The session takes ownership
		/// of the socket and closes it when it's no
		/// longer used.

	virtual ~HTTPSession();
		/// Destroys the HTTPSession and closes the
		/// underlying socket.

	[[nodiscard]] virtual int get();
		/// Returns the next byte in the buffer.
		/// Reads more data from the socket if there are
		/// no bytes left in the buffer.

	[[nodiscard]] virtual int peek();
		/// Peeks at the next character in the buffer.
		/// Reads more data from the socket if there are
		/// no bytes left in the buffer.

	virtual int read(char* buffer, std::streamsize length);
		/// Reads up to length bytes.
		///
		/// If there is data in the buffer, this data
		/// is returned. Otherwise, data is read from
		/// the socket to avoid unnecessary buffering.

	virtual int write(const char* buffer, std::streamsize length);
		/// Writes data to the socket.

	int receive(char* buffer, int length);
		/// Reads up to length bytes.

	int buffered() const;
		/// Returns the number of bytes in the buffer.

	[[nodiscard]] bool dataPending() const;
		/// Returns true if the buffer holds data, or if the socket has data
		/// to read or has been closed by the peer. Does not block.

	void refill();
		/// Refills the internal buffer.
		///
		/// A receive that returns no data, such as a non-blocking
		/// receive with no data available, leaves the buffer empty.

	virtual void connect(const SocketAddress& address);
		/// Connects the underlying socket to the given address
		/// and sets the socket's send and receive timeouts.

	void connect(const SocketAddress& targetAddress, const SocketAddress& sourceAddress);
		/// Binds the underlying socket to the source address
		/// and connects to the targetAddress.

	void attachSocket(const StreamSocket& socket);
		/// Attaches a socket to the session, replacing the
		/// previously attached socket.
		///
		/// Data received from the previous socket and not yet
		/// read from the session is discarded.

	void close();
		/// Closes the underlying socket.

	void setException(const Poco::Exception& exc);
		/// Stores a clone of the exception.

	void clearException();
		/// Clears the stored exception.

	void setReadDeadline(const Poco::Clock& deadline);
		/// Sets a deadline for reading from the socket. A read
		/// waits for data at most until the deadline, and a read
		/// after it fails with a TimeoutException. Over TLS, the
		/// deadline is checked between records; within a record,
		/// each read waits up to the receive timeout.

	void clearReadDeadline();
		/// Removes the deadline set with setReadDeadline().

private:
	enum
	{
		HTTP_DEFAULT_TIMEOUT = 60000000,
		HTTP_DEFAULT_CONNECTION_TIMEOUT = 30000000
	};

	HTTPSession(const HTTPSession&);
	HTTPSession& operator = (const HTTPSession&);

	StreamSocket     _socket;
	char*            _pBuffer;
	char*            _pCurrent;
	char*            _pEnd;
	bool             _keepAlive;
	Poco::Timespan   _connectionTimeout;
	Poco::Timespan   _receiveTimeout;
	Poco::Timespan   _sendTimeout;
	std::optional<Poco::Clock> _readDeadline;
	Poco::Exception* _pException;
	MessageHeader    _requestTrailer;
	MessageHeader    _responseTrailer;
	Poco::Any        _data;

	friend class HTTPStreamBuf;
	friend class HTTPHeaderStreamBuf;
	friend class HTTPFixedLengthStreamBuf;
	friend class HTTPChunkedStreamBuf;
	friend class HTTPServerRequestImpl;
	friend class HTTPServerConnection;
};


//
// inlines
//
inline bool HTTPSession::getKeepAlive() const
{
	return _keepAlive;
}


inline Poco::Timespan HTTPSession::getTimeout() const
{
	return _receiveTimeout;
}


inline void HTTPSession::setConnectTimeout(const Poco::Timespan& timeout)
{
	_connectionTimeout = timeout;
}


inline Poco::Timespan HTTPSession::getConnectTimeout() const
{
	return _connectionTimeout;
}


inline Poco::Timespan HTTPSession::getSendTimeout() const
{
	return _sendTimeout;
}


inline Poco::Timespan HTTPSession::getReceiveTimeout() const
{
	return _receiveTimeout;
}


inline StreamSocket& HTTPSession::socket()
{
	return _socket;
}


inline const Poco::Exception* HTTPSession::networkException() const
{
	return _pException;
}


inline int HTTPSession::buffered() const
{
	return static_cast<int>(_pEnd - _pCurrent);
}


inline const Poco::Any& HTTPSession::sessionData() const
{
	return _data;
}


inline const MessageHeader& HTTPSession::requestTrailer() const
{
	return _requestTrailer;
}


inline MessageHeader& HTTPSession::requestTrailer()
{
	return _requestTrailer;
}


inline const MessageHeader& HTTPSession::responseTrailer() const
{
	return _responseTrailer;
}


inline MessageHeader& HTTPSession::responseTrailer()
{
	return _responseTrailer;
}


} // namespace Poco::Net


#endif // Net_HTTPSession_INCLUDED
