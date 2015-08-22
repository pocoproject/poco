// file      : cutl/xml/QName.hxx
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

	const std::string& namespace_() const
	{
		return _ns;
	}
	const std::string& name() const
	{
		return _name;
	}
	const std::string& prefix() const
	{
		return _prefix;
	}

	std::string& namespace_()
	{
		return _ns;
	}
	std::string& name()
	{
		return _name;
	}
	std::string& prefix()
	{
		return _prefix;
	}

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

XML_API std::ostream& operator<<(std::ostream&, const QName&);
}
}

#endif // CUTL_XML_QNAME_HXX
