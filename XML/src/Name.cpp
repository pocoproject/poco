//
// Name.cpp
//
// $Id: //poco/1.4/XML/src/Name.cpp#1 $
//
// Library: XML
// Package: XML
// Module:  Name
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XML/Name.h"
#include <algorithm>


namespace Poco {
namespace XML {


const XMLString Name::EMPTY_NAME;


Name::Name()
{
}


Name::Name(const XMLString& rQname):
	_qname(rQname)
{
}


Name::Name(const XMLString& rQname, const XMLString& rNamespaceURI):
	_qname(rQname),
	_namespaceURI(rNamespaceURI),
	_localName(localName(rQname))
{
}


Name::Name(const XMLString& rQname, const XMLString& rNamespaceURI, const XMLString& rLocalName):
	_qname(rQname),
	_namespaceURI(rNamespaceURI),
	_localName(rLocalName)
{
}


Name::Name(const Name& name):
	_qname(name._qname),
	_namespaceURI(name._namespaceURI),
	_localName(name._localName)
{
}

	
Name::~Name()
{
}


Name& Name::operator = (const Name& name)
{
	if (this != &name)
	{
		_qname        = name._qname;
		_namespaceURI = name._namespaceURI;
		_localName    = name._localName;
	}
	return *this;
}


void Name::swap(Name& name)
{
	std::swap(_qname, name._qname);
	std::swap(_namespaceURI, name._namespaceURI);
	std::swap(_localName, name._localName);
}


void Name::assign(const XMLString& rQname)
{
	_qname        = rQname;
	_namespaceURI.clear();
	_localName.clear();
}


void Name::assign(const XMLString& rQname, const XMLString& rNamespaceURI)
{
	_qname        = rQname;
	_namespaceURI = rNamespaceURI;
	_localName    = localName(rQname);
}


void Name::assign(const XMLString& rQname, const XMLString& rNamespaceURI, const XMLString& rLocalName)
{
	_qname        = rQname;
	_namespaceURI = rNamespaceURI;
	_localName    = rLocalName;
}


bool Name::equals(const Name& name) const
{
	return name._namespaceURI == _namespaceURI && name._localName == _localName && name._qname == _qname;
}


bool Name::equals(const XMLString& rQname, const XMLString& rNamespaceURI, const XMLString& rLocalName) const
{
	return _namespaceURI == rNamespaceURI && _localName == rLocalName && _qname == rQname;
}


bool Name::equalsWeakly(const XMLString& rQname, const XMLString& rNamespaceURI, const XMLString& rLocalName) const
{
	return (_qname == rQname && !rQname.empty()) || (_namespaceURI == rNamespaceURI && _localName == rLocalName && !_localName.empty());
}


XMLString Name::prefix() const
{
	return prefix(_qname);
}


void Name::split(const XMLString& rQname, XMLString& prefix, XMLString& rLocalName)
{
	XMLString::size_type pos = rQname.find(':');
	if (pos != XMLString::npos)
	{
		prefix.assign(rQname, 0, pos);
		rLocalName.assign(rQname, pos + 1, rQname.size() - pos - 1);
	}
	else
	{
		prefix.clear();
		rLocalName.assign(rQname);
	}
}


XMLString Name::localName(const XMLString& rQname)
{
	XMLString::size_type pos = rQname.find(':');
	if (pos != XMLString::npos) 
		return XMLString(rQname, pos + 1, rQname.size() - pos - 1);
	else
		return rQname;
}


XMLString Name::prefix(const XMLString& rQname)
{
	XMLString::size_type pos = rQname.find(':');
	if (pos != XMLString::npos)
		return XMLString(rQname, 0, pos);
	else
		return EMPTY_NAME;
}


} } // namespace Poco::XML
