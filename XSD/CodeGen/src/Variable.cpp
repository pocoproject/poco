//
// Variable.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
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
