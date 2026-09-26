//
// Utility.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Utility.h"
#include "Poco/String.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/Ascii.h"


using Poco::XSD::Types::TypesManager;


std::string Utility::xsdNameToClassName(const std::string& xsdName)
{
	poco_assert_dbg (!xsdName.empty());
	//xsd allows - in names
	std::string className = cleanupName(xsdName);
	
	// first char is upper case
	className[0] = Poco::Ascii::toUpper(className[0]);
	// remove "Type" suffix
	if (className.length() > 4 && className.find("Type") == className.size() - 4)
	{
		className.resize(className.size() - 4);
	}
	return className;
}


std::string Utility::xsdNameToVarName(const std::string& xsdName)
{
	poco_assert_dbg (!xsdName.empty());
	//xsd allows - in names
	std::string varName = cleanupName(xsdName);
	// first char is lower case
	varName[0] = Poco::Ascii::toLower(varName[0]);
	return std::string("_") + varName;
}


std::string Utility::xsdNameToMethodName(const std::string& xsdName)
{
	poco_assert_dbg (!xsdName.empty());
	//xsd allows - in names
	std::string methodName = cleanupName(xsdName);
	// first char is lower case
	methodName[0] = Poco::Ascii::toLower(methodName[0]);
	if (isReservedName(methodName))
		methodName += "_";
	return methodName;
}


std::string Utility::xsdNameToAttrName(const std::string& xsdName)
{
	return Utility::xsdNameToVarName(xsdName) + "Attr";
}


std::string Utility::xsdNameToParamName(const std::string& xsdName)
{
	return xsdNameToMethodName(xsdName);
}


std::string Utility::getterMethodName(const std::string& cppVarName)
{
	// strip _ from name
	poco_assert (!cppVarName.empty());
	std::string cleanName = cppVarName;
	if (cppVarName[0] == '_')
		cleanName = cppVarName.substr(1);

	poco_assert (!cleanName.empty());
	cleanName[0] = Poco::Ascii::toUpper(cleanName[0]);
	return "get" + cleanName;
}


std::string Utility::setterMethodName(const std::string& cppVarName)
{
	// strip _ from name
	poco_assert (!cppVarName.empty());
	std::string cleanName = cppVarName;
	if (cppVarName[0] == '_')
		cleanName = cppVarName.substr(1);

	poco_assert (!cleanName.empty());
	cleanName[0] = Poco::Ascii::toUpper(cleanName[0]);
	return "set" + cleanName;
}


bool Utility::isBuiltinNamespace(const std::string& xmlNamespace)
{
	return xmlNamespace == TypesManager::XSD_NAMESPACE || xmlNamespace == TypesManager::XSD_NAMESPACE1998;
}


std::string Utility::varNameToParamName(const std::string& varName)
{
	poco_assert (varName.size() > 1);
	poco_assert_dbg (varName[0] == '_');
	std::string result = varName.substr(1);
	if (isReservedName(result))
		result += "_";
	return result;
}


bool Utility::isReservedName(const std::string& name)
{
	static const char* reserved[] = {
		"alignas",
		"alignof",
		"and",
		"and_eq",
		"asm",
		"auto",
		"bitand",
		"bitor",
		"bool",
		"break",
		"case",
		"catch",
		"char",
		"char16_t",
		"char32_t",
		"class",
		"compl",
		"const",
		"constexpr",
		"const_cast",
		"continue",
		"decltype",
		"default",
		"delete",
		"do",
		"double",
		"dynamic_cast",
		"else",
		"enum",
		"explicit",
		"export",
		"extern",
		"false",
		"float",
		"for",
		"friend",
		"goto",
		"if",
		"inline",
		"int",
		"long",
		"mutable",
		"namespace",
		"new",
		"noexcept",
		"not",
		"not_eq",
		"nullptr",
		"operator",
		"or",
		"or_eq",
		"private",
		"protected",
		"public",
		"register",
		"reinterpret_cast",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"static_assert",
		"static_cast",
		"struct",
		"switch",
		"template",
		"this",
		"thread_local",
		"throw",
		"true",
		"try",
		"typedef",
		"typeid",
		"typename",
		"union",
		"unsigned",
		"using(1)",
		"virtual",
		"void",
		"volatile",
		"wchar_t",
		"while",
		"xor",
		"xor_eq",
		0
	};

	for (const char* const* p = &reserved[0]; *p; p++)
	{
		if (name == *p)
			return true;
	}
	return false;
}


std::string Utility::cleanupName(const std::string& name)
{
	std::string result;
	std::string::const_iterator it = name.begin();
	std::string::const_iterator end = name.end();
	if (it != end && *it == '#') ++it; // anonymous type
	for (; it != end; ++it)
	{
		if (Poco::Ascii::isAlphaNumeric(*it))
			result += *it;
		else
			result += '_';
	}
	return result;
}
