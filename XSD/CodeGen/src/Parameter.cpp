//
// Parameter.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Parameter.h"


Parameter::Parameter(const std::string& name, 
		const TypeInfo& type, 
		int order,
		Variable::Modifiers t, 
		bool optional, 
		bool soapHeader, 
		bool nillable):
	Variable(name, type, Utility::AC_PUBLIC, order, t, optional, nillable),
	_soapHeader(soapHeader),
	_direction(DIR_IN)
{
}


Parameter::~Parameter()
{
}
