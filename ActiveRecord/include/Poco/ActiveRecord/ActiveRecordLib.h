//
// ActiveRecordLib.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  ActiveRecordLib
//
// Basic definitions for the ActiveRecord library.
// This file must be the first file included by every other ActiveRecordLib
// header file.
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_ActiveRecordLib_INCLUDED
#define ActiveRecord_ActiveRecordLib_INCLUDED


#include "Poco/Poco.h"


//
// ActiveRecordLib_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define ActiveRecordLib_API POCO_API(ActiveRecordLib_EXPORTS)


//
// Automatically link ActiveRecordLib library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ActiveRecordLib_EXPORTS)
		#pragma comment(lib, "PocoActiveRecord" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ActiveRecord_ActiveRecordLib_INCLUDED
