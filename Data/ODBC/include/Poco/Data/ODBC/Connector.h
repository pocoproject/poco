//
// Connector.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Connector
//
// Definition of the Connector class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_Connector_INCLUDED
#define Data_ODBC_Connector_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/Connector.h"


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API Connector: public Poco::Data::Connector
	/// Connector instantiates SqLite SessionImpl objects.
{
public:
	static const std::string KEY;
		/// Keyword for creating ODBC sessions.

	Connector();
		/// Creates the Connector.

	~Connector();
		/// Destroys the Connector.

	const std::string& name() const;
		/// Returns the name associated with this connector.

	Poco::AutoPtr<Poco::Data::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a ODBC SessionImpl object and initializes it with the given connectionString.

	static void registerConnector();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory

	static void unregisterConnector();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory
};


///
/// inlines
///
inline const std::string& Connector::name() const
{
	return KEY;
}


} } } // namespace Poco::Data::ODBC


#endif // Data_ODBC_Connector_INCLUDED
