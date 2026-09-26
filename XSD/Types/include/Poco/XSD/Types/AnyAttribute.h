//
// AnyAttribute.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AnyAttribute
//
// Definition of the AnyAttribute class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AnyAttribute_INCLUDED
#define XSDTypes_AnyAttribute_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AttributeContent.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AnyAttribute: public AttributeContent
	/// The AnyAttribute has the special name "*".
{
public:
	using Ptr = AutoPtr<AnyAttribute>;

	enum ProcessStyle
	{
		PS_LAX = 0,
		PS_SKIP,
		PS_STRICT
	};

	AnyAttribute(const std::string& id, const std::string& nameSpace = "##any", ProcessStyle style = PS_STRICT);
		/// Creates the AnyAttribute.

	virtual ~AnyAttribute();
		/// Destroys the AnyAttribute.

	const std::string& nameSpace() const;
		/// The namespace allowed for the any attribute.

	ProcessStyle style() const;
		/// The processing for any attribute.

	// AttributeContent
	bool isAny() const;
	void fixup();
	void accept(Visitor& v) const;

private:
	std::string _nameSpace;
	ProcessStyle   _style;
};


//
// inlines
//
inline const std::string& AnyAttribute::nameSpace() const
{
	return _nameSpace;
}


inline AnyAttribute::ProcessStyle AnyAttribute::style() const
{
	return _style;
}


inline bool AnyAttribute::isAny() const
{
	return true;
}


inline void AnyAttribute::fixup()
{
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AnyAttribute_INCLUDED
