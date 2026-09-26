//
// Sequence.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Sequence
//
// Definition of the Sequence class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Sequence_INCLUDED
#define XSDTypes_Sequence_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Order.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Sequence: public Order
	/// This class represents the XML Schema sequence element group.
{
public:
	using Content = std::vector<OrderContent::Ptr>;
	using Ptr = AutoPtr<Sequence>;

	Sequence(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Sequence.

	virtual ~Sequence();
		/// Destroys the Sequence.

	void add(OrderContent::Ptr pChild);
		/// Adds a child to the end of the sequence

	void accept(Visitor& v) const;

	const Sequence::Content& getContent() const;
		/// Returns all the children in the sequence

	void fixup();

	OrderIterator iterator() const;

private:
	Sequence::Content _content;
};


//
// inlines
//
inline const Sequence::Content& Sequence::getContent() const
{
	return _content;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Sequence_INCLUDED
