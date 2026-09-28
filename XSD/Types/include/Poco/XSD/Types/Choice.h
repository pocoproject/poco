//
// Choice.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Choice
//
// Definition of the Choice class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Choice_INCLUDED
#define XSDTypes_Choice_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Order.h"
#include <vector>


namespace Poco::XSD::Types {


class XSDTypes_API Choice: public Order
	/// This class represents the XML Schema choice element group.
{
public:
	using Content = std::vector<OrderContent::Ptr>;
	using Ptr = AutoPtr<Choice>;

	Choice(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Choice.

	~Choice() override;
		/// Destroys the Choice.

	void add(OrderContent::Ptr pChild) override;

	[[nodiscard]] const Choice::Content& getContent() const;
		/// Returns the children of the Choice collection.

	void fixup() override;

	void accept(Visitor& v) const override;

	[[nodiscard]] OrderIterator iterator() const override;

private:
	Choice::Content _content;
};


//
// inlines
//
inline const Choice::Content& Choice::getContent() const
{
	return _content;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Choice_INCLUDED
