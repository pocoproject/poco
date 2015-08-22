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
#include "Poco/XML/XMLStreamSerializer.h"

using namespace std;

namespace Poco
{
namespace XML
{

XMLStreamSerializerException::~XMLStreamSerializerException() throw ()
{
}

XMLStreamSerializerException::XMLStreamSerializerException(const string& n, const string& d)
	: name_(n), description_(d)
{
	init();
}

XMLStreamSerializerException::XMLStreamSerializerException(const XMLStreamSerializer& s, const std::string& d)
	: name_(s.outputName()), description_(d)
{
	init();
}

void XMLStreamSerializerException::init()
{
	if (!name_.empty())
	{
		what_ += name_;
		what_ += ": ";
	}

	what_ += "error: ";
	what_ += description_;
}


const char* XMLStreamSerializerException::name() const throw ()
{
	return name_.c_str();
}


const std::string& XMLStreamSerializerException::description() const
{
	return description_;
}


char const* XMLStreamSerializerException::what() const throw ()
{
	return what_.c_str();
}

} /* namespace XML */
} /* namespace Poco */
