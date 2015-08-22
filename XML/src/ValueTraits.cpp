//
// ValueTraits.cpp
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


#include "Poco/XML/XMLStreamParser.h"
#include "Poco/XML/XMLStreamParserException.h"


using namespace std;


namespace Poco
{
namespace XML
{


bool default_value_traits<bool>::parse(string s, const XMLStreamParser& p)
{
	if (s == "true" || s == "1" || s == "True" || s == "TRUE")
		return true;
	else if (s == "false" || s == "0" || s == "False" || s == "FALSE")
		return false;
	else
		throw XMLStreamParserException(p, "invalid bool value '" + s + "'");
}


}
}
