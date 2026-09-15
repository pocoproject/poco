//
// GeneratorEngine.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  GeneratorEngine
//
// Definition of the GeneratorEngine class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGeneration_GeneratorEngine_INCLUDED
#define CodeGeneration_GeneratorEngine_INCLUDED


#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Struct.h"
#include <map>


namespace Poco {
namespace CodeGeneration {


class CodeGenerator;


class CodeGeneration_API GeneratorEngine
	/// A GeneratorEngine iterates over a Struct and calls the correct methods in a CodeGenerator.
{
public:
	typedef void (*MethodGenerator)(const Poco::CppParser::Function*, const Poco::CppParser::Struct*, CodeGenerator&, void* param);
	typedef std::map<std::string, MethodGenerator> Callbacks;

	GeneratorEngine();
		/// Creates the GeneratorEngine.

	~GeneratorEngine();
		/// Destroys the GeneratorEngine.

	void registerDefaultCallback(MethodGenerator funcGen);
		/// Registers a default callback, invoked always when no callback is found for a given methodname

	void unregisterDefaultCallback();
		/// Unregisters the default callback.

	void registerPreClassCallback(MethodGenerator funcGen);
		/// Allows to register a code generator that will write code to the implementation file
		/// before the first constructor is written. Useful to initialize static members.
		/// With templates the code will be written right after namespace_begin.

	void registerPostClassCallback(MethodGenerator funcGen);
		/// Allows to register a code generator that will write code to the implementation file
		/// after the last method was written. Useful to initialize static members.

	void unregisterPreClassCallback();
		/// Unregisters the pre class callback.

	void unregisterPostClassCallback();
		/// Unregisters the post class callback.

	void registerPreNameSpaceCallback(MethodGenerator funcGen);
		/// Allows to register a code generator that will write code to the implementation file
		/// before the namespace is opened.

	void registerPostNameSpaceCallback(MethodGenerator funcGen);
		/// Allows to register a code generator that will write code to the implementation file
		/// after the namespace was closed.

	void unregisterPreNameSpaceCallback();
		/// Unregisters the pre namespace callback.

	void unregisterPostNameSpaceCallback();
		/// Unregisters the post namespace callback.

	void registerCallback(const std::string& methodName, MethodGenerator funcGen);
		/// Registers a callback for a method name iff no other callback is registered yet. Throws an exception in the latter case.

	void unregisterCallback(const std::string& methodName);
		/// Unregisters a callback.

	void generate(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam = 0, bool isFirstClass = true, bool isLastClass = true);
		/// Generates code for the given struct using the given CodeGenerator, addParam can be used to provide a pointer to additional information, required for the
		/// registered callbacks. If isFirstClass is true, the copyright header and the namespace begin is written, if isLastClass is true
		/// the namespace is closed.

	static void handleIncludeFile(const Poco::CppParser::Struct* pStruct, const std::string& classDecl, const std::string& defaultLibrary, CodeGenerator& gen);
		/// Checks first the file in pStruct, then regenerates the include based on the classDecl

	static void handleIncludes(const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Detects and handles all includes.

	static void emptyCodeGen(const Poco::CppParser::Function*, const Poco::CppParser::Struct*, CodeGenerator&, void* param);
		// Empty code generator used for functions.

	static void parseProperties(const Poco::CppParser::Symbol* pSym, CodeGenerator::Properties& props);
		/// Parses and adds properties in lower case to props.

	static void parseElementProperties(const std::string& elemVal, CodeGenerator::Properties& props);
		/// Parses and adds element properties in lower case to props. Element properties are properties defined
		/// for single parameters or variables

	static bool getStringProperty(const CodeGenerator::Properties& props, const std::string& propName, std::string& value);
		/// Returns true if it finds for the given property name the attrValue, writes the found value to parameter value.

	static bool getBoolProperty(const CodeGenerator::Properties& props, const std::string& propName, bool& value);
		/// Returns true if it finds for the given property name the attrValue, writes the found value to parameter value.

	static bool getUInt32Property(const CodeGenerator::Properties& props, const std::string& propName, Poco::UInt32& value);
		/// Returns true if it finds for the given property name the attrValue, writes the found value to parameter value.

private:
	static void parseKeyValue(const std::string& keyValue, CodeGenerator::Properties& props);
		/// Parses a key = value pair

	void handleFunctions(const Poco::CppParser::Struct::Functions& functions, const Poco::CppParser::Struct* pStruct, void* addparam, const CodeGenerator::Properties& classProperties, CodeGenerator& gen);
		/// Iterates over all functions and writes them.

	void handleFunction(const Poco::CppParser::Function* pFunction, const Poco::CppParser::Struct* pStruct, void* addparam, const CodeGenerator::Properties& methodProperties, CodeGenerator& gen);
		/// Generates a single method.

	void handleTypedefs(const Poco::CppParser::NameSpace::SymbolTable& typedefs, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Iterates over all functions and writes them.

	void handleUsings(const Poco::CppParser::NameSpace::SymbolTable& typedefs, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen);
		/// Iterates over all functions and writes them.

	static void handleSystemInclude(const std::string& typeDecl, CodeGenerator& gen);
		/// Handle system includes for a given type.

	static void skipWhiteSpace(const std::string& str, std::size_t& startPos);
	static std::string parseToken(const std::string& str, std::size_t& beginPos);
	static bool isWhiteSpace(const char c);

private:
	Callbacks _callbacks;
	static const std::string WILDCARD;
	static const std::string PRECLASS;
	static const std::string POSTCLASS;
	static const std::string PRENAMESPACE;
	static const std::string POSTNAMESPACE;
};


inline void GeneratorEngine::registerDefaultCallback(MethodGenerator funcGen)
{
	registerCallback(GeneratorEngine::WILDCARD, funcGen);
}


inline void GeneratorEngine::unregisterDefaultCallback()
{
	unregisterCallback(GeneratorEngine::WILDCARD);
}


inline void GeneratorEngine::registerPreClassCallback(MethodGenerator funcGen)
{
	registerCallback(GeneratorEngine::PRECLASS, funcGen);
}


inline void GeneratorEngine::registerPostClassCallback(MethodGenerator funcGen)
{
	registerCallback(GeneratorEngine::POSTCLASS, funcGen);
}


inline void GeneratorEngine::unregisterPreClassCallback()
{
	unregisterCallback(GeneratorEngine::PRECLASS);
}


inline void GeneratorEngine::unregisterPostClassCallback()
{
	unregisterCallback(GeneratorEngine::POSTCLASS);
}


inline void GeneratorEngine::registerPreNameSpaceCallback(MethodGenerator funcGen)
{
	registerCallback(GeneratorEngine::PRENAMESPACE, funcGen);
}


inline void GeneratorEngine::registerPostNameSpaceCallback(MethodGenerator funcGen)
{
	registerCallback(GeneratorEngine::POSTNAMESPACE, funcGen);
}


inline void GeneratorEngine::unregisterPreNameSpaceCallback()
{
	unregisterCallback(GeneratorEngine::PRENAMESPACE);
}


inline void GeneratorEngine::unregisterPostNameSpaceCallback()
{
	unregisterCallback(GeneratorEngine::POSTNAMESPACE);
}


inline void GeneratorEngine::unregisterCallback(const std::string& methodName)
{
	_callbacks.erase(methodName);
}


} } // namespace Poco::CodeGeneration


#endif // CodeGeneration_GeneratorEngine_INCLUDED
