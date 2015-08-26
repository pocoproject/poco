//
// XMLStreamParserException.cpp
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  XMLStreamParserException
//
// Definition of the XMLStreamParserException class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


#include "Poco/XML/XMLStreamParserException.h"
#include "Poco/XML/XMLStreamParser.h"

using namespace std;

namespace Poco
{
namespace XML
{


XMLStreamParserException::~XMLStreamParserException() throw ()
{
}


XMLStreamParserException::XMLStreamParserException(const string& n, Poco::UInt64 l, Poco::UInt64 c, const string& d)
	: _name(n), _line(l), _column(c), _description(d)
{
	init();
}


XMLStreamParserException::XMLStreamParserException(const XMLStreamParser& p, const std::string& d)
	: _name(p.inputName()), _line(p.line()), _column(p.column()), _description(d)
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


const char* XMLStreamParserException::name() const throw()
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

char const* XMLStreamParserException::what() const throw ()
{
	return _what.c_str();
}


} /* namespace XML */
} /* namespace Poco */
