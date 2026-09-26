//
// SimpleRestriction.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleRestriction
//
// Definition of the SimpleRestriction class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_SimpleRestriction_INCLUDED
#define XSDTypes_SimpleRestriction_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API SimpleRestriction: public SimpleTypeInheritance
	/// This class represents a simple restriction in an XML Schema.
{
public:
	SimpleRestriction(const std::string& id, const QName& baseClass);
		/// Creates the SimpleRestriction.

	virtual ~SimpleRestriction();
		/// Destroys the SimpleRestriction.

	const QName& baseTypeRef() const;
		/// Returns the type reference of the base class.

	const std::vector<const Type*>& types() const;
		/// Returns the type that we inherit from.

	void fixup();
		/// Replaces type references with the referenced type object.

	bool isRestriction() const;
		/// True if we inherit by restriction.

	bool isList() const;
		/// True if we inherit by list, thus returns false.

	bool isUnion() const;
		/// True if we inherit by Union, thus returns false.

	void accept(Visitor& v) const;

private:
	QName       _baseClass;
	std::vector<const Type*> _baseType;
};


//
// inlines
//
inline const QName& SimpleRestriction::baseTypeRef() const
{
	return _baseClass;
}


inline const std::vector<const Type*>& SimpleRestriction::types() const
{
	return _baseType;
}


inline bool SimpleRestriction::isRestriction() const
{
	return true;
}


inline bool SimpleRestriction::isList() const
{
	return false;
}


inline bool SimpleRestriction::isUnion() const
{
	return false;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_SimpleRestriction_INCLUDED
