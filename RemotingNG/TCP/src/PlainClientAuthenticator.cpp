//
// PlainClientAuthenticator.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  PlainClientAuthenticator
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/PlainClientAuthenticator.h"


namespace Poco::RemotingNG::TCP {


const std::string PlainClientAuthenticator::PLAIN("PLAIN");


PlainClientAuthenticator::PlainClientAuthenticator() = default;

	
PlainClientAuthenticator::~PlainClientAuthenticator() = default;


std::string PlainClientAuthenticator::startAuthentication(Credentials&)
{
	return PLAIN;
}


bool PlainClientAuthenticator::continueAuthentication(const Credentials&, Credentials&)
{
	return false;
}


} // namespace Poco::RemotingNG::TCP
