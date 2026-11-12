//
// SQLite.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  SQLite
//
// Basic definitions for the Poco SQLite library.
// This file must be the first file included by every other SQLite
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLite_SQLite_INCLUDED
#define SQLite_SQLite_INCLUDED


#include "Poco/Foundation.h"


//
// SQLite_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define SQLite_API POCO_API(SQLite_EXPORTS)


//
// Automatically link SQLite library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SQLite_EXPORTS)
		#pragma comment(lib, "PocoDataSQLite" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SQLite_SQLite_INCLUDED
