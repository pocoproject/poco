//
// SocketFactory.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SocketFactory
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/SocketFactory.h"
#include "Poco/Net/SocketAddress.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


SocketFactory::SocketFactory()
{
}

	
SocketFactory::~SocketFactory()
{
}


Poco::Net::StreamSocket SocketFactory::createSocket(const Poco::URI& uri)
{
	std::string auth;
	Poco::URI::decode(uri.getAuthority(), auth);
	Poco::Net::SocketAddress addr(auth);
	Poco::Net::StreamSocket ss(addr);
	
#if defined(POCO_OS_FAMILY_UNIX)
	if (addr.family() != Poco::Net::SocketAddress::UNIX_LOCAL)
#endif
	{
		ss.setNoDelay(true);
	}
	return ss;
}


} } } // namespace Poco::RemotingNG::TCP
