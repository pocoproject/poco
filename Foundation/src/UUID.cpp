//
// UUID.cpp
//
// $Id: //poco/1.4/Foundation/src/UUID.cpp#2 $
//
// Library: Foundation
// Package: UUID
// Module:  UUID
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/UUID.h"
#include "Poco/ByteOrder.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cstring>


namespace Poco {


UUID::UUID(): 
	_timeLow(0), 
	_timeMid(0),
	_timeHiAndVersion(0),
	_clockSeq(0)
{
	std::memset(_node, 0, sizeof(_node));
}


UUID::UUID(const UUID& uuid):
	_timeLow(uuid._timeLow), 
	_timeMid(uuid._timeMid),
	_timeHiAndVersion(uuid._timeHiAndVersion),
	_clockSeq(uuid._clockSeq)
{
	std::memcpy(_node, uuid._node, sizeof(_node));
}


UUID::UUID(const std::string& uuid)
{
	parse(uuid);
}

	
UUID::UUID(const char* uuid)
{
	poco_check_ptr (uuid);
	parse(std::string(uuid));
}


UUID::UUID(UInt32 timeLow, UInt32 timeMid, UInt32 timeHiAndVersion, UInt16 clockSeq, UInt8 node[]):
	_timeLow(timeLow),
	_timeMid(timeMid),
	_timeHiAndVersion(timeHiAndVersion),
	_clockSeq(clockSeq)
{
	std::memcpy(_node, node, sizeof(_node));
}


UUID::UUID(const char* bytes, Version version)
{
	UInt32 i32;
	UInt16 i16;
	std::memcpy(&i32, bytes, sizeof(i32));
	_timeLow = ByteOrder::fromNetwork(i32);
	bytes += sizeof(i32);
	std::memcpy(&i16, bytes, sizeof(i16));
	_timeMid = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(&i16, bytes, sizeof(i16));
	_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(&i16, bytes, sizeof(i16));
	_clockSeq = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(_node, bytes, sizeof(_node));

	_timeHiAndVersion &= 0x0FFF;
	_timeHiAndVersion |= (version << 12);
	_clockSeq &= 0x3FFF;
	_clockSeq |= 0x8000;
}


UUID::~UUID()
{
}


UUID& UUID::operator = (const UUID& uuid)
{
	if (&uuid != this)
	{
		_timeLow = uuid._timeLow;
		_timeMid = uuid._timeMid;
		_timeHiAndVersion = uuid._timeHiAndVersion;
		_clockSeq         = uuid._clockSeq;
		std::memcpy(_node, uuid._node, sizeof(_node));
	}
	return *this;
}


void UUID::swap(UUID& uuid)
{
	std::swap(_timeLow, uuid._timeLow);
	std::swap(_timeMid, uuid._timeMid);
	std::swap(_timeHiAndVersion, uuid._timeHiAndVersion);
	std::swap(_clockSeq, uuid._clockSeq);
	std::swap_ranges(_node, _node + 6, &uuid._node[0]);
}


void UUID::parse(const std::string& uuid)
{
	if (!tryParse(uuid))
		throw SyntaxException(uuid);
}	

bool UUID::tryParse(const std::string& uuid)
{
	if (uuid.size() < 32)
		return false;

	bool haveHyphens = false;
	if (uuid[8] == '-' && uuid[13] == '-' && uuid[18] == '-' && uuid[23] == '-')
	{
		if (uuid.size() >= 36) 
			haveHyphens = true;
		else
			return false;
	}
	
	std::string::const_iterator it = uuid.begin();
	_timeLow = 0;
	for (int i = 0; i < 8; ++i)
	{
		_timeLow = (_timeLow << 4) | nibble(*it++);
	}
	if (haveHyphens) ++it;
	_timeMid = 0;
	for (int i = 0; i < 4; ++i)
	{
		_timeMid = (_timeMid << 4) | nibble(*it++);
	}
	if (haveHyphens) ++it;
	_timeHiAndVersion = 0;
	for (int i = 0; i < 4; ++i)
	{
		_timeHiAndVersion = (_timeHiAndVersion << 4) | nibble(*it++);
	}
	if (haveHyphens) ++it;
	_clockSeq = 0;
	for (int i = 0; i < 4; ++i)
	{
		_clockSeq = (_clockSeq << 4) | nibble(*it++);
	}
	if (haveHyphens) ++it;
	for (int i = 0; i < 6; ++i)
	{
		_node[i] = (nibble(*it++) << 4) | nibble(*it++) ;			
	}

	return true;
}


std::string UUID::toString() const
{
	std::string result;
	result.reserve(36);
	appendHex(result, _timeLow);
	result += '-';
	appendHex(result, _timeMid);
	result += '-';
	appendHex(result, _timeHiAndVersion);
	result += '-';
	appendHex(result, _clockSeq);
	result += '-';
	for (int i = 0; i < sizeof(_node); ++i)
		appendHex(result, _node[i]);
	return result;
}


void UUID::copyFrom(const char* buffer)
{
	UInt32 i32;
	UInt16 i16;
	std::memcpy(&i32, buffer, sizeof(i32));
	_timeLow = ByteOrder::fromNetwork(i32);
	buffer += sizeof(i32);
	std::memcpy(&i16, buffer, sizeof(i16));
	_timeMid = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(&i16, buffer, sizeof(i16));
	_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(&i16, buffer, sizeof(i16));
	_clockSeq = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(_node, buffer, sizeof(_node));
}


void UUID::copyTo(char* buffer) const
{
	UInt32 i32 = ByteOrder::toNetwork(_timeLow);
	std::memcpy(buffer, &i32, sizeof(i32));
	buffer += sizeof(i32);
	UInt16 i16 = ByteOrder::toNetwork(_timeMid);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(_timeHiAndVersion);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(_clockSeq);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	std::memcpy(buffer, _node, sizeof(_node));
}


int UUID::variant() const
{
	int v = _clockSeq >> 13;
	if ((v & 6) == 6)
		return v;
	else if (v & 4)
		return 2;
	else
		return 0;
}


int UUID::compare(const UUID& uuid) const
{
	if (_timeLow != uuid._timeLow) return _timeLow < uuid._timeLow ? -1 : 1;
	if (_timeMid != uuid._timeMid) return _timeMid < uuid._timeMid ? -1 : 1;
	if (_timeHiAndVersion != uuid._timeHiAndVersion) return _timeHiAndVersion < uuid._timeHiAndVersion ? -1 : 1;
	if (_clockSeq != uuid._clockSeq) return _clockSeq < uuid._clockSeq ? -1 : 1;
	for (int i = 0; i < sizeof(_node); ++i)
	{
		if (_node[i] < uuid._node[i]) 
			return -1;
		else if (_node[i] > uuid._node[i])
			return 1;	
	}
	return 0;
}


void UUID::appendHex(std::string& str, UInt8 n) 
{
	static const char* digits = "0123456789abcdef";
	str += digits[(n >> 4) & 0xF];
	str += digits[n & 0xF];
}


void UUID::appendHex(std::string& str, UInt16 n)
{
	appendHex(str, UInt8(n >> 8));
	appendHex(str, UInt8(n & 0xFF));
}


void UUID::appendHex(std::string& str, UInt32 n)
{
	appendHex(str, UInt16(n >> 16));
	appendHex(str, UInt16(n & 0xFFFF));
}


UInt8 UUID::nibble(char hex)
{
	if (hex >= 'a' && hex <= 'f')
		return UInt8(hex - 'a' + 10);
	else if (hex >= 'A' && hex <= 'F')
		return UInt8(hex - 'A' + 10);
	else if (hex >= '0' && hex <= '9')
		return UInt8(hex - '0');
	else
		return UInt8(0);
}


void UUID::fromNetwork()
{
	_timeLow = ByteOrder::fromNetwork(_timeLow);
	_timeMid = ByteOrder::fromNetwork(_timeMid);
	_timeHiAndVersion = ByteOrder::fromNetwork(_timeHiAndVersion);
	_clockSeq = ByteOrder::fromNetwork(_clockSeq);
}


void UUID::toNetwork()
{
	_timeLow = ByteOrder::toNetwork(_timeLow);
	_timeMid = ByteOrder::toNetwork(_timeMid);
	_timeHiAndVersion = ByteOrder::toNetwork(_timeHiAndVersion);
	_clockSeq = ByteOrder::toNetwork(_clockSeq);
}


namespace
{
	static UUID uuidNull;
	static UUID uuidDNS("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
	static UUID uuidURI("6ba7b811-9dad-11d1-80b4-00c04fd430c8");
	static UUID uuidOID("6ba7b812-9dad-11d1-80b4-00c04fd430c8");
	static UUID uuidX500("6ba7b814-9dad-11d1-80b4-00c04fd430c8");
}


const UUID& UUID::null()
{
	return uuidNull;
}


const UUID& UUID::dns()
{
	return uuidDNS;
}

	
const UUID& UUID::uri()
{
	return uuidURI;
}


const UUID& UUID::oid()
{
	return uuidOID;
}


const UUID& UUID::x500()
{
	return uuidX500;
}


} // namespace Poco
