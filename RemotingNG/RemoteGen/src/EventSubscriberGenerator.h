//
// EventSubscriberGenerator.h
//
// Definition of the EventSubscriberGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_EventSubscriberGenerator_INCLUDED
#define RemoteGen_EventSubscriberGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "ProxyGenerator.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include <map>


class EventSubscriberGenerator: public AbstractGenerator
	/// EventSubscriberGenerator generates a proxy for a given class definition. C++ specific.
{
public:

	EventSubscriberGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the EventSubscriberGenerator.

	~EventSubscriberGenerator() override;
		/// Destroys the EventSubscriberGenerator.

	[[nodiscard]] static std::string generateClassName(const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	[[nodiscard]] static std::string generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct);
		/// Generates for a given class, its interface class name.

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties) override;
		/// Must not write the namespace

	void structEnd() override;

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties) override;
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e) override;

	[[nodiscard]] std::string newClassName(const Poco::CppParser::Struct* pStruct) override;
		/// generates the new class name based on the old one

	[[nodiscard]] std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct) override;

private:
	void checkForEventMembers(const Poco::CppParser::Struct* pStruct, std::vector<const Poco::CppParser::Function*>& eventFunctions, const CodeGenerator::Properties& properties);
		/// checks if the class or any parent contains public BasicEvents

	void checkForEventMembersImpl(const Poco::CppParser::Struct* pStruct, std::vector<const Poco::CppParser::Function*>& eventFunctions, const CodeGenerator::Properties& properties);

	void generateEventFunction(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);

	[[nodiscard]] const Poco::CppParser::Function* getCurrentFct() const;
	[[nodiscard]] bool currentFctHasOneWayProperty() const;

	static void constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void invokeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void mhctorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);

	static void staticMembersInitializer(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
	static void writePrepareAttribute(EventSubscriberGenerator* pGen, const ProxyGenerator::OrderedParameters& attrs, const std::string& indentation, CodeGenerator& gen);
	static void writeTypeSerializer(const Poco::CppParser::Function* pFunc,
		const ProxyGenerator::OrderedParameters& params,
		const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
		const std::string& indentation,
		bool isAttr,
		int funcNsIdx,
		CodeGenerator& gen);

	static void writePushAttributes(EventSubscriberGenerator* pGen,
		const Poco::CppParser::Function* pFunc,
		const ProxyGenerator::OrderedParameters& attrs,
		const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
		const std::string& indentation,
		CodeGenerator& gen);

	static void writeTypeDeserializers(const Poco::CppParser::Function* pFunc,
		const ProxyGenerator::OrderedParameters& params,
		const std::string& indentation,
		CodeGenerator& gen);

private:
	using MethodHandlers = std::map<std::string, Poco::CppParser::Struct*>;
	using FunctionToEvent = std::map<std::string, std::string>;

	const Poco::CppParser::Function* _pCurrent = nullptr;
	bool                       _currentFctOneWayProperty = false;
	bool                       _isEvent = false;
	MethodHandlers             _methodHandlers;
	FunctionToEvent            _events;
};


//
// inlines
//
inline std::string EventSubscriberGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return EventSubscriberGenerator::generateClassName(pStruct);
}


inline const Poco::CppParser::Function* EventSubscriberGenerator::getCurrentFct() const
{
	poco_assert (_pCurrent);
	return _pCurrent;
}


inline bool EventSubscriberGenerator::currentFctHasOneWayProperty() const
{
	return _currentFctOneWayProperty;
}


#endif // RemoteGen_EventSubscriberGenerator_INCLUDED
