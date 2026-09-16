//
// GeneratorEngine.cpp
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  GeneratorEngine
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/TypeDef.h"
#include "Poco/Exception.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Path.h"
#include "Poco/NumberParser.h"


namespace Poco {
namespace CodeGeneration {


using Poco::CppParser::TypeDef;
using Poco::CppParser::TypeAlias;


//always choose names that real functions cannot have
const std::string GeneratorEngine::WILDCARD("*");
const std::string GeneratorEngine::PRECLASS("1PRE*CLASS");
const std::string GeneratorEngine::POSTCLASS("2POST*CLASS");
const std::string GeneratorEngine::PRENAMESPACE("1PRE*NAMESPACE");
const std::string GeneratorEngine::POSTNAMESPACE("2POST*NAMESPACE");


GeneratorEngine::GeneratorEngine():_callbacks()
{
}


GeneratorEngine::~GeneratorEngine()
{
}



void GeneratorEngine::registerCallback(const std::string& methodName, MethodGenerator funcGen)
{
	bool ok = _callbacks.insert(make_pair(methodName, funcGen)).second;
	if (!ok)
		throw Poco::ExistsException("callback already exists for the given method");
}


void GeneratorEngine::generate(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam, bool isFirstClass, bool isLastClass)
{
	if (isFirstClass)
	{
		gen.writeDefaultHeader(pStruct, pStruct->name(), pStruct->getLibrary(), pStruct->getPackage());
		handleIncludes(pStruct, gen);

		Callbacks::iterator itPreNS = _callbacks.find(PRENAMESPACE);
		if (itPreNS != _callbacks.end())
		{
			(*(itPreNS->second))(0, pStruct, gen, addParam);
		}

		gen.writeNameSpaceBegin(gen.nameSpace());
	}

	Callbacks::iterator itPreC = _callbacks.find(PRECLASS);
	if (itPreC != _callbacks.end())
	{
		(*(itPreC->second))(0, pStruct, gen, addParam);
	}

	CodeGenerator::Properties classProperties;
	parseProperties(pStruct, classProperties);
	gen.structStart(pStruct, classProperties);

	Poco::CppParser::NameSpace::SymbolTable typeDefs;
	pStruct->typeDefs(typeDefs);
	handleTypedefs(typeDefs, pStruct, gen);

	Poco::CppParser::NameSpace::SymbolTable typeAliases;
	pStruct->typeAliases(typeAliases);
	handleUsings(typeAliases, pStruct, gen);

	Poco::CppParser::Struct::Functions functions;
	pStruct->constructors(functions);
	handleFunctions(functions, pStruct, addParam, classProperties, gen);
	functions.clear();
	if (pStruct->destructor() != 0)
		handleFunction(pStruct->destructor(), pStruct, addParam, classProperties, gen);

	pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functions);
	handleFunctions(functions, pStruct, addParam, classProperties, gen);
	functions.clear();
	pStruct->methods(Poco::CppParser::Symbol::ACC_PROTECTED, functions);
	handleFunctions(functions, pStruct, addParam, classProperties, gen);
	functions.clear();
	pStruct->methods(Poco::CppParser::Symbol::ACC_PRIVATE, functions);
	handleFunctions(functions, pStruct, addParam, classProperties, gen);

	// now generate member variables
	gen.variablesStart();
	Poco::CppParser::NameSpace::SymbolTable vars;
	pStruct->variables(vars);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itVar = vars.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itVarEnd = vars.end();
	for (; itVar != itVarEnd; ++itVar)
	{
		gen.variable(static_cast<const Poco::CppParser::Variable*>(itVar->second));
	}
	gen.variablesEnd();

	gen.structEnd();

	Callbacks::iterator itPostC = _callbacks.find(POSTCLASS);
	if (itPostC != _callbacks.end())
	{
		(*(itPostC->second))(0, pStruct, gen, addParam);
	}

	if (isLastClass)
	{
		gen.writeNameSpaceEnd(gen.nameSpace());

		Callbacks::iterator itPostNS = _callbacks.find(POSTNAMESPACE);
		if (itPostNS != _callbacks.end())
		{
			(*(itPostNS->second))(0, pStruct, gen, addParam);
		}

		gen.endFile();
	}
}


void GeneratorEngine::handleFunctions(const Poco::CppParser::Struct::Functions& functions, const Poco::CppParser::Struct* pStruct, void* addparam, const CodeGenerator::Properties& classProperties, CodeGenerator& gen)
{
	Poco::CppParser::Struct::Functions::const_iterator it = functions.begin();
	Poco::CppParser::Struct::Functions::const_iterator itEnd = functions.end();
	for (; it != itEnd; ++it)
	{
		CodeGenerator::Properties methodProperties(classProperties);
		parseProperties(*it, methodProperties);
		handleFunction(*it, pStruct, addparam, methodProperties, gen);
	}
}


void GeneratorEngine::handleFunction(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, void* addparam, const CodeGenerator::Properties& methodProperties, CodeGenerator& gen)
{
	gen.methodStart(pFunc, methodProperties);
	Callbacks::iterator itC = _callbacks.find(pFunc->name());
	if (itC != _callbacks.end())
	{
		(*(itC->second))(pFunc, pStruct, gen, addparam);
	}
	else
	{
		// try default callback
		itC = _callbacks.find(GeneratorEngine::WILDCARD);
		if (itC != _callbacks.end())
			(*(itC->second))(pFunc, pStruct, gen, addparam);
	}
	gen.methodEnd(pFunc, methodProperties);
}


void GeneratorEngine::handleTypedefs(const Poco::CppParser::NameSpace::SymbolTable& typedefs, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	int cnt = 0;
	Poco::CppParser::NameSpace::Iterator it = typedefs.begin();
	for (; it != typedefs.end(); ++it, ++cnt)
	{
		const TypeDef* pDef = static_cast<const TypeDef*>(it->second);
		gen.writeTypeDef(pDef);
	}
	if (cnt > 0)
		gen.writeTypeDef(0);
}


void GeneratorEngine::handleUsings(const Poco::CppParser::NameSpace::SymbolTable& usings, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	int cnt = 0;
	Poco::CppParser::NameSpace::Iterator it = usings.begin();
	for (; it != usings.end(); ++it, ++cnt)
	{
		const TypeAlias* pDef = static_cast<const TypeAlias*>(it->second);
		gen.writeUsing(pDef);
	}
	if (cnt > 0)
		gen.writeUsing(0);
}


void GeneratorEngine::handleIncludes(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	// iterate over all direct base classes, include all of them
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		GeneratorEngine::handleIncludeFile(pParent, itB->name, pStruct->getLibrary(), gen);
	}
	// iterate over all variables, member as function parameters
	// TODO
	gen.writeIncludes();
}


void GeneratorEngine::handleIncludeFile(const Poco::CppParser::Struct* pStruct, const std::string& classDecl, const std::string& defaultLibrary, CodeGenerator& gen)
{
	if (pStruct)
	{
		Utility::handleInclude(pStruct, gen);
	}
	else
	{
		// no parent means either unknown file (not included in the search dirs)
		// or a system file
		// check if the file is from std
		std::size_t pos = classDecl.find("std::");
		if (pos != std::string::npos)
		{
			GeneratorEngine::handleSystemInclude(classDecl.substr(pos+5), gen);
		}
		else
		{
			// check if we have a namespace, if non, we assume that we extend from a file
			// that has the same namespace
			pos = classDecl.rfind("::");
			std::string libraryName;
			if (pos != std::string::npos)
			{
				libraryName = classDecl;
			}
			else
			{
				// parentdecl contains only the class
				libraryName = defaultLibrary + "::" + classDecl;
			}
			libraryName.append(".h");
			libraryName = Poco::CppParser::replace(libraryName, "::", "/");
			gen.addIncludeFile(libraryName);
		}
	}
}


void GeneratorEngine::handleSystemInclude(const std::string& typeDecl, CodeGenerator& gen)
{
	// could be a template
	std::size_t pos = typeDecl.find("<");
	if (pos != std::string::npos)
	{
		std::string tmp = typeDecl.substr(0, pos);
		Poco::trimInPlace(tmp);
		// most common templates have an include file with the same name as the template
		gen.addSystemIncludeFile(tmp);
	}
	else
	{
		// TODO: check the type
		gen.addSystemIncludeFile(typeDecl);
	}

}


void GeneratorEngine::parseProperties(const Poco::CppParser::Symbol* pSym, CodeGenerator::Properties& props)
{
	const Poco::CppParser::Attributes& attrs = pSym->getAttributes();
	Poco::CppParser::Attributes::Iterator it = attrs.begin();
	Poco::CppParser::Attributes::Iterator itEnd = attrs.end();
	CodeGenerator::Properties tmpProps;
	for (;it != itEnd; ++it)
	{
		// attrs can contain: name=value, var1.name=value, var1.type=attr
		// -> convert to name=value, var1={name =value, type = attr
		Poco::StringTokenizer tok(it->first, ".", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		if (tok.count() == 1)
		{
			std::pair<CodeGenerator::Properties::iterator, bool> ok = tmpProps.insert(std::make_pair(tok[0], it->second));
			poco_assert (ok.second);
		}
		else
		{
			poco_assert (tok.count() == 2);
			CodeGenerator::Properties::iterator itSub = tmpProps.find(tok[0]);
			poco_assert (itSub != tmpProps.end());
			if (itSub->second.empty())
				itSub->second = "{";
			else
				itSub->second.append(", ");

			itSub->second.append(tok[1]);
			itSub->second.append(" = ");
			itSub->second.append(it->second);
		}
	}
	CodeGenerator::Properties::iterator itP = tmpProps.begin();
	CodeGenerator::Properties::iterator itPEnd = tmpProps.end();
	for (;itP != itPEnd; ++itP)
	{
		if (!itP->second.empty() && itP->second[0] == '{')
		{
			if (itP->second[itP->second.size() - 1] != '}')
			{
				itP->second.append("}");
			}
		}
		std::pair<CodeGenerator::Properties::iterator, bool> ok = props.insert(std::make_pair(itP->first, itP->second));
		if (!ok.second)
		{
			ok.first->second = itP->second;
		}
	}
}


void GeneratorEngine::parseKeyValue(const std::string& keyValue, CodeGenerator::Properties& props)
{
	// search for =. don't use the stringtokenizer becuase we will have attr = {val = x, val2 = y}
	std::string key;
	std::string value(Utility::VAL_TRUE);
	std::size_t pos = keyValue.find(Utility::VAL_ASSIGNMENT);
	if (pos != std::string::npos)
	{
		key = keyValue.substr(0, pos);
		value = keyValue.substr(pos + Utility::VAL_ASSIGNMENT.length());
	}
	else
		key = keyValue;

	Poco::trimInPlace(key);
	Poco::trimInPlace(value);

	std::pair<CodeGenerator::Properties::iterator, bool> ok = props.insert(std::make_pair(key, value));
	if (!ok.second)
	{
		ok.first->second = value;
	}
}


void GeneratorEngine::parseElementProperties(const std::string& elemVal, CodeGenerator::Properties& props)
{
	if (elemVal.empty())
		return;
	std::size_t posStart = elemVal.find(Utility::PROPERTYSTARTELEM);
	std::size_t posEnd = elemVal.rfind(Utility::PROPERTYENDELEM);
	if (posStart == std::string::npos || posEnd == std::string::npos)
		return;

	std::string prop = elemVal.substr(posStart+Utility::PROPERTYSTARTELEM.length(), posEnd-posStart-Utility::PROPERTYSTARTELEM.length());
	Poco::StringTokenizer tok(prop, Utility::VAL_SEPARATOR, Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
	Poco::StringTokenizer::Iterator it = tok.begin();
	Poco::StringTokenizer::Iterator itEnd = tok.end();
	for (; it != itEnd; ++it)
	{
		const std::string& keyValue = *it;
		parseKeyValue(keyValue, props);
	}
}


bool GeneratorEngine::getStringProperty(const CodeGenerator::Properties& props, const std::string& propName, std::string& value)
{
	CodeGenerator::Properties::const_iterator it = props.find(propName);
	bool found = (it != props.end());
	if (found)
	{
		value = it->second;
	}
	return found;
}


bool GeneratorEngine::getBoolProperty(const CodeGenerator::Properties& props, const std::string& propName, bool& value)
{
	std::string val;
	bool retVal = getStringProperty(props, propName, val);
	if (!retVal)
		return retVal;
	Poco::toLowerInPlace(val);
	if (val == Utility::VAL_FALSE || val == "0")
		value = false;
	else
		value = true;

	return retVal;
}


bool GeneratorEngine::getUInt32Property(const CodeGenerator::Properties& props, const std::string& propName, Poco::UInt32& value)
{
	std::string val;
	bool retVal = getStringProperty(props, propName, val);
	if (!retVal)
		return retVal;
	value = Poco::NumberParser::parseUnsigned(val);

	return retVal;
}


void GeneratorEngine::emptyCodeGen(const Poco::CppParser::Function*, const Poco::CppParser::Struct*, CodeGenerator&, void*)
{
}


void GeneratorEngine::skipWhiteSpace(const std::string& str, std::size_t& startPos)
{
	while (isWhiteSpace(str[startPos]) && startPos < str.size())
		++startPos;
}


std::string GeneratorEngine::parseToken(const std::string& str, std::size_t& endPos)
{
	skipWhiteSpace(str, endPos);
	std::size_t beginPos = endPos;
	while (endPos < str.length() && !isWhiteSpace(str[endPos]) && str[endPos] != ',' && str[endPos] != '=' && str[endPos] != '{' && str[endPos] != '[' && str[endPos] != '}' && str[endPos] != ']')
	{
		++endPos;
	}
	std::string result = str.substr(beginPos, endPos-beginPos);
	skipWhiteSpace(str, endPos);
	return result;
}


bool GeneratorEngine::isWhiteSpace(const char c)
{
	return (c==' ' || c=='\t' || c == '\r' || c == '\n');
}


} } // namespace Poco::CodeGeneration
