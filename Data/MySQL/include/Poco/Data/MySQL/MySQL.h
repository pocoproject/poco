//
// MySQL.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  MySQL
//
// Basic definitions for the MySQL library.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MySQL_MySQL_INCLUDED
#define MySQL_MySQL_INCLUDED


#include "Poco/Foundation.h"
#include <mysql/mysql.h>


//
// MySQL_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define MySQL_API POCO_API(MySQL_EXPORTS)


//
// Automatically link Data library.
//
#if defined(_MSC_VER) && !defined(POCO_NO_AUTOMATIC_LIBS)
	#if !defined(MySQL_EXPORTS)
		#pragma comment(lib, "PocoDataMySQL" POCO_LIB_SUFFIX)
	#endif
	#if defined(LIBMARIADB)
		#pragma comment(lib, "libmariadb")
	#else
		#pragma comment(lib, "libmysql")
	#endif
#endif

//
// Detect support for JSON data type
//
#if defined(MARIADB_VERSION_ID)
	#if MARIADB_VERSION_ID >= 100207
		#define POCO_MYSQL_JSON
	#endif
#else
	#if MYSQL_VERSION_ID >= 50708
		#define POCO_MYSQL_JSON
	#endif
#endif

#endif // MySQL_MySQL_INCLUDED
