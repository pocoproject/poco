//
// AbstractList.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AbstractList
//
// Definition of the AbstractList class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AbstractList_INCLUDED
#define XSDTypes_AbstractList_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco::XSD::Types {


class XSDTypes_API AbstractList: public SimpleTypeInheritance
{
public:
	using Ptr = AutoPtr<AbstractList>;

	explicit AbstractList(const std::string& id);
		/// Creates the AbstractList.

	~AbstractList() override;
		/// Destroys the AbstractList.

	[[nodiscard]] bool isRestriction() const override;

	[[nodiscard]] bool isList() const override;

	[[nodiscard]] bool isUnion() const override;
};


//
// inlines
//
inline bool AbstractList::isRestriction() const
{
	return false;
}


inline bool AbstractList::isList() const
{
	return true;
}


inline bool AbstractList::isUnion() const
{
	return false;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractList_INCLUDED
