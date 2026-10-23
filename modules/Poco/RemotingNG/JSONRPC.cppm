//
// JSONRPC.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG_JSONRPC
#include "Poco/RemotingNG/JSONRPC/CookieStore.h"
#include "Poco/RemotingNG/JSONRPC/Deserializer.h"
#include "Poco/RemotingNG/JSONRPC/JSArray.h"
#include "Poco/RemotingNG/JSONRPC/JSObject.h"
#include "Poco/RemotingNG/JSONRPC/JSONParser.h"
#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandlerFactory.h"
#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandler.h"
#include "Poco/RemotingNG/JSONRPC/JSScalar.h"
#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include "Poco/RemotingNG/JSONRPC/Listener.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/TransportFactory.h"
#include "Poco/RemotingNG/JSONRPC/Transport.h"
#endif

export module Poco:RemotingNG.JSONRPC;

export namespace Poco::RemotingNG::JSONRPC {
	#ifdef ENABLE_REMOTINGNG_JSONRPC
	using Poco::RemotingNG::JSONRPC::CookieStore;
	using Poco::RemotingNG::JSONRPC::Deserializer;
	using Poco::RemotingNG::JSONRPC::JSArray;
	using Poco::RemotingNG::JSONRPC::JSONParser;
	using Poco::RemotingNG::JSONRPC::JSONRPCRequestHandler;
	using Poco::RemotingNG::JSONRPC::JSONRPCRequestHandlerFactory;
	using Poco::RemotingNG::JSONRPC::JSObject;
	using Poco::RemotingNG::JSONRPC::JSScalar;
	using Poco::RemotingNG::JSONRPC::JSValue;
	using Poco::RemotingNG::JSONRPC::Listener;
	using Poco::RemotingNG::JSONRPC::Serializer;
	using Poco::RemotingNG::JSONRPC::Transport;
	using Poco::RemotingNG::JSONRPC::TransportFactory;
	#endif
}
