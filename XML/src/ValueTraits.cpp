// file      : cutl/xml/value-traits.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "XMLStreamParser.h"
#include "XMLStreamParserException.h"

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
