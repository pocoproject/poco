//
// Group.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Group
//
// Definition of the Group class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Group_INCLUDED
#define XSDTypes_Group_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractGroup.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Group: public AbstractGroup
	/// This class represents an element group in an XML Schema.
{
public:
	using Ptr = AutoPtr<Group>;

	Group(const std::string& id, const std::string& name, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Group.

	virtual ~Group();
		/// Destroys the Group.

	const std::string& name() const;
		/// Returns the non-empty name.

	Order::Ptr getChild() const;

	void setChild(Order::Ptr ptr);

	void fixup();

	void accept(Visitor& v) const;

private:
	std::string _name;
	Order::Ptr     _pChild;
};


//
// inlines
//
inline const std::string& Group::name() const
{
	return _name;
}


inline Order::Ptr Group::getChild() const
{
	return _pChild;
}


inline void Group::setChild(Order::Ptr ptr)
{
	_pChild = ptr;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Group_INCLUDED
