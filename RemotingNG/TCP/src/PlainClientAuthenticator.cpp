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
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/PlainClientAuthenticator.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


const std::string PlainClientAuthenticator::PLAIN("PLAIN");


PlainClientAuthenticator::PlainClientAuthenticator()
{
}

	
PlainClientAuthenticator::~PlainClientAuthenticator()
{
}


std::string PlainClientAuthenticator::startAuthentication(Credentials&)
{
	return PLAIN;
}


bool PlainClientAuthenticator::continueAuthentication(const Credentials&, Credentials&)
{
	return false;
}


} } } // namespace Poco::RemotingNG::TCP
