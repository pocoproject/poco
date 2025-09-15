//
// DynamicStruct.h
//
// Library: Foundation
// Package: Dynamic
// Module:  Struct
//
// Forward header for Struct class to maintain backward compatibility.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DynamicStruct_INCLUDED
#define Foundation_DynamicStruct_INCLUDED

#include "Poco/Dynamic/Struct.h"

namespace {
	POCO_DEPRECATED("Deprecated header: include Poco/Dynamic/Struct.h instead")
	constexpr static int dynamicstruct_hpp_is_deprecated {0xBAD};
	constexpr static int please_dont_use_badheader_hpp = dynamicstruct_hpp_is_deprecated;
}

#endif // Foundation_DynamicStruct_INCLUDED
