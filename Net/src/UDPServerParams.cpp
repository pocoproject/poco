//
// UDPServerParams.cpp
//
// Library: Net
// Package: UDP
// Module:  UDPServerParams
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/UDPServerParams.h"


namespace Poco {
namespace Net {


UDPServerParams::UDPServerParams(const Poco::Net::SocketAddress& sa,
	int nSockets,
	Poco::Timespan timeout,
	std::size_t handlerBufListSize,
	bool notifySender,
	int  backlogThreshold): _sa(sa),
		_nSockets(nSockets),
		_timeout(timeout),
		_handlerBufListSize(handlerBufListSize),
		_notifySender(notifySender),
		_backlogThreshold(backlogThreshold)
{
}


UDPServerParams::~UDPServerParams()
{
}


} } // namespace Poco::Net
