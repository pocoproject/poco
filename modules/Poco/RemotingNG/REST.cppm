//
// REST.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG_REST
#include "Poco/RemotingNG/REST/CookieStore.h"
#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/REST/FormDeserializer.h"
#include "Poco/RemotingNG/REST/FormSerializer.h"
#include "Poco/RemotingNG/REST/HeaderDeserializer.h"
#include "Poco/RemotingNG/REST/HeaderSerializer.h"
#include "Poco/RemotingNG/REST/JSArray.h"
#include "Poco/RemotingNG/REST/JSObject.h"
#include "Poco/RemotingNG/REST/JSONDeserializer.h"
#include "Poco/RemotingNG/REST/JSONParser.h"
#include "Poco/RemotingNG/REST/JSONSerializer.h"
#include "Poco/RemotingNG/REST/JSScalar.h"
#include "Poco/RemotingNG/REST/JSValue.h"
#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/RemotingNG/REST/PathDeserializer.h"
#include "Poco/RemotingNG/REST/PathSerializer.h"
#include "Poco/RemotingNG/REST/RawDeserializer.h"
#include "Poco/RemotingNG/REST/RawSerializer.h"
#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/RESTRequestHandlerFactory.h"
#include "Poco/RemotingNG/REST/RESTRequestHandler.h"
#include "Poco/RemotingNG/REST/RESTServerException.h"
#include "Poco/RemotingNG/REST/ScalarDeserializer.h"
#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/REST/TransportFactory.h"
#include "Poco/RemotingNG/REST/Transport.h"
#endif

export module Poco:RemotingNG.REST;

export namespace Poco::RemotingNG::REST {
	#ifdef ENABLE_REMOTINGNG_REST
	using Poco::RemotingNG::REST::CookieStore;
	using Poco::RemotingNG::REST::Deserializer;
	using Poco::RemotingNG::REST::FormDeserializer;
	using Poco::RemotingNG::REST::FormSerializer;
	using Poco::RemotingNG::REST::HeaderDeserializer;
	using Poco::RemotingNG::REST::HeaderSerializer;
	using Poco::RemotingNG::REST::JSArray;
	using Poco::RemotingNG::REST::JSONDeserializer;
	using Poco::RemotingNG::REST::JSONParser;
	using Poco::RemotingNG::REST::JSONSerializer;
	using Poco::RemotingNG::REST::JSObject;
	using Poco::RemotingNG::REST::JSScalar;
	using Poco::RemotingNG::REST::JSValue;
	using Poco::RemotingNG::REST::Listener;
	using Poco::RemotingNG::REST::PathDeserializer;
	using Poco::RemotingNG::REST::PathSerializer;
	using Poco::RemotingNG::REST::RESTRequestHandler;
	using Poco::RemotingNG::REST::RESTRequestHandlerFactory;
	using Poco::RemotingNG::REST::RESTServerException;
	using Poco::RemotingNG::REST::RawDeserializer;
	using Poco::RemotingNG::REST::RawSerializer;
	using Poco::RemotingNG::REST::ScalarDeserializer;
	using Poco::RemotingNG::REST::ScalarSerializer;
	using Poco::RemotingNG::REST::Serializer;
	using Poco::RemotingNG::REST::Transport;
	using Poco::RemotingNG::REST::TransportFactory;
	#endif
}
