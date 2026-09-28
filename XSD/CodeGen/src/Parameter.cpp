//
// Parameter.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
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
	_soapHeader(soapHeader)
{
}
