//
// RawSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RawSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/RawSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <ostream>


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string RawSerializer::VAL_TRUE("true");
const std::string RawSerializer::VAL_FALSE("false");


RawSerializer::RawSerializer(std::ostream& stream):
	_pStream(&stream)
{
}


RawSerializer::~RawSerializer()
{
}


void RawSerializer::serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_bugcheck();
}


void RawSerializer::serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_bugcheck();
}


void RawSerializer::serializeFaultMessage(const std::string& methodName, const Poco::Exception& /* e */)
{
	poco_bugcheck();
}


void RawSerializer::serializeStructBegin(const std::string& name)
{
	throw Poco::RemotingNG::SerializerException("Cannot serialize a structure using raw format");
}


void RawSerializer::serializeStructEnd(const std::string& name)
{
	poco_bugcheck();
}


void RawSerializer::serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems)
{
	throw Poco::RemotingNG::SerializerException("Cannot serialize a sequence using raw format");
}


void RawSerializer::serializeSequenceEnd(const std::string& name)
{
	poco_bugcheck();
}


void RawSerializer::serializeNullableBegin(const std::string& name, bool isNull)
{
}


void RawSerializer::serializeNullableEnd(const std::string&)
{
}


void RawSerializer::serialize(const std::string& name, Poco::Int8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::UInt8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::Int16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::UInt16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::Int32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::UInt32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, unsigned long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


#ifndef POCO_INT64_IS_LONG
void RawSerializer::serialize(const std::string& name, Poco::Int64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, Poco::UInt64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}
#endif


void RawSerializer::serialize(const std::string& name, float val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, double val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void RawSerializer::serialize(const std::string& name, bool val)
{
	serializeData(name, val? VAL_TRUE : VAL_FALSE);
}


void RawSerializer::serialize(const std::string& name, const std::string& val)
{
	serializeData(name, val);
}


void RawSerializer::serialize(const std::string& name, char val)
{
	std::string tmp;
	tmp += val;
	serializeData(name, tmp);
}


void RawSerializer::serialize(const std::string& name, const std::vector<char>& val)
{
	poco_check_ptr (_pStream);

	_pStream->write(&val[0], static_cast<std::streamsize>(val.size()));
}


void RawSerializer::serializeData(const std::string& name, const std::string& val)
{
	poco_check_ptr (_pStream);

	_pStream->write(val.data(), static_cast<std::streamsize>(val.size()));
}


void RawSerializer::setupImpl(std::ostream& stream)
{
	resetImpl();
	_pStream = &stream;
}


void RawSerializer::resetImpl()
{
	_pStream = 0;
}


} } } // namespace Poco::RemotingNG::REST
