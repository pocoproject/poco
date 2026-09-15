//
// SerializerGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SerializerGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NullStream.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/LayeredConfiguration.h"
#include <cctype>


using namespace Poco::CodeGeneration;


SerializerGenerator::SerializationOrder SerializerGenerator::_serializationOrder(SerializerGenerator::SERIALIZE_AS_DECLARED);


SerializerGenerator::SerializerGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


SerializerGenerator::~SerializerGenerator()
{
}


void SerializerGenerator::setSerializationOrder(SerializationOrder order)
{
	_serializationOrder = order;
}


std::string SerializerGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
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

	newClassName.append("Serializer");
	return newClassName;
}


std::string SerializerGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return SerializerGenerator::generateClassName(pStruct);

	return ns + "::" + SerializerGenerator::generateClassName(pStruct);
}


void SerializerGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStructIn = pStruct;
	std::string newClassName(SerializerGenerator::generateClassName(pStruct));
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (!itB->pClass)
		{
			if (GenUtility::mustBeSerializable(itB->name))
			{
				std::string msg("While handling " + pStruct->fullName() + ": ");
				msg.append("Missing base class " + itB->name + ". Please add the header file defining the class to the code generator configuration and make sure the class is serializable.");
				throw CodeGenerationException(msg);
			}
		}
	}

	// we create the following class: template <> TypeSerializer<StructInputData>
	std::string decl("template <>\nclass TypeSerializer<");
	decl.append(pStruct->fullName());
	decl.append(">");

	_pStruct = new Poco::CppParser::Struct(decl, true, _pNs);

	//we need to provide the following public methods:
	// static void serialize(const std::string& elem, const T& value, Serializer& ser)
	// static void serializeImpl(const T& value, Serializer& ser)
	// T gets substituted by the concrete type

	Poco::CppParser::Function* pFuncRef = new Poco::CppParser::Function("static void serialize", _pStruct);
	addParameters(pFuncRef, pStruct->fullName());
	Poco::CppParser::Function* pFuncRef2 = new Poco::CppParser::Function("static void serializeImpl", _pStruct);
	addImplParameters(pFuncRef2, pStruct->fullName());

	// if we have attributes, we must add two additional methods
	if (hasAttributes(pStruct))
	{
		// static void serializeAttributes(const T& value, Serializer& ser)
		// static void prepareSerializeAttributes(const T& value, Serializer& ser)
		Poco::CppParser::Function* pFuncRef3 = new Poco::CppParser::Function("static void serializeAttributes", _pStruct);
		addImplParameters(pFuncRef3, pStruct->fullName());
		Poco::CppParser::Function* pFuncRef4 = new Poco::CppParser::Function("static void prepareSerializeAttributes", _pStruct);
		pFuncRef4->addParameter(new Poco::CppParser::Parameter("Serializer& ser", 0));
	}

	// we must include TypeSerializer, and the file of the struct we serialize
	_cppGen.addIncludeFile("Poco/RemotingNG/TypeSerializer.h");
	Poco::CodeGeneration::Utility::handleInclude(pStruct, _cppGen);

	Poco::Path aPath(pStruct->getFile());
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	std::string incFile = includePath(cfg, pStruct->nameSpace()->fullName());
	if (!incFile.empty())
	{
		aPath = Poco::Path(incFile);
		aPath.makeDirectory();
	}
	setDocumentation(pStruct, _pStruct);
	aPath.setFileName(newClassName+".h");
	_pStruct->setFile(aPath.toString());
	_pStruct->setPackage("Generated");

	// now include typeserializers for variables (public)
	Poco::CppParser::NameSpace::SymbolTable tbl;
	pStruct->variables(tbl);
	Poco::CppParser::NameSpace::SymbolTable::const_iterator it = tbl.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itEnd = tbl.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		// just a precaution: only include serializers for public types
		if (pVar->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC)
		{
			std::string varType = Utility::resolveType(pVar->nameSpace(), pVar->declType());
			Poco::CppParser::Symbol* pS = pVar->nameSpace()->lookup(varType);
			if (pS)
			{
				if (pS->kind() != Poco::CppParser::Symbol::SYM_ENUM)
					handleIncludeTypeSerializers(pVar, true, false);
			}
			else handleIncludeTypeSerializers(pVar, true, false);
		}
	}

	VarGet matches;
	matchVarsWithFunctionsRecursive(pStruct, matches);
	for (VarGet::iterator it = matches.begin(); it != matches.end(); ++it)
	{
		handleIncludeTypeSerializers(it->second.first, true, false);
	}
}


std::vector<std::string> SerializerGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// we extend from Skeleton or from a superclass skeleton
	std::vector<std::string> bases;
	return bases;
}


void SerializerGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("serialize", &SerializerGenerator::serializeCodeGen);
	e.registerCallback("serializeImpl", &SerializerGenerator::serializeImplCodeGen);
	e.registerCallback("serializeAttributes", &SerializerGenerator::serializeAttributesCodeGen);
	e.registerCallback("prepareSerializeAttributes", &SerializerGenerator::prepareSerializeAttributesCodeGen);

}


void SerializerGenerator::methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
}


void SerializerGenerator::addParameters(Poco::CppParser::Function* pFunc, const std::string& typeName)
{
	//serialize(const std::string& name, const T& value, RemotingNG::Serializer& ser)
	pFunc->addParameter(new Poco::CppParser::Parameter("const std::string& name", 0));
	std::string type("const ");
	type.append(typeName);
	type.append("&");
	type.append(" value");
	pFunc->addParameter(new Poco::CppParser::Parameter(type, 0));
	pFunc->addParameter(new Poco::CppParser::Parameter("Serializer& ser", 0));
}


void SerializerGenerator::addImplParameters(Poco::CppParser::Function* pFunc, const std::string& typeName)
{
	//serializeImpl(const T& value, RemotingNG::Serializer& ser)
	std::string type("const ");
	type.append(typeName);
	type.append("&");
	type.append(" value");
	pFunc->addParameter(new Poco::CppParser::Parameter(type, 0));
	pFunc->addParameter(new Poco::CppParser::Parameter("Serializer& ser", 0));
}


void SerializerGenerator::serializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SerializerGenerator* pSer = dynamic_cast<SerializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pDataType, structProps);
	std::string defaultNS;
	bool hasDefaultNS = GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, defaultNS);
	if (hasDefaultNS && !defaultNS.empty())
	{
		std::string code("static const std::string REMOTING__NAMESPACE(\"");
		code.append(defaultNS);
		code.append("\"s);");
		gen.writeMethodImplementation(code);
		gen.writeMethodImplementation("ser.registerNamespace(REMOTING__NAMESPACE);");
	}

	bool hasAttr = hasAttributes(pDataType);
	if (hasAttr)
		gen.writeMethodImplementation("prepareSerializeAttributes(ser);");
	gen.writeMethodImplementation("ser.serializeStructBegin(name);");
	if (hasDefaultNS && !defaultNS.empty())
	{
		gen.writeMethodImplementation("ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);");
	}
	if (hasAttr)
		gen.writeMethodImplementation("serializeAttributes(value, ser);");
	gen.writeMethodImplementation("serializeImpl(value, ser);");
	if (hasDefaultNS && !defaultNS.empty())
		gen.writeMethodImplementation("ser.popProperty(SerializerBase::PROP_NAMESPACE);");
	gen.writeMethodImplementation("ser.serializeStructEnd(name);");
}


void SerializerGenerator::serializeImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SerializerGenerator* pSer = dynamic_cast<SerializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	pSer->handleSuperClassCalls(pDataType, &SerializerGenerator::serializeImplCodeGenImpl, false, gen);
	pSer->serializeImplCodeGenImpl(pDataType, gen, "");
}


void SerializerGenerator::serializeImplCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// now split up in two multimaps: one containing attrs only, the other containing elems only
	OrderedVars attrs;
	OrderedVars elems;
	doElemAttrSplit(matches, attrs, elems);
	std::vector<std::string> elemCodeLines;
	std::vector<std::string> namespaces;
	std::map<std::string, int> namespacesMap; // to avoid duplicate namespace entries, the int is the pos in namespace

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pDataType, structProps);
	std::string defaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, defaultNS);
	namespaces.push_back(defaultNS);
	namespacesMap.insert(std::make_pair(defaultNS, static_cast<int>(namespaces.size()) - 1));
	// the first line contains a static string array containing names
	if (!elems.empty())
	{
		std::string staticVarNames(Poco::format("static const std::string REMOTING__NAMES%s[] = {", suffix));
		// after the names will be all used namespaces
		int firstNSPos = static_cast<int>(elems.size());
		int curNamesPos = 0;

		std::string lastPushedNS(defaultNS);
		OrderedVars::const_iterator itElem = elems.begin();
		OrderedVars::const_iterator itElemEnd = elems.end();
		for (; itElem != itElemEnd; ++itElem, ++curNamesPos)
		{
			appendStaticVarName(getVariableName(itElem->second), staticVarNames);
			VarGet::const_iterator it = itElem->second;
			CodeGenerator::Properties attrProps(structProps);
			GeneratorEngine::parseProperties(it->second.first, attrProps);
			std::string ns;
			GeneratorEngine::getStringProperty(attrProps, Utility::NAMESPACE, ns);
			std::pair<std::map<std::string, int>::iterator, bool> in = namespacesMap.insert(std::make_pair(ns, static_cast<int>(namespaces.size())));
			if (in.second)
				namespaces.push_back(ns);

			if (ns != lastPushedNS)
			{
				if (lastPushedNS != defaultNS)
					elemCodeLines.push_back("ser.popProperty(SerializerBase::PROP_NAMESPACE);");
				if (ns != defaultNS)
				{
					// set the new NS
					std::string codeLine(Poco::format("ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMES%s[", suffix));
					int posNS = in.first->second + firstNSPos; // minus defaultNS
					codeLine.append(Poco::NumberFormatter::format(posNS));
					codeLine.append("]);");
					elemCodeLines.push_back(codeLine);
				}
				lastPushedNS = ns;
			}
			std::string length;
			bool haveLength = GeneratorEngine::getStringProperty(attrProps, Utility::LENGTH, length);
			if (haveLength)
			{
				elemCodeLines.push_back("ser.pushProperty(SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
			}
			std::string xsdType;
			bool haveXsdType = GeneratorEngine::getStringProperty(attrProps, Utility::XSDTYPE, xsdType);
			if (haveXsdType)
			{
				elemCodeLines.push_back("ser.pushProperty(SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
			}
			elemCodeLines.push_back(generateTypeSerializerLine(pDataType, it, curNamesPos, suffix));
			if (haveLength)
			{
				elemCodeLines.push_back("ser.popProperty(SerializerBase::PROP_LENGTH);");
			}
			if (haveXsdType)
			{
				elemCodeLines.push_back("ser.popProperty(SerializerBase::PROP_XSDTYPE);");
			}
		}
		if (lastPushedNS != defaultNS)
			elemCodeLines.push_back("ser.popProperty(SerializerBase::PROP_NAMESPACE);");
		// append the namespaces
		std::vector<std::string>::const_iterator itNS = namespaces.begin();
		std::vector<std::string>::const_iterator itNSEnd = namespaces.end();
		// we write the default NS for attributes only
		// we never access them here, so simply write an empty string
		appendStaticVarName("", staticVarNames);
		++itNS;

		for (; itNS != itNSEnd; ++itNS)
		{
			appendStaticVarName(*itNS, staticVarNames);
		}
		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		// write elem code lines
		writeAll(elemCodeLines, gen);
	}
}


void SerializerGenerator::writeAll(const std::vector<std::string>& codeLines, CodeGenerator& gen)
{
	std::vector<std::string>::const_iterator itCode = codeLines.begin();
	std::vector<std::string>::const_iterator itCodeEnd = codeLines.end();
	for (; itCode != itCodeEnd; ++itCode)
	{
		gen.writeMethodImplementation(*itCode);
	}
}

void SerializerGenerator::serializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SerializerGenerator* pSer = dynamic_cast<SerializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	pSer->handleSuperClassCalls(pDataType, &SerializerGenerator::serializeAttributesCodeGenImpl, true, gen);
	pSer->serializeAttributesCodeGenImpl(pDataType, gen, "");
}


void SerializerGenerator::serializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// varget contains now all variables that we write
	// now split up in attrs + elems
	OrderedVars attrs;
	OrderedVars elems;
	doElemAttrSplit(matches, attrs, elems);
	std::vector<std::string> attrCodeLines;

	// the first line contains a static string array containing names
	if (!attrs.empty())
	{
		gen.writeMethodImplementation("using namespace std::string_literals;\n");

		std::string staticVarNames(Poco::format("static const std::string REMOTING__NAMES%s[] = {", suffix));
		int curNamesPos = 0;
		OrderedVars::const_iterator itAttr = attrs.begin();
		OrderedVars::const_iterator itAttrEnd = attrs.end();
		for (; itAttr != itAttrEnd; ++itAttr, ++curNamesPos)
		{
			appendStaticVarName(getVariableName(itAttr->second), staticVarNames);
			// for each attr we need a ser.pushAttribute(ns, name) followed by a serialize
			// check if the attr has a namespace attribute
			VarGet::const_iterator it = itAttr->second;
			attrCodeLines.push_back(generateTypeSerializerLine(pDataType, it, curNamesPos, suffix));
		}

		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		// write attr code lines
		writeAll(attrCodeLines, gen);
	}
}


void SerializerGenerator::prepareSerializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	SerializerGenerator* pSer = dynamic_cast<SerializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	pSer->handleSuperClassCalls(pDataType, &SerializerGenerator::prepareSerializeAttributesCodeGenImpl, true, gen);
	pSer->prepareSerializeAttributesCodeGenImpl(pDataType, gen, "");
}


void SerializerGenerator::prepareSerializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// varget contains now all variables that we write
	// now split up in attrs + elems
	OrderedVars attrs;
	OrderedVars elems;
	doElemAttrSplit(matches, attrs, elems);
	std::vector<std::string> attrCodeLines;
	std::vector<std::string> namespaces;
	std::map<std::string, int> namespacesMap; // to avoid duplicate namespace entries, the int is the pos in namespace

	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pDataType, structProps);
	std::string defaultNS;
	GeneratorEngine::getStringProperty(structProps, Utility::NAMESPACE, defaultNS);
	namespaces.push_back(defaultNS);
	namespacesMap.insert(std::make_pair(defaultNS, static_cast<int>(namespaces.size()) - 1));
	// the first line contains a static string array containing names
	if (!attrs.empty())
	{
		gen.writeMethodImplementation("using namespace std::string_literals;\n");
		std::string staticVarNames(Poco::format("static const std::string REMOTING__NAMES%s[] = {", suffix));
		// after the names will be all used namespaces
		int firstNSPos = static_cast<int>(attrs.size());
		int curNamesPos = 0;
		OrderedVars::const_iterator itAttr = attrs.begin();
		OrderedVars::const_iterator itAttrEnd = attrs.end();
		for (; itAttr != itAttrEnd; ++itAttr, ++curNamesPos)
		{
			appendStaticVarName(getVariableName(itAttr->second), staticVarNames);
			// for each attr we need a ser.pushAttribute(ns, name) followed by a serialize
			// check if the attr has a namespace attribute
			VarGet::const_iterator it = itAttr->second;
			CodeGenerator::Properties attrProps(structProps);
			GeneratorEngine::parseProperties(it->second.first, attrProps);
			std::string ns;
			GeneratorEngine::getStringProperty(attrProps, Utility::NAMESPACE, ns);
			std::pair<std::map<std::string, int>::iterator, bool> in = namespacesMap.insert(std::make_pair(ns, static_cast <int>(namespaces.size())));
			if (in.second)
				namespaces.push_back(ns);

			//ser.pushAttribute(ns, name)
			std::string pushLine(Poco::format("ser.pushAttribute(REMOTING__NAMES%s[", suffix));
			int posNS = in.first->second + firstNSPos;
			pushLine.append(Poco::NumberFormatter::format(posNS));
			pushLine.append(Poco::format("], REMOTING__NAMES%s[", suffix));
			pushLine.append(Poco::NumberFormatter::format(curNamesPos));
			pushLine.append("]);");
			attrCodeLines.push_back(pushLine);
		}

		// append the namespaces
		std::vector<std::string>::const_iterator itNS = namespaces.begin();
		std::vector<std::string>::const_iterator itNSEnd = namespaces.end();
		for (; itNS != itNSEnd; ++itNS)
		{
			appendStaticVarName(*itNS, staticVarNames);
		}
		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		// write attr code lines
		writeAll(attrCodeLines, gen);
	}
}


void SerializerGenerator::setDocumentation(const Poco::CppParser::Symbol* pSymOld, Poco::CppParser::Symbol* pNew)
{
}


void SerializerGenerator::matchVarsWithFunctions(const Poco::CppParser::Struct* pDataType, VarGet& matches)
{
	// the serializer can only access public members
	Poco::CppParser::Struct::Functions functs;
	pDataType->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functs);
	Poco::CppParser::NameSpace::SymbolTable vars;
	pDataType->variables(vars);
	matchVarsWithFunctions(functs, vars, matches);
}


void SerializerGenerator::matchVarsWithFunctions(const Poco::CppParser::Struct::Functions& functs, Poco::CppParser::NameSpace::SymbolTable& varsOrig, VarGet& matches)
{
	Poco::CppParser::NameSpace::SymbolTable varsCopy(varsOrig);
	std::map<std::string, Poco::CppParser::NameSpace::SymbolTable::iterator> varsLower;

	Poco::CppParser::NameSpace::SymbolTable::iterator itO = varsOrig.begin();
	Poco::CppParser::NameSpace::SymbolTable::iterator itOEnd = varsOrig.end();
	for (; itO != itOEnd; ++itO)
	{
		bool ok = (varsLower.insert(std::make_pair(Poco::toLower(itO->first), itO)).second);
		if (!ok)
		{
			throw Poco::LogicException("Found two variables with the same name: " + itO->first);
		}

		// filter out public members! for those we don't need set and get
		// do not include static or const variables, though
		Poco::CppParser::Variable* pSO = dynamic_cast<Poco::CppParser::Variable*>(itO->second);
		if (pSO)
		{
			if (pSO->getAccess() == Poco::CppParser::Symbol::ACC_PUBLIC)
			{
				int flags = pSO->flags();
				if ((flags & Poco::CppParser::Variable::VAR_STATIC) == 0
			       && (flags & Poco::CppParser::Variable::VAR_CONST) == 0)
			    {
					//public: use directly, set 0 as funct pointer to indicate direct usage
					Poco::CppParser::Function* pNullFct = 0;
					bool ok = matches.insert(std::make_pair(itO->first, std::make_pair(static_cast<Poco::CppParser::Variable*>(itO->second), pNullFct))).second;
					if (!ok)
					{
						throw Poco::LogicException("Found two variables with the same name: " + itO->first);
					}
				}
				varsCopy.erase(itO->first);
				varsLower.erase(Poco::toLower(itO->first));
			}
		}
	}

	// search for each variable a matching function
	// if we don't find one assume that this is deliberate and the user doesn't want to send that member

	Poco::CppParser::Struct::Functions::const_iterator it = functs.begin();
	Poco::CppParser::Struct::Functions::const_iterator itEnd = functs.end();
	for (; it != itEnd; ++it)
	{
		std::string fctName = Poco::toLower((*it)->name());

		if ( (*it)->isConst() &&
			!(*it)->getReturnParameter().empty() &&
			(*it)->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID)
		{
			std::vector<std::string> possibleNames;
			possibleNames.push_back(fctName);
			possibleNames.push_back("_"+fctName);
			possibleNames.push_back("m_"+fctName);
			possibleNames.push_back(fctName+"_");

			size_t pos = fctName.find("get");
			std::string varName("_");
			if (pos == 0)
			{
				varName.append((*it)->name().substr(3));
				Poco::toLowerInPlace(varName);
				possibleNames.push_back(varName);
				possibleNames.push_back("m"+varName);
				possibleNames.push_back(varName.substr(1));
				possibleNames.push_back(varName.substr(1)+"_");
			}
			pos = (*it)->name().find("has");
			if (pos == 0)
			{
				varName = "_" +(*it)->name().substr(3);
				Poco::toLowerInPlace(varName);
				possibleNames.push_back(varName);
				possibleNames.push_back("m"+varName);
				possibleNames.push_back(varName.substr(1));
				possibleNames.push_back(varName.substr(1)+"_");
			}
			pos = (*it)->name().find("is");
			if (pos == 0)
			{
				varName = "_" +(*it)->name().substr(2);
				Poco::toLowerInPlace(varName);
				possibleNames.push_back(varName);
				possibleNames.push_back("m"+varName);
				possibleNames.push_back(varName.substr(1));
				possibleNames.push_back(varName.substr(1)+"_");
			}
			std::vector<std::string>::const_iterator itNames = possibleNames.begin();
			std::vector<std::string>::const_iterator itNamesEnd = possibleNames.end();
			bool found = false;
			for (; itNames != itNamesEnd && !found; ++itNames)
			{
				const std::string& lowerName = *itNames;
				std::map<std::string, Poco::CppParser::NameSpace::SymbolTable::iterator>::iterator itVar = varsLower.find(lowerName);
				if (itVar != varsLower.end())
				{
					// now check if the return type of the function matches the type of the variable!
					Poco::CppParser::Variable* pFoundVar = static_cast<Poco::CppParser::Variable*>(itVar->second->second);
					Poco::CppParser::Function* pFoundFct = *it;

					std::string paramT = GenUtility::getResolvedReturnParameterType(pFoundFct->nameSpace(), pFoundFct);
					Poco::CppParser::Parameter param(paramT + " dummy", 0);

					std::string paramType = Poco::CodeGeneration::Utility::resolveType(pFoundFct->nameSpace(), param.declType());

					std::string typeOfVar = Poco::CodeGeneration::Utility::resolveType(pFoundVar->nameSpace(), pFoundVar->declType());
					if (typeOfVar == paramType)
					{
						found = true;
						bool ok = matches.insert(std::make_pair(itVar->second->first, std::make_pair(pFoundVar, pFoundFct))).second;
						if (!ok)
						{
							throw Poco::LogicException("Found two variables with the same name: " + *itNames);
						}
						varsCopy.erase(itVar->second->first);
					}
				}
			}
		}
	}
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itV = varsCopy.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itVEnd = varsCopy.end();
	for (; itV != itVEnd; ++itV)
	{
		EError.notify(0, "Warning: Failed to find getter method for " + itV->second->nameSpace()->fullName() + ":" + itV->first);
	}
}


void SerializerGenerator::matchVarsWithFunctionsRecursive(const Poco::CppParser::Struct* pDataType, VarGet& matches)
{
	matchVarsWithFunctions(pDataType, matches);
	for (Poco::CppParser::Struct::BaseIterator it = pDataType->baseBegin(); it != pDataType->baseEnd(); ++it)
	{
		matchVarsWithFunctionsRecursive(it->pClass, matches);
	}
}


void SerializerGenerator::doElemAttrSplit(const VarGet& matches, OrderedVars& attrs, OrderedVars& elems)
{
	const Poco::UInt32 unknownOrderOffset = 0xffff0000;
	Poco::UInt32 varCount = 0;
	VarGet::const_iterator it = matches.begin();
	VarGet::const_iterator itEnd = matches.end();
	for (; it != itEnd; ++it, ++varCount)
	{
		Poco::CodeGeneration::CodeGenerator::Properties props; // we need type and order
		Poco::CodeGeneration::GeneratorEngine::parseProperties(it->second.first, props);
		std::string type;
		bool hasType = GeneratorEngine::getStringProperty(props, Utility::TYPE, type);
		bool isAttr = false; // the default is elem
		if (hasType && type == Utility::VAL_ATTR)
			isAttr = true;
		Poco::UInt32 order = _serializationOrder == SERIALIZE_AS_DECLARED ? static_cast<Poco::UInt32>(it->second.first->getOrder()) : unknownOrderOffset + varCount;
		GeneratorEngine::getUInt32Property(props, Utility::ORDER, order);
		if (isAttr)
			attrs.insert(std::make_pair(order, it));
		else
			elems.insert(std::make_pair(order, it));
	}
}


std::string SerializerGenerator::generateTypeSerializerLine(const Poco::CppParser::Struct* pContext, VarGet::const_iterator it, int namePos, const std::string& suffix)
{
	std::string code("TypeSerializer<");

	Poco::CppParser::Parameter ret(it->second.first->declType()+" dummy", 0);
	std::string retType (Poco::CodeGeneration::Utility::resolveType(pContext, ret.declType()));
	Poco::CppParser::Symbol* pSym = pContext->lookup(it->second.first->declType());

	bool enumMode = false;
	std::string enumBaseType;
	if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
	{
		enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
		if (enumBaseType.empty()) enumBaseType = "int";
		retType = enumBaseType;
		enumMode = true;
	}

	code.append(retType);
	if (ret.isPointer())
		code.append("*");
	code.append(Poco::format(">::serialize(REMOTING__NAMES%s[", suffix));
	code.append(Poco::NumberFormatter::format(namePos));
	code.append("], ");
	if (enumMode)
	{
		code.append("static_cast<");
		code.append(enumBaseType);
		code.append(">(");
	}
	code.append("value");
	if (it->second.second != 0)
	{
		code.append("."+it->second.second->name());
		code.append("()");
		if (enumMode) code.append(")");
		code.append(", ser);");
	}
	else
	{
		code.append(".");
		code.append(it->second.first->name());
		if (enumMode) code.append(")");
		code.append(", ser);");
	}
	return code;
}


void SerializerGenerator::appendStaticVarName(const std::string& varName, std::string& result)
{
	result.append("\"");
	result.append(varName);
	result.append("\"s,");
}


bool SerializerGenerator::hasAttributes(const Poco::CppParser::Struct* pClass)
{
	bool ret = false;
	Poco::CppParser::Struct::BaseIterator itB = pClass->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pClass->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (!itB->pClass)
		{
			if (GenUtility::mustBeSerializable(itB->name))
			{
				std::string msg("While handling " + pClass->fullName() + ": ");
				msg.append("Missing base class " + itB->name + ". Please add the header file defining the class to the code generator configuration and make sure the class is serializable.");
				throw CodeGenerationException(msg);
			}
		}
		else
		{
			ret |= hasAttributes(itB->pClass);
		}

	}
	if (!ret)
	{
		VarGet matches;
		matchVarsWithFunctions(pClass, matches);
		OrderedVars attrs;
		OrderedVars elems;
		doElemAttrSplit(matches, attrs, elems);
		ret = !attrs.empty();
	}
	return ret;
}


void SerializerGenerator::handleSuperClassCalls(const Poco::CppParser::Struct* pClass, void(SerializerGenerator::*generate)(const Poco::CppParser::Struct*, CodeGenerator&, const std::string&), bool dependsOnAttributes, CodeGenerator& gen)
{
	// add superclass calls
	Poco::CppParser::Struct::BaseIterator itB = pClass->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pClass->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (!itB->pClass)
		{
			if (GenUtility::mustBeSerializable(itB->name))
			{
				std::string msg("While handling " + pClass->fullName() + ": ");
				msg.append("Missing base class " + itB->name + ". Please add the header file defining the class to the code generator configuration and make sure the class is serializable.");
				throw CodeGenerationException(msg);
			}
		}
		else
		{
			// only call the parent if it has attributes
			if (hasAttributes(itB->pClass) || !dependsOnAttributes)
			{
				handleSuperClassCalls(itB->pClass, generate, dependsOnAttributes, gen);

				std::string suffix = "__";
				suffix += Poco::replace(Poco::toUpper(itB->pClass->fullName()), ":", "_");
				gen.writeMethodImplementation("// " + itB->pClass->fullName());
				gen.writeMethodImplementation("{");
				(this->*generate)(itB->pClass, gen, suffix);
				gen.writeMethodImplementation("}");
				gen.writeMethodImplementation("");
			}
		}
	}
}

