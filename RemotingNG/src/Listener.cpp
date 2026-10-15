//
// Listener.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  Listener
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Listener.h"
#include "Poco/Exception.h"


namespace Poco::RemotingNG {


Listener::Listener(const std::string& endPoint): _endPoint(endPoint)
{
	poco_assert_dbg (!_endPoint.empty());
}


Listener::~Listener() = default;


void Listener::setAuthenticator(Authenticator::Ptr pAuthenticator)
{
	_pAuthenticator = pAuthenticator;
}


void Listener::setAuthorizer(Authorizer::Ptr pAuthorizer)
{
	_pAuthorizer = pAuthorizer;
}


} // namespace Poco::RemotingNG
