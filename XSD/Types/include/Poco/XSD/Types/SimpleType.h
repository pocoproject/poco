//
// SimpleType.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleType
//
// Definition of the SimpleType class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_SimpleType_INCLUDED
#define XSDTypes_SimpleType_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"


namespace Poco::XSD::Types {


class XSDTypes_API SimpleType: public Type
	/// This class represents a simple XML Schema type.
{
public:
	using Ptr = AutoPtr<SimpleType>;

	SimpleType();
		/// Creates the SimpleType.

	SimpleType(const std::string& id, const std::string& name, bool finalRestriction, bool finalList, bool finalUnion);
		/// Creates the Type.

	~SimpleType() override;
		/// Destroys the SimpleType.

	[[nodiscard]] bool finalRestriction() const;
		/// Returns if inheritance by restriction is final.

	[[nodiscard]] bool finalList() const;
		/// Returns if this is a type that can be used by another one as list content.

	[[nodiscard]] bool finalUnion() const;
		/// Returns if this is a type that can be used by another one as union content.

	void setContent(SimpleTypeInheritance::Ptr pContent);
		// Sets the content

	[[nodiscard]] SimpleTypeInheritance::Ptr getContent() const;
		/// Returns the content. Note that builtin types will have a null content
		/// because they do not inherit from another type.

	void fixup() override;
		/// Resolves type references to a parent class.

	[[nodiscard]] const std::vector<const Type*>& parents() const override;

	void accept(Visitor& v) const override;

	void createIterator(std::vector<OrderIterator>& seq) const override;

private:
	bool _finalRestriction;
	bool _finalList;
	bool _finalUnion;
	SimpleTypeInheritance::Ptr _pContent;
};


//
// inlines
//
inline void SimpleType::setContent(SimpleTypeInheritance::Ptr pContent)
{
	_pContent = pContent;
}


inline SimpleTypeInheritance::Ptr SimpleType::getContent() const
{
	return _pContent;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_SimpleType_INCLUDED
