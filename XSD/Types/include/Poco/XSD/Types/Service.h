//
// Service.h
//
// Library: XSD/Types
// Package: WSDL
// Module:  Service
//
// Definition of the Service class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Service_INCLUDED
#define XSDTypes_Service_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XML/Name.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Service: public AnnotatedObject
	/// This class represents a WSDL Service.
{
public:
	using Ptr = Poco::AutoPtr<Service>;

	struct Port
	{
		std::string name;
		XML::Name binding;
	};
	using Ports = std::vector<Port>;

	Service();
		/// Creates the Service.

	Service(const std::string& name);
		/// Creates the Service.

	~Service();
		/// Destroys the Service.

	void setName(const std::string& name);
		/// Sets the name.

	const std::string& name() const;
		/// Returns the name.
		
	void addPort(const std::string& name, const Poco::XML::Name& binding);
		/// Adds a new port to the Service.
		
	const Ports& ports() const;
		/// Returns the Service ports.
	
	void accept(Visitor& v) const;

private:
	std::string _name;
	Ports _ports;
};


//
// inlines
//
inline void Service::setName(const std::string& name)
{
	_name = name;
}


inline const std::string& Service::name() const
{
	return _name;
}


inline const Service::Ports& Service::ports() const
{
	return _ports;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Service_INCLUDED
