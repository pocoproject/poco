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


#ifndef Data_Test_Connector_INCLUDED
#define Data_Test_Connector_INCLUDED


#include "Poco/Data/Connector.h"


namespace Poco::Data::Test {


class Connector: public Poco::Data::Connector
	/// Connector instantiates SessionImpl objects for testing.
{
public:
	static const std::string KEY;
		/// Keyword for creating test sessions.

	static const std::string UNREACHABLE;
		/// Connection string for which createSession() fails,
		/// as if the database could not be reached.

	Connector();
		/// Creates the Connector.

	~Connector();
		/// Destroys the Connector.

	const std::string& name() const;
		/// Returns the name associated with this connector.

	Poco::AutoPtr<Poco::Data::SessionImpl> createSession(const std::string& connectionString,
		std::size_t timeout = Poco::Data::SessionImpl::LOGIN_TIMEOUT_DEFAULT);
		/// Creates a test SessionImpl object and initializes it with the given connectionString.
		/// Throws ConnectionFailedException for the UNREACHABLE connection string.

	static void addToFactory();
		/// Registers the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory.

	static void removeFromFactory();
		/// Unregisters the Connector under the Keyword Connector::KEY at the Poco::Data::SessionFactory.
};


///
/// inlines
///
inline const std::string& Connector::name() const
{
	return KEY;
}


} // namespace Poco::Data::Test


#endif // Data_Test_Connector_INCLUDED
