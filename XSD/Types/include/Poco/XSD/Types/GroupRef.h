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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_GroupRef_INCLUDED
#define XSDTypes_GroupRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractGroup.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class Group;


class XSDTypes_API GroupRef: public AbstractGroup
	/// This class represents a reference to an element group in an XML Schema.
{
public:
	using Ptr = AutoPtr<GroupRef>;

	GroupRef(const std::string& id, const QName& ref, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the GroupRef.

	virtual ~GroupRef();
		/// Destroys the GroupRef.

	void fixup();

	void accept(Visitor& v) const;

	Order::Ptr getChild() const;

	const std::string& name() const;
		/// Returns the non-empty name.

private:
	QName _ref;
	const Group* _pGroup;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_GroupRef_INCLUDED
