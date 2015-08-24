//
// ValueTraits.h
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  ValueTraits
//
// Definition of the ValueTraits templates.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


#ifndef POCO_XML_VALUE_TRAITS_HXX
#define POCO_XML_VALUE_TRAITS_HXX


#include "XMLStreamParserException.h"
#include <string>
#include <cstddef> // std::size_t
#include <iostream>
#include <sstream>

namespace Poco
{
namespace XML
{


class XMLStreamParser;
class XMLStreamSerializer;


template<typename T>
struct default_value_traits
{
	static T
	parse(std::string, const XMLStreamParser&);

	static std::string
	serialize(const T&, const XMLStreamSerializer&);
};


template<>
struct XML_API default_value_traits<bool>
{
	static bool
	parse(std::string, const XMLStreamParser&);

	static std::string serialize(bool v, const XMLStreamSerializer&)
	{
		return v ? "true" : "false";
	}
};


template<>
struct XML_API default_value_traits<std::string>
{
	static std::string parse(std::string s, const XMLStreamParser&)
	{
		return s;
	}

	static std::string serialize(const std::string& v, const XMLStreamSerializer&)
	{
		return v;
	}
};


template<typename T>
struct ValueTraits: default_value_traits<T>
{
};


template<typename T, std::size_t N>
struct ValueTraits<T[N]> : default_value_traits<const T*>
{
};


template<typename T>
T default_value_traits<T>::parse(std::string s, const XMLStreamParser& p)
{
	T r;
	std::istringstream is(s);
	if (!(is >> r && is.eof()))
		throw XMLStreamParserException(p, "invalid value '" + s + "'");
	return r;
}


}
}

#endif // CUTL_XML_VALUE_TRAITS_HXX
