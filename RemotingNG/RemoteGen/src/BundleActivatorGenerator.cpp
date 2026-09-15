//
// BundleActivatorGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "BundleActivatorGenerator.h"
#include "ProxyFactoryGenerator.h"
#include "RemoteObjectGenerator.h"
#include "InterfaceGenerator.h"
#include "SkeletonGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/NumberFormatter.h"
#include <cctype>


using Poco::CodeGeneration::Utility;
using Poco::CodeGeneration::CppGenerator;


BundleActivatorGenerator::BundleActivatorGenerator(
		Poco::CodeGeneration::CppGenerator& cppGen, 
		std::ostream* pImplOut,
		const std::string& className,
		const std::vector<BundleService>& services): 
	AbstractGenerator(cppGen),
	_pImplOut(pImplOut),
	_clientMode(requiresClientCode(services)),
	_serverMode(requiresServerCode(services)),
	_className(className),
	_services(services)
{
}


BundleActivatorGenerator::~BundleActivatorGenerator()
{
}


void BundleActivatorGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	// we don't care about pStruct, we only care about our service members!
	_pStruct = structClone(pStruct, _className, newBaseClasses(pStruct));
	// member variables
	
	Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable("std::vector<Poco::OSP::ServiceRef::Ptr> _services", _pStruct);
	pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	Poco::CppParser::Variable* pVar2 = new Poco::CppParser::Variable("std::set<std::string> _uris", _pStruct);
	pVar2->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	Poco::CppParser::Variable* pVar3 = new Poco::CppParser::Variable("Poco::RemotingNG::ORB* _pORB", _pStruct);
	pVar3->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	Poco::CppParser::Variable* pVar4 = new Poco::CppParser::Variable("std::set<std::string> _listeners", _pStruct);
	pVar4->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	
	// methods
	// constructor
	{
		Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
		pConstr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
		pConstr->addDocumentation(" Creates a " + _pStruct->name() + ". Initializes the bundle.");
	}
	//destructor
	{
		Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("~")+_pStruct->name(), _pStruct);
		pDestr->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
		pDestr->addDocumentation(" Destroys the " + _pStruct->name() + ".");
	}

	// void start(BundleContext::Ptr pContext)
	{
		Poco::CppParser::Function* pStart = new Poco::CppParser::Function("void start", _pStruct);
		pStart->addDocumentation(" Starts the services inside the bundle.");
		Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
		pStart->addParameter(pParam1);
	}

	// void stop(BundleContext::Ptr pContext)
	{
		Poco::CppParser::Function* pStop = new Poco::CppParser::Function("void stop", _pStruct);
		pStop->addDocumentation(" Stops the services inside the bundle.");
		Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
		pStop->addParameter(pParam1);
	}
	
	// Poco::RemotingNG::ORB& orb()
	{
		Poco::CppParser::Function* pORB = new Poco::CppParser::Function("Poco::RemotingNG::ORB& orb", _pStruct);
		pORB->addDocumentation(" Returns a reference to the Remoting ORB.");
	}

	//protected:
	//server methods: add always
	{
		// std::pair<std::string, Poco::OSP::ServiceRef::Ptr> 
		// registerRemoteService(BundleContext::Ptr pContext, Poco::OSP::Service::Ptr pService, const std::string& listener, const std::string& serviceName, Poco::OSP::Properties props = Poco::OSP::Properties())
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("std::pair<std::string, Poco::OSP::ServiceRef::Ptr> registerRemoteService", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Registers the service with the ORB and at the ServiceRegistry the bundle. Returns the Remoting URI and the serviceRef.");
			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::OSP::Service::Ptr pService", 0);
			Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("const std::string& listener", 0);
			Poco::CppParser::Parameter* pParam3= new Poco::CppParser::Parameter("const std::string& serviceName", 0);
			Poco::CppParser::Parameter* pParam4= new Poco::CppParser::Parameter("Poco::OSP::Properties props = Poco::OSP::Properties()", 0);

			pReg->addParameter(pParam0);
			pReg->addParameter(pParam1);
			pReg->addParameter(pParam2);
			pReg->addParameter(pParam3);
			pReg->addParameter(pParam4);
		}

		// std::string registerRemoteObject(Poco::RemotingNG::RemoteObject::Ptr pObj, const std::string& listener)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("std::string registerRemoteObject", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Registers the service with the ORB only. Returns the Remoting URI for the remote object.");
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::RemotingNG::RemoteObject::Ptr pObj", 0);
			Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("const std::string& listener", 0);
	
			pReg->addParameter(pParam1);
			pReg->addParameter(pParam2);
		}

		// std::string registerListener(Listener::Ptr pListener)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("std::string registerListener", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Registers the listener at the ORB. Returns the id of the endpoint. listener will be automatically unregistered with bundle stop");
			Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("Poco::RemotingNG::Listener::Ptr pListener", 0);
			pReg->addParameter(pParam1);
		}

		// Poco::OSP::ServiceRef::Ptr registerService(Poco::OSP::BundleContext::Ptr pContext, const std::string& serviceName, Poco::OSP::Service::Ptr pService, Poco::OSP::Properties props = Poco::OSP::Properties())
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("Poco::OSP::ServiceRef::Ptr registerService", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Registers the service at the serviceRegistry only. Stores the serviceRef for auto unregister");

			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("const std::string& serviceName", 0);
			Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("Poco::OSP::Service::Ptr pService", 0);
			Poco::CppParser::Parameter* pParam3= new Poco::CppParser::Parameter("Poco::OSP::Properties props = Poco::OSP::Properties()", 0);

			pReg->addParameter(pParam0);
			pReg->addParameter(pParam1);
			pReg->addParameter(pParam2);
			pReg->addParameter(pParam3);
		}

		// void unregisterRemoteService(Poco::OSP::BundleContext::Ptr pContext, const std::string& uri, Poco::OSP::ServiceRef::Ptr pService)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void unregisterRemoteService", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Unregisters the service at the ORB and at the ServiceRegistry.");

			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);
			Poco::CppParser::Parameter* pParam2= new Poco::CppParser::Parameter("Poco::OSP::ServiceRef::Ptr pService", 0);

			pReg->addParameter(pParam0);
			pReg->addParameter(pParam1);
			pReg->addParameter(pParam2);
		}

		// void unregisterRemoteObject(const std::string& uri)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void unregisterRemoteObject", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Unregisters the service at the ORB only.");

			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("const std::string& uri", 0);

			pReg->addParameter(pParam1);
		}

		// void unregisterService(Poco::OSP::BundleContext::Ptr pContext, Poco::OSP::ServiceRef::Ptr pService)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void unregisterService", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Unregisters the service at the serviceRegistry only.");

			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::OSP::ServiceRef::Ptr pService", 0);

			pReg->addParameter(pParam0);
			pReg->addParameter(pParam1);
		}

		// void unregisterListener(const std::string& listener)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void unregisterListener", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Unregisters the listener at the ORB.");
			Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("const std::string& listener", 0);
			pReg->addParameter(pParam1);
		}

		// void unregisterAll(BundleContext::Ptr pContext)
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void unregisterAll", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Unregisters all services and remoteobjects. Automatically called by stop method.");
			Poco::CppParser::Parameter* pParam1= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			pReg->addParameter(pParam1);
		}

		//void setupRemotingOSP()
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void setupRemotingOSP", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Initializes skeletons, proxyfactories and servicefactories.");
			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			pReg->addParameter(pParam0);
		}

		//void shutdownRemotingOSP()
		{
			Poco::CppParser::Function* pReg = new Poco::CppParser::Function("void shutdownRemotingOSP", _pStruct);
			pReg->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			pReg->addDocumentation(" Deinitializes skeletons, proxyfactories and servicefactories for this bundle.");
			Poco::CppParser::Parameter* pParam0= new Poco::CppParser::Parameter("Poco::OSP::BundleContext::Ptr pContext", 0);
			pReg->addParameter(pParam0);
		}

		// includes
		_cppGen.addIncludeFile("Poco/OSP/ServiceRef.h");
		_cppGen.addIncludeFile("Poco/RemotingNG/ORB.h");
		_cppGen.addSystemIncludeFile("vector");
		_cppGen.addSystemIncludeFile("set");
		_cppGen.addSystemIncludeFile("algorithm");
	} // _serverMode


	// includes
	_cppGen.addIncludeFile("Poco/OSP/BundleActivator.h");
	_cppGen.addIncludeFile("Poco/OSP/ServiceRef.h");
	
	_cppGen.addIncludeFile("Poco/RemotingNG/Identifiable.h");

	// src includes
	
	// we need to register the proxyfactories for all structs
	std::vector<BundleService>::const_iterator it = _services.begin();
	Poco::CppParser::NameSpace* pRoot = Poco::CppParser::NameSpace::root();
	for (; it != _services.end(); ++it)
	{
		if (it->clientCode)
		{
			std::string fullName = ProxyFactoryGenerator::generateQualifiedClassName(nameSpace(), it->pStruct);
			const Poco::CppParser::Struct* pF = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(fullName));
			if (!pF)
				throw Poco::NotFoundException("didn't find proxyfactory for class " + it->pStruct->fullName());
			Poco::CodeGeneration::Utility::handleSrcInclude(pF, _cppGen);
		}
		if (it->serverCode)
		{
			std::string fullName = SkeletonGenerator::generateQualifiedClassName(nameSpace(), it->pStruct);
			const Poco::CppParser::Struct* pF = dynamic_cast <const Poco::CppParser::Struct*>(pRoot->lookup(fullName));
			if (!pF)
				throw Poco::NotFoundException("didn't find skeleton for class " + it->pStruct->fullName());
			Poco::CodeGeneration::Utility::handleSrcInclude(pF, _cppGen);
		}
	}
	
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/URIUtility.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/RemoteObject.h");
	_cppGen.addSrcIncludeFile("Poco/OSP/OSPException.h");
	_cppGen.addSrcIncludeFile("Poco/OSP/ServiceRegistry.h");
	_cppGen.addSrcIncludeFile("Poco/ClassLibrary.h");
}


void BundleActivatorGenerator::structEnd()
{
	std::string className = _pStruct->name();
	std::string fullClassName = _pStruct->fullName();
	AbstractGenerator::structEnd(); // clears _pStruct!
	cppGen().writeImplementation("POCO_BEGIN_MANIFEST(Poco::OSP::BundleActivator)");
	cppGen().writeImplementation("\tPOCO_EXPORT_CLASS(" + fullClassName + ")");
	cppGen().writeImplementation("POCO_END_MANIFEST");
	cppGen().writeImplementation("");
}


void BundleActivatorGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
}


void BundleActivatorGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	e.registerCallback(_pStruct->name(), &BundleActivatorGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("start", &BundleActivatorGenerator::startCodeGen);
	e.registerCallback("stop", &BundleActivatorGenerator::stopCodeGen);
	e.registerCallback("registerRemoteService", &BundleActivatorGenerator::registerRemoteServiceCodeGen);
	e.registerCallback("registerRemoteObject", &BundleActivatorGenerator::registerRemoteObjectCodeGen);
	e.registerCallback("registerService", &BundleActivatorGenerator::registerServiceCodeGen);
	e.registerCallback("registerListener", &BundleActivatorGenerator::registerListenerCodeGen);
	e.registerCallback("unregisterRemoteService", &BundleActivatorGenerator::unregisterRemoteServiceCodeGen);
	e.registerCallback("unregisterRemoteObject", &BundleActivatorGenerator::unregisterRemoteObjectCodeGen);
	e.registerCallback("unregisterService", &BundleActivatorGenerator::unregisterServiceCodeGen);
	e.registerCallback("unregisterListener", &BundleActivatorGenerator::unregisterListenerCodeGen);
	e.registerCallback("unregisterAll", &BundleActivatorGenerator::unregisterAllCodeGen);
	e.registerCallback("setupRemotingOSP", &BundleActivatorGenerator::setupRemotingOSPCodeGen);
	e.registerCallback("shutdownRemotingOSP", &BundleActivatorGenerator::shutdownRemotingOSPCodeGen);
	e.registerCallback("orb", &BundleActivatorGenerator::orbCodeGen);
}


std::vector<std::string> BundleActivatorGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// Poco::OSP::BundleActivator
	std::vector<std::string> bases;
	bases.push_back("Poco::OSP::BundleActivator");
	return bases;
}


void BundleActivatorGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("_pORB = &Poco::RemotingNG::ORB::instance();");
}


void BundleActivatorGenerator::startCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("setupRemotingOSP(pContext);");
	gen.writeMethodImplementation("startImpl(pContext);");
	// create service here
	createServicesCodeGen(pFunc, pStruct, gen, addParam);
}


void BundleActivatorGenerator::stopCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("stopImpl(pContext);");
	gen.writeMethodImplementation("unregisterAll(pContext);");
	gen.writeMethodImplementation("shutdownRemotingOSP(pContext);");
}


void BundleActivatorGenerator::registerRemoteServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);
	// std::pair<std::string, Poco::OSP::ServiceRef::Ptr> 
	// registerRemoteService(BundleContext::Ptr pContext, 
	//		Poco::OSP::Service::Ptr pService, 
	//      const std::string& listener
	//		const std::string& serviceName
	//		Poco::OSP::Properties props)
	gen.writeMethodImplementation("Poco::RemotingNG::RemoteObject::Ptr pObj = pService.cast<Poco::RemotingNG::RemoteObject>();");
	gen.writeMethodImplementation("std::string uri = registerRemoteObject(pObj, listener);");
	gen.writeMethodImplementation("props.set(\"uri\", uri);");
	gen.writeMethodImplementation("Poco::OSP::ServiceRef::Ptr pRef;");
	gen.writeMethodImplementation("try");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tpRef = registerService(pContext, serviceName, pService, props);");
	gen.writeMethodImplementation("\tif (!pRef)");
	gen.writeMethodImplementation("\t\tthrow Poco::OSP::OSPException(\"Failed to register service \" + serviceName);");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("catch (...)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tunregisterRemoteObject(uri);");
	gen.writeMethodImplementation("\tthrow;");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("return std::make_pair(uri, pRef);");

	CppGenerator* pCppGen = dynamic_cast<CppGenerator*>(&gen);
	if (pCppGen)
	{
		pCppGen->writeToHeader("\tvirtual void startImpl(Poco::OSP::BundleContext::Ptr pContext);");
		pCppGen->writeToHeader("\t\t/// Implement this method in the Impl file to create all the services/listeners and register them via registerService");
		pCppGen->writeToHeader("");
		pCppGen->writeToHeader("\tvirtual void stopImpl(Poco::OSP::BundleContext::Ptr pContext);");
		pCppGen->writeToHeader("\t\t/// Implement this method in the Impl file for custom stop functionality.");
		pCppGen->writeToHeader("");
	}
}


void BundleActivatorGenerator::unregisterRemoteServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);
	//void unregisterRemoteService(const std::string& uri, Poco::OSP::ServiceRef::Ptr pService)
	gen.writeMethodImplementation("unregisterService(pContext, pService);");
	gen.writeMethodImplementation("unregisterRemoteObject(uri);");
}


void BundleActivatorGenerator::registerRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);
	gen.writeMethodImplementation("std::string uri = _pORB->registerObject(pObj, listener);");
	gen.writeMethodImplementation("_uris.insert(uri);");
	gen.writeMethodImplementation("return uri;");
}


void BundleActivatorGenerator::unregisterRemoteObjectCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("_uris.erase(uri);");
	gen.writeMethodImplementation("_pORB->unregisterObject(uri);");
}


void BundleActivatorGenerator::registerServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("Poco::OSP::ServiceRef::Ptr pRef = pContext->registry().registerService(serviceName, pService, props);");
	gen.writeMethodImplementation("_services.push_back(pRef);");
	gen.writeMethodImplementation("return pRef;");
}


void BundleActivatorGenerator::unregisterServiceCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("std::vector<Poco::OSP::ServiceRef::Ptr>::iterator it = std::find(_services.begin(), _services.end(), pService);");
	gen.writeMethodImplementation("if (it != _services.end())");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\t_services.erase(it);");
	gen.writeMethodImplementation("\tpContext->registry().unregisterService(pService);");
	gen.writeMethodImplementation("}");
}


void BundleActivatorGenerator::registerListenerCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("std::string listener = _pORB->registerListener(pListener);");
	gen.writeMethodImplementation("_listeners.insert(listener);");
	gen.writeMethodImplementation("return listener;");
}


void BundleActivatorGenerator::unregisterListenerCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("_listeners.erase(listener);");
	gen.writeMethodImplementation("_pORB->unregisterListener(listener);");
}


void BundleActivatorGenerator::unregisterAllCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);
	
	gen.writeMethodImplementation("for (std::vector<Poco::OSP::ServiceRef::Ptr>::reverse_iterator it = _services.rbegin(); it != _services.rend(); ++it)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\ttry");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tunregisterService(pContext, *it);");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("\tcatch (...)");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tpContext->logger().error(\"Failed to unregister service \" + (*it)->name());");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("}");
	
	gen.writeMethodImplementation("for (std::set<std::string>::iterator it = _uris.begin(); it != _uris.end(); ++it)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\ttry");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tunregisterRemoteObject(*it);");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("\tcatch (...)");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tpContext->logger().error(\"Failed to unregister RemoteObject \" + (*it));");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("for (std::set<std::string>::iterator it = _listeners.begin(); it != _listeners.end(); ++it)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\ttry");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tunregisterListener(*it);");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("\tcatch (...)");
	gen.writeMethodImplementation("\t{");
	gen.writeMethodImplementation("\t\tpContext->logger().error(\"Failed to unregister listener \" + (*it));");
	gen.writeMethodImplementation("\t}");
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("_services.clear();");
	gen.writeMethodImplementation("_uris.clear();");
	gen.writeMethodImplementation("_listeners.clear();");
}


void BundleActivatorGenerator::createServicesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);
	if (pGen->_pImplOut)
	{
		std::string incl = Poco::CodeGeneration::Utility::createInclude(pGen->_pStruct, gen.usePocoIncludeStyle());
		
		std::ostream& out = *(pGen->_pImplOut);
		out << "#include \"" << incl << "\"\n\n"<< std::endl;

		Poco::CodeGeneration::Utility::writeNameSpaceBegin(out, pGen->nameSpace());		

		out << "\n" << std::endl;
		out << "void " << pGen->_className << "::startImpl(Poco::OSP::BundleContext::Ptr pContext)" << std::endl;
		out << "{" << std::endl;
		out << "\t// Create your services here. You can add them via the various register" << std::endl;
		out << "\t// functions. Services will be automatically unregistered in the stop function." << std::endl;
		out << "\t// Changes made to this function are safe and will not be lost when regenerating." << std::endl;
		out << "}" << std::endl << std::endl << std::endl;

		out << "void " << pGen->_className << "::stopImpl(Poco::OSP::BundleContext::Ptr pContext)" << std::endl;
		out << "{" << std::endl;
		out << "\t// Add custom shutdown code here. You do not need to unregister listeners, services or remote objects." << std::endl;
		out << "\t// This will all happen automatically after returning from this method." << std::endl;
		out << "\t// Changes made to this function are safe and will not be lost when regenerating." << std::endl;
		out << "}" << std::endl << std::endl << std::endl;

		Poco::CodeGeneration::Utility::writeNameSpaceEnd(out, pGen->nameSpace());
	}
}


void BundleActivatorGenerator::orbCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return *_pORB;");
}


void BundleActivatorGenerator::setupRemotingOSPCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);

	// in clientMode we register ProxyFactories
	// in serverMode we register skeletons

	std::vector<BundleService>::const_iterator it = pGen->_services.begin();
	for (; it != pGen->_services.end(); ++it)
	{
		const Poco::CppParser::Struct* pStructIn = it->pStruct;
		Poco::CodeGeneration::CodeGenerator::Properties props;
		Poco::CodeGeneration::GeneratorEngine::parseProperties(pStructIn, props);
		if (it->clientCode)
		{
			// simply call ORB::instance().registerProxyFactory(className, new ProxyFactory());
			std::string codeLine("_pORB->registerProxyFactory(\"");
			codeLine.append(pStructIn->name());
			codeLine.append("\", new ");
			codeLine.append(ProxyFactoryGenerator::generateClassName(pStructIn));
			codeLine.append("());");
			gen.writeMethodImplementation(codeLine);
		}
		if (it->serverCode)
		{
			std::string codeLine("_pORB->registerSkeleton(\"");
			codeLine.append(pStructIn->name());
			codeLine.append("\", new ");
			codeLine.append(SkeletonGenerator::generateClassName(pStructIn));
			codeLine.append("());");
			gen.writeMethodImplementation(codeLine);
		}
	}
}


void BundleActivatorGenerator::shutdownRemotingOSPCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	BundleActivatorGenerator* pGen = dynamic_cast<BundleActivatorGenerator*>(pAGen);
	poco_check_ptr (pGen);

	// we never unregister namespace
	// in clientMode we unregister ProxyFactories
	// in serverMode we unregister skeletons

	std::vector<BundleService>::const_iterator it = pGen->_services.begin();
	for (; it != pGen->_services.end(); ++it)
	{
		const Poco::CppParser::Struct* pStructIn = it->pStruct;
		
		if (it->clientCode)
		{
			// simply call ORB::instance().registerProxyFactory(className, new ProxyFactory());
			std::string codeLine("_pORB->unregisterProxyFactory(\"");
			codeLine.append(pStructIn->name());
			codeLine.append("\");");
			gen.writeMethodImplementation(codeLine);
		}
		if (it->serverCode)
		{
			std::string codeLine("_pORB->unregisterSkeleton(\"");
			codeLine.append(pStructIn->name());
			codeLine.append("\");");
			gen.writeMethodImplementation(codeLine);
		}
	}
}


bool BundleActivatorGenerator::requiresClientCode(const std::vector<BundleService>& services)
{
	bool req = false;
	std::vector<BundleService>::const_iterator it = services.begin();
	for (; it != services.end() && !req; ++it)
	{
		req |= it->clientCode;
	}
	return req;
}


bool BundleActivatorGenerator::requiresServerCode(const std::vector<BundleService>& services)
{
	bool req = false;
	std::vector<BundleService>::const_iterator it = services.begin();
	for (; it != services.end() && !req; ++it)
	{
		req |= it->serverCode;
	}
	return req;
}


BundleActivatorGenerator::BundleService::BundleService(const Poco::CppParser::Struct* pS, bool cc, bool sc):
	pStruct(pS),
	clientCode(cc),
	serverCode(sc)
{
}
