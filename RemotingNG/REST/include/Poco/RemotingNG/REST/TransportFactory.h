//
// TransportFactory.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  TransportFactory
//
// Definition of the TransportFactory class.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_TransportFactory_INCLUDED
#define RemotingNG_REST_TransportFactory_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/TransportFactory.h"


namespace Poco::RemotingNG::REST {


class RemotingNGREST_API TransportFactory: public Poco::RemotingNG::TransportFactory
	/// The TransportFactory for RemotingNG REST Transport objects.
{
public:
	TransportFactory();
		/// Creates a TransportFactory.

	~TransportFactory() override;
		/// Destroys the TransportFactory.

	[[nodiscard]] Poco::RemotingNG::Transport* createTransport() override;

	static void registerFactory();
		/// Helper function to register the factory with the manager.

	static void unregisterFactory();
		/// Helper function to unregister the factory with the manager.
};


} // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_TransportFactory_INCLUDED
