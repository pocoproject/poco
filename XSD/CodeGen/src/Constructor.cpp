//
// Constructor.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Constructor.h"
#include "ClassInfo.h"
#include "Poco/String.h"


Constructor::Constructor(const ClassInfo& owner, Utility::Access acc):
	AbstractMethod(owner.name(), acc, false, false, false, false),
	_constrCode()
{
}


Constructor::~Constructor()
{
}


void Constructor::addInitializationCode(const std::string& line)
{
	std::string trimmed = Poco::trim(line);
	if (trimmed[trimmed.size()-1] == ',')
		trimmed = trimmed.substr(0, trimmed.size()-1);
	_constrCode.push_back(trimmed);
}


void Constructor::addInitializationCode(const std::vector<std::string>& lines)
{
	std::vector<std::string>::const_iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		addCode(*it);
	}
}

