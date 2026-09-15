//
// ServerTransport.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  ServerTransport
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/ServerTransport.h"


namespace Poco {
namespace RemotingNG {


ServerTransport::ServerTransport()
{
}


ServerTransport::~ServerTransport()
{
}


bool ServerTransport::authorize(const std::string& method, const std::string& permission)
{
	return true;
}


bool ServerTransport::authenticate(const std::string& method)
{
	return true;
}


void ServerTransport::reportException(const std::string& /* method */, const Poco::Exception& /* exc */)
{
}


} } // namespace Poco::RemotingNG
