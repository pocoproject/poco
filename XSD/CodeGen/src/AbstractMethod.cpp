//
// AbstractMethod.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AbstractMethod.h"
#include "Poco/String.h"


AbstractMethod::AbstractMethod(const std::string& name, Utility::Access acc, bool isConst, bool isStatic, bool isVirtual, bool isAbstract):
	_name(name),
	_access(acc),
	_isConst(isConst),
	_isStatic(isStatic),
	_isVirtual(isVirtual),
	_isAbstract(isAbstract)
{
	poco_assert (!_name.empty());
}


AbstractMethod::~AbstractMethod()
{
}


void AbstractMethod::addCode(const std::string& line)
{
	_code.push_back(Poco::trim(line));
}


void AbstractMethod::addCode(const std::vector<std::string>& lines)
{
	std::vector<std::string>::const_iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		addCode(*it);
	}
}


void AbstractMethod::addDocu(const std::string& line)
{
	_docu.push_back(Poco::trim(line));
}


void AbstractMethod::addDocu(const std::vector<std::string>& lines)
{
	std::vector<std::string>::const_iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		addDocu(*it);
	}
}
