//
// Constructor.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
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

