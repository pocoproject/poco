//
// Decl.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Decl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Decl.h"
#include "Poco/String.h"


namespace Poco::CppParser {


Decl::Decl(const std::string& decl, NameSpace* pNameSpace):
	Decl(decl, std::string::npos, pNameSpace)
{
}


Decl::Decl(const std::string& decl, std::size_t declaratorLength, NameSpace* pNameSpace):
	Symbol(extractName(decl.substr(0, declaratorLength)), pNameSpace),
		_decl(Poco::trim(decl))
{
}


Decl::~Decl()
{
}


std::string Decl::toString() const
{
	return _decl;
}


} // namespace Poco::CppParser

