//
// All.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  All
//
// Definition of the All class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_All_INCLUDED
#define XSDTypes_All_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Order.h"
#include "Poco/XSD/Types/Element.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API All: public Order
	/// This class represents the XML Schema all element group.
{
public:
	using Content = std::map<std::string, Element::Ptr>;
	using Ptr = AutoPtr<All>;

	All(const std::string& id, Poco::UInt32 minOcc);
		/// Creates the All.

	virtual ~All();
		/// Destroys the All.

	void add(OrderContent::Ptr pChild);
		/// Note: All only accepts elements as children. If the order content cannot be cast to Element::Ptr it will throw an exception.

	void add(Element::Ptr pChild);
		/// Adds an element to the All collection

	const All::Content& getContent() const;
		/// Returns the children of the All collection

	void fixup();

	void accept(Visitor& v) const;

	OrderIterator iterator() const;

private:
	All::Content _content;
};


//
// inlines
//
inline const All::Content& All::getContent() const
{
	return _content;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_All_INCLUDED
