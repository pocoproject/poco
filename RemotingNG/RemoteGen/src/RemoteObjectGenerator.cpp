//
// RemoteObjectGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RemoteObjectGenerator.h"
#include "InterfaceGenerator.h"
#include "EventDispatcherGenerator.h"
#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Util/Application.h"


using Poco::CodeGeneration::Utility;


RemoteObjectGenerator::RemoteObjectGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen), _hasEvents(false)
{
}


RemoteObjectGenerator::~RemoteObjectGenerator()
{
}


std::string RemoteObjectGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("RemoteObject");
	return newClassName;
}


std::string RemoteObjectGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return RemoteObjectGenerator::generateClassName(pStruct);

	return ns + "::" + RemoteObjectGenerator::generateClassName(pStruct);
}


void RemoteObjectGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	_codeInjectors.clear();
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	//const Identifiable::ObjectId& oid
	Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("const Poco::RemotingNG::Identifiable::ObjectId& oid", pConstr);
	Poco::CppParser::Parameter* pParam2 = new Poco::CppParser::Parameter("Poco::SharedPtr<" + pStruct->fullName() + "> pServiceObject", pConstr);
	pConstr->addParameter(pParam1);
	pConstr->addParameter(pParam2);

	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("virtual ~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");

	[[maybe_unused]] Poco::CppParser::TypeAlias* pTypeAlias = new Poco::CppParser::TypeAlias("using Ptr = Poco::AutoPtr<" + generateClassName(pStruct) + ">", _pStruct); // registers itself with _pStruct

	// adds the member var
	_cppGen.addIncludeFile("Poco/SharedPtr.h");
	_cppGen.addIncludeFile("Poco/RemotingNG/RemoteObject.h");
	_cppGen.addIncludeFile("Poco/RemotingNG/Identifiable.h");
	Poco::CppParser::Variable* pVar = new Poco::CppParser::Variable("Poco::SharedPtr<" + pStruct->fullName() + "> _pServiceObject", _pStruct);
	pVar->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
	// add a method virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const
	Poco::CppParser::Function* pTypeId = new Poco::CppParser::Function("virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId", _pStruct);
	pTypeId->makeConst();
	pTypeId->makeInline();

	if (GenUtility::hasEvents(pStruct))
	{
		Poco::CppParser::Function* pEvents = new Poco::CppParser::Function("virtual std::string remoting__enableEvents", _pStruct);
		Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter("Poco::RemotingNG::Listener::Ptr pListener", 0);
		pEvents->addParameter(pParam);
		pParam = new Poco::CppParser::Parameter("bool enable = true", 0);
		pEvents->addParameter(pParam);

		Poco::CppParser::Function* pHasEvents = new Poco::CppParser::Function("virtual bool remoting__hasEvents", _pStruct);
		pHasEvents->makeConst();

		Poco::CppParser::Function* pRemoteEvents = new Poco::CppParser::Function("virtual void remoting__enableRemoteEvents", _pStruct);
		pParam = new Poco::CppParser::Parameter("const std::string& protocol", 0);
		pRemoteEvents->addParameter(pParam);

		std::string inc = Utility::createInclude(_pStruct, _cppGen.usePocoIncludeStyle());
		Poco::replaceInPlace(inc, "RemoteObject", "EventDispatcher");
		_cppGen.addSrcIncludeFile(inc);
	}

	handleParentFunctions(_pStructIn);
	checkForEventMembers(pStruct, properties);
}


void RemoteObjectGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	// for all other methods we want the simple fwd implementation
	e.registerDefaultCallback(&RemoteObjectGenerator::fwdCodeGen);
	e.registerCallback(_pStruct->name(), &RemoteObjectGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &RemoteObjectGenerator::destructorCodeGen);
	e.registerCallback("remoting__typeId", &AbstractGenerator::typeIdCodeGen);
	if (eventsFound())
	{
		e.registerCallback("remoting__enableEvents", &RemoteObjectGenerator::enableEventsCodeGen);
		e.registerCallback("remoting__hasEvents", &RemoteObjectGenerator::hasEventsCodeGen);
		e.registerCallback("remoting__enableRemoteEvents", &RemoteObjectGenerator::enableRemoteEventsCodeGen);
	}

	std::vector<std::string>::const_iterator itE = _events.begin();
	for (; itE != _events.end(); ++itE)
	{
		e.registerCallback(RemoteObjectGenerator::generateEventFunctionName(*itE), &RemoteObjectGenerator::eventCodeGen);
	}

	// now register others
	std::map<std::string, Poco::CodeGeneration::GeneratorEngine::MethodGenerator>::const_iterator it = _codeInjectors.begin();
	std::map<std::string, Poco::CodeGeneration::GeneratorEngine::MethodGenerator>::const_iterator itEnd = _codeInjectors.end();

	for (; it != itEnd; ++it)
	{
		e.registerCallback(it->first, it->second);
	}
	_codeInjectors.clear();
}


void RemoteObjectGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	if (_functions.find(pFuncOld->name()) != _functions.end())
		return;

	if (GenUtility::isRemoteMethod(pFuncOld, properties))
	{
		_functions.insert(pFuncOld->name());
		Poco::CppParser::Function* pFunc = methodClone(pFuncOld, properties);
		pFunc->makeInline(); // impl is just one single line
		Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itSync = properties.find(Poco::CodeGeneration::Utility::SYNCHRONIZED);
		if (itSync != properties.end() && (itSync->second == Utility::VAL_TRUE || itSync->second.empty() || itSync->second == "all" || itSync->second == "remote"))
		{
			_codeInjectors.insert(std::make_pair(pFunc->name(), &RemoteObjectGenerator::syncFwdCodeGen));
		}
		else
		{
			_codeInjectors.insert(std::make_pair(pFunc->name(), &RemoteObjectGenerator::fwdCodeGen));
		}
	}
}


std::vector<std::string> RemoteObjectGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;
	bases.push_back(InterfaceGenerator::generateQualifiedClassName(nameSpace(), pStruct));
	bases.push_back("Poco::RemotingNG::RemoteObject");
	return bases;
}


void RemoteObjectGenerator::fwdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	// we only write one line
	// either _pServiceObject->method (params) or return _pServiceObject->method (params)
	std::string codeLine;
	if (pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID)
	{
		codeLine.append(Poco::CodeGeneration::Utility::RETURN+" ");
	}
	codeLine.append("_pServiceObject->");
	codeLine.append(pFunc->name());
	codeLine.append("(");
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();

	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Parameter* pParam = *it;
		Poco::CppParser::Function::Iterator itBeforeEnd = pFunc->end();
		--itBeforeEnd;
		codeLine.append(pParam->name());
		if (it != itBeforeEnd)
			codeLine.append(", ");
	}
	codeLine.append(");");
	gen.writeMethodImplementation(codeLine);
}


void RemoteObjectGenerator::syncFwdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("Poco::FastMutex::ScopedLock lock(mutex());");
	RemoteObjectGenerator::fwdCodeGen(pFunc, pStruct, gen, addParam);
}


void RemoteObjectGenerator::checkForEventMembers(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
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


void RemoteObjectGenerator::checkForEventMembersImpl(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
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
			_hasEvents = true;
			_events.push_back(pVar->name());
			_cppGen.addSrcIncludeFile("Poco/RemotingNG/ORB.h");
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
			Poco::CppParser::Function* pFunc = new Poco::CppParser::Function(funcDecl, _pStruct);
			pFunc->setAccess(Poco::CppParser::Symbol::ACC_PROTECTED);
			if (paramDecl != "void")
			{
				Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter(paramDecl, 0);
				pFunc->addParameter(pParam);
			}
		}
	}
}


std::string RemoteObjectGenerator::generateEventFunctionName(const std::string& eventVarname)
{
	return "event__" + Poco::trim(eventVarname);
}


void RemoteObjectGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	RemoteObjectGenerator* pProxy = dynamic_cast<RemoteObjectGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	const Poco::CppParser::Struct* pDataType = pProxy->_pStructIn; // returns the data type for which pStruct was generated
	const std::vector<std::string>& events = pProxy->_events;
	std::vector<std::string>::const_iterator it = events.begin();
	for (; it != events.end(); ++it)
	{
		std::string line("_pServiceObject->");
		line.append(*it);
		line.append(" += ");
		line.append("Poco::delegate(this, &");
		line.append(RemoteObjectGenerator::generateClassName(pDataType));
		line.append("::");
		line.append(RemoteObjectGenerator::generateEventFunctionName(*it));
		line.append(");");
		gen.writeMethodImplementation(line);
	}
}


void RemoteObjectGenerator::destructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	RemoteObjectGenerator* pProxy = dynamic_cast<RemoteObjectGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	gen.writeMethodImplementation("try");
	gen.writeMethodImplementation("{");
	const Poco::CppParser::Struct* pDataType = pProxy->_pStructIn; // returns the data type for which pStruct was generated
	const std::vector<std::string>& events = pProxy->_events;
	std::vector<std::string>::const_iterator it = events.begin();
	for (; it != events.end(); ++it)
	{
		std::string line("\t_pServiceObject->");
		line.append(*it);
		line.append(" -= ");
		line.append("Poco::delegate(this, &");
		line.append(RemoteObjectGenerator::generateClassName(pDataType));
		line.append("::");
		line.append(RemoteObjectGenerator::generateEventFunctionName(*it));
		line.append(");");
		gen.writeMethodImplementation(line);
	}
	gen.writeMethodImplementation("}");
	gen.writeMethodImplementation("catch (...)");
	gen.writeMethodImplementation("{");
	gen.writeMethodImplementation("\tpoco_unexpected();");
	gen.writeMethodImplementation("}");
}


void RemoteObjectGenerator::eventCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	RemoteObjectGenerator* pProxy = dynamic_cast<RemoteObjectGenerator*>(pAGen);
	poco_check_ptr (pProxy);

	const std::vector<std::string>& events = pProxy->_events;

	std::vector<std::string>::const_iterator it = events.begin();
	for (; it != events.end(); ++it)
	{
		std::string name = RemoteObjectGenerator::generateEventFunctionName(*it);
		if (name == pFunc->name())
		{
			// jep, we found the value
			if (pFunc->countParameters() == 0) // void event
				gen.writeMethodImplementation(*it + "(this);");
			else
				gen.writeMethodImplementation(*it + "(this, data);");
			return;
		}
	}
}


void RemoteObjectGenerator::hasEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return true;");
}


void RemoteObjectGenerator::enableEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	gen.writeMethodImplementation("return std::string();");
}


void RemoteObjectGenerator::enableRemoteEventsCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	RemoteObjectGenerator* pGen = dynamic_cast<RemoteObjectGenerator*>(pAGen);
	poco_check_ptr (pGen);
	const Poco::CppParser::Struct* pStructIn = pGen->_pStructIn;

	gen.writeMethodImplementation("Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = new " + EventDispatcherGenerator::generateClassName(pStructIn) + "(this, remoting__objectId(), protocol);");
	gen.writeMethodImplementation("Poco::RemotingNG::ORB::instance().registerEventDispatcher(remoting__getURI().toString(), pEventDispatcher);");
}
