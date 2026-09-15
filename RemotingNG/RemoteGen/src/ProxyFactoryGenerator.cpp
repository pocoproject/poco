//
// ProxyFactoryGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProxyFactoryGenerator.h"
#include "ProxyGenerator.h"
#include "InterfaceGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/NumberFormatter.h"


using Poco::CodeGeneration::Utility;


ProxyFactoryGenerator::ProxyFactoryGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


ProxyFactoryGenerator::~ProxyFactoryGenerator()
{
}


std::string ProxyFactoryGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("ProxyFactory");
	return newClassName;
}


std::string ProxyFactoryGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return ProxyFactoryGenerator::generateClassName(pStruct);

	return ns + "::" + ProxyFactoryGenerator::generateClassName(pStruct);
}


void ProxyFactoryGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");

	std::string proxyName = ProxyGenerator::generateQualifiedClassName(nameSpace(), pStruct);
	Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(proxyName);
	if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_STRUCT)
		Poco::CodeGeneration::Utility::handleInclude(static_cast<const Poco::CppParser::Struct*>(pSym), _cppGen);

	//add method: Poco::RemotingNG::Proxy* createProxy() const
	Poco::CppParser::Function* pCreate = new Poco::CppParser::Function("Poco::RemotingNG::Proxy* createProxy", _pStruct);
	pCreate->addDocumentation(	" Creates and returns a new Proxy object.");
	Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", pCreate);
	pCreate->addParameter(pParam1);
	pCreate->makeConst();
}


void ProxyFactoryGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	// ignore the other methods
}


void ProxyFactoryGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("createProxy", &ProxyFactoryGenerator::createCodeGen);
}


std::vector<std::string> ProxyFactoryGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// we extend from Proxy and from the Interface
	std::vector<std::string> bases;
	bases.push_back("Poco::RemotingNG::ProxyFactory");
	return bases;
}


void ProxyFactoryGenerator::createCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	ProxyFactoryGenerator* pGen = dynamic_cast<ProxyFactoryGenerator*>(pAGen);
	poco_check_ptr (pGen);
	std::string fullName (pStruct->fullName());
	size_t pos = fullName.rfind("::");
	std::string ns;
	if (pos != std::string::npos)
		ns = fullName.substr(0, pos);

	std::string proxyName = ProxyGenerator::generateQualifiedClassName(ns, pGen->_pStructIn);
	gen.writeMethodImplementation("return new "+ proxyName + "(oid);");
}
