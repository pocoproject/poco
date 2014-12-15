//
// ICMPClientUser_WIN32.cpp
//
// $Id: //poco/1.4/Net/src/ICMPClientUser_WIN32.cpp#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClientUser_WIN32
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/ICMPClientUser_WIN32.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include "Poco/Buffer.h"
#include <sstream>

#include <iphlpapi.h>
#include <icmpapi.h>


namespace Poco {
namespace Net {


ICMPClientUserImpl::ICMPClientUserImpl(IPAddress::Family family): 
	_family(family)
{
}


ICMPClientUserImpl::~ICMPClientUserImpl()
{
}


int ICMPClientUserImpl::ping(const std::string& address, int repeat) const
{
	if (repeat <= 0) return 0;

	SocketAddress addr(address, 0);
	return ping(addr, repeat);
}


class ICMPClientUserImpl_ICMPHandle
{
public:
	ICMPClientUserImpl_ICMPHandle(HANDLE hIcmpFile) : _hIcmpFile(hIcmpFile) {}
	~ICMPClientUserImpl_ICMPHandle() { if (_hIcmpFile != INVALID_HANDLE_VALUE) IcmpCloseHandle(_hIcmpFile); }

	HANDLE hIcmpFile() const { return _hIcmpFile; }
private:
	HANDLE _hIcmpFile;
};


int ICMPClientUserImpl::ping(SocketAddress& address, int repeat) const
{
	if (repeat <= 0) return 0;

    char SendData[] = "Data Buffer";
    DWORD dwRetVal = 0;
    DWORD dwError = 0;
    DWORD ReplySize = 0;

	ICMPEventArgs eventArgs(address, repeat, sizeof(SendData), 128);
	pingBegin.notify(this, eventArgs);

	for (int i = 0; i < repeat; ++i)
	{
		++eventArgs;

		ICMPClientUserImpl_ICMPHandle hIcmpFile(IcmpCreateFile());
		if (hIcmpFile.hIcmpFile() == INVALID_HANDLE_VALUE)
		{
			eventArgs.setError(i, Poco::format("IcmpCreatefile returned error: %?d", GetLastError()));
			continue;
		}

		// Allocate space for at a single reply
		ReplySize = sizeof (ICMP_ECHO_REPLY) + sizeof (SendData) + 8;
		Poco::Buffer<char> ReplyBuffer(0);
		try
		{
			ReplyBuffer.resize(ReplySize);
		}
		catch (std::exception &e)
		{
			eventArgs.setError(i, Poco::format("Unable to allocate memory for reply buffer: %s", std::string(e.what())));
			continue;
		}
		if (ReplyBuffer == NULL) 
		{
			eventArgs.setError(i, "Unable to allocate memory for reply buffer");
			continue;
		}

		dwRetVal = IcmpSendEcho2(hIcmpFile.hIcmpFile(), NULL, NULL, NULL,
				*((IPAddr*)address.host().addr()), SendData, sizeof (SendData), NULL,
								 ReplyBuffer.begin(), ReplySize, 1000);
		if (dwRetVal != 0) 
		{
			PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY) ReplyBuffer.begin();
			struct in_addr ReplyAddr;
			ReplyAddr.S_un.S_addr = pEchoReply->Address;

			eventArgs.setReplyTime(i, pEchoReply->RoundTripTime);

			switch (pEchoReply->Status) 
			{
			case IP_DEST_HOST_UNREACHABLE:
				eventArgs.setError(i, "Destination host was unreachable");
				pingError.notify(this, eventArgs);
				break;
			case IP_DEST_NET_UNREACHABLE:
				eventArgs.setError(i, "Destination Network was unreachable");
				pingError.notify(this, eventArgs);
				break;
			case IP_REQ_TIMED_OUT:
				eventArgs.setError(i, "Request timed out");
				pingError.notify(this, eventArgs);
				break;
			default:
				pingReply.notify(this, eventArgs);
			}
		} else {
			dwError = GetLastError();
			switch (dwError) {
			case IP_BUF_TOO_SMALL:
				eventArgs.setError(i, "ReplyBufferSize to small");
				pingError.notify(this, eventArgs);
				break;
			case IP_REQ_TIMED_OUT:
				eventArgs.setError(i, "Request timed out");
				pingError.notify(this, eventArgs);
				break;
			default:
				eventArgs.setError(i, Poco::format("Extended error returned: %?d", dwError));
				pingError.notify(this, eventArgs);
				break;
			}
		}
	}

	pingEnd.notify(this, eventArgs);
	return eventArgs.received();
}


} } // namespace Poco::Net
