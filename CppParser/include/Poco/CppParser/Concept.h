//
// Concept.h
//
// Library: CppParser
// Package: SymbolTable
// Module:  Concept
//
// Definition of the Concept class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParser_Concept_INCLUDED
#define CppParser_Concept_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/Decl.h"


namespace Poco::CppParser {


class CppParser_API Concept: public Decl
	/// This class represents a concept definition. The declaration
	/// contains the template parameter list and the constraint expression.
{
public:
	Concept() = delete;

	Concept(const std::string& decl, NameSpace* pNameSpace);
		/// Creates the Concept.

	~Concept() override;
		/// Destroys the Concept.

	[[nodiscard]] Symbol::Kind kind() const override;
};


} // namespace Poco::CppParser


#endif // CppParser_Concept_INCLUDED
