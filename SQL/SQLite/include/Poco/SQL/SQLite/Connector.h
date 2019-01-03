//
// Connector.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_SQLite_Connector_INCLUDED
#define SQL_SQLite_Connector_INCLUDED


#include "Poco/SQL/SQLite/SQLite.h"
#include "Poco/SQL/Connector.h"


// Note: to avoid static (de)initialization problems,
// during connector automatic (un)registration, it is
// best to have this as a macro.
#define POCO_DATA_SQLITE_CONNECTOR_NAME "sqlite"


namespace Poco {
namespace SQL {
namespace SQLite {


class SQLite_API Connector: public Poco::SQL::Connector
	/// Connector instantiates SqLite SessionImpl objects.
{
public:
	static const std::string KEY;
		/// Keyword for creating SQLite sessions ("SQLite").

	Connector();
		/// Creates the Connector.

	~Connector();
		/// Destroys the Connector.

	const std::string& name() const;
		/// Returns the name associated with this connector.

	Poco::AutoPtr<Poco::SQL::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = Poco::SQL::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a SQLite SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory.

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory.

	static void enableSharedCache(bool flag = true);
		/// Enables or disables SQlite shared cache mode
		/// (see http://www.sqlite.org/sharedcache.html for a discussion).

	static void enableSoftHeapLimit(int limit);
		/// Sets a soft upper limit to the amount of memory allocated
		/// by SQLite. For more information, please see the SQLite
		/// sqlite_soft_heap_limit() function (http://www.sqlite.org/c3ref/soft_heap_limit.html).
};


///
/// inlines
///
inline const std::string& Connector::name() const
{
	static const std::string n(POCO_DATA_SQLITE_CONNECTOR_NAME);
	return n;
}


} } } // namespace Poco::SQL::SQLite


//
// Automatic Connector registration
//

struct SQLite_API SQLiteConnectorRegistrator
	/// Connector registering class.
	/// A global instance of this class is instantiated
	/// with sole purpose to automatically register the
	/// SQLite connector with central Poco Data registry.
{
	SQLiteConnectorRegistrator()
		/// Calls Poco::SQL::SQLite::registerConnector();
	{
		Poco::SQL::SQLite::Connector::registerConnector();
	}

	~SQLiteConnectorRegistrator()
		/// Calls Poco::SQL::SQLite::unregisterConnector();
	{
		try
		{
			Poco::SQL::SQLite::Connector::unregisterConnector();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}
};


#if !defined(POCO_NO_AUTOMATIC_LIB_INIT)
	#if defined(POCO_OS_FAMILY_WINDOWS) && !defined(__GNUC__)
		extern "C" const struct SQLite_API SQLiteConnectorRegistrator pocoSQLiteConnectorRegistrator;
		#if defined(SQLite_EXPORTS)
			#if defined(_WIN64) || defined(_WIN32_WCE)
				#define POCO_DATA_SQLITE_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_SQLITE_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:_"#s))
			#endif
		#else  // !SQLite_EXPORTS
			#if defined(_WIN64) || defined(_WIN32_WCE)
				#define POCO_DATA_SQLITE_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_SQLITE_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:_"#s))
			#endif
		#endif // SQLite_EXPORTS
	#else // !POCO_OS_FAMILY_WINDOWS
			#define POCO_DATA_SQLITE_FORCE_SYMBOL(s) extern "C" const struct SQLiteConnectorRegistrator s;
	#endif // POCO_OS_FAMILY_WINDOWS
	POCO_DATA_SQLITE_FORCE_SYMBOL(pocoSQLiteConnectorRegistrator)
#endif // POCO_NO_AUTOMATIC_LIB_INIT

//
// End automatic Connector registration
//

#endif // Data_SQLite_Connector_INCLUDED
