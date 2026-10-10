//
// ULIDGenerator.cpp
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


#include "Poco/ULIDGenerator.h"
#include "Poco/RandomStream.h"
#include "Poco/BinaryReader.h"
#include "Poco/Exception.h"


namespace Poco {


ULIDGenerator::ULIDGenerator()
{
}


ULIDGenerator::~ULIDGenerator()
{
}


ULID ULIDGenerator::create()
{
	Poco::Timestamp now;
	Poco::UInt64 time = now.epochMicroseconds()/1000;
	Poco::UInt16 lastRandomHi;
	Poco::UInt64 lastRandomLo;

	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		while (true)
		{
			if (time == _lastTime)
			{
				if (++_lastRandomLo == 0) 
				{
					if (++_lastRandomHi == 0)
					{
						// random overflow, wait until next millisecond
						Poco::UInt64 ntime = time;
						while (ntime == time)
						{
							now.update();
							ntime = now.epochMicroseconds()/1000;
						}
						time = ntime;
						continue;
					}
				}
			}
			else
			{
				_lastTime = time;
				Poco::RandomInputStream random;
				Poco::BinaryReader reader(random, Poco::BinaryReader::NETWORK_BYTE_ORDER);
				reader >> _lastRandomHi >> _lastRandomLo;
			}
			lastRandomHi = _lastRandomHi;
			lastRandomLo = _lastRandomLo;
			break;
		}
	}

	return ULID(time, lastRandomHi, lastRandomLo);
}


ULID ULIDGenerator::create(UInt64 time)
{
	Poco::UInt16 lastRandomHi;
	Poco::UInt64 lastRandomLo;

	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (time == _lastTime)
		{
			if (++_lastRandomLo == 0) 
			{
				if (++_lastRandomHi == 0) throw RuntimeException("Too many ULIDs created in a single millisecond");
			}
		}
		else
		{
			_lastTime = time;
			Poco::RandomInputStream random;
			Poco::BinaryReader reader(random, Poco::BinaryReader::NETWORK_BYTE_ORDER);
			reader >> _lastRandomHi >> _lastRandomLo;
		}
		lastRandomHi = _lastRandomHi;
		lastRandomLo = _lastRandomLo;
	}

	return ULID(time, lastRandomHi, lastRandomLo);
}


ULIDGenerator& ULIDGenerator::defaultGenerator()
{
	static ULIDGenerator gen;
	return gen;
}


} // namespace Poco
