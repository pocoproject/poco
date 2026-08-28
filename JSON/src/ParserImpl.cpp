//
// Parser.cpp
//
// Library: JSON
// Package: JSON
// Module:  Parser
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include <Poco/JSON/ParserImpl.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/StreamCopier.h>

#undef min
#undef max

#include <istream>
#include <streambuf>
#include <clocale>
#include <pdjson.h>


using json_stream = struct json_stream;


namespace Poco {
namespace JSON {


extern "C"
{
	static int istream_get(void* ptr)
	{
		auto pBuf = reinterpret_cast<std::streambuf*>(ptr);
		return pBuf->sbumpc();
	}

	static int istream_peek(void* ptr)
	{
		auto pBuf = reinterpret_cast<std::streambuf*>(ptr);
		return pBuf->sgetc();
	}
}


ParserImpl::ParserImpl(const Handler::Ptr& pHandler):
	_pJSON(new json_stream),
	_pHandler(pHandler),
	_depth(JSON_DEFAULT_DEPTH),
	_decimalPoint('.'),
	_allowNullByte(true),
	_allowComments(false)
{
}


ParserImpl::~ParserImpl()
{
	delete _pJSON;
}


void ParserImpl::handle(const std::string& json)
{
	if (!_allowNullByte && json.find("\\u0000") != json.npos)
		throw JSONException("Null bytes in strings not allowed.");

	try
	{
		json_open_buffer(_pJSON, json.data(), json.size());
		checkError();
		//////////////////////////////////
		// Underlying parser is capable of parsing multiple consecutive JSONs;
		// we do not currently support this feature; to force error on
		// excessive characters past valid JSON end, this MUST be called
		// AFTER opening the buffer - otherwise it is overwritten by
		// json_open*() call, which calls internal init()
		json_set_streaming(_pJSON, false);
		/////////////////////////////////
		handle();
		checkError();
		if (JSON_DONE != json_next(_pJSON))
			throw JSONException("Excess characters found after JSON end.");
		json_close(_pJSON);
	}
	catch (...)
	{
		json_close(_pJSON);
		throw;
	}
}


void ParserImpl::handle(std::istream& json)
{
	try
	{
		json_open_user(_pJSON, istream_get, istream_peek, json.rdbuf());
		checkError();
		json_set_streaming(_pJSON, false);
		handle();
		checkError();
		if (JSON_DONE != json_next(_pJSON))
			throw JSONException("Excess characters found after JSON end.");
		json_close(_pJSON);
	}
	catch (...)
	{
		json_close(_pJSON);
		throw;
	}
}


Dynamic::Var ParserImpl::parseImpl(const std::string& json)
{
	if (_allowComments)
	{
		std::string str = json;
		stripComments(str);
		handle(str);
	}
	else
	{
		handle(json);
	}

	return asVarImpl();
}


Dynamic::Var ParserImpl::parseImpl(std::istream& json)
{
	if (_allowComments || !_allowNullByte)
	{
		std::string str;
		Poco::StreamCopier::copyToString(json, str);
		if (_allowComments)
		{
			stripComments(str);
		}
		handle(str);
	}
	else
	{
		handle(json);
	}

	return asVarImpl();
}


void ParserImpl::stripComments(std::string& json)
{
	if (!_allowComments) return;

	// The bundled parser has no notion of comments, so they have to be removed
	// before it sees the document. That means repeating its string lexing here,
	// and any disagreement with it is a defect: text inside a string is data,
	// never a comment. Compacting in place because erasing per character is
	// quadratic, which one long comment turns into a denial of service.
	std::string::iterator out = json.begin();
	bool inString = false;
	bool escaped = false;
	for (std::string::const_iterator it = json.cbegin(); it != json.cend(); ++it)
	{
		if (inString)
		{
			// Only an unescaped quote ends the string, so a comment delimiter
			// inside a value is never taken for a real comment.
			if (escaped) escaped = false;
			else if (*it == '\\') escaped = true;
			else if (*it == '"') inString = false;
		}
		else if (*it == '"')
		{
			inString = true;
		}
		else if (*it == '/' && it + 1 != json.cend() && *(it + 1) == '*')
		{
			// Skip to the closing delimiter. "/*/" does not close the comment:
			// the '/' is part of the opening delimiter.
			it += 2;
			while (it != json.cend() && !(*it == '*' && it + 1 != json.cend() && *(it + 1) == '/'))
				++it;
			if (it == json.cend()) break;
			++it;
			continue;
		}
		*out++ = *it;
	}
	json.erase(out, json.end());
}


void ParserImpl::handleArray()
{
	if (json_get_depth(_pJSON) > _depth)
		throw JSONException("Maximum depth exceeded");

	json_type tok = json_peek(_pJSON);
	while (tok != JSON_ARRAY_END && checkError())
	{
		handle();
		tok = json_peek(_pJSON);
	}

	if (tok == JSON_ARRAY_END)
	{
		handle();
	}
	else throw JSONException("JSON array end not found");
}


void ParserImpl::handleObject()
{
	if (json_get_depth(_pJSON) > _depth)
		throw JSONException("Maximum depth exceeded");

	json_type tok = json_peek(_pJSON);
	while (tok != JSON_OBJECT_END && checkError())
	{
		json_next(_pJSON);
		if (_pHandler)
		{
			// Read the key with its length: the C string form stops at an
			// embedded NUL, which would silently merge distinct keys.
			std::size_t length = 0;
			const char* key = json_get_string(_pJSON, &length);
			_pHandler->key(std::string(key, length == 0 ? 0 : length - 1));
		}
		handle();
		tok = json_peek(_pJSON);
	}

	if (tok == JSON_OBJECT_END)
	{
		handle();
	}
	else throw JSONException("JSON object end not found");
}


void ParserImpl::handle()
{
	enum json_type type = json_next(_pJSON);
	switch (type)
	{
	case JSON_DONE:
		return;
	case JSON_NULL:
		_pHandler->null();
		break;
	case JSON_TRUE:
		if (_pHandler) _pHandler->value(true);
		break;
	case JSON_FALSE:
		if (_pHandler) _pHandler->value(false);
		break;
	case JSON_NUMBER:
		if (_pHandler)
		{
			std::string str(json_get_string(_pJSON, nullptr));
			if (str.find(_decimalPoint) != str.npos || str.find('e') != str.npos || str.find('E') != str.npos)
			{
				_pHandler->value(NumberParser::parseFloat(str));
			}
			else
			{
				Poco::Int64 val;
				if (NumberParser::tryParse64(str, val))
					_pHandler->value(val);
				else
					_pHandler->value(NumberParser::parseUnsigned64(str));
			}
		}
		break;
	case JSON_STRING:
		if (_pHandler)
		{
			std::size_t length = 0;
			const char* val = json_get_string(_pJSON, &length);
			_pHandler->value(std::string(val, length == 0 ? 0 : length - 1)); // Decrease the length by 1 because it also contains the terminating null character
		}
		break;
	case JSON_OBJECT:
		if (_pHandler) _pHandler->startObject();
		handleObject();
		break;
	case JSON_OBJECT_END:
		if (_pHandler) _pHandler->endObject();
		return;
	case JSON_ARRAY:
		if (_pHandler) _pHandler->startArray();
		handleArray();
		break;
	case JSON_ARRAY_END:
		if (_pHandler) _pHandler->endArray();
		return;
	case JSON_ERROR:
		{
			const char* pErr = json_get_error(_pJSON);
			std::string err(pErr ? pErr : "JSON parser error.");
			throw JSONException(err);
		}
	}
}


bool ParserImpl::checkError()
{
	const char* err = json_get_error(_pJSON);
	if (err) throw Poco::JSON::JSONException(err);
	return true;
}


} } // namespace Poco::JSON
