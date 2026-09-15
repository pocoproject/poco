//
// TransportFactory.h
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  TransportFactory
//
// Definition of the TransportFactory class.
//
// Copyright (c) 2006-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_HTTP_TransportFactory_INCLUDED
#define RemotingNG_HTTP_TransportFactory_INCLUDED


#include "Poco/RemotingNG/HTTP/HTTP.h"
#include "Poco/RemotingNG/TransportFactory.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


class RemotingNGHTTP_API TransportFactory: public Poco::RemotingNG::TransportFactory
	/// The TransportFactory for RemotingNG HTTP Transport objects.
{
public:
	TransportFactory();
		/// Creates a TransportFactory.

	~TransportFactory();
		/// Destroys the TransportFactory.

	Poco::RemotingNG::Transport* createTransport();

	static void registerFactory();
		/// Helper function to register the factory with the manager.

	static void unregisterFactory();
		/// Helper function to unregister the factory with the manager.
};


} } } // namespace Poco::RemotingNG::HTTP


#endif // RemotingNG_HTTP_TransportFactory_INCLUDED
