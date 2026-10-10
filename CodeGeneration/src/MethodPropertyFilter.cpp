//
// MethodPropertyFilter.cpp
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  MethodPropertyFilter
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CodeGeneration/MethodPropertyFilter.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CppParser/Function.h"


namespace Poco {
namespace CodeGeneration {


MethodPropertyFilter::MethodPropertyFilter(CodeGenerator* pGen, const CodeGenerator::Properties& properties):
	CodeGenerator("","", true, ""),
	_pGen(pGen),
	_properties(properties),
	_forwardMethod(false),
	_inMethod(false)
{
	poco_assert (pGen);
}


MethodPropertyFilter::MethodPropertyFilter(CodeGenerator* pGen, const std::string& prop):
	CodeGenerator("","", true, ""),
	_pGen(pGen),
	_properties(),
	_forwardMethod(false),
	_inMethod(false)
{
	poco_assert (pGen);
	_properties.insert(std::make_pair(prop, Utility::VAL_TRUE));
}


MethodPropertyFilter::~MethodPropertyFilter()
{
	delete _pGen;
}


void MethodPropertyFilter::methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	_inMethod = true;
	_forwardMethod = matches(properties) && !(pFunc->isConstructor() || pFunc->isDestructor() || pFunc->isDeleted()) && pFunc->getAccess() == Poco::CppParser::Symbol::ACC_PUBLIC;

	if (_forwardMethod)
		_pGen->methodStart(pFunc, properties);
}


void MethodPropertyFilter::writeMethodImplementation(const std::string& code)
{
	if (_forwardMethod || !_inMethod)
		_pGen->writeMethodImplementation(code);
}


void MethodPropertyFilter::methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	if (_forwardMethod)
		_pGen->methodEnd(pFunc, properties);
	_inMethod = false;
	_forwardMethod = false;
}


bool MethodPropertyFilter::matches(const CodeGenerator::Properties& properties) const
{
	// look if we find all of the internalproperties in properties
	CodeGenerator::Properties::const_iterator it = _properties.begin();
	CodeGenerator::Properties::const_iterator itEnd = _properties.end();
	bool ok = true;
	for (; it != itEnd && ok; ++it)
	{
		CodeGenerator::Properties::const_iterator itF = properties.find(it->first);

		ok &= (itF != properties.end() && (itF->second == it->second || (it->second == Utility::VAL_TRUE && itF->second.empty())));
	}
	return ok;
}


} } // namespace Poco::CodeGeneration
