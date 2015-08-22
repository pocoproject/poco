// file      : cutl/xml/QName.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "QName.h"
#include <ostream>

using namespace std;

namespace Poco
{
namespace XML
{

string QName::string() const
{
	std::string r;
	if (!ns_.empty())
	{
		r += ns_;
		r += '#';
	}

	r += name_;
	return r;
}

ostream& operator<<(ostream& os, const QName& qn)
{
	return os << qn.string();
}

}
}
