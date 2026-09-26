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
	enum Access
	{
		AC_PUBLIC,
		AC_PROTECTED,
		AC_PRIVATE
	};

	static std::string xsdNameToClassName(const std::string& xsdName);

	static std::string xsdNameToVarName(const std::string& xsdName);

	static std::string xsdNameToMethodName(const std::string& xsdName);

	static std::string xsdNameToAttrName(const std::string& xsdName);
	
	static std::string xsdNameToParamName(const std::string& xsdName);

	static std::string getterMethodName(const std::string& cppVarName);

	static std::string setterMethodName(const std::string& cppVarName);

	static bool isBuiltinNamespace(const std::string& xmlNamespace);

	static std::string varNameToParamName(const std::string& varName);
	
	static bool isReservedName(const std::string& name);
	
	static std::string cleanupName(const std::string& name);

	static std::string cppStringLiteral(const std::string& value);
		/// Returns value as a C++ string literal: backslash, double quote and question mark
		/// are escaped (the question mark so that the literal contains no trigraph sequence),
		/// and every byte outside printable ASCII is written as a three-digit octal escape
		/// (octal, not \x, because a hex escape would swallow following hex digits).

private:
	Utility();
	Utility(const Utility&);
	Utility& operator=(const Utility&);
	~Utility();
};


#endif // CodeGen_Utility_H_INCLUDED
