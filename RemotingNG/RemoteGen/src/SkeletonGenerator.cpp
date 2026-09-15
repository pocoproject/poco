//
// SkeletonGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SkeletonGenerator.h"
#include "InterfaceGenerator.h"
#include "ProxyGenerator.h"
#include "GenUtility.h"
#include "RemoteObjectGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include "Poco/String.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include <cctype>


using namespace Poco::CodeGeneration;


SkeletonGenerator::SkeletonGenerator(Poco::CodeGeneration::CppGenerator& cppGen):
	AbstractGenerator(cppGen),
	_pCurrent(0),
	_currentFctOneWayProperty(false),
	_isEvent(false)
{
}


SkeletonGenerator::~SkeletonGenerator()
{
}


std::string SkeletonGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	newClassName.append("Skeleton");
	return newClassName;
}


std::string SkeletonGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return SkeletonGenerator::generateClassName(pStruct);

	return ns + "::" + SkeletonGenerator::generateClassName(pStruct);
}


void SkeletonGenerator::processRemoteMethods(const Poco::CppParser::Struct* pStruct)
{
	Poco::CodeGeneration::CodeGenerator::Properties classProperties;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, classProperties);
	Poco::CppParser::Struct::Functions functions;
	pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functions);

	Poco::CppParser::Struct::Functions::const_iterator it = functions.begin();
	Poco::CppParser::Struct::Functions::const_iterator itEnd = functions.end();
	for (; it != itEnd; ++it)
	{
		if (!((*it)->isDefault() || (*it)->isDeleted()))
		{
			Poco::CodeGeneration::CodeGenerator::Properties methodProperties(classProperties);
			Poco::CodeGeneration::GeneratorEngine::parseProperties(*it, methodProperties);
			if (GenUtility::isRemoteMethod(*it, methodProperties))
			{
				GenUtility::checkFunctionParams(*it);
				includeTypeSerializers(*it, false, false);
			}
		}
	}

	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent && Utility::hasAnyRemoteProperty(pParent))
		{
			processRemoteMethods(pParent);
		}
	}
}


void SkeletonGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	AbstractGenerator::structStart(pStruct, properties);
	// add constructor/destructor
	Poco::CppParser::Function* pConstr = new Poco::CppParser::Function(_pStruct->name(), _pStruct);

	pConstr->addDocumentation(	" Creates a " + _pStruct->name() + ".");

	Poco::CppParser::Function* pDestr = new Poco::CppParser::Function(std::string("virtual ~")+_pStruct->name(), _pStruct);
	pDestr->addDocumentation(	" Destroys a " + _pStruct->name() + ".");


	// add a method virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const
	Poco::CppParser::Function* pTypeId = new Poco::CppParser::Function("virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId", _pStruct);
	pTypeId->makeConst();
	pTypeId->makeInline();

	// add a member: static const std::string DEFAULT_NS;
	Poco::CppParser::Variable* pVar2 = new Poco::CppParser::Variable("static const std::string DEFAULT_NS", _pStruct);
	pVar2->setAccess(Poco::CppParser::Symbol::ACC_PUBLIC);

	// we will write the inline classes here already!
	// add src includes at the beginning
	std::string remoteObject = RemoteObjectGenerator::generateQualifiedClassName(nameSpace(), _pStructIn);
	Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(remoteObject);
	poco_assert (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_STRUCT);
	Poco::CodeGeneration::Utility::handleBaseClassesInclude(_pStruct, _cppGen);
	Poco::CodeGeneration::Utility::handleInclude(static_cast<const Poco::CppParser::Struct*>(pSym), _cppGen);
	_cppGen.writeDefaultHeader(_pStruct, _pStruct->name(), _pStruct->getLibrary(), _pStruct->getPackage());

	processRemoteMethods(pStruct);

	_cppGen.addSrcIncludeFile("Poco/RemotingNG/MethodHandler.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/ServerTransport.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Serializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/Deserializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeSerializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/TypeDeserializer.h");
	_cppGen.addSrcIncludeFile("Poco/RemotingNG/RemotingException.h");
	_cppGen.addSrcIncludeFile("Poco/SharedPtr.h");
	_cppGen.writeIncludes();
	_cppGen.writeNameSpaceBegin(nameSpace());

	handleParentFunctions(pStruct);
	checkForEventMembers(pStruct);
}


void SkeletonGenerator::structEnd()
{
	Poco::CodeGeneration::GeneratorEngine e;
	registerCallbacks(e);
	AbstractGenerator* pGen = this; // cast first to pGen for void* cast safety
	e.generate(_pStruct, _cppGen, reinterpret_cast<void*>(pGen) ,false, false);
	_cppGen.writeNameSpaceEnd(nameSpace());
	_cppGen.endFile();
	_pStruct = 0;
	_pStructIn = 0;
}


void SkeletonGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	if (!GenUtility::isRemoteMethod(pFuncOld, properties)) return;
	if (GenUtility::isOverride(pFuncOld->name(), _pStructIn)) return;

	_pCurrent = pFuncOld;

	_currentFctOneWayProperty = (properties.find(Poco::CodeGeneration::Utility::ONEWAY) != properties.end());
	// we create for each method a class className##MethodName##MethodHandler
	std::string className(_pStructIn->name());
	std::string methodName(pFuncOld->name());
	methodName[0] = std::toupper(methodName[0]);
	className.append(methodName);
	className.append("MethodHandler");
	std::string decl("class ");
	if (!_cppGen.libraryName().empty())
	{
		decl.append(_cppGen.libraryName());
		decl.append("_API ");
	}
	decl.append(className);

	Poco::CppParser::Struct* pStruct = new Poco::CppParser::Struct(decl, true, _pNs);
	pStruct->addBase("Poco::RemotingNG::MethodHandler", Poco::CppParser::Symbol::ACC_PUBLIC, false);
	pStruct->makeInline();
	pStruct->fixupBases();
	if (pFuncOld->attrs().has("event"))
	{
		pStruct->attrs().set("event", "true");
		_isEvent = true;
	}
	else
		_isEvent = false;

	// add: void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deserializer, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	Poco::CppParser::Function* pInvoke = new Poco::CppParser::Function("void invoke", pStruct);
	Poco::CppParser::Parameter* pParam1 = new Poco::CppParser::Parameter("Poco::RemotingNG::ServerTransport& remoting__trans", 0);
	Poco::CppParser::Parameter* pParam2 = new Poco::CppParser::Parameter("Poco::RemotingNG::Deserializer& remoting__deser", 0);
	Poco::CppParser::Parameter* pParam3 = new Poco::CppParser::Parameter("Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject", 0);
	pInvoke->addParameter(pParam1);
	pInvoke->addParameter(pParam2);
	pInvoke->addParameter(pParam3);

	// don't append the real method name. Use the renamed version!
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFuncOld, funcProps);
	std::string aName(pFuncOld->name());
	GeneratorEngine::getStringProperty(funcProps, Utility::NAME, aName);
	_methodHandlers.insert(std::make_pair(aName, pStruct));
	Poco::CodeGeneration::GeneratorEngine e;
	e.registerCallback("invoke", &SkeletonGenerator::invokeCodeGen);
	AbstractGenerator* pGen = this;
	e.generate(pStruct, _cppGen, reinterpret_cast<void*>(pGen), false, false);
}


void SkeletonGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &SkeletonGenerator::constructorCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("remoting__typeId", &SkeletonGenerator::typeIdCodeGen);
	e.registerPostClassCallback(&SkeletonGenerator::staticMembersInitializer);
}


std::vector<std::string> SkeletonGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;
	bases.push_back("Poco::RemotingNG::Skeleton");
	return bases;
}


void SkeletonGenerator::constructorCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SkeletonGenerator* pGen = dynamic_cast<SkeletonGenerator*>(pAGen);
	poco_check_ptr (pGen);

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	SkeletonGenerator::MethodHandlers::const_iterator it = pGen->_methodHandlers.begin();
	SkeletonGenerator::MethodHandlers::const_iterator itEnd = pGen->_methodHandlers.end();
	for (; it != itEnd; ++it)
	{
		// add: addMethodHandler("create", new TransportManagerCreateMethodHandler());
		std::string codeLine("addMethodHandler(\"");
		codeLine.append(it->first);
		codeLine.append("\"s, new ");
		codeLine.append(it->second->fullName());
		codeLine.append(");");
		gen.writeMethodImplementation(codeLine);
	}
}


void SkeletonGenerator::invokeCodeGen(const Poco::CppParser::Function* pFuncNew, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SkeletonGenerator* pGen = dynamic_cast<SkeletonGenerator*>(pAGen);
	const Poco::CppParser::Function* pFunc = pGen->getCurrentFct();
	poco_check_ptr(pFunc);
	bool isEvent = pGen->_isEvent;

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	ProxyGenerator::OrderedParameters attrs;
	ProxyGenerator::OrderedParameters elems;
	std::set<std::string> nsSet;
	ProxyGenerator::doElemAttrSplit(pFunc, attrs, elems, nsSet);
	std::map<std::string, int> nsIdx;
	//static std::string REMOTING__NAMES[] = {"create"s, "protocol"s, "endPoint"s}; // methodName followed by param names, followed by other than default namespaces
	std::string staticIds = ProxyGenerator::generateStaticIdString(pFunc, nsSet, attrs, elems, nsIdx);
	gen.writeMethodImplementation(staticIds);
	std::map<std::string, const Poco::CppParser::Parameter*> outParams;
	ProxyGenerator::detectOutParams(pFunc, outParams);

	Poco::CppParser::Parameter retParam(pFunc->getReturnParameter() + " dummy", 0);
	bool hasReturnParam = pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID && !pFunc->getReturnParameter().empty();
	bool isOneWay = outParams.empty() && !hasReturnParam && pGen->currentFctHasOneWayProperty();
	std::string indentation;
	if (!isOneWay)
	{
		gen.writeMethodImplementation("bool remoting__requestSucceeded = false;");
		gen.writeMethodImplementation("try");
		gen.writeMethodImplementation("{");
		indentation = "\t";
	}

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pFunc->nameSpace(), structProps);
	CodeGenerator::Properties funcProps(structProps);
	GeneratorEngine::parseProperties(pFunc, funcProps);

	// now create for each method variable a placeholder where we can deserialize data to
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		const Poco::CppParser::Parameter* pParam = *it;
		std::string fullDeclType(Poco::CodeGeneration::Utility::resolveType(pGen->_pStructIn, pParam->declType()));
		poco_assert (!pParam->isPointer());
		std::string decl(fullDeclType);
		decl += " ";
		decl += (*it)->name();
		if ((*it)->hasDefaultValue() && !(*it)->defaultValue().empty())
		{
			decl += "(";
			decl += (*it)->defaultValue();
			decl += ")";
		}
		decl += ";";
		gen.writeMethodImplementation(indentation+decl);
	}
	// now deserialize the individual entries
	//not needed: done in Skeleton::invoke gen.writeMethodImplementation(indentation + "remoting__deser.setup(istr);");
	writePrepareAttribute(pGen, attrs, indentation, gen);

	std::string structDefaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, structDefaultNS);
	std::string funcDefaultNS(structDefaultNS);
	GeneratorEngine::getStringProperty(funcProps, Utility::NAMESPACE, funcDefaultNS);
	int funcNsIdx = -1;
	if (!funcDefaultNS.empty())
	{
		if (funcDefaultNS == structDefaultNS)
			gen.writeMethodImplementation(indentation + "remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, " + SkeletonGenerator::generateClassName(pGen->_pStructIn) +"::DEFAULT_NS);");
		else
		{
			std::map<std::string, int>::const_iterator itNS = nsIdx.find(funcDefaultNS);
			poco_assert_dbg (itNS != nsIdx.end());
			std::string code("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
			funcNsIdx = itNS->second;
			code.append(Poco::NumberFormatter::format(itNS->second));
			code.append("]);");
			gen.writeMethodImplementation(indentation+code);
		}
	}

	std::string structPath;
	GeneratorEngine::getStringProperty(structProps, Utility::PATH, structPath);
	std::string funcPath(structPath);
	GeneratorEngine::getStringProperty(funcProps, Utility::PATH, funcPath);
	if (!funcPath.empty())
	{
		std::string line("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, \"");
		line.append(funcPath);
		line.append("\"s);");
		gen.writeMethodImplementation(indentation+line);
	}

	std::string structRequestContentType;
	GeneratorEngine::getStringProperty(structProps, Utility::CONSUMES, structRequestContentType);
	std::string funcRequestContentType(structRequestContentType);
	GeneratorEngine::getStringProperty(funcProps, Utility::CONSUMES, funcRequestContentType);
	if (!funcRequestContentType.empty())
	{
		std::string line("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONSUMES, \"");
		line.append(funcRequestContentType);
		line.append("\"s);");
		gen.writeMethodImplementation(indentation+line);
	}

	// push soapHeader attributes
	ProxyGenerator::OrderedParameters::const_iterator itElem = elems.begin();
	ProxyGenerator::OrderedParameters::const_iterator itElemEnd = elems.end();
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
				std::string header;
				GeneratorEngine::getStringProperty(elemProps, GenUtility::ATTR_HEADER, header);
				Poco::toLowerInPlace(header);
				if (elemProps.find(GenUtility::ATTR_HEADER) != elemProps.end() && (header.empty() || header == "true"))
				{
					std::string code("remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, ");
					code.append("REMOTING__NAMES[");
					code.append(Poco::NumberFormatter::format(itElem->second.namePos));
					code.append("]);");
					gen.writeMethodImplementation(indentation+code);
				}
			}
		}
	}

	std::string messageType(isEvent ? "EVENT" : "REQUEST");
	gen.writeMethodImplementation(indentation + "remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");

	writeTypeDeserializers(pFunc, attrs, indentation, gen);
	writeTypeDeserializers(pFunc, elems, indentation, gen);

	gen.writeMethodImplementation(indentation + "remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");

	if (!funcRequestContentType.empty())
	{
		gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONSUMES);");
	}
	if (!funcPath.empty())
	{
		gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);");
	}
	if (!funcDefaultNS.empty())
	{
		gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}

	bool authenticated = false;
	std::string structPermission;
	GeneratorEngine::getStringProperty(structProps, Utility::PERMISSION, structPermission);
	std::string funcPermission(structPermission);
	GeneratorEngine::getStringProperty(funcProps, Utility::PERMISSION, funcPermission);

	if (funcPermission.empty())
	{
		GeneratorEngine::getBoolProperty(structProps, Utility::AUTHENTICATED, authenticated);
		GeneratorEngine::getBoolProperty(funcProps, Utility::AUTHENTICATED, authenticated);
	}
	else
	{
		authenticated = true;
	}

	if (authenticated)
	{
		gen.writeMethodImplementation(indentation + "if (!remoting__trans.authenticate(REMOTING__NAMES[0]))");
		gen.writeMethodImplementation(indentation + "\tthrow Poco::RemotingNG::AuthenticationFailedException();");
	}

	if (!funcPermission.empty())
	{
		gen.writeMethodImplementation(indentation + "static const std::string REMOTING__PERMISSION(\"" + funcPermission + "\");");

		gen.writeMethodImplementation(indentation + "if (!remoting__trans.authorize(REMOTING__NAMES[0], REMOTING__PERMISSION))");
		gen.writeMethodImplementation(indentation + "\tthrow Poco::RemotingNG::NoPermissionException(REMOTING__PERMISSION);");
	}

	// now get the pRemoteObject and static_cast it to the required RemoteObject subclass
	std::string fN = pStruct->fullName();
	size_t pos = fN.rfind("::");
	std::string ns;
	if (pos != std::string::npos)
		ns = fN.substr(0, pos);

	std::string remoteObject = RemoteObjectGenerator::generateQualifiedClassName(ns, pGen->_pStructIn);
	std::string localObjectLine(remoteObject+"* remoting__pCastedRO = static_cast<");
	localObjectLine.append(remoteObject+"*>(remoting__pRemoteObject.get());");
	gen.writeMethodImplementation(indentation+localObjectLine);

	std::string invokeLine;
	if (hasReturnParam)
	{

		std::string fullDeclType(Poco::CodeGeneration::Utility::resolveType(pGen->_pStructIn, retParam.declType()));
		invokeLine = fullDeclType + " remoting__return = ";
	}
	// now add the call to the method
	if (isOneWay)
	{
		gen.writeMethodImplementation(indentation + "try");
		gen.writeMethodImplementation(indentation + "{");
	}
	invokeLine.append("remoting__pCastedRO->");
	invokeLine.append(pFunc->name());
	invokeLine.append("(");
	// write single params
	it = pFunc->begin();
	itEnd = pFunc->end();

	for (; it != itEnd; ++it)
	{
		invokeLine.append((*it)->name());
		Poco::CppParser::Function::Iterator itBeforeEnd = itEnd;
		--itBeforeEnd;
		if (it != itBeforeEnd)
			invokeLine.append(", ");
	}

	invokeLine.append(");");
	if (isOneWay)
	{
		gen.writeMethodImplementation(indentation + "\t"+invokeLine);
		gen.writeMethodImplementation(indentation + "}");
		gen.writeMethodImplementation(indentation + "catch (...)");
		gen.writeMethodImplementation(indentation + "{");
		gen.writeMethodImplementation(indentation + "}");
	}
	else
		gen.writeMethodImplementation(indentation+invokeLine);

	if (!isOneWay)
	{
		gen.writeMethodImplementation(indentation + "remoting__requestSucceeded = true;");

		std::string structResponseContentType;
		GeneratorEngine::getStringProperty(structProps, Utility::PRODUCES, structResponseContentType);
		std::string funcResponseContentType = structResponseContentType;
		GeneratorEngine::getStringProperty(funcProps, Utility::PRODUCES, funcResponseContentType);
		if (!funcResponseContentType.empty())
		{
			gen.writeMethodImplementation(indentation + "remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + funcResponseContentType + "\"s);");
		}

		gen.writeMethodImplementation(indentation + "Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);");
		if (!funcDefaultNS.empty())
		{
			if (funcDefaultNS == structDefaultNS)
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, " + SkeletonGenerator::generateClassName(pGen->_pStructIn) +"::DEFAULT_NS);");
			else
			{
				std::map<std::string, int>::const_iterator itNS = nsIdx.find(funcDefaultNS);
				poco_assert_dbg (itNS != nsIdx.end());
				std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
				funcNsIdx = itNS->second;
				code.append(Poco::NumberFormatter::format(itNS->second));
				code.append("]);");
				gen.writeMethodImplementation(indentation+code);
			}
		}

		if (!funcResponseContentType.empty())
		{
			std::string line("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PRODUCES, \"");
			line.append(funcResponseContentType);
			line.append("\"s);");
			gen.writeMethodImplementation(indentation + line);
		}

		// write attrs before serializeRequest
		writePushAttributes(pGen, pFunc, attrs, outParams, indentation, gen);
		// push soapHeader attributes

		ProxyGenerator::OrderedParameters::const_iterator itElem = elems.begin();
		ProxyGenerator::OrderedParameters::const_iterator itElemEnd = elems.end();
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
						std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, ");
						code.append("REMOTING__NAMES[");
						code.append(Poco::NumberFormatter::format(itElem->second.namePos));
						code.append("]);");
						gen.writeMethodImplementation(indentation+code);
					}
				}
			}
		}

		std::string name(GenUtility::getMethodName(pFunc));
		std::string responseName(GenUtility::getReplyMethodName(pFunc));
		std::string messageType(isEvent ? "EVENT" : "REPLY");
		if (name != responseName)
		{
			gen.writeMethodImplementation(indentation + "static const std::string REMOTING__REPLY_NAME(\"" + responseName + "\");");
			gen.writeMethodImplementation(indentation + "remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation(indentation + "remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}

		// write first the attrs, then the return param, then the other out params

		writeTypeSerializer(pFunc, attrs, outParams, indentation, true, funcNsIdx, gen);

		std::string serLine("Poco::RemotingNG::TypeSerializer<");
		if (hasReturnParam)
		{
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
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
			}
			if (!format.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
			}
			if (!contentType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
			}
			if (!length.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
			}

			std::string retType(Poco::CodeGeneration::Utility::resolveType(pGen->_pStructIn, retParam.declType()));
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(retType);
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				retType = "int";
			}
			std::string retName(GenUtility::getReturnParameterName(pFunc));
			if (retName != Poco::RemotingNG::SerializerBase::RETURN_PARAM)
			{
				// static const std::string REMOTING__RETURN_PARAM_NAME("$retName")
				std::string line("static const std::string REMOTING__RETURN_PARAM_NAME(\"");
				line.append(retName);
				line.append("\"s);");
				gen.writeMethodImplementation(indentation+line);
				retName = "REMOTING__RETURN_PARAM_NAME";
			}
			else
				retName = "Poco::RemotingNG::SerializerBase::RETURN_PARAM";
			bool doInline = GenUtility::getInlineReturnParam(pFunc);
			if (doInline)
				gen.writeMethodImplementation(indentation+serLine + retType +">::serializeImpl("+retName+", remoting__return, remoting__ser);");
			else
				gen.writeMethodImplementation(indentation+serLine + retType +">::serialize("+retName+", remoting__return, remoting__ser);");

			if (!format.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
			}
			if (!location.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
			}
			if (!contentType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
			}
			if (!length.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
			}
		}

		writeTypeSerializer(pFunc, elems, outParams, indentation, false, funcNsIdx, gen);
		if (name != responseName)
		{
			gen.writeMethodImplementation(indentation + "remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		else
		{
			gen.writeMethodImplementation(indentation + "remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_" + messageType + ");");
		}
		if (!funcResponseContentType.empty())
		{
			gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PRODUCES);");
		}
		if (!funcDefaultNS.empty())
		{
			gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
		}

		gen.writeMethodImplementation("}");
		gen.writeMethodImplementation("catch (const Poco::Exception& e)");
		gen.writeMethodImplementation("{");
		// don't catch errors which happen during serializing data back to the caller
		gen.writeMethodImplementation(indentation + "if (!remoting__requestSucceeded)");
		gen.writeMethodImplementation(indentation + "{");
		gen.writeMethodImplementation(indentation + "\tremoting__trans.reportException(\"" + pFunc->fullName() + "\"s, e);");
		gen.writeMethodImplementation(indentation + "\tPoco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);");
		gen.writeMethodImplementation(indentation + "\tremoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);");
		gen.writeMethodImplementation(indentation + "}");
		gen.writeMethodImplementation("}");
		gen.writeMethodImplementation("catch (const std::exception& e)");
		gen.writeMethodImplementation("{");
		// don't catch errors which happen during serializing data back to the caller
		gen.writeMethodImplementation(indentation + "if (!remoting__requestSucceeded)");
		gen.writeMethodImplementation(indentation + "{");
		gen.writeMethodImplementation(indentation + "\tconst Poco::Exception exc(e.what());");
		gen.writeMethodImplementation(indentation + "\tremoting__trans.reportException(\"" + pFunc->fullName() + "\"s, exc);");
		gen.writeMethodImplementation(indentation + "\tPoco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);");
		gen.writeMethodImplementation(indentation + "\tremoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);");
		gen.writeMethodImplementation(indentation + "}");
		gen.writeMethodImplementation("}");
		gen.writeMethodImplementation("catch (...)");
		gen.writeMethodImplementation("{");
		// don't catch errors which happen during serializing data back to the caller
		gen.writeMethodImplementation(indentation + "if (!remoting__requestSucceeded)");
		gen.writeMethodImplementation(indentation + "{");
		gen.writeMethodImplementation(indentation + "\tconst Poco::Exception exc(\"Unknown Exception\"s);");
		gen.writeMethodImplementation(indentation + "\tremoting__trans.reportException(\"" + pFunc->fullName() + "\"s, exc);");
		gen.writeMethodImplementation(indentation + "\tPoco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);");
		gen.writeMethodImplementation(indentation + "\tremoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);");
		gen.writeMethodImplementation(indentation + "}");
		gen.writeMethodImplementation("}");
	}
}


void SkeletonGenerator::writePushAttributes(SkeletonGenerator* pGen,
											const Poco::CppParser::Function* pFunc,
											const ProxyGenerator::OrderedParameters& attrs,
											const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
											const std::string& indentation,
											CodeGenerator& gen)
{
	ProxyGenerator::OrderedParameters::const_iterator itOP = attrs.begin();
	ProxyGenerator::OrderedParameters::const_iterator itOPEnd = attrs.end();
	for (; itOP != itOPEnd; ++itOP)
	{
		poco_check_ptr (itOP->second.pParam);
		const Poco::CppParser::Parameter* pParam = itOP->second.pParam;
		std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itD = outParams.find(pParam->name());
		if (itD != outParams.end())
		{
			//ser.pushAttribute(ns, name);
			std::string code("remoting__ser.pushAttribute(");
			if (itOP->second.nameSpacePos < 0)
			{
				code.append(SkeletonGenerator::generateClassName(pGen->_pStructIn) +"::DEFAULT_NS, ");
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
			gen.writeMethodImplementation(indentation+code);
		}
	}
}


void SkeletonGenerator::writePrepareAttribute(SkeletonGenerator* pGen, const ProxyGenerator::OrderedParameters& attrs, const std::string& indentation, CodeGenerator& gen)
{
	ProxyGenerator::OrderedParameters::const_iterator itAttrs = attrs.begin();
	ProxyGenerator::OrderedParameters::const_iterator itAttrsEnd = attrs.end();
	for (; itAttrs != itAttrsEnd; ++itAttrs)
	{
		//deser.pushAttribute(ns, name, bool)
		std::string code("remoting__deser.pushAttribute(");
		if (itAttrs->second.nameSpacePos < 0)
		{
			code.append(SkeletonGenerator::generateClassName(pGen->_pStructIn) +"::DEFAULT_NS, ");
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


void SkeletonGenerator::writeTypeDeserializers(const Poco::CppParser::Function* pFunc,
											const ProxyGenerator::OrderedParameters& params,
											const std::string& indentation,
											CodeGenerator& gen)
{
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFunc, funcProps);

	ProxyGenerator::OrderedParameters::const_iterator itOP = params.begin();
	ProxyGenerator::OrderedParameters::const_iterator itOPEnd = params.end();

	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "out")
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
				gen.writeMethodImplementation(indentation + "remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
			}
			if (!format.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
			}
			if (!format.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
			}

			poco_check_ptr (itOP->second.pParam);
			const Poco::CppParser::Parameter* pParam = itOP->second.pParam;

			std::string retType(Poco::CodeGeneration::Utility::resolveType(pFunc->nameSpace(), pParam->declType()));
			std::string cntStr(Poco::NumberFormatter::format(itOP->second.namePos));
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(retType);
			bool enumMode = false;
			std::string enumBaseType;
			std::string codeLine;
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				enumMode = true;
				enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
				if (enumBaseType.empty()) enumBaseType = "int";
				gen.writeMethodImplementation(indentation + enumBaseType + " remoting__"+itOP->second.varName+"Tmp;");
				if (!itOP->second.mandatory)
					codeLine.append("if (");
			}
			codeLine.append("Poco::RemotingNG::TypeDeserializer<");
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
			codeLine.append("remoting__deser, ");
			if (enumMode)
			{
				codeLine.append("remoting__"+itOP->second.varName+"Tmp");
			}
			else
			{
				codeLine.append(itOP->second.varName);
			}
			if (enumMode && !itOP->second.mandatory)
				codeLine.append("))");
			else
				codeLine.append(");");
			gen.writeMethodImplementation(indentation+codeLine);
			if (enumMode)
			{
				std::string addIndent;
				if (!itOP->second.mandatory) addIndent = "\t";
				gen.writeMethodImplementation(indentation + addIndent + itOP->second.varName+" = static_cast<"+retType+">("+"remoting__"+itOP->second.varName+"Tmp);");
			}

			if (!format.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
			}
			if (!location.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
			}
			if (!contentType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
			}
			if (!length.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
			}
			if (!xsdType.empty())
			{
				gen.writeMethodImplementation(indentation + "remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
			}
		}
	}
}


void SkeletonGenerator::writeTypeSerializer(const Poco::CppParser::Function* pFunc,
											const ProxyGenerator::OrderedParameters& params,
											const std::map<std::string, const Poco::CppParser::Parameter*>& outParams,
											const std::string& indentation,
											bool isAttr,
											int funcNsIdx,
											CodeGenerator& gen)
{
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFunc, funcProps);

	ProxyGenerator::OrderedParameters::const_iterator itOP = params.begin();
	ProxyGenerator::OrderedParameters::const_iterator itOPEnd = params.end();
	int lastNS = funcNsIdx;
	for (; itOP != itOPEnd; ++itOP)
	{
		if (itOP->second.direction != "in")
		{
			poco_check_ptr (itOP->second.pParam);
			const Poco::CppParser::Parameter* pParam = itOP->second.pParam;
			std::map<std::string, const Poco::CppParser::Parameter*>::const_iterator itD = outParams.find(pParam->name());
			if (itD != outParams.end())
			{
				if (!isAttr && lastNS != itOP->second.nameSpacePos)
				{
					if (lastNS != funcNsIdx)
					{
						gen.writeMethodImplementation("remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
					}
					std::string code("remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[");
					code.append(Poco::NumberFormatter::format(itOP->second.nameSpacePos));
					code.append("]);");
					gen.writeMethodImplementation(indentation+code);
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
					gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, \"" + location + "\"s);");
				}
				if (!format.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, \"" + format + "\"s);");
				}
				if (!contentType.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, \"" + contentType + "\"s);");
				}
				if (!length.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
				}
				if (!xsdType.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
				}

				std::string serLine("Poco::RemotingNG::TypeSerializer<");
				std::string type(Poco::CodeGeneration::Utility::resolveType(pFunc->nameSpace(), itOP->second.pParam->declType()));
				Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(type);

				bool enumMode = false;
				std::string enumBaseType;
				if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
				{
					type = "int";
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
				gen.writeMethodImplementation(indentation+serLine);

				if (!format.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);");
				}
				if (!location.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);");
				}
				if (!contentType.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);");
				}
				if (!length.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LENGTH);");
				}
				if (!xsdType.empty())
				{
					gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);");
				}
			}
		}
	}
	if (!isAttr && lastNS != funcNsIdx)
	{
		gen.writeMethodImplementation(indentation + "remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);");
	}
}


void SkeletonGenerator::staticMembersInitializer(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
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


void SkeletonGenerator::checkForEventMembers(const Poco::CppParser::Struct* pStruct)
{
	return;
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
				//_events.push_back(pVar->name());
				_cppGen.addSrcIncludeFile("Poco/Delegate.h");
				// generate a serializer method for that member too
				// call methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& methodProperties)
				// convert the basicevent to a private function
				std::string funcDecl("void ");
				std::string fctName = ProxyGenerator::generateEventFunctionName(pVar->name());
				funcDecl.append(fctName);
				std::vector<std::string> templTypes = GenUtility::getResolvedInnerTemplateTypes(pStruct, varType);
				if (templTypes.size() != 1)
					throw Poco::InvalidArgumentException("Illegal remote event param: " + pVar->fullName());
				std::string paramDecl = templTypes[0];
				paramDecl.append("& ");
				paramDecl.append (" var");
				{
					Poco::CppParser::Function* pFunc = new Poco::CppParser::Function(funcDecl, _pStruct);
					Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter(paramDecl, 0);
					pFunc->addParameter(pParam);
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
					pFunc->setAttributes(funcAttr);
					pFunc->setAccess(Poco::CppParser::Symbol::ACC_PRIVATE);
					methodStart(pFunc, methodProperties);
					methodEnd(pFunc, methodProperties);
				}
			}
		}
	}
}
