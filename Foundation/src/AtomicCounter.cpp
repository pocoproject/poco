//
// AtomicCounter.cpp
//
// $Id: //poco/1.4/Foundation/src/AtomicCounter.cpp#2 $
//
// Library: Foundation
// Package: Core
// Module:  AtomicCounter
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AtomicCounter.h"


namespace Poco {

//
// C++11
//
AtomicCounter::AtomicCounter(): 
	std::atomic<int>()
{
}

	
AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
	std::atomic<int>(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
	std::atomic<int>(counter.load())
{
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	store(counter.load());
	return *this;
}

	
AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	store(value);
	return *this;
}


} // namespace Poco
