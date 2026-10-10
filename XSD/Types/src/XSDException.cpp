//
// XSDException.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  XSDException
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/XSDException.h"
#include <typeinfo>


namespace Poco::XSD::Types {


POCO_IMPLEMENT_EXCEPTION(XSDException, Exception, "XSD exception")
POCO_IMPLEMENT_EXCEPTION(TypeException, XSDException, "Type exception")
POCO_IMPLEMENT_EXCEPTION(SchemaException, XSDException, "Schema exception")
POCO_IMPLEMENT_EXCEPTION(IllegalOrderException, SchemaException, "illegal order exception")
POCO_IMPLEMENT_EXCEPTION(ElementException, XSDException, "Element exception")
POCO_IMPLEMENT_EXCEPTION(NullTypeException, TypeException, "NULL Type exception")
POCO_IMPLEMENT_EXCEPTION(InvalidTypeException, TypeException, "Invalid Type")
POCO_IMPLEMENT_EXCEPTION(TypeAlreadyDefinedException, TypeException, "Type already defined")
POCO_IMPLEMENT_EXCEPTION(NullElementException, ElementException, "NULL Element exception")
POCO_IMPLEMENT_EXCEPTION(InvalidElementException, ElementException, "Invalid Element")
POCO_IMPLEMENT_EXCEPTION(ElementAlreadyDefinedException, ElementException, "Element already defined")


} // namespace Poco::XSD::Types
