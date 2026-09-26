//
// Binding.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  Binding
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Binding.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Binding::Binding()
{
}


Binding::Binding(const std::string& name):
	_name(name)
{
}


Binding::~Binding()
{
}


void Binding::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
