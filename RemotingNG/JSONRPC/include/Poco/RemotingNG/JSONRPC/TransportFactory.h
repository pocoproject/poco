//
// TransportFactory.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  TransportFactory
//
// Definition of the TransportFactory class.
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_TransportFactory_INCLUDED
#define RemotingNG_JSONRPC_TransportFactory_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/TransportFactory.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API TransportFactory: public Poco::RemotingNG::TransportFactory
	/// The TransportFactory for RemotingNG JSONRPC Transport objects.
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


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_TransportFactory_INCLUDED
