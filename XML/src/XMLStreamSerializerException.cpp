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
	: _name(n), _description(d)
{
	init();
}

XMLStreamSerializerException::XMLStreamSerializerException(const XMLStreamSerializer& s, const std::string& d)
	: _name(s.outputName()), _description(d)
{
	init();
}

void XMLStreamSerializerException::init()
{
	if (!_name.empty())
	{
		_what += _name;
		_what += ": ";
	}

	_what += "error: ";
	_what += _description;
}


const char* XMLStreamSerializerException::name() const throw ()
{
	return _name.c_str();
}


const std::string& XMLStreamSerializerException::description() const
{
	return _description;
}


char const* XMLStreamSerializerException::what() const throw ()
{
	return _what.c_str();
}

} /* namespace XML */
} /* namespace Poco */
