//
// Utility.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Utility
//
// Definition of Utility.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_Utility_INCLUDED
#define SQL_PostgreSQL_Utility_INCLUDED


#include "Poco/SQL/PostgreSQL/PostgreSQL.h"
#include "Poco/SQL/PostgreSQL/SessionHandle.h"
#include "Poco/SQL/Session.h"


namespace Poco {
namespace SQL {
namespace PostgreSQL {


class PostgreSQL_API Utility
	/// Various utility functions for PostgreSQL.
{
public:

	static std::string serverInfo(SessionHandle* aHandlePtr);
		/// Returns server info.

	static std::string serverInfo(Poco::SQL::Session& aSession);
		/// Returns server info.

	static int serverVersion(SessionHandle* aHandlePtr);
		/// Returns server version.

	static int serverVersion(Poco::SQL::Session& aSession);
		/// Returns server version.

	static std::string hostInfo(SessionHandle* aHandlePtr);
		/// Returns host info.

	static std::string hostInfo(Poco::SQL::Session& aSession);
		/// Returns host info.

	static std::string sessionEncoding(SessionHandle* aHandlePtr);
		/// Returns session encoding.

	static std::string sessionEncoding(Poco::SQL::Session& aSession);
		/// Returns session encoding.

	static bool hasMicrosecond() { return true; }
		/// Rturns true if microseconds are suported.

	static SessionHandle* handle(Poco::SQL::Session& aSession);
		/// Returns native PostgreSQL handle for the session.
};


//
// inlines
//


inline SessionHandle* Utility::handle(Session& aSession)
{
	return Poco::AnyCast< SessionHandle* >(aSession.getProperty("handle"));
}


} } } // namespace Poco::SQL::PostgreSQL


#endif // SQL_PostgreSQL_Utility_INCLUDED
