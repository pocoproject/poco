//
// AttributeHolder.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeHolder
//
// Definition of the AttributeHolder class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AttributeHolder_INCLUDED
#define XSDTypes_AttributeHolder_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include <vector>


namespace Poco::XSD::Types {


class SimpleType;


class XSDTypes_API AttributeHolder
	/// Interface for classes storing attributes.
{
public:
	AttributeHolder();
		/// Creates the AttributeHolder.

	virtual ~AttributeHolder();
		/// Destroys the AttributeHolder.

	[[nodiscard]] virtual const std::vector<AttributeContent::Ptr>& attributeContent() const = 0;
		/// Returns the attributes defined for the complex type.

	virtual void addAttribute(AttributeContent::Ptr pAttr) = 0;
		/// Adds the attribute to the set.

	[[nodiscard]] virtual bool hasAnyAttribute() const = 0;
		/// Returns true if the any attribute is allowed.
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeHolder_INCLUDED
