//
// Utility.h
//
// $Id: //poco/Main/Data/PostgreSQL/include/Poco/Data/PostgreSQL/Utility.h#2 $
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


#ifndef PostgreSQL_Utility_INCLUDED
#define PostgreSQL_Utility_INCLUDED

#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"

#include "Poco/Data/Session.h"

namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API Utility
	/// Various utility functions for PostgreSQL.
{
public:

	static std::string serverInfo(SessionHandle* aHandlePtr);
		/// Returns server info.

	static std::string serverInfo(Poco::Data::Session& aSession);
		/// Returns server info.

	static int serverVersion(SessionHandle* aHandlePtr);
		/// Returns server version.

	static int serverVersion(Poco::Data::Session& aSession);
		/// Returns server version.

	static std::string hostInfo(SessionHandle* aHandlePtr);
		/// Returns host info.

	static std::string hostInfo(Poco::Data::Session& aSession);
		/// Returns host info.

	static std::string sessionEncoding(SessionHandle* aHandlePtr);
		/// Returns session encoding.

	static std::string sessionEncoding(Poco::Data::Session& aSession);
		/// Returns session encoding.

	static bool hasMicrosecond() { return true; }
		/// Rturns true if microseconds are suported.

	static SessionHandle* handle(Poco::Data::Session& aSession);
		/// Returns native PostgreSQL handle for the session.
};


//
// inlines
//


inline SessionHandle* Utility::handle(Session& aSession)
{
	return Poco::AnyCast< SessionHandle* >(aSession.getProperty("handle"));
}


} } } // namespace Poco::Data::PostgreSQL


#endif // PostgreSQL_Utility_INCLUDED
