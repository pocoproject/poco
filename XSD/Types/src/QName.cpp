//
// QName.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  QName
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


QName::QName(const std::string& name, const std::string& ns):
	_name(name), 
	_it(NamespaceManager::instance().set(ns))
{
}


QName::~QName()
{
}


QName::QName(const QName& qname):
	_name(qname.name()), 
	_it(qname.getIterator())
{
}


QName& QName::operator = (const QName& other)
{
	if (&other != this)
	{
		_name = other._name;
		_it = other._it;
	}

	return *this;
}


QName::QName():
	_name(), 
	_it(NamespaceManager::instance().end())
{
}


const QName QName::INVALID;


} } } // namespace Poco::XSD::Types
