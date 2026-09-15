//
// ClientHelperGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "ClientHelperGenerator.h"
#include "ProxyFactoryGenerator.h"
#include "RemoteObjectGenerator.h"
#include "InterfaceGenerator.h"
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


ClientHelperGenerator::ClientHelperGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


ClientHelperGenerator::~ClientHelperGenerator()
{
}


std::string ClientHelperGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("ClientHelper");
	return newClassName;
}


std::string ClientHelperGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return ClientHelperGenerator::generateClassName(pStruct);

	return ns + "::" + ClientHelperGenerator::generateClassName(pStruct);
}


void ClientHelperGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	pConstr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("~")+_pStruct->name(), _pStruct);
	pDestr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");

	// add a method TestProject::IMyClass::Ptr find(const std::string& uri)
	std::string iName (InterfaceGenerator::generateClassName(pStruct));
	Poco::CppParser::Function* pFind = new Poco::CppParser::Function("static " + iName + "::Ptr find", _pStruct);
	pFind->makeInline();
	pFind->addDocumentation(" Return an interface for the service object identified by the given URI.");
	pFind->addDocumentation("");
	pFind->addDocumentation(" Depending on whether the service object has been registered on the same ORB, or not,");
	pFind->addDocumentation(" the ORB will either return a RemoteObject (with forwards calls locally, without the");
	pFind->addDocumentation(" need for serialization/deserialization), or a Proxy.");
	pFind->addDocumentation("");
	pFind->addDocumentation(" The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>");
	Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
	pFind->addParameter(pParam1);

	pFind = new Poco::CppParser::Function(iName + "::Ptr findImpl", _pStruct);
	pFind->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
	pFind->addParameter(pParam1);

	pFind = new Poco::CppParser::Function("static " + iName + "::Ptr find", _pStruct);
	pFind->makeInline();
	pFind->addDocumentation(" Return a Proxy for the service object identified by the given URI.");
	pFind->addDocumentation("");
	pFind->addDocumentation(" The given protocol name is used to determine the Transport used by");
	pFind->addDocumentation(" the Proxy. This is used for objects identified by URIs that do not");
	pFind->addDocumentation(" follow the standard Remoting URI structure.");
	pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
	Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("const std::string& protocol", 0);
	pFind->addParameter(pParam1);
	pFind->addParameter(pParam2);

	pFind = new Poco::CppParser::Function(iName + "::Ptr findImpl", _pStruct);
	pFind->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
	pParam2= new Poco::CppParser::Parameter("const std::string& protocol", 0);
	pFind->addParameter(pParam1);
	pFind->addParameter(pParam2);

	_cppGen.addIncludeFile("Poco/RemotingNG/Identifiable.h");
	_cppGen.addIncludeFile("Poco/RemotingNG/ORB.h");
	Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable("Poco::RemotingNG::ORB* _pORB", _pStruct);
	pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);

	// the src file will need most includes: for generated RemoteObject+ ProxyFactory, + "Net/DNS.h"
	std::string proxyFactName = ProxyFactoryGenerator::generateQualifiedClassName(nameSpace(), pStruct);
	std::string iFullName (InterfaceGenerator::generateQualifiedClassName(nameSpace(), pStruct));
	Poco::CppParser::NameSpace* pRoot = Poco::CppParser::NameSpace::root();
	const Poco::CppParser::Struct* pPF = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(proxyFactName));
	const Poco::CppParser::Struct* pI = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(iFullName));
	poco_check_ptr (pPF);
	poco_check_ptr (pI);

	Poco::CodeGeneration::Utility::handleInclude(pI, _cppGen);
	Poco::CodeGeneration::Utility::handleSrcInclude(pPF, _cppGen);
	_cppGen.addSrcIncludeFile("Poco/SingletonHolder.h");

	// add a method static ClassNameHelper& instance();
	Poco::CppParser::Function* pInstance = new Poco::CppParser::Function("static "+_pStruct->name()+"& instance", _pStruct);
	pInstance->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
}



void ClientHelperGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
}


void ClientHelperGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &ClientHelperGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("instance", &ClientHelperGenerator::instanceCodeGen);
	e.registerCallback("findImpl", &ClientHelperGenerator::findImplCodeGen);
	e.registerCallback("find", &ClientHelperGenerator::findCodeGen);
	e.registerPreClassCallback(&ClientHelperGenerator::singletonHolder);
}


std::vector<std::string> ClientHelperGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// no base classes for this one
	std::vector<std::string> bases;

	return bases;
}


void ClientHelperGenerator::singletonHolder(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
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


void ClientHelperGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ClientHelperGenerator* pGen = dynamic_cast<ClientHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;
	// register namespace (if necessary)
	Poco::CodeGeneration::CodeGenerator::Properties props;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pStructIn, props);
	std::string ns;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(props, Poco::CodeGeneration::Utility::NAMESPACE, ns);

	gen.writeMethodImplementation("_pORB = &Poco::RemotingNG::ORB::instance();");
	// simply call ORB::instance().registerProxyFactory(className, new ProxyFactory);
	std::string codeLine("_pORB->registerProxyFactory(\"");
	codeLine.append(Poco::replace(pStructIn->fullName(), "::", "."));
	codeLine.append("\", new ");
	codeLine.append(ProxyFactoryGenerator::generateClassName(pStructIn));
	codeLine.append(");");
	gen.writeMethodImplementation(codeLine);
}


void ClientHelperGenerator::instanceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return *sh" + pStruct->name() + ".get();");
}


void ClientHelperGenerator::findCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (pFunc->countParameters() == 1)
		gen.writeMethodImplementation("return " + pStruct->name() + "::instance().findImpl(uri);");
	else
		gen.writeMethodImplementation("return " + pStruct->name() + "::instance().findImpl(uri, protocol);");
}


void ClientHelperGenerator::findImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ClientHelperGenerator* pGen = dynamic_cast<ClientHelperGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;
	std::string iName (InterfaceGenerator::generateClassName(pStructIn));
	if (pFunc->countParameters() == 1)
		gen.writeMethodImplementation("Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);");
	else
	{
		std::string codeLine("Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, ");
		codeLine.append(iName);
		codeLine.append("::remoting__typeId(), ");
		codeLine.append("protocol);");
		gen.writeMethodImplementation(codeLine);
	}
	gen.writeMethodImplementation("return pInterface.cast<" + iName + ">();");
}
