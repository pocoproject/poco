//
// XSDGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "XSDGenerator.h"
#include "GenUtility.h"
#include "SerializerGenerator.h"
#include "ProxyGenerator.h"
#include "WSDLGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/SAX/AttributesImpl.h"
#include "Poco/NumberFormatter.h"
#include <stack>
#include <cctype>


using Poco::CodeGeneration::Utility;
using Poco::CodeGeneration::GeneratorEngine;
using namespace Poco::XML;


const Poco::XML::XMLString XSDGenerator::SCHEMA_NS("http://www.w3.org/2001/XMLSchema");
const Poco::XML::XMLString XSDGenerator::SCHEMALOCATION("schemaLocation");
const Poco::XML::XMLString XSDGenerator::TARGETNAMESPACE("targetNamespace");
const Poco::XML::XMLString XSDGenerator::SCHEMA("schema");
const Poco::XML::XMLString XSDGenerator::XMLNS("xmlns");
const Poco::XML::XMLString XSDGenerator::EMPTYSTRING("");
const Poco::XML::XMLString XSDGenerator::COMPLEXTYPE("complexType");
const Poco::XML::XMLString XSDGenerator::ELEMENT("element");
const Poco::XML::XMLString XSDGenerator::SEQUENCE("sequence");
const Poco::XML::XMLString XSDGenerator::NAME("name");
const Poco::XML::XMLString XSDGenerator::TYPE("type");
const Poco::XML::XMLString XSDGenerator::NILLABLE("nillable");
const Poco::XML::XMLString XSDGenerator::MINOCCURS("minOccurs");
const Poco::XML::XMLString XSDGenerator::MAXOCCURS("maxOccurs");
const Poco::XML::XMLString XSDGenerator::UNBOUNDED("unbounded");
const Poco::XML::XMLString XSDGenerator::COMPLEXCONTENT("complexContent");
const Poco::XML::XMLString XSDGenerator::BASE("base");
const Poco::XML::XMLString XSDGenerator::EXTENSION("extension");
const Poco::XML::XMLString XSDGenerator::SCHEMA_PREFIX("xsd");
const Poco::XML::XMLString XSDGenerator::USE("use");
const Poco::XML::XMLString XSDGenerator::REQUIRED("required");
const Poco::XML::XMLString XSDGenerator::ATTRIBUTE("attribute");
const Poco::XML::XMLString XSDGenerator::REQUEST("Request");
const Poco::XML::XMLString XSDGenerator::HEADER("Header");
const Poco::XML::XMLString XSDGenerator::ELEMENTFORMDEFAULT("elementFormDefault");


XSDGenerator::XSDGenerator(Poco::CodeGeneration::CppGenerator& cppGen, const std::string& targetNamespace, Poco::XML::ContentHandler& xsdOut, bool documentStarted):
	AbstractGenerator(cppGen),
	_ns(nameSpace()),
	_xsdOut(xsdOut),
	_targetNamespace(targetNamespace),
	_documentStarted(documentStarted)
{
}


XSDGenerator::~XSDGenerator()
{
}


std::string XSDGenerator::generateFileName(const Poco::CppParser::Struct* pStruct)
{
	return generateFileName(pStruct->name());
}


std::string XSDGenerator::generateFileName(const std::string& structName)
{
	return structName + ".xsd";
}


void XSDGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStructIn = pStruct;
	if (!_documentStarted)
	{
		_xsdOut.startDocument();
		_xsdOut.startPrefixMapping(WSDLGenerator::TYPE_PREFIX, _targetNamespace);
	}
	_xsdOut.startPrefixMapping(SCHEMA_PREFIX, SCHEMA_NS);

	AttributesImpl attr;
	attr.addAttribute(EMPTYSTRING, TARGETNAMESPACE, TARGETNAMESPACE, EMPTYSTRING, _targetNamespace);
	attr.addAttribute(EMPTYSTRING, ELEMENTFORMDEFAULT, ELEMENTFORMDEFAULT, EMPTYSTRING, "qualified");
	_xsdOut.startElement(SCHEMA_NS, SCHEMA, SCHEMA, attr);
}


void XSDGenerator::structEnd()
{
	// include all types of all superclasses for simplicity in one single xsd file
	handleParentFunctions(_pStructIn);
	exportElements();
	_xsdOut.endElement(SCHEMA_NS, SCHEMA, SCHEMA);
	if (!_documentStarted)
	{
		_xsdOut.endPrefixMapping("");
		_xsdOut.endDocument();
	}
}


void XSDGenerator::exportElements()
{
	ExportedTypes::const_iterator it = _exportElementTypes.begin();
	ExportedTypes::const_iterator itEnd = _exportElementTypes.end();
	for (; it != itEnd; ++it)
	{
		//<element name="xy" type = "prefix:xy"/>
		AttributesImpl attr;
		attr.addAttribute(EMPTYSTRING, NAME, NAME, EMPTYSTRING, it->first);
		attr.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, WSDLGenerator::TYPE_PREFIX+":"+ it->second);
		_xsdOut.startElement(SCHEMA_NS, ELEMENT, ELEMENT, attr);
		_xsdOut.endElement(SCHEMA_NS, ELEMENT, ELEMENT);
	}
	_exportElementTypes.clear();

	SoapHeaderTypes::const_iterator itS = _exportSoapHeaderTypes.begin();
	SoapHeaderTypes::const_iterator itSEnd = _exportSoapHeaderTypes.end();
	for (; itS != itSEnd; ++itS)
	{
		//<element name="xy" type = "prefix:xy"/>
		AttributesImpl attr;
		attr.addAttribute(EMPTYSTRING, NAME, NAME, EMPTYSTRING, itS->first);
		std::string type = resolveType(itS->second);
		if (AbstractGenerator::BUILTIN_TYPES.find(type) != AbstractGenerator::BUILTIN_TYPES.end())
		{
			type = mapToSchemaType(type);
			attr.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, XSDGenerator::SCHEMA_PREFIX+":"+ type);
		}
		else
		{
			attr.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, WSDLGenerator::TYPE_PREFIX+":"+ itS->second);
		}
		_xsdOut.startElement(SCHEMA_NS, ELEMENT, ELEMENT, attr);
		_xsdOut.endElement(SCHEMA_NS, ELEMENT, ELEMENT);
	}
}


void XSDGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	CodeGenerator::Properties funcProps(properties);
	GeneratorEngine::parseProperties(pFuncOld, funcProps);
	if (!GenUtility::isRemoteMethod(pFuncOld, funcProps)) return;

	// get parameters of the function, analyze if one of the parameters has a complex type
	std::vector<const Poco::CppParser::Struct*> detectedTypes;
	detectTypes(pFuncOld, detectedTypes);
	generateTypes(detectedTypes);

	// now create the Request: it is easiest to convert the Function to a struct
	if (pFuncOld->isMethod())
	{
		Poco::CppParser::Struct* pRequest = convertToStruct(pFuncOld, false);
		std::string typeName = generateComplexType(pRequest);
		std::string elemName = GenUtility::getMethodName(pFuncOld);
		_exportElementTypes[elemName] = typeName;
	}

	std::vector<const Poco::CppParser::Parameter*> outParams;
	detectOutParams(pFuncOld, outParams);
	if (pFuncOld->isMethod())
	{
		Poco::CppParser::Struct* pResponse = convertToStruct(pFuncOld, true);
		std::string name = generateComplexType(pResponse);
		_exportElementTypes[name] = name;
	}
}


void XSDGenerator::detectTypes(const Poco::CppParser::Function* pFuncOld, std::vector<const Poco::CppParser::Struct*>& detectedTypes)
{
	if (pFuncOld->isConstructor() || pFuncOld->isDestructor())
		return;
	detectedTypes.clear();
	Poco::CppParser::Function::Iterator it = pFuncOld->begin();
	Poco::CppParser::Function::Iterator itEnd = pFuncOld->end();
	for (; it != itEnd; ++it)
	{
		const Poco::CppParser::Parameter* pParam = *it;
		std::string aType = resolveType(pParam->declType());
		Poco::trimInPlace(aType);
		if (GenUtility::isPtrType(aType))
		{
			std::vector<std::string> types = GenUtility::getInnerTemplateTypes(aType);
			if (types.size() != 1)
				throw CodeGenerationException("Unsupported template type", pParam->declType());
			aType = types[0];
		}
		std::stack<std::string> types;
		types.push(aType);
		while (!types.empty())
		{
			std::string type = types.top();
			types.pop();
			if (GenUtility::isTemplateType(type))
			{
				std::vector<std::string> itypes = GenUtility::getInnerTemplateTypes(type);
				if (itypes.size() != 1)
					throw CodeGenerationException("Unsupported template type", pParam->declType());
				types.push(itypes[0]);
			}
			else
			{
				type = resolveType(type);
				if (type.find("std::") != 0)
				{
					if (AbstractGenerator::BUILTIN_TYPES.find(type) == AbstractGenerator::BUILTIN_TYPES.end())
					{
						// we need a schema for the type! first find the class!
						const Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(type);
						const Poco::CppParser::Struct* pType = dynamic_cast<const Poco::CppParser::Struct*>(pSym);
						if (!pSym)
						{
							throw CodeGenerationException("Missing type info: " + type);
						}
						if (pType == 0)
						{
							if (!pSym || pSym->kind() != Poco::CppParser::Symbol::SYM_ENUM) // treat enum as builtin type
								throw CodeGenerationException("Missing class", type);
						}
						else detectedTypes.push_back(pType);
					}
				}
			}
		}
	}
}


void XSDGenerator::detectOutParams(const Poco::CppParser::Function* pFuncOld, std::vector<const Poco::CppParser::Parameter*>& outParams)
{
	// we don't return soapHeader params
	outParams.clear();
	Poco::CppParser::Function::Iterator it = pFuncOld->begin();
	Poco::CppParser::Function::Iterator itEnd = pFuncOld->end();
	for (; it != itEnd; ++it)
	{
		const Poco::CppParser::Parameter* pParam = *it;
		if (isOutParameter(pParam) && !GenUtility::getIsInHeader(pFuncOld, pParam))
		{
			outParams.push_back(pParam);
		}
	}
}


bool XSDGenerator::isOutParameter(const Poco::CppParser::Parameter* pParam)
{
	return ((pParam->isReference()||pParam->isPointer()) && !pParam->isConst());
}


Poco::CppParser::Struct* XSDGenerator::convertToStruct(const Poco::CppParser::Function* pFunc, bool isResponse)
{
	/// a type created by function parameters can never extend any other type
	std::string inputDataType;
	if (isResponse)
		inputDataType = GenUtility::getReplyMethodName(pFunc);
	else
		inputDataType = GenUtility::getRequestMethodName(pFunc);

	std::string decl(Poco::CodeGeneration::Utility::CLASS);
	decl.append(" ");
	decl.append(inputDataType);

	Poco::CppParser::Struct* pStruct = new Poco::CppParser::Struct(decl, true, &_ns);

	// now add for each param a set and get method,
	// also add a variable for each param (set the docu!!!!)
	// don't forget the return value!
	// return parameter
	std::string rdecl = pFunc->getReturnParameter();
	if (!rdecl.empty() && rdecl != "void" && isResponse)
	{
		// if we have an inline type we must fetch the struct for that type
		// and add all children of that type as direct members of pStruct
		// or simply inherit from it :-)
		bool isInline = GenUtility::getInlineReturnParam(pFunc);
		if (isInline)
		{
			std::string fullResType = GenUtility::getResolvedReturnParameterType(pFunc->nameSpace(), pFunc);
			std::string resType(fullResType);
			//restype can either be a native type or a complex type which we serialize, or it can be hidden inside a template like vector, set,... SharedPtr
			bool isPtrType = GenUtility::isPtrType(resType);
			if (isPtrType)
			{
				std::vector<std::string> inn = GenUtility::getInnerTemplateTypes(resType);
				poco_assert (inn.size() == 1);
				resType = inn[0];
			}
			bool isVectorOrNullable = GenUtility::isVectorType(resType) || GenUtility::isNullableType(resType) || GenUtility::isOptionalType(resType);
			if (isVectorOrNullable)
			{
				throw Poco::LogicException(pFunc->fullName() + ": For an inlined data type maxOccurs must be 1! No vector/set/multiset allowed");
			}
			if (AbstractGenerator::BUILTIN_TYPES.find(resType) != AbstractGenerator::BUILTIN_TYPES.end())
			{
				throw Poco::LogicException(pFunc->fullName() + ": Only complex data types can be inlined!");
			}

			// now get the class definition for that file
			Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(resType);
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_STRUCT)
			{
				Poco::CppParser::Struct* pRetStruct = static_cast<Poco::CppParser::Struct*>(pSym);
				// deep copy members pRetStruct
				deepCopyMembers(pRetStruct, pStruct);
			}
			else
				throw Poco::LogicException("Failed to find data type for inlined return value in method: " + pFunc->fullName());
		}
		else
		{
			std::string retParamName (GenUtility::getReturnParameterName(pFunc));
			rdecl.append(" " + retParamName);
			Poco::CppParser::Parameter p(rdecl, 0);
			Poco::CppParser::Variable* pVar = createVariable(&p, pStruct);
			Poco::CppParser::Attributes attr;
			attr["order"] = "0";
			pVar->setAttributes(attr);
			//pVar->setDocumentation("///[[order=0]]");
			createGetFct(&p, pStruct);
			createSetFct(&p, pStruct);
		}
	}
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	Poco::UInt32 unknownOrderOffset = 0xffff0000;
	CodeGenerator::Properties funcProps;
	GeneratorEngine::parseProperties(pFunc, funcProps);
	for (; it != itEnd; ++it)
	{
		std::string direction;
		CodeGenerator::Properties::iterator itProp = funcProps.find("$" + (*it)->name());
		if (itProp != funcProps.end())
		{
			CodeGenerator::Properties paramProps; // we need direction
			GeneratorEngine::parseElementProperties(itProp->second, paramProps);
			GeneratorEngine::getStringProperty(paramProps, Utility::DIRECTION, direction);
		}

		//isOutParam excludes soapheader already, do the same for replies
		bool inSoapHeader = GenUtility::getIsInHeader(pFunc, *it);
		if ( (isResponse && isOutParameter(*it) && direction != "in") || (!isResponse && !inSoapHeader && direction != "out"))
		{
			Poco::CppParser::Variable* pVar = createVariable(*it, pStruct);
			addVarDocu(pVar, pFunc, (*it)->name(), unknownOrderOffset++);
			createGetFct(*it, pStruct);
			createSetFct(*it, pStruct);
		}
		else if (inSoapHeader)
		{
			// Note: we must export for each soapHeader an element that has the same name as the parameter!
			Poco::CppParser::Parameter* pParam = *it;
			const std::string& declType = pParam->declType();
			std::string name = GenUtility::getParameterName(pFunc, pParam);
			std::string aType = Poco::CodeGeneration::Utility::resolveType(pFunc->nameSpace(), declType);
			if (aType == declType)
			{
				_exportSoapHeaderTypes.insert (std::make_pair(name, aType));
			}
			else
			{
				Poco::CppParser::Symbol* pSym = pFunc->nameSpace()->lookup(aType);
				poco_assert (pSym);
				_exportSoapHeaderTypes.insert (std::make_pair(name, pSym->name()));
			}
		}
	}
	return pStruct;
}


void XSDGenerator::deepCopyMembers(const Poco::CppParser::Struct* pIn, Poco::CppParser::Struct* pOut)
{
	// deep copy all variables and functions, exluding constructor/destructor
	Poco::CppParser::Struct::BaseIterator itB = pIn->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pIn->baseEnd();
	for (; itB!= itBEnd; ++itB)
	{
		if (itB->pClass)
			deepCopyMembers(itB->pClass, pOut);
	}
	// handle members
	Poco::CppParser::NameSpace::SymbolTable members;
	pIn->variables(members);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = members.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = members.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		Poco::CppParser::Variable* pCpyVar = new Poco::CppParser::Variable(pVar->declaration(), pOut);
		pCpyVar->setAccess(pVar->getAccess());
		pCpyVar->setAttributes(pVar->getAttributes());
	}
	// handle methods
	members.clear();
	Poco::CppParser::Struct::Functions fcts;
	pIn->methods(Poco::CppParser::Symbol::ACC_PUBLIC, fcts);
	Poco::CppParser::Struct::Functions::const_iterator itt = fcts.begin();
	Poco::CppParser::Struct::Functions::const_iterator ittEnd = fcts.end();
	for (; itt != ittEnd; ++itt)
	{
		Poco::CppParser::Function* pFct = static_cast<Poco::CppParser::Function*>(*itt);
		if (!pFct->isConstructor() && ! pFct->isDestructor())
		{
			Poco::CppParser::Function* pCpyFct = new Poco::CppParser::Function(pFct->declaration(), pOut);
			pCpyFct->setAccess (pFct->getAccess());
			addDocumentation(pFct, pCpyFct);
			pCpyFct->setAttributes(pFct->getAttributes());
			if (pFct->isConst())
				pCpyFct->makeConst();

			Poco::CppParser::Function::Iterator itF = pFct->begin();
			Poco::CppParser::Function::Iterator itFEnd = pFct->end();

			for (; itF != itFEnd; ++itF)
			{
				std::string decl = Poco::CodeGeneration::Utility::resolveParamDecl(pIn, *itF);
				if ((*itF)->hasDefaultValue())
				{
					decl.append(" = ");
					decl.append(Poco::CodeGeneration::Utility::resolveType(_pStructIn, (*itF)->declType()));
					decl.append("(");
					decl.append((*itF)->defaultValue());
					decl.append(")");
				}

				Poco::CppParser::Parameter* pParam = new Poco::CppParser::Parameter(decl, pCpyFct);

				pCpyFct->addParameter(pParam);
			}
		}
	}
}


void XSDGenerator::addVarDocu(Poco::CppParser::Variable* pVar, const Poco::CppParser::Function* pFunc, const std::string& paramName, Poco::UInt32 order)
{
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);
	std::string elemVal;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + paramName, elemVal);
	Poco::CodeGeneration::CodeGenerator::Properties varProps;
	Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemVal, varProps);
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator it = varProps.begin();
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator itEnd = varProps.end();
	Poco::CppParser::Attributes attr;
	for (; it != itEnd; ++it)
	{
		attr[it->first] = it->second;
	}
	if (!attr.has(Utility::ORDER))
	{
		attr[Utility::ORDER] = Poco::NumberFormatter::format(order);
	}
	pVar->setAttributes(attr);
}


Poco::CppParser::Parameter* XSDGenerator::createParameter(const Poco::CppParser::Parameter* pCopy, Poco::CppParser::Function* pConstructorFull)
{
	const std::string& declType = pCopy->declType();

	std::string decl(createParameterTypeDecl(declType));
	decl.append("_" + pCopy->name());

	return new Poco::CppParser::Parameter(decl, pConstructorFull);
}


std::string XSDGenerator::createParameterTypeDecl(const std::string& typeDecl)
{
	std::string aType = resolveType(typeDecl);
	std::string decl;
	if (AbstractGenerator::BUILTIN_TYPES.find(aType) != AbstractGenerator::BUILTIN_TYPES.end())
	{
		// simply use declType
		decl = typeDecl + " ";
	}
	else
	{
		decl = "const ";
		decl.append(typeDecl);
		decl.append("& ");
	}
	return decl;
}


Poco::CppParser::Variable* XSDGenerator::createVariable(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct)
{
	// a variable is always a non const, non-ref, non-pointer
	std::string decl(pParam->declType());
	if (pParam->isPointer())
		decl.append("*");
	decl.append(" _");
	decl.append(pParam->name());
	Poco::CppParser::Variable* pRet = new Poco::CppParser::Variable(decl, pStruct);
	pRet->setAccess(Poco::CppParser::Variable::ACC_PRIVATE);
	return pRet;
}


Poco::CppParser::Function* XSDGenerator::createGetFct(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct)
{
	std::string methodName("get");
	methodName.append(pParam->name());
	methodName[3] = (char)std::toupper(methodName[3]);

	// we return either by value or by const &
	std::string decl(createParameterTypeDecl(pParam->declType()));
	decl.append(" ");
	decl.append(methodName);
	Poco::CppParser::Function* pFct = new Poco::CppParser::Function(decl, pStruct);
	pFct->makeConst();
	pFct->makeInline();

	return pFct;
}


Poco::CppParser::Function* XSDGenerator::createSetFct(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct)
{
	std::string methodName("set");
	methodName.append(pParam->name());
	methodName[3] = (char)std::toupper(methodName[3]);

	// we return either by value or by const &
	std::string decl("void ");
	decl.append(methodName);
	Poco::CppParser::Function* pFct = new Poco::CppParser::Function(decl, pStruct);
	pFct->makeInline();
	// add the one parameter
	Poco::CppParser::Parameter* pParam2 = createParameter(pParam, pFct);
	pFct->addParameter(pParam2);

	return pFct;
}


void XSDGenerator::generateTypes(const std::vector<const Poco::CppParser::Struct*>& detectedTypes)
{
	std::vector<const Poco::CppParser::Struct*>::const_iterator itT = detectedTypes.begin();
	std::vector<const Poco::CppParser::Struct*>::const_iterator itTEnd = detectedTypes.end();
	for (; itT != itTEnd; ++itT)
		generateComplexType(*itT);
}


void XSDGenerator::methodEnd(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
}


void XSDGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
}


std::vector<std::string> XSDGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;

	return bases;
}


std::string XSDGenerator::generateComplexTypeName(const Poco::CppParser::Struct* pStruct)
{
	poco_assert_dbg (pStruct);
	std::string newClassName(pStruct->name());
	Poco::CppParser::NameSpace* pNS = pStruct->nameSpace();
	while (pNS)
	{
		Poco::CppParser::Struct* pOuter = dynamic_cast<Poco::CppParser::Struct*>(pNS);
		if (pOuter)
		{
			newClassName = pOuter->name() + newClassName;
			pNS = pOuter->nameSpace();
		}
		else
			pNS = 0;
	}

	return newClassName;
}


std::string XSDGenerator::generateComplexType(const Poco::CppParser::Struct* pType)
{
	std::string fullName = pType->fullName();
	std::string complexName = generateComplexTypeName(pType);
	if (_handledTypes.find(fullName) != _handledTypes.end())
		return complexName;
	_handledTypes.insert(fullName);

	std::vector<const Poco::CppParser::Struct*> detectedTypes;

	// types are always a sequence, a vector has always infinite as limit
	//<complexType name="someName">
	//    <sequence>
	//          <element name="whereExample" type="string" minOccurs="0" maxOccurs="unbounded"/>
	const Poco::XML::AttributesImpl emptyAttr;
	Poco::XML::AttributesImpl attr;
	attr.addAttribute(EMPTYSTRING, NAME, NAME, EMPTYSTRING, complexName);

	_xsdOut.startElement(SCHEMA_NS, COMPLEXTYPE, COMPLEXTYPE, attr);
	SerializerGenerator::VarGet varMatch;
	SerializerGenerator::OrderedVars attrs;
	SerializerGenerator::OrderedVars elems;
	SerializerGenerator::matchVarsWithFunctions(pType, varMatch);
	SerializerGenerator::doElemAttrSplit(varMatch, attrs, elems);

	Poco::CppParser::Struct::BaseIterator itB = pType->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pType->baseEnd();
	// mutiple inheritance is not supported (we could do it by defining for all members for all structs a group
	// and for each multiple inheritance we have to define another group which groups together these groups :-)
	// Anyway, too much work for a rarely used feature
	bool usesInheritance = false;
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct::Base& b = *itB;
		if (GenUtility::mustBeSerializable(b.name))
		{
			if (!b.pClass)
				throw CodeGenerationException("Missing class", b.name);
			detectedTypes.push_back(b.pClass);
			usesInheritance = true;
			_xsdOut.startElement(SCHEMA_NS, COMPLEXCONTENT, COMPLEXCONTENT, emptyAttr);
			Poco::XML::AttributesImpl extAttr;
			extAttr.addAttribute(EMPTYSTRING, BASE, BASE, EMPTYSTRING, WSDLGenerator::TYPE_PREFIX+":"+generateComplexTypeName(b.pClass));
			_xsdOut.startElement(SCHEMA_NS, EXTENSION, EXTENSION, extAttr);
			break;
		}
	}

	// check for header attribute:
	// remove the header stuff from the type
	// continue writing the body type
	std::vector<Poco::XML::AttributesImpl> soapHeaderElements;
	std::vector<Poco::XML::AttributesImpl> soapBodyElements;

	SerializerGenerator::OrderedVars::const_iterator itElems = elems.begin();
	SerializerGenerator::OrderedVars::const_iterator itElemsEnd = elems.end();
	for (; itElems != itElemsEnd; ++itElems)
	{
		std::string elemName;
		bool isVector = false;
		bool isMandatory = true;
		bool isNullable = false;
		std::string resolvedType;
		std::string soapHeader;
		Poco::CodeGeneration::CodeGenerator::Properties varProps;
		Poco::CodeGeneration::GeneratorEngine::parseProperties(itElems->second->second.first, varProps);
		detectProperties(itElems->second->second.first, isVector, isMandatory, isNullable, elemName, resolvedType);
		const Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
		if (pSym == 0)
		{
			pSym = pType->lookup(resolvedType); // for inner classes
			if (pSym != 0) // 0 is ok for primitve types
				resolvedType = pSym->fullName();
		}
		if (GenUtility::isPtrType(resolvedType) || GenUtility::isNullableType(resolvedType) || GenUtility::isOptionalType(resolvedType))
		{
			std::vector<std::string> tt = GenUtility::getInnerTemplateTypes(resolvedType);
			if (tt.size() == 1 || (tt.size() == 2 && GenUtility::isNullableType(tt[0])))
			{
				resolvedType = tt[tt.size() - 1];
				pSym = pType->lookup(resolvedType);
				if (!pSym)
					pSym = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
			}
			else if (tt.size() > 0)
			{
				throw CodeGenerationException("Unsupported template type", resolvedType + "<" + tt[0] + ">");
			}
			else throw CodeGenerationException("Unparsable template type", resolvedType);
		}
		Poco::XML::AttributesImpl elemAttrs;
		elemAttrs.addAttribute(EMPTYSTRING, NAME, NAME, EMPTYSTRING, elemName);
		if (!isMandatory || isVector)
			elemAttrs.addAttribute(EMPTYSTRING, MINOCCURS, MINOCCURS, EMPTYSTRING, "0");
		if (isVector)
			elemAttrs.addAttribute(EMPTYSTRING, MAXOCCURS, MAXOCCURS, EMPTYSTRING, UNBOUNDED);
		if (isNullable)
			elemAttrs.addAttribute(EMPTYSTRING, NILLABLE, NILLABLE, EMPTYSTRING, "true");

		if (GenUtility::isVectorType(resolvedType))
		{
			// we have a matrix
			throw CodeGenerationException("A collection of collections is not supported with an XML schema. Create a class that holds the inner collection, and then create a collection of that class.");
		}

		if (resolvedType.find("std::") == 0 || AbstractGenerator::BUILTIN_TYPES.find(resolvedType) != AbstractGenerator::BUILTIN_TYPES.end())
		{
			const std::string& xsdType = mapToSchemaType(resolvedType);
			elemAttrs.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, SCHEMA_PREFIX+":"+xsdType);
		}
		else
		{
			const Poco::CppParser::Struct* pResType = dynamic_cast<const Poco::CppParser::Struct*>(pSym);
			if (pResType == 0)
			{
				if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
				{
					// map to integer
					std::string enumBaseType = static_cast<const Poco::CppParser::Enum*>(pSym)->baseType();
					if (enumBaseType.empty()) enumBaseType = "int";
					const std::string& xsdType = mapToSchemaType(enumBaseType);
					elemAttrs.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, SCHEMA_PREFIX+":"+xsdType);
				}
				else
					throw CodeGenerationException("Missing class", resolvedType);
			}
			else
			{
				detectedTypes.push_back(pResType);
				elemAttrs.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, WSDLGenerator::TYPE_PREFIX+":"+XSDGenerator::generateComplexTypeName(pResType));
			}
		}
		soapBodyElements.push_back(elemAttrs);
	}

	if (!soapBodyElements.empty())
	{
		_xsdOut.startElement(SCHEMA_NS, SEQUENCE, SEQUENCE, emptyAttr);
		std::vector<Poco::XML::AttributesImpl>::const_iterator it = soapBodyElements.begin();
		std::vector<Poco::XML::AttributesImpl>::const_iterator itEnd = soapBodyElements.end();
		for (; it != itEnd; ++it)
		{
			_xsdOut.startElement(SCHEMA_NS, ELEMENT, ELEMENT, *it);
			_xsdOut.endElement(SCHEMA_NS, ELEMENT, ELEMENT);
		}
		_xsdOut.endElement(SCHEMA_NS, SEQUENCE, SEQUENCE);
	}

	// attrs
	SerializerGenerator::OrderedVars::const_iterator itAttrs = attrs.begin();
	SerializerGenerator::OrderedVars::const_iterator itAttrsEnd = attrs.end();
	for (; itAttrs != itAttrsEnd; ++itAttrs)
	{
		std::string attrName;
		bool isVector = false;
		bool isMandatory = true;
		bool isNullable = false;
		std::string resolvedType;
		detectProperties(itAttrs->second->second.first, isVector, isMandatory, isNullable, attrName, resolvedType);
		if (isVector)
			throw CodeGenerationException("A vector cannot be serialized as an attribute.");
		if (isNullable)
			throw CodeGenerationException("A Nullable cannot be serialized as an attribute.");

		const Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
		if (pSym == 0)
		{
			pSym = pType->lookup(resolvedType); // for inner classes
			if (pSym != 0) // 0 is ok for primitve types
				resolvedType = pSym->fullName();
		}
		Poco::XML::AttributesImpl attrAttrs;
		attrAttrs.addAttribute(EMPTYSTRING, NAME, NAME, EMPTYSTRING, attrName);
		if (isMandatory)
			attrAttrs.addAttribute(EMPTYSTRING, USE, USE, EMPTYSTRING, REQUIRED);
		Poco::replaceInPlace(resolvedType, "mutable", "");
		Poco::trimInPlace(resolvedType);
		if (resolvedType.find("std::") == 0 || AbstractGenerator::BUILTIN_TYPES.find(resolvedType) != AbstractGenerator::BUILTIN_TYPES.end())
		{
			// a simple type
			const std::string& xsdType = mapToSchemaType(resolvedType);
			attrAttrs.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, SCHEMA_PREFIX+":"+xsdType);
		}
		else
		{
			if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
			{
				// map to integer
				const std::string& xsdType = mapToSchemaType("int");
				attrAttrs.addAttribute(EMPTYSTRING, TYPE, TYPE, EMPTYSTRING, SCHEMA_PREFIX+":"+xsdType);
			}
			else throw CodeGenerationException("An attribute can only have a simple content.");
		}
		//<element name="whereExample" type="string" minOccurs="0" maxOccurs="unbounded"/>
		_xsdOut.startElement(SCHEMA_NS, ATTRIBUTE, ATTRIBUTE, attrAttrs);
		_xsdOut.endElement(SCHEMA_NS, ATTRIBUTE, ATTRIBUTE);
	}

	if (usesInheritance)
	{
		_xsdOut.endElement(SCHEMA_NS, EXTENSION, EXTENSION);
		_xsdOut.endElement(SCHEMA_NS, COMPLEXCONTENT, COMPLEXCONTENT);
	}
	_xsdOut.endElement(SCHEMA_NS, COMPLEXTYPE, COMPLEXTYPE);

	generateTypes(detectedTypes);
	return complexName;
}


const std::string& XSDGenerator::mapToSchemaType(const std::string& type)
{
	static std::map<std::string, std::string> typeMap(initTypeMapping());
	std::string aType = Poco::trim(type);

	if (aType.find("std::") == 0)
		aType = aType.substr(5);

	std::map<std::string, std::string>::const_iterator it = typeMap.find(aType);
	if (it == typeMap.end())
		throw CodeGenerationException("Failed to map type " + type + " to a schema type.");

	return it->second;
}


void XSDGenerator::detectProperties(const Poco::CppParser::Variable* pVar, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType)
{
	const std::string& varDecl = pVar->declType();

	std::string aType = varDecl;
	std::string tmp;
	poco_assert (!aType.empty());
	do
	{
		tmp = resolveType(aType);
		if (tmp != aType)
		{
			aType = tmp;
			tmp.clear();
		}
	}
	while (tmp != aType); // resolve recursive typedefs

	poco_assert (!aType.empty());
	if (GenUtility::isPtrType(aType))
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(aType);
		if (types.size() != 1)
			throw CodeGenerationException("Unsupported template type", varDecl);
		aType = types[0];
	}
	poco_assert (!aType.empty());
	isVector = GenUtility::isVectorType(aType);
	isNullable = GenUtility::isNullableType(aType);
	isMandatory = !GenUtility::isOptionalType(aType);
	bool isVectorOrOptional = isVector || !isMandatory || isNullable;
	if (isVectorOrOptional)
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(aType);
		if (types.size() == 1 || (types.size() == 2 && GenUtility::isNullableType(types[0])))
		{
			aType = types[types.size() - 1];
		}
		else throw CodeGenerationException("Unsupported template type", varDecl);
	}
	else if (GenUtility::isTemplateType(aType))
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(aType);
		if (types.size() != 1)
			throw CodeGenerationException("Unsupported template type", varDecl);
	}
	isMandatory = isMandatory || GenUtility::getIsMandatory(pVar);
	name = GenUtility::getVariableName(pVar);

	do
	{
		tmp = resolveType(aType);
		if (tmp != aType)
		{
			aType = tmp;
			tmp.clear();
		}
	}
	while (tmp != aType); // resolve recursive typedefs
	resolvedType = aType;
	poco_assert (!resolvedType.empty());
}


std::map<std::string, std::string> XSDGenerator::initTypeMapping()
{
	std::map<std::string, std::string> ret;
	ret.insert(std::make_pair("int", "int"));
	ret.insert(std::make_pair("signed int", "int"));
	ret.insert(std::make_pair("unsigned int", "unsignedInt"));
	ret.insert(std::make_pair("short", "short"));
	ret.insert(std::make_pair("signed short", "short"));
	ret.insert(std::make_pair("unsigned short", "unsignedShort"));
	ret.insert(std::make_pair("short int", "short"));
	ret.insert(std::make_pair("signed short int", "short"));
	ret.insert(std::make_pair("unsigned short int", "unsignedShort"));
	ret.insert(std::make_pair("long", "long"));
	ret.insert(std::make_pair("signed long", "long"));
	ret.insert(std::make_pair("unsigned long", "unsignedLong"));
	ret.insert(std::make_pair("long int", "long"));
	ret.insert(std::make_pair("signed long int", "long"));
	ret.insert(std::make_pair("unsigned long int", "unsignedLong"));
	ret.insert(std::make_pair("long long", "long"));
	ret.insert(std::make_pair("signed long long", "long"));
	ret.insert(std::make_pair("unsigned long long", "unsignedLong"));
	ret.insert(std::make_pair("long long int", "long"));
	ret.insert(std::make_pair("signed long long int", "long"));
	ret.insert(std::make_pair("unsigned long long int", "unsignedLong"));
	ret.insert(std::make_pair("char", "byte"));
	ret.insert(std::make_pair("signed char", "byte"));
	ret.insert(std::make_pair("unsigned char", "unsignedByte"));
	ret.insert(std::make_pair("bool", "boolean"));
	ret.insert(std::make_pair("Int8", "byte"));
	ret.insert(std::make_pair("UInt8", "unsignedByte"));
	ret.insert(std::make_pair("Int16", "short"));
	ret.insert(std::make_pair("UInt16", "unsignedShort"));
	ret.insert(std::make_pair("Int32", "int"));
	ret.insert(std::make_pair("UInt32", "unsignedInt"));
	ret.insert(std::make_pair("Int64", "long"));
	ret.insert(std::make_pair("UInt64", "unsignedLong"));
	ret.insert(std::make_pair("Poco::Int8", "byte"));
	ret.insert(std::make_pair("Poco::UInt8", "unsignedByte"));
	ret.insert(std::make_pair("Poco::Int16", "short"));
	ret.insert(std::make_pair("Poco::UInt16", "unsignedShort"));
	ret.insert(std::make_pair("Poco::Int32", "int"));
	ret.insert(std::make_pair("Poco::UInt32", "unsignedInt"));
	ret.insert(std::make_pair("Poco::Int64", "long"));
	ret.insert(std::make_pair("Poco::UInt64", "unsignedLong"));
	ret.insert(std::make_pair("float", "float"));
	ret.insert(std::make_pair("double", "double"));
	ret.insert(std::make_pair("long double", "double"));
	ret.insert(std::make_pair("Poco::DateTime", "dateTime"));
	ret.insert(std::make_pair("DateTime", "dateTime"));
	ret.insert(std::make_pair("Poco::LocalDateTime", "dateTime"));
	ret.insert(std::make_pair("LocalDateTime", "dateTime"));
	ret.insert(std::make_pair("Poco::Timestamp", "dateTime"));
	ret.insert(std::make_pair("Timestamp", "dateTime"));
	ret.insert(std::make_pair("Poco::Timespan", "long"));
	ret.insert(std::make_pair("Timespan", "long"));
	ret.insert(std::make_pair("string", "string"));
	ret.insert(std::make_pair("URI", "string"));
	ret.insert(std::make_pair("Poco::URI", "string"));
	ret.insert(std::make_pair("UUID", "string"));
	ret.insert(std::make_pair("Poco::UUID", "string"));
	ret.insert(std::make_pair("ULID", "string"));
	ret.insert(std::make_pair("Poco::ULID", "string"));
	ret.insert(std::make_pair("vector < char >", "base64Binary"));
	return ret;
}
