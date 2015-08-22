//
// XMLStreamSerializerException.cpp
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  XMLStreamSerializerException
//
// Definition of the XMLStreamSerializerException class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


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
