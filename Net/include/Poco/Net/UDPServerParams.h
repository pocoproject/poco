//
// UDPServerParams.h
//
// Library: Net
// Package: UDP
// Module:  UDPServerParams
//
// Definition of the UDPServerParams class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPServerParams_INCLUDED
#define Net_UDPServerParams_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class Net_API UDPServerParams
	/// A class encapsulating UDP server parameters.
{
public:
	UDPServerParams(const Poco::Net::SocketAddress& sa,
		int nSockets = 10,
		Poco::Timespan timeout = 250000,
		std::size_t handlerBufListSize = 1000,
		bool notifySender = false,
		int  backlogThreshold = 10);
		/// Creates UDPServerParams.

	~UDPServerParams();
		/// Destroys UDPServerParams.

	const Poco::Net::SocketAddress& address() const;
		/// Returns the server address.

	Poco::Timespan timeout() const;
		/// Returns polling timeout.

	int numberOfSockets() const;
		/// Returns nuber of sockets for the server.

	std::size_t handlerBufListSize() const;
		/// Returns the number of handler buffers allocated
		/// before buffers start being reused.

	bool notifySender() const;
		/// Returns the flag inidcating whether server
		/// should send notifications back to the client.

	int backlogThreshold() const;
		/// Size of mesage or error backlogs at which server
		/// reports backlogs back to the client. Only meaningful
		/// if notifySender() is true.

private:
	UDPServerParams();

	Poco::Net::SocketAddress _sa;
	int                      _nSockets;
	Poco::Timespan           _timeout;
	std::size_t              _handlerBufListSize;
	bool                     _notifySender;
	int                      _backlogThreshold;
};


//
// inlines
//

inline const Poco::Net::SocketAddress& UDPServerParams::address() const
{
	return _sa;
}


inline int UDPServerParams::numberOfSockets() const
{
	return _nSockets;
}


inline Poco::Timespan UDPServerParams::timeout() const
{
	return _timeout;
}


inline std::size_t UDPServerParams::handlerBufListSize() const
{
	return _handlerBufListSize;
}


inline bool UDPServerParams::notifySender() const
{
	return _notifySender;
}


inline int UDPServerParams::backlogThreshold() const
{
	return _backlogThreshold;
}


} } // namespace Poco::Net


#endif // Net_UDPServerParams_INCLUDED
