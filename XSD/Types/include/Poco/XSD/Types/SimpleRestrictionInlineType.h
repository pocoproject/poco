//
// SimpleRestrictionInlineType.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleRestrictionInlineType
//
// Definition of the SimpleRestrictionInlineType class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_SimpleRestrictionInlineType_INCLUDED
#define XSDTypes_SimpleRestrictionInlineType_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class SimpleType;


class XSDTypes_API SimpleRestrictionInlineType: public SimpleTypeInheritance
{
public:
	SimpleRestrictionInlineType(const std::string& id);
		/// Creates the SimpleRestrictionInlineType.

	virtual ~SimpleRestrictionInlineType();
		/// Destroys the SimpleRestrictionInlineType.

	void setType(AutoPtr<SimpleType> ptr);
		/// the inline type definition for the restriction can be set here

	const std::vector<const Type*>& types() const;
		/// Returns the type that we inherit from

	void fixup();
		/// Replaces type references with the referenced type object.

	bool isRestriction() const;
		/// True if we inherit by restriction

	bool isList() const;
		/// True if we inherit by list, thus returns false

	bool isUnion() const;
		/// True if we inherit by Union, thus returns false

	void accept(Visitor& v) const;

private:
	std::vector<const Type*> _baseType;
	SimpleType* _pInlineType;
};


//
// inlines
//
inline const std::vector<const Type*>& SimpleRestrictionInlineType::types() const
{
	return _baseType;
}


inline bool SimpleRestrictionInlineType::isRestriction() const
{
	return true;
}


inline bool SimpleRestrictionInlineType::isList() const
{
	return false;
}


inline bool SimpleRestrictionInlineType::isUnion() const
{
	return false;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_SimpleRestrictionInlineType_INCLUDED
