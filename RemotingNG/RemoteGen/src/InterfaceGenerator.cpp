//
// InterfaceGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "InterfaceGenerator.h"
#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/TypeDef.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/String.h"


using Poco::CodeGeneration::Utility;


InterfaceGenerator::InterfaceGenerator(Poco::CodeGeneration::CppGenerator& cppGen, bool enableOSP):
	AbstractGenerator(cppGen),
	_enableOSP(enableOSP)

{
}


InterfaceGenerator::~InterfaceGenerator()
{
}


std::string InterfaceGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName("I");
	newClassName.append(pStruct->name());
	return newClassName;
}


std::string InterfaceGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return InterfaceGenerator::generateClassName(pStruct);

	return ns + "::" + InterfaceGenerator::generateClassName(pStruct);;
}


void InterfaceGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	bool hasAnyRemote = GenUtility::hasAnyRemoteParent(pStruct);
	if (!hasAnyRemote)
	{
		// pushBack of a name not enough, we need virtual inheritance
		if (!_enableOSP)
		{
			_pStruct->addBase("Poco::RefCountedObject", Poco::CppParser::Symbol::ACC_PUBLIC, true);
			_cppGen.addIncludeFile("Poco/RefCountedObject.h");
		}
		else
		{
			// virtual not needed, service inherists virtual from RefCountedObject
			_pStruct->addBase("Poco::OSP::Service", Poco::CppParser::Symbol::ACC_PUBLIC, false);
			_cppGen.addIncludeFile("Poco/OSP/Service.h");
		}
		_cppGen.addIncludeFile("Poco/RemotingNG/Identifiable.h");
		_cppGen.addIncludeFile("Poco/AutoPtr.h");
	}

	Poco::CodeGeneration::Utility::handleInclude(pStruct, _cppGen);
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);
	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("virtual ~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys the " + _pStruct->name() + ".");

	Poco::CppParser::Function* pTypeId = new Poco::CppParser::Function(std::string("static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId()"), _pStruct);
	pTypeId->addDocumentation(	" Returns the TypeId of the class.");

	Poco::CppParser::TypeAlias* pTypeAlias = new Poco::CppParser::TypeAlias("using Ptr = Poco::AutoPtr<" + generateClassName(pStruct) + ">", _pStruct);
	poco_check_ptr (pTypeAlias); // just avoid unused variable warning

	if (_enableOSP)
	{
		// implement service interface
		// const std::type_info& type() const;
		Poco::CppParser::Function* pType = new Poco::CppParser::Function(std::string("const std::type_info& type"), _pStruct);
		pType->makeConst();
		pType->addDocumentation(" Returns the type information for the object's class.");
		// bool isA(const std::type_info& otherType) const;
		Poco::CppParser::Function* pIsA = new Poco::CppParser::Function(std::string("bool isA"), _pStruct);
		pIsA->makeConst();
		Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter("const std::type_info& otherType", 0);
		pIsA->addParameter(pParam);
		pIsA->addDocumentation(" Returns true if the class is a subclass of the class given by otherType.");
	}

	bool ev = checkForEventMembers(pStruct, properties);
	if (ev)
	{
		Poco::CppParser::Function* pEvents = new Poco::CppParser::Function("virtual std::string remoting__enableEvents", _pStruct);
		Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter("Poco::RemotingNG::Listener::Ptr pListener", 0);
		pEvents->addParameter(pParam);
		pParam = new Poco::CppParser::Parameter("bool enable = true", 0);
		pEvents->addParameter(pParam);
		pEvents->makePureVirtual();
		pEvents->addDocumentation(" Enable or disable delivery of remote events.");
		pEvents->addDocumentation("");
		pEvents->addDocumentation(" The given Listener instance must implement the Poco::RemotingNG::EventListener");
		pEvents->addDocumentation(" interface, otherwise this method will fail with a RemotingException.");
		pEvents->addDocumentation("");
		pEvents->addDocumentation(" This method is only used with Proxy objects; calling this method on a");
		pEvents->addDocumentation(" RemoteObject will do nothing.");
		_cppGen.addIncludeFile("Poco/RemotingNG/Listener.h");
	}
}


void InterfaceGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	poco_assert_dbg (_pStruct && _pStructIn);

	if (!GenUtility::isRemoteMethod(pFuncOld, properties)) return;
	if (GenUtility::isOverride(pFuncOld->name(), _pStructIn)) return;

	// recreate a "rettype method signature" as "virtual retType method signature = 0"
	// note that a decl contains only the string up to method, no params!
	std::string decl (Poco::CodeGeneration::Utility::VIRTUAL+" ");
	std::string resRetType = Utility::resolveType(_pStructIn, pFuncOld->getReturnParameter());

	if (resRetType != pFuncOld->getReturnParameter())
	{
		const std::string& oldRet = pFuncOld->getReturnParameter();
		if (!oldRet.empty())
		{
			const std::string& oldDecl = pFuncOld->declaration();
			std::size_t pos = oldDecl.find(oldRet);
			if (pos != std::string::npos)
			{
				pos += oldRet.size();
				decl.append(resRetType);
				decl.append(oldDecl.substr(pos));
			}
			else
				decl.append(oldDecl);
		}
		else
			decl.append(pFuncOld->declaration());
	}
	else
	{
		decl.append(pFuncOld->declaration());
	}

	Poco::CppParser::Function* pFunc = new Poco::CppParser::Function(decl, _pStruct);
	pFunc->setAccess (pFuncOld->getAccess());
	addDocumentation(pFuncOld, pFunc);
	pFunc->setAttributes(pFuncOld->getAttributes());
	if (pFuncOld->isConst())
		pFunc->makeConst();

	pFunc->makePureVirtual();

	Poco::CppParser::Function::Iterator it = pFuncOld->begin();
	Poco::CppParser::Function::Iterator itEnd = pFuncOld->end();

	for (; it != itEnd; ++it)
	{
		std::string decl = Poco::CodeGeneration::Utility::resolveParamDecl(_pStructIn, *it);
		if ((*it)->hasDefaultValue())
		{
			decl.append(" = ");
			std::string type = Poco::CodeGeneration::Utility::resolveType(_pStructIn, (*it)->declType());
			Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(type);
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				Poco::CppParser::Enum* pEnum = static_cast<Poco::CppParser::Enum*>(pSym);
				if (pEnum->flags() & Poco::CppParser::Enum::ENUM_IS_CLASS)
					decl.append(pEnum->fullName());
				else
					decl.append(pEnum->nameSpace()->fullName());
				decl.append("::");
				std::string defaultValue((*it)->defaultValue());
				std::string::size_type scpos = defaultValue.find_last_of("::");
				if (scpos != std::string::npos)
				{
					defaultValue.erase(0, scpos + 1);
				}
				decl.append(defaultValue);
			}
			else
			{
				decl.append(type);
				decl.append("(");
				decl.append((*it)->defaultValue());
				decl.append(")");
			}
		}
		if ((*it)->isPointer())
			throw Poco::InvalidArgumentException("Pointer parameters are not supported: " + pFuncOld->nameSpace()->fullName() + "::" + pFuncOld->name() + "::" + (*it)->name());

		Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter(decl, pFunc);
		pFunc->addParameter(pParam);
	}
}


void InterfaceGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	e.registerCallback("remoting__typeId", &InterfaceGenerator::ifaceTypeIdCodeGen);
	e.registerCallback("type", &InterfaceGenerator::ifaceTypeCodeGen);
	e.registerCallback("isA", &InterfaceGenerator::ifaceIsACodeGen);
}


void InterfaceGenerator::ifaceTypeIdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	InterfaceGenerator* pIG = dynamic_cast<InterfaceGenerator*>(pAGen);
	poco_check_ptr (pIG);

	const Poco::CppParser::Struct* pDataType = pIG->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);
	gen.writeMethodImplementation("static const std::string REMOTING__TYPE_ID(\"" + Poco::replace(pDataType->fullName(), "::", ".") + "\");");
	gen.writeMethodImplementation("return REMOTING__TYPE_ID;");
}


void InterfaceGenerator::ifaceTypeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	InterfaceGenerator* pIG = dynamic_cast<InterfaceGenerator*>(pAGen);
	poco_check_ptr (pIG);

	const Poco::CppParser::Struct* pDataType = pIG->_pStructIn; // returns the data type for which pStruct was generated

	poco_assert(pDataType);
	gen.writeMethodImplementation("return typeid(" + InterfaceGenerator::generateClassName(pDataType) + ");");
}


void InterfaceGenerator::ifaceIsACodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_check_ptr (addParam);

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	InterfaceGenerator* pIG = dynamic_cast<InterfaceGenerator*>(pAGen);
	poco_check_ptr (pIG);

	const Poco::CppParser::Struct* pDataType = pIG->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);
	// static const std::string name(typeid(MyClass).name());
	// return name == otherType.name() || MyBaseClass::isA(otherType);

	std::string nameCheck("return name == otherType.name()");

	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();

	for (; itB != itBEnd; ++itB)
	{
		nameCheck.append(" || ");
		nameCheck.append(itB->name + "::isA(otherType)");
	}
	nameCheck.append(";");
	gen.writeMethodImplementation("static const std::string name(typeid(" + pStruct->fullName() + ").name());");
	gen.writeMethodImplementation(nameCheck);
}


std::vector<std::string> InterfaceGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;
	GenUtility::hasAnyRemoteParent(pStruct);

	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();

	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent && Utility::hasAnyRemoteProperty(pParent))
		{
			bases.push_back(generateQualifiedClassName(nameSpace(), pParent));
		}
	}

	return bases;
}


bool InterfaceGenerator::checkForEventMembers(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	bool ev = false;
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
			// add a variable with the same name
			Poco::CppParser::Variable* pVarNew = new Poco::CppParser::Variable(resolveEventType(pStruct, pVar->declaration()), _pStruct);
			pVarNew->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);
			ev = true;
		}
	}
	return ev;
}


std::string InterfaceGenerator::resolveEventType(const Poco::CppParser::Struct* pStruct, const std::string& decl)
{
	std::string::size_type pos = decl.find_last_of(' ');
	std::string typeDecl(decl, 0, pos);
	return Utility::resolveType(pStruct, typeDecl) + decl.substr(pos);
}
