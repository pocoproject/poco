//
// SecureSocketImpl.h
//
// Library: NetSSL_OpenSSL
// Package: SSLSockets
// Module:  SecureSocketImpl
//
// Definition of the SecureSocketImpl class.
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_SecureSocketImpl_INCLUDED
#define NetSSL_SecureSocketImpl_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/Session.h"
#include "Poco/Mutex.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>


namespace Poco::Net {


class HostEntry;


class NetSSL_API SecureSocketImpl
	/// The SocketImpl for SecureStreamSocket.
{
public:
	SecureSocketImpl(Poco::AutoPtr<SocketImpl> pSocketImpl, Context::Ptr pContext);
		/// Creates the SecureSocketImpl using an already
		/// connected stream socket.

	virtual ~SecureSocketImpl();
		/// Destroys the SecureSocketImpl.

	SocketImpl* acceptConnection(SocketAddress& clientAddr);
		/// Get the next completed connection from the
		/// socket's completed connection queue.
		///
		/// If the queue is empty, waits until a connection
		/// request completes.
		///
		/// Returns a new SSL socket for the connection
		/// with the client.
		///
		/// The client socket's address is returned in clientAddr.

	void connect(const SocketAddress& address, bool performHandshake);
		/// Initializes the socket and establishes a secure connection to
		/// the TCP server at the given address.
		///
		/// If performHandshake is true, the SSL handshake is performed immediately
		/// after establishing the connection. Otherwise, the handshake is performed
		/// the first time sendBytes(), receiveBytes() or completeHandshake() is called.

	void connect(const SocketAddress& address, const Poco::Timespan& timeout, bool performHandshake);
		/// Initializes the socket, sets the socket timeout and
		/// establishes a secure connection to the TCP server at the given address.
		///
		/// If performHandshake is true, the SSL handshake is performed immediately
		/// after establishing the connection. Otherwise, the handshake is performed
		/// the first time sendBytes(), receiveBytes() or completeHandshake() is called.
		///
		/// If performHandshake is true, the timeout also limits the whole
		/// handshake, not only the wait for a single read or write.

	void connectNB(const SocketAddress& address);
		/// Initializes the socket and establishes a secure connection to
		/// the TCP server at the given address. Prior to opening the
		/// connection the socket is set to nonblocking mode.

	void bind(const SocketAddress& address, bool reuseAddress = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.

	void bind(const SocketAddress& address, bool reuseAddress, bool reusePort);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. SSL clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.

	void bind6(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false);
		/// Bind a local IPv6 address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// The given address must be an IPv6 address. The
		/// IPPROTO_IPV6/IPV6_V6ONLY option is set on the socket
		/// according to the ipV6Only parameter.
		///
		/// If the library has not been built with IPv6 support,
		/// a Poco::NotImplementedException will be thrown.

	void bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only);
		/// Bind a local IPv6 address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.
		///
		/// The given address must be an IPv6 address. The
		/// IPPROTO_IPV6/IPV6_V6ONLY option is set on the socket
		/// according to the ipV6Only parameter.
		///
		/// If the library has not been built with IPv6 support,
		/// a Poco::NotImplementedException will be thrown.

	void listen(int backlog = 64);
		/// Puts the socket into listening state.
		///
		/// The socket becomes a passive socket that
		/// can accept incoming connection requests.
		///
		/// The backlog argument specifies the maximum
		/// number of connections that can be queued
		/// for this socket.

	int shutdown();
		/// Shuts down the connection by attempting
		/// an orderly SSL shutdown, then actually
		/// shutting down the TCP connection in the
		/// send direction.
		///
		/// On a blocking socket, the send timeout limits sending the own
		/// close_notify alert. If the socket has a receive timeout, the alert
		/// of the peer is then awaited for that time, but at most one second.
		///
		/// Returns 1 if the alert of the peer was received, 0 if only
		/// the own alert was sent, and a negative value
		/// (SecureStreamSocket::ERR_SSL_WANT_WRITE) if the own alert
		/// could not be sent within the send timeout.
		///
		/// If another thread is sending, receiving or completing the
		/// handshake on this socket and does not return within a
		/// short time, or the handshake is not complete, no alert
		/// is sent: the TCP connection is shut down in both
		/// directions, which ends the call of the other thread, and
		/// 0 is returned.
		///
		/// After a fatal TLS or I/O error on the connection, including one
		/// while the alert is sent or awaited, no further alert is sent: only
		/// the send direction of the TCP connection is shut down, and 0 is
		/// returned. A send or receive that timed out is not a fatal error.

	void close();
		/// Close the socket.
		///
		/// Except on Windows, the socket is closed only after a thread
		/// that is sending, receiving or completing the handshake on
		/// it has left that call.

	void abort();
		/// Aborts the connection by closing the
		/// underlying TCP connection. No orderly SSL shutdown
		/// is performed.

	int sendBytes(const void* buffer, int length, int flags = 0);
		/// Sends the contents of the given buffer through
		/// the socket. Any specified flags are ignored.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.

	int receiveBytes(void* buffer, int length, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		///
		/// Returns the number of bytes received.

	int available() const;
		/// Returns the number of bytes available from the
		/// SSL buffer for immediate reading.

	int completeHandshake();
		/// Completes the SSL handshake.
		///
		/// If the SSL connection was the result of an accept(),
		/// the server-side handshake is completed, otherwise
		/// a client-side handshake is performed.
		///
		/// Once the handshake is complete, the peer certificate is verified
		/// as in verifyPeerCertificate(), on the server side against the IP
		/// address of the client. Throws a CertificateValidationException if
		/// the certificate is not acceptable; the handshake then remains
		/// incomplete.
		///
		/// On a blocking socket, the receive timeout, if set, limits the whole
		/// handshake, not only the wait for a single read or write. Throws a
		/// Poco::TimeoutException if the handshake does not complete in time.

	[[nodiscard]] poco_socket_t sockfd();
		/// Returns the underlying socket descriptor.

	[[nodiscard]] X509* peerCertificate() const;
		/// Returns the peer's certificate, or a null pointer if
		/// no certificate is available. The caller is responsible
		/// for releasing the returned reference with X509_free().

	[[nodiscard]] Context::Ptr context() const;
		/// Returns the SSL context used for this socket.

	void verifyPeerCertificate();
		/// Performs post-connect (or post-accept) peer certificate validation,
		/// using the peer host name set with setPeerHostName(), or the peer's
		/// IP address string if no peer host name has been set.

	void verifyPeerCertificate(const std::string& hostName);
		/// Performs post-connect (or post-accept) peer certificate validation
		/// using the given peer host name.

	void setPeerHostName(const std::string& hostName);
		/// Sets the peer host name for certificate validation purposes.

	[[nodiscard]] const std::string& getPeerHostName() const;
		/// Returns the peer host name.

	void setTunneled(bool tunneled);
		/// Sets whether the connection is tunneled, that is, reaches the
		/// peer host through a proxy whose address is the peer address of
		/// the socket. Must be called before the handshake to be effective.

	[[nodiscard]] bool getTunneled() const;
		/// Returns true if the connection is tunneled.

	[[nodiscard]] Session::Ptr currentSession();
		/// Returns the SSL session of the current connection,
		/// for reuse in a future connection (if session caching
		/// is enabled).
		///
		/// If no connection is established, returns null.

	void useSession(Session::Ptr pSession);
		/// Sets the SSL session to use for the next
		/// connection. Setting a previously saved Session
		/// object is necessary to enable session caching.
		///
		/// To remove the currently set session, a null pointer
		/// can be given.
		///
		/// Must be called before connect() to be effective.

	[[nodiscard]] bool sessionWasReused();
		/// Returns true iff a reused session was negotiated during
		/// the handshake.

	[[nodiscard]] SocketImpl* socket();
		/// Returns the underlying SocketImpl.

	[[nodiscard]] const SocketImpl* socket() const;
		/// Returns the underlying SocketImpl.

protected:
	void acceptSSL();
		/// Performs a server-side SSL handshake and certificate verification.

	void connectSSL(bool performHandshake);
		/// Performs a client-side SSL handshake and establishes a secure
		/// connection over an already existing TCP connection.

	long verifyPeerCertificateImpl(const std::string& hostName);
		/// Performs post-connect (or post-accept) peer certificate validation.

	[[nodiscard]] static bool isLocalHost(const std::string& hostName);
		/// Returns true iff the given host name is "localhost"
		/// (in any letter case) or a loopback IP address literal.
		/// The name is not resolved.

	[[nodiscard]] bool mustRetry(int rc);
		/// Returns true if the last operation should be retried,
		/// otherwise false.
		///
		/// In case of an SSL_ERROR_WANT_READ error, and if the socket is
		/// blocking, waits for the underlying socket to become readable.
		///
		/// In case of an SSL_ERROR_WANT_WRITE error, and if the socket is
		/// blocking, waits for the underlying socket to become writable.
		///
		/// Can also throw a Poco::TimeoutException if the socket does
		/// not become readable or writable within the sockets
		/// receive or send timeout.

	int handleError(int rc);
		/// Handles an SSL error by throwing an appropriate exception.

	void reset();
		/// Prepares the socket for re-use.
		///
		/// After closing and resetting a socket, the socket can
		/// be used for a new connection.
		///
		/// Note that simply closing a socket is not sufficient
		/// to be able to re-use it again.

	static int onSessionCreated(SSL* pSSL, SSL_SESSION* pSession);
		/// Callback to handle new session data sent by server.

private:
	using MutexT = Poco::FastMutex;
	using LockT = MutexT::ScopedLock;
	using UnLockT = Poco::ScopedLockWithUnlock<MutexT>;

	SecureSocketImpl(const SecureSocketImpl&);
	SecureSocketImpl& operator = (const SecureSocketImpl&);

	int completeHandshakeLocked();
		/// Completes the SSL handshake as completeHandshake() does,
		/// for a caller that holds the mutex.

	int handshakeWithDeadline(const Poco::Timespan& timeout);
		/// Runs the SSL handshake and returns the result of
		/// SSL_do_handshake().
		///
		/// On a blocking socket and with a non-zero timeout, throws a
		/// Poco::TimeoutException if the handshake does not complete
		/// within the timeout, however the peer spreads its data.

	std::atomic<SSL*> _pSSL;
	Poco::AutoPtr<SocketImpl> _pSocket;
	Context::Ptr _pContext;
	bool _needHandshake;
	bool _ticketPending;
	bool _pendingWrite;
		/// A TLS record of an unfinished SSL_write() is unsent. A close_notify
		/// alert cannot reach the peer before that record.
	bool _fatalError;
		/// A fatal TLS or I/O error has occurred on the connection.
		/// A send or receive that timed out is not one.
	std::string _peerHostName;
	bool _tunneled;
	Session::Ptr _pSession;
	mutable MutexT _mutex;

	friend class SecureStreamSocketImpl;
	friend class Context;
};


//
// inlines
//
inline SocketImpl* SecureSocketImpl::socket()
{
	return _pSocket.get();
}


inline const SocketImpl* SecureSocketImpl::socket() const
{
	return _pSocket.get();
}


inline poco_socket_t SecureSocketImpl::sockfd()
{
	return _pSocket->sockfd();
}


inline Context::Ptr SecureSocketImpl::context() const
{
	return _pContext;
}


inline const std::string& SecureSocketImpl::getPeerHostName() const
{
	return _peerHostName;
}


inline void SecureSocketImpl::setTunneled(bool tunneled)
{
	_tunneled = tunneled;
}


inline bool SecureSocketImpl::getTunneled() const
{
	return _tunneled;
}


} // namespace Poco::Net


#endif // NetSSL_SecureSocketImpl_INCLUDED
