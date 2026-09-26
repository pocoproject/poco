//
// Union.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Union
//
// Definition of the Union class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Union_INCLUDED
#define XSDTypes_Union_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/SimpleType.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Union: public SimpleTypeInheritance
	/// This class represents a Union in an XML Schema.
{
public:
	using Ptr = AutoPtr<Union>;

	Union(const std::string& id, const std::vector<QName>& memberTypes);
		/// Creates the Union.

	virtual ~Union();
		/// Destroys the Union.

	const std::vector<QName>& typeReferences() const;
		/// Returns all the types that are referenced.

	const std::vector<SimpleType::Ptr>& inlineTypes() const;
		/// Returns all types defined internally.

	std::vector<SimpleType::Ptr>& inlineTypes();
		/// Returns all types defined internally. If you change this vector you must call fixup later.

	const std::vector<const Type*>& types() const;
		/// Returns the type that we use in the union. Are guaranteed to be all SimpleTypes.

	void fixup();
		/// Replaces type references with the referenced type object.

	bool isRestriction() const;
		/// True if we inherit by restriction.

	bool isList() const;
		/// Inherit by list.

	bool isUnion() const;

	void accept(Visitor& v) const;

private:
	std::vector<QName> _memberTypes;
	std::vector<SimpleType::Ptr> _inlineTypes;
	std::vector<const Type*> _allTypes;
};


//
// inlines
//
inline const std::vector<QName>& Union::typeReferences() const
{
	return _memberTypes;
}


inline const std::vector<SimpleType::Ptr>& Union::inlineTypes() const
{
	return _inlineTypes;
}


inline std::vector<SimpleType::Ptr>& Union::inlineTypes()
{
	return _inlineTypes;
}


inline const std::vector<const Type*>& Union::types() const
{
	return _allTypes;
}


inline bool Union::isRestriction() const
{
	return false;
}


inline bool Union::isList() const
{
	return false;
}


inline bool Union::isUnion() const
{
	return true;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Union_INCLUDED
