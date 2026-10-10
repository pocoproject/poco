//
// JSONParser.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONParser
// Module:  JSONParser
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/JSONParser.h"
#include "Poco/RemotingNG/JSONRPC/JSScalar.h"
#include "Poco/RemotingNG/JSONRPC/JSObject.h"
#include "Poco/RemotingNG/JSONRPC/JSArray.h"
#include "Poco/NumberParser.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class DepthGuard
{
public:
	DepthGuard(std::size_t& depth, std::size_t maxDepth):
		_depth(depth)
	{
		if (++_depth > maxDepth) throw Poco::SyntaxException("too many levels");
	}

	~DepthGuard()
	{
		--_depth;
	}

private:
	std::size_t& _depth;
};


JSONParser::JSONParser(std::istream& istr):
	_streambuf(*istr.rdbuf())
{
}


JSONParser::~JSONParser()
{
}


JSValue::Ptr JSONParser::parse()
{
	_depth = 0;
	JSValue::Ptr pResult;
	parseValue(_streambuf.sbumpc(), pResult);
	return pResult;
}


int JSONParser::parseValue(int ch, JSValue::Ptr& pValue)
{
	ch = skipWhitespace(ch);
	if (ch == '{')
		return parseObject(ch, pValue);
	else if (ch == '[')
		return parseArray(ch, pValue);
	else if (!isEOF(ch))
		return parseScalar(ch, pValue);
	else
		throw Poco::DataFormatException("no JSON content found");
}


int JSONParser::parseObject(int ch, JSValue::Ptr& pObject)
{
	poco_assert_dbg (ch == '{');

	DepthGuard depthGuard(_depth, _maxDepth);

	pObject = new JSObject;
	ch = skipWhitespace(_streambuf.sbumpc());
	while (ch != '}')
	{
		std::string name;
		ch = skipWhitespace(ch);
		ch = parseString(ch, name);
		ch = skipWhitespace(ch);
		if (ch != ':') throw Poco::SyntaxException("excepted ':'");
		JSValue::Ptr pValue;
		ch = parseValue(_streambuf.sbumpc(), pValue);
		(*pObject)[name] = pValue;
		ch = skipWhitespace(ch);
		if (ch == ',')
			ch = skipWhitespace(_streambuf.sbumpc());
		else if (ch != '}')
			throw Poco::SyntaxException("expected '}' or ','");
	}
	if (ch == '}') ch = _streambuf.sbumpc();
	return ch;
}


int JSONParser::parseArray(int ch, JSValue::Ptr& pArray)
{
	poco_assert_dbg (ch == '[');

	DepthGuard depthGuard(_depth, _maxDepth);

	pArray = new JSArray;
	ch = skipWhitespace(_streambuf.sbumpc());
	while (ch != ']')
	{
		ch = skipWhitespace(ch);
		JSValue::Ptr pValue;
		ch = parseValue(ch, pValue);
		(*pArray)[pArray->size()] = pValue;
		ch = skipWhitespace(ch);
		if (ch == ',')
			ch = skipWhitespace(_streambuf.sbumpc());
		else if (ch != ']')
			throw Poco::SyntaxException("expected ']' or ','");
	}
	if (ch == ']') ch = _streambuf.sbumpc();
	return ch;
}


int JSONParser::parseScalar(int ch, JSValue::Ptr& pObject)
{
	std::string value;
	if (ch == '"')
	{
		ch = parseString(ch, value);
		pObject = new JSScalar(value, JSScalar::JS_STRING);
	}
	else
	{
		while (!isEOF(ch) && !isWhitespace(ch) && ch != ',' && ch != ']' && ch != '}')
		{
			value += static_cast<char>(ch);
			ch = _streambuf.sbumpc();
		}
		if (value == "true" || value == "false")
			pObject = new JSScalar(value, JSScalar::JS_BOOL);
		else if (value != "null")
			pObject = new JSScalar(value, JSScalar::JS_NUMBER); // TODO: check number syntax
	}
	return ch;
}


int JSONParser::parseString(int ch, std::string& str)
{
	str.clear();
	if (ch != '"') throw Poco::SyntaxException(std::string("string expected, but found '") + std::string(1, ch) + "\'");
	ch = _streambuf.sbumpc();
	while (ch != '"' && !isEOF(ch))
	{
		if (ch == '\\')
		{
			ch = _streambuf.sbumpc();
			switch (ch)
			{
			case '"':
			case '\\':
			case '/':
				str += static_cast<char>(ch);
				break;
			case 'b':
				str += '\b';
				break;
			case 'f':
				str += '\f';
				break;
			case 'n':
				str += '\n';
				break;
			case 'r':
				str += '\r';
				break;
			case 't':
				str += '\t';
				break;
			case 'u':
				ch = parseUnicode(ch, str);
				continue;
			default:
				throw Poco::SyntaxException("invalid escape sequence");
			}
		}
		else
		{
			str += static_cast<char>(ch);
		}
		ch = _streambuf.sbumpc();
	}
	if (ch == '"') ch = _streambuf.sbumpc();
	return ch;
}


int JSONParser::parseUnicode(int ch, std::string& str)
{
	poco_assert (ch == 'u');

	std::string hex;
	ch = _streambuf.sbumpc();
	while (ch != -1 && hex.size() < 4)
	{
		hex += static_cast<char>(ch);
		ch = _streambuf.sbumpc();
	}
	if (hex.size() == 4)
	{
		int code = Poco::NumberParser::parseHex(hex);
		unsigned char buffer[8];
		Poco::UTF8Encoding utf8;
		int n = utf8.convert(code, buffer, sizeof(buffer));
		str.append(reinterpret_cast<char*>(&buffer[0]), n);
	}
	else throw Poco::SyntaxException("bad unicode character reference in string");

	return ch;
}


int JSONParser::skipWhitespace(int ch)
{
	while (isWhitespace(ch))
	{
		ch = _streambuf.sbumpc();
	}
	return ch;
}


} } } // namespace Poco::RemotingNG::JSONRPC
