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
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/QName.h"


namespace Poco::XSD::Types {


QName::QName(const std::string& name, const std::string& ns):
	_name(name), 
	_it(NamespaceManager::instance().set(ns))
{
}


QName::~QName() = default;


QName::QName(const QName&) = default;


QName& QName::operator = (const QName&) = default;


QName::QName():
	_name(), 
	_it(NamespaceManager::instance().end())
{
}


const QName QName::INVALID;


} // namespace Poco::XSD::Types
