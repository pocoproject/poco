//
// HTTP.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG_HTTP
#include "Poco/RemotingNG/HTTP/HTTP.h"
#include "Poco/RemotingNG/HTTP/HTTPRequestHandlerFactory.h"
#include "Poco/RemotingNG/HTTP/HTTPRequestHandler.h"
#include "Poco/RemotingNG/HTTP/Listener.h"
#include "Poco/RemotingNG/HTTP/TransportFactory.h"
#include "Poco/RemotingNG/HTTP/Transport.h"
#endif

export module Poco:RemotingNG.HTTP;

export namespace Poco::RemotingNG::HTTP {
	#ifdef ENABLE_REMOTINGNG_HTTP
	using Poco::RemotingNG::HTTP::HTTPRequestHandler;
	using Poco::RemotingNG::HTTP::HTTPRequestHandlerFactory;
	using Poco::RemotingNG::HTTP::Listener;
	using Poco::RemotingNG::HTTP::Transport;
	using Poco::RemotingNG::HTTP::TransportFactory;
	#endif
}
