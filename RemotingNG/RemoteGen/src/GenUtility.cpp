//
// GenUtility.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/TypeDef.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"
#include <typeinfo>
#include <cctype>


POCO_IMPLEMENT_EXCEPTION(CodeGenerationException, Poco::ApplicationException, "Code generation error")


const std::string GenUtility::ATTR_INLINE("inline");
const std::string GenUtility::ATTR_NAME("name");
const std::string GenUtility::ATTR_REPLYNAME("replyName");
const std::string GenUtility::VAL_REQUEST("Request");
const std::string GenUtility::VAL_REPLY("Reply");
const std::string GenUtility::ATTR_RETURN("return");
const std::string GenUtility::ATTR_HEADER("header");
const std::string GenUtility::KEYS_VECTOR[KEYS_VECTOR_SIZE] = {"std::vector", "std::set", "std::multiset", "std::unordered_set", "std::unordered_multiset", "std::list", "std::array", "Poco::Array"};
const std::string GenUtility::KEYS_UNIQUE_VECTOR[KEYS_UNIQUE_VECTOR_SIZE] = {"std::set", "std::unordered_set"};


bool GenUtility::isRemoteMethod(const Poco::CppParser::Function* pFunc, const Poco::CodeGeneration::CodeGenerator::Properties& properties)
{
	if (!pFunc->isDefault() && !pFunc->isDeleted() && !pFunc->isStatic() && pFunc->isPublic())
	{
		bool remote = false;
		return (Poco::CodeGeneration::GeneratorEngine::getBoolProperty(properties, Poco::CodeGeneration::Utility::REMOTE, remote) && remote);
	}
	else return false;
}


bool GenUtility::isRemoteEvent(const Poco::CppParser::Variable* pVar, const Poco::CodeGeneration::CodeGenerator::Properties& properties)
{
	if (pVar->isPublic() && !pVar->isStatic())
	{
		const std::string& varType = pVar->declType();
		if (varType.find("Poco::BasicEvent") == 0 || varType.find("Poco::FIFOEvent") == 0)
		{
			bool remote = false;
			return (Poco::CodeGeneration::GeneratorEngine::getBoolProperty(properties, Poco::CodeGeneration::Utility::REMOTE, remote) && remote);
		}
	}
	return false;
}


std::string GenUtility::getMethodName(const Poco::CppParser::Function* pFunc)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string aName(pFunc->name());
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_NAME, aName);
	return aName;
}


std::string GenUtility::getRequestMethodName(const Poco::CppParser::Function* pFunc)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string aName(pFunc->name());
	std::string replyName(aName+GenUtility::VAL_REQUEST);
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_REPLYNAME, replyName);
	return replyName;
}


std::string GenUtility::getReplyMethodName(const Poco::CppParser::Function* pFunc)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string aName(pFunc->name());
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_NAME, aName);
	std::string replyName(aName+GenUtility::VAL_REPLY);
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_REPLYNAME, replyName);
	return replyName;
}


std::string GenUtility::getParameterName(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string name(pParam->name());
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + pParam->name(), elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		Poco::CodeGeneration::GeneratorEngine::getStringProperty(elemProps, GenUtility::ATTR_NAME, name);
	}
	return name;
}


std::string GenUtility::getVariableName(const Poco::CppParser::Variable* pVar)
{
	Poco::CodeGeneration::CodeGenerator::Properties props;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pVar, props);
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itName = props.find(GenUtility::ATTR_NAME);
	std::string result;
	if (itName != props.end())
	{
		result = itName->second;
	}
	else
	{
		result = cleanVariableName(pVar->name());
	}

	return result;
}


Poco::UInt32 GenUtility::getVariableOrder(const Poco::CppParser::Variable* pVar, Poco::UInt32 defValue)
{
	Poco::CodeGeneration::CodeGenerator::Properties props;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pVar, props);
	Poco::CodeGeneration::GeneratorEngine::getUInt32Property(props, Poco::CodeGeneration::Utility::ORDER, defValue);

	return defValue;
}


Poco::UInt32 GenUtility::getParameterOrder(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, Poco::UInt32 defValue)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + pParam->name(), elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		Poco::CodeGeneration::GeneratorEngine::getUInt32Property(elemProps, Poco::CodeGeneration::Utility::ORDER, defValue);
	}

	return defValue;
}


bool GenUtility::getIsMandatory(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam)
{
	bool isMandatory = true;
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + pParam->name(), elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		isMandatory = getIsMandatory(elemProps);
	}
	return isMandatory;
}


bool GenUtility::getIsMandatory(const Poco::CppParser::Variable* pVar)
{
	Poco::CodeGeneration::CodeGenerator::Properties props;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pVar, props);
	return getIsMandatory(props);
}


bool GenUtility::getIsMandatory(const Poco::CodeGeneration::CodeGenerator::Properties& props)
{
	bool isMandatory = true;
	if (!Poco::CodeGeneration::GeneratorEngine::getBoolProperty(props, Poco::CodeGeneration::Utility::MANDATORY, isMandatory))
	{
		bool isOptional = !isMandatory;
		if (Poco::CodeGeneration::GeneratorEngine::getBoolProperty(props, Poco::CodeGeneration::Utility::OPTIONAL, isOptional))
		{
			isMandatory = !isOptional;
		}
	}
	return isMandatory;
}


bool GenUtility::getIsInHeader(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam)
{
	bool soapHeader = false;
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + pParam->name(), elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		Poco::CodeGeneration::GeneratorEngine::getBoolProperty(elemProps, ATTR_HEADER, soapHeader);
	}

	return soapHeader;
}


std::string GenUtility::cleanVariableName(const std::string& origName)
{
	std::string result;
	std::size_t pos = origName.find("_");
	if (pos != std::string::npos)
	{
		// is it at end or at the beginning?
		if (pos == (origName.size()-1))
		{
			result = origName.substr(0, pos);
		}
		else if (pos == 0 || (pos == 1 && origName[0] == 'm'))
		{
			// at beginning
			result = origName.substr(pos+1);
		}
		else
			result = origName;
	}
	else
		result = origName;

	return result;
}


std::string GenUtility::getReturnParameterName(const Poco::CppParser::Function* pFunc)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string name(GenUtility::ATTR_RETURN);
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_RETURN, elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		Poco::CodeGeneration::GeneratorEngine::getStringProperty(elemProps, GenUtility::ATTR_NAME, name);
	}
	return name;
}


bool GenUtility::getInlineReturnParam(const Poco::CppParser::Function* pFunc)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	bool doInline = false;
	std::string elemStr;
	bool found = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_RETURN, elemStr);
	if (found && !elemStr.empty())
	{
		Poco::CodeGeneration::CodeGenerator::Properties elemProps;
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemStr, elemProps);
		Poco::CodeGeneration::GeneratorEngine::getBoolProperty(elemProps, GenUtility::ATTR_INLINE, doInline);
	}
	return doInline;
}


std::string GenUtility::getReturnParameterType(const Poco::CppParser::Function* pFunc)
{
	if (pFunc->getReturnParameter().empty() || pFunc->getReturnParameter() == Poco::CodeGeneration::Utility::TYPE_VOID)
		return Poco::CodeGeneration::Utility::TYPE_VOID;

	return pFunc->getReturnParameter();
}


std::string GenUtility::getResolvedReturnParameterType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Function* pFunc)
{
	std::string retParam(getReturnParameterType(pFunc));
	return Poco::CodeGeneration::Utility::resolveType(pNS, retParam);
}


bool GenUtility::isVectorType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam)
{
	std::string paramType = Poco::CodeGeneration::Utility::resolveType(pNS, pParam->declType());
	return isVectorType(paramType);
}


bool GenUtility::isVectorType(const std::string& resolvedType)
{
	if (resolvedType == "std::vector < char >") return false;
	bool found = false;
	for (int i = 0; i < KEYS_VECTOR_SIZE && !found; ++i)
	{
		found = (resolvedType.find(KEYS_VECTOR[i]) == 0);
	}
	return found;
}


bool GenUtility::isUniqueVectorType(const std::string& resolvedType)
{
	bool found = false;
	for (int i = 0; i < KEYS_UNIQUE_VECTOR_SIZE && !found; ++i)
	{
		found = (resolvedType.find(KEYS_UNIQUE_VECTOR[i]) == 0);
	}
	return found;
}


bool GenUtility::isArrayType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam)
{
	std::string paramType = Poco::CodeGeneration::Utility::resolveType(pNS, pParam->declType());
	return isArrayType(paramType);
}


bool GenUtility::isArrayType(const std::string& resolvedType)
{
	return resolvedType.compare(0, 11, "Poco::Array") == 0 || resolvedType.compare(0, 10, "std::array") == 0;
}


bool GenUtility::isNullableType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam)
{
	std::string resolvedType = Poco::CodeGeneration::Utility::resolveType(pNS, pParam->declType());
	return isNullableType(resolvedType);
}


bool GenUtility::isOptionalType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam)
{
	std::string resolvedType = Poco::CodeGeneration::Utility::resolveType(pNS, pParam->declType());
	return isOptionalType(resolvedType);
}


bool GenUtility::isStdOptionalType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam)
{
	std::string resolvedType = Poco::CodeGeneration::Utility::resolveType(pNS, pParam->declType());
	return isStdOptionalType(resolvedType);
}


bool GenUtility::isTemplateType(const std::string& resolvedType)
{
	std::size_t posStart = resolvedType.find('<');
	std::size_t posEnd = resolvedType.find('>');
	return (posStart != std::string::npos && posEnd != std::string::npos && posStart < posEnd);
}


bool GenUtility::isPtrType(const std::string& type)
{
	return type.compare(0, 7, "AutoPtr") == 0
	    || type.compare(0, 13, "Poco::AutoPtr") == 0
	    || type.compare(0, 9, "SharedPtr") == 0
	    || type.compare(0, 15, "Poco::SharedPtr") == 0
	    || type.compare(0, 9, "std::shared_ptr") == 0
	    || type.compare(0, 15, "std::unique_ptr") == 0;
}


bool GenUtility::isNullableType(const std::string& type)
{
	return type.compare(0, 8, "Nullable") == 0
	    || type.compare(0, 14, "Poco::Nullable") == 0;
}


bool GenUtility::isOptionalType(const std::string& type)
{
	return type.compare(0, 8, "Optional") == 0
	    || type.compare(0, 14, "Poco::Optional") == 0
		|| type.compare(0, 13, "std::optional") == 0;
}


bool GenUtility::isStdOptionalType(const std::string& type)
{
	return type.compare(0, 13, "std::optional") == 0;
}


std::vector<std::string> GenUtility::getResolvedInnerTemplateTypes(const Poco::CppParser::NameSpace* pNS, const std::string& typeDecl)
{
	std::string resolved(Poco::CodeGeneration::Utility::resolveType(pNS, typeDecl));
	return getInnerTemplateTypes(resolved);
}


std::vector<std::string> GenUtility::getInnerTemplateTypes(const std::string& typeDecl)
{
	std::vector<std::string> templTypes;
	//a typical template def std::map<int, std::pair<int,int> >
	int lvl = 0;
	std::string temp;
	for (int i = 0; i < typeDecl.size(); ++i)
	{
		char c = typeDecl[i];
		if (c == '<')
		{
			++lvl;
			if (lvl > 1)
				temp.append(&c, 1);
		}
		else if (c == '>')
		{
			--lvl;
			if (lvl == 0)
			{
				Poco::trimInPlace(temp);
				if (!temp.empty() && (Poco::Ascii::isAlpha(temp[0]) || temp[0] == '_')) // Poco::Array<int, 8> -> 8 is not a type
					templTypes.push_back(temp);
				temp.clear();
			}
			else
				temp.append(&c, 1);
		}
		else if (c == ',')
		{
			if (lvl > 0)
			{
				Poco::trimInPlace(temp);
				if (!temp.empty() && (Poco::Ascii::isAlpha(temp[0]) || temp[0] == '_')) // Poco::Array<int, 8> -> 8 is not a type
					templTypes.push_back(temp);
				temp.clear();
			}
			else
				temp.append(&c, 1);
		}
		else
		{
			if (lvl > 0)
			{
				temp.append(&c, 1);
			}
		}
	}
	return templTypes;
}


bool GenUtility::mustBeSerializable(const std::string& name)
{
	return (name != "Thread" && name != "Runnable" && name != "RefCountedObject" && name != "Service");
}


bool GenUtility::hasEvents(const Poco::CppParser::Struct* pStruct)
{
	if (!pStruct)
		return false;

	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		const std::string& varType = pVar->declType();
		if (pVar->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC)
		{
			if (varType.find("Poco::BasicEvent") == 0 || varType.find("Poco::FIFOEvent") == 0)
			{
				return true;
			}
		}
	}
	return parentsHaveEvents(pStruct);
}


bool GenUtility::parentsHaveEvents(const Poco::CppParser::Struct* pStruct)
{
	if (!pStruct)
		return false;

	// check parents
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (itB->pClass)
		{
			if (GenUtility::hasEvents(itB->pClass))
				return true;
		}
	}
	return false;
}


bool GenUtility::hasAnyRemoteParent(const Poco::CppParser::Struct* pStruct)
{
	bool hasAnyRemote = false;
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (itB->pClass)
			hasAnyRemote |= Poco::CodeGeneration::Utility::hasAnyRemoteProperty(itB->pClass);
	}
	return hasAnyRemote;
}


Poco::UInt64 GenUtility::parseExpireTime(const std::string& expireTimeStr)
{
	std::size_t pos = 0;
	while (std::isdigit(expireTimeStr[pos]) && pos < expireTimeStr.size())
		pos++;

	Poco::UInt64 expireTime = Poco::NumberParser::parseUnsigned(expireTimeStr.substr(0, pos));

	// now search for either ms, s(ec), m(in), h(our),
	// the default is ms

	std::string tmp(expireTimeStr.substr(pos));
	Poco::toLowerInPlace(tmp);
	Poco::trimInPlace(tmp);
	// convert to microsec
	if (tmp == "ms" || (tmp.find("milli") == 0))
		expireTime *= 1000;
	else if (tmp == "s" || (tmp.find("sec") == 0))
		expireTime *= (1000*1000);
	else if (tmp == "m" || (tmp.find("min") == 0))
		expireTime *= (1000*1000*60);
	else if (tmp == "h" || (tmp.find("hour") == 0))
	{
		expireTime *= (1000*1000*60);
		expireTime *= 60;
	}
	else
		expireTime *= 1000;

	return expireTime;
}


void GenUtility::checkFunctionParams(const Poco::CppParser::Function* pFunc)
{
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		if ((*it)->name() == (*it)->declType())
			throw CodeGenerationException("Unnamed function parameters are not allowed", pFunc->fullName() + "()");
	}
}


bool GenUtility::isOverride(const std::string& funcName, const Poco::CppParser::Struct* pStruct)
{
	poco_check_ptr (pStruct);

	Poco::CppParser::Struct::Functions methods;
	pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, methods);
	bool isDefinedHere = false;
	for (Poco::CppParser::Struct::Functions::const_iterator it = methods.begin(); !isDefinedHere && it != methods.end(); ++it)
	{
		if ((*it)->name() == funcName)
		{
			isDefinedHere = true;
		}
	}
	if (!isDefinedHere) return false;

	Poco::CppParser::Struct::FunctionSet inherited;
	pStruct->inheritedMethods(inherited);
	for (Poco::CppParser::Struct::FunctionSet::const_iterator it = inherited.begin(); it != inherited.end(); ++it)
	{
		if ((*it)->name() == funcName)
			return true;
	}
	return false;
}
