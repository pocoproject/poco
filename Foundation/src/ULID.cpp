//
// ULID.cpp
//
// Library: Foundation
// Package: ULID
// Module:  ULID
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ULID.h"
#include "Poco/Exception.h"
#include "Poco/ByteOrder.h"
#include <algorithm>
#include <cstring>


namespace Poco {


namespace 
{
	template <typename T>
	int cmp(T a, T b)
	{
		if (a == b)
			return 0;
		else if (a < b)
			return -1;
		else
			return 1;
	}
}


const char ULID::ENCODING[32] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'J', 'K', 'M', 'N', 'P', 'Q',
	'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'
};


const UInt8 ULID::REVERSE_ENCODING[256] = 
{
	/* 00 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 08 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 10 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 18 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 20 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 28 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 30 */ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	/* 38 */ 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 
	/* 40 */ 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 
	/* 48 */ 0x11, 0x01, 0x12, 0x13, 0x01, 0x14, 0x15, 0x00, 
	/* 50 */ 0x16, 0x17, 0x18, 0x19, 0x1A, 0xFF, 0x1B, 0x1C, 
	/* 58 */ 0x1D, 0x1E, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 60 */ 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 
	/* 68 */ 0x11, 0x01, 0x12, 0x13, 0x01, 0x14, 0x15, 0x00, 
	/* 70 */ 0x16, 0x17, 0x18, 0x19, 0x1A, 0xFF, 0x1B, 0x1C, 
	/* 78 */ 0x1D, 0x1E, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 80 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 88 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 90 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* 98 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* A0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* A8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* B0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* B8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* C0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* C8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* D0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* D8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* E0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* E8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* F0 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* F8 */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


ULID::ULID(const std::string& ulid)
{
	parse(ulid);	
}


ULID::ULID(const char* ulid)
{	
	poco_check_ptr(ulid);
	parse(std::string(ulid));
}


ULID::ULID(UInt64 time, UInt16 randomHi, UInt64 randomLo):
	_timeHi(static_cast<UInt32>(time >> 16)),
	_timeLo(static_cast<UInt16>(time & 0xFFFF)),
	_randomHi(randomHi),
	_randomLo(randomLo)
{
}


ULID& ULID::operator = (const ULID& ulid)
{
	if (&ulid != this)
	{
		_timeHi = ulid._timeHi;
		_timeLo = ulid._timeLo;
		_randomHi = ulid._randomHi;
		_randomLo = ulid._randomLo;
	}
	return *this;	
}


void ULID::swap(ULID& ulid) noexcept
{
	std::swap(_timeHi, ulid._timeHi);
	std::swap(_timeLo, ulid._timeLo);
	std::swap(_randomHi, ulid._randomHi);
	std::swap(_randomLo, ulid._randomLo);
}


void ULID::parse(const std::string& ulid)
{
	if (!tryParse(ulid))
		throw SyntaxException(ulid);
}


bool ULID::tryParse(const std::string& ulid)
{
	if (ulid.size() != 26) return false;
	if (ulid[0] > '7' && !(ulid[0] == 'l' || ulid[0] == 'L' || ulid[0] == 'i' || ulid[0] == 'I' || ulid[0] == 'o' || ulid[0] == 'O')) return false;

	for (auto c: ulid)
	{
		if (REVERSE_ENCODING[static_cast<unsigned char>(c)] == 0xFF) return false;
	}

	_timeHi = (decode<UInt32>(ulid[0]) << 29)
	        | (decode<UInt32>(ulid[1]) << 24)
			| (decode<UInt32>(ulid[2]) << 19)
			| (decode<UInt32>(ulid[3]) << 14)
			| (decode<UInt32>(ulid[4]) << 9)
			| (decode<UInt32>(ulid[5]) << 4)
			| (decode<UInt32>(ulid[6]) >> 1);

	_timeLo = ((decode<UInt16>(ulid[6]) & 1) << 15)
			| (decode<UInt16>(ulid[7]) << 10)
			| (decode<UInt16>(ulid[8]) << 5)
			| decode<UInt16>(ulid[9]);

	_randomHi = (decode<UInt16>(ulid[10]) << 11)
			| (decode<UInt16>(ulid[11]) << 6)
			| (decode<UInt16>(ulid[12]) << 1)
			| (decode<UInt16>(ulid[13]) >> 4);

	_randomLo = ((decode<UInt64>(ulid[13]) & 0xF) << 60)
			| (decode<UInt64>(ulid[14]) << 55)
			| (decode<UInt64>(ulid[15]) << 50)
			| (decode<UInt64>(ulid[16]) << 45)
			| (decode<UInt64>(ulid[17]) << 40)
			| (decode<UInt64>(ulid[18]) << 35)
			| (decode<UInt64>(ulid[19]) << 30)
			| (decode<UInt64>(ulid[20]) << 25)
			| (decode<UInt64>(ulid[21]) << 20)
			| (decode<UInt64>(ulid[22]) << 15)
			| (decode<UInt64>(ulid[23]) << 10)
			| (decode<UInt64>(ulid[24]) << 5)
			| decode<UInt64>(ulid[25]);

	return true;
}


std::string ULID::toString() const
{
	std::string result(26, 0);

	result[0] = ENCODING[_timeHi >> 29];
	result[1] = ENCODING[_timeHi >> 24 & 0x1F];
	result[2] = ENCODING[_timeHi >> 19 & 0x1F];
	result[3] = ENCODING[_timeHi >> 14 & 0x1F];
	result[4] = ENCODING[_timeHi >> 9 & 0x1F];
	result[5] = ENCODING[_timeHi >> 4 & 0x1F];
	result[6] = ENCODING[((_timeHi & 0x0F) << 1) + (_timeLo >> 15)];
	result[7] = ENCODING[_timeLo >> 10 & 0x1F];
	result[8] = ENCODING[_timeLo >> 5 & 0x1F];
	result[9] = ENCODING[_timeLo & 0x1F];

	result[10] = ENCODING[_randomHi >> 11 & 0x1F];
	result[11] = ENCODING[_randomHi >> 6 & 0x1F];
	result[12] = ENCODING[_randomHi >> 1 & 0x1F];
	result[13] = ENCODING[((_randomHi & 0x01) << 4) + (_randomLo >> 60)];
	result[14] = ENCODING[_randomLo >> 55 & 0x1F];
	result[15] = ENCODING[_randomLo >> 50 & 0x1F];
	result[16] = ENCODING[_randomLo >> 45 & 0x1F];
	result[17] = ENCODING[_randomLo >> 40 & 0x1F];
	result[18] = ENCODING[_randomLo >> 35 & 0x1F];
	result[19] = ENCODING[_randomLo >> 30 & 0x1F];
	result[20] = ENCODING[_randomLo >> 25 & 0x1F];
	result[21] = ENCODING[_randomLo >> 20 & 0x1F];
	result[22] = ENCODING[_randomLo >> 15 & 0x1F];
	result[23] = ENCODING[_randomLo >> 10 & 0x1F];
	result[24] = ENCODING[_randomLo >> 5 & 0x1F];
	result[25] = ENCODING[_randomLo & 0x1F];

	return result;
}


void ULID::copyFrom(const char* buffer)
{
	Poco::UInt32 timeHi;
	std::memcpy(&timeHi, buffer, 4);
	_timeHi = Poco::ByteOrder::fromNetwork(timeHi);
	Poco::UInt16 timeLo;
	std::memcpy(&timeLo, buffer + 4, 2);
	_timeLo = Poco::ByteOrder::fromNetwork(timeLo);
	Poco::UInt16 randomHi;
	std::memcpy(&randomHi, buffer + 6, 2);
	_randomHi = Poco::ByteOrder::fromNetwork(randomHi);
	Poco::UInt64 randomLo;
	std::memcpy(&randomLo, buffer + 8, 8);
	_randomLo = Poco::ByteOrder::fromNetwork(randomLo);
}


void ULID::copyTo(char* buffer) const
{
	Poco::UInt32 timeHi = Poco::ByteOrder::toNetwork(_timeHi);
	std::memcpy(buffer, &timeHi, 4);
	Poco::UInt16 timeLo = Poco::ByteOrder::toNetwork(_timeLo);
	std::memcpy(buffer + 4, &timeLo, 2);
	Poco::UInt16 randomHi = Poco::ByteOrder::toNetwork(_randomHi);
	std::memcpy(buffer + 6, &randomHi, 2);
	Poco::UInt64 randomLo = Poco::ByteOrder::toNetwork(_randomLo);
	std::memcpy(buffer + 8, &randomLo, 8);
}


const ULID& ULID::null()
{
	static const ULID nil;
	return nil;	
}


int ULID::compare(const ULID& ulid) const
{
	int c = cmp(_timeHi, ulid._timeHi);
	if (c == 0)
	{
		c = cmp(_timeLo, ulid._timeLo);
		if (c == 0)
		{
			c = cmp(_randomHi, ulid._randomHi);
			if (c == 0)
			{
				c = cmp(_randomLo, ulid._randomLo);
			}
		}
	}
	return c;
}


} // namespace Poco
