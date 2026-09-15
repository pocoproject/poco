//
// Utility.cpp
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  Utility
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Function.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <fstream>
#include <sstream>
#include <cctype>


namespace Poco {
namespace CodeGeneration {


const std::string Utility::HFILEEXTENSION(".h");
const std::string Utility::CPPFILEEXTENSION(".cpp");
const std::string Utility::REMOTE("remote");
const std::string Utility::SYNCHRONIZED("synchronized");
const std::string Utility::SERIALIZE("serialize");
const std::string Utility::GENERATED("generated");
const std::string Utility::CACHING("cacheResult");
const std::string Utility::CACHEEXPIRETIME("cacheExpire");
const std::string Utility::ONEWAY("oneway");
const std::string Utility::RETURN("return");
const std::string Utility::PUBLIC("public");
const std::string Utility::PRIVATE("private");
const std::string Utility::PROTECTED("protected");
const std::string Utility::VIRTUAL("virtual");
const std::string Utility::STATIC("static");
const std::string Utility::CLASS("class");
const std::string Utility::PROPERTYSTART("[[");
const std::string Utility::PROPERTYEND("]]");
const std::string Utility::PROPERTYSTARTELEM("{");
const std::string Utility::PROPERTYENDELEM("}");
const std::string Utility::TYPE_VOID("void");
const std::string Utility::NAME("name");
const std::string Utility::TYPE("type");
const std::string Utility::ORDER("order");
const std::string Utility::DIRECTION("direction");
const std::string Utility::NAMESPACE("namespace");
const std::string Utility::MANDATORY("mandatory");
const std::string Utility::OPTIONAL("optional");
const std::string Utility::HEADER("header");
const std::string Utility::ACTION("action");
const std::string Utility::REQUEST("request");
const std::string Utility::REPLY("reply");
const std::string Utility::FAULT("fault");
const std::string Utility::FILTER("filter");
const std::string Utility::IN("in");
const std::string Utility::FORMAT("format");
const std::string Utility::PATH("path");
const std::string Utility::PRODUCES("produces");
const std::string Utility::CONSUMES("consumes");
const std::string Utility::CONTENT_TYPE("contentType");
const std::string Utility::PERMISSION("permission");
const std::string Utility::AUTHENTICATED("authenticated");
const std::string Utility::LENGTH("length");
const std::string Utility::XSDTYPE("xsdType");
const std::string Utility::VAL_TRUE("true");
const std::string Utility::VAL_FALSE("false");
const std::string Utility::VAL_ASSIGNMENT("=");
const std::string Utility::VAL_SEPARATOR(",");
const std::string Utility::VAL_INFINITE("infinite");
const std::string Utility::VAL_ATTR("attr");
const std::string Utility::VAL_ELEM("elem");


void Utility::writeStdHeader(std::ostream& ostr, const std::string& fileName, const std::string& libraryName, const std::string& packageName, const std::string& moduleName, const std::string& copyright, bool writeTimestamp)
{
	Poco::DateTime now;
	ostr << "//\n";
	ostr << "// " << fileName << "\n";
	ostr << "//\n";
	if (!libraryName.empty()) ostr << "// Library: " << libraryName << "\n";
	if (!packageName.empty()) ostr << "// Package: " << packageName << "\n";
	if (!moduleName.empty()) ostr << "// Module:  " << moduleName  << "\n";
	ostr << "//\n";
	ostr << "// This file has been generated";
	if (writeTimestamp)
		ostr << " on " << Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::SORTABLE_FORMAT) << " UTC.\n";
	else
		ostr << ".\n";
	ostr << "// Warning: All changes to this will be lost when the file is re-generated.\n";
	if (!copyright.empty())
	{
		ostr << "//\n";
		char line[1024];
		std::istringstream iStr(copyright);
		while (!iStr.eof())
		{
			iStr.getline(line, 1024);
			std::string strLine(line);
			Poco::trimInPlace(strLine);
			ostr << "// " << strLine << "\n";
		}
	}
	ostr << "//\n\n\n";
}


void Utility::writeIncludeGuard(std::ostream& ostr, const std::string& libraryName, const std::string& fileName)
{
	std::string define(Utility::createIncludeGuard(libraryName, fileName));
	ostr << "#ifndef " << define << "\n";
	ostr << "#define " << define << "\n\n\n";
}


void Utility::writeInclude(std::ostream& ostr, const std::string& libraryName, const std::string& fileName)
{
	if (!libraryName.empty())
		ostr << "#include \"" << libraryName << "/" << fileName << "\"\n";
	else
		ostr << "#include \"" << fileName << "\"\n";
}


std::string Utility::createIncludeGuard(const std::string& libraryName, const std::string& fileName)
{
	std::string define(libraryName);
	if (!define.empty())
		define.append("_");
	define.append(fileName);
	define.append("_INCLUDED");
	Poco::replaceInPlace(define, "/", "_");
	return define;
}


void Utility::writeLines(std::ostream& ostr, const std::vector<std::string>& lines, bool appendNewLine)
{
	std::vector<std::string>::const_iterator it = lines.begin();
	std::vector<std::string>::const_iterator itEnd = lines.end();

	for (; it != itEnd; ++it)
	{
		ostr << *it;
		if (appendNewLine)
			ostr << "\n";
	}
}


void Utility::writeClassDeclaration(std::ostream& ostr, const std::string& libName, const std::string& className)
{
	ostr << "class ";
	if (!libName.empty())
	{
		ostr << libName << "_API ";
	}
	ostr << className << "\n" << "{" << "\n";
}


void Utility::writeAttributes(std::ostream& out, const Poco::CppParser::Attributes& attrs, int indent)
{
	std::string prefix;
	while (indent--)
	{
		prefix.append("\t");
	}

	for (Poco::CppParser::Attributes::Iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		out << prefix << "//@ " << it->first;
		if (!it->second.empty())
		{
			out << "=";
			bool mustQuote = false;
			if (it->second[0] != '{')
			{
				for (std::string::const_iterator itv = it->second.begin(); itv != it->second.end(); ++itv)
				{
					bool isAlNum = (*itv >= '0' && *itv <= '9')
						|| (*itv >= 'a' && *itv <= 'z')
						|| (*itv >= 'A' && *itv <= 'Z')
						|| (*itv == '_');
					if (!isAlNum) mustQuote = true;

				}
			}
			std::string quote(mustQuote ? "\"" : "");
			out << quote << it->second << quote << std::endl;
		}
		else out << std::endl;
	}
}


void Utility::writeClassDeclaration(std::ostream& ostr, const std::string& libName, const Poco::CppParser::Struct* pStruct, bool enableAttributes)
{
	if (enableAttributes)
	{
		writeAttributes(ostr, pStruct->attrs(), 0);
	}
	Poco::CppParser::Struct::BaseIterator begin = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator end = pStruct->baseEnd();
	bool isTemplate = (pStruct->flags() & Poco::CppParser::Struct::FN_TEMPLATE);
	if (isTemplate)
	{
		ostr << pStruct->declaration();
	}
	else
	{
		ostr << "class ";
		if (!libName.empty())
		{
			ostr << libName << "_API ";
		}
		ostr << pStruct->name();
	}
	if (begin != end)
	{
		// inheritance info
		ostr << ": ";
		Utility::writeAccess(ostr, begin->access);
		if (begin->isVirtual)
			ostr << " " << Utility::VIRTUAL;
		ostr << " " << begin->name;

		++begin;
		while (begin != end)
		{
			ostr << ", ";
			Utility::writeAccess(ostr, begin->access);
			if (begin->isVirtual)
				ostr << " " << Utility::VIRTUAL;
			ostr << " " << begin->name;
			++begin;
		}
	}
	const std::string& doc = pStruct->getDocumentation();
	if (!doc.empty())
	{
		ostr << "\n\t///";
		ostr << Poco::CppParser::replace(doc, "\n", "\n\t///");
	}
	ostr << "\n" << "{" << "\n";
}


void Utility::writeAccess(std::ostream& ostr, Poco::CppParser::Symbol::Access acc)
{
	if (acc == Poco::CppParser::Symbol::ACC_PUBLIC)
		ostr << Utility::PUBLIC;
	else if(acc == Poco::CppParser::Symbol::ACC_PROTECTED)
		ostr << Utility::PROTECTED;
	else
		ostr << Utility::PRIVATE;
}


void Utility::createFiles(const std::string& fileNameCore, std::ofstream& hFile, std::ofstream& cppFile)
{
	std::string hFileName(fileNameCore);
	std::string cppFileName(fileNameCore);
	hFileName.append(Utility::HFILEEXTENSION);
	cppFileName.append(Utility::CPPFILEEXTENSION);
	hFile.open(hFileName.c_str());
	if (!hFile.good())
		throw Poco::OpenFileException(hFileName);
	cppFile.open(cppFileName.c_str());
	if (!cppFile.good())
	{
		hFile.close();
		throw Poco::OpenFileException(cppFileName);
	}
}


std::string Utility::extractTemplateFromClassDecl(const Poco::CppParser::Struct* pStruct)
{
	const std::string& origDecl = pStruct->declaration();
	std::size_t pos = origDecl.find(pStruct->name());
	pos = origDecl.rfind(">", pos);
	std::size_t posStart = origDecl.find("template");
	poco_assert (pos != std::string::npos);
	return origDecl.substr(posStart, pos+1-posStart);
}


bool Utility::hasAnyRemoteProperty(const Poco::CppParser::Struct* pStruct)
{
	CodeGenerator::Properties classProperties;
	GeneratorEngine::parseProperties(pStruct,classProperties);
	bool remote = (classProperties.find(REMOTE) != classProperties.end());
	if (remote)
		return remote;

	// else check if we find any method that has this property

	Poco::CppParser::Struct::Functions functions;
	pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functions);
	Poco::CppParser::Struct::Functions::const_iterator it = functions.begin();
	Poco::CppParser::Struct::Functions::const_iterator itEnd = functions.end();
	for (; it != itEnd; ++it)
	{
		CodeGenerator::Properties methodProperties;
		GeneratorEngine::parseProperties(*it, methodProperties);
		remote = (methodProperties.find(REMOTE) != methodProperties.end());
		if (remote)
			return remote;
	}
	return remote;
}


void Utility::handleInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	bool usePocoIncludes = gen.usePocoIncludeStyle();
	std::string inc(createInclude(pStruct, usePocoIncludes));
	gen.addIncludeFile(inc);
}


void Utility::handleSrcInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	bool usePocoIncludes = gen.usePocoIncludeStyle();
	std::string inc(createInclude(pStruct, usePocoIncludes));
	gen.addSrcIncludeFile(inc);
}


std::string Utility::createInclude(const Poco::CppParser::Struct* pStruct, bool usePocoIncludes)
{
	const std::string& file2 = pStruct->getFile();
	if (!file2.empty())
	{
		return createInclude(file2, false);
	}
	else
	{
		// take the ns as path
		std::string fullName(pStruct->fullName());
		fullName = CppParser::replace(fullName, "::","/");
		fullName.append(".h");
		return fullName;
	}
}


std::string Utility::createInclude(const std::string& filePath, bool flatInclude)
{
	poco_assert (!filePath.empty());
	const Poco::Path aFile(filePath);
	// common rule: include/Poco/libraryName/FileName.h or with core components simply include Poco/FileName.h

	if (flatInclude)
		return aFile.getFileName();

	int start = aFile.depth()-1;
	bool found = false;
	do
	{
		std::string directory = Poco::toLower(aFile.directory(start));
		if (directory.find("include") != std::string::npos)
			found = true;
		else
			--start;
	}
	while(!found && start >= 0);
	if (!found)
		start = aFile.depth();
	else
		++start;

	std::string result;
	for (int i = start; i < aFile.depth(); ++i)
	{
		if (i > start)
		{
			result.append("/");
		}
		result.append(aFile.directory(i));
	}
	if (!result.empty())
		result.append("/");

	result.append(aFile.getFileName());
	return result;
}


void Utility::handleBaseClassesInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen)
{
	poco_assert_dbg(pStruct);
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent)
			Utility::handleInclude(pParent, gen);
		else
		{
			// guess the include from the lib and name
			// no parent means either unknown file (not included in the search dirs)
			// or a system file
			// check if the file is from std
			const std::string& classDecl = itB->name;
			std::size_t pos = classDecl.find("std::");
			if (pos == 0)
			{
				gen.addSystemIncludeFile(classDecl.substr(pos+5));
			}
		}
	}
}


std::string Utility::resolveType(const Poco::CppParser::NameSpace* pParent, const std::string& name)
{
	return resolveTypeImpl(pParent, name);
}


std::string Utility::resolveTypeImpl(const Poco::CppParser::NameSpace* pParent, const std::string& name)
{
	if (!pParent) return name;

	static int recursionCount = 0;
	RecursionGuard recursionGuard(recursionCount, Poco::RuntimeException("Recursion limit reached resolving type", name));

	// template support
	std::size_t pos = name.find("<");
	std::size_t posEnd = name.rfind(">");
	if (pos != std::string::npos && posEnd != std::string::npos)
	{
		std::string innerType = name.substr(pos+1, posEnd-pos-1);
		std::string resolvedInnerType;
		// inner type of a map: int, std::map<std::string, int>
		// parse that
		int openTemplates = 0;
		int end = 0;
		for (int i = 0; i < innerType.size(); ++i)
		{
			if (innerType[i] == '<')
				openTemplates++;
			else if ((innerType[i] == ',' && openTemplates == 0) || (i+1 == innerType.size()))
			{
				if (i+1 == innerType.size())
					++i;
				std::string innerPart(innerType.substr(end, i-end));
				end = i+1;
				if (!resolvedInnerType.empty())
					resolvedInnerType.append(", ");
				resolvedInnerType.append(resolveTypeImpl(pParent, innerPart));
			}
			else if (innerType[i] == '>')
				openTemplates--;
		}

		std::string pre = name.substr(0, pos);
		std::string post = name.substr(posEnd+1);
		Poco::trimInPlace(pre);
		Poco::trimInPlace(post);
		std::string templateType(resolveTypeImpl(pParent, pre));
		templateType.append("<");
		templateType.append(resolvedInnerType);
		templateType.append(">");
		templateType.append(post);
		return templateType;
	}

	std::string core(Poco::trim(name));
	bool isConst = (core.find("const") == 0) && std::isspace(core[5]);
	std::size_t constPos = isConst? 5 : 0;
	std::string result;
	if (isConst)
	{
		while (std::isspace(core[constPos]))
			++constPos;
		result = "const ";
	}
	std::size_t endPos = core.size();
	while (endPos > 1 && (core[endPos-1] == '&' || core[endPos-1] == '*'))
		--endPos;
	std::string post(core.substr(endPos));
	std::string type(core.substr(constPos, endPos-constPos));
	Poco::trimInPlace(type);
	Poco::CppParser::Symbol* pSym = pParent->lookup(type);
	if (!pSym)
	{
		return result + type + post;
	}
	// Handle the rare case of a recursive data structure. In this case we
	// will find a constructor, but we need the class (which is the parent).
	if (pSym->kind() == Poco::CppParser::Symbol::SYM_FUNCTION && pSym->name() == pParent->name())
	{
		pSym = pSym->nameSpace();
	}
	std::string fN = pSym->fullName();
	if (pSym->kind() == Poco::CppParser::Symbol::SYM_TYPEDEF)
	{
		std::string base = static_cast<Poco::CppParser::TypeDef*>(pSym)->baseType();
		if (base != name) 
			return result + resolveTypeImpl(pSym->nameSpace(), base) + post;
		else
			return result + base + post;
	}
	else if (pSym->kind() == Poco::CppParser::Symbol::SYM_TYPEALIAS)
	{
		std::string base = static_cast<Poco::CppParser::TypeAlias*>(pSym)->baseType();
		if (base != name)
			return result + resolveTypeImpl(pSym->nameSpace(), base) + post;
		else
			return result + base + post;
	}
	else return result + fN + post;
}


std::string Utility::resolveParamDecl(const Poco::CppParser::NameSpace* pParent, const Poco::CppParser::Parameter* pSym)
{
	const std::string& decl = pSym->declaration();
	// type <space> var
	std::size_t pos = decl.rfind(' ');

	std::string resType = Utility::resolveType(pParent, decl.substr(0, pos));
	if (pos != std::string::npos)
	{
		resType.append(decl.substr(pos));
	}
	return resType;
}


void Utility::writeNameSpaceBegin(std::ostream& out, const std::string& nameSpace)
{
	if (!nameSpace.empty())
	{
		Poco::StringTokenizer aTok(nameSpace, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		int nsCnt = (int)aTok.count();

		for (int i = 0; i < nsCnt; ++i)
		{
			out << "namespace " << aTok[i] << " {" << std::endl;
		}
	}
}


void Utility::writeNameSpaceEnd(std::ostream& out, const std::string& nameSpace)
{
	if (!nameSpace.empty())
	{
		Poco::StringTokenizer aTok(nameSpace, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		int nsCnt = (int)aTok.count();

		for (int i = 0; i < nsCnt; ++i)
		{
			out << "}" << std::endl;
		}
	}
}


void Utility::writeDocumentation(std::ostream& out, const std::string& docu, int indent)
{
	std::string prefix;
	while (indent--)
	{
		prefix.append("\t");
	}

	char line[1024];
	std::istringstream iStr(docu);
	while (!iStr.eof())
	{
		iStr.getline(line, 1024);
		std::string strLine(line);
		Poco::trimInPlace(strLine);
		out << prefix << "/// " << strLine << "\n";
	}
}


} } // namespace Poco::CodeGeneration
