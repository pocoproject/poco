//
// ICMPv4PacketImpl.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPv4PacketImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/ICMPv4PacketImpl.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/NumberFormatter.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#endif
#include <sstream>
#include <cstring>


using Poco::InvalidArgumentException;
using Poco::Timestamp;
using Poco::Timespan;
using Poco::NumberFormatter;
using Poco::UInt8;
using Poco::UInt16;
using Poco::Int32;


namespace Poco::Net {


const std::string ICMPv4PacketImpl::MESSAGE_TYPE[] =
{
	"Echo Reply",
	"ICMP 1",
	"ICMP 2",
	"Dest Unreachable",
	"Source Quench",
	"Redirect",
	"ICMP 6",
	"ICMP 7",
	"Echo",
	"ICMP 9",
	"ICMP 10",
	"Time Exceeded",
	"Parameter Problem",
	"Timestamp",
	"Timestamp Reply",
	"Info Request",
	"Info Reply",
	"Unknown type"
};


const std::string ICMPv4PacketImpl::DESTINATION_UNREACHABLE_CODE[] =
{
	"Net unreachable",
	"Host unreachable",
	"Protocol unreachable",
	"Port unreachable",
	"Fragmentation needed and DF set",
	"Source route failed",
	"Unknown code"
};


const std::string ICMPv4PacketImpl::REDIRECT_MESSAGE_CODE[] =
{
	"Redirect datagrams for the network",
	"Redirect datagrams for the host",
	"Redirect datagrams for the type of service and network",
	"Redirect datagrams for the type of service and host",
	"Unknown code"
};


const std::string ICMPv4PacketImpl::TIME_EXCEEDED_CODE[] =
{
	"Time to live exceeded in transit",
	"Fragment reassembly time exceeded",
	"Unknown code"
};


const std::string ICMPv4PacketImpl::PARAMETER_PROBLEM_CODE[] =
{
	"Pointer indicates the error",
	"Unknown code"
};


ICMPv4PacketImpl::ICMPv4PacketImpl(int dataSize)
	: ICMPPacketImpl(dataSize),
	_seq(0)
{
	initPacket();
}


ICMPv4PacketImpl::~ICMPv4PacketImpl()
{
}


int ICMPv4PacketImpl::packetSize() const
{
	return getDataSize() + sizeof(Header);
}


void ICMPv4PacketImpl::initPacket()
{
	if (_seq >= MAX_SEQ_VALUE) resetSequence();

	Header* icp = (Header*) packet(false);
	icp->type     = ECHO_REQUEST;
	icp->code     = 0;
	icp->checksum = 0;
	icp->seq      = ++_seq;
#if defined(POCO_VXWORKS)
	icp->id       = 0;
#else
	icp->id       = static_cast<UInt16>(Poco::Process::id());
#endif

	const struct timeval now = time();
	std::memcpy(icp + 1, &now, sizeof(now));

	icp->checksum = checksum((UInt16*) icp, getDataSize() + sizeof(Header));
}


struct timeval ICMPv4PacketImpl::time(Poco::UInt8* buffer, int length) const
{
	struct timeval tv;

	if (nullptr == buffer || 0 == length)
	{
		Timespan value(Timestamp().epochMicroseconds());
		tv.tv_sec  = (long) value.totalSeconds();
		tv.tv_usec = (long) value.useconds();
	}
	else
	{
		// The time value follows the IP header, which leaves it without the
		// alignment of struct timeval: copy it from the bytes.
		const Poco::UInt8* pData = data(buffer, length);
		if (pData == nullptr || static_cast<std::size_t>(pData - buffer) + sizeof(tv) > static_cast<std::size_t>(length))
			throw InvalidArgumentException("Invalid packet.");
		std::memcpy(&tv, pData, sizeof(tv));
	}
	return tv;
}


ICMPv4PacketImpl::Header* ICMPv4PacketImpl::header(Poco::UInt8* buffer, int length) const
{
	poco_check_ptr (buffer);

	int offset = (buffer[0] & 0x0F) * 4;
	if (static_cast<std::size_t>(offset) + sizeof(Header) > static_cast<std::size_t>(length)) return nullptr;

	buffer += offset;
	return reinterpret_cast<Header*>(buffer);
}


Poco::UInt8* ICMPv4PacketImpl::data(Poco::UInt8* buffer, int length) const
{
	Header* icp = header(buffer, length);
	return (icp != nullptr) ? reinterpret_cast<Poco::UInt8*>(icp + 1) : nullptr;
}


bool ICMPv4PacketImpl::validReplyID(Poco::UInt8* buffer, int length) const
{
	Header *icp = header(buffer, length);
#if defined(POCO_VXWORKS)
	return icp && icp->id == 0;
#else
	return icp && (static_cast<Poco::UInt16>(Process::id()) == icp->id);
#endif
}


std::string ICMPv4PacketImpl::errorDescription(unsigned char* buffer, int length, int& type, int& code)
{
	Header *icp = header(buffer, length);

	if (!icp) return "Invalid header.";
	if (ECHO_REPLY == icp->type) return std::string(); // not an error

	UInt8 pointer = 0;
	if (PARAMETER_PROBLEM == icp->type)
	{
		UInt8 mask = 0x00FF;
		pointer = icp->id & mask;
	}

	type = icp->type;
	MessageType msgType = static_cast<MessageType>(type);
	code = static_cast<int>(icp->code);
	std::ostringstream err;

	switch (msgType)
	{
	case DESTINATION_UNREACHABLE_TYPE:
		// lower-bound check is defensive (enum values may change)
		if (code >= static_cast<int>(NET_UNREACHABLE) && code < static_cast<int>(DESTINATION_UNREACHABLE_UNKNOWN))
			err << DESTINATION_UNREACHABLE_CODE[code];
		else
			err << DESTINATION_UNREACHABLE_CODE[DESTINATION_UNREACHABLE_UNKNOWN];
		break;

	case SOURCE_QUENCH_TYPE:
		err << "Source quench";
		break;

	case REDIRECT_MESSAGE_TYPE:
		// lower-bound check is defensive (enum values may change)
		if (code >= static_cast<int>(REDIRECT_NETWORK) && code < static_cast<int>(REDIRECT_MESSAGE_UNKNOWN))
			err << REDIRECT_MESSAGE_CODE[code];
		else
			err << REDIRECT_MESSAGE_CODE[REDIRECT_MESSAGE_UNKNOWN];
		break;

	case TIME_EXCEEDED_TYPE:
		// lower-bound check is defensive (enum values may change)
		if (code >= static_cast<int>(TIME_TO_LIVE) && code < static_cast<int>(TIME_EXCEEDED_UNKNOWN))
			err << TIME_EXCEEDED_CODE[code];
		else
			err << TIME_EXCEEDED_CODE[TIME_EXCEEDED_UNKNOWN];
		break;

	case PARAMETER_PROBLEM_TYPE:
		if (static_cast<int>(POINTER_INDICATES_THE_ERROR) != code)
			code = static_cast<int>(PARAMETER_PROBLEM_UNKNOWN);
		err << PARAMETER_PROBLEM_CODE[code] << ": error in octet #" << pointer;
		break;

	default:
		err << "Unknown type.";
		break;
	}

	return err.str();
}

std::string ICMPv4PacketImpl::typeDescription(int typeId)
{
	poco_assert (typeId >= ECHO_REPLY && typeId < MESSAGE_TYPE_LENGTH);

	return MESSAGE_TYPE[typeId];
}


} // namespace Poco::Net
