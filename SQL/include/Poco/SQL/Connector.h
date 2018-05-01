//
// Connector.h
//
// Library: Data
// Package: DataCore
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Connector_INCLUDED
#define SQL_Connector_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/SessionImpl.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace SQL {


class Poco_SQL_API Connector
	/// A Connector creates SessionImpl objects.
	///
	/// Every connector library (like the SQLite or the ODBC connector)
	/// provides a subclass of this class, an instance of which is
	/// registered with the SessionFactory.
{
public:
	typedef Poco::SharedPtr<Connector> Ptr;

	Connector();
		/// Creates the Connector.

	virtual ~Connector();
		/// Destroys the Connector.

	virtual const std::string& name() const = 0;
		/// Returns the name associated with this connector.

	virtual SessionImpl::Ptr createSession(const std::string& connectionString,
		std::size_t timeout = SessionImpl::LOGIN_TIMEOUT_DEFAULT) = 0;
		/// Create a SessionImpl object and initialize it with the given connectionString.
};


} } // namespace Poco::SQL


#endif // Data_Connector_INCLUDED
