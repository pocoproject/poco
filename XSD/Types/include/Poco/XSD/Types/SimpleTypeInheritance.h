//
// SimpleTypeInheritance.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleTypeInheritance
//
// Definition of the SimpleTypeInheritance class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_SimpleTypeInheritance_INCLUDED
#define XSDTypes_SimpleTypeInheritance_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class Type;


class XSDTypes_API SimpleTypeInheritance: public AnnotatedObject
	/// Describes how a simple type extends from another one.
{
public:
	using Ptr = AutoPtr<SimpleTypeInheritance>;

	SimpleTypeInheritance();
		/// Creates the SimpleTypeInheritance.

	SimpleTypeInheritance(const std::string& id);
		/// Creates the SimpleTypeInheritance.

	virtual ~SimpleTypeInheritance();
		/// Destroys the SimpleTypeInheritance.

	virtual const std::vector<const Type*>& types() const = 0;
		/// Returns the types that we inherit from. Will only contain more than one element for the union case.

	virtual void fixup() = 0;
		/// Replaces type references with the referenced type object.

	virtual bool isRestriction() const = 0;
		/// True if we inherit by restriction.

	virtual bool isList() const = 0;
		/// Inherit by list.

	virtual bool isUnion() const = 0;
		/// Inherit by union.
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_SimpleTypeInheritance_INCLUDED
