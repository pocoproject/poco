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
	QName()
	{
	}
	QName(const std::string& name) :
		name_(name)
	{
	}
	QName(const std::string& ns, const std::string& name) :
		ns_(ns),
		name_(name)
	{
	}
	QName(const std::string& ns, const std::string& name, const std::string& prefix) :
		ns_(ns),
		name_(name),
		prefix_(prefix)
	{
	}

	const std::string& namespace_() const
	{
		return ns_;
	}
	const std::string& name() const
	{
		return name_;
	}
	const std::string& prefix() const
	{
		return prefix_;
	}

	std::string& namespace_()
	{
		return ns_;
	}
	std::string& name()
	{
		return name_;
	}
	std::string& prefix()
	{
		return prefix_;
	}

	// Printable representation in the [<namespace>#]<name> form.
	//
	std::string string() const;

	// Note that comparison operators
	//
public:
	friend bool operator<(const QName& x, const QName& y)
	{
		return x.ns_ < y.ns_ || (x.ns_ == y.ns_ && x.name_ < y.name_);
	}

	friend bool operator==(const QName& x, const QName& y)
	{
		return x.ns_ == y.ns_ && x.name_ == y.name_;
	}

	friend bool operator!=(const QName& x, const QName& y)
	{
		return !(x == y);
	}

private:
	std::string ns_;
	std::string name_;
	std::string prefix_;
};

XML_API std::ostream& operator<<(std::ostream&, const QName&);
}
}

#endif // CUTL_XML_QNAME_HXX
