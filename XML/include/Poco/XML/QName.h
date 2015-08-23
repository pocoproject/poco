//
// QName.h
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  QName
//
// Definition of the QName class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef POCO_XML_QNAME_HXX
#define POCO_XML_QNAME_HXX

#include "Poco/XML/XML.h"

#include <string>
#include <iosfwd>


namespace Poco
{
namespace XML
{

// Note that the optional prefix is just a "syntactic sugar". In
// particular, it is ignored by the comparison operators and the
// std::ostream insertion operator.
//
class XML_API QName
{
public:
	QName();
	QName(const std::string& name);
	QName(const std::string& ns, const std::string& name);
	QName(const std::string& ns, const std::string& name, const std::string& prefix);

	const std::string& namespace_() const;
	const std::string& name() const;
	const std::string& prefix() const;
	std::string& namespace_();
	std::string& name();
	std::string& prefix();

	// Printable representation in the [<namespace>#]<name> form.
	//
	std::string string() const;

	// Note that comparison operators
	//
public:
	friend bool operator<(const QName& x, const QName& y)
	{
		return x._ns < y._ns || (x._ns == y._ns && x._name < y._name);
	}

	friend bool operator==(const QName& x, const QName& y)
	{
		return x._ns == y._ns && x._name == y._name;
	}

	friend bool operator!=(const QName& x, const QName& y)
	{
		return !(x == y);
	}

private:
	std::string _ns;
	std::string _name;
	std::string _prefix;
};


inline const std::string& QName::namespace_() const
{
	return _ns;
}


inline const std::string& QName::name() const
{
	return _name;
}


inline const std::string& QName::prefix() const
{
	return _prefix;
}


inline std::string& QName::namespace_()
{
	return _ns;
}


inline std::string& QName::name()
{
	return _name;
}


inline std::string& QName::prefix()
{
	return _prefix;
}


XML_API std::ostream& operator<<(std::ostream&, const QName&);

}
}

#endif // CUTL_XML_QNAME_HXX
