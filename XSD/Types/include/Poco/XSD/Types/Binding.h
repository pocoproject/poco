//
// Binding.h
//
// Library: XSD/Types
// Package: WSDL
// Module:  Binding
//
// Definition of the Binding class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Binding_INCLUDED
#define XSDTypes_Binding_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/PortType.h"
#include "Poco/XML/Name.h"
#include <vector>


namespace Poco::XSD::Types {


class XSDTypes_API Binding: public AnnotatedObject
	/// This class represents a WSDL Binding.
{
public:
	using Ptr = Poco::AutoPtr<Binding>;

	Binding();
		/// Creates the Binding.

	explicit Binding(const std::string& name);
		/// Creates the Binding.

	~Binding() override;
		/// Destroys the Binding.

	void setName(const std::string& name);
		/// Sets the name.

	[[nodiscard]] const std::string& name() const;
		/// Returns the name.
	
	void setPortType(PortType::Ptr pPortType);
		/// Associates a PortType with the Binding.
		
	[[nodiscard]] PortType::Ptr getPortType() const;
		/// Returns the associated PortType.

	[[nodiscard]] const BindingProperties& bindingProperties() const;
		/// Returns the binding properties for the entire binding (e.g., "soap.binding.stye", "soap.binding.transport").
		
	[[nodiscard]] BindingProperties& bindingProperties();
		/// Returns the binding properties for the entire binding (e.g., "soap.binding.stye", "soap.binding.transport").
	
	void accept(Visitor& v) const override;

private:
	std::string _name;
	PortType::Ptr _pPortType;
	BindingProperties _bindingProperties;
};


//
// inlines
//
inline void Binding::setName(const std::string& name)
{
	_name = name;
}


inline const std::string& Binding::name() const
{
	return _name;
}


inline void Binding::setPortType(PortType::Ptr pPortType)
{
	_pPortType = pPortType;
}

	
inline PortType::Ptr Binding::getPortType() const
{
	return _pPortType;
}


inline const BindingProperties& Binding::bindingProperties() const
{
	return _bindingProperties;
}

	
inline BindingProperties& Binding::bindingProperties()
{
	return _bindingProperties;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Binding_INCLUDED
