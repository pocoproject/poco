//
// Connector.h
//
// Library: Data/SQLiteBDB
// Package: SQLiteBDB
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SQLiteBDB_Connector_INCLUDED
#define Data_SQLiteBDB_Connector_INCLUDED


#include "Poco/Data/SQLiteBDB/SQLite.h"
#include "Poco/Data/Connector.h"


namespace Poco {
namespace Data {
namespace SQLiteBDB {


class SQLiteBDB_API Connector: public Poco::Data::Connector
	/// Connector instantiates SQLiteBDB SessionImpl objects.
{
public:
	static const std::string KEY;
		/// Keyword for creating SQLiteBDB sessions ("SQLiteBDB").

	Connector();
		/// Creates the Connector.

	~Connector();
		/// Destroys the Connector.

	const std::string& name() const;
		/// Returns the name associated with this connector.

	Poco::AutoPtr<Poco::Data::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a SQLiteBDB SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory.

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory.

	static void enableSharedCache(bool flag = true);
		/// Enables or disables SQLiteBDB shared cache mode
		/// (see http://www.SQLiteBDB.org/sharedcache.html for a discussion).

	static void enableSoftHeapLimit(int limit);
		/// Sets a soft upper limit to the amount of memory allocated
		/// by SQLiteBDB. For more information, please see the SQLiteBDB
		/// SQLiteBDB_soft_heap_limit() function (http://www.SQLiteBDB.org/c3ref/soft_heap_limit.html).
};


///
/// inlines
///
inline const std::string& Connector::name() const
{
	return KEY;
}


} } } // namespace Poco::Data::SQLiteBDB


#endif // Data_SQLiteBDB_Connector_INCLUDED
