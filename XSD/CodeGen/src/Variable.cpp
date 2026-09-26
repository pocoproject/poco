//
// Variable.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Variable.h"


Variable::Variable(const std::string& name, const TypeInfo& type, Utility::Access acc, int order, Modifiers t, bool optional, bool nillable):
	_name(name),
	_type(type),
	_order(order),
	_mod(t),
	_optional(optional),
	_nillable(nillable),
	_access(acc)
{
	if (type.getFullName() == "Poco::DateTime")
	{
		insert("xsdType", type.xsdType());
	}
}


Variable::~Variable()
{
}
