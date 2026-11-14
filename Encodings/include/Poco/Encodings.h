//
// Encodings.h
//
// Library: Encodings
// Package: Encodings
// Module:  Encodings
//
// Basic definitions for the Poco Encodings library.
// This file must be the first file included by every other Encodings
// header file.
//
// Copyright (c) 2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Encodings_Encodings_INCLUDED
#define Encodings_Encodings_INCLUDED


#include "Poco/Foundation.h"


//
// Encodings_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Encodings_API POCO_API(Encodings_EXPORTS)


//
// Automatically link Encodings library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Encodings_EXPORTS)
		#pragma comment(lib, "PocoEncodings" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {


void Encodings_API registerExtraEncodings();
	/// Registers the character encodings from the Encodings library
	/// with the TextEncoding class.


} // namespace Poco


#endif // Encodings_Encodings_INCLUDED
