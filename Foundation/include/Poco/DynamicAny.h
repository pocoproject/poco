//
// DynamicAny.h
//
// Library: Foundation
// Package: Dynamic
// Module:  Var
//
// Forward header for Var class to maintain backward compatibility.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DynamicAny_INCLUDED
#define Foundation_DynamicAny_INCLUDED

#include "Poco/Dynamic/Var.h"

namespace {
	POCO_DEPRECATED("Deprecated header: include Poco/Dynamic/Var.h instead")
	constexpr static int dynamicany_hpp_is_deprecated {0xBAD};
	constexpr static int please_dont_use_badheader_hpp = dynamicany_hpp_is_deprecated;
}


#endif // Foundation_DynamicAny_INCLUDED
