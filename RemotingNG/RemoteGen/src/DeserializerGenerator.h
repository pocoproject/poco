//
// DeserializerGenerator.h
//
// Definition of the DeserializerGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef CodeGeneration_DeserializerGenerator_INCLUDED
#define CodeGeneration_DeserializerGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "SerializerGenerator.h"
#include <fstream>
#include <vector>


class DeserializerGenerator: public AbstractGenerator
	/// A DeserializerGenerator only cares about member variables and generates a serializer class for them
{
public:
	DeserializerGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the DeserializerGenerator.

	~DeserializerGenerator();
		/// Destroys the DeserializerGenerator.

	static std::string generateClassName(const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	static std::string generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Must not write the namespace

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	std::string newClassName(const Poco::CppParser::Struct* pStruct);
		/// generates the new class name based on the old one

	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

private:
	void addParameters(Poco::CppParser::Function* pFunc, const std::string& className);

	void addImplParameters(Poco::CppParser::Function* pFunc, const std::string& className);

	static void deserializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
		/// Code generator for deserializers

	static void deserializeImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void deserializeImplCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	static void prepareDeserializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void prepareDeserializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	static void deserializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void deserializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	void setDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew);

	void handleSuperClassCalls(const Poco::CppParser::Struct* pClass, void (DeserializerGenerator::*generate)(const Poco::CppParser::Struct*, CodeGenerator&, const std::string&), bool dependsOnAttributes, CodeGenerator& gen);

	static void matchVarsWithFunctions(const Poco::CppParser::Struct* pDataType, SerializerGenerator::VarGet& matches);

	static void matchVarsWithFunctions(const Poco::CppParser::Struct::Functions& functs, Poco::CppParser::NameSpace::SymbolTable& vars, SerializerGenerator::VarGet& matches);

	static void matchVarsWithFunctionsRecursive(const Poco::CppParser::Struct* pDataType, SerializerGenerator::VarGet& matches);

	static bool hasAttributes(const Poco::CppParser::Struct* pClass);
		/// Returns true if the class or any parent has attributes!

	static void generateTypeDeserializerLines(const Poco::CppParser::Struct* pContext, SerializerGenerator::VarGet::const_iterator it, int namePos, const std::string& suffix, std::vector<std::string>& lines, int& retUsageCount);
};


//
// inlines
//
inline std::string DeserializerGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return DeserializerGenerator::generateClassName(pStruct);
}


#endif // CodeGeneration_DeserializerGenerator_INCLUDED
