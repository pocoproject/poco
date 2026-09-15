//
// Serializer.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Serializer
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


const std::string Serializer::JSON_JSONRPC("jsonrpc");
const std::string Serializer::JSON_VERSION("2.0");
const std::string Serializer::JSON_METHOD("method");
const std::string Serializer::JSON_PARAMS("params");
const std::string Serializer::JSON_ID("id");
const std::string Serializer::JSON_RESULT("result");
const std::string Serializer::JSON_ERROR("error");
const std::string Serializer::JSON_CODE("code");
const std::string Serializer::JSON_MESSAGE("message");
const std::string Serializer::JSON_TRUE("true");
const std::string Serializer::JSON_FALSE("false");
const std::string Serializer::JSON_NULL("null");


Serializer::Serializer():
	_pStream(0),
	_indent(0),
	_id(0)
{
}


Serializer::~Serializer()
{
}


void Serializer::setupImpl(std::ostream& out)
{
	resetImpl();
	_pStream = &out;
}


void Serializer::serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pStream);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		*_pStream <<
			"{\n" <<
			"\t\"" << JSON_JSONRPC << "\": \"" << JSON_VERSION << "\",\n" <<
			"\t\"" << JSON_ID << "\": " << _id++ << ",\n" <<
			"\t\"" << JSON_METHOD << "\": \"" << name << "\",\n" <<
			"\t\"" << JSON_PARAMS << "\":\n" <<
			"\t{";
		_indent = 2;
		_state.push_back(ST_OBJECT_FIRST);
		break;

	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		*_pStream <<
			"{\n" <<
			"\t\"" << JSON_JSONRPC << "\": \"" << JSON_VERSION << "\",\n" <<
			"\t\"" << JSON_ID << "\": " << _id++ << ",\n" <<
			"\t\"" << JSON_RESULT << "\":\n" <<
			"\t{";
		_indent = 2;
		_state.push_back(ST_OBJECT_FIRST);
		break;

	default:
		poco_bugcheck();
	}
}


void Serializer::serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pStream);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		_state.pop_back();
		_indent = 0;
		*_pStream <<
			"\n" <<
			"\t}\n" <<
			"}\n";
		_pStream->flush();
		_pStream = 0;
		break;

	default:
		poco_bugcheck();
	}
}


void Serializer::serializeFaultMessage(const std::string& methodName, const Poco::Exception& e)
{
	poco_assert_dbg(_pStream);

	*_pStream <<
		"{\n" <<
		"\t\"" << JSON_JSONRPC << "\": \"" << JSON_VERSION << "\",\n" <<
		"\t\"" << JSON_ERROR << "\":\n" <<
		"\t{\n" <<
		"\t\t\"" << JSON_CODE << "\": " << 32000 << ",\n" <<
		"\t\t\"" << JSON_MESSAGE << "\": " << escapeString(e.displayText()) << "\n" <<
		"\t}\n" <<
		"}\n";
	_pStream->flush();
}


void Serializer::serializeStructBegin(const std::string& name)
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


void Serializer::serializeStructEnd(const std::string& name)
{
	_state.pop_back();
	--_indent;
	*_pStream <<
		"\n" <<
		std::string(_indent, '\t') << "}";
}


void Serializer::serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems)
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


void Serializer::serializeSequenceEnd(const std::string& name)
{
	_state.pop_back();
	--_indent;
	*_pStream <<
		"\n" <<
		std::string(_indent, '\t') << "]";
}


void Serializer::serializeNullableBegin(const std::string& name, bool isNull)
{
	if (isNull)
	{
		serializeData(name, JSON_NULL);
	}
}


void Serializer::serializeNullableEnd(const std::string&)
{
}


void Serializer::serialize(const std::string& name, char val)
{
	std::string tmp("\"");
	tmp += val;
	tmp += '\"';
	serializeData(name, tmp);
}


void Serializer::serialize(const std::string& name, const std::vector<char>& val)
{
	std::stringstream base64;
	base64 << '\"';
	Poco::Base64Encoder encoder(base64);
	encoder.write(&val[0], static_cast<std::streamsize>(val.size()));
	encoder.close();
	base64 << '\"';
	serializeData(name, base64.str());
}


void Serializer::serialize(const std::string& name, const std::string& val)
{
	serializeData(name, escapeString(val));
}


void Serializer::serializeData(const std::string& name, const std::string& val)
{
	writeSeparator();
	*_pStream << std::string(_indent, '\t');
	if (_state.back() != ST_ARRAY)
	{
		*_pStream << "\"" << name << "\": ";
	}
	*_pStream << val;
}


void Serializer::writeSeparator()
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


void Serializer::resetImpl()
{
	_indent = 0;
	_state.clear();
}


std::string Serializer::escapeString(const std::string& val)
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


} } } // namespace Poco::RemotingNG::JSONRPC
