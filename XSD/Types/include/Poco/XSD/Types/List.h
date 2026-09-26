//
// List.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  List
//
// Definition of the List class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_List_INCLUDED
#define XSDTypes_List_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractList.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API List: public AbstractList
	/// class List that references an existing item type.
{
public:
	using Ptr = AutoPtr<List>;

	List(const std::string& id);
		/// Creates the List.

	virtual ~List();
		/// Destroys the List.

	void setType(SimpleType::Ptr pSimple);
		/// Sets the simple type.

	const std::vector<const Type*>& types() const;
		/// Returns the type that we inherit from.

	void fixup();
		/// Replaces type references with the referenced type object.

	void accept(Visitor& v) const;

private:
	SimpleType::Ptr _pSimple;
	std::vector<const Type*> _types;
};


//
// inlines
//
inline const std::vector<const Type*>& List::types() const
{
	return _types;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_List_INCLUDED
