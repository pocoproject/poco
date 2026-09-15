//
// DeserializerGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "DeserializerGenerator.h"
#include "SerializerGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Variable.h"
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


DeserializerGenerator::DeserializerGenerator(Poco::CodeGeneration::CppGenerator& cppGen): AbstractGenerator(cppGen)
{
}


DeserializerGenerator::~DeserializerGenerator()
{
}


std::string DeserializerGenerator::generateClassName(const Poco::CppParser::Struct* pStruct)
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

	newClassName.append("Deserializer");
	return newClassName;
}


std::string DeserializerGenerator::generateQualifiedClassName(const std::string& ns, const Poco::CppParser::Struct* pStruct)
{
	if (ns.empty())
		return DeserializerGenerator::generateClassName(pStruct);
	return ns + "::" + DeserializerGenerator::generateClassName(pStruct);
}


void DeserializerGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStructIn = pStruct;
	std::string newClassName (DeserializerGenerator::generateClassName(pStruct));
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		if (!itB->pClass)
		{
			if (GenUtility::mustBeSerializable(itB->name))
			{
				throw CodeGenerationException("Cannot create deserializer for class/struct which has a non serializable base", itB->name);
			}
		}
	}

	// we create the following class: template <> TypeSerializer<StructInputData>
	std::string decl("template <>\nclass TypeDeserializer<");
	decl.append(pStruct->fullName());
	decl.append(">");

	_pStruct = new Poco::CppParser::Struct(decl, true, _pNs);

	Poco::CppParser::Function* pFuncRef = new Poco::CppParser::Function("static bool deserialize", _pStruct);
	addParameters(pFuncRef, pStruct->fullName());
	Poco::CppParser::Function* pFuncRef2 = new Poco::CppParser::Function("static void deserializeImpl", _pStruct);
	addImplParameters(pFuncRef2, pStruct->fullName());

	// if we have attributes, we must add two additional methods
	if (hasAttributes(pStruct))
	{
		// static void serializeAttributes(const Element& elem, const T& value, Serializer& ser)
		// static void prepareSerializeAttributes(const Element& elem, const T& value, Serializer& ser)
		Poco::CppParser::Function* pFuncRef3 = new Poco::CppParser::Function("static void deserializeAttributes", _pStruct);
		addImplParameters(pFuncRef3, pStruct->fullName());
		Poco::CppParser::Function* pFuncRef4 = new Poco::CppParser::Function("static void prepareDeserializeAttributes", _pStruct);
		pFuncRef4->addParameter(new Poco::CppParser::Parameter("Deserializer& deser", 0));
	}

	// we must include TypeSerializer, and the file of the struct we serialize
	_cppGen.addIncludeFile("Poco/RemotingNG/TypeDeserializer.h");
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

	SerializerGenerator::VarGet matches;
	matchVarsWithFunctionsRecursive(pStruct, matches);
	for (SerializerGenerator::VarGet::iterator it = matches.begin(); it != matches.end(); ++it)
	{
		handleIncludeTypeSerializers(it->second.first, true, false);
	}
}


std::vector<std::string> DeserializerGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	// we extend from Skeleton or from a superclass skeleton
	std::vector<std::string> bases;
	return bases;
}


void DeserializerGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
	//we don't want any code for constructor and destructor
	e.registerCallback(_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("~"+_pStruct->name(), &Poco::CodeGeneration::GeneratorEngine::emptyCodeGen);
	e.registerCallback("deserialize", &DeserializerGenerator::deserializeCodeGen);
	e.registerCallback("deserializeImpl", &DeserializerGenerator::deserializeImplCodeGen);
	e.registerCallback("deserializeAttributes", &DeserializerGenerator::deserializeAttributesCodeGen);
	e.registerCallback("prepareDeserializeAttributes", &DeserializerGenerator::prepareDeserializeAttributesCodeGen);
}


void DeserializerGenerator::methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
}


void DeserializerGenerator::addParameters(Poco::CppParser::Function* pFunc, const std::string& typeName)
{
	//deserialize(const std::string& name, bool isMandatory, Deserializer& deser, T& value)
	pFunc->addParameter(new Poco::CppParser::Parameter("const std::string& name", 0));
	pFunc->addParameter(new Poco::CppParser::Parameter("bool isMandatory", 0));
	pFunc->addParameter(new Poco::CppParser::Parameter("Deserializer& deser", 0));
	std::string type(typeName);
	type.append("&");
	type.append(" value");
	pFunc->addParameter(new Poco::CppParser::Parameter(type, 0));
}


void DeserializerGenerator::addImplParameters(Poco::CppParser::Function* pFunc, const std::string& typeName)
{
	//deserialize(const std::string& name, bool isMandatory, Deserializer& deser, T& value)
	pFunc->addParameter(new Poco::CppParser::Parameter("Deserializer& deser", 0));
	std::string type(typeName);
	type.append("&");
	type.append(" value");
	pFunc->addParameter(new Poco::CppParser::Parameter(type, 0));
}


void DeserializerGenerator::deserializeCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	DeserializerGenerator* pDeser = dynamic_cast<DeserializerGenerator*>(pAGen);
	if (pDeser == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pDeser->_pStructIn; // returns the data type for which pStruct was generated
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
	}

	bool hasAttr = hasAttributes(pDataType);
	if (hasAttr)
		gen.writeMethodImplementation("prepareDeserializeAttributes(deser);");
	gen.writeMethodImplementation("bool ret = deser.deserializeStructBegin(name, isMandatory);");
	if (hasDefaultNS && !defaultNS.empty())
	{
		gen.writeMethodImplementation("deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);");
	}
	gen.writeMethodImplementation("if (ret)");
	gen.writeMethodImplementation("{");
	if (hasAttr)
		gen.writeMethodImplementation("\tdeserializeAttributes(deser, value);");
	gen.writeMethodImplementation("\tdeserializeImpl(deser, value);");
	if (hasDefaultNS && !defaultNS.empty())
		gen.writeMethodImplementation("\tdeser.popProperty(SerializerBase::PROP_NAMESPACE);");
	gen.writeMethodImplementation("\tdeser.deserializeStructEnd(name);");
	gen.writeMethodImplementation("}");
	if (hasDefaultNS && !defaultNS.empty())
		gen.writeMethodImplementation("else deser.popProperty(SerializerBase::PROP_NAMESPACE);");
	gen.writeMethodImplementation("return ret;");
}


void DeserializerGenerator::deserializeImplCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	DeserializerGenerator* pSer = dynamic_cast<DeserializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	// now seach for each variable a matching function
	// if we don't find one assume that this is deliberate and the user doesn't want to send that member

	gen.writeMethodImplementation("using namespace std::string_literals;\n");

	pSer->handleSuperClassCalls(pDataType, &DeserializerGenerator::deserializeImplCodeGenImpl, false, gen);
	pSer->deserializeImplCodeGenImpl(pDataType, gen, "");
}


void DeserializerGenerator::deserializeImplCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	SerializerGenerator::VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// now split up in two multimaps: one containing attrs only, the other containing elems only
	SerializerGenerator::OrderedVars attrs;
	SerializerGenerator::OrderedVars elems;
	SerializerGenerator::doElemAttrSplit(matches, attrs, elems);
	std::vector<std::string> elemCodeLines;
	CodeGenerator::Properties structProps;
	GeneratorEngine::parseProperties(pDataType, structProps);
	// the deserializer doesn't care about namespaces!

	// the first line contains a static string array containing names
	if (!elems.empty())
	{
		std::string staticVarNames(Poco::format("static const std::string REMOTING__NAMES%s[] = {", suffix));
		int curNamesPos = 0;
		int retUsage(0);

		SerializerGenerator::OrderedVars::const_iterator itElem = elems.begin();
		SerializerGenerator::OrderedVars::const_iterator itElemEnd = elems.end();
		for (; itElem != itElemEnd; ++itElem, ++curNamesPos)
		{
			SerializerGenerator::appendStaticVarName(SerializerGenerator::getVariableName(itElem->second), staticVarNames);
			SerializerGenerator::VarGet::const_iterator it = itElem->second;
			generateTypeDeserializerLines(pDataType, it, curNamesPos, suffix, elemCodeLines, retUsage);
		}

		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		// write elem code lines
		if (!elemCodeLines.empty())
		{
			if (retUsage > 0)
				gen.writeMethodImplementation("bool ret = false;");
			SerializerGenerator::writeAll(elemCodeLines, gen);
		}
	}
}


void DeserializerGenerator::prepareDeserializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	DeserializerGenerator* pSer = dynamic_cast<DeserializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	// now seach for each variable a matching function
	// if we don't find one assume that this is deliberate and the user doesn't want to send that member

	pSer->handleSuperClassCalls(pDataType, &DeserializerGenerator::prepareDeserializeAttributesCodeGenImpl, true, gen);
	pSer->prepareDeserializeAttributesCodeGenImpl(pDataType, gen, "");
}


void DeserializerGenerator::prepareDeserializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	SerializerGenerator::VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// now split up in two multimaps: one containing attrs only, the other containing elems only
	SerializerGenerator::OrderedVars attrs;
	SerializerGenerator::OrderedVars elems;
	SerializerGenerator::doElemAttrSplit(matches, attrs, elems);

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
		SerializerGenerator::OrderedVars::const_iterator itAttr = attrs.begin();
		SerializerGenerator::OrderedVars::const_iterator itAttrEnd = attrs.end();
		for (; itAttr != itAttrEnd; ++itAttr, ++curNamesPos)
		{
			SerializerGenerator::appendStaticVarName(SerializerGenerator::getVariableName(itAttr->second), staticVarNames);
			// for each attr we need a ser.pushAttribute(ns, name) followed by a serialize
			// check if the attr has a namespace attribute
			SerializerGenerator::VarGet::const_iterator it = itAttr->second;
			CodeGenerator::Properties attrProps(structProps);
			GeneratorEngine::parseProperties(it->second.first, attrProps);
			std::string ns;
			GeneratorEngine::getStringProperty(attrProps, Utility::NAMESPACE, ns);
			std::pair<std::map<std::string, int>::iterator, bool> in = namespacesMap.insert(std::make_pair(ns, static_cast <int>(namespaces.size())));
			if (in.second)
				namespaces.push_back(ns);

			bool mandatory = GenUtility::getIsMandatory(attrProps);

			//deser.pushAttribute(ns, name, bool)
			std::string pushLine(Poco::format("deser.pushAttribute(REMOTING__NAMES%s[", suffix));
			int posNS = in.first->second + firstNSPos;
			pushLine.append(Poco::NumberFormatter::format(posNS));
			pushLine.append(Poco::format("], REMOTING__NAMES%s[", suffix));
			pushLine.append(Poco::NumberFormatter::format(curNamesPos));
			pushLine.append("], ");

			if (mandatory)
				pushLine.append("true");
			else
				pushLine.append("false");

			pushLine.append(");");

			attrCodeLines.push_back(pushLine);
		}

		// append the namespaces
		std::vector<std::string>::const_iterator itNS = namespaces.begin();
		std::vector<std::string>::const_iterator itNSEnd = namespaces.end();
		for (; itNS != itNSEnd; ++itNS)
		{
			SerializerGenerator::appendStaticVarName(*itNS, staticVarNames);
		}
		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		// write attr code lines
		SerializerGenerator::writeAll(attrCodeLines, gen);
	}
}


void DeserializerGenerator::deserializeAttributesCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	if (addParam == 0)
		return;

	AbstractGenerator* pAGen = reinterpret_cast<AbstractGenerator*>(addParam);
	DeserializerGenerator* pSer = dynamic_cast<DeserializerGenerator*>(pAGen);
	if (pSer == 0)
		return;

	const Poco::CppParser::Struct* pDataType = pSer->_pStructIn; // returns the data type for which pStruct was generated
	poco_assert(pDataType);

	// now seach for each variable a matching function
	// if we don't find one assume that this is deliberate and the user doesn't want to send that member

	pSer->handleSuperClassCalls(pDataType, &DeserializerGenerator::deserializeAttributesCodeGenImpl, true, gen);
	pSer->deserializeAttributesCodeGenImpl(pDataType, gen, "");
}


void DeserializerGenerator::deserializeAttributesCodeGenImpl(const Poco::CppParser::Struct* pDataType, CodeGenerator& gen, const std::string& suffix)
{
	SerializerGenerator::VarGet matches;
	matchVarsWithFunctions(pDataType, matches);

	// now split up in two multimaps: one containing attrs only, the other containing elems only
	SerializerGenerator::OrderedVars attrs;
	SerializerGenerator::OrderedVars elems;
	SerializerGenerator::doElemAttrSplit(matches, attrs, elems);

	std::vector<std::string> attrCodeLines;

	// the first line contains a static string array containing names
	if (!attrs.empty())
	{
		gen.writeMethodImplementation("using namespace std::string_literals;\n");
		std::string staticVarNames(Poco::format("static const std::string REMOTING__NAMES%s[] = {", suffix));
		int curNamesPos = 0;
		int retUsage(0);
		SerializerGenerator::OrderedVars::const_iterator itAttr = attrs.begin();
		SerializerGenerator::OrderedVars::const_iterator itAttrEnd = attrs.end();
		for (; itAttr != itAttrEnd; ++itAttr, ++curNamesPos)
		{
			SerializerGenerator::appendStaticVarName(SerializerGenerator::getVariableName(itAttr->second), staticVarNames);
			// for each attr we need a ser.pushAttribute(ns, name) followed by a serialize
			// check if the attr has a namespace attribute
			SerializerGenerator::VarGet::const_iterator it = itAttr->second;
			generateTypeDeserializerLines(pDataType, it, curNamesPos, suffix, attrCodeLines, retUsage);
		}

		poco_assert_dbg (staticVarNames[staticVarNames.size() -1] == ',');
		staticVarNames[staticVarNames.size() -1] = '}';
		staticVarNames.append(";");
		gen.writeMethodImplementation(staticVarNames);
		if (retUsage > 0)
			gen.writeMethodImplementation("bool ret = false;");
		// write attr code lines
		SerializerGenerator::writeAll(attrCodeLines, gen);
	}
}


void DeserializerGenerator::setDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew)
{
}


void DeserializerGenerator::handleSuperClassCalls(const Poco::CppParser::Struct* pClass, void (DeserializerGenerator::*generate)(const Poco::CppParser::Struct*, CodeGenerator&, const std::string&), bool dependsOnAttributes, CodeGenerator& gen)
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


void DeserializerGenerator::matchVarsWithFunctions(const Poco::CppParser::Struct* pDataType, SerializerGenerator::VarGet& matches)
{
	// the deserializer can only access public members
	Poco::CppParser::Struct::Functions functs;
	pDataType->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functs);
	Poco::CppParser::NameSpace::SymbolTable vars;
	pDataType->variables(vars);
	matchVarsWithFunctions(functs, vars, matches);
}


void DeserializerGenerator::matchVarsWithFunctions(const Poco::CppParser::Struct::Functions& functs, Poco::CppParser::NameSpace::SymbolTable& varsOrig, SerializerGenerator::VarGet& matches)
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
					matches.insert(std::make_pair(itO->first, std::make_pair(static_cast<Poco::CppParser::Variable*>(itO->second), pNullFct)));
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
		if ( !(*it)->isConst() &&
			(*it)->countParameters() == 1 &&
			(*it)->getReturnParameter() == Poco::CodeGeneration::Utility::TYPE_VOID)
		{
			std::set<std::string> possibleNames;

			possibleNames.insert(fctName);
			possibleNames.insert("_"+fctName);
			possibleNames.insert("m_"+fctName);
			possibleNames.insert(fctName+"_");

			bool isSwap = false;
			std::string varName("_");
			size_t pos = fctName.find("swap");
			if (pos == 0)
			{
				isSwap = true;
				varName.append(fctName.substr(4));
				Poco::toLowerInPlace(varName);
				possibleNames.insert(varName);
				possibleNames.insert("m"+varName);
				possibleNames.insert(varName.substr(1));
				possibleNames.insert(varName.substr(1)+"_");
			}
			else
			{
				pos = fctName.find("set");
				if (pos == 0)
				{
					varName.append(fctName.substr(3));
					Poco::toLowerInPlace(varName);
					possibleNames.insert(varName);
					possibleNames.insert("m"+varName);
					possibleNames.insert(varName.substr(1));
					possibleNames.insert(varName.substr(1)+"_");
				}
			}
			std::set<std::string>::const_iterator itNames = possibleNames.begin();
			std::set<std::string>::const_iterator itNamesEnd = possibleNames.end();
			bool found = false;
			for (; itNames != itNamesEnd && !found; ++itNames)
			{
				const std::string& lowerName = *itNames;
				std::map<std::string, Poco::CppParser::NameSpace::SymbolTable::iterator>::iterator itVar = varsLower.find(lowerName);
				if (itVar != varsLower.end())
				{
					// now check if the argument type of the function matches the type of the variable!
					Poco::CppParser::Variable* pFoundVar = static_cast<Poco::CppParser::Variable*>(itVar->second->second);
					Poco::CppParser::Function* pFoundFct = *it;
					Poco::CppParser::Function::Iterator it = pFoundFct->begin();
					if (it != pFoundFct->end())
					{
						Poco::CppParser::Parameter* param = *it;
						++it;
						if (it == pFoundFct->end())
						{
							std::string paramType = Poco::CodeGeneration::Utility::resolveType(pFoundFct->nameSpace(), param->declType());

							std::string typeOfVar = Poco::CodeGeneration::Utility::resolveType(pFoundVar->nameSpace(), pFoundVar->declType());
							if (typeOfVar == paramType)
							{
								found = true;
								SerializerGenerator::VarGet::iterator itM = matches.find(itVar->second->first);
								if (itM != matches.end() && isSwap)
									matches.erase(itM);
								matches.insert(std::make_pair(itVar->second->first, std::make_pair(pFoundVar, pFoundFct)));
								varsCopy.erase(itVar->second->first);
							}
						}
					}
				}
			}
		}
	}
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itV = varsCopy.begin();
	Poco::CppParser::NameSpace::SymbolTable::const_iterator itVEnd = varsCopy.end();
	for (; itV != itVEnd; ++itV)
	{
		EError.notify(0, "Warning: Failed to find setter method for " + itV->second->nameSpace()->fullName() + ":" + itV->first);
	}
}


void DeserializerGenerator::matchVarsWithFunctionsRecursive(const Poco::CppParser::Struct* pDataType, SerializerGenerator::VarGet& matches)
{
	matchVarsWithFunctions(pDataType, matches);
	for (Poco::CppParser::Struct::BaseIterator it = pDataType->baseBegin(); it != pDataType->baseEnd(); ++it)
	{
		matchVarsWithFunctionsRecursive(it->pClass, matches);
	}
}


bool DeserializerGenerator::hasAttributes(const Poco::CppParser::Struct* pClass)
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
		SerializerGenerator::VarGet matches;
		matchVarsWithFunctions(pClass, matches);
		SerializerGenerator::OrderedVars attrs;
		SerializerGenerator::OrderedVars elems;
		SerializerGenerator::doElemAttrSplit(matches, attrs, elems);
		ret = !attrs.empty();
	}
	return ret;
}


void DeserializerGenerator::generateTypeDeserializerLines(const Poco::CppParser::Struct* pDataType, SerializerGenerator::VarGet::const_iterator it, int namePos, const std::string& suffix, std::vector<std::string>& lines, int& retUsageCount)
{
	bool isPublicMember = (it->second.second == 0);
	const Poco::CppParser::Variable* pVar = it->second.first;
	CodeGenerator::Properties varProps;
	GeneratorEngine::parseProperties(pVar, varProps);
	bool mandatory = GenUtility::getIsMandatory(varProps);

	std::string length;
	bool haveLength = GeneratorEngine::getStringProperty(varProps, Utility::LENGTH, length);
	if (haveLength)
	{
		lines.push_back("deser.pushProperty(SerializerBase::PROP_LENGTH, \"" + length + "\"s);");
	}
	std::string xsdType;
	bool haveXsdType = GeneratorEngine::getStringProperty(varProps, Utility::XSDTYPE, xsdType);
	if (haveXsdType)
	{
		lines.push_back("deser.pushProperty(SerializerBase::PROP_XSDTYPE, \"" + xsdType + "\"s);");
	}

	std::string declType (Poco::CodeGeneration::Utility::resolveType(pDataType, pVar->declType()));
	const std::string& origVarName = it->second.first->name();
	std::string varName("gen" + origVarName);
	std::string code("ret = TypeDeserializer<");
	Poco::CppParser::Symbol* pSym = pDataType->lookup(declType);
	bool enumMode = false;
	std::string enumBaseType;
	if (pSym && pSym->kind() == Poco::CppParser::Symbol::SYM_ENUM)
	{
		enumMode = true;
		enumBaseType = static_cast<Poco::CppParser::Enum*>(pSym)->baseType();
		if (enumBaseType.empty()) enumBaseType = "int";
	}
	if (enumMode)
	{
		code = enumBaseType;
		code += " ";
		code.append(varName);
		code.append(";");
		lines.push_back(code);
		code = "ret = TypeDeserializer<";
		++retUsageCount;
	}
	else if (isPublicMember)
	{
		code = "TypeDeserializer<";
		varName = "value";
		varName.append(".");
		varName.append(pVar->name());
	}
	else ++retUsageCount; // we use the ret variable

	if (enumMode)
		code.append(enumBaseType);
	else
		code.append(declType);
	if (pVar->isPointer())
		code.append("*");
	code.append(Poco::format(">::deserialize(REMOTING__NAMES%s[", suffix));
	code.append(Poco::NumberFormatter::format(namePos));
	code.append("], ");

	if (mandatory)
		code.append("true");
	else
		code.append("false");

	code.append(", deser, ");
	code.append(varName);
	code.append(");");
	if (pVar->isPointer())
	{
		throw CodeGenerationException("Pointer types are not supported", pDataType->name());
	}
	else
	{
		if (isPublicMember)
		{
			// no decl
			lines.push_back(code);
			if (enumMode)
			{
				code = "if (ret) value";
				code.append(".");
				code.append(pVar->name());
				code.append(" = static_cast<");
				code.append(declType);
				code.append(">(");
				code.append(varName);
				code.append(");");
				lines.push_back(code);
			}
			// no need to check retCode
			// no need to assign
		}
		else if (enumMode)
		{
			lines.push_back(code);
			lines.push_back("if (ret) value." + it->second.second->name() + "("
				+ "static_cast<" + declType + ">(" + varName + "));");
		}
		else
		{
			lines.push_back(declType + " " + varName + ";");
			lines.push_back(code);
			lines.push_back("if (ret) value." + it->second.second->name() + "(" + varName + ");");
		}
	}

	if (haveLength)
	{
		lines.push_back("deser.popProperty(SerializerBase::PROP_LENGTH);");
	}
	if (haveXsdType)
	{
		lines.push_back("deser.popProperty(SerializerBase::PROP_XSDTYPE);");
	}
}
