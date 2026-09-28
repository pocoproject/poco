//
// Utility.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_Utility_H_INCLUDED
#define CodeGen_Utility_H_INCLUDED


#include <string>
#include "TypeInfo.h"
#include "Poco/Poco.h"


class Utility
{
public:
	Utility() = delete;

	enum Access
	{
		AC_PUBLIC,
		AC_PROTECTED,
		AC_PRIVATE
	};

	[[nodiscard]] static std::string xsdNameToClassName(const std::string& xsdName);

	[[nodiscard]] static std::string xsdNameToVarName(const std::string& xsdName);

	[[nodiscard]] static std::string xsdNameToMethodName(const std::string& xsdName);

	[[nodiscard]] static std::string xsdNameToAttrName(const std::string& xsdName);
	
	[[nodiscard]] static std::string xsdNameToParamName(const std::string& xsdName);

	[[nodiscard]] static std::string getterMethodName(const std::string& cppVarName);

	[[nodiscard]] static std::string setterMethodName(const std::string& cppVarName);

	[[nodiscard]] static bool isBuiltinNamespace(const std::string& xmlNamespace);

	[[nodiscard]] static std::string varNameToParamName(const std::string& varName);
	
	[[nodiscard]] static bool isReservedName(const std::string& name);
	
	[[nodiscard]] static std::string cleanupName(const std::string& name);

	[[nodiscard]] static std::string cppStringLiteral(const std::string& value);
		/// Returns value as a C++ string literal. Backslash, double quote and question mark
		/// (to avoid trigraphs) are escaped; every byte outside printable ASCII becomes a
		/// three-digit octal escape, because a \x escape would absorb following hex digits.
};


#endif // CodeGen_Utility_H_INCLUDED
