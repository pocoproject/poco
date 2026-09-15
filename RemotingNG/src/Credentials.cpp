//
// Credentials.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  Credentials
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Credentials.h"


namespace Poco {
namespace RemotingNG {


const std::string Credentials::ATTR_USERNAME("n");
const std::string Credentials::ATTR_PASSWORD("p");
const std::string Credentials::ATTR_MECHANISM("#m");
const std::string Credentials::ATTR_TOKEN("t");
const std::string Credentials::ATTR_NONCE("r");
const std::string Credentials::ATTR_SALT("s");
const std::string Credentials::ATTR_ITERATIONS("i");
const std::string Credentials::ATTR_SIGNATURE("v");
const std::string Credentials::ATTR_CREATED("c");


Credentials::Credentials()
{
}


Credentials::Credentials(const Credentials& other):
	AttributedObject(other)
{
}


Credentials::Credentials(Credentials&& other) noexcept:
	AttributedObject(std::move(other))
{
}

	
Credentials& Credentials::operator = (const Credentials& other)
{
	AttributedObject::operator = (other);
	return *this;
}


Credentials& Credentials::operator = (Credentials&& other) noexcept
{
	AttributedObject::operator = (std::move(other));
	return *this;
}

	
Credentials::~Credentials()
{
}


} } // namespace Poco::RemotingNG
