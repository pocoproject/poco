//
// ICMPSocket.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/ICMPSocket.h#1 $
//
// Library: NetEx
// Package: ICMP
// Module:  ICMPSocket
//
// Definition of the ICMPSocket class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetEx_ICMPSocket_INCLUDED
#define NetEx_ICMPSocket_INCLUDED


#include "Poco/Net/NetEx.h"
#include "Poco/Net/Socket.h"


namespace Poco {
namespace Net {


class NetEx_API ICMPSocket: public Socket
	/// This class provides an interface to an
	/// ICMP client socket.
{
public:
	ICMPSocket(IPAddress::Family family, int dataSize = 48, int ttl = 128, int timeout = 500000);
		/// Creates an unconnected ICMP socket.
		///
		/// The socket will be created for the
		/// given address family.

	ICMPSocket(const Socket& socket);
		/// Creates the ICMPSocket with the SocketImpl
		/// from another socket. The SocketImpl must be
		/// a DatagramSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	~ICMPSocket();
		/// Destroys the ICMPSocket.

	ICMPSocket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.	

	int sendTo(const SocketAddress& address, int flags = 0);
		/// Sends an ICMP request through
		/// the socket to the given address.
		///
		/// Returns the number of bytes sent.

	int receiveFrom(SocketAddress& address, int flags = 0);
		/// Receives data from the socket.
		/// Stores the address of the sender in address.
		///
		/// Returns the time elapsed since the originating 
		/// request was sent.

	int dataSize() const;
		/// Returns the data size in bytes.

	int ttl() const;
		/// Returns the Time-To-Live value.

	int timeout() const;
		/// Returns the socket timeout value.

protected:
	ICMPSocket(SocketImpl* pImpl);
		/// Creates the Socket and attaches the given SocketImpl.
		/// The socket takes owership of the SocketImpl.
		///
		/// The SocketImpl must be a ICMPSocketImpl, otherwise
		/// an InvalidArgumentException will be thrown.

private:
	int _dataSize; 
	int _ttl;
	int _timeout;
};


//
// inlines
//
inline int ICMPSocket::dataSize() const
{
	return _dataSize;
}


inline int ICMPSocket::ttl() const
{
	return _ttl;
}


inline int ICMPSocket::timeout() const
{
	return _timeout;
}


} } // namespace Poco::Net


#endif // NetEx_ICMPSocket_INCLUDED
