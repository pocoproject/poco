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
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Union_INCLUDED
#define XSDTypes_Union_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/SimpleType.h"


namespace Poco::XSD::Types {


class XSDTypes_API Union: public SimpleTypeInheritance
	/// This class represents a Union in an XML Schema.
{
public:
	using Ptr = AutoPtr<Union>;

	Union(const std::string& id, const std::vector<QName>& memberTypes);
		/// Creates the Union.

	~Union() override;
		/// Destroys the Union.

	[[nodiscard]] const std::vector<QName>& typeReferences() const;
		/// Returns all the types that are referenced.

	[[nodiscard]] const std::vector<SimpleType::Ptr>& inlineTypes() const;
		/// Returns all types defined internally.

	[[nodiscard]] std::vector<SimpleType::Ptr>& inlineTypes();
		/// Returns all types defined internally. If you change this vector you must call fixup later.

	[[nodiscard]] const std::vector<const Type*>& types() const override;
		/// Returns the type that we use in the union. Are guaranteed to be all SimpleTypes.

	void fixup() override;
		/// Replaces type references with the referenced type object.

	[[nodiscard]] bool isRestriction() const override;
		/// True if we inherit by restriction.

	[[nodiscard]] bool isList() const override;
		/// Inherit by list.

	[[nodiscard]] bool isUnion() const override;

	void accept(Visitor& v) const override;

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


} // namespace Poco::XSD::Types


#endif // XSDTypes_Union_INCLUDED
