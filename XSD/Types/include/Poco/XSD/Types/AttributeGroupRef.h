//
// AttributeGroupRef.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeGroupRef
//
// Definition of the AttributeGroupRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AttributeGroupRef_INCLUDED
#define XSDTypes_AttributeGroupRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/AbstractAttributeGroup.h"


namespace Poco::XSD::Types {


class XSDTypes_API AttributeGroupRef: public AbstractAttributeGroup
	/// This class represents a reference to a group of attributes.
{
public:
	using Ptr = AutoPtr<AttributeGroupRef>;

	AttributeGroupRef(const std::string& id, const QName& ref);
		/// Creates the AttributeGroupRef.

	~AttributeGroupRef() override;
		/// Destroys the AttributeGroupRef.

	// AbstractAttributeGroup
	[[nodiscard]] const AbstractAttributeGroup::Attributes& getAttributes() const override;
	[[nodiscard]] bool hasAnyAttribute() const override;
	[[nodiscard]] AnyAttribute::Ptr getAny() const override;
	[[noreturn]] void add(AttributeContent::Ptr ptr) override;
	void fixup() override;
	void accept(Visitor& v) const override;

private:
	QName _ref;
	const AbstractAttributeGroup* _pGroup = nullptr;
};


//
// inlines
//
inline void AttributeGroupRef::add([[maybe_unused]] AttributeContent::Ptr ptr)
{
	throw Poco::NoPermissionException("Adding attributes to refs not allowed!");
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeGroupRef_INCLUDED
