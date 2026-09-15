//
// ScalarSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  ScalarSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string ScalarSerializer::VAL_TRUE("true");
const std::string ScalarSerializer::VAL_FALSE("false");


ScalarSerializer::ScalarSerializer()
{
}


ScalarSerializer::~ScalarSerializer()
{
}


void ScalarSerializer::serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_bugcheck();
}


void ScalarSerializer::serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_bugcheck();
}


void ScalarSerializer::serializeFaultMessage(const std::string& methodName, const Poco::Exception& /* e */)
{
	poco_bugcheck();
}


void ScalarSerializer::serializeStructBegin(const std::string& name)
{
	throw Poco::RemotingNG::SerializerException("Cannot serialize a structure to a header, form or path");
}


void ScalarSerializer::serializeStructEnd(const std::string& name)
{
	poco_bugcheck();
}


void ScalarSerializer::serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems)
{
	throw Poco::RemotingNG::SerializerException("Cannot serialize a sequence to a header, form or path");
}


void ScalarSerializer::serializeSequenceEnd(const std::string& name)
{
	poco_bugcheck();
}


void ScalarSerializer::serializeNullableBegin(const std::string& name, bool isNull)
{
}


void ScalarSerializer::serializeNullableEnd(const std::string&)
{
}


void ScalarSerializer::serialize(const std::string& name, Poco::Int8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::UInt8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::Int16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::UInt16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::Int32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::UInt32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, unsigned long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


#ifndef POCO_INT64_IS_LONG
void ScalarSerializer::serialize(const std::string& name, Poco::Int64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, Poco::UInt64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}
#endif


void ScalarSerializer::serialize(const std::string& name, float val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, double val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void ScalarSerializer::serialize(const std::string& name, bool val)
{
	serializeData(name, val? VAL_TRUE : VAL_FALSE);
}


void ScalarSerializer::serialize(const std::string& name, const std::string& val)
{
	serializeData(name, val);
}


void ScalarSerializer::serialize(const std::string& name, char val)
{
	std::string tmp;
	tmp += val;
	serializeData(name, tmp);
}


void ScalarSerializer::serialize(const std::string& name, const std::vector<char>& val)
{
	serializeBuffer(name, val);
}


void ScalarSerializer::serializeBuffer(const std::string& name, const std::vector<char>& val)
{
	std::stringstream base64;
	Poco::Base64Encoder encoder(base64);
	encoder.write(&val[0], static_cast<std::streamsize>(val.size()));
	encoder.close();
	serializeData(name, base64.str());
}


void ScalarSerializer::setupImpl(std::ostream&)
{
}


void ScalarSerializer::resetImpl()
{
}


} } } // namespace Poco::RemotingNG::REST
