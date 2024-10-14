//
// Format.cpp
//
// Library: Foundation
// Package: Core
// Module:  Format
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Ascii.h"
#include <sstream>
#if !defined(POCO_NO_LOCALE)
#include <locale>
#endif
#include <cstddef>
#include <string_view>


namespace Poco {


namespace
{
	void parseFlags(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
	{
		bool isFlag = true;
		while (isFlag && itFmt != endFmt)
		{
			switch (*itFmt)
			{
			case '-': str.setf(std::ios::left); ++itFmt; break;
			case '+': str.setf(std::ios::showpos); ++itFmt; break;
			case '0': str.fill('0'); str.setf(std::ios::internal); ++itFmt; break;
			case '#': str.setf(std::ios::showpoint | std::ios::showbase); ++itFmt; break;
			default:  isFlag = false; break;
			}
		}
	}


	void parseWidth(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt, std::vector<Any>::const_iterator& itVal)
	{
		int width = 0;
		if (itFmt != endFmt && *itFmt == '*')
		{
			++itFmt;
			width = AnyCast<int>(*itVal++);
		}
		else
		{
		while (itFmt != endFmt && Ascii::isDigit(*itFmt))
		{
			width = 10*width + *itFmt - '0';
			++itFmt;
		}
		}
		if (width > 0) str.width(width);
	}


	void parsePrec(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt, std::vector<Any>::const_iterator& itVal)
	{
		if (itFmt != endFmt && *itFmt == '.')
		{
			++itFmt;
			int prec = 0;
			if (itFmt != endFmt && *itFmt == '*')
			{
				++itFmt;
				prec = AnyCast<int>(*itVal++);
			}
			else
			{
				while (itFmt != endFmt && Ascii::isDigit(*itFmt))
				{
					prec = 10*prec + *itFmt - '0';
					++itFmt;
				}
			}
			if (prec >= 0) str.precision(prec);
		}
	}

	char parseMod(std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
	{
		char mod = 0;
		if (itFmt != endFmt)
		{
			switch (*itFmt)
			{
			case 'l':
			case 'h':
			case 'L':
			case '?': mod = *itFmt++; break;
			}
		}
		return mod;
	}

	std::size_t parseIndex(std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
	{
		int index = 0;
		while (itFmt != endFmt && Ascii::isDigit(*itFmt))
		{
			index = 10*index + *itFmt - '0';
			++itFmt;
		}
		if (itFmt != endFmt && *itFmt == ']') ++itFmt;
		return index;
	}

	void prepareFormat(std::ostream& str, char type)
	{
		switch (type)
		{
		case 'd':
		case 'i': str << std::dec; break;
		case 'o': str << std::oct; break;
		case 'x': str << std::hex; break;
		case 'X': str << std::hex << std::uppercase; break;
		case 'e': str << std::scientific; break;
		case 'E': str << std::scientific << std::uppercase; break;
		case 'f': str << std::fixed; break;
		}
	}


	void writeAnyInt(std::ostream& str, const Any& any)
	{
		if (any.type() == typeid(char))
			str << static_cast<int>(AnyCast<char>(any));
		else if (any.type() == typeid(signed char))
			str << static_cast<int>(AnyCast<signed char>(any));
		else if (any.type() == typeid(unsigned char))
			str << static_cast<unsigned>(AnyCast<unsigned char>(any));
		else if (any.type() == typeid(short))
			str << AnyCast<short>(any);
		else if (any.type() == typeid(unsigned short))
			str << AnyCast<unsigned short>(any);
		else if (any.type() == typeid(int))
			str << AnyCast<int>(any);
		else if (any.type() == typeid(unsigned int))
			str << AnyCast<unsigned int>(any);
		else if (any.type() == typeid(long))
			str << AnyCast<long>(any);
		else if (any.type() == typeid(unsigned long))
			str << AnyCast<unsigned long>(any);
		else if (any.type() == typeid(Int64))
			str << AnyCast<Int64>(any);
		else if (any.type() == typeid(UInt64))
			str << AnyCast<UInt64>(any);
		else if (any.type() == typeid(bool))
			str << AnyCast<bool>(any);
	}


	void formatOne(std::string& result, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt, std::vector<Any>::const_iterator& itVal)
	{
		std::ostringstream str;
#if !defined(POCO_NO_LOCALE)
		str.imbue(std::locale::classic());
#endif
		try
		{
			parseFlags(str, itFmt, endFmt);
			parseWidth(str, itFmt, endFmt, itVal);
			parsePrec(str, itFmt, endFmt, itVal);
			char mod = parseMod(itFmt, endFmt);
			if (itFmt != endFmt)
			{
				char type = *itFmt++;
				prepareFormat(str, type);
				switch (type)
				{
				case 'b':
					str << AnyCast<bool>(*itVal++);
					break;
				case 'c':
					str << AnyCast<char>(*itVal++);
					break;
				case 'd':
				case 'i':
					switch (mod)
					{
					case 'l': str << AnyCast<long>(*itVal++); break;
					case 'L': str << AnyCast<Int64>(*itVal++); break;
					case 'h': str << AnyCast<short>(*itVal++); break;
					case '?': writeAnyInt(str, *itVal++); break;
					default:  str << AnyCast<int>(*itVal++); break;
					}
					break;
				case 'o':
				case 'u':
				case 'x':
				case 'X':
					switch (mod)
					{
					case 'l': str << AnyCast<unsigned long>(*itVal++); break;
					case 'L': str << AnyCast<UInt64>(*itVal++); break;
					case 'h': str << AnyCast<unsigned short>(*itVal++); break;
					case '?': writeAnyInt(str, *itVal++); break;
					default:  str << AnyCast<unsigned>(*itVal++); break;
					}
					break;
				case 'e':
				case 'E':
				case 'f':
					switch (mod)
					{
					case 'l': str << AnyCast<long double>(*itVal++); break;
					case 'L': str << AnyCast<long double>(*itVal++); break;
					case 'h': str << AnyCast<float>(*itVal++); break;
					default:  str << AnyCast<double>(*itVal++); break;
					}
					break;
				case 's':
					str << RefAnyCast<std::string>(*itVal++);
					break;
				case 'v':
					str << RefAnyCast<std::string_view>(*itVal++);
					break;
				case 'z':
					str << AnyCast<std::size_t>(*itVal++);
					break;
				case 'I':
				case 'D':
				default:
					str << type;
				}
			}
		}
		catch (Poco::BadCastException&)
		{
			str << "[ERRFMT]";
		}
		result.append(str.str());
	}
}


std::string format(const std::string& fmt, const Any& value)
{
	std::string result;
	format(result, fmt, value);
	return result;
}


void format(std::string& result, const char *fmt, const std::vector<Any>& values)
{
	format(result, std::string(fmt), values);
}


void format(std::string& result, const std::string& fmt, const std::vector<Any>& values)
{
	std::string::const_iterator itFmt  = fmt.begin();
	std::string::const_iterator endFmt = fmt.end();
	std::vector<Any>::const_iterator itVal  = values.begin();
	std::vector<Any>::const_iterator endVal = values.end();
	while (itFmt != endFmt)
	{
		switch (*itFmt)
		{
		case '%':
			++itFmt;
			if (itFmt != endFmt && (itVal != endVal || *itFmt == '['))
			{
				if (*itFmt == '[')
				{
					++itFmt;
					std::size_t index = parseIndex(itFmt, endFmt);
					if (index < values.size())
					{
						std::vector<Any>::const_iterator it = values.begin() + index;
						formatOne(result, itFmt, endFmt, it);
					}
					else throw InvalidArgumentException("format argument index out of range", fmt);
				}
				else
				{
					formatOne(result, itFmt, endFmt, itVal);
				}
			}
			else if (itFmt != endFmt)
			{
				result += *itFmt++;
			}
			break;
		default:
			result += *itFmt;
			++itFmt;
		}
	}
}


} // namespace Poco
