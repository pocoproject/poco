//
// Utility.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Utility
//
// Definition of Utility.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MySQL_Utility_INCLUDED
#define MySQL_Utility_INCLUDED


#include "Poco/Data/MySQL/MySQL.h"
#include "Poco/Data/Session.h"
#include <mysql/mysql.h>


namespace Poco::Data::MySQL {


class MySQL_API Utility
	/// Various utility functions for MySQL.
{
public:
	[[nodiscard]] static std::string serverInfo(MYSQL* pHandle);
		/// Returns server info.
		/// Throws NullPointerException if pHandle is null or has no server info.

	[[nodiscard]] static std::string serverInfo(Poco::Data::Session& session);
		/// Returns server info.
		/// Throws NullPointerException if the session is not connected.

	[[nodiscard]] static unsigned long serverVersion(MYSQL* pHandle);
		/// Returns server version.
		/// Throws NullPointerException if pHandle is null.

	[[nodiscard]] static unsigned long serverVersion(Poco::Data::Session& session);
		/// Returns server version.
		/// Throws NullPointerException if the session is not connected.

	[[nodiscard]] static std::string hostInfo(MYSQL* pHandle);
		/// Returns host info.
		/// Throws NullPointerException if pHandle is null or has no host info.

	[[nodiscard]] static std::string hostInfo(Poco::Data::Session& session);
		/// Returns host info.
		/// Throws NullPointerException if the session is not connected.

	[[nodiscard]] static MYSQL* handle(Poco::Data::Session& session);
		/// Returns native MySQL handle for the session.
		/// Returns nullptr if the session is not connected; the handle changes when the session is opened again.
};


//
// inlines
//


inline MYSQL* Utility::handle(Session& session)
{
	return Poco::AnyCast<MYSQL*>(session.getProperty("handle"));
}


} // namespace Poco::Data::MySQL


#endif // MySQL_Utility_INCLUDED
