//
// ComplexType.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  ComplexType
//
// Definition of the ComplexType class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_ComplexType_INCLUDED
#define XSDTypes_ComplexType_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/Content.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include "Poco/XSD/Types/AttributeHolder.h"
#include "Poco/XSD/Types/InheritanceInfo.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API ComplexType: public Type
	/// This class represents a complexType in an XML Schema.
{
public:
	using Ptr = AutoPtr<ComplexType>;

	ComplexType(const std::string& id, 
		const std::string& name,
		bool isAbstract,
		bool blockExtension,
		bool blockRestriction,
		bool finalExtension,
		bool finalRestriction,
		bool mixed);
		/// Creates the ComplexType.

	virtual ~ComplexType();
		/// Destroys the ComplexType.


	bool isAbstract() const;

	bool blockExtension() const;

	bool blockRestriction() const;

	bool finalExtension() const;

	bool finalRestriction() const;

	bool mixed() const;

	void setParent(InheritanceInfo::Ptr pInh);

	InheritanceInfo::Ptr getParent() const;

	void setContent(Content::Ptr ptr);

	Content::Ptr getContent() const;

	void fixup();
		/// Resolves type references to a parent class.

	const std::vector<const Type*>& parents() const;

	void accept(Visitor& v) const;

	void addAttribute(AttributeContent::Ptr pAttr);
		/// Adds the attribute to the set.

	const std::vector<AttributeContent::Ptr>& attributeContent() const;
		/// Returns the attributes defined for the complex type.

	bool hasAnyAttribute() const;
		/// Returns true if the any attribute is allowed.

	void createIterator(std::vector<OrderIterator>& seq) const;

private:
	bool _abstract;
	bool _blockExtension;
	bool _blockRestriction;
	bool _finalExtension;
	bool _finalRestriction;
	bool _mixed;
	InheritanceInfo::Ptr _pParent;
	Content::Ptr _pContent;
	std::vector<AttributeContent::Ptr> _attrContent;
	bool _containsAny;
};


//
// inlines
//
inline void ComplexType::setParent(InheritanceInfo::Ptr pInh)
{
	_pParent = pInh;
}


inline InheritanceInfo::Ptr ComplexType::getParent() const
{
	return  _pParent;
}


inline void ComplexType::addAttribute(AttributeContent::Ptr pAttr)
{
	_containsAny |= pAttr->isAny();
	_attrContent.push_back(pAttr);
}


inline bool ComplexType::hasAnyAttribute() const
{
	return _containsAny;
}


inline bool ComplexType::isAbstract() const
{
	return _abstract;
}


inline bool ComplexType::blockExtension() const
{
	return _blockExtension;
}


inline bool ComplexType::blockRestriction() const
{
	return _blockRestriction;
}


inline bool ComplexType::finalExtension() const
{
	return _finalExtension;
}


inline bool ComplexType::finalRestriction() const
{
	return _finalRestriction;
}


inline bool ComplexType::mixed() const
{
	return _mixed;
}


inline void ComplexType::setContent(Content::Ptr ptr)
{
	_pContent = ptr;
}


inline Content::Ptr ComplexType::getContent() const
{
	return _pContent;
}


inline const std::vector<AttributeContent::Ptr>& ComplexType::attributeContent() const
{
	return _attrContent;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_ComplexType_INCLUDED
