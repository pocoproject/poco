//
// SerializerGenerator.h
//
// Definition of the SerializerGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGeneration_SerializerGenerator_INCLUDED
#define CodeGeneration_SerializerGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "GenUtility.h"
#include <fstream>


class SerializerGenerator: public AbstractGenerator
	/// A SerializerGenerator only cares about member variables and generates a serializer class for them
{
public:
	using VarGet = std::map<std::string, std::pair<Poco::CppParser::Variable*, Poco::CppParser::Function*>>;
	using OrderedVars = std::multimap<Poco::UInt32, VarGet::const_iterator>;

	enum SerializationOrder
	{
		SERIALIZE_LEXICAL = 0,
		SERIALIZE_AS_DECLARED = 1,
	};

	SerializerGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the SerializerGenerator.

	~SerializerGenerator();
		/// Destroys the SerializerGenerator.

	static void setSerializationOrder(SerializationOrder order);
		/// Sets the serialization order for struct/class members.

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

	static void doElemAttrSplit(const VarGet& matches, OrderedVars& attrs, OrderedVars& elems);

	static std::string getVariableName(VarGet::const_iterator it);

	static void appendStaticVarName(const std::string& varName, std::string& result);

	static void matchVarsWithFunctions(const Poco::CppParser::Struct* pDataType, VarGet& matches);

	static void matchVarsWithFunctions(const Poco::CppParser::Struct::Functions& functs, Poco::CppParser::NameSpace::SymbolTable& vars, VarGet& matches);

	static void matchVarsWithFunctionsRecursive(const Poco::CppParser::Struct* pDataType, VarGet& matches);

	static std::string generateTypeSerializerLine(const Poco::CppParser::Struct* pContext, VarGet::const_iterator it, int namePos, const std::string& suffix);

	static bool hasAttributes(const Poco::CppParser::Struct* pClass);
		/// Returns true if the class or any parent has attributes!

	void handleSuperClassCalls(const Poco::CppParser::Struct* pClass, void (SerializerGenerator::*generate)(const Poco::CppParser::Struct*, CodeGenerator&, const std::string&), bool dependsOnAttributes, CodeGenerator& gen);

	static void writeAll(const std::vector<std::string>& codeLines, CodeGenerator& gen);

private:
	void addParameters(Poco::CppParser::Function* pFunc, const std::string& className);

	void addImplParameters(Poco::CppParser::Function* pFunc, const std::string& className);

	static void serializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
		/// Code generator for serializers

	static void serializeImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void serializeImplCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	static void serializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void serializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	static void prepareSerializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	void prepareSerializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix);

	void setDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew);
	static const std::string SERIALIZERIMPLDECL;

	static SerializationOrder _serializationOrder;
};


//
// inlines
//
inline std::string SerializerGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return SerializerGenerator::generateClassName(pStruct);
}


inline std::string SerializerGenerator::getVariableName(VarGet::const_iterator it)
{
	return GenUtility::getVariableName(it->second.first);
}


#endif // CodeGeneration_SerializerGenerator_INCLUDED
