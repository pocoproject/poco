//
// Notation.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Notation
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Notation.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Notation::Notation(const std::string& id, const std::string& name, const std::string& pub, const std::string& system):
	AnnotatedObject(id),
	_name(name),
	_public(pub),
	_system(system)
{
}


Notation::~Notation()
{
}


void Notation::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
