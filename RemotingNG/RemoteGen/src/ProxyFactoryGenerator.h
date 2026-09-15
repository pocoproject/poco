//
// ProxyFactoryGenerator.h
//
// Definition of the ProxyFactoryGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_ProxyFactoryGenerator_INCLUDED
#define RemoteGen_ProxyFactoryGenerator_INCLUDED


#include "AbstractGenerator.h"


class ProxyFactoryGenerator: public AbstractGenerator
	/// ProxyFactoryGenerator generates a proxy for a given class definition. C++ specific. 
{
public:
	ProxyFactoryGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the ProxyFactoryGenerator.

	~ProxyFactoryGenerator();
		/// Destroys the ProxyFactoryGenerator.

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
	static void createCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
};


//
// inlines
//
inline std::string ProxyFactoryGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return ProxyFactoryGenerator::generateClassName(pStruct);
}


#endif // RemoteGen_ProxyFactoryGenerator_INCLUDED
