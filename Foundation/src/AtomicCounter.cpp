//
// AtomicCounter.cpp
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


AtomicCounter::AtomicCounter():
	_counter(0)
{
}


AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
	_counter(counter.value())
{
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	_counter.store(counter._counter.load());
	return *this;
}


AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	_counter.store(value);
	return *this;
}


} // namespace Poco
