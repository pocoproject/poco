//
// EventDispatcherGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EventDispatcherGenerator.h"
#include "RemoteObjectGenerator.h"
#include "RemoteBridgeGenerator.h"
#include "InterfaceGenerator.h"
#include "SerializerGenerator.h"
#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Timespan.h"
#include "Poco/Format.h"
#include "Poco/RemotingNG/SerializerBase.h"


using namespace Poco::CodeGeneration;


EventDispatcherGenerator::EventDispatcherGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


EventDispatcherGenerator::~EventDispatcherGenerator()
{
}


std::string EventDispatcherGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("EventDispatcher");
	return newClassName;
}


std::string EventDispatcherGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return EventDispatcherGenerator::generateClassName(pStruct);

	return ns + "::" + EventDispatcherGenerator::generateClassName(pStruct);
}


std::string EventDispatcherGenerator::generateOutParamName(const Poco::CppParser::Parameter* pParam, const std::string& functionName)
{
	std::string result("_");
	result.append(functionName);
	result.append(pParam->name());
	result.append("Out");
	return result;
}


std::string EventDispatcherGenerator::generateRetParamName(const Poco::CppParser::Function* pFunc)
{
	if (pFunc->getReturnParameter() == Utility::TYPE_VOID || pFunc->getReturnParameter().empty())
		return std::string();

	std::string result("_");
	result.append(pFunc->name());
	result.append("Ret");
	return result;
}


void EventDispatcherGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	//takes asinput the concrete RemoteObject pointer where we will register for the event

	Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter(InterfaceGenerator::generateClassName(pStruct) + "* pInterface", pConstr);
	pConstr->addParameter(pParam1);
	Poco::CppParser::Parameter* pParam2 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& objectId", pConstr);
	pConstr->addParameter(pParam2);
	Poco::CppParser::Parameter* pParam3 = new Poco::CppParser::Parameter("const std::string& protocol", pConstr);
	pConstr->addParameter(pParam3);

	Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable(InterfaceGenerator::generateClassName(pStruct) + "* _pInterface", _pStruct);
	pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	_cppGen.registerConstructorHint(pVar, pParam1);

	Poco::CppParser::Variable* pVar2 = new Poco::CppParser::Variable("Poco::RemotingNG::Identifiable::ObjectId _objectId", _pStruct);
	pVar2->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	_cppGen.registerConstructorHint(pVar2, pParam2);

	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("virtual ~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");
	// include as fwd decl the concrete RemoteObject
	_cppGen.addIncludeFile("Poco/RemotingNG/EventDispatcher.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/URIUtility.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Serializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Deserializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeSerializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeDeserializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/RemotingException.h");
	// add a method virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const
	Poco::CppParser::Function* pTypeId = new Poco::CppParser::Function("virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId", _pStruct);
	pTypeId->makeConst();
	pTypeId->makeInline();

	// add a member: static const std::string DEFAULT_NS;
	Poco::CppParser::Variable* pVar3 = new Poco::CppParser::Variable("static const std::string DEFAULT_NS", _pStruct);
	pVar3->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	// checks if the class or any parent contains public BasicEvents
	checkForEventMembers(pStruct, properties);
}


void EventDispatcherGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& methodProperties)
{
	// we don't handle the regular methods here, only events
}


void EventDispatcherGenerator::methodStartImpl(Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& methodProperties)
{
	includeTypeSerializers(pFunc, false, false);
}


void EventDispatcherGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &EventDispatcherGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &EventDispatcherGenerator::destructorCodeGen);
	e.registerCallback("remoting__typeId", &AbstractGenerator::typeIdCodeGen);

	std::vector<std::string>::const_iterator it = _outerEventFunctions.begin();
	for (; it != _outerEventFunctions.end(); ++it)
		e.registerCallback(*it, &EventDispatcherGenerator::eventCodeGen);

	// for all other methods we want the serializing implementation
	e.registerDefaultCallback(&EventDispatcherGenerator::serializeCodeGen);
	e.registerPostClassCallback(&EventDispatcherGenerator::staticMembersInitializer);
}


std::vector<std::string> EventDispatcherGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;
	bases.push_back("Poco::RemotingNG::EventDispatcher");
	return bases;
}


void EventDispatcherGenerator::serializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	OrderedParameters attrs;
	OrderedParameters elems;
	std::set<std::string> nsSet;
	doElemAttrSplit(pFunc, attrs, elems, nsSet);
	std::map<std::string, int> nsIdx;
	//static std::string REMOTING__NAMES[] = {"create", "protocol", "endPoint"}; // methodName followed by param names, followed by other than default namespaces
	std::string staticIds = EventDispatcherGenerator::generateStaticIdString(pFunc, nsSet, attrs, elems, nsIdx);
	gen.writeMethodImplementation(staticIds);

	std::map<std::string, const Poco::CppParser::Parameter*> outParams;
	detectOutParams(pFunc, outParams);
	Poco::CodeGeneration::CodeGenerator::Properties classProperties;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, classProperties);

	Poco::CodeGeneration::CodeGenerator::Properties methodProperties(classProperties);
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, methodProperties);

	bool hasReturnParam = pFunc->getReturnParameter() != Utility::TYPE_VOID && !pFunc->getReturnParameter().empty();
	bool isOneWay = (outParams.empty() && !hasReturnParam) || (methodProperties.find(Utility::ONEWAY) != methodProperties.end());

	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itEvent = methodProperties.find("event");
	bool isEvent = (itEvent != methodProperties.end());

	writeSerializingBlock(pFunc, attrs, elems, nsIdx, gen, isOneWay, isEvent);
	writeDeserializingBlock(pFunc, attrs, elems, gen, isOneWay, isEvent);
}


void EventDispatcherGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	EventDispatcherGenerator* pProxy = dynamic_cast<EventDispatcherGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	const Poco::CppParser::Struct* pDataType = pProxy->_pStructIn; // returns the data type for which pStruct was generated
	const std::vector<std::string>& events = pProxy->_events;
	std::vector<std::string>::const_iterator it = events.begin();
	for (; it != events.end(); ++it)
	{
		std::string line("_pInterface->" + *it);
		line.append(" += ");
		line.append("Poco::delegate(this, &");
		line.append(EventDispatcherGenerator::generateClassName(pDataType));
		line.append("::");
		line.append(EventDispatcherGenerator::generateEventFunctionName(*it));
		line.append(");");
		gen.writeMethodImplementation(line);
	}
}


void EventDispatcherGenerator::destructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	EventDispatcherGenerator* pProxy = dynamic_cast<EventDispatcherGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	gen.writeMethodImplementation("try");
	gen.writeMethodImplementation("{");
	const Poco::CppParser::Struct* pDataType = pProxy->_pStructIn; // returns the data type for which pStruct was generated
	const std::vector<std::string>& events = pProxy->_events;
	std::vector<std::string>::const_iterator it = events.begin();
	for (; it != events.end(); ++it)
	{
		std::string line("\t_pInterface->" + *it);
		line.append(" -= ");
		line.append("Poco::delegate(this, &");
		line.append(EventDispatcherGenerator::generateClassName(pDataType));
		line.append("::");
		line.append(EventDispatcherGenerator::generateEventFunctionName(*it));
		line.append(");");
		gen.writeMethodImplementation(line);
	}
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("catch (...)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tpoco_unexpected();");
	gen.writeMethodImplementation("}");
}


void EventDispatcherGenerator::staticMembersInitializer(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	std::string code("const std::string ");
	code.append(pStruct->name());
	code.append("::DEFAULT_NS(\"");
	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pStruct, structProps);
	std::string defNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, defNS);
	code.append(defNS);
	code.append("\");");
	gen.writeImplementation(code);
}


std::string EventDispatcherGenerator::generateStaticIdString(const Poco::CppParser::Function* pFunc, const std::set<std::string>& nameSpaces, OrderedParameters& attrs, OrderedParameters& elems, std::map<std::string, int>& nsIndex)
{
	std::set<std::string>::const_iterator itNS = nameSpaces.begin();
	std::set<std::string>::const_iterator itNSEnd = nameSpaces.end();
	nsIndex.clear();
	int firstNSIdx = static_cast<int>(attrs.size()+elems.size()+1);
	for (; itNS != itNSEnd; ++itNS, ++firstNSIdx)
	{
		nsIndex.insert(std::make_pair(*itNS, firstNSIdx));
	}
	std::string staticIds("static const std::string REMOTING__NAMES[] = {\"");
	std::string aName(GenUtility::getMethodName(pFunc));
	staticIds.append(aName);
	staticIds.append("\"s,");

	OrderedParameters::iterator it = attrs.begin();
	OrderedParameters::iterator itEnd = attrs.end();

	int namePos = 1;
	for (; it != itEnd; ++it, ++namePos)
	{
		SerializerGenerator::appendStaticVarName(it->second.name, staticIds);
		/// set nameId and nameSpaceId
		std::map<std::string, int>::const_iterator itIdx = nsIndex.find(it->second.nameSpace);
		if (itIdx != nsIndex.end())
		{
			it->second.nameSpacePos = itIdx->second;
		}
		else
			it->second.nameSpacePos = -1;
		it->second.namePos = namePos;
	}
	it = elems.begin();
	itEnd = elems.end();
	for (; it != itEnd; ++it, ++namePos)
	{
		SerializerGenerator::appendStaticVarName(it->second.name, staticIds);
		/// set nameId and nameSpaceId
		std::map<std::string, int>::const_iterator itIdx = nsIndex.find(it->second.nameSpace);
		if (itIdx != nsIndex.end())
		{
			it->second.nameSpacePos = itIdx->second;
		}
		else
			it->second.nameSpacePos = -1;
		it->second.namePos = namePos;
	}

	// add namespaces
	itNS = nameSpaces.begin();
	itNSEnd = nameSpaces.end();
	for (; itNS != itNSEnd; ++itNS)
	{
		SerializerGenerator::appendStaticVarName(*itNS, staticIds);
	}
	if (staticIds[staticIds.size() -1] == ',')
	{
		staticIds[staticIds.size() -1] = '}';
		staticIds.append(";");
	}
	else
		staticIds.append("};");

	return staticIds;
}


void EventDispatcherGenerator::writeSerializingBlock(const Poco::CppParser::Function* pFunc, const OrderedParameters& attrs, const OrderedParameters& elems, const std::map<std::string, int>& nsIdx, CodeGenerator& gen, bool isOneWay, bool isEvent)
{
	gen.writeMethodImplementation("Poco::RemotingNG::Transport& remoting__trans = transportForSubscriber(subscriberURI);");
	gen.writeMethodImplementation("Poco::ScopedLock<Poco::RemotingNG::Transport> remoting__lock(remoting__trans);");
	if (isOneWay)
	{
		// invoke oneway
		gen.writeMethodImplementation("Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginMessage(_objectId, remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);");
	}
	else
	{
		gen.writeMethodImplementation("Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(_objectId, remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);");
	}

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pFunc->nameSpace(), structProps);
	CodeGenerator::Properties funcProps(structProps);
	GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string structDefaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, structDefaultNS);
	std::string funcDefaultNS(structDefaultNS);
	GeneratorEngine::getStringProperty(funcProps, Utility::NAMESPACE, funcDefaultNS);
	int funcNsIdx = -1;
	if (!funcDefaultNS.empty())
	{
		if (funcDefaultNS == structDefaultNS)
			gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);");
		else
		{
			std::map<std::string, int>::const_iterator itNS = nsIdx.find(funcDefaultNS);
			poco_assert_dbg (itNS != nsIdx.end());
			std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
			funcNsIdx = itNS->second;
			code.append(Poco::NumberFormatter::format(itNS->second));
			code.append("]);");
			gen.writeMethodImplementation(code);
		}
	}

	// write attrs before serializeRequest
	OrderedParameters::const_iterator itOP = attrs.begin();
	OrderedParameters::const_iterator itOPEnd = attrs.end();
	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "out")
		{
			//ser.pushAttribute(ns, name);
			std::string code("remoting__ser.pushAttribute(");
			if (itOP->second.nameSpacePos < 0)
			{
				code.append("DEFAULT_NS, ");
			}
			else
			{
				code.append("REMOTING__NAMES[");
				code.append(Poco::NumberFormatter::format(itOP->second.nameSpacePos));
				code.append("], ");
			}
			code.append("REMOTING__NAMES[");
			code.append(Poco::NumberFormatter::format(itOP->second.namePos));
			code.append("]);");
			gen.writeMethodImplementation(code);
		}
	}

	OrderedParameters::const_iterator itElem = elems.begin();
	OrderedParameters::const_iterator itElemEnd = elems.end();
	for (; itElem != itElemEnd; ++itElem)
	{
		if (itElem->second.direction != "out")
		{
			std::string elemStr;
			GeneratorEngine::getStringProperty(funcProps, "$" + itElem->second.varName, elemStr);
			if (!elemStr.empty())
			{
				CodeGenerator::Properties elemProps;
				GeneratorEngine::parseElementProperties(elemStr, elemProps);
				bool header = false;
				GeneratorEngine::getBoolProperty(elemProps, GenUtility::ATTR_HEADER, header);

				if (header)
				{
					std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, ");
					code.append("REMOTING__NAMES[");
					code.append(Poco::NumberFormatter::format(itElem->second.namePos));
					code.append("]);");
					gen.writeMethodImplementation(code);
				}
			}
		}
	}

	std::string messageType(isEvent ? "EVENT" : "REQUEST");
	gen.writeMethodImplementation("remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");

	writeTypeSerializer(pFunc, attrs, true, funcNsIdx, gen);
	writeTypeSerializer(pFunc, elems, false, funcNsIdx, gen);

	gen.writeMethodImplementation("remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");

	if (!funcDefaultNS.empty())
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}
}


void EventDispatcherGenerator::writeTypeSerializer(const Poco::CppParser::Function* pFunc, const OrderedParameters& params, bool isAttr, int funcNsIdx, CodeGenerator& gen)
{
	OrderedParameters::const_iterator itOP = params.begin();
	OrderedParameters::const_iterator itOPEnd = params.end();
	int lastNS = funcNsIdx;
	// do not write the first param!
	if (itOP != itOPEnd)
		++itOP;
	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "out")
		{
			poco_check_ptr (itOP->second.pParam);
			if (!isAttr && lastNS != itOP->second.nameSpacePos)
			{
				if (lastNS != funcNsIdx)
				{
					gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
				}
				std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
				code.append(Poco::NumberFormatter::format(itOP->second.nameSpacePos));
				code.append("]);");
				gen.writeMethodImplementation(code);
				lastNS = itOP->second.nameSpacePos;
			}


			const Poco::CppParser::Parameter* pParam = itOP->second.pParam;
			std::string type(Utility::resolveType(pFunc->nameSpace(), pParam->declType()));
			std::string serLine("Poco::RemotingNG::TypeSerializer<");
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(type);
			bool enumMode = false;
			std::string enumBaseType;
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
				if (enumBaseType.empty()) enumBaseType = "int";
				enumMode = true;
				type = enumBaseType;
			}
			serLine.append(type);
			if (itOP->second.pParam->isPointer())
				serLine.append("*");
			serLine.append(">::serialize(REMOTING__NAMES[");

			serLine.append(Poco::NumberFormatter::format(itOP->second.namePos));
			serLine.append("], ");
			if (enumMode)
			{
				serLine.append("static_cast<");
				serLine.append(enumBaseType);
				serLine.append(">(");
				serLine.append(itOP->second.varName);
				serLine.append(")");
			}
			else
			{
				serLine.append(itOP->second.varName);
			}
			serLine.append(", remoting__ser);");
			gen.writeMethodImplementation(serLine);
		}
	}
	if (!isAttr && lastNS != funcNsIdx)
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}
}


void EventDispatcherGenerator::writeDeserializingBlock(const Poco::CppParser::Function* pFunc, const OrderedParameters& attrs, const OrderedParameters& elems,CodeGenerator& gen, bool isOneWay, bool isEvent)
{
	if (isOneWay)
	{
		gen.writeMethodImplementation("remoting__trans.sendMessage(_objectId, remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);");
	}
	else
	{
		gen.writeMethodImplementation("Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(_objectId, remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);");
		std::map<std::string, const Poco::CppParser::Parameter*> outParams;
		detectOutParams(pFunc, outParams);

		writePrepareAttribute(attrs, outParams, "", gen);
		CodeGenerator::Properties funcProps;
		GeneratorEngine::parseProperties(pFunc, funcProps);

		OrderedParameters::const_iterator itElem = elems.begin();
		OrderedParameters::const_iterator itElemEnd = elems.end();
		for (; itElem != itElemEnd; ++itElem)
		{
			std::string elemStr;
			GeneratorEngine::getStringProperty(funcProps, "$" + itElem->second.varName, elemStr);
			if (!elemStr.empty())
			{
				CodeGenerator::Properties elemProps;
				GeneratorEngine::parseElementProperties(elemStr, elemProps);
				bool header = false;
				GeneratorEngine::getBoolProperty(elemProps, GenUtility::ATTR_HEADER, header);

				if (header)
				{
					std::string code("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, ");
					code.append("REMOTING__NAMES[");
					code.append(Poco::NumberFormatter::format(itElem->second.namePos));
					code.append("]);");
					gen.writeMethodImplementation(code);
				}
			}
		}

		std::string name(GenUtility::getMethodName(pFunc));
		std::string responseName(GenUtility::getReplyMethodName(pFunc));
		std::string messageType(isEvent ? "EVENT_REPLY" : "REPLY");
		if (name != responseName)
		{
			gen.writeMethodImplementation("static const std::string REMOTING__REPLY_NAME(\"" + responseName + "\");");
			gen.writeMethodImplementation("remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}

		writeTypeDeserializers(pFunc, attrs, outParams, "", gen);

		std::string retParamName(generateRetParamName(pFunc));
		bool hasReturnParam = !retParamName.empty();
		writeDeserializeReturnParam(pFunc, gen);

		writeTypeDeserializers(pFunc, elems, outParams, "", gen);
		if (name != responseName)
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}

		gen.writeMethodImplementation("remoting__trans.endRequest();");

		if (hasReturnParam)
		{
			gen.writeMethodImplementation(Utility::RETURN + " " + retParamName + ";");
		}
	}
}


void EventDispatcherGenerator::writeTypeDeserializers(const Poco::CppParser::Function* pFunc,
											const OrderedParameters& params,
											const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
											const std::string& indentation,
											CodeGenerator& gen)
{
	std::string deserPre("Poco::RemotingNG::TypeDeserializer<");
	OrderedParameters::const_iterator itOP = params.begin();
	OrderedParameters::const_iterator itOPEnd = params.end();

	for (; itOP != itOPEnd; ++itOP)
	{
		poco_check_ptr (itOP->second.pParam);
		const Poco::CppParser::Parameter* pParam = itOP->second.pParam;
		std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itD = outParams.find(pParam->name());
		if (itD != outParams.end())
		{
			std::string retType(Utility::resolveType(pFunc->nameSpace(), itD->second->declType()));
			bool isOutParam = (itD->second->isReference()||itD->second->isPointer()) && !itD->second->isConst();
			poco_assert (isOutParam);
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(retType);
			bool enumMode = false;
			std::string enumBaseType;
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				enumMode = true;
				enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
				if (enumBaseType.empty()) enumBaseType = "int";
			}
			std::string cntStr(Poco::NumberFormatter::format(itOP->second.namePos));

			std::string codeLine(deserPre);
			if (enumMode)
				codeLine += enumBaseType;
			else
				codeLine += retType;
			codeLine.append(">::deserialize(REMOTING__NAMES[");
			codeLine.append(cntStr);
			codeLine.append("], ");
			if (itOP->second.mandatory)
				codeLine.append("true, ");
			else
				codeLine.append("false, ");
			codeLine.append("remoting__deser, " );
			if (enumMode)
			{
				gen.writeMethodImplementation(Poco::format("%s%s remoting__%sTmp;", indentation, enumBaseType, itOP->second.varName));
				codeLine.append("remoting__");
				codeLine.append(itOP->second.varName);
				codeLine.append("Tmp");
			}
			else
			{
				codeLine.append(itOP->second.varName);
			}
			codeLine.append(");");
			gen.writeMethodImplementation(indentation+codeLine);
			if (enumMode)
			{
				gen.writeMethodImplementation(Poco::format("%s%s = static_cast<%s>(remoting__%sTmp);", indentation, itOP->second.varName, retType, itOP->second.varName));
			}
		}
	}
}


void EventDispatcherGenerator::writeDeserializeReturnParam(const Poco::CppParser::Function* pFunc, CodeGenerator& gen)
{
	std::string retParamName(generateRetParamName(pFunc));
	bool hasReturnParam = !retParamName.empty();

	// always get the return param first
	if (hasReturnParam)
	{
		std::string retParamType(GenUtility::getResolvedReturnParameterType(pFunc->nameSpace(), pFunc));
		Poco::CppParser::Parameter retParam(pFunc->getReturnParameter() + " dummy", 0);
		bool needsClear = (GenUtility::isVectorType(pFunc->nameSpace(), &retParam)
			|| GenUtility::isNullableType(pFunc->nameSpace(), &retParam)
			|| GenUtility::isOptionalType(pFunc->nameSpace(), &retParam))
			&& !GenUtility::isArrayType(pFunc->nameSpace(), &retParam);

		if (needsClear)
		{
			gen.writeMethodImplementation(retParamName+".clear();");
		}
		std::string retName(GenUtility::getReturnParameterName(pFunc));
		if (retName != Poco::RemotingNG::SerializerBase::RETURN_PARAM)
		{
			// static const std::string REMOTING__RETURN_PARAM_NAME("$retName")
			std::string line("static const std::string REMOTING__RETURN_PARAM_NAME(\"");
			line.append(retName);
			line.append("\");");
			gen.writeMethodImplementation(line);
		}
		Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(retParamType);
		bool enumMode = false;
		std::string enumBaseType;
		std::string deserType;
		if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
		{
			enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
			if (enumBaseType.empty()) enumBaseType = "int";
			enumMode = true;
			deserType = enumBaseType;
		}
		else
		{
			retParamType = retParam.declType();
			deserType = retParamType;
		}
		if (enumMode)
		{
			gen.writeMethodImplementation(Poco::format("%s remoting__%sTmp;", enumBaseType, retParamName));
		}
		std::string line("Poco::RemotingNG::TypeDeserializer<" + deserType);
		if (retParam.isPointer())
			line.append("*");
		bool doInline = GenUtility::getInlineReturnParam(pFunc);
		if (doInline)
			line.append(">::deserializeImpl(");
		else
			line.append(">::deserialize(");
		if (retName == Poco::RemotingNG::SerializerBase::RETURN_PARAM)
			line.append("Poco::RemotingNG::SerializerBase::RETURN_PARAM");
		else
			line.append("REMOTING__RETURN_PARAM_NAME");
		line.append(", true, remoting__deser, ");
		if (enumMode)
			line.append(Poco::format("remoting__%sTmp", retParamName));
		else
			line.append(retParamName);
		line.append(");");
		gen.writeMethodImplementation(line);
		if (enumMode)
		{
			gen.writeMethodImplementation(Poco::format("%s = static_cast<%s>(remoting__%sTmp);", retParamName, retParamType, retParamName));
		}
	}
}


void EventDispatcherGenerator::writePrepareAttribute(const OrderedParameters& attrs, const std::map<std::string, const Poco::CppParser::Parameter*>& outParams, const std::string& indentation, CodeGenerator& gen)
{
	OrderedParameters::const_iterator itAttrs = attrs.begin();
	OrderedParameters::const_iterator itAttrsEnd = attrs.end();
	for (; itAttrs != itAttrsEnd; ++itAttrs)
	{
		if (outParams.find(itAttrs->second.pParam->name()) != outParams.end())
		{
			//deser.pushAttribute(ns, name, bool)
			std::string code("remoting__deser.pushAttribute(");
			if (itAttrs->second.nameSpacePos < 0)
			{
				code.append("DEFAULT_NS, ");
			}
			else
			{
				code.append("REMOTING__NAMES[");
				code.append(Poco::NumberFormatter::format(itAttrs->second.nameSpacePos));
				code.append("], ");
			}

			code.append("REMOTING__NAMES[");
			code.append(Poco::NumberFormatter::format(itAttrs->second.namePos));
			code.append("], ");

			if (itAttrs->second.mandatory)
				code.append("true);");
			else
				code.append("false);");

			gen.writeMethodImplementation(indentation+code);
		}
	}
}


void EventDispatcherGenerator::detectOutParams(const Poco::CppParser::Function* pFunc, std::map<std::string, const Poco::CppParser::Parameter*>& outParams)
{
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		// add all non-const refs, add all pointers
		Poco::CppParser::Parameter* pParam = *it;
		if ( (pParam->isReference()||pParam->isPointer()) && !pParam->isConst())
		{
			outParams.insert(std::make_pair(pParam->name(), pParam));
		}
	}
}


bool EventDispatcherGenerator::hasAnyOutParams(const Poco::CppParser::Function* pFunc)
{
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		// add all non-const refs, add all pointers
		Poco::CppParser::Parameter* pParam = *it;
		if ( (pParam->isReference()||pParam->isPointer()) && !pParam->isConst())
		{
			return true;
		}
	}

	return pFunc->getReturnParameter() != Utility::TYPE_VOID && !pFunc->getReturnParameter().empty();
}


void EventDispatcherGenerator::doElemAttrSplit(const Poco::CppParser::Function* pFunc, OrderedParameters& attrs, OrderedParameters& elems, std::set<std::string>& nameSpaces)
{
	nameSpaces.clear();
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pFunc->nameSpace(), structProps);
	CodeGenerator::Properties funcProps(structProps);
	GeneratorEngine::parseProperties(pFunc, funcProps);
	Poco::UInt32 paramCnt = 0;
	const Poco::UInt32 unknownOrderOffset = 0xffff0000;
	std::string structDefaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, structDefaultNS);
	std::string funcDefaultNS(structDefaultNS);
	GeneratorEngine::getStringProperty(funcProps, Utility::NAMESPACE, funcDefaultNS);

	for (; it != itEnd; ++it, ++paramCnt)
	{
		Poco::UInt32 order = unknownOrderOffset + paramCnt;
		std::string aName((*it)->name());
		std::string aNS(funcDefaultNS);
		bool mandatory = true;
		std::string direction;
		CodeGenerator::Properties::iterator itProp = funcProps.find("$" + aName);

		if (itProp != funcProps.end())
		{
			CodeGenerator::Properties paramProps; // we need type and order
			GeneratorEngine::parseElementProperties(itProp->second, paramProps);

			GeneratorEngine::getStringProperty(paramProps, Utility::NAME, aName);
			GeneratorEngine::getStringProperty(paramProps, Utility::NAMESPACE, aNS);
			mandatory = GenUtility::getIsMandatory(paramProps);
			GeneratorEngine::getStringProperty(paramProps, Utility::DIRECTION, direction);
			if (!aNS.empty() && aNS != structDefaultNS)
				nameSpaces.insert(aNS);

			std::string type;
			bool hasType = GeneratorEngine::getStringProperty(paramProps, Utility::TYPE, type);
			bool isAttr = false; // the default is elem
			if (hasType && type == Utility::VAL_ATTR)
				isAttr = true;

			GeneratorEngine::getUInt32Property(paramProps, Utility::ORDER, order);
			if (isAttr)
				attrs.insert(std::make_pair(order, ExtParam((*it)->name(), aName, aNS, *it, mandatory, direction)));
			else
				elems.insert(std::make_pair(order, ExtParam((*it)->name(), aName, aNS, *it, mandatory, direction)));
		}
		else
		{
			elems.insert(std::make_pair(order, ExtParam((*it)->name(), aName, aNS, *it, mandatory, direction)));
		}
	}
}


void EventDispatcherGenerator::checkForEventMembers(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	checkForEventMembersImpl(pStruct, properties);

	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent && Utility::hasAnyRemoteProperty(pParent))
		{
			Poco::CodeGeneration::CodeGenerator::Properties parentProperties;
			Poco::CodeGeneration::GeneratorEngine::parseProperties(pParent, parentProperties);
			checkForEventMembers(pParent, parentProperties);
		}
	}
}


void EventDispatcherGenerator::checkForEventMembersImpl(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		Poco::CodeGeneration::CodeGenerator::Properties eventProperties(properties);
		Poco::CodeGeneration::GeneratorEngine::parseProperties(pVar, eventProperties);
		if (GenUtility::isRemoteEvent(pVar, eventProperties))
		{
			_events.push_back(pVar->name());
			_cppGen.addSrcIncludeFile("Poco/Delegate.h");
			// generate a serializer method for that member too
			// call methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& methodProperties)
			// convert the basicevent to a private function
			std::string funcDecl("void ");
			std::string fctName = generateEventFunctionName(pVar->name());
			funcDecl.append(fctName);
			std::vector<std::string> templTypes = GenUtility::getResolvedInnerTemplateTypes(pStruct, pVar->declType());
			if (templTypes.size() != 1)
				throw Poco::InvalidArgumentException("Illegal remote event param: " + pVar->fullName());
			std::string paramDecl = templTypes[0];
			if (paramDecl != "void")
			{
				paramDecl.append("& data");
			}
			{
				Poco::CppParser::Function* pFunc = new Poco::CppParser::Function(funcDecl, _pStruct);

				Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter("const void* pSender", 0);
				pFunc->addParameter(pParam);

				if (paramDecl != "void")
				{
					Poco::CppParser::Parameter* pParam2 = new Poco::CppParser::Parameter(paramDecl, 0);
					pFunc->addParameter(pParam2);
				}
				_outerEventFunctions.push_back(fctName);

				if (pVar->getAttributes().has("oneway"))
				{
					Poco::CppParser::Attributes funcAttr = pFunc->getAttributes();
					funcAttr.set("oneway", pVar->getAttributes().getString("oneway"));
					pFunc->setAttributes(funcAttr);
				}

				if (pVar->getAttributes().has(Utility::FILTER))
				{
					Poco::CppParser::Attributes funcAttr = pFunc->getAttributes();
					funcAttr.set(Utility::FILTER, pVar->getAttributes().getString(Utility::FILTER));
					pFunc->setAttributes(funcAttr);
				}
			}
			funcDecl.append("Impl");
			{
				Poco::CppParser::Function* pFunc = new Poco::CppParser::Function(funcDecl, _pStruct);
				Poco::CppParser::Parameter* pParam0 = new Poco::CppParser::Parameter("const std::string& subscriberURI", 0);
				pFunc->addParameter(pParam0);
				if (templTypes[0] != "void")
				{
					Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter(paramDecl, 0);
					pFunc->addParameter(pParam1);
				}
				CodeGenerator::Properties methodProperties;
				Poco::CppParser::Attributes funcAttr;
				const Poco::CppParser::Attributes& varAttr = pVar->getAttributes();
				// we have one single parameter, this one defines the name of the function!
				funcAttr.set("inline", "true");
				funcAttr.set("event", "true");
				methodProperties.insert(std::make_pair("inline", "true"));
				methodProperties.insert(std::make_pair("event", "true"));
				if (varAttr.has("name"))
				{
					funcAttr.set("name", varAttr.getString("name"));
					methodProperties.insert(std::make_pair("name", varAttr.getString("name")));
				}
				else
				{
					funcAttr.set("name", pVar->name());
					methodProperties.insert(std::make_pair("name", pVar->name()));
				}
				if (varAttr.has("oneway"))
				{
					std::string oneVal = varAttr.getString("oneway");
					funcAttr.set("oneway", oneVal);
					methodProperties.insert(std::make_pair("oneway", oneVal));
				}
				pFunc->setAttributes(funcAttr);
				pFunc->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
				methodStartImpl(pFunc, methodProperties);
				methodEnd(pFunc, methodProperties);
			}
		}
	}

	if (!_events.empty())
	{
		Poco::Path file (Utility::createInclude(_pStruct, true));

		std::string newFileName = InterfaceGenerator::generateClassName(_pStructIn);
		file.setBaseName(newFileName);
		std::string inclFile = file.toString(Poco::Path::PATH_UNIX);
		_cppGen.addIncludeFile(inclFile);
	}
}


std::string EventDispatcherGenerator::generateEventFunctionName(const std::string& eventVarname)
{
	return "event__" + Poco::trim(eventVarname);
}


void EventDispatcherGenerator::eventCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	EventDispatcherGenerator* pProxy = dynamic_cast<EventDispatcherGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	bool hasFilter = pFunc->getAttributes().getBool(Utility::FILTER, false);
	if (hasFilter)
	{
		std::string nameLine("static const std::string REMOTING__EVENT_NAME(\"");
		nameLine.append(pFunc->name().substr(7)); // strip out "event__" prefix
		nameLine.append("\");");
		gen.writeMethodImplementation(nameLine);
	}

	// only forward to the network if the network is not the sender
	gen.writeMethodImplementation("if (pSender)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tPoco::Clock now;");
	gen.writeMethodImplementation("\tPoco::FastMutex::ScopedLock lock(_mutex);");
	gen.writeMethodImplementation("\tSubscriberMap::iterator it = _subscribers.begin();");
	gen.writeMethodImplementation("\twhile (it != _subscribers.end())");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tif (it->second->expireTime != 0 && it->second->expireTime < now)");
	gen.writeMethodImplementation("\t\t{");
	gen.writeMethodImplementation("\t\t\tSubscriberMap::iterator itDel(it++);");
	gen.writeMethodImplementation("\t\t\t_subscribers.erase(itDel);");
	gen.writeMethodImplementation("\t\t}");
	gen.writeMethodImplementation("\t\telse");
	gen.writeMethodImplementation("\t\t{");
	if (pFunc->getAttributes().has("oneway"))
	{
		gen.writeMethodImplementation("\t\t\ttry");
		gen.writeMethodImplementation("\t\t\t{");
		if (pFunc->countParameters() == 1) // void event
		{
			gen.writeMethodImplementation("\t\t\t\t" + pFunc->name() + "Impl(it->first);");
		}
		else
		{
			if (hasFilter)
			{
				gen.writeMethodImplementation("\t\t\t\tif (accept(it->second->filters, REMOTING__EVENT_NAME, data))");
				gen.writeMethodImplementation("\t\t\t\t{");
				gen.writeMethodImplementation("\t\t\t\t\t" + pFunc->name() + "Impl(it->first, data);");
				gen.writeMethodImplementation("\t\t\t\t}");
			}
			else
			{
				gen.writeMethodImplementation("\t\t\t\t" + pFunc->name() + "Impl(it->first, data);");
			}
		}
		gen.writeMethodImplementation("\t\t\t}");
		gen.writeMethodImplementation("\t\t\tcatch (Poco::Exception&)");
		gen.writeMethodImplementation("\t\t\t{");
		gen.writeMethodImplementation("\t\t\t}");
	}
	else
	{
		gen.writeMethodImplementation("\t\t\ttry");
		gen.writeMethodImplementation("\t\t\t{");
		if (pFunc->countParameters() == 1) // void event
		{
			gen.writeMethodImplementation("\t\t\t\t" + pFunc->name() + "Impl(it->first);");
		}
		else
		{
			if (hasFilter)
			{
				gen.writeMethodImplementation("\t\t\t\tif (accept(it->second->filters, REMOTING__EVENT_NAME, data))");
				gen.writeMethodImplementation("\t\t\t\t{");
				gen.writeMethodImplementation("\t\t\t\t\t" + pFunc->name() + "Impl(it->first, data);");
				gen.writeMethodImplementation("\t\t\t\t}");
			}
			else
			{
				gen.writeMethodImplementation("\t\t\t\t" + pFunc->name() + "Impl(it->first, data);");
			}
		}
		gen.writeMethodImplementation("\t\t\t}");
		gen.writeMethodImplementation("\t\t\tcatch (Poco::RemotingNG::RemoteException&)");
		gen.writeMethodImplementation("\t\t\t{");
		gen.writeMethodImplementation("\t\t\t\tthrow;");
		gen.writeMethodImplementation("\t\t\t}");
		gen.writeMethodImplementation("\t\t\tcatch (Poco::Exception&)");
		gen.writeMethodImplementation("\t\t\t{");
		gen.writeMethodImplementation("\t\t\t}");
	}
	gen.writeMethodImplementation("\t\t\t++it;");
	gen.writeMethodImplementation("\t\t}");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("}");
}
