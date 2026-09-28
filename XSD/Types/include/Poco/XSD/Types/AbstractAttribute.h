//
// AbstractAttribute.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AbstractAttribute
//
// Definition of the AbstractAttribute class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AbstractAttribute_INCLUDED
#define XSDTypes_AbstractAttribute_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AttributeContent.h"


namespace Poco::XSD::Types {


class SimpleType;


class XSDTypes_API AbstractAttribute: public AttributeContent
	/// The base class for all Attribute classes.
{
public:
	using Ptr = AutoPtr<AbstractAttribute>;

	enum Usage
	{
		USE_OPTIONAL = 0,
		USE_PROHIBITED,
		USE_REQUIRED
	};

	AbstractAttribute(const std::string& id, const std::string& name);
		/// Creates the AbstractAttribute.

	~AbstractAttribute() override;
		/// Destroys the AbstractAttribute.

	[[nodiscard]] virtual const std::string& defaultValue() const = 0;
		/// Returns the (optional) default value of the attribute. Empty if none is set.

	[[nodiscard]] virtual const std::string& fixedValue() const = 0;
		/// Returns the (optional) fixed value of the attribute. Empty if none is set.

	[[nodiscard]] bool hasDefault() const;
		/// Returns true if the attribute contains a default value.

	[[nodiscard]] bool hasFixed() const;
		/// Returns true if the attribute contains a fixed value.

	[[nodiscard]] virtual bool qualifiedForm() const = 0;
		/// Returns true if the attribute must be used qualified.

	[[nodiscard]] virtual const SimpleType* type() const = 0;
		/// Returns the type the attribute uses.

	[[nodiscard]] virtual AbstractAttribute::Usage usage() const = 0;
		/// Returns the usage options for the Attribute.
		
	[[nodiscard]] virtual const std::string& nameSpace() const = 0;

	// AttributeContent
	[[nodiscard]] bool isAny() const override;
};


//
// inlines
//
inline bool AbstractAttribute::isAny() const
{
	return false;
}


inline bool AbstractAttribute::hasDefault() const
{
	return !defaultValue().empty();
}


inline bool AbstractAttribute::hasFixed() const
{
	return !fixedValue().empty();
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractAttribute_INCLUDED
