//
// ListTypeRef.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  ListTypeRef
//
// Definition of the ListTypeRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_ListTypeRef_INCLUDED
#define XSDTypes_ListTypeRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractList.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API ListTypeRef: public AbstractList
	/// This class represents a reference to a list type in an XML Schema.
{
public:
	using Ptr = AutoPtr<ListTypeRef>;

	ListTypeRef(const std::string& id, const QName& itemType);
		/// Creates the ListTypeRef.

	virtual ~ListTypeRef();
		/// Destroys the ListTypeRef.

	const QName& itemTypeRef() const;
		/// Returns the type reference of the base class

	const std::vector<const Type*>& types() const;
		/// Returns the type that we inherit from.

	void fixup();
		/// Replaces type references with the referenced type object.

	void accept(Visitor& v) const;

private:
	QName       _itemType;
	std::vector<const Type*> _baseType;
};


//
// inlines
//
inline const QName& ListTypeRef::itemTypeRef() const
{
	return _itemType;
}


inline const std::vector<const Type*>& ListTypeRef::types() const
{
	return _baseType;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_ListTypeRef_INCLUDED
