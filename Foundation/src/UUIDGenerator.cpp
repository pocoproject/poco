//
// UUIDGenerator.cpp
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


#include "Poco/UUIDGenerator.h"
#include "Poco/Thread.h"
#include "Poco/RandomStream.h"
#include "Poco/DigestEngine.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA1Engine.h"
#include <cstring>


namespace Poco {


UUIDGenerator::UUIDGenerator(): 
	_ticks(0), 
	_counter(0),
	_haveNode(false)
{
	seed();
}


UUIDGenerator::~UUIDGenerator()
{
}


UUID UUIDGenerator::create()
{
	FastMutex::ScopedLock lock(_mutex);

	// 0                   1                   2                   3
	// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                           time_low                            |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |           time_mid            |  ver  |       time_high       |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |var|         clock_seq         |             node              |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                              node                             |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	if (!_haveNode)
	{
		Environment::nodeId(_node);
		_haveNode = true;
	}
	Timestamp::UtcTimeVal tv = timeStamp();
	UInt32 timeLow = UInt32(tv & 0xFFFFFFFF);
	UInt16 timeMid = UInt16((tv >> 32) & 0xFFFF);
	UInt16 timeHiAndVersion = UInt16((tv >> 48) & 0x0FFF) + (UUID::UUID_TIME_BASED << 12);
	UInt16 clockSeq = (UInt16(_random.next() >> 4) & 0x3FFF) | 0x8000;
	return UUID(timeLow, timeMid, timeHiAndVersion, clockSeq, _node);
}


UUID UUIDGenerator::createFromName(const UUID& nsid, const std::string& name)
{
	MD5Engine md5;
	return createFromName(nsid, name, md5);
}


UUID UUIDGenerator::createFromName(const UUID& nsid, const std::string& name, DigestEngine& de)
{
	UUID::Version version = UUID::UUID_NAME_BASED;
	if (dynamic_cast<SHA1Engine*>(&de)) version = UUID::UUID_NAME_BASED_SHA1;
	return createFromName(nsid, name, de, version);
}


UUID UUIDGenerator::createFromName(const UUID& nsid, const std::string& name, DigestEngine& de, UUID::Version version)
{
	poco_assert_dbg (de.digestLength() >= 16);

	UUID netNsid = nsid;
	netNsid.toNetwork();
	de.reset();
	de.update(&netNsid._timeLow, sizeof(netNsid._timeLow));
	de.update(&netNsid._timeMid, sizeof(netNsid._timeMid));
	de.update(&netNsid._timeHiAndVersion, sizeof(netNsid._timeHiAndVersion));
	de.update(&netNsid._clockSeq, sizeof(netNsid._clockSeq));
	de.update(&netNsid._node[0], sizeof(netNsid._node));
	de.update(name);
	char buffer[16];
	const DigestEngine::Digest& d = de.digest();
	for (int i = 0; i < 16; ++i)
	{
		buffer[i] = d[i];
	}
	return UUID(buffer, version);
}


UUID UUIDGenerator::createRandom()
{
	char buffer[16];
	RandomInputStream ris;
	ris.read(buffer, sizeof(buffer));
	return UUID(buffer, UUID::UUID_RANDOM);
}


UUID UUIDGenerator::createOne()
{
	try
	{
		return create();
	}
	catch (Exception&)
	{
		return createRandom();
	}
}


UUID UUIDGenerator::createV6()
{
	FastMutex::ScopedLock lock(_mutex);

	// 0                   1                   2                   3
	// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                           time_high                           |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |           time_mid            |  ver  |       time_low        |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |var|         clock_seq         |             node              |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                              node                             |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	if (!_haveNode)
	{
		try
		{
			Environment::nodeId(_node);
			_haveNode = true;
		}
		catch (Poco::Exception&)
		{
			RandomInputStream ris;
			ris.read(reinterpret_cast<char*>(_node), sizeof(_node));
		}
	}
	Timestamp::UtcTimeVal tv = timeStamp();
	UInt32 timeHigh = UInt32((tv >> 28) & 0xFFFFFFFF);
	UInt16 timeMid = UInt16((tv >> 12) & 0xFFFF);
	UInt16 timeLoAndVersion = UInt16(tv & 0x0FFF) + (UUID::UUID_TIME_BASED_V6 << 12);
	UInt16 clockSeq = (UInt16(_random.next() >> 4) & 0x3FFF) | 0x8000;
	return UUID(timeHigh, timeMid, timeLoAndVersion, clockSeq, _node);
}


UUID UUIDGenerator::createV7()
{
	FastMutex::ScopedLock lock(_mutex);

	//  0                   1                   2                   3
	//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                           unix_ts_ms                          |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |          unix_ts_ms           |  ver  |       rand_a          |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |var|                        rand_b                             |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                            rand_b                             |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


	if (_counter == 0) _counter = static_cast<Poco::UInt16>(_random.next(4096));

	Timestamp::TimeVal tv = Poco::Timestamp().epochMicroseconds()/1000;
	UInt32 timeHigh = UInt32((tv >> 16) & 0xFFFFFFFF);
	UInt16 timeMid = UInt16(tv & 0xFFFF);
	UInt16 randAVersion = (_counter++ & 0x0FFF) + (UUID::UUID_TIME_BASED_V7 << 12);
	UInt16 randBHiVar = (UInt16(_random.next() >> 4) & 0x3FFF) | 0x8000;
	Poco::UInt8 randBLow[6];
	RandomInputStream ris;
	ris.read(reinterpret_cast<char*>(randBLow), sizeof(randBLow));
	return UUID(timeHigh, timeMid, randAVersion, randBHiVar, randBLow);
}


Timestamp::UtcTimeVal UUIDGenerator::timeStamp()
{
	Timestamp now;
	for (;;)
	{
		if (now != _lastTime)
		{
			_lastTime = now;
			_ticks = 0;
			break;
		}
		if (_ticks < 100)
		{
			++_ticks;
			break;
		}
		now.update();
	}
	Timestamp::UtcTimeVal tv = now.utcTime();
	return tv + _ticks;
}


void UUIDGenerator::seed()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_random.seed();
}


void UUIDGenerator::seed(UInt32 n)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_random.seed(n);
}


UUIDGenerator& UUIDGenerator::defaultGenerator()
{
	static UUIDGenerator g;
	return g;
}


} // namespace Poco
