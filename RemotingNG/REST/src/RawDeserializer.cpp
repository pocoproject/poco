//
// RawDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RawDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/RawDeserializer.h"
#include "Poco/RemotingNG/REST/RawSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/NumberParser.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include <istream>


namespace Poco {
namespace RemotingNG {
namespace REST {


RawDeserializer::RawDeserializer(std::istream& stream):
	_pStream(&stream)
{
}


RawDeserializer::~RawDeserializer()
{
}


Poco::RemotingNG::SerializerBase::MessageType RawDeserializer::findMessage(std::string&)
{
	poco_bugcheck();
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void RawDeserializer::deserializeMessageBegin(const std::string&, SerializerBase::MessageType)
{
	poco_bugcheck();
}


void RawDeserializer::deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type)
{
	poco_bugcheck();
}


bool RawDeserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	throw Poco::RemotingNG::DeserializerException("Cannot deserialize a structure using raw format");
}


void RawDeserializer::deserializeStructEnd(const std::string& name)
{
}


bool RawDeserializer::deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint)
{
	throw Poco::RemotingNG::DeserializerException("Cannot deserialize a sequence using raw format");
}


void RawDeserializer::deserializeSequenceEnd(const std::string& name)
{
}


bool RawDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	return false;
}


void RawDeserializer::deserializeNullableEnd(const std::string& name)
{
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::Int8>(Poco::NumberParser::parse(strVal));
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::UInt8>(Poco::NumberParser::parseUnsigned(strVal));
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::Int16>(Poco::NumberParser::parse(strVal));
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::UInt16>(Poco::NumberParser::parseUnsigned(strVal));
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse(strVal);
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseUnsigned(strVal);
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, long& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
#ifndef POCO_INT64_IS_LONG
		val = Poco::NumberParser::parse(strVal);
#else
		val = Poco::NumberParser::parse64(strVal);
#endif
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
#ifndef POCO_INT64_IS_LONG
		val = Poco::NumberParser::parseUnsigned(strVal);
#else
		val = Poco::NumberParser::parseUnsigned64(strVal);
#endif
	}
	return ret;
}


#ifndef POCO_INT64_IS_LONG
bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse64(strVal);
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseUnsigned64(strVal);
	}
	return ret;
}
#endif


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = (float)Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	// parse for true/1 or false/0
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		if (Poco::icompare(strVal, RawSerializer::VAL_FALSE) == 0 || strVal == "0")
			val = false;
		else if (Poco::icompare(strVal, RawSerializer::VAL_TRUE) == 0 || strVal == "1")
			val = true;
		else
		{
			reset();
			throw Poco::RemotingNG::DeserializerException("Illegal bool value ", strVal);
		}
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, char& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		if (strVal.size() != 1)
		{
			reset();
			throw Poco::RemotingNG::DeserializerException("Illegal char value ", strVal);
		}
		val = strVal[0];
	}
	return ret;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	poco_check_ptr (_pStream);

	val.clear();
	Poco::StreamCopier::copyToString(*_pStream, val);
	return true;
}


bool RawDeserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	poco_check_ptr (_pStream);

	val.clear();
	int ch = _pStream->get();
	while (ch != -1)
	{
		val.push_back(static_cast<char>(ch));
		ch = _pStream->get();
	}
	return !val.empty();
}


const std::string& RawDeserializer::deserializeData(const std::string& name, bool isMandatory, bool& found)
{
	poco_check_ptr (_pStream);

	_data.clear();
	Poco::StreamCopier::copyToString(*_pStream, _data);
	found = !_data.empty();
	return _data;
}


void RawDeserializer::resetImpl()
{
	_data.clear();
	_pStream = 0;
}


void RawDeserializer::setupImpl(std::istream& stream)
{
	resetImpl();
	_pStream = &stream;
}


} } } // namespace Poco::RemotingNG::REST
