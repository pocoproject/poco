//
// PropertyHolder.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
