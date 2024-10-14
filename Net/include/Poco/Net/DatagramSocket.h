//
// DatagramSocket.h
//
// Library: Net
// Package: Sockets
// Module:  DatagramSocket
//
// Definition of the DatagramSocket class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_DatagramSocket_INCLUDED
#define Net_DatagramSocket_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Socket.h"
#include "Poco/Buffer.h"


namespace Poco {
namespace Net {


class Net_API DatagramSocket: public Socket
	/// This class provides an interface to an
	/// UDP stream socket.
{
public:
	DatagramSocket();
		/// Creates an unconnected, unbound datagram socket.
		///
		/// Before the datagram socket can be used, bind(),
		/// bind6() or connect() must be called.
		///
		/// Notice: The behavior of this constructor has changed
		/// in release 2.0. Previously, the constructor created
		/// an unbound IPv4 datagram socket.

	explicit DatagramSocket(SocketAddress::Family family);
		/// Creates an unconnected datagram socket.
		///
		/// The socket will be created for the
		/// given address family.

	DatagramSocket(const SocketAddress& address, bool reuseAddress, bool reusePort = false, bool ipV6Only = false);
		/// Creates a datagram socket and binds it
		/// to the given address.
		///
		/// Depending on the address family, the socket
		/// will be either an IPv4 or an IPv6 socket.
		/// If ipV6Only is true, socket will be bound
		/// to the IPv6 address only.

	DatagramSocket(const Socket& socket);
		/// Creates the DatagramSocket with the SocketImpl
		/// from another socket. The SocketImpl must be
		/// a DatagramSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	DatagramSocket(const DatagramSocket& socket);
		/// Creates the DatagramSocket with the SocketImpl
		/// from another socket.

	~DatagramSocket();
		/// Destroys the DatagramSocket.

	DatagramSocket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.

#if POCO_NEW_STATE_ON_MOVE

	DatagramSocket(Socket&& socket);
		/// Creates the DatagramSocket with the SocketImpl
		/// from another socket and zeroes the other socket's
		/// SocketImpl.The SocketImpl must be
		/// a DatagramSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	DatagramSocket(DatagramSocket&& socket);
		/// Creates the DatagramSocket with the SocketImpl
		/// from another socket and zeroes the other socket's
		/// SocketImpl.

	DatagramSocket& operator = (Socket&& socket);
		/// Assignment move operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// zeroes the other socket's SocketImpl.

	DatagramSocket& operator = (DatagramSocket&& socket);
		/// Assignment move operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// zeroes the other socket's SocketImpl.

#endif // POCO_NEW_STATE_ON_MOVE

	DatagramSocket& operator = (const DatagramSocket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.

	void connect(const SocketAddress& address);
		/// Restricts incoming and outgoing
		/// packets to the specified address.
		///
		/// Calls to connect() cannot come before calls to bind().

	void bind(const SocketAddress& address, bool reuseAddress = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// Calls to connect() cannot come before calls to bind().

	void bind(const SocketAddress& address, bool reuseAddress, bool reusePort);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.
		///
		/// Calls to connect() cannot come before calls to bind().

	void bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.
		///
		/// Sets the IPV6_V6ONLY socket option in accordance with
		/// the supplied ipV6Only value.
		///
		/// Calls to connect() cannot come before calls to bind().

	int sendBytes(const void* buffer, int length, int flags = 0);
		/// Sends the contents of the given buffer through
		/// the socket.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for send() like MSG_DONTROUTE.

	int sendBytes(const SocketBufVec& buffer, int flags = 0);
		/// Sends the contents of the given buffers through
		/// the socket.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for send() like MSG_DONTROUTE.

	int receiveBytes(void* buffer, int length, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_PEEK.

	int receiveBytes(SocketBufVec& buffer, int flags = 0);
		/// Receives data from the socket and stores it in buffers.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_PEEK.

	int receiveBytes(Poco::Buffer<char>& buffer, int flags = 0, const Poco::Timespan& timeout = 100000);
		/// Receives data from the socket and stores it in buffers.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recv() like MSG_PEEK.

	int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0);
		/// Sends the contents of the given buffer through
		/// the socket to the given address.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for sendto() like MSG_DONTROUTE.

	int sendTo(const SocketBufVec& buffers, const SocketAddress& address, int flags = 0);
		/// Sends the contents of the given buffers through
		/// the socket to the given address.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for sendto() like MSG_DONTROUTE.

	int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		/// Stores the address of the sender in address.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recvfrom() like MSG_PEEK.

	int receiveFrom(void* buffer, int length, struct sockaddr** ppSA, poco_socklen_t** ppSALen, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		/// Stores the native address of the sender in
		/// ppSA, and the length of native address in ppSALen.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recvfrom() like MSG_PEEK.

	int receiveFrom(SocketBufVec& buffers, SocketAddress& address, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffers. Up to total length of all buffers
		/// are received.
		/// Stores the address of the sender in address.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recvfrom() like MSG_PEEK.

	int receiveFrom(SocketBufVec& buffers, struct sockaddr** ppSA, poco_socklen_t** ppSALen, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffers.
		/// Stores the native address of the sender in
		/// ppSA, and the length of native address in ppSALen.
		///
		/// Returns the number of bytes received.
		///
		/// The flags parameter can be used to pass system-defined flags
		/// for recvfrom() like MSG_PEEK.

	void setBroadcast(bool flag);
		/// Sets the value of the SO_BROADCAST socket option.
		///
		/// Setting this flag allows sending datagrams to
		/// the broadcast address.

	bool getBroadcast() const;
		/// Returns the value of the SO_BROADCAST socket option.

protected:
	DatagramSocket(SocketImpl* pImpl);
		/// Creates the Socket and attaches the given SocketImpl.
		/// The socket takes ownership of the SocketImpl.
		///
		/// The SocketImpl must be a DatagramSocketImpl, otherwise
		/// an InvalidArgumentException will be thrown.
};


//
// inlines
//
inline void DatagramSocket::setBroadcast(bool flag)
{
	impl()->setBroadcast(flag);
}


inline bool DatagramSocket::getBroadcast() const
{
	return impl()->getBroadcast();
}


} } // namespace Poco::Net


#endif // Net_DatagramSocket_INCLUDED
