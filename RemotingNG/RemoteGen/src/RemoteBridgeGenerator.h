//
// RemoteBridgeGenerator.h
//
// Definition of the RemoteBridgeGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_RemoteBridgeGenerator_INCLUDED
#define RemoteGen_RemoteBridgeGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include <map>


class RemoteBridgeGenerator: public AbstractGenerator
	/// RemoteBridgeGenerator generates a wrapper extending RemoteObject for a given interface class definition. 
{
public:
	RemoteBridgeGenerator(Poco::CodeGeneration::CppGenerator& cppGen, bool forceSynchronized = true);
		/// Creates the RemoteBridgeGenerator.

	~RemoteBridgeGenerator();
		/// Destroys the RemoteBridgeGenerator.

	static std::string generateClassName(const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	static std::string generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	static std::string generateEventFunctionName(const std::string& eventVarname);

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Must not write the namespace

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	std::string newClassName(const Poco::CppParser::Struct* pStruct);
		/// generates the new class name based on the old one

	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

	bool eventsFound() const;
		/// Returns true if events were found

private:
	static void fwdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void syncFwdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void eventCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void hasEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void enableEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void enableRemoteEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static void constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
		/// Code generator for constructor

	static void destructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
		/// Code generator for destructor

	void checkForEventMembers(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// checks if the class contains public BasicEvents

	void checkForEventMembersImpl(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);

	bool _forceSynchronized;
	std::map<std::string, Poco::CodeGeneration::GeneratorEngine::MethodGenerator> _codeInjectors;
	std::set<std::string> _functions;
	std::vector<std::string> _events;
	bool _hasEvents;
};


//
// inlines
//
inline std::string RemoteBridgeGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return RemoteBridgeGenerator::generateClassName(pStruct);
}


inline bool RemoteBridgeGenerator::eventsFound() const
{
	return _hasEvents;
}


#endif // RemoteGen_RemoteBridgeGenerator_INCLUDED
