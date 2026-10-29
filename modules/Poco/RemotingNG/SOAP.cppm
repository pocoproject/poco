//
// SOAP.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG_SOAP
#include "Poco/RemotingNG/SOAP/CookieStore.h"
#include "Poco/RemotingNG/SOAP/Deserializer.h"
#include "Poco/RemotingNG/SOAP/Listener.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/SOAP/SOAPContentHandler.h"
#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/SOAPRequestHandler.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/WSDLRequestHandler.h"
#include "Poco/RemotingNG/SOAP/XMLElement.h"
#endif

export module Poco:RemotingNG.SOAP;

export namespace Poco::RemotingNG::SOAP {
	#ifdef ENABLE_REMOTINGNG_SOAP
	using Poco::RemotingNG::SOAP::CookieStore;
	using Poco::RemotingNG::SOAP::Deserializer;
	using Poco::RemotingNG::SOAP::Listener;
	using Poco::RemotingNG::SOAP::SOAPContentHandler;
	using Poco::RemotingNG::SOAP::SOAPFaultException;
	using Poco::RemotingNG::SOAP::SOAPRequestHandler;
	using Poco::RemotingNG::SOAP::Serializer;
	using Poco::RemotingNG::SOAP::Transport;
	using Poco::RemotingNG::SOAP::TransportFactory;
	using Poco::RemotingNG::SOAP::WSDLRequestHandler;
	using Poco::RemotingNG::SOAP::XMLElement;
	#endif
}
