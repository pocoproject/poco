//
// UDPClient.h
//
// Library: Net
// Package: UDP
// Module:  UDPClient
//
// Definition of the UDPClient class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPClient_INCLUDED
#define Net_UDPClient_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Timespan.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"


namespace Poco {
namespace Net {


class Net_API UDPClient : public Poco::Runnable
	/// UDP client can either send, or send/receive UDP packets.
	/// The mode of operation is specified at construction time.
	/// If receiving functionality is enabled, it will run in a
	/// separate thread.
	/// This class is written as a "companion" to Poco::Net::UDPServer.
	/// For other servers, inherit from this class and override the
	/// handleResponse(char*, int) virtual member.
{
public:
	UDPClient(const std::string& address, Poco::UInt16 port, bool listen = false);
		/// Creates UDP client and connects it to specified address/port.
		/// If listen is true, a thread is launched where client can receive
		/// responses rom the server.

	virtual ~UDPClient();
		/// Destroys UDPClient.

	void run();
		/// Runs listener (typically invoked internally, in separate thread).

	SocketAddress address() const;
		/// Returns client address.

	SocketAddress peerAddress() const;
		/// Returns server address.

	int send(void* data, int length);
		/// Sends data.

	int send(const SocketBufVec& vec);
		/// Sends data.

	virtual int handleResponse(char* buffer, int length);
		/// Handles responses from UDP server. For non-POCO UDP servers,
		/// this function should be overridden in inheriting class.

	void setOption(int opt, int val);
		/// Sets socket option.

	int getOption(int opt);
		/// Returns socket option.

	void stop();
		/// Stops the server reply receiving thread, if running.

	int dataBacklog() const;
		/// Returns current server data backlog.

	int errorBacklog();
		/// Returns current server error backlog.

private:
	DatagramSocket      _socket;
	SocketAddress       _address;
	Thread*             _pThread;
	std::atomic<bool>   _stop;
	Poco::AtomicCounter _dataBacklog;
	Poco::AtomicCounter _errorBacklog;
};


//
// inlines
//

inline SocketAddress UDPClient::address() const
{
	return _socket.address();
}


inline SocketAddress UDPClient::peerAddress() const
{
	return _address;
}


inline int UDPClient::send(void* data, int length)
{
	return _socket.sendBytes(data, length);
}


inline int UDPClient::send(const SocketBufVec& vec)
{
	return _socket.sendBytes(vec);
}


inline void UDPClient::setOption(int opt, int val)
{
	_socket.setOption(SOL_SOCKET, opt, val);
}


inline int UDPClient::getOption(int opt)
{
	int val = 0;
	_socket.getOption(SOL_SOCKET, opt, val);
	return val;
}


inline void UDPClient::stop()
{
	_stop = true;
}


inline int UDPClient::dataBacklog() const
{
	return _dataBacklog;
}


inline int UDPClient::errorBacklog()
{
	return _errorBacklog;
}


} } // namespace Poco::Net


#endif // Net_UDPClient_INCLUDED
