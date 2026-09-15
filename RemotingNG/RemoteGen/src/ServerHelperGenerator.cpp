//
// ServerHelperGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ServerHelperGenerator.h"
#include "ProxyFactoryGenerator.h"
#include "RemoteObjectGenerator.h"
#include "SkeletonGenerator.h"
#include "InterfaceGenerator.h"
#include "EventDispatcherGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include <cctype>


using Poco::CodeGeneration::Utility;
using Poco::CodeGeneration::GeneratorEngine;


ServerHelperGenerator::ServerHelperGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


ServerHelperGenerator::~ServerHelperGenerator()
{
}


std::string ServerHelperGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("ServerHelper");
	return newClassName;
}


std::string ServerHelperGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return ServerHelperGenerator::generateClassName(pStruct);

	return ns + "::" + ServerHelperGenerator::generateClassName(pStruct);
}


void ServerHelperGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	std::string roName = RemoteObjectGenerator::generateQualifiedClassName(nameSpace(), pStruct);
	std::string skelName = SkeletonGenerator::generateQualifiedClassName(nameSpace(), pStruct);
	std::string iFullName = InterfaceGenerator::generateQualifiedClassName(nameSpace(), pStruct);
	std::string edFullName = EventDispatcherGenerator::generateQualifiedClassName(nameSpace(), pStruct);

	Poco::CppParser::TypeAlias* pTypeAlias = new Poco::CppParser::TypeAlias("using Service = " + pStruct->fullName(), _pStruct);
	poco_check_ptr (pTypeAlias); // just avoid unused variable warning

	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	pConstr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("~")+_pStruct->name(), _pStruct);
	pDestr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");

	Poco::CppParser::Function* pRegisterSkeleton = new Poco::CppParser::Function("void registerSkeleton", _pStruct);
	pRegisterSkeleton->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	Poco::CppParser::Function* pUnregisterSkeleton = new Poco::CppParser::Function("void unregisterSkeleton", _pStruct);
	pUnregisterSkeleton->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	// add a method void registerObject(MyClass* pObj, const RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
	Poco::CppParser::Function* pRegObj = new Poco::CppParser::Function("static std::string registerObject", _pStruct);
	pRegObj->makeInline();
	pRegObj->addDocumentation(" Creates a RemoteObject wrapper for the given " + pStruct->fullName() + " instance");
	pRegObj->addDocumentation(" and registers it with the ORB and the Listener instance");
	pRegObj->addDocumentation(" uniquely identified by the Listener's ID.");
	pRegObj->addDocumentation(" ");
	pRegObj->addDocumentation("	Returns the URI created for the object.");

	Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::SharedPtr<" + pStruct->fullName() + "> pServiceObject", 0);
	Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", 0);
	Poco::CppParser::Parameter* pParam3= new Poco::CppParser::Parameter("const std::string& listenerId", 0);
	pRegObj->addParameter(pParam1);
	pRegObj->addParameter(pParam2);
	pRegObj->addParameter(pParam3);

	pRegObj = new Poco::CppParser::Function("std::string registerObjectImpl", _pStruct);
	pRegObj->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pParam1 = new Poco::CppParser::Parameter("Poco::AutoPtr<" + roName + "> pRemoteObject", 0);
	pParam3 = new Poco::CppParser::Parameter("const std::string& listenerId", 0);
	pRegObj->addParameter(pParam1);
	pRegObj->addParameter(pParam3);

	pRegObj = new Poco::CppParser::Function("static std::string registerRemoteObject", _pStruct);
	pRegObj->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
	pRegObj->makeInline();
	pRegObj->addDocumentation(" Registers the given RemoteObject with the ORB and the Listener instance");
	pRegObj->addDocumentation(" uniquely identified by the Listener's ID.");
	pRegObj->addDocumentation(" ");
	pRegObj->addDocumentation("	Returns the URI created for the object.");
	pParam1 = new Poco::CppParser::Parameter("Poco::AutoPtr<" + roName + "> pRemoteObject", 0);
	pParam3 = new Poco::CppParser::Parameter("const std::string& listenerId", 0);
	pRegObj->addParameter(pParam1);
	pRegObj->addParameter(pParam3);

	Poco::CppParser::Function* pUnregObj = new Poco::CppParser::Function("static void unregisterObject", _pStruct);
	pUnregObj->makeInline();
	pUnregObj->addDocumentation(" Unregisters a service object identified by URI from the ORB.");
	pParam1 = new Poco::CppParser::Parameter("const std::string& uri", 0);
	pUnregObj->addParameter(pParam1);

	pUnregObj = new Poco::CppParser::Function("void unregisterObjectImpl", _pStruct);
	pUnregObj->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pParam1 = new Poco::CppParser::Parameter("const std::string& uri", 0);
	pUnregObj->addParameter(pParam1);

	Poco::CppParser::Function* pCreateObj = new Poco::CppParser::Function("static Poco::AutoPtr<" + roName + "> createRemoteObject", _pStruct);
	pCreateObj->makeInline();
	pCreateObj->addDocumentation(" Creates and returns a RemoteObject wrapper for the given " + pStruct->fullName() + " instance.");
	pParam1 = new Poco::CppParser::Parameter("Poco::SharedPtr<" + pStruct->fullName() + "> pServiceObject", 0);
	pParam2 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", 0);
	pCreateObj->addParameter(pParam1);
	pCreateObj->addParameter(pParam2);

	pCreateObj = new Poco::CppParser::Function("static Poco::AutoPtr<" + roName + "> createRemoteObjectImpl", _pStruct);
	pCreateObj->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pParam1 = new Poco::CppParser::Parameter("Poco::SharedPtr<" + pStruct->fullName() + "> pServiceObject", 0);
	pParam2 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", 0);
	pCreateObj->addParameter(pParam1);
	pCreateObj->addParameter(pParam2);

	Poco::CppParser::Function* pShutdown = new Poco::CppParser::Function("static void shutdown", _pStruct);
	pShutdown->addDocumentation(" Removes the Skeleton for " + pStruct->fullName() + " from the ORB.");

	bool events = hasEvents(pStruct);
	if (events)
	{
		Poco::CppParser::Function* pEnableEvents = new Poco::CppParser::Function("static void enableEvents", _pStruct);
		pEnableEvents->makeInline();
		pEnableEvents->addDocumentation(" Enables remote events for the RemoteObject identified by the given URI.");
		pEnableEvents->addDocumentation("");
		pEnableEvents->addDocumentation(" Events will be delivered using the Transport for the given protocol.");
		pEnableEvents->addDocumentation(" Can be called multiple times for the same URI with different protocols.");
		pParam1 = new Poco::CppParser::Parameter("const std::string& uri", 0);
		pEnableEvents->addParameter(pParam1);
		pParam2 = new Poco::CppParser::Parameter("const std::string& protocol", 0);
		pEnableEvents->addParameter(pParam2);

		pEnableEvents = new Poco::CppParser::Function("void enableEventsImpl", _pStruct);
		pEnableEvents->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
		pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
		pEnableEvents->addParameter(pParam1);
		pParam2= new Poco::CppParser::Parameter("const std::string& protocol", 0);
		pEnableEvents->addParameter(pParam2);
	}

	Utility::handleInclude(pStruct, _cppGen);
	_cppGen.addIncludeFile("Poco/RemotingNG/Identifiable.h");
	_cppGen.addIncludeFile("Poco/RemotingNG/ORB.h");
	_cppGen.addIncludeFile("Poco/RemotingNG/ServerHelper.h");
	Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable("Poco::RemotingNG::ORB* _pORB", _pStruct);
	pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	// the src file will need most includes: for generated RemoteObject+ ProxyFactory, + "Net/DNS.h"
	Poco::CppParser::NameSpace* pRoot = Poco::CppParser::NameSpace::root();
	const Poco::CppParser::Struct* pRO = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(roName));
	const Poco::CppParser::Struct* pS = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(skelName));
	const Poco::CppParser::Struct* pI = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(iFullName));
	poco_check_ptr (pRO);
	poco_check_ptr (pS);
	poco_check_ptr (pI);

	Poco::CodeGeneration::Utility::handleInclude(pI, _cppGen);
	Poco::CodeGeneration::Utility::handleInclude(pRO, _cppGen);
	Poco::CodeGeneration::Utility::handleSrcInclude(pS, _cppGen);
	_cppGen.addSrcIncludeFile("Poco/SingletonHolder.h");

	if (events)
	{
		const Poco::CppParser::Struct* pED = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(edFullName));
		poco_check_ptr (pED);
		Poco::CodeGeneration::Utility::handleSrcInclude(pED, _cppGen);
	}

	_cppGen.addSrcIncludeFile("Poco/RemotingNG/URIUtility.h");

	// add a method static ClassNameHelper& instance();
	Poco::CppParser::Function* pInstance = new Poco::CppParser::Function("static "+_pStruct->name()+"& instance", _pStruct);
	pInstance->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pInstance->addDocumentation(" Returns a static instance of the helper class.");
}


void ServerHelperGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
}


void ServerHelperGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	e.registerCallback(_pStruct->name(), &ServerHelperGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &ServerHelperGenerator::destructorCodeGen);
	e.registerCallback("registerSkeleton", &ServerHelperGenerator::registerSkeletonCodeGen);
	e.registerCallback("unregisterSkeleton", &ServerHelperGenerator::unregisterSkeletonCodeGen);
	e.registerCallback("registerObjectImpl", &ServerHelperGenerator::registerObjectImplCodeGen);
	e.registerCallback("instance", &ServerHelperGenerator::instanceCodeGen);
	e.registerCallback("registerObject", &ServerHelperGenerator::registerObjectCodeGen);
	e.registerCallback("registerRemoteObject", &ServerHelperGenerator::registerRemoteObjectCodeGen);
	e.registerCallback("unregisterObject", &ServerHelperGenerator::unregisterObjectCodeGen);
	e.registerCallback("unregisterObjectImpl", &ServerHelperGenerator::unregisterObjectImplCodeGen);
	e.registerCallback("createRemoteObject", &ServerHelperGenerator::createRemoteObjectCodeGen);
	e.registerCallback("createRemoteObjectImpl", &ServerHelperGenerator::createRemoteObjectImplCodeGen);
	e.registerCallback("shutdown", &ServerHelperGenerator::shutdownCodeGen);
	e.registerCallback("enableEvents", &ServerHelperGenerator::enableEventsCodeGen);
	e.registerCallback("enableEventsImpl", &ServerHelperGenerator::enableEventsImplCodeGen);
	e.registerPreClassCallback(&ServerHelperGenerator::singletonHolder);
	e.registerPostNameSpaceCallback(&ServerHelperGenerator::helperSpecialization);
}


std::vector<std::string> ServerHelperGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// no base classes for this one
	std::vector<std::string> bases;

	return bases;
}


void ServerHelperGenerator::singletonHolder(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	std::string code("namespace\n");
	code.append("{\n");
	code.append("\tPoco::SingletonHolder<");
	code.append(pStruct->name());
	code.append("> sh");
	code.append(pStruct->name());
	code.append(";\n");
	code.append("}\n\n");
	gen.writeImplementation(code);
}


void ServerHelperGenerator::helperSpecialization(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	std::string nameSpace(pStruct->nameSpace()->fullName());
	std::string clazz(pStruct->name());
	poco_assert (clazz.size() > 12);
	clazz.resize(clazz.size() - 12); // strip "ServerHelper"
	std::string code("REMOTING_SPECIALIZE_SERVER_HELPER(");
	code.append(nameSpace);
	code.append(", ");
	code.append(clazz);
	code.append(")\n\n");
	pGen->_cppGen.writeToHeader(code);
}


void ServerHelperGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("_pORB = &Poco::RemotingNG::ORB::instance();");
	gen.writeMethodImplementation("registerSkeleton();");
}


void ServerHelperGenerator::destructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
}


void ServerHelperGenerator::registerSkeletonCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;

	std::string codeLine("_pORB->registerSkeleton(\"");
	codeLine.append(Poco::replace(pStructIn->fullName(), "::", "."));
	codeLine.append("\", new ");
	codeLine.append(SkeletonGenerator::generateClassName(pStructIn));
	codeLine.append(");");
	gen.writeMethodImplementation(codeLine);
}


void ServerHelperGenerator::unregisterSkeletonCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;

	std::string codeLine("_pORB->unregisterSkeleton(\"");
	codeLine.append(Poco::replace(pStructIn->fullName(), "::", "."));
	codeLine.append("\", true);");
	gen.writeMethodImplementation(codeLine);
}


void ServerHelperGenerator::registerRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return " + pStruct->name() + "::instance().registerObjectImpl(pRemoteObject, listenerId);");
}


void ServerHelperGenerator::registerObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);

	std::string ro("createRemoteObject");
	ro.append("(pServiceObject, oid)");
	gen.writeMethodImplementation("return " + pStruct->name() + "::instance().registerObjectImpl("+ro+", listenerId);");
}


void ServerHelperGenerator::registerObjectImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation("return _pORB->registerObject(pRemoteObject, listenerId);");
}


void ServerHelperGenerator::instanceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return *sh" + pStruct->name() + ".get();");
}


void ServerHelperGenerator::unregisterObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation(pStruct->name() + "::instance().unregisterObjectImpl(uri);");
}


void ServerHelperGenerator::unregisterObjectImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	std::string regLine("_pORB->unregisterObject(uri);");
	gen.writeMethodImplementation(regLine);
}


void ServerHelperGenerator::createRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation("return " + pStruct->name() + "::instance().createRemoteObjectImpl(pServiceObject, oid);");
}


void ServerHelperGenerator::createRemoteObjectImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pStructIn, structProps);
	std::string path;
	GeneratorEngine::getStringProperty(structProps, Utility::PATH, path);

	if (!path.empty())
	{
		std::string code("Poco::AutoPtr<");
		code.append(RemoteObjectGenerator::generateClassName(pStructIn));
		code.append(" > pRemoteObject = new ");
		code.append(RemoteObjectGenerator::generateClassName(pStructIn));
		code.append("(oid, pServiceObject);");
		gen.writeMethodImplementation(code);
		code = "pRemoteObject->remoting__setURI(Poco::URI(\"";
		code.append(path);
		code.append("\"));");
		gen.writeMethodImplementation(code);
		gen.writeMethodImplementation("return pRemoteObject;");
	}
	else
	{
		std::string code("return new ");
		code.append(RemoteObjectGenerator::generateClassName(pStructIn));
		code.append("(oid, pServiceObject);");
		gen.writeMethodImplementation(code);
	}
}


void ServerHelperGenerator::shutdownCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation(pStruct->name() + "::instance().unregisterSkeleton();");
	gen.writeMethodImplementation("sh" + pStruct->name() + ".reset();");
}


void ServerHelperGenerator::enableEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation(pStruct->name() + "::instance().enableEventsImpl(uri, protocol);");
}


void ServerHelperGenerator::enableEventsImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ServerHelperGenerator* pGen = dynamic_cast<ServerHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;

	std::string roType(RemoteObjectGenerator::generateClassName(pStructIn));
	gen.writeMethodImplementation("Poco::RemotingNG::Identifiable::Ptr pIdentifiable = _pORB->findObject(uri);");
	gen.writeMethodImplementation("Poco::AutoPtr<" + roType + "> pRemoteObject = pIdentifiable.cast<" + roType + ">();");
	gen.writeMethodImplementation("if (pRemoteObject)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tpRemoteObject->remoting__enableRemoteEvents(protocol);");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("else throw Poco::NotFoundException(\"remote object\", uri);");
}


bool ServerHelperGenerator::hasEvents(const Poco::CppParser::Struct* pStruct)
{
	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		const std::string& varType = pVar->declType();
		if (pVar->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC)
		{
			if (varType.find("Poco::BasicEvent") == 0 || varType.find("Poco::FIFOEvent") == 0)
			{
				return true;
			}
		}
	}
	return false;
}
