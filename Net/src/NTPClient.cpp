//
// NTPClient.cpp
//
// Library: Net
// Package: NTP
// Module:  NTPClient
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NTPClient.h"
#include "Poco/Net/NTPPacket.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/NetException.h"


using Poco::TimeoutException;


namespace Poco {
namespace Net {


NTPClient::NTPClient(IPAddress::Family family, int timeout): 
	_family(family), _timeout(timeout)
{
}


NTPClient::~NTPClient()
{
}


int NTPClient::request(const std::string& address) const
{
	SocketAddress addr(address, 123);
	return request(addr);
}


int NTPClient::request(SocketAddress& address) const
{
	SocketAddress sa;
	DatagramSocket ntpSocket(_family);
	ntpSocket.setReceiveTimeout(_timeout);
	ntpSocket.bind(sa);

	SocketAddress returnAddress;

	NTPEventArgs eventArgs(address);

	NTPPacket packet;
	Poco::UInt8 p[1024];
	packet.packet(&p[0]);

	ntpSocket.sendTo(p, 48, address);

	int received = 0;
	Timestamp start;
	while (true)
	{
		try
		{
			int n = ntpSocket.receiveFrom(p, sizeof(p) - 1, sa);
			if (sa != address) // reply mixup, try until timeout ...
			{
				if ((Timestamp() - start) < _timeout) continue;
				break;
			}
			if (n < 48) // NTP packet must have at least 48 bytes
				throw Poco::Net::NTPException("Invalid response received");

			packet.setPacket(p);
			eventArgs.setPacket(packet);
			++received;
			response.notify(this, eventArgs);
			break;
		}
		catch (Poco::TimeoutException &)
		{
			break;
		}
	}

	return received;
}


} } // namespace Poco::Net
