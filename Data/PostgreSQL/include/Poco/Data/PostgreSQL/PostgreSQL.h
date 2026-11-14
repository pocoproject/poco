//
// PostgreSQL.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQL
//
// Basic definitions for the PostgreSQL library.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_INCLUDED
#define SQL_PostgreSQL_INCLUDED


#include "Poco/Foundation.h"


//
// PostgreSQL_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define PostgreSQL_API POCO_API(PostgreSQL_EXPORTS)


//
// Automatically link Data library.
//
#if defined(_MSC_VER) && !defined(POCO_NO_AUTOMATIC_LIBS)
	#if !defined(PostgreSQL_EXPORTS)
		#pragma comment(lib, "PocoDataPostgreSQL" POCO_LIB_SUFFIX)
	#endif
	#pragma comment(lib, "libpq")
#endif


#endif // SQL_PostgreSQL_INCLUDED
