//
// Data.h
//
// Library: Data
// Package: DataCore
// Module:  Data
//
// Basic definitions for the Poco Data library.
// This file must be the first file included by every other Data
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Data_INCLUDED
#define Data_Data_INCLUDED


#include "Poco/Foundation.h"


//
// Data_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Data_API POCO_API(Data_EXPORTS)


//
// Automatically link Data library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Data_EXPORTS)
		#pragma comment(lib, "PocoData" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Data_Data_INCLUDED
