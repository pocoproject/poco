//
// TransportFactory.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  TransportFactory
//
// Definition of the TransportFactory class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_TransportFactory_INCLUDED
#define RemotingNG_SOAP_TransportFactory_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/TransportFactory.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API TransportFactory: public Poco::RemotingNG::TransportFactory
	/// The TransportFactory for RemotingNG SOAP Transport objects.
{
public:
	TransportFactory(Serializer::SOAPVersion = Serializer::SOAP_1_1);
		/// Creates a TransportFactory.

	~TransportFactory();
		/// Destroys the TransportFactory.

	Poco::RemotingNG::Transport* createTransport();

	static void registerFactory();
		/// Helper function to register the factory with the manager.
		///
		/// This will actually register three factory instances, one 
		/// for the "soap" protocol (default), one for "soap-1.1" and
		/// one for "soap-1.2".

	static void unregisterFactory();
		/// Helper function to unregister the factory with the manager.

private:
	Serializer::SOAPVersion _soapVersion;
};


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_TransportFactory_INCLUDED
