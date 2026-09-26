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


namespace Poco {
namespace XSD {
namespace Types {


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

	virtual ~Attribute();
		/// Destroys the Attribute.

	const std::string& defaultValue() const;
		/// Returns the (optional) default value of the attribute. Empty if none is set.

	const std::string& fixedValue() const;
		/// Returns the (optional) fixed value of the attribute. Empty if none is set.

	bool qualifiedForm() const;
		/// Returns true if the attribute must be used qualified.

	void setType(SimpleType::Ptr ptr);
		// Sets the internal anonymous type.

	const SimpleType* type() const;
		/// Returns the type the attribute uses.

	AbstractAttribute::Usage usage() const;
		/// Returns the usage options for the Attribute.

	// AbstractAttribute
	const std::string& nameSpace() const;
	void fixup();
	void accept(Visitor& v) const;

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


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Attribute_INCLUDED
