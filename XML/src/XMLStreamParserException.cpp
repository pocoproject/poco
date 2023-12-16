//
// XMLStreamParserException.cpp
//
// Library: XML
// Package: XML
// Module:  XMLStreamParserException
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/XML/XMLStreamParserException.h"
#include "Poco/XML/XMLStreamParser.h"


namespace Poco {
namespace XML {


XMLStreamParserException::~XMLStreamParserException() noexcept
= default;


XMLStreamParserException::XMLStreamParserException(std::string  n, Poco::UInt64 l, Poco::UInt64 c, std::string  d):
	_name(std::move(n)),
	_line(l),
	_column(c),
	_description(std::move(d))
{
	init();
}


XMLStreamParserException::XMLStreamParserException(const XMLStreamParser& p, std::string  d):
	_name(p.inputName()),
	_line(p.line()),
	_column(p.column()),
	_description(std::move(d))
{
	init();
}


void XMLStreamParserException::init()
{
	std::ostringstream os;
	if (!_name.empty())
		os << _name << ':';
	os << _line << ':' << _column << ": error: " << _description;
	_what = os.str();
}


const char* XMLStreamParserException::name() const noexcept
{
	return _name.c_str();
}


Poco::UInt64 XMLStreamParserException::line() const
{
	return _line;
}


Poco::UInt64 XMLStreamParserException::column() const
{
	return _column;
}


const std::string& XMLStreamParserException::description() const
{
	return _description;
}


char const* XMLStreamParserException::what() const noexcept
{
	return _what.c_str();
}


} } // namespace Poco::XML
