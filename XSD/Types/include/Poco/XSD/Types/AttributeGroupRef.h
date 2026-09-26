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


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AttributeGroupRef: public AbstractAttributeGroup
	/// This class represents a reference to a group of attributes.
{
public:
	using Ptr = AutoPtr<AttributeGroupRef>;

	AttributeGroupRef(const std::string& id, const QName& ref);
		/// Creates the AttributeGroupRef.

	virtual ~AttributeGroupRef();
		/// Destroys the AttributeGroupRef.

	// AbstractAttributeGroup
	const AbstractAttributeGroup::Attributes& getAttributes() const;
	bool hasAnyAttribute() const;
	AnyAttribute::Ptr getAny() const;
	void add(AttributeContent::Ptr ptr);
	void fixup();
	void accept(Visitor& v) const;

private:
	QName _ref;
	const AbstractAttributeGroup* _pGroup;
};


//
// inlines
//
inline void AttributeGroupRef::add(AttributeContent::Ptr ptr)
{
	throw Poco::NoPermissionException("Adding attributes to refs not allowed!");
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeGroupRef_INCLUDED
