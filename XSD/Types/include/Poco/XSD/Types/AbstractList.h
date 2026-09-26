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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_AbstractList_INCLUDED
#define XSDTypes_AbstractList_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AbstractList: public SimpleTypeInheritance
{
public:
	using Ptr = AutoPtr<AbstractList>;

	AbstractList(const std::string& id);
		/// Creates the AbstractList.

	virtual ~AbstractList();
		/// Destroys the AbstractList.

	bool isRestriction() const;

	bool isList() const;

	bool isUnion() const;
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


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractList_INCLUDED
