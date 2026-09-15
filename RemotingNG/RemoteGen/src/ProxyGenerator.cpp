//
// ProxyGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "ProxyGenerator.h"
#include "InterfaceGenerator.h"
#include "EventSubscriberGenerator.h"
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
#include "Poco/Format.h"
#include "Poco/Timespan.h"
#include "Poco/RemotingNG/SerializerBase.h"


using namespace Poco::CodeGeneration;


ProxyGenerator::ProxyGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen), _cacheVariableSet(false), _hasEvents(false)
{
}


ProxyGenerator::~ProxyGenerator()
{
}


std::string ProxyGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("Proxy");
	return newClassName;
}


std::string ProxyGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return ProxyGenerator::generateClassName(pStruct);

	return ns + "::" + ProxyGenerator::generateClassName(pStruct);
}


std::string ProxyGenerator::generateOutParamName(const Poco::CppParser::Parameter* pParam, const std::string& functionName)
{
	std::string result("_");
	result.append(functionName);
	result.append(pParam->name());
	result.append("Out");
	return result;
}


std::string ProxyGenerator::generateRetParamName(const Poco::CppParser::Function* pFunc)
{
	if (pFunc->getReturnParameter() == Poco::CodeGeneration::Utility::TYPE_VOID || pFunc->getReturnParameter().empty())
		return std::string();

	std::string result("_");
	result.append(pFunc->name());
	result.append("Ret");
	return result;
}

std::string ProxyGenerator::generateFunctResultName(const Poco::CppParser::Function* pFunc)
{
	std::string result("_");
	result.append(pFunc->name());
	result.append("ResultIsSet");
	return result;
}


void ProxyGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	//const Identifiable::ObjectId& oid
	Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", pConstr);
	pConstr->addParameter(pParam1);

	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("virtual ~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");


	_cppGen.addIncludeFile("Poco/RemotingNG/Proxy.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/URIUtility.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Serializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Deserializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeSerializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeDeserializer.h");
	// add a method virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const
	Poco::CppParser::Function* pTypeId = new Poco::CppParser::Function("virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId", _pStruct);
	pTypeId->makeConst();
	pTypeId->makeInline();

	// add a member: static const std::string DEFAULT_NS;
	Poco::CppParser::Variable* pVar2 = new Poco::CppParser::Variable("static const std::string DEFAULT_NS", _pStruct);
	pVar2->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	Poco::CppParser::TypeAlias* pTypeAlias = new Poco::CppParser::TypeAlias("using Ptr = Poco::AutoPtr<" + generateClassName(pStruct) + ">", _pStruct);
	poco_check_ptr (pTypeAlias); // just avoid unused variable warning

	// replicate parent functions
	handleParentFunctions(pStruct);
	// checks if the class or any parent contains public BasicEvents
	checkForParentEventMembers(pStruct);
	checkForEventMembers(pStruct);

	if (_hasEvents)
	{
		Poco::CppParser::Function* pEvents = new Poco::CppParser::Function("virtual std::string remoting__enableEvents", _pStruct);
		Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter("Poco::RemotingNG::Listener::Ptr pListener", 0);
		pEvents->addParameter(pParam);
		pParam = new Poco::CppParser::Parameter("bool enable = true", 0);
		pEvents->addParameter(pParam);
		Poco::CppParser::Variable* pEvSub = new Poco::CppParser::Variable("Poco::RemotingNG::EventSubscriber::Ptr _pEventSubscriber", _pStruct);
		pEvSub->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
		Poco::CppParser::Variable* pEvLis = new Poco::CppParser::Variable("Poco::RemotingNG::EventListener::Ptr _pEventListener", _pStruct);
		pEvLis->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
		_cppGen.addIncludeFile("Poco/RemotingNG/EventSubscriber.h");
		_cppGen.addIncludeFile("Poco/RemotingNG/EventListener.h");
		_cppGen.addSrcIncludeFile("Poco/RemotingNG/RemotingException.h");
	}

	if (!_cacheVariableSet)
	{
		//check the local functions
		Poco::CppParser::Struct::Functions functions;
		// we only handle public functions, others cannot be called remote
		pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functions);
		Poco::CppParser::Struct::Functions::const_iterator it = functions.begin();
		Poco::CppParser::Struct::Functions::const_iterator itEnd = functions.end();

		for (; it != itEnd && !_cacheVariableSet; ++it)
		{
			Poco::CppParser::Function* pFunc = *it;
			CodeGenerator::Properties methodProperties(properties);
			Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, methodProperties);
			if (GenUtility::isRemoteMethod(pFunc, methodProperties))
			{
				bool enableCachingForThisMethod = false;
				CodeGenerator::Properties::const_iterator itPropCache = methodProperties.find(Utility::CACHING);
				if (itPropCache != methodProperties.end() && itPropCache->second != Utility::VAL_FALSE)
					enableCachingForThisMethod = true;
				_cacheVariableSet |= enableCachingForThisMethod;
			}
		}
	}

	if (_cacheVariableSet)
	{
		_cppGen.addIncludeFile("Poco/UniqueExpireCache.h");
		_cppGen.addIncludeFile("Poco/ExpirationDecorator.h");
		Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable("mutable Poco::UniqueExpireCache<std::string, Poco::ExpirationDecorator<int> > _cache", _pStruct);
		pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	}
}


void ProxyGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& methodProperties)
{
	if (_functions.find(pFuncOld->name()) != _functions.end())
		return;

	if (GenUtility::isRemoteMethod(pFuncOld, methodProperties))
	{
		_functions.insert(pFuncOld->name());

		Poco::CppParser::Function* pFunc = methodClone(pFuncOld, methodProperties);
		methodStartImpl(pFunc, methodProperties);
	}
}


void ProxyGenerator::methodStartImpl(Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& methodProperties)
{
	includeTypeSerializers(pFunc, false, false);
	CodeGenerator::Properties::const_iterator it = methodProperties.find(Utility::CACHEEXPIRETIME);

	bool neverExpires = (it != methodProperties.end() && it->second == Utility::VAL_INFINITE);

	GenUtility::checkFunctionParams(pFunc);
	bool enableCachingForThisMethod = false;
	CodeGenerator::Properties::const_iterator itPropCache = methodProperties.find(Utility::CACHING);
	if (itPropCache != methodProperties.end() && itPropCache->second != Utility::VAL_FALSE)
		enableCachingForThisMethod = true;
	std::map<std::string, const Poco::CppParser::Parameter*> outParams;
	detectOutParams(pFunc, outParams);
	//bool isOneWay = outParams.empty() && (pFunc->getReturnParameter().empty() || pFunc->getReturnParameter() == Poco::CodeGeneration::Utility::TYPE_VOID);
	// if we find any method that has return/out parameters add a place where we can store these results
	if (enableCachingForThisMethod)
	{
		std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itO = outParams.begin();
		std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itOEnd = outParams.end();
		for (; itO != itOEnd; ++itO)
		{
			std::string oName = generateOutParamName(itO->second, pFunc->name());
			std::string decl("mutable " + itO->second->declType());
			if (itO->second->isPointer())
				decl.append("*");
			decl.append(" ");
			decl.append(oName);
			Poco::CppParser::Variable* pVarInt = new Poco::CppParser::Variable(decl, _pStruct);
			pVarInt->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

		}
		if (neverExpires)
		{
			std::string boolName = generateFunctResultName(pFunc);
			std::string decl("mutable bool " + boolName);
			Poco::CppParser::Variable* pVarInt = new Poco::CppParser::Variable(decl, _pStruct);
			pVarInt->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
			_cppGen.registerConstructorHint(pVarInt, Utility::VAL_FALSE);

		}
	}

	std::string retParamName = generateRetParamName(pFunc);
	if (!retParamName.empty())
	{
		Poco::CppParser::Parameter p(pFunc->getReturnParameter() + " " + retParamName, 0);
		std::string decl("mutable " + p.declType());
		if (p.isPointer())
			decl.append("*");
		decl.append(" ");
		decl.append(retParamName);
		Poco::CppParser::Variable* pVarInt = new Poco::CppParser::Variable(decl, _pStruct);
		pVarInt->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	}
	_cacheVariableSet |= enableCachingForThisMethod;
}


void ProxyGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &ProxyGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &ProxyGenerator::destructorCodeGen);
	e.registerCallback("remoting__typeId", &AbstractGenerator::typeIdCodeGen);
	e.registerCallback("remoting__enableEvents", &ProxyGenerator::remotingEventsCodeGen);

	std::vector<std::string>::const_iterator it = _outerEventFunctions.begin();
	for (; it != _outerEventFunctions.end(); ++it)
		e.registerCallback(*it, &ProxyGenerator::eventCodeGen);

	// for all other methods we want the serializing implementation
	e.registerDefaultCallback(&ProxyGenerator::serializeCodeGen);
	e.registerPostClassCallback(&ProxyGenerator::staticMembersInitializer);
}


std::vector<std::string> ProxyGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// we extend from Proxy and from the Interface
	std::vector<std::string> bases;
	bases.push_back(InterfaceGenerator::generateQualifiedClassName(nameSpace(), pStruct));
	bases.push_back("Poco::RemotingNG::Proxy");
	return bases;
}


void ProxyGenerator::serializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	OrderedParameters attrs;
	OrderedParameters elems;
	std::set<std::string> nsSet;
	doElemAttrSplit(pFunc, attrs, elems, nsSet);
	std::map<std::string, int> nsIdx;

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	//static std::string REMOTING__NAMES[] = {"create"s, "protocol"s, "endPoint"s}; // methodName followed by param names, followed by other than default namespaces
	std::string staticIds = ProxyGenerator::generateStaticIdString(pFunc, nsSet, attrs, elems, nsIdx);
	gen.writeMethodImplementation(staticIds);

	std::map<std::string, const Poco::CppParser::Parameter*> outParams;
	detectOutParams(pFunc, outParams);
	Poco::CodeGeneration::CodeGenerator::Properties classProperties;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, classProperties);
	if (classProperties.find(Poco::CodeGeneration::Utility::CACHING) != classProperties.end())
		throw Poco::InvalidArgumentException("cacheResult property is not allowed on class level!");

	Poco::CodeGeneration::CodeGenerator::Properties methodProperties(classProperties);
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, methodProperties);

	bool hasReturnParam = pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID && !pFunc->getReturnParameter().empty();
	bool isOneWay = outParams.empty() && !hasReturnParam && (methodProperties.find(Poco::CodeGeneration::Utility::ONEWAY) != methodProperties.end());

	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itSync = methodProperties.find(Poco::CodeGeneration::Utility::SYNCHRONIZED);
	if (itSync != methodProperties.end() && (itSync->second == Utility::VAL_TRUE || itSync->second.empty() || itSync->second == "all" || itSync->second == "proxy"))
		gen.writeMethodImplementation("Poco::FastMutex::ScopedLock remoting__lock(remoting__mutex());");

	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itCache = methodProperties.find(Poco::CodeGeneration::Utility::CACHING);
	// now check if we have sth cached and if the property is set
	bool useCache = (itCache != methodProperties.end() && itCache->second != Utility::VAL_FALSE);
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itEvent = methodProperties.find("event");
	bool isEvent = (itEvent != methodProperties.end());
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itExp = methodProperties.find(Utility::CACHEEXPIRETIME);
	bool cacheCanExpire = true;
	std::string expireTimeStr;
	if (itExp != methodProperties.end())
	{
		cacheCanExpire = (itExp->second != Utility::VAL_INFINITE);
		expireTimeStr = itExp->second;
	}

	if (useCache && !isOneWay)
		writeCachingBlock(pFunc, gen, cacheCanExpire);

	writeSerializingBlock(pFunc, attrs, elems, nsIdx, gen, isOneWay, isEvent);

	Poco::UInt64 expireTime = 0;
	if (cacheCanExpire && !expireTimeStr.empty())
	{
		expireTime = GenUtility::parseExpireTime(expireTimeStr);
		// we need expireTime in millisecs not micro!
		expireTime /= 1000;
	}
	writeDeserializingBlock(pFunc, attrs, elems, nsIdx, gen, isOneWay, useCache, cacheCanExpire, expireTime, isEvent);
}


void ProxyGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
}


void ProxyGenerator::destructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ProxyGenerator* pProxy = dynamic_cast<ProxyGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	if (!pProxy->_events.empty())
	{
		gen.writeMethodImplementation("if (_pEventListener)");
		gen.writeMethodImplementation("{");
		gen.writeMethodImplementation("\ttry");
		gen.writeMethodImplementation("\t{");
		gen.writeMethodImplementation("\t\t_pEventListener->unsubscribeFromEvents(_pEventSubscriber);");
		gen.writeMethodImplementation("\t}");
		gen.writeMethodImplementation("\tcatch (...)");
		gen.writeMethodImplementation("\t{");
		gen.writeMethodImplementation("\t}");
		gen.writeMethodImplementation("\t_pEventSubscriber.reset();");
		gen.writeMethodImplementation("\t_pEventListener.reset();");
		gen.writeMethodImplementation("}");
	}
}


void ProxyGenerator::staticMembersInitializer(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	std::string code("const std::string ");
	code.append(pStruct->name());
	code.append("::DEFAULT_NS");
	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pStruct, structProps);
	std::string defNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, defNS);
	if (!defNS.empty())
	{
		code.append("(\"");
		code.append(defNS);
		code.append("\")");
	}
	code.append(";");
	gen.writeImplementation(code);
}


std::string ProxyGenerator::generateStaticIdString(const Poco::CppParser::Function* pFunc, const std::set<std::string>& nameSpaces, OrderedParameters& attrs, OrderedParameters& elems, std::map<std::string, int>& nsIndex)
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


void ProxyGenerator::writeCachingBlock(const Poco::CppParser::Function* pFunc, CodeGenerator& gen, bool cacheCanExpire)
{
	static std::string indent("\t");
	if (!hasAnyOutParams(pFunc))
		return;

	std::string prefix("\t");
	if (cacheCanExpire)
	{
		gen.writeMethodImplementation("if (_cache.has(REMOTING__NAMES[0]))");
	}
	else
	{
		gen.writeMethodImplementation("if (" + generateFunctResultName(pFunc) + ")");
	}
	gen.writeMethodImplementation("{");
	// return all outParams, and the ret Param
	std::map<std::string, const Poco::CppParser::Parameter*> outParams;
	detectOutParams(pFunc, outParams);
	std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itParams = outParams.begin();
	std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itParamsEnd = outParams.end();
	for (; itParams != itParamsEnd; ++itParams)
	{
		std::string setLine(itParams->second->name());
		setLine.append("= ");
		setLine.append(generateOutParamName(itParams->second, pFunc->name()));
		setLine.append(";");
		gen.writeMethodImplementation(prefix+setLine);
	}
	std::string returnParam = generateRetParamName(pFunc);

	if (!returnParam.empty())
	{
		gen.writeMethodImplementation(prefix+"return " + returnParam + ";");
	}
	else
		gen.writeMethodImplementation(prefix+"return;");

	gen.writeMethodImplementation("}");
}


void ProxyGenerator::writeSerializingBlock(const Poco::CppParser::Function* pFunc, const OrderedParameters& attrs, const OrderedParameters& elems, const std::map<std::string, int>& nsIdx, CodeGenerator& gen, bool isOneWay, bool isEvent)
{
	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pFunc->nameSpace(), structProps);
	CodeGenerator::Properties funcProps(structProps);
	GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string structDefaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, structDefaultNS);
	std::string funcDefaultNS(structDefaultNS);
	GeneratorEngine::getStringProperty(funcProps, Utility::NAMESPACE, funcDefaultNS);
	std::string action;
	GeneratorEngine::getStringProperty(funcProps, Utility::ACTION, action);
	std::string request;
	GeneratorEngine::getStringProperty(funcProps, Utility::REQUEST, request);
	std::string reply;
	GeneratorEngine::getStringProperty(funcProps, Utility::REPLY, reply);
	std::string fault;
	GeneratorEngine::getStringProperty(funcProps, Utility::FAULT, fault);

	int funcNsIdx = -1;
	if (!funcDefaultNS.empty())
	{
		if (funcDefaultNS == structDefaultNS)
		{
			gen.writeMethodImplementation("const std::string& remoting__namespace(DEFAULT_NS);");
		}
		else
		{
			std::map<std::string, int>::const_iterator itNS = nsIdx.find(funcDefaultNS);
			poco_assert_dbg (itNS != nsIdx.end());
			std::string code("const std::string& remoting__namespace(REMOTING__NAMES[");
			funcNsIdx = itNS->second;
			code.append(Poco::NumberFormatter::format(itNS->second));
			code.append("]);");
			gen.writeMethodImplementation(code);
		}
	}

	gen.writeMethodImplementation("Poco::RemotingNG::Transport& remoting__trans = remoting__transport();");

	if (!funcDefaultNS.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);");
	}
	if (!action.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_ACTION, \"" + action + "\"s);");
	}
	if (!request.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_REQUEST, \"" + request + "\"s);");
	}
	if (!reply.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_REPLY, \"" + reply + "\"s);");
	}
	if (!fault.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_FAULT, \"" + fault + "\"s);");
	}

	std::string structContentType;
	GeneratorEngine::getStringProperty(structProps, Utility::CONSUMES, structContentType);
	std::string funcContentType(structContentType);
	GeneratorEngine::getStringProperty(funcProps, Utility::CONSUMES, funcContentType);
	if (!funcContentType.empty())
	{
		gen.writeMethodImplementation("remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + funcContentType + "\"s);");
	}

	if (isOneWay)
	{
		// invoke oneway
		gen.writeMethodImplementation("Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginMessage(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);");
	}
	else
	{
		gen.writeMethodImplementation("Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);");
	}

	if (!funcDefaultNS.empty())
	{
		gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);");
	}

	std::string structPath;
	GeneratorEngine::getStringProperty(structProps, Utility::PATH, structPath);
	std::string funcPath(structPath);
	GeneratorEngine::getStringProperty(funcProps, Utility::PATH, funcPath);
	if (!funcPath.empty())
	{
		std::string line("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, \"");
		line.append(funcPath);
		line.append("\"s);");
		gen.writeMethodImplementation(line);
	}

	if (!funcContentType.empty())
	{
		std::string line("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONSUMES, \"");
		line.append(funcContentType);
		line.append("\"s);");
		gen.writeMethodImplementation(line);
	}

	// write attrs before serializeRequest
	OrderedParameters::const_iterator itOP = attrs.begin();
	OrderedParameters::const_iterator itOPEnd = attrs.end();
	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "out")
		{
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

	if (!funcContentType.empty())
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONSUMES);");
	}
	if (!funcPath.empty())
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);");
	}
	if (!funcDefaultNS.empty())
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}
}


void ProxyGenerator::writeTypeSerializer(const Poco::CppParser::Function* pFunc, const OrderedParameters& params, bool isAttr, int funcNsIdx, CodeGenerator& gen)
{
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFunc, funcProps);

	OrderedParameters::const_iterator itOP = params.begin();
	OrderedParameters::const_iterator itOPEnd = params.end();
	int lastNS = funcNsIdx;
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

			std::string location;
			std::string format;
			std::string contentType;
			std::string length;
			std::string xsdType;
			std::string propStr;
			GeneratorEngine::getStringProperty(funcProps, "$" + itOP->second.varName, propStr);
			if (!propStr.empty())
			{
				CodeGenerator::Properties paramProps;
				GeneratorEngine::parseElementProperties(propStr, paramProps);
				GeneratorEngine::getStringProperty(paramProps, Utility::IN, location);
				GeneratorEngine::getStringProperty(paramProps, Utility::FORMAT, format);
				GeneratorEngine::getStringProperty(paramProps, Utility::CONTENT_TYPE, contentType);
				GeneratorEngine::getStringProperty(paramProps, Utility::LENGTH, length);
				GeneratorEngine::getStringProperty(paramProps, Utility::XSDTYPE, xsdType);
			}
			if (!location.empty())
			{
				gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
			}
			if (!format.empty())
			{
				gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
			}
			if (!contentType.empty())
			{
				gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
			}
			if (!length.empty())
			{
				gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
			}

			std::string serLine("Poco::RemotingNG::TypeSerializer<");
			std::string type(Poco::CodeGeneration::Utility::resolveType(pFunc->nameSpace(), itOP->second.pParam->declType()));
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(type);

			bool enumMode = false;
			std::string enumBaseType;
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				enumMode = true;
				enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
				if (enumBaseType.empty()) enumBaseType = "int";
				type = enumBaseType;
			}
			serLine.append(type);
			if (itOP->second.pParam->isPointer())
				serLine.append("*");
			serLine.append(">::serialize(REMOTING__NAMES[");

			serLine.append(Poco::NumberFormatter::format(itOP->second.namePos));
			serLine.append("], ");
			if (enumMode)
				serLine.append(Poco::format("static_cast<%s>(%s)", enumBaseType, itOP->second.varName));
			else
				serLine.append(itOP->second.varName);
			serLine.append(", remoting__ser);");
			gen.writeMethodImplementation(serLine);

			if (!location.empty())
			{
				gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
			}
			if (!format.empty())
			{
				gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
			}
			if (!contentType.empty())
			{
				gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
			}
			if (!length.empty())
			{
				gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
			}
		}
	}
	if (!isAttr && lastNS != funcNsIdx)
	{
		gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}
}


void ProxyGenerator::writeDeserializingBlock(const Poco::CppParser::Function* pFunc, const OrderedParameters& attrs, const OrderedParameters& elems, const std::map<std::string, int>& nsIdx, CodeGenerator& gen, bool isOneWay, bool useCache, bool cacheCanExpire, Poco::UInt64 expireTime, bool isEvent)
{
	if (isOneWay)
	{
		gen.writeMethodImplementation("remoting__trans.sendMessage(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);");
	}
	else
	{
		gen.writeMethodImplementation("Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);");
		std::map<std::string, const Poco::CppParser::Parameter*> outParams;
		detectOutParams(pFunc, outParams);

		writePrepareAttribute(attrs, outParams, "", gen);
		CodeGenerator::Properties structProps;
		GeneratorEngine::parseProperties(pFunc->nameSpace(), structProps);
		CodeGenerator::Properties funcProps;
		GeneratorEngine::parseProperties(pFunc, funcProps);

		std::string structDefaultNS;
		GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, structDefaultNS);
		std::string funcDefaultNS(structDefaultNS);
		GeneratorEngine::getStringProperty(funcProps, Utility::NAMESPACE, funcDefaultNS);

		if (!funcDefaultNS.empty())
		{
			if (funcDefaultNS == structDefaultNS)
				gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);");
			else
			{
				std::map<std::string, int>::const_iterator itNS = nsIdx.find(funcDefaultNS);
				poco_assert_dbg (itNS != nsIdx.end());
				std::string code("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
				code.append(Poco::NumberFormatter::format(itNS->second));
				code.append("]);");
				gen.writeMethodImplementation(code);
			}
		}

		std::string structContentType;
		GeneratorEngine::getStringProperty(structProps, Utility::PRODUCES, structContentType);
		std::string funcContentType(structContentType);
		GeneratorEngine::getStringProperty(funcProps, Utility::PRODUCES, funcContentType);
		if (!funcContentType.empty())
		{
			std::string line("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PRODUCES, \"");
			line.append(funcContentType);
			line.append("\"s);");
			gen.writeMethodImplementation(line);
		}

		OrderedParameters::const_iterator itElem = elems.begin();
		OrderedParameters::const_iterator itElemEnd = elems.end();
		for (; itElem != itElemEnd; ++itElem)
		{
			if (itElem->second.direction != "in")
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
		}

		std::string name(GenUtility::getMethodName(pFunc));
		std::string responseName(GenUtility::getReplyMethodName(pFunc));
		std::string messageType(isEvent ? "EVENT" : "REPLY");
		if (name != responseName)
		{
			gen.writeMethodImplementation("static const std::string REMOTING__REPLY_NAME(\"" + responseName + "\");");
			gen.writeMethodImplementation("remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}

		writeTypeDeserializers(pFunc, attrs, outParams, useCache, "", gen);

		std::string retParamName(generateRetParamName(pFunc));
		bool hasReturnParam = !retParamName.empty();
		writeDeserializeReturnParam(pFunc, gen);

		writeTypeDeserializers(pFunc, elems, outParams, useCache, "", gen);
		if (name != responseName)
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation("remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}

		// notify the cache about the update
		if (useCache)
		{
			if (cacheCanExpire)
			{
				std::string cacheAdd("_cache.add(REMOTING__NAMES[0], Poco::ExpirationDecorator<int>(0, ");

				// set 1 billion as limit 1.000.000.000
				// yes, we could split it up in 32bits values but then we have to be careful with appending u at the end of the constants,
				// so we don't
				if (expireTime > 1000000000)
				{
					// we can't write an Int64 constant in a platform independent way
					// compose it of 32bits
					cacheAdd.append("((Poco::Timespan::TimeDiff)");
					std::vector <Poco::UInt32> fragments;
					Poco::Timespan::TimeDiff val(expireTime);
					while (val > 1000000000)
					{
						fragments.push_back(static_cast<Poco::UInt32>(val%1000000000));
						val /= 1000000000;
					}
					//write val, even if it is zero
					std::string expireString = Poco::NumberFormatter::format(val);
					cacheAdd.append(expireString+")"); // close the cast bracket
					poco_assert (!fragments.empty());
					std::vector <Poco::UInt32>::const_iterator it = fragments.end();

					do
					{
						--it;
						expireString = Poco::NumberFormatter::format(*it);
						cacheAdd.append("*");
						cacheAdd.append(expireString);
					}
					while (it != fragments.begin());
					cacheAdd.append("));");
				}
				else
				{
					std::string expireString = Poco::NumberFormatter::format(expireTime);
					cacheAdd.append(expireString);
					cacheAdd.append("));");
				}
				gen.writeMethodImplementation(cacheAdd);
			}
			else
				gen.writeMethodImplementation(generateFunctResultName(pFunc)+ " = true;");
		}

		if (!funcContentType.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PRODUCES);");
		}
		if (!funcDefaultNS.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
		}
		gen.writeMethodImplementation("remoting__trans.endRequest();");
		if (hasReturnParam)
		{
			gen.writeMethodImplementation(Utility::RETURN + " " + retParamName + ";");
		}
	}
}


void ProxyGenerator::writeTypeDeserializers(const Poco::CppParser::Function* pFunc,
											const OrderedParameters& params,
											const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
											bool useCache,
											const std::string& indentation,
											CodeGenerator& gen)
{
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFunc, funcProps);

	OrderedParameters::const_iterator itOP = params.begin();
	OrderedParameters::const_iterator itOPEnd = params.end();

	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "in")
		{
			poco_check_ptr (itOP->second.pParam);
			const Poco::CppParser::Parameter* pParam = itOP->second.pParam;
			std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itD = outParams.find(pParam->name());
			if (itD != outParams.end())
			{
				std::string location;
				std::string format;
				std::string contentType;
				std::string length;
				std::string xsdType;
				std::string propStr;
				GeneratorEngine::getStringProperty(funcProps, "$" + itOP->second.varName, propStr);
				if (!propStr.empty())
				{
					CodeGenerator::Properties paramProps;
					GeneratorEngine::parseElementProperties(propStr, paramProps);
					GeneratorEngine::getStringProperty(paramProps, Utility::IN, location);
					GeneratorEngine::getStringProperty(paramProps, Utility::FORMAT, format);
					GeneratorEngine::getStringProperty(paramProps, Utility::CONTENT_TYPE, contentType);
					GeneratorEngine::getStringProperty(paramProps, Utility::LENGTH, length);
					GeneratorEngine::getStringProperty(paramProps, Utility::XSDTYPE, xsdType);
				}
				if (!location.empty())
				{
					gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
				}
				if (!format.empty())
				{
					gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
				}
				if (!contentType.empty())
				{
					gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
				}
				if (!length.empty())
				{
					gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
				}
				if (!xsdType.empty())
				{
					gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
				}

				std::string retType(Poco::CodeGeneration::Utility::resolveType(pFunc->nameSpace(), itD->second->declType()));
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
				std::string codeLine("Poco::RemotingNG::TypeDeserializer<");
				if (enumMode)
					codeLine.append(enumBaseType);
				else
					codeLine.append(retType);
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
					codeLine.append("remoting__" + itOP->second.varName + "Tmp");
				}
				else
				{
					codeLine.append(itOP->second.varName);
				}
				codeLine.append(");");
				gen.writeMethodImplementation(indentation+codeLine);
				// assign the result
				if (enumMode)
				{
					gen.writeMethodImplementation(indentation + itOP->second.varName + " = static_cast<" + retType + ">(remoting__" + itOP->second.varName + "Tmp);");
				}
				if (useCache)
				{
					std::string cacheName (generateOutParamName(itD->second, pFunc->name()));
					gen.writeMethodImplementation(indentation+cacheName + "= "+ itD->second->name() + ";");
				}
				if (!location.empty())
				{
					gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
				}
				if (!format.empty())
				{
					gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
				}
				if (!contentType.empty())
				{
					gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
				}
				if (!length.empty())
				{
					gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
				}
				if (!xsdType.empty())
				{
					gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
				}
			}
		}
	}
}


void ProxyGenerator::writeDeserializeReturnParam(const Poco::CppParser::Function* pFunc, CodeGenerator& gen)
{
	std::string retParamName(generateRetParamName(pFunc));
	bool hasReturnParam = !retParamName.empty();

	// always get the return param first
	if (hasReturnParam)
	{
		CodeGenerator::Properties funcProps;
		GeneratorEngine::parseProperties(pFunc, funcProps);

		std::string format;
		std::string location;
		std::string contentType;
		std::string length;
		std::string xsdType;
		std::string propStr;
		GeneratorEngine::getStringProperty(funcProps, GenUtility::ATTR_RETURN, propStr);
		if (!propStr.empty())
		{
			CodeGenerator::Properties paramProps;
			GeneratorEngine::parseElementProperties(propStr, paramProps);
			GeneratorEngine::getStringProperty(paramProps, Utility::IN, location);
			GeneratorEngine::getStringProperty(paramProps, Utility::FORMAT, format);
			GeneratorEngine::getStringProperty(paramProps, Utility::CONTENT_TYPE, contentType);
			GeneratorEngine::getStringProperty(paramProps, Utility::LENGTH, length);
			GeneratorEngine::getStringProperty(paramProps, Utility::XSDTYPE, xsdType);
		}
		if (!location.empty())
		{
			gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
		}
		if (!format.empty())
		{
			gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
		}
		if (!contentType.empty())
		{
			gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
		}
		if (!length.empty())
		{
			gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
		}
		if (!xsdType.empty())
		{
			gen.writeMethodImplementation("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
		}

		std::string retParamType(GenUtility::getResolvedReturnParameterType(pFunc->nameSpace(), pFunc));
		Poco::CppParser::Parameter retParam(pFunc->getReturnParameter() + " dummy", 0);
		bool needsClear = (GenUtility::isVectorType(pFunc->nameSpace(), &retParam)
			|| GenUtility::isNullableType(pFunc->nameSpace(), &retParam)
			|| GenUtility::isOptionalType(pFunc->nameSpace(), &retParam))
			&& !GenUtility::isArrayType(pFunc->nameSpace(), &retParam)
			&& !GenUtility::isStdOptionalType(pFunc->nameSpace(), &retParam);
		bool needsReset = GenUtility::isStdOptionalType(pFunc->nameSpace(), &retParam);

		if (needsClear)
		{
			gen.writeMethodImplementation(retParamName+".clear();");
		}
		else if (needsReset)
		{
			gen.writeMethodImplementation(retParamName+".reset();");
		}
		std::string retName (GenUtility::getReturnParameterName(pFunc));
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
		if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
		{
			enumMode = true;
			enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
			if (enumBaseType.empty()) enumBaseType = "int";
		}
		else
		{
			retParamType = retParam.declType();
		}
		std::string line("Poco::RemotingNG::TypeDeserializer<");
		if (enumMode)
			line.append(enumBaseType);
		else
			line.append(retParamType);
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
		{
			gen.writeMethodImplementation("int remoting__tmpReturn;");
			line.append("remoting__tmpReturn);");
			gen.writeMethodImplementation(line);
			gen.writeMethodImplementation(retParamName + " = static_cast<" + retParamType + ">(remoting__tmpReturn);");
		}
		else
		{
			line.append(retParamName +");");
			gen.writeMethodImplementation(line);
		}
		if (!location.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
		}
		if (!format.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
		}
		if (!contentType.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
		}
		if (!length.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
		}
		if (!xsdType.empty())
		{
			gen.writeMethodImplementation("remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
		}
	}
}


void ProxyGenerator::writePrepareAttribute(const OrderedParameters& attrs, const std::map<std::string, const Poco::CppParser::Parameter*>& outParams, const std::string& indentation, CodeGenerator& gen)
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


void ProxyGenerator::detectOutParams(const Poco::CppParser::Function* pFunc, std::map<std::string, const Poco::CppParser::Parameter*>& outParams)
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


bool ProxyGenerator::hasAnyOutParams(const Poco::CppParser::Function* pFunc)
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
	return pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID && !pFunc->getReturnParameter().empty();
}


void ProxyGenerator::doElemAttrSplit(const Poco::CppParser::Function* pFunc, OrderedParameters& attrs, OrderedParameters& elems, std::set<std::string>& nameSpaces)
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
			GeneratorEngine::parseElementProperties("$" + itProp->second, paramProps);

			GeneratorEngine::getStringProperty(paramProps, Utility::NAME, aName);
			GeneratorEngine::getStringProperty(paramProps, Utility::NAMESPACE, aNS);
			mandatory = GenUtility::getIsMandatory(paramProps);
			GeneratorEngine::getStringProperty(paramProps, Utility::DIRECTION, direction);
			if (!(direction.empty() || direction == "in" || direction == "out" || direction == "inout"))
				throw Poco::InvalidArgumentException("direction must be one of \"in\", \"out\" or \"inout\"");
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


void ProxyGenerator::checkForEventMembers(const Poco::CppParser::Struct* pStruct)
{
	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		const std::string& varType = pVar->declType();
		if (pVar->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC && !(pVar->flags() & Poco::CppParser::Variable::VAR_STATIC))
		{
			if (varType.find("Poco::BasicEvent") == 0 || varType.find("Poco::FIFOEvent") == 0)
			{
				_events.push_back(pVar->name());
			}
		}

	}

	if (_hasEvents)
	{
		Poco::Path file (Poco::CodeGeneration::Utility::createInclude(_pStruct, true));

		std::string newFileName = EventSubscriberGenerator::generateClassName(_pStructIn);
		file.setBaseName(newFileName);
		std::string inclFile = file.toString(Poco::Path::PATH_UNIX);
		_cppGen.addSrcIncludeFile(inclFile);
	}
}


void ProxyGenerator::checkForParentEventMembers(const Poco::CppParser::Struct* pStruct)
{
	checkForParentEventMembersImpl(pStruct);

	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent && Utility::hasAnyRemoteProperty(pParent))
		{
			checkForParentEventMembers(pParent);
		}
	}
}


void ProxyGenerator::checkForParentEventMembersImpl(const Poco::CppParser::Struct* pStruct)
{
	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		const std::string& varType = pVar->declType();
		if (pVar->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC && !(pVar->flags() & Poco::CppParser::Variable::VAR_STATIC))
		{
			if (varType.find("Poco::BasicEvent") == 0 || varType.find("Poco::FIFOEvent") == 0)
			{
				_hasEvents = true;
			}
		}
	}
}


std::string ProxyGenerator::generateEventFunctionName(const std::string& eventVarname)
{
	return "event__" + Poco::trim(eventVarname);
}


void ProxyGenerator::eventCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ProxyGenerator* pProxy = dynamic_cast<ProxyGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	// only forward to the network if the network is not the sender
	gen.writeMethodImplementation ("if (pSender) " + pFunc->name() + "Impl(var);");
}


void ProxyGenerator::remotingEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ProxyGenerator* pProxy = dynamic_cast<ProxyGenerator*>(pAGen);
	poco_check_ptr (pProxy);
	const Poco::CppParser::Struct* pStructIn = pProxy->_pStructIn;
	poco_check_ptr (pStructIn);

	gen.writeMethodImplementation("std::string subscriberURI;");
	gen.writeMethodImplementation("if ((_pEventListener && !enable) || (!_pEventListener && enable))");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tPoco::RemotingNG::EventListener::Ptr pEventListener = pListener.cast<Poco::RemotingNG::EventListener>();");
	gen.writeMethodImplementation("\tif (pEventListener)");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tif (enable)");
	gen.writeMethodImplementation("\t\t{");
	gen.writeMethodImplementation("\t\t\tstd::string eventURI = remoting__getEventURI().empty() ? remoting__getURI().toString() : remoting__getEventURI().toString();");
	std::string className = EventSubscriberGenerator::generateClassName(pStructIn);
	gen.writeMethodImplementation("\t\t\t_pEventSubscriber = new " + className + "(eventURI, this);");
	gen.writeMethodImplementation("\t\t\tsubscriberURI = pEventListener->subscribeToEvents(_pEventSubscriber);");
	gen.writeMethodImplementation("\t\t\t_pEventListener = pEventListener;");
	gen.writeMethodImplementation("\t\t}");
	gen.writeMethodImplementation("\t\telse if (_pEventListener == pEventListener)");
	gen.writeMethodImplementation("\t\t{");
	gen.writeMethodImplementation("\t\t\ttry");
	gen.writeMethodImplementation("\t\t\t{");
	gen.writeMethodImplementation("\t\t\t\t_pEventListener->unsubscribeFromEvents(_pEventSubscriber);");
	gen.writeMethodImplementation("\t\t\t}");
	gen.writeMethodImplementation("\t\t\tcatch (...)");
	gen.writeMethodImplementation("\t\t\t{");
	gen.writeMethodImplementation("\t\t\t\t_pEventSubscriber.reset();");
	gen.writeMethodImplementation("\t\t\t\t_pEventListener.reset();");
	gen.writeMethodImplementation("\t\t\t\tthrow;");
	gen.writeMethodImplementation("\t\t\t}");
	gen.writeMethodImplementation("\t\t\t_pEventSubscriber.reset();");
	gen.writeMethodImplementation("\t\t\t_pEventListener.reset();");
	gen.writeMethodImplementation("\t\t}");
	gen.writeMethodImplementation("\t\telse throw Poco::RemotingNG::RemotingException(\"EventListener mismatch\");");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("\telse throw Poco::RemotingNG::RemotingException(\"Listener is not an EventListener\");");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("return subscriberURI;");
}
