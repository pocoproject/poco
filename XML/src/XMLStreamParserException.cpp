///
/// \package metamodel
/// \file XMLStreamException.cpp
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date Aug 21, 2015 - 6:52:24 PM
/// \brief definicia typu
///
/// (C) Copyright 2015 R-SYS,s.r.o
/// All rights reserved.
///

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
