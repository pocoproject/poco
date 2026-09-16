//
// HeaderSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  HeaderSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/HeaderSerializer.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


HeaderSerializer::HeaderSerializer(Poco::Net::HTTPMessage& message):
	_pMessage(&message)
{
}


HeaderSerializer::~HeaderSerializer()
{
}


void HeaderSerializer::serializeData(const std::string& name, const std::string& val)
{
	_pMessage->set(name, val);
}


} } } // namespace Poco::RemotingNG::REST
