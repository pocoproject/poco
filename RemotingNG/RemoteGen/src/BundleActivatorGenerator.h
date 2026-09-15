//
// BundleActivatorGenerator.h
//
// Definition of the BundleActivatorGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_BundleActivatorGenerator_INCLUDED
#define RemoteGen_BundleActivatorGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include <vector>
#include <ostream>


class BundleActivatorGenerator: public AbstractGenerator
	/// BundleActivatorGenerator generates a proxy for a given class definition. C++ specific. 
{
public:
	struct BundleService
	{
		const Poco::CppParser::Struct* pStruct;
		bool  clientCode;
		bool  serverCode;
		BundleService(const Poco::CppParser::Struct* pStruct, bool  clientCode, bool  serverCode);
	};

	BundleActivatorGenerator(Poco::CodeGeneration::CppGenerator& cppGen, std::ostream* pImplOut, const std::string& className, const std::vector<BundleService>& services);
		/// Creates the BundleActivatorGenerator.

	~BundleActivatorGenerator();
		/// Destroys the BundleActivatorGenerator.

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Must not write the namespace

	void structEnd();
		/// generates the class

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

	std::string newClassName(const Poco::CppParser::Struct* pStruct);

private:
	static void constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void startCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void stopCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void registerRemoteServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void unregisterRemoteServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void registerRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void unregisterRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void registerServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void unregisterServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void unregisterAllCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void createServicesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void orbCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static void setupRemotingOSPCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void shutdownRemotingOSPCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static void registerListenerCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void unregisterListenerCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static bool requiresClientCode(const std::vector<BundleService>& services);

	static bool requiresServerCode(const std::vector<BundleService>& services);

private:
	std::ostream*              _pImplOut;
	bool                       _clientMode;
	bool                       _serverMode;
	std::string                _className; 
	std::vector<BundleService> _services;
};


//
// inlines
//
inline std::string BundleActivatorGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	throw Poco::NotImplementedException("Should never be called");
}


#endif // RemoteGen_BundleActivatorGenerator_INCLUDED
