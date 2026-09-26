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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Choice_INCLUDED
#define XSDTypes_Choice_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Order.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Choice: public Order
	/// This class represents the XML Schema choice element group.
{
public:
	using Content = std::vector<OrderContent::Ptr>;
	using Ptr = AutoPtr<Choice>;

	Choice(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Choice.

	virtual ~Choice();
		/// Destroys the Choice.

	void add(OrderContent::Ptr pChild);

	const Choice::Content& getContent() const;
		/// Returns the children of the Choice collection.

	void fixup();

	void accept(Visitor& v) const;

	OrderIterator iterator() const;

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


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Choice_INCLUDED
