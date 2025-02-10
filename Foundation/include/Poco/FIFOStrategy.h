//
// FIFOStrategy.h
//
// Library: Foundation
// Package: Events
// Module:  FIFOStragegy
//
// Implementation of the FIFOStrategy template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FIFOStrategy_INCLUDED
#define Foundation_FIFOStrategy_INCLUDED


#include "Poco/DefaultStrategy.h"


namespace Poco {


template <class TArgs, class TDelegate>
class POCO_DEPRECATED("") FIFOStrategy: public DefaultStrategy<TArgs, TDelegate>
	/// Note: As of release 1.4.2, DefaultStrategy already
	/// implements FIFO behavior, so this class is provided
	/// for backwards compatibility only.
{
public:
	FIFOStrategy() = default;

	FIFOStrategy(const FIFOStrategy& s):
		DefaultStrategy<TArgs, TDelegate>(s)
	{
	}

	FIFOStrategy(FIFOStrategy&& s):
		DefaultStrategy<TArgs, TDelegate>(std::move(s))
	{
	}

	~FIFOStrategy() = default;

	FIFOStrategy& operator = (const FIFOStrategy& s)
	{
		DefaultStrategy<TArgs, TDelegate>::operator = (s);
		return *this;
	}
	
	FIFOStrategy& operator = (FIFOStrategy&& s)
	{
		DefaultStrategy<TArgs, TDelegate>::operator = (s);
		return *this;
	}
};


} // namespace Poco


#endif // Foundation_FIFOStrategy_INCLUDED
