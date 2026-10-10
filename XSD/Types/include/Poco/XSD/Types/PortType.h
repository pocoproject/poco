//
// PortType.h
//
// Library: XSD/Types
// Package: WSDL
// Module:  PortType
//
// Definition of the PortType class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_PortType_INCLUDED
#define XSDTypes_PortType_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/Operation.h"
#include "Poco/XSD/Types/BindingProperties.h"
#include <map>


namespace Poco::XSD::Types {


class XSDTypes_API PortType: public AnnotatedObject
	/// This class represents a WSDL PortType, which
	/// is a collection of Operation objects.
{
public:
	using Ptr = Poco::AutoPtr<PortType>;
	
	using Operations = std::map<std::string, Operation::Ptr>;

	PortType();
		/// Creates the PortType.

	explicit PortType(const std::string& name);
		/// Creates the PortType.

	~PortType() override;
		/// Destroys the PortType.

	void setName(const std::string& name);
		/// Sets the name.

	[[nodiscard]] const std::string& name() const;
		/// Returns the name.
		
	void addOperation(Operation::Ptr pOperation);
		/// Adds an Operation to the PortType.
		
	[[nodiscard]] Operation::Ptr findOperation(const std::string& name) const;
		/// Returns the Operation object with the given name, or
		/// a null pointer if the operation does not exist.
	
	[[nodiscard]] const Operations& operations() const;
		/// Returns the Operations map.
		
	[[nodiscard]] const BindingProperties& bindingProperties() const;
		/// Returns the binding properties.
		
	[[nodiscard]] BindingProperties& bindingProperties();
		/// Returns the binding properties.

	void accept(Visitor& v) const override;
 		
private:
	std::string _name;
	Operations _operations;
	BindingProperties _bindingProperties;
};


//
// inlines
//
inline void PortType::setName(const std::string& name)
{
	_name = name;
}


inline const std::string& PortType::name() const
{
	return _name;
}


inline const PortType::Operations& PortType::operations() const
{
	return _operations;
}


inline Operation::Ptr PortType::findOperation(const std::string& name) const
{
	if (const auto it = _operations.find(name); it != _operations.end())
		return it->second;
	else
		return {};
}


inline const BindingProperties& PortType::bindingProperties() const
{
	return _bindingProperties;
}

	
inline BindingProperties& PortType::bindingProperties()
{
	return _bindingProperties;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_PortType_INCLUDED
