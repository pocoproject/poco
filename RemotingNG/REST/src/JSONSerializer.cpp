//
// JSONSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  JSONSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/JSONSerializer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string JSONSerializer::JSON_ERROR("error");
const std::string JSONSerializer::JSON_CODE("code");
const std::string JSONSerializer::JSON_DETAIL("detail");
const std::string JSONSerializer::JSON_TRUE("true");
const std::string JSONSerializer::JSON_FALSE("false");
const std::string JSONSerializer::JSON_NULL("null");


JSONSerializer::JSONSerializer(std::ostream& stream):
	_pStream(&stream),
	_indent(0)
{
}


JSONSerializer::~JSONSerializer()
{
}


void JSONSerializer::serializeMessageBegin(const std::string&, Poco::RemotingNG::SerializerBase::MessageType type)
{
}


void JSONSerializer::serializeMessageEnd(const std::string&, Poco::RemotingNG::SerializerBase::MessageType type)
{
}


void JSONSerializer::serializeFaultMessage(const std::string& methodName, const Poco::Exception& e)
{
	poco_assert_dbg(_pStream);

	*_pStream <<
		"{\n" <<
		"\t\"" << JSON_ERROR << "\": \"" << e.name() << "\",\n"
		"\t\"" << JSON_DETAIL << "\": " << escapeString(e.message()) << ",\n" <<
		"\t\"" << JSON_CODE << "\": " << e.code() << "\n" <<
		"}\n";
	_pStream->flush();
}


void JSONSerializer::serializeStructBegin(const std::string& name)
{
	if (!_state.empty())
	{
		writeSeparator();
		if (_state.back() != ST_ARRAY)
		{
			*_pStream << std::string(_indent, '\t') << "\"" << name << "\":\n";
		}
	}
	*_pStream << std::string(_indent, '\t') << "{";
	++_indent;
	_state.push_back(ST_OBJECT_FIRST);
}


void JSONSerializer::serializeStructEnd(const std::string& name)
{
	_state.pop_back();
	--_indent;
	*_pStream <<
		"\n" <<
		std::string(_indent, '\t') << "}";
}


void JSONSerializer::serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems)
{
	if (!_state.empty())
	{
		writeSeparator();
		if (_state.back() != ST_ARRAY)
		{
			*_pStream << std::string(_indent, '\t') << "\"" << name << "\":\n";
		}
	}
	*_pStream << std::string(_indent, '\t') << "[";
	++_indent;
	_state.push_back(ST_ARRAY_FIRST);
}


void JSONSerializer::serializeSequenceEnd(const std::string& name)
{
	_state.pop_back();
	--_indent;
	*_pStream <<
		"\n" <<
		std::string(_indent, '\t') << "]";
}


void JSONSerializer::serializeNullableBegin(const std::string& name, bool isNull)
{
	if (isNull)
	{
		serializeData(name, JSON_NULL);
	}
}


void JSONSerializer::serializeNullableEnd(const std::string&)
{
}


void JSONSerializer::serializeOptionalBegin(const std::string& /*name*/, bool /*isSpecified*/)
{
}


void JSONSerializer::serializeOptionalEnd(const std::string&)
{
}


void JSONSerializer::serialize(const std::string& name, Poco::Int8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::UInt8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::Int16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::UInt16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::Int32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::UInt32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, unsigned long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


#ifndef POCO_INT64_IS_LONG
void JSONSerializer::serialize(const std::string& name, Poco::Int64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, Poco::UInt64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}
#endif


void JSONSerializer::serialize(const std::string& name, float val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, double val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void JSONSerializer::serialize(const std::string& name, bool val)
{
	serializeData(name, val? JSON_TRUE : JSON_FALSE);
}


void JSONSerializer::serialize(const std::string& name, char val)
{
	std::string tmp("\"");
	tmp += val;
	tmp += '\"';
	serializeData(name, tmp);
}


void JSONSerializer::serialize(const std::string& name, const std::vector<char>& val)
{
	std::stringstream base64;
	base64 << '\"';
	Poco::Base64Encoder encoder(base64);
	encoder.write(&val[0], static_cast<std::streamsize>(val.size()));
	encoder.close();
	base64 << '\"';
	serializeData(name, base64.str());
}


void JSONSerializer::serialize(const std::string& name, const std::string& val)
{
	serializeData(name, escapeString(val));
}


void JSONSerializer::serializeData(const std::string& name, const std::string& val)
{
	writeSeparator();
	*_pStream << std::string(_indent, '\t');
	if (!_state.empty() && _state.back() != ST_ARRAY)
	{
		*_pStream << "\"" << name << "\": ";
	}
	*_pStream << val;
}


void JSONSerializer::writeSeparator()
{
	if (!_state.empty())
	{
		switch (_state.back())
		{
		case ST_OBJECT_FIRST:
			*_pStream << "\n";
			_state.back() = ST_OBJECT;
			break;
		case ST_ARRAY_FIRST:
			*_pStream << "\n";
			_state.back() = ST_ARRAY;
			break;
		default:
			*_pStream << ",\n";
			break;
		}
	}
}


void JSONSerializer::resetImpl()
{
	_indent = 0;
	_state.clear();
}


void JSONSerializer::setupImpl(std::ostream& stream)
{
	resetImpl();
	_pStream = &stream;
}


std::string JSONSerializer::escapeString(const std::string& val)
{
	std::string str("\"");
	for (std::string::const_iterator it = val.begin(); it != val.end(); ++it)
	{
		switch (*it)
		{
		case '\f':
			str += "\\f";
			break;
		case '\t':
			str += "\\t";
			break;
		case '\r':
			str += "\\r";
			break;
		case '\n':
			str += "\\n";
			break;
		case '\b':
			str += "\\b";
			break;
		case '"':
			str += "\\\"";
			break;
		case '\\':
			str += "\\\\";
			break;
		default:
			if (static_cast<unsigned char>(*it) < ' ')
			{
				str += "\\u";
				Poco::NumberFormatter::appendHex(str, static_cast<unsigned>(static_cast<unsigned char>(*it)), 4);
			}
			else
			{
				str += *it;
			}
		}
	}
	str.append("\"");
	return str;
}


} } } // namespace Poco::RemotingNG::REST
