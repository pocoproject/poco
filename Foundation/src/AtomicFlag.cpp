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


#if defined(POCO_HAVE_STD_ATOMICS)


#include "Poco/AtomicCounter.h"


namespace Poco {


AtomicFlag::AtomicFlag()
{
}


AtomicFlag::~AtomicFlag()
{
}


} // namespace Poco


#endif // POCO_HAVE_STD_ATOMICS
