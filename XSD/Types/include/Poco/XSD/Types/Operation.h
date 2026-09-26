//
// Operation.h
//
// Library: XSD/Types
// Package: WSDL
// Module:  Operation
//
// Definition of the Operation class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Operation_INCLUDED
#define XSDTypes_Operation_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/BindingProperties.h"
#include "Poco/XML/Name.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Operation: public AnnotatedObject
	/// This class represents a WSDL Operation, including
	/// SOAP binding information.
{
public:
	using Ptr = Poco::AutoPtr<Operation>;

	Operation();
		/// Creates the Operation.

	Operation(const std::string& name);
		/// Creates the Operation.

	~Operation();
		/// Destroys the Operation.

	void setName(const std::string& name);
		/// Sets the name.

	const std::string& name() const;
		/// Returns the name.
		
	void setInputName(const std::string& name);
		/// Sets the input name.

	const std::string& getInputName() const;
		/// Returns the input name.

	void setInputMessage(const XML::Name& name);
		/// Sets the input message.

	const XML::Name& getInputMessage() const;
		/// Returns the input message.
		
	const BindingProperties& inputBindingProperties() const;
		/// Returns the input binding properties.

	BindingProperties& inputBindingProperties();
		/// Returns the input binding properties.

	void setOutputName(const std::string& name);
		/// Sets the input name.

	const std::string& getOutputName() const;
		/// Returns the input name.

	void setOutputMessage(const XML::Name& name);
		/// Sets the input message.

	const XML::Name& getOutputMessage() const;
		/// Returns the input message.

	const BindingProperties& outputBindingProperties() const;
		/// Returns the output binding properties.

	BindingProperties& outputBindingProperties();
		/// Returns the output binding properties.

	void setFaultName(const std::string& name);
		/// Sets the input name.

	const std::string& getFaultName() const;
		/// Returns the input name.

	void setFaultMessage(const XML::Name& name);
		/// Sets the input message.

	const XML::Name& getFaultMessage() const;
		/// Returns the input message.

	const BindingProperties& faultBindingProperties() const;
		/// Returns the fault binding properties.

	BindingProperties& faultBindingProperties();
		/// Returns the fault binding properties.

	const BindingProperties& bindingProperties() const;
		/// Returns the binding properties for the entire operation (e.g., "soap.soapAction", "soap.style").
		
	BindingProperties& bindingProperties();
		/// Returns the binding properties for the entire operation (e.g., "soap.soapAction", "soap.style").
		
	const std::vector<std::string>& parameterOrder() const;
		/// Returns the specified parameter order (based on the parameterOrder attribute).
		
	void setParameterOrder(const std::string& parameterOrder);
		/// Sets the parameter order.

	void accept(Visitor& v) const;

private:
	std::string _name;
	std::string _inputName;
	XML::Name _inputMessage;
	BindingProperties _inputBindingProperties;
	std::string _outputName;
	XML::Name _outputMessage;
	BindingProperties _outputBindingProperties;
	std::string _faultName;
	XML::Name _faultMessage;
	BindingProperties _faultBindingProperties;
	BindingProperties _bindingProperties;
	std::vector<std::string> _parameterOrder;
};


//
// inlines
//
inline void Operation::setName(const std::string& name)
{
	_name = name;
}


inline const std::string& Operation::name() const
{
	return _name;
}


inline void Operation::setInputName(const std::string& name)
{
	_inputName = name;
}


inline const std::string& Operation::getInputName() const
{
	return _inputName;
}

inline void Operation::setInputMessage(const XML::Name& message)
{
	_inputMessage = message;
}


inline const XML::Name& Operation::getInputMessage() const
{
	return _inputMessage;
}


inline const BindingProperties& Operation::inputBindingProperties() const
{
	return _inputBindingProperties;
}


inline BindingProperties& Operation::inputBindingProperties()
{
	return _inputBindingProperties;
}


inline void Operation::setOutputName(const std::string& name)
{
	_outputName = name;
}


inline const std::string& Operation::getOutputName() const
{
	return _outputName;
}

inline void Operation::setOutputMessage(const XML::Name& message)
{
	_outputMessage = message;
}


inline const XML::Name& Operation::getOutputMessage() const
{
	return _outputMessage;
}


inline const BindingProperties& Operation::outputBindingProperties() const
{
	return _outputBindingProperties;
}


inline BindingProperties& Operation::outputBindingProperties()
{
	return _outputBindingProperties;
}


inline void Operation::setFaultName(const std::string& name)
{
	_faultName = name;
}


inline const std::string& Operation::getFaultName() const
{
	return _faultName;
}

inline void Operation::setFaultMessage(const XML::Name& message)
{
	_faultMessage = message;
}


inline const XML::Name& Operation::getFaultMessage() const
{
	return _faultMessage;
}


inline const BindingProperties& Operation::faultBindingProperties() const
{
	return _faultBindingProperties;
}


inline BindingProperties& Operation::faultBindingProperties()
{
	return _faultBindingProperties;
}


inline const BindingProperties& Operation::bindingProperties() const
{
	return _bindingProperties;
}

	
inline BindingProperties& Operation::bindingProperties()
{
	return _bindingProperties;
}


inline const std::vector<std::string>& Operation::parameterOrder() const
{
	return _parameterOrder;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Operation_INCLUDED
