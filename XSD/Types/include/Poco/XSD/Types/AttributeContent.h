//
// AttributeContent.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeContent
//
// Definition of the AttributeContent class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AttributeGroupContent_INCLUDED
#define XSDTypes_AttributeGroupContent_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"


namespace Poco::XSD::Types {


class XSDTypes_API AttributeContent: public AnnotatedObject
	/// The base class for Attribute and AttributeGroup classes.
{
public:
	using Ptr = AutoPtr<AttributeContent>;

	AttributeContent(const std::string& id, const std::string& name);
		/// Creates the AttributeContent.

	~AttributeContent() override;
		/// Destroys the AttributeContent.

	[[nodiscard]] virtual const std::string& name() const;
		/// The name of the attribute. Only set for root level attributes.

	[[nodiscard]] virtual bool isAny() const = 0;
		/// Returns true for the any attribute.

	virtual void fixup() = 0;
		/// Resolves all references to types and AttributeRefs

private:
	std::string _name;
};


//
// inlines
//
inline const std::string& AttributeContent::name() const
{
	return _name;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeGroupContent_INCLUDED
