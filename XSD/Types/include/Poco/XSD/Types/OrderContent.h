//
// OrderContent.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  OrderContent
//
// Definition of the OrderContent class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_OrderContent_INCLUDED
#define XSDTypes_OrderContent_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Content.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API OrderContent: public Content
	/// OrderContent is the super class for all children of an Order, ie. sequence, choice, all, group.
{
public:
	using Ptr = Poco::AutoPtr<OrderContent>;

	OrderContent();
		/// Creates the OrderContent.

	OrderContent(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the OrderContent.

	virtual ~OrderContent();
		/// Destroys the OrderContent.

	const std::string& getId() const;
		/// Returns the id.

	void setId(const std::string& id);
		/// Sets the id.

	Poco::UInt32 getMinOccurs() const;
		/// Returns how often an object must occur at least in the XML instance.

	void setMinOccurs(Poco::UInt32 minOcc);
		/// Sets how often an object must occur at least in the XML instance.

	Poco::UInt32 getMaxOccurs() const;
		/// Returns how often an object can occur at most in the XML instance.

	void setMaxOccurs(Poco::UInt32 maxOcc);
		/// Sets how often an object can occur at most in the XML instance.

	const std::vector<const Type*>& types() const;
		/// OrderContent never references existing content. Returns empty vector.

private:
	std::string _id;
	Poco::UInt32 _minOccurs;
	Poco::UInt32 _maxOccurs;
};


//
// inlines
//
inline const std::string& OrderContent::getId() const
{
	return _id;
}


inline void OrderContent::setId(const std::string& id)
{
	_id = id;
}


inline Poco::UInt32 OrderContent::getMinOccurs() const
{
	return _minOccurs;
}


inline void OrderContent::setMinOccurs(Poco::UInt32 minOcc)
{
	_minOccurs = minOcc;
}


inline Poco::UInt32 OrderContent::getMaxOccurs() const
{
	return _maxOccurs;
}


inline void OrderContent::setMaxOccurs(Poco::UInt32 maxOcc)
{
	_maxOccurs = maxOcc;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_OrderContent_INCLUDED
