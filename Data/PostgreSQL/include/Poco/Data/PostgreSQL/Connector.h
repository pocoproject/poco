//
// Connector.h
//
// $Id: //poco/1.5/Data/PostgreSQL/include/Poco/Data/PostgreSQL/Connector.h#1 $
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Data_PostgreSQL_Connector_INCLUDED
#define Data_PostgreSQL_Connector_INCLUDED

#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/Connector.h"

#include "Poco/AutoPtr.h"

#include <string>

// Note: to avoid static (de)initialization problems,
// during connector automatic (un)registration, it is 
// best to have this as a macro.

#define POCO_DATA_POSTGRESQL_CONNECTOR_NAME "postgresql"

namespace Poco {
namespace Data {
namespace PostgreSQL {

class PostgreSQL_API Connector: public Poco::Data::Connector
	/// Connector instantiates PostgreSQL SessionImpl objects.
{
public:

	static std::string KEY;

	Connector();
		/// Creates the Connector.

	virtual ~Connector();
		/// Destroys the Connector.

	virtual const std::string& name() const;
		/// Returns the name associated with this connector.

	virtual Poco::AutoPtr<Poco::Data::SessionImpl> createSession(const std::string&  aConnectionString,
																  std::size_t aTimeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a PostgreSQL SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory
};


} } } // namespace Poco::Data::PostgreSQL


// 
// Automatic Connector registration
// 

struct PostgreSQL_API PostgreSQLConnectorRegistrator
	/// Connector registering class.
	/// A global instance of this class is instantiated
	/// with sole purpose to automatically register the 
	/// PostgreSQL connector with central Poco Data registry.
{
	PostgreSQLConnectorRegistrator()
		/// Calls Poco::Data::PostgreSQL::registerConnector();
	{
		Poco::Data::PostgreSQL::Connector::registerConnector();
	}

	~PostgreSQLConnectorRegistrator()
		/// Calls Poco::Data::PostgreSQL::unregisterConnector();
	{
		Poco::Data::PostgreSQL::Connector::unregisterConnector();
	}
};


#if !defined(POCO_NO_AUTOMATIC_LIB_INIT)
	#if defined(POCO_OS_FAMILY_WINDOWS)
		extern "C" const struct PostgreSQL_API PostgreSQLConnectorRegistrator pocoPostgreSQLConnectorRegistrator;
		#if defined(PostgreSQL_EXPORTS)
			#if defined(_WIN64)
				#define POCO_DATA_POSTGRESQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_POSTGRESQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/export:_"#s))
			#endif
		#else  // !PostgreSQL_EXPORTS
			#if defined(_WIN64)
				#define POCO_DATA_POSTGRESQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:"#s))
			#elif defined(_WIN32)
				#define POCO_DATA_POSTGRESQL_FORCE_SYMBOL(s) __pragma(comment (linker, "/include:_"#s))
			#endif
		#endif // PostgreSQL_EXPORTS
	#else // !POCO_OS_FAMILY_WINDOWS
			#define POCO_DATA_POSTGRESQL_FORCE_SYMBOL(s) extern "C" const struct PostgreSQLConnectorRegistrator s;
	#endif // POCO_OS_FAMILY_WINDOWS
	POCO_DATA_POSTGRESQL_FORCE_SYMBOL(pocoPostgreSQLConnectorRegistrator)
#endif // POCO_NO_AUTOMATIC_LIB_INIT

// 
// End automatic Connector registration
// 


#endif // Data_PostgreSQL_Connector_INCLUDED
