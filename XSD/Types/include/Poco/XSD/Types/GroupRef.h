//
// GroupRef.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  GroupRef
//
// Definition of the GroupRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_GroupRef_INCLUDED
#define XSDTypes_GroupRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractGroup.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco::XSD::Types {


class Group;


class XSDTypes_API GroupRef: public AbstractGroup
	/// This class represents a reference to an element group in an XML Schema.
{
public:
	using Ptr = AutoPtr<GroupRef>;

	GroupRef(const std::string& id, const QName& ref, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the GroupRef.

	~GroupRef() override;
		/// Destroys the GroupRef.

	void fixup() override;

	void accept(Visitor& v) const override;

	[[nodiscard]] Order::Ptr getChild() const override;

	[[nodiscard]] const std::string& name() const override;
		/// Returns the non-empty name.

private:
	QName _ref;
	const Group* _pGroup = nullptr;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_GroupRef_INCLUDED
