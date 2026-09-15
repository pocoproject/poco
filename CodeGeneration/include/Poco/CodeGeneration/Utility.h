//
// Utility.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  Utility
//
// Definition of the Utility class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef CodeGeneration_Utility_INCLUDED
#define CodeGeneration_Utility_INCLUDED


#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/Exception.h"
#include <vector>


#if defined(_WIN32)
#undef IN
#undef OPTIONAL
#endif


namespace Poco {
namespace CodeGeneration {


class CodeGenerator;


class CodeGeneration_API Utility
	/// Constants and utility functions for code generation.
{
public:
	static const std::string HFILEEXTENSION;
	static const std::string CPPFILEEXTENSION;
	static const std::string RETURN;
	static const std::string PUBLIC;
	static const std::string PRIVATE;
	static const std::string PROTECTED;
	static const std::string VIRTUAL;
	static const std::string STATIC;
	static const std::string CLASS;
	static const std::string PROPERTYSTART;
	static const std::string PROPERTYEND;
	static const std::string PROPERTYSTARTELEM;
	static const std::string PROPERTYENDELEM;
	static const std::string TYPE_VOID;

	// Properties
	static const std::string REMOTE;
	static const std::string ONEWAY;
	static const std::string SYNCHRONIZED;
	static const std::string SERIALIZE;
	static const std::string GENERATED;
	static const std::string CACHING;
	static const std::string CACHEEXPIRETIME;
	static const std::string NAME;
	static const std::string TYPE;
	static const std::string ORDER;
	static const std::string DIRECTION;
	static const std::string NAMESPACE;
	static const std::string MANDATORY;
	static const std::string OPTIONAL;
	static const std::string HEADER;
	static const std::string ACTION;
	static const std::string REQUEST;
	static const std::string REPLY;
	static const std::string FAULT;
	static const std::string FILTER;
	static const std::string IN;
	static const std::string FORMAT;
	static const std::string PATH;
	static const std::string PRODUCES;
	static const std::string CONSUMES;
	static const std::string CONTENT_TYPE;
	static const std::string PERMISSION;
	static const std::string AUTHENTICATED;
	static const std::string LENGTH;
	static const std::string XSDTYPE;

	static const std::string VAL_TRUE;
	static const std::string VAL_FALSE;
	static const std::string VAL_ASSIGNMENT;
	static const std::string VAL_SEPARATOR;
	static const std::string VAL_INFINITE;
	static const std::string VAL_ATTR;
	static const std::string VAL_ELEM;

	static void writeStdHeader(std::ostream& ostr,
		const std::string& fileName,
		const std::string& libraryName,
		const std::string& packageName,
		const std::string& moduleName,
		const std::string& copyright,
		bool writeTimestamp = true);

	static void writeIncludeGuard(std::ostream& ostr, const std::string& libraryName, const std::string& fileName);
		/// Writes an includeguard: ie. #ifndef xy #define xy

	static void writeInclude(std::ostream& ostr, const std::string& libraryName, const std::string& fileName);
		/// Writes an include.

	static void writeLines(std::ostream& ostr, const std::vector<std::string>& lines, bool appendNewLine);
		/// Writes code lines to the output stream.

	static void writeClassDeclaration(std::ostream& ostr, const std::string& libName, const std::string& className);
		/// Writes a class declaration. Do not use for templates!

	static void writeClassDeclaration(std::ostream& ostr, const std::string& libName, const Poco::CppParser::Struct* pStruct, bool enableAttributes = false);
		/// Writes a class declaration. Works for templates.

	static void writeAccess(std::ostream& ostr, Poco::CppParser::Symbol::Access acc);
		/// Writes string version of the Access.

	static void createFiles(const std::string& fileNameCore, std::ofstream& hFile, std::ofstream& cppFile);
		/// Takes the fileNameCore (name without extension), appends HFILEEXTENSION, and CPPFILEEXTENSION, and opens both files
		/// In case of an error an exception is thrown

	static std::string createIncludeGuard(const std::string& libraryName, const std::string& fileName);
		/// Creates an include guard found at the very beginning of the h file.

	static std::string createInclude(const Poco::CppParser::Struct* pStruct, bool usePocoIncludes);
		/// Creates an include path for the given struct.

	static std::string createInclude(const std::string& filePath, bool flatInclude);
		/// Creates an include path for the given filePath.

	static void handleInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Takes the absolute/relative file path from the struct as input and converts it to an include dir

	static void handleSrcInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Takes the absolute/relative file path from the struct as input and converts it to an include dir

	static void handleBaseClassesInclude(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Includes all files for the parents

	static std::string extractTemplateFromClassDecl(const Poco::CppParser::Struct* pStruct);
		/// Extracts the template name from the class.

	static bool hasAnyRemoteProperty(const Poco::CppParser::Struct* pStruct);
		/// Checks if a class or a single PUBLIC method of it has remote properties.

	static std::string resolveType(const Poco::CppParser::NameSpace* pSym, const std::string& name);
		/// Resolves types by appending namespaces to them

	static std::string resolveTypeImpl(const Poco::CppParser::NameSpace* pSym, const std::string& name);
		/// Resolves types by appending namespaces to them

	static std::string resolveParamDecl(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pSym);
		/// Returns the declaration of a parameter with resolved types.

	static void writeNameSpaceBegin(std::ostream& out, const std::string& nameSpace);
		/// Starts the namespace

	static void writeNameSpaceEnd(std::ostream& out, const std::string& nameSpace);
		/// Ends the namespace

	static void writeDocumentation(std::ostream& out, const std::string& docu, int indent);

	static void writeAttributes(std::ostream& out, const Poco::CppParser::Attributes& attrs, int indent);

private:
	Utility();
	~Utility();
	Utility& operator = (const Utility&);
	Utility(const Utility&);
};


class RecursionGuard
{
public:
	RecursionGuard(int& counter, const Poco::Exception& exc = Poco::RuntimeException("Recursion limit reached"), int limit = 50):
		_c(counter)
	{
		if (++_c >= limit) throw exc;
	}

	~RecursionGuard()
	{
		--_c;
	}

private:
	int& _c;
};


} } // namespace Poco::CodeGeneration


#endif // CodeGeneration_Utility_INCLUDED
