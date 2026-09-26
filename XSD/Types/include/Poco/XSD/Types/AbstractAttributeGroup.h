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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_AbstractAttributeGroup_INCLUDED
#define XSDTypes_AbstractAttributeGroup_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AbstractAttributeGroup: public AttributeContent
	/// The base class for all attribute groups.
{
public:
	using Attributes = std::map<std::string, AttributeContent::Ptr>;
	using Ptr = AutoPtr<AbstractAttributeGroup>;

	AbstractAttributeGroup(const std::string& id, const std::string& name);
		/// Creates the AbstractAttributeGroup.

	virtual ~AbstractAttributeGroup();
		/// Destroys the AbstractAttributeGroup.

	virtual const AbstractAttributeGroup::Attributes& getAttributes() const = 0;
		/// Returns all the children.

	virtual bool hasAnyAttribute() const = 0;
		/// Returns true if the any attribute is set.

	virtual AnyAttribute::Ptr getAny() const = 0;
		/// Returns the any attribute

	virtual void add(AttributeContent::Ptr ptr) = 0;
		/// Adds an attribute content child to the group.
		///
		/// Throws an exception if a child with that name already exists.

	// AttributeContent
	bool isAny() const;
};


//
// inlines
//
inline bool AbstractAttributeGroup::isAny() const
{
	return false;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractAttributeGroup_INCLUDED
