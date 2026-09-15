//
// ScalarDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  ScalarDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/ScalarDeserializer.h"
#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


ScalarDeserializer::ScalarDeserializer()
{
}


ScalarDeserializer::~ScalarDeserializer()
{
}


Poco::RemotingNG::SerializerBase::MessageType ScalarDeserializer::findMessage(std::string&)
{
	poco_bugcheck();
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void ScalarDeserializer::deserializeMessageBegin(const std::string&, SerializerBase::MessageType)
{
	poco_bugcheck();
}


void ScalarDeserializer::deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type)
{
	poco_bugcheck();
}


bool ScalarDeserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	throw Poco::RemotingNG::DeserializerException("Cannot deserialize a structure from a header, form or path");
}


void ScalarDeserializer::deserializeStructEnd(const std::string& name)
{
}


bool ScalarDeserializer::deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint)
{
	throw Poco::RemotingNG::DeserializerException("Cannot deserialize a sequence from a header, form or path");
}


void ScalarDeserializer::deserializeSequenceEnd(const std::string& name)
{
}


bool ScalarDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	return false;
}


void ScalarDeserializer::deserializeNullableEnd(const std::string& name)
{
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::Int8>(Poco::NumberParser::parse(strVal));
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::UInt8>(Poco::NumberParser::parseUnsigned(strVal));
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::Int16>(Poco::NumberParser::parse(strVal));
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = static_cast<Poco::UInt16>(Poco::NumberParser::parseUnsigned(strVal));
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse(strVal);
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseUnsigned(strVal);
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, long& val)
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


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
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
bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse64(strVal);
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
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


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = (float)Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	// parse for true/1 or false/0
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		if (Poco::icompare(strVal, ScalarSerializer::VAL_FALSE) == 0 || strVal == "0")
			val = false;
		else if (Poco::icompare(strVal, ScalarSerializer::VAL_TRUE) == 0 || strVal == "1")
			val = true;
		else
		{
			reset();
			throw Poco::RemotingNG::DeserializerException("Illegal bool value ", strVal);
		}
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, char& val)
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


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		val = strVal;
	}
	return ret;
}


bool ScalarDeserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	return deserializeBuffer(name, isMandatory, val);
}


bool ScalarDeserializer::deserializeBuffer(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	bool ret = false;
	const std::string& strVal = deserializeData(name, isMandatory, ret);
	if (ret)
	{
		std::istringstream istr(strVal);
		Poco::Base64Decoder decoder(istr);
		val.clear();
		int ch = decoder.get();
		while (ch != -1)
		{
			val.push_back(static_cast<char>(ch));
			ch = decoder.get();
		}
	}
	return ret;
}


void ScalarDeserializer::resetImpl()
{
}


void ScalarDeserializer::setupImpl(std::istream&)
{
}


} } } // namespace Poco::RemotingNG::REST
