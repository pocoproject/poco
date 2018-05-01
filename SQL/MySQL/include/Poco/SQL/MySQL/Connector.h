//
// Connector.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_MySQL_Connector_INCLUDED
#define SQL_MySQL_Connector_INCLUDED


#include "Poco/SQL/MySQL/MySQL.h"
#include "Poco/SQL/Connector.h"
#include "Poco/Mutex.h"


// Note: to avoid static (de)initialization problems,
// during connector automatic (un)registration, it is
// best to have this as a macro.
#define POCO_DATA_MYSQL_CONNECTOR_NAME "mysql"


namespace Poco {
namespace SQL {
namespace MySQL {


class MySQL_API Connector: public Poco::SQL::Connector
	/// Connector instantiates MySQL SessionImpl objects.
{
public:

	static std::string KEY;

	Connector();
		/// Creates the Connector.
	
	virtual ~Connector();
		/// Destroys the Connector.

	virtual const std::string& name() const;
		/// Returns the name associated with this connector.

	virtual Poco::AutoPtr<Poco::SQL::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = Poco::SQL::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a MySQL SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory

	static Poco::FastMutex _mutex;
};


} } } // namespace Poco::SQL::MySQL


//
// Automatic Connector registration
//

struct MySQL_API MySQLConnectorRegistrator
	/// Connector registering class.
	/// A global instance of this class is instantiated
	/// with sole purpose to automatically register the
	/// MySQL connector with central Poco Data registry.
{
	MySQLConnectorRegistrator()
		/// Calls Poco::SQL::MySQL::registerConnector();
	{
		Poco::SQL::MySQL::Connector::registerConnector();
	}

	~MySQLConnectorRegistrator()
		/// Calls Poco::SQL::MySQL::unregisterConnector();
	{
		try
		{
			Poco::SQL::MySQL::Connector::unregisterConnector();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}
};


#if !defined(POCO_NO_AUTOMATIC_LIB_INIT)
	#if defined(POCO_OS_FAMILY_WINDOWS) && !defined(__GNUC__)
		extern "C" const struct MySQL_API MySQLConnectorRegistrator pocoMySQLConnectorRegistrator;
		#if defined(MySQL_EXPORTS)
			#if defined(_WIN64)
				#define POCO_DATA_MYSQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_MYSQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:_"#s))
			#endif
		#else  // !MySQL_EXPORTS
			#if defined(_WIN64)
				#define POCO_DATA_MYSQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_MYSQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:_"#s))
			#endif
		#endif // MySQL_EXPORTS
	#else // !POCO_OS_FAMILY_WINDOWS
			#define POCO_DATA_MYSQL_FORCE_SYMBOL(s) extern "C" const struct MySQLConnectorRegistrator s;
	#endif // POCO_OS_FAMILY_WINDOWS
	POCO_DATA_MYSQL_FORCE_SYMBOL(pocoMySQLConnectorRegistrator)
#endif // POCO_NO_AUTOMATIC_LIB_INIT

//
// End automatic Connector registration
//


#endif // Data_MySQL_Connector_INCLUDED
