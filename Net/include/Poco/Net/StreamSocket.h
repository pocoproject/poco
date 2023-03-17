//
// StreamSocket.h
//
// Library: Net
// Package: Sockets
// Module:  StreamSocket
//
// Definition of the StreamSocket class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_StreamSocket_INCLUDED
#define Net_StreamSocket_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Socket.h"
#include "Poco/FIFOBuffer.h"


namespace Poco {
namespace Net {


class StreamSocketImpl;


class Net_API StreamSocket: public Socket
	/// This class provides an interface to a
	/// TCP stream socket.
{
public:
	StreamSocket();
		/// Creates an unconnected stream socket.
		///
		/// Before sending or receiving data, the socket
		/// must be connected with a call to connect().

	explicit StreamSocket(const SocketAddress& address);
		/// Creates a stream socket and connects it to
		/// the socket specified by address.

	explicit StreamSocket(SocketAddress::Family family);
		/// Creates an unconnected stream socket
		/// for the given address family.
		///
		/// This is useful if certain socket options
		/// (like send and receive buffer) sizes, that must
		/// be set before connecting the socket, will be
		/// set later on.

	StreamSocket(const Socket& socket);
		/// Creates the StreamSocket with the SocketImpl
		/// from another socket. The SocketImpl must be
		/// a StreamSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	StreamSocket(const StreamSocket& socket);
		/// Creates the StreamSocket with the SocketImpl
		/// from another socket.

	virtual ~StreamSocket();
		/// Destroys the StreamSocket.

	StreamSocket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.

	StreamSocket& operator = (const StreamSocket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.

#if POCO_NEW_STATE_ON_MOVE

	StreamSocket(Socket&& socket);
		/// Creates the StreamSocket with the SocketImpl
		/// from another socket and zeroes the other socket's
		/// SocketImpl.The SocketImpl must be
		/// a StreamSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	StreamSocket(StreamSocket&& socket);
		/// Creates the StreamSocket with the SocketImpl
		/// from another socket and zeroes the other socket's
		/// SocketImpl.

	StreamSocket& operator = (Socket&& socket);
		/// Assignment move operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// zeroes the other socket's SocketImpl.

	StreamSocket& operator = (StreamSocket&& socket);
		/// Assignment move operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// zeroes the other socket's SocketImpl.

#endif //POCO_NEW_STATE_ON_MOVE

	void connect(const SocketAddress& address);
		/// Initializes the socket and establishes a connection to
		/// the TCP server at the given address.
		///
		/// Can also be used for UDP sockets. In this case, no
		/// connection is established. Instead, incoming and outgoing
		/// packets are restricted to the specified address.

	void connect(const SocketAddress& address, const Poco::Timespan& timeout);
		/// Initializes the socket, sets the socket timeout and
		/// establishes a connection to the TCP server at the given address.

	void connectNB(const SocketAddress& address);
		/// Initializes the socket and establishes a connection to
		/// the TCP server at the given address. Prior to opening the
		/// connection the socket is set to nonblocking mode.

	void bind(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket.
		///
		/// TCP clients normally do not bind to a local address,
		/// but in some special advanced cases it may be useful to have
		/// this type of functionality.  (e.g. in multihoming situations
		/// where the traffic will be sent through a particular interface;
		/// or in computer clustered environments with active/standby
		/// servers and it is desired to make the traffic from either
		/// active host present the same source IP address).
		///
		/// Note:  Practical use of client source IP address binding
		///        may require OS networking setup outside the scope of
		///        the Poco library.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.

	void shutdownReceive();
		/// Shuts down the receiving part of the socket connection.

	void shutdownSend();
		/// Shuts down the sending part of the socket connection.

	void shutdown();
		/// Shuts down both the receiving and the sending part
		/// of the socket connection.

	int sendBytes(const void* buffer, int length, int flags = 0);
		/// Sends the contents of the given buffer through
		/// the socket.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// Certain socket implementations may also return a negative
		/// value denoting a certain condition.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for send() like MSG_OOB.

	int sendBytes(const SocketBufVec& buffer, int flags = 0);
		/// Sends the contents of the given buffers through
		/// the socket.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for send() like MSG_OOB.

	int sendBytes(Poco::FIFOBuffer& buffer);
		/// Sends the contents of the given buffer through
		/// the socket. FIFOBuffer has writable/readable transition
		/// notifications which may be enabled to notify the caller when
		/// the buffer transitions between empty, partially full and
		/// full states.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// Certain socket implementations may also return a negative
		/// value denoting a certain condition.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for send() like MSG_OOB.

	int receiveBytes(void* buffer, int length, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		///
		/// Returns the number of bytes received.
		/// A return value of 0 means a graceful shutdown
		/// of the connection from the peer.
		///
		/// Throws a TimeoutException if a receive timeout has
		/// been set and nothing is received within that interval.
		/// Throws a NetException (or a subclass) in case of other errors.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_OOB, MSG_PEEK or MSG_WAITALL.

	int receiveBytes(SocketBufVec& buffer, int flags = 0);
		/// Receives data from the socket and stores it in buffers.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_OOB, MSG_PEEK or MSG_WAITALL.

	int receiveBytes(Poco::Buffer<char>& buffer, int flags = 0, const Poco::Timespan& timeout = 100000);
		/// Receives data from the socket and stores it in buffers.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_OOB, MSG_PEEK or MSG_WAITALL.

	int receiveBytes(Poco::FIFOBuffer& buffer);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received. FIFOBuffer has
		/// writable/readable transition notifications which may be enabled
		/// to notify the caller when the buffer transitions between empty,
		/// partially full and full states.
		///
		/// Returns the number of bytes received.
		/// A return value of 0 means a graceful shutdown
		/// of the connection from the peer.
		///
		/// Throws a TimeoutException if a receive timeout has
		/// been set and nothing is received within that interval.
		/// Throws a NetException (or a subclass) in case of other errors.

	void sendUrgent(unsigned char data);
		/// Sends one byte of urgent data through
		/// the socket.
		///
		/// The data is sent with the MSG_OOB flag.
		///
		/// The preferred way for a socket to receive urgent data
		/// is by enabling the SO_OOBINLINE option.

	StreamSocket(SocketImpl* pImpl);
		/// Creates the Socket and attaches the given SocketImpl.
		/// The socket takes ownership of the SocketImpl.
		///
		/// The SocketImpl must be a StreamSocketImpl, otherwise
		/// an InvalidArgumentException will be thrown.

private:
	enum
	{
		BUFFER_SIZE = 1024
	};

	friend class ServerSocket;
	friend class SocketIOS;
};


} } // namespace Poco::Net


#endif // Net_StreamSocket_INCLUDED
