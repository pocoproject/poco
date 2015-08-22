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
	: name_(n), line_(l), column_(c), description_(d)
{
	init();
}


XMLStreamParserException::XMLStreamParserException(const XMLStreamParser& p, const std::string& d)
	: name_(p.input_name()), line_(p.line()), column_(p.column()), description_(d)
{
	init();
}


void XMLStreamParserException::init()
{
	std::ostringstream os;
	if (!name_.empty())
		os << name_ << ':';
	os << line_ << ':' << column_ << ": error: " << description_;
	what_ = os.str();
}


char const* XMLStreamParserException::what() const throw ()
{
	return what_.c_str();
}


} /* namespace XML */
} /* namespace Poco */
