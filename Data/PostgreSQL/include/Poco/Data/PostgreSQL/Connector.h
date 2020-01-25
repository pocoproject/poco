//
// Connector.h
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


#ifndef SQL_PostgreSQL_Connector_INCLUDED
#define SQL_PostgreSQL_Connector_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/Connector.h"
#include "Poco/AutoPtr.h"
#include <string>


namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API Connector: public Poco::Data::Connector
	/// Connector instantiates PostgreSQL SessionImpl objects.
{
public:
	static const std::string KEY;

	Connector();
		/// Creates the Connector.

	virtual ~Connector();
		/// Destroys the Connector.

	virtual const std::string& name() const;
		/// Returns the name associated with this connector.

	virtual Poco::Data::SessionImpl::Ptr createSession(const std::string&  aConnectionString,
		std::size_t aTimeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a PostgreSQL SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory
};


} } } // namespace Poco::Data::PostgreSQL


#endif // Data_PostgreSQL_Connector_INCLUDED
