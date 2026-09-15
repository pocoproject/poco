//
// SkeletonGenerator.h
//
// Definition of the SkeletonGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_SkeletonGenerator_INCLUDED
#define RemoteGen_SkeletonGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "ProxyGenerator.h"
#include "Poco/CodeGeneration/CppGenerator.h"


class SkeletonGenerator: public AbstractGenerator
	/// SkeletonGenerator generates a proxy for a given class definition. C++ specific. 
{
public:
	SkeletonGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the SkeletonGenerator.

	~SkeletonGenerator();
		/// Destroys the SkeletonGenerator.

	static std::string generateClassName(const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	static std::string generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Must not write the namespace

	void structEnd();

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	std::string newClassName(const Poco::CppParser::Struct* pStruct);
		/// generates the new class name based on the old one

	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

private:
	void checkForEventMembers(const Poco::CppParser::Struct* pStruct);
		/// checks if the class or any parent contains public BasicEvents

	void processRemoteMethods(const Poco::CppParser::Struct* pStruct);

	const Poco::CppParser::Function* getCurrentFct() const;
	bool currentFctHasOneWayProperty() const;

	static void constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void invokeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void staticMembersInitializer(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void writePrepareAttribute(SkeletonGenerator* pGen, const ProxyGenerator::OrderedParameters& attrs, const std::string& indentation, CodeGenerator& gen);
	static void writeTypeSerializer(const Poco::CppParser::Function* pFunc, 
		const ProxyGenerator::OrderedParameters& params, 
		const std::map<std::string, const Poco::CppParser::Parameter*>& outParams, 
		const std::string& indentation, 
		bool isAttr, 
		int funcNsIdx, 
		CodeGenerator& gen);

	static void writePushAttributes(SkeletonGenerator* pGen,
		const Poco::CppParser::Function* pFunc, 
		const ProxyGenerator::OrderedParameters& attrs, 
		const std::map<std::string, const Poco::CppParser::Parameter*>& outParams, 
		const std::string& indentation,
		CodeGenerator& gen);

	static void writeTypeDeserializers(const Poco::CppParser::Function* pFunc, 
		const ProxyGenerator::OrderedParameters& params, 
		const std::string& indentation,
		CodeGenerator& gen);

	typedef std::map<std::string, Poco::CppParser::Struct*> MethodHandlers;

	const Poco::CppParser::Function* _pCurrent;
	bool                       _currentFctOneWayProperty;
	bool                       _isEvent;
	MethodHandlers             _methodHandlers;
	std::set<std::string>      _functions;
};


//
// inlines
//
inline std::string SkeletonGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return SkeletonGenerator::generateClassName(pStruct);
}


inline const Poco::CppParser::Function* SkeletonGenerator::getCurrentFct() const
{
	poco_assert (_pCurrent);
	return _pCurrent;
}


inline bool SkeletonGenerator::currentFctHasOneWayProperty() const
{
	return _currentFctOneWayProperty;
}


#endif // RemoteGen_SkeletonGenerator_INCLUDED
