//
// AbstractGroup.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AbstractGroup
//
// Definition of the AbstractGroup class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AbstractGroup_INCLUDED
#define XSDTypes_AbstractGroup_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/CollectionContent.h"
#include "Poco/XSD/Types/Order.h"


namespace Poco::XSD::Types {


class XSDTypes_API AbstractGroup: public CollectionContent
{
public:
	using Ptr = AutoPtr<AbstractGroup>;

	AbstractGroup(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the AbstractGroup.

	~AbstractGroup() override;
		/// Destroys the AbstractGroup.

	void fixup() override = 0;
		/// Resolves references to point to the actual types

	[[nodiscard]] virtual Order::Ptr getChild() const = 0;
		/// Returns the child of the group

	[[nodiscard]] virtual const std::string& name() const = 0;
		/// Returns the non-empty name

	[[nodiscard]] OrderIterator iterator() const override;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractGroup_INCLUDED
