//
// Connector.h
//
// Definition of the Connector class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Test_Connector_INCLUDED
#define SQL_Test_Connector_INCLUDED


#include "Poco/SQL/Connector.h"


namespace Poco {
namespace SQL {
namespace Test {


class Connector: public Poco::SQL::Connector
	/// Connector instantiates SessionImpl objects for testing.
{
public:
	static const std::string KEY;
		/// Keyword for creating test sessions.

	Connector();
		/// Creates the Connector.

	~Connector();
		/// Destroys the Connector.

	const std::string& name() const;
		/// Returns the name associated with this connector.

	Poco::AutoPtr<Poco::SQL::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a test SessionImpl object and initializes it with the given connectionString.

	static void addToFactory();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory.

	static void removeFromFactory();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::SQL::SessionFactory.
};


///
/// inlines
///
inline const std::string& Connector::name() const
{
	return KEY;
}


} } } // namespace Poco::SQL::Test


#endif // Data_Test_Connector_INCLUDED
