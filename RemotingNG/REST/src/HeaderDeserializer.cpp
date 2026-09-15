//
// HeaderDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  HeaderDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/HeaderDeserializer.h"
#include "Poco/RemotingNG/REST/HeaderSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string HeaderDeserializer::EMPTY;


HeaderDeserializer::HeaderDeserializer(const Poco::Net::HTTPMessage& message):
	_message(message)
{
}


HeaderDeserializer::~HeaderDeserializer()
{
}


bool HeaderDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	isNull = !_message.has(name);
	return !isNull;
}


const std::string& HeaderDeserializer::deserializeData(const std::string& name, bool isMandatory, bool& found)
{
	if (_message.has(name))
	{
		found = true;
		return _message.get(name);
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	else
	{
		found = false;
		return EMPTY;
	}
}


} } } // namespace Poco::RemotingNG::REST
