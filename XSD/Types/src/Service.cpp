//
// Service.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  Service
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Service.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Service::Service()
{
}


Service::Service(const std::string& name):
	_name(name)
{
}


Service::~Service()
{
}

	
void Service::addPort(const std::string& name, const Poco::XML::Name& binding)
{
	Port port;
	port.name = name;
	port.binding = binding;
	_ports.push_back(port);
}


void Service::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
