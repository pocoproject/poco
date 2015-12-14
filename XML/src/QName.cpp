//
// QName.cpp
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  QName
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Based on libstudxml (http://www.codesynthesis.com/projects/libstudxml/).
// Copyright (c) 2009-2013 Code Synthesis Tools CC.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XML/QName.h"
#include <ostream>


namespace Poco {
namespace XML {


QName::QName()
{
}


QName::QName(const std::string& name) :
	_name(name)
{
}


QName::QName(const std::string& ns, const std::string& name) :
	_ns(ns),
	_name(name)
{
}


QName::QName(const std::string& ns, const std::string& name, const std::string& rPrefix) :
	_ns(ns),
	_name(name),
	_prefix(rPrefix)
{
}


std::string QName::toString() const
{
	std::string r;
	if (!_ns.empty())
	{
		r += _ns;
		r += '#';
	}

	r += _name;
	return r;
}


std::ostream& operator << (std::ostream& os, const QName& qn)
{
	return os << qn.toString();
}


} } // namespace Poco::XML
