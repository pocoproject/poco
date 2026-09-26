//
// PropertyHolder.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PropertyHolder.h"


PropertyHolder::PropertyHolder():
	_props()
{
}


PropertyHolder::~PropertyHolder()
{
}


const std::string& PropertyHolder::get(const std::string& id) const
{
	Properties::const_iterator it = _props.find(id);
	if (it == _props.end())
		throw Poco::NotFoundException(id);
	return it->second;
}
