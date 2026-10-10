//
// Attribute.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  Attribute
//
// Definition of the Attribute class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Attribute_INCLUDED
#define XSDTypes_Attribute_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/SimpleType.h"


namespace Poco::XSD::Types {


class XSDTypes_API Attribute: public AbstractAttribute
	/// An Attribute that defines an internal anonymous simple type.
{
public:
	using Ptr = AutoPtr<Attribute>;

	Attribute(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use = AbstractAttribute::USE_OPTIONAL);
		/// Creates the Attribute.

	~Attribute() override;
		/// Destroys the Attribute.

	[[nodiscard]] const std::string& defaultValue() const override;
		/// Returns the (optional) default value of the attribute. Empty if none is set.

	[[nodiscard]] const std::string& fixedValue() const override;
		/// Returns the (optional) fixed value of the attribute. Empty if none is set.

	[[nodiscard]] bool qualifiedForm() const override;
		/// Returns true if the attribute must be used qualified.

	void setType(SimpleType::Ptr ptr);
		// Sets the internal anonymous type.

	[[nodiscard]] const SimpleType* type() const override;
		/// Returns the type the attribute uses.

	[[nodiscard]] AbstractAttribute::Usage usage() const override;
		/// Returns the usage options for the Attribute.

	// AbstractAttribute
	[[nodiscard]] const std::string& nameSpace() const override;
	void fixup() override;
	void accept(Visitor& v) const override;

private:
	std::string _nameSpace;
	std::string _fixedValue;
	std::string _defaultValue;
	bool _qualifiedForm;
	AbstractAttribute::Usage _use;
	SimpleType::Ptr _pType;
};


//
// inlines
//
inline void Attribute::setType(SimpleType::Ptr ptr)
{
	_pType = ptr;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Attribute_INCLUDED
