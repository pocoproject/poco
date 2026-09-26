//
// InheritanceInfo.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  InheritanceInfo
//
// Definition of the InheritanceInfo class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_InheritanceInfo_INCLUDED
#define XSDTypes_InheritanceInfo_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"


namespace Poco {
namespace XSD {
namespace Types {


class Type;


class XSDTypes_API InheritanceInfo: public AnnotatedObject
	/// Class InheritanceInfo models inheritance information for complex types.
{
public:
	using Ptr = AutoPtr<InheritanceInfo>;
	
	InheritanceInfo();
		/// Creates the InheritanceInfo.

	virtual ~InheritanceInfo();
		/// Destroys the InheritanceInfo.

	const Type* type() const;
		/// Returns the parent that we inherit from.

	void setType(const QName& parent);
		///Set the parent class

	void setRestrictionType(SimpleType::Ptr pInline);
		/// A restriction type can define an inline simple type that we inherit from.
		/// This method will fail for getRestriction() != true or getSimpleContent != true.

	void fixup();
		/// Replaces type references with the referenced type object.

	bool getRestriction() const;
		/// True if we inherit by restriction, otherwise we inherit by extension.

	bool getSimpleContent() const;
		/// True if we inherit from a simple type, false if we have a complexContent.

	void setRestriction(bool rest);
		/// Set to true if we inherit by restriction, otherwise we inherit by extension.

	void setSimpleContent(bool cont);
		/// Set to true if we inherit from a simple type, false if we have a complexContent.

	void accept(Visitor& v) const;

	const std::vector<const Type*>& parents() const;

private:
	QName       _ref;
	bool        _restriction;
	bool        _simpleContent;
	std::vector<const Type*> _parent;
	SimpleType::Ptr _pSimple;
};


//
// inlines
//
inline const std::vector<const Type*>& InheritanceInfo::parents() const
{
	return _parent;
}


inline bool InheritanceInfo::getRestriction() const
{
	return _restriction;
}


inline bool InheritanceInfo::getSimpleContent() const
{
	return _simpleContent;
}


inline void InheritanceInfo::setRestriction(bool rest)
{
	_restriction = rest;
}


inline void InheritanceInfo::setSimpleContent(bool cont)
{
	_simpleContent = cont;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_InheritanceInfo_INCLUDED
