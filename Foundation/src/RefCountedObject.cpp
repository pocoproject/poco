//
// RefCountedObject.cpp
//
// Library: Foundation
// Package: Core
// Module:  RefCountedObject
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RefCountedObject.h"


namespace Poco {


RefCounter::RefCounter() : _counter(1)
{
}


RefCounter::~RefCounter()
{
}


void RefCounter::duplicate() const
{
	++_counter;
}


int RefCounter::release()
{
	int counter = --_counter;
	if (counter == 0)
	{
		delete this;
		return 0;
	}
	poco_assert(counter > 0);
	return counter;
}


int RefCounter::count() const
{
	return _counter;
}


} // namespace Poco
