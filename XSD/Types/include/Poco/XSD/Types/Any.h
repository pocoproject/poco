//
// Any.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Any
//
// Definition of the Any class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Any_INCLUDED
#define XSDTypes_Any_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderContent.h"


namespace Poco::XSD::Types {


class XSDTypes_API Any: public OrderContent
	/// This class represents the XML Schema ur-type.
{
public:
	using Ptr = AutoPtr<Any>;

	enum ProcessStyle
	{
		PS_LAX = 0,
		PS_SKIP,
		PS_STRICT
	};

	Any(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc, const std::string& ns="##any", ProcessStyle style = PS_STRICT);
		/// Creates the Any.

	~Any() override;
		/// Destroys the Any.

	void fixup() override;

	void accept(Visitor& v) const override;

	[[nodiscard]] const std::string& nameSpace() const;
		/// The namespace allowed for the any attribute

	[[nodiscard]] ProcessStyle style() const;
		/// The processing for any attribute

	[[nodiscard]] OrderIterator iterator() const override;

private:
	std::string _nameSpace;
	ProcessStyle   _style;
};


//
// inlines
//
inline const std::string& Any::nameSpace() const
{
	return _nameSpace;
}


inline Any::ProcessStyle Any::style() const
{
	return _style;
}



} // namespace Poco::XSD::Types


#endif // XSDTypes_Any_INCLUDED
