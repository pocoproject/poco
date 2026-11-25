//
// Concept.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Concept
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Concept.h"


namespace Poco::CppParser {


Concept::Concept(const std::string& decl, NameSpace* pNameSpace):
	Decl(decl, pNameSpace)
{
}


Concept::~Concept() = default;


Symbol::Kind Concept::kind() const
{
	return Symbol::SYM_CONCEPT;
}


} // namespace Poco::CppParser
