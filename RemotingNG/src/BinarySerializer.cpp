//
// BinarySerializer.cpp
//
// Library: RemotingNG
// Package: Serialization
// Module:  BinarySerializer
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/BinarySerializer.h"
#include <typeinfo>


namespace Poco {
namespace RemotingNG {


const std::string BinarySerializer::MESSAGE_START_TAG("!msg");
const std::string BinarySerializer::MESSAGE_END_TAG("!eom");


BinarySerializer::BinarySerializer()
{
}


BinarySerializer::~BinarySerializer()
{
}


void BinarySerializer::serializeEndPoint(const std::string& oid, const std::string& tid)
{
	*_pWriter << tid << oid;
}


void BinarySerializer::serializeMessageBegin(const std::string& name, SerializerBase::MessageType type)
{
	poco_check_ptr (_pWriter.get());

	_pWriter->writeRaw(MESSAGE_START_TAG);
	Poco::UInt8 code(0);
	switch (type)
	{
	case SerializerBase::MESSAGE_REQUEST:
		code = MESSAGE_CODE_REQUEST;
		break;
	case SerializerBase::MESSAGE_REPLY:
		code = MESSAGE_CODE_REPLY;
		break;
	case SerializerBase::MESSAGE_EVENT:
		code = MESSAGE_CODE_EVENT;
		break;
	case SerializerBase::MESSAGE_EVENT_REPLY:
		code = MESSAGE_CODE_EVENT_REPLY;
		break;
	case SerializerBase::MESSAGE_FAULT:
		code = MESSAGE_CODE_FAULT;
		break;
	default:
		poco_bugcheck();
	}
	*_pWriter << code << name;
}


void BinarySerializer::serializeMessageEnd(const std::string& /*name*/, SerializerBase::MessageType /*type*/)
{
	_pWriter->writeRaw(MESSAGE_END_TAG);
}


void BinarySerializer::serializeFaultMessage(const std::string& name, const Poco::Exception& exc)
{
	serializeMessageBegin(name, SerializerBase::MESSAGE_FAULT);
	*_pWriter << exc.name() << exc.message() << std::string(typeid(exc).name());
	serializeMessageEnd(name, SerializerBase::MESSAGE_FAULT);
}


void BinarySerializer::serializeStructBegin(const std::string& /*name*/)
{
}


void BinarySerializer::serializeStructEnd(const std::string& /*name*/)
{
}


void BinarySerializer::serializeSequenceBegin(const std::string& /*name*/, Poco::UInt32 length)
{
	_pWriter->write7BitEncoded(length);
}


void BinarySerializer::serializeSequenceEnd(const std::string& /*name*/)
{
}


void BinarySerializer::serializeNullableBegin(const std::string& /*name*/, bool isNull)
{
	*_pWriter << isNull;
}


void BinarySerializer::serializeNullableEnd(const std::string& /*name*/)
{
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::Int8 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::UInt8 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::Int16 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::UInt16 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::Int32 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::UInt32 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, long value)
{
	*_pWriter << static_cast<Poco::Int64>(value);
}


void BinarySerializer::serialize(const std::string& /*name*/, unsigned long value)
{
	*_pWriter << static_cast<Poco::UInt64>(value);
}


#ifndef POCO_INT64_IS_LONG
void BinarySerializer::serialize(const std::string& /*name*/, Poco::Int64 value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, Poco::UInt64 value)
{
	*_pWriter << value;
}
#endif


void BinarySerializer::serialize(const std::string& /*name*/, float value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, double value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, bool value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, char value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, const std::string& value)
{
	*_pWriter << value;
}


void BinarySerializer::serialize(const std::string& /*name*/, const std::vector<char>& value)
{
	Poco::UInt32 length = static_cast<Poco::UInt32>(value.size());
	_pWriter->write7BitEncoded(length);
	if (length > 0)
	{
		_pWriter->writeRaw(&value[0], length);
	}
}


void BinarySerializer::resetImpl()
{
}


void BinarySerializer::setupImpl(std::ostream& ostr)
{
	_pWriter.reset(new Poco::BinaryWriter(ostr));
	_pWriter->writeBOM();
}


} } // namespace Poco::RemotingNG
