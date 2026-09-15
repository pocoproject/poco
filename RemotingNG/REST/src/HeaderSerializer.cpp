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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
