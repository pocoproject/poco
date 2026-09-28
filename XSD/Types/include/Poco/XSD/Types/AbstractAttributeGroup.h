//
// AbstractAttributeGroup.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AbstractAttributeGroup
//
// Definition of the AbstractAttributeGroup class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AbstractAttributeGroup_INCLUDED
#define XSDTypes_AbstractAttributeGroup_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include <map>


namespace Poco::XSD::Types {


class XSDTypes_API AbstractAttributeGroup: public AttributeContent
	/// The base class for all attribute groups.
{
public:
	using Attributes = std::map<std::string, AttributeContent::Ptr>;
	using Ptr = AutoPtr<AbstractAttributeGroup>;

	AbstractAttributeGroup(const std::string& id, const std::string& name);
		/// Creates the AbstractAttributeGroup.

	~AbstractAttributeGroup() override;
		/// Destroys the AbstractAttributeGroup.

	[[nodiscard]] virtual const AbstractAttributeGroup::Attributes& getAttributes() const = 0;
		/// Returns all the children.

	[[nodiscard]] virtual bool hasAnyAttribute() const = 0;
		/// Returns true if the any attribute is set.

	[[nodiscard]] virtual AnyAttribute::Ptr getAny() const = 0;
		/// Returns the any attribute

	virtual void add(AttributeContent::Ptr ptr) = 0;
		/// Adds an attribute content child to the group.
		///
		/// Throws an exception if a child with that name already exists.

	// AttributeContent
	[[nodiscard]] bool isAny() const override;
};


//
// inlines
//
inline bool AbstractAttributeGroup::isAny() const
{
	return false;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractAttributeGroup_INCLUDED
