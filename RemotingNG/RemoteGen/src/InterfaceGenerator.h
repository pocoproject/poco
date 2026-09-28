//
// InterfaceGenerator.h
//
// Definition of the InterfaceGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_InterfaceGenerator_INCLUDED
#define RemoteGen_InterfaceGenerator_INCLUDED


#include "AbstractGenerator.h"


class InterfaceGenerator: public AbstractGenerator
	/// InterfaceGenerator generates a remote interface for a given class definition. C++ specific.
	/// Has no members, a default constructor/destructor and only virtual = 0; methods
{
public:
	InterfaceGenerator(Poco::CodeGeneration::CppGenerator& cppGen, bool enableOSP);
		/// Creates the InterfaceGenerator.

	~InterfaceGenerator();
		/// Destroys the InterfaceGenerator.

	[[nodiscard]] static std::string generateClassName(const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	[[nodiscard]] static std::string generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Must not write the namespace

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	[[nodiscard]] std::string newClassName(const Poco::CppParser::Struct* pStruct);
		/// generates the new class name based on the old one

	[[nodiscard]] std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

	[[nodiscard]] bool enableOSP() const;

private:
	static void ifaceTypeIdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, Poco::CodeGeneration::CodeGenerator& gen, void* addParam);

	static void ifaceIsACodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static void ifaceTypeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	bool checkForEventMembers(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);

	[[nodiscard]] std::string resolveEventType(const Poco::CppParser::Struct* pStruct, const std::string& decl);

	bool _enableOSP;
};


//
// inlines
//
inline std::string InterfaceGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return InterfaceGenerator::generateClassName(pStruct);
}


inline bool InterfaceGenerator::enableOSP() const
{
	return _enableOSP;
}


#endif // RemoteGen_InterfaceGenerator_INCLUDED
