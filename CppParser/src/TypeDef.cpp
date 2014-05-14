//
// TypeDef.cpp
//
// $Id: //poco/1.4/CppParser/src/TypeDef.cpp#1 $
//
// Library: CppParser
// Package: SymbolTable
// Module:  TypeDef
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/TypeDef.h"
#include "Poco/String.h"
#include <cctype>


namespace Poco {
namespace CppParser {


TypeDef::TypeDef(const std::string& decl, NameSpace* pNameSpace):
	Decl(decl, pNameSpace)
{
}


TypeDef::~TypeDef()
{
}


Symbol::Kind TypeDef::kind() const
{
	return Symbol::SYM_TYPEDEF;
}


std::string TypeDef::baseType() const
{
	std::string decl = declaration();
	if (decl.compare(0, 7, "typedef") == 0)
	{
		decl.erase(0, 7);
		std::string::size_type pos = decl.size() - 1;
		while (pos > 0 && std::isspace(decl[pos])) pos--;
		while (pos > 0 && !std::isspace(decl[pos])) pos--;
		decl.resize(pos + 1);
		Poco::trimInPlace(decl);
	}
	return decl;
}


} } // namespace Poco::CppParser
