//
// CodeGenerator.cpp
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  CodeGenerator
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CodeGeneration/CodeGenerator.h"


namespace Poco {
namespace CodeGeneration {


CodeGenerator::FwdDecl::FwdDecl(const std::string& inc, const std::string& cN, const std::string& ns):
	includeFile(inc),
	className(cN),
	fullNameSpace(ns)
{
	poco_assert_dbg (!includeFile.empty());
	poco_assert_dbg (!className.empty());
}


bool CodeGenerator::FwdDecl::operator<(const FwdDecl& other) const
{
	if (fullNameSpace < other.fullNameSpace)
		return true;
	if (fullNameSpace > other.fullNameSpace)
		return false;

	return className < other.className;
}


CodeGenerator::CodeGenerator(const std::string& nameSpace, const std::string& libraryName, bool st, const std::string& copyright):
	_nameSpace(nameSpace),
	_libraryName(libraryName),
	_usePocoIncludeStyle(st),
	_copyright(copyright),
	_includes(),
	_systemIncludes(),
	_fwdDecls()
{
}


CodeGenerator::~CodeGenerator()
{
}


void CodeGenerator::writeIncludes()
{
	CodeGenerator::IncludeSet::const_iterator it = systemIncludes().begin();
	CodeGenerator::IncludeSet::const_iterator itEnd = systemIncludes().end();
	for (; it != itEnd; ++it)
		writeSystemInclude(*it);

	it = includes().begin();
	itEnd = includes().end();
	for (; it != itEnd; ++it)
		writeInclude(*it, true);

	it = _srcIncludes.begin();
	itEnd = _srcIncludes.end();
	for (; it != itEnd; ++it)
		writeInclude(*it, false);

	writeFwdDecls(_fwdDecls);
}


void CodeGenerator::addFwdDecl(const std::string& inclFile, const std::string& className, const std::string namespacePrefix)
{
	CodeGenerator::FwdDecl decl(inclFile, className, namespacePrefix);
	_fwdDecls.insert(decl);
}


} } // namespace Poco::CodeGeneration
