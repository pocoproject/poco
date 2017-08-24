//
// Parser.cpp
//
// $Id$
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


#include "Poco/JSON/Parser.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Ascii.h"
#include "Poco/Token.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/String.h"
#include "Poco/StreamCopier.h"
#undef min
#undef max
#include <limits>
#include <clocale>
#include <istream>


namespace Poco {
namespace JSON {


#ifdef PD_JSON_PARSER


ParserImpl::ParserImpl(const Handler::Ptr& pHandler, std::size_t bufSize):
	_pHandler(pHandler),
	_depth(JSON_UNLIMITED_DEPTH),
	_decimalPoint('.'),
	_allowNullByte(true),
	_allowComments(false)
{
}


ParserImpl::~ParserImpl()
{
}


void ParserImpl::handle(const std::string& json)
{
	if (!_allowNullByte && json.find("\\u0000") != json.npos)
		throw JSONException("Null bytes in strings not allowed.");

	try
	{
		json_open_buffer(&_json, json.data(), json.size());
		checkError();
		//////////////////////////////////
		// Underlying parser is capable of parsing multiple consecutive JSONs;
		// we do not currently support this feature; to force error on
		// excessive characters past valid JSON end, this MUST be called
		// AFTER opening the buffer - otherwise it is overwritten by
		// json_open*() call, which calls internal init()
		json_set_streaming(&_json, false);
		/////////////////////////////////
		handle(); checkError();
		if (JSON_DONE != json_next(&_json))
			throw JSONException("Excess characters found after JSON end.");
		json_close(&_json);
	}
	catch (std::exception&)
	{
		json_close(&_json);
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
	else handle(json);

	return asVarImpl();
}


Dynamic::Var ParserImpl::parseImpl(std::istream& in)
{
	std::ostringstream os;
	StreamCopier::copyStream(in, os);
	return parseImpl(os.str());
}


void ParserImpl::stripComments(std::string& json)
{
	if (_allowComments)
	{
		bool inString = false;
		bool inComment = false;
		char prevChar = 0;
		std::string::iterator it = json.begin();
		for (; it != json.end();)
		{
			if (*it == '"' && !inString) inString = true;
			else inString = false;
			if (!inString)
			{
				if (*it == '/' && it + 1 != json.end() && *(it + 1) == '*')
					inComment = true;
			}
			if (inComment)
			{
				char c = *it;
				it = json.erase(it);
				if (prevChar == '*' && c == '/')
				{
					inComment = false;
					prevChar = 0;
				}
				else prevChar = c;
			}
			else ++it;
		}
	}
}


void ParserImpl::handleArray()
{
	json_type tok = json_peek(&_json);
	while (tok != JSON_ARRAY_END && checkError())
	{
		handle();
		tok = json_peek(&_json);
	}

	if (tok == JSON_ARRAY_END) handle();
	else throw JSONException("JSON array end not found");
}


void ParserImpl::handleObject()
{
	json_type tok = json_peek(&_json);
	while (tok != JSON_OBJECT_END && checkError())
	{
		json_next(&_json);
		if (_pHandler) _pHandler->key(std::string(json_get_string(&_json, NULL)));
		handle();
		tok = json_peek(&_json);
	}

	if (tok == JSON_OBJECT_END) handle();
	else throw JSONException("JSON object end not found");
}


void ParserImpl::handle()
{
	enum json_type type = json_next(&_json);
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
		{
			if (_pHandler)
			{
				std::string str(json_get_string(&_json, NULL));
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
		}
		case JSON_STRING:
			if (_pHandler) _pHandler->value(std::string(json_get_string(&_json, NULL)));
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
			const char* pErr = json_get_error(&_json);
			std::string err(pErr ? pErr : "JSON parser error.");
			throw JSONException(err);
		}
	}
}


#elif defined(JSON_ORG_PARSER)


static const unsigned char UTF8_LEAD_BITS[4] = { 0x00, 0xC0, 0xE0, 0xF0 };


const int ParserImpl::_asciiClass[] = 
{
    xx,      xx,      xx,      xx,      xx,      xx,      xx,      xx,
    xx,      C_WHITE, C_WHITE, xx,      xx,      C_WHITE, xx,      xx,
    xx,      xx,      xx,      xx,      xx,      xx,      xx,      xx,
    xx,      xx,      xx,      xx,      xx,      xx,      xx,      xx,

    C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_STAR,  C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
    C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
    C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,

    C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,

    C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
    C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
};


const int ParserImpl::_stateTransitionTable[NR_STATES][NR_CLASSES] = 
{
/*
                 white                                      1-9                                   ABCDF  etc
             space |  {  }  [  ]  :  ,  "  \  /  +  -  .  0  |  a  b  c  d  e  f  l  n  r  s  t  u  |  E  |  * */
/*start  GO*/ {GO,GO,-6,xx,-5,xx,xx,xx,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*ok     OK*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*object OB*/ {OB,OB,xx,-9,xx,xx,xx,xx,SB,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*key    KE*/ {KE,KE,xx,xx,xx,xx,xx,xx,SB,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*colon  CO*/ {CO,CO,xx,xx,xx,xx,-2,xx,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*value  VA*/ {VA,VA,-6,xx,-5,xx,xx,xx,SB,xx,CB,xx,MX,xx,ZX,IX,xx,xx,xx,xx,xx,FA,xx,NU,xx,xx,TR,xx,xx,xx,xx,xx},
/*array  AR*/ {AR,AR,-6,xx,-5,-7,xx,xx,SB,xx,CB,xx,MX,xx,ZX,IX,xx,xx,xx,xx,xx,FA,xx,NU,xx,xx,TR,xx,xx,xx,xx,xx},
/*string ST*/ {ST,xx,ST,ST,ST,ST,ST,ST,-4,EX,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST},
/*escape EC*/ {xx,xx,xx,xx,xx,xx,xx,xx,ST,ST,ST,xx,xx,xx,xx,xx,xx,ST,xx,xx,xx,ST,xx,ST,ST,xx,ST,U1,xx,xx,xx,xx},
/*u1     U1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,U2,U2,U2,U2,U2,U2,U2,U2,xx,xx,xx,xx,xx,xx,U2,U2,xx,xx},
/*u2     U2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,U3,U3,U3,U3,U3,U3,U3,U3,xx,xx,xx,xx,xx,xx,U3,U3,xx,xx},
/*u3     U3*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,U4,U4,U4,U4,U4,U4,U4,U4,xx,xx,xx,xx,xx,xx,U4,U4,xx,xx},
/*u4     U4*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,UC,UC,UC,UC,UC,UC,UC,UC,xx,xx,xx,xx,xx,xx,UC,UC,xx,xx},
/*minus  MI*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,ZE,IT,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*zero   ZE*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,CB,xx,xx,DF,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*int    IT*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,CB,xx,xx,DF,IT,IT,xx,xx,xx,xx,DE,xx,xx,xx,xx,xx,xx,xx,xx,DE,xx,xx},
/*frac   FR*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,CB,xx,xx,xx,FR,FR,xx,xx,xx,xx,E1,xx,xx,xx,xx,xx,xx,xx,xx,E1,xx,xx},
/*e      E1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,E2,E2,xx,E3,E3,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*ex     E2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,E3,E3,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*exp    E3*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,xx,xx,xx,xx,E3,E3,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*tr     T1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,T2,xx,xx,xx,xx,xx,xx,xx},
/*tru    T2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,T3,xx,xx,xx,xx},
/*1      T3*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,OK,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*fa     F1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,F2,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*fal    F2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,F3,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*fals   F3*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,F4,xx,xx,xx,xx,xx,xx},
/*0      F4*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,OK,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*nu     N1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,N2,xx,xx,xx,xx},
/*nul    N2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,N3,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*null   N3*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,CB,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,OK,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*/      C1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,C2},
/*/*     C2*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
/**      C3*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,CE,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
/*_.     FX*/ {OK,OK,xx,-8,xx,-7,xx,-3,xx,xx,xx,xx,xx,xx,FR,FR,xx,xx,xx,xx,E1,xx,xx,xx,xx,xx,xx,xx,xx,E1,xx,xx},
/*\      D1*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,D2,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx},
/*\      D2*/ {xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,U1,xx,xx,xx,xx},
};


ParserImpl::ParserImpl(const Handler::Ptr& pHandler, std::size_t bufSize):
	_pHandler(pHandler),
	_state(GO),
	_beforeCommentState(0),
	_type(JSON_T_NONE),
	_escaped(0),
	_comment(0),
	_utf16HighSurrogate(0),
	_depth(JSON_UNLIMITED_DEPTH),
	_top(-1),
	_stack(JSON_PARSER_STACK_SIZE),
	_parseBuffer(bufSize),
	_decimalPoint('.'),
	_allowNullByte(true),
	_allowComments(false)
{
	_parseBuffer.resize(0);
	push(MODE_DONE);
}


ParserImpl::~ParserImpl()
{
}


void ParserImpl::resetImpl()
{
	_state = GO;
	_beforeCommentState = 0;
	_type = JSON_T_NONE;
	_escaped = 0;
	_utf16HighSurrogate = 0;
	_top = -1;

	_stack.clear();
	_parseBuffer.resize(0);
	push(MODE_DONE);
	if (_pHandler) _pHandler->reset();
}


Dynamic::Var ParserImpl::parseImpl(const std::string& json)
{
	std::string::const_iterator it = json.begin();
	std::string::const_iterator end = json.end();
	Source<std::string::const_iterator> source(it, end);

	int c = 0;
	while (source.nextChar(c))
	{
		if (0 == parseChar(c, source))
			throw SyntaxException("JSON syntax error");
	}

	if (!done())
		throw JSONException("JSON syntax error");

	return asVarImpl();
}


Dynamic::Var ParserImpl::parseImpl(std::istream& in)
{
	std::istreambuf_iterator<char> it(in.rdbuf());
	std::istreambuf_iterator<char> end;
	Source<std::istreambuf_iterator<char> > source(it, end);

	int c = 0;
	while (source.nextChar(c))
	{
		if (0 == parseChar(c, source)) throw JSONException("JSON syntax error");
	}

	if (!done())
		throw JSONException("JSON syntax error");

	return asVarImpl();
}


bool ParserImpl::push(int mode)
{
	_top += 1;
	if (_depth < 0)
	{
		if (_top >= _stack.size())
			_stack.resize(_stack.size() * 2, true);
	}
	else
	{
		if (_top >= _depth) return false;
	}

	_stack[_top] = mode;
	return true;
}


bool ParserImpl::pop(int mode)
{
	if (_top < 0 || _stack[_top] != mode)
		return false;

	_top -= 1;
	return true;
}


void ParserImpl::clearBuffer()
{
	_parseBuffer.resize(0);
}


void ParserImpl::parseBufferPopBackChar()
{
	poco_assert(_parseBuffer.size() >= 1);
	_parseBuffer.resize(_parseBuffer.size() - 1);
}


void ParserImpl::parseBufferPushBackChar(char c)
{
	if (_parseBuffer.size() + 1 >= _parseBuffer.capacity())
		_parseBuffer.setCapacity(_parseBuffer.capacity() * 2);

	_parseBuffer.append(c);
}


void ParserImpl::addEscapedCharToParseBuffer(CharIntType nextChar)
{
	_escaped = 0;
	// remove the backslash
	parseBufferPopBackChar();

	switch (nextChar)
	{
	case 'b':
		parseBufferPushBackChar('\b');
		break;
	case 'f':
		parseBufferPushBackChar('\f');
		break;
	case 'n':
		parseBufferPushBackChar('\n');
		break;
	case 'r':
		parseBufferPushBackChar('\r');
		break;
	case 't':
		parseBufferPushBackChar('\t');
		break;
	case '"':
		parseBufferPushBackChar('"');
		break;
	case '\\':
		parseBufferPushBackChar('\\');
		break;
	case '/':
		parseBufferPushBackChar('/');
		break;
	case 'u':
		parseBufferPushBackChar('\\');
		parseBufferPushBackChar('u');
		break;
	default:
		break;
	}
}


void ParserImpl::addCharToParseBuffer(CharIntType nextChar, int nextClass)
{
	if (_escaped)
	{
		addEscapedCharToParseBuffer(nextChar);
		return;
	}
	else if (!_comment)
	{
		if ((_type != JSON_T_NONE) ||
			!((nextClass == C_SPACE) || (nextClass == C_WHITE)))
		{
			parseBufferPushBackChar((char) nextChar);
		}
	}
}


ParserImpl::CharIntType ParserImpl::decodeUnicodeChar()
{
	int i;
	unsigned uc = 0;
	char* p;
	int trailBytes;

	poco_assert(_parseBuffer.size() >= 6);
	p = &_parseBuffer[_parseBuffer.size() - 4];

	for (i = 12; i >= 0; i -= 4, ++p)
	{
		unsigned x = *p;

		if (x >= 'a')      x -= ('a' - 10);
		else if (x >= 'A') x -= ('A' - 10);
		else               x &= ~0x30u;

		poco_assert(x < 16);
		uc |= x << i;
	}

	if (!_allowNullByte && uc == 0) return 0; 

	// clear UTF-16 char from buffer
	_parseBuffer.resize(_parseBuffer.size() - 6);

	if (_utf16HighSurrogate)
	{
		if (isLowSurrogate(uc))
		{
			uc = decodeSurrogatePair(_utf16HighSurrogate, uc);
			trailBytes = 3;
			_utf16HighSurrogate = 0;
		}
		else // high surrogate without a following low surrogate
		{
			return 0;
		}
	}
	else
	{
		if (uc < 0x80)
		{
			trailBytes = 0;
		}
		else if (uc < 0x800)
		{
			trailBytes = 1;
		}
		else if (isHighSurrogate(uc))
		{
			// save the high surrogate and wait for the low surrogate
			_utf16HighSurrogate = uc;
			return 1;
		}
		else if (isLowSurrogate(uc))
		{
			// low surrogate without a preceding high surrogate 
			return 0;
		}
		else
		{
			trailBytes = 2;
		}
	}

	_parseBuffer.append((char) ((uc >> (trailBytes * 6)) | UTF8_LEAD_BITS[trailBytes]));

	for (i = trailBytes * 6 - 6; i >= 0; i -= 6)
	{
		_parseBuffer.append((char) (((uc >> i) & 0x3F) | 0x80));
	}

	return 1;
}


void ParserImpl::parseBuffer()
{
	if (_pHandler)
	{
		int type = _type; // just to silence g++

		if (type != JSON_T_NONE)
		{
			assertNonContainer();

			switch (type)
			{
			case JSON_T_TRUE:
				{
					_pHandler->value(true);
					break;
				}
			case JSON_T_FALSE:
				{
					_pHandler->value(false);
					break;
				}
			case JSON_T_NULL:
				{
					_pHandler->null();
					break;
				}
			case JSON_T_FLOAT:
				{ 
					// Float can't end with a dot
					if (_parseBuffer[_parseBuffer.size() - 1] == '.') throw SyntaxException("JSON syntax error");

					double float_value = NumberParser::parseFloat(std::string(_parseBuffer.begin(), _parseBuffer.size()));
					_pHandler->value(float_value);
					break;
				}
			case JSON_T_INTEGER:
				{
#if defined(POCO_HAVE_INT64)
					std::string numStr(_parseBuffer.begin(), _parseBuffer.size());
					try
					{
						Poco::trimInPlace(numStr);
						Int64 value = NumberParser::parse64(numStr);
						// if number is 32-bit, then handle as such
						if (value > std::numeric_limits<int>::max()
						 || value < std::numeric_limits<int>::min())
						{
							_pHandler->value(value);
						}
						else
						{
							_pHandler->value(static_cast<int>(value));
						}
					}
					// try to handle error as unsigned in case of overflow
					catch (const SyntaxException&)
					{
						UInt64 value = NumberParser::parseUnsigned64(numStr);
						// if number is 32-bit, then handle as such
						if (value > std::numeric_limits<unsigned>::max())
						{
							_pHandler->value(value);
						}
						else
						{
							_pHandler->value(static_cast<unsigned>(value));
						}
					}
#else
					try
					{
						int value = NumberParser::parse(numStr);
						_pHandler->value(value);
					}
					// try to handle error as unsigned in case of overflow
					catch (const SyntaxException&)
					{
						unsigned value = NumberParser::parseUnsigned(numStr);
						_pHandler->value(value);
					}
#endif
				}
				break;
			case JSON_T_STRING:
				{
					_pHandler->value(std::string(_parseBuffer.begin(), _parseBuffer.size()));
					break;
				}
			}
		}
	}

	clearBuffer();
}


int ParserImpl::utf8CheckFirst(char byte)
{
	unsigned char u = (unsigned char) byte;

	if (u < 0x80)
		return 1;

	if (0x80 <= u && u <= 0xBF) 
	{
		// second, third or fourth byte of a multi-byte
		// sequence, i.e. a "continuation byte"
		return 0;
	}
	else if (u == 0xC0 || u == 0xC1) 
	{
		// overlong encoding of an ASCII byte
		return 0;
	}
	else if (0xC2 <= u && u <= 0xDF) 
	{
		// 2-byte sequence
		return 2;
	}
	else if (0xE0 <= u && u <= 0xEF) 
	{
		// 3-byte sequence
		return 3;
	}
	else if (0xF0 <= u && u <= 0xF4) 
	{
		// 4-byte sequence
		return 4;
	}
	else 
	{ 
		// u >= 0xF5
		// Restricted (start of 4-, 5- or 6-byte sequence) or invalid UTF-8
		return 0;
	}
}


#endif // JSON_ORG_PARSER


} } // namespace Poco::JSON
