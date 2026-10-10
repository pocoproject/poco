//
// DynamicAnyHolder.h
//
// Library: Foundation
// Package: Dynamic
// Module:  VarHolder
//
// Forward header for VarHolder class to maintain backward compatibility
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DynamicAnyHolder_INCLUDED
#define Foundation_DynamicAnyHolder_INCLUDED

#include "Poco/Dynamic/VarHolder.h"

namespace {
	POCO_DEPRECATED("Deprecated header: include Poco/Dynamic/VarHolder.h instead")
	constexpr static int dynamicanyholder_hpp_is_deprecated {0xBAD};
	constexpr static int please_dont_use_badheader_hpp = dynamicanyholder_hpp_is_deprecated;
}


#endif // Foundation_DynamicAnyHolder_INCLUDED
