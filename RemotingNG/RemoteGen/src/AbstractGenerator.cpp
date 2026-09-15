//
// AbstractGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AbstractGenerator.h"
#include "InterfaceGenerator.h"
#include "SerializerGenerator.h"
#include "DeserializerGenerator.h"
#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/TypeDef.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include <stack>


using Poco::CodeGeneration::Utility;


Poco::BasicEvent<const std::string> AbstractGenerator::EError;
const std::set<std::string> AbstractGenerator::BUILTIN_TYPES(initBuiltinTypes());


AbstractGenerator::AbstractGenerator(Poco::CodeGeneration::CppGenerator& cppGen):
	Poco::CodeGeneration::CodeGenerator(cppGen.nameSpace(), cppGen.libraryName(), cppGen.usePocoIncludeStyle(), cppGen.copyright()),
	_cppGen(cppGen),
	_pStruct(0),
	_pStructIn(0),
	_pNs(0)
{
	Poco::CppParser::Symbol* pS = Poco::CppParser::NameSpace::root();
	if (!_cppGen.nameSpace().empty())
	{
		pS = Poco::CppParser::NameSpace::root()->lookup(_cppGen.nameSpace());
		if (!pS)
			throw CodeGenerationException("Missing information for namespace " + _cppGen.nameSpace() + ". No class included from that namespace?");
	}
	//root namespace must always exist
	poco_assert (pS);
	poco_assert (pS->kind() == pS->SYM_NAMESPACE);
	_pNs = static_cast<Poco::CppParser::NameSpace*>(pS);
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	if (cfg.hasProperty("RemoteGen.output.alwaysInclude"))
	{
		std::string incFile = cfg.getString("RemoteGen.output.alwaysInclude");
		if (!incFile.empty())
		{
			_cppGen.addIncludeFile(incFile);
		}
	}
	else if (cfg.hasProperty("RemoteGen.output.namespacedeclfile"))
	{
		std::string incFile = cfg.getString("RemoteGen.output.namespacedeclfile");
		if (!incFile.empty())
		{
			_cppGen.addIncludeFile(incFile);
		}
	}
}


AbstractGenerator::~AbstractGenerator()
{
}


void AbstractGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	poco_assert_dbg(_pStruct == 0);
	poco_assert_dbg(_pStructIn == 0);
	_pStructIn = pStruct;
	_pStruct = structClone(pStruct, newClassName(pStruct), newBaseClasses(pStruct));
}


void AbstractGenerator::structEnd()
{
	Poco::CodeGeneration::GeneratorEngine e;
	registerCallbacks(e);
	e.generate(_pStruct, _cppGen, reinterpret_cast<void*>(this));
	_pStruct = 0;
	_pStructIn = 0;
}


Poco::CppParser::Function* AbstractGenerator::methodClone(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	poco_assert_dbg (_pStruct && _pStructIn);

	// simply copy the method signature, just update docu
	// note that a decl contains only the string up to method, no params!
	std::string decl;
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

	return pFunc;
}


Poco::CppParser::Struct* AbstractGenerator::structClone(const Poco::CppParser::Struct* aStruct, const std::string& newClassName, const std::vector<std::string>& baseClasses)
{
	poco_assert_dbg (_pStruct == 0);
	poco_assert_dbg (aStruct != 0);

	// generate the decl: class NS_API newClassName: public BaseNS::IbaseClass
	std::string decl("class ");
	if (!_cppGen.libraryName().empty())
	{
		decl.append(_cppGen.libraryName());
		decl.append("_API ");
	}
	else
		decl.append(" ");

	decl.append(newClassName);

	Poco::CppParser::Struct* pStruct = new Poco::CppParser::Struct(decl, true, _pNs);
	std::vector<std::string>::const_iterator it = baseClasses.begin();
	std::vector<std::string>::const_iterator itEnd = baseClasses.end();
	for (; it != itEnd; ++it)
		pStruct->addBase(*it, Poco::CppParser::Symbol::ACC_PUBLIC, false);

	pStruct->fixupBases();

	Poco::Path aPath(aStruct->getFile());
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	std::string incFile = includePath(cfg, aStruct);
	if (!incFile.empty())
	{
		aPath = Poco::Path(incFile);
		aPath.makeDirectory();
	}
	addDocumentation(aStruct, pStruct);
	pStruct->setAttributes(aStruct->getAttributes());
	pStruct->attrs().remove("remote");
	aPath.setFileName(newClassName+".h");
	pStruct->setFile(aPath.toString());
	pStruct->setLibrary(aStruct->getLibrary());
	pStruct->setPackage("Generated");
	return pStruct;
}


void AbstractGenerator::handleParentFunctions(const Poco::CppParser::Struct* pStruct)
{
	std::set<std::string> methodSigs;
	bool root = true;
	handleParentFunctionsImpl(pStruct, methodSigs, root);
}


void AbstractGenerator::handleParentFunctionsImpl(const Poco::CppParser::Struct* pStruct, std::set<std::string>& methodSigs, bool root)
{
	Poco::CppParser::Struct::BaseIterator itB = pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = pStruct->baseEnd();
	for (; itB != itBEnd; ++itB)
	{
		const Poco::CppParser::Struct* pParent = itB->pClass;
		if (pParent && Utility::hasAnyRemoteProperty(pParent))
		{
			handleParentFunctionsImpl(pParent, methodSigs, false);
		}
	}

	Poco::CodeGeneration::CodeGenerator::Properties classProperties;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, classProperties);

	if (!root)
	{
		Poco::CppParser::Struct::Functions functions;
		// we only handle public functions, others cannot be called remote
		pStruct->methods(Poco::CppParser::Symbol::ACC_PUBLIC, functions);
		Poco::CppParser::Struct::Functions::const_iterator it = functions.begin();
		Poco::CppParser::Struct::Functions::const_iterator itEnd = functions.end();

		for (; it != itEnd; ++it)
		{
			CodeGenerator::Properties methodProperties(classProperties);
			Poco::CodeGeneration::GeneratorEngine::parseProperties(*it, methodProperties);
			std::string signature = (*it)->signature();
			if (methodSigs.find(signature) == methodSigs.end())
			{
				methodSigs.insert(signature);
				methodStart(*it, methodProperties);
				methodEnd(*it, methodProperties);
			}
		}
	}
}


void AbstractGenerator::addDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew)
{
	if (pSymOld != 0 && !pSymOld->getDocumentation().empty())
	{
		pNew->addDocumentation(pSymOld->getDocumentation());
	}
	if (pSymOld != 0 && pNew != 0)
	{
		pNew->setAttributes(pSymOld->getAttributes());
	}
}


void AbstractGenerator::addParamDocu(Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& props)
{
	Poco::CodeGeneration::CodeGenerator::Properties::const_iterator it = props.find("$" + pParam->name());
	if (it != props.end())
		pParam->setDocumentation(it->second);
}


void AbstractGenerator::typeIdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam)
{
	poco_assert (addParam);
	AbstractGenerator* pGen = reinterpret_cast<AbstractGenerator*>(addParam);

	gen.writeMethodImplementation("return " + InterfaceGenerator::generateClassName(pGen->_pStructIn) + "::remoting__typeId();");
}


void AbstractGenerator::includeTypeSerializers(const Poco::CppParser::Function* pFunc, bool toHFile, bool writeDirectly)
{
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		std::string resType = Utility::resolveType(pFunc->nameSpace(), (*it)->declType());
		Poco::CppParser::Symbol* pS = pFunc->nameSpace()->lookup(resType);
		if (pS)
		{
			if (pS->kind() != Poco::CppParser::Symbol::SYM_ENUM)
				handleIncludeTypeSerializers(*it, toHFile, writeDirectly);
		}
		else
			handleIncludeTypeSerializers(*it, toHFile, writeDirectly);
	}
	if (!pFunc->getReturnParameter().empty() && pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID)
	{
		Poco::CppParser::Parameter ret(pFunc->getReturnParameter() + " dummy", 0);
		std::string resType = Utility::resolveType(pFunc->nameSpace(), ret.declType());
		Poco::CppParser::Symbol* pS = pFunc->nameSpace()->lookup(resType);
		if (pS)
		{
			if (pS->kind() != Poco::CppParser::Symbol::SYM_ENUM)
				handleIncludeTypeSerializers(&ret, toHFile, writeDirectly);
		}
		else
			handleIncludeTypeSerializers(&ret, toHFile, writeDirectly);
	}
}


void AbstractGenerator::includeTypes(const Poco::CppParser::Function* pFunc, bool toHFile, bool writeDirectly)
{
	if (pFunc->isConstructor() || pFunc->isDestructor())
		return;
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		handleIncludeTypes(*it, toHFile, writeDirectly);
	}
	if (!pFunc->getReturnParameter().empty() && pFunc->getReturnParameter() != Poco::CodeGeneration::Utility::TYPE_VOID)
	{
		Poco::CppParser::Parameter ret(pFunc->getReturnParameter() + " dummy", 0);
		handleIncludeTypes(&ret, toHFile, writeDirectly);
	}
}


void AbstractGenerator::handleIncludeTypeSerializers(const Poco::CppParser::Parameter* pParam, bool toHFile, bool writeDirectly)
{
	if (pParam->kind() == Poco::CppParser::Symbol::SYM_ENUM)
		return;
	std::string resType(resolveType(pParam->declType()));
	Poco::trimInPlace(resType);
	handleIncludeTypeSerializersImpl(resType, toHFile, writeDirectly);
}


void AbstractGenerator::handleIncludeTypeSerializers(const Poco::CppParser::Variable* pVar, bool toHFile, bool writeDirectly)
{
	const std::string& declType = pVar->declType();
	std::string resType(resolveType(declType));
	Poco::trimInPlace(resType);
	handleIncludeTypeSerializersImpl(resType, toHFile, writeDirectly);
}


void AbstractGenerator::handleIncludeTypeSerializersImpl(const std::string& complexResolvedType, bool toHFile, bool writeDirectly)
{
	std::stack<std::string> notIncludedTypes;
	notIncludedTypes.push(complexResolvedType);
	while (!notIncludedTypes.empty())
	{
		std::string aType = notIncludedTypes.top();
		notIncludedTypes.pop();

		if (GenUtility::isTemplateType(aType))
		{
			std::vector<std::string> innerTypes = GenUtility::getInnerTemplateTypes(aType);
			for (std::size_t i = 0; i < innerTypes.size(); ++i)
			{
				notIncludedTypes.push(innerTypes[i]);
			}
		}
		else
			handleIncludeTypeSerializers(aType, toHFile, writeDirectly);
	}
}


void AbstractGenerator::handleIncludeTypeSerializers(const std::string& simpleType, bool toHFile, bool writeDirectly)
{
	std::string resType(simpleType);
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	if (resType.find("std::") != 0 &&
		(BUILTIN_TYPES.find(resType) == BUILTIN_TYPES.end()))
	{
		resType = resType.substr(0, resType.find("<"));
		Poco::CppParser::Symbol* pSym = Poco::CppParser::NameSpace::root()->lookup(resType);
		if (!pSym)
			pSym = _pStructIn->lookup(resType);
		if (cfg.hasProperty("RemoteGen.output.includeRoot"))
		{
			std::string nameSpace;
			if (pSym) 
			{
				nameSpace = pSym->nameSpace()->fullName();
			}
			else
			{
				std::string::size_type pos = resType.rfind("::");
				if (pos != std::string::npos)
				{
					nameSpace.assign(resType, 0, pos);
				}
			}
			std::string includeBase = cfg.getString(Poco::format("RemoteGen.output.includePrefix[@namespace=%s]", nameSpace), "");
			if (includeBase.empty())
			{
				Poco::Path incDir(includePath(cfg, nameSpace));
				Poco::Path rootDir(cfg.getString("RemoteGen.output.includeRoot"));
				incDir.makeDirectory();
				rootDir.makeDirectory();
				incDir.makeAbsolute();
				rootDir.makeAbsolute();
				std::string incl = incDir.toString(Poco::Path::PATH_UNIX);
				std::string root = rootDir.toString(Poco::Path::PATH_UNIX);
				if (root.size() > incl.size() || root.compare(0, root.size(), incl, 0, root.size()) != 0)
					throw Poco::InvalidArgumentException("The path specified in <includeRoot> must be a parent of the one in <include>", root);
				includeBase = incl.substr(root.size());
			}
			else
			{
				if (includeBase.back() != '/')
				{
					includeBase += '/';
				}
			}
			std::string serType;
			std::string deserType;
			Poco::CppParser::Struct* pStruct = dynamic_cast<Poco::CppParser::Struct*>(pSym);
			if (pStruct)
			{
				serType = SerializerGenerator::generateClassName(pStruct);
				deserType = DeserializerGenerator::generateClassName(pStruct);
			}
			else if (!pSym || pSym->kind() != Poco::CppParser::Symbol::SYM_ENUM)
			{
				Poco::StringTokenizer tok(resType, "::", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
				if (tok.count() == 1)
				{
					serType = *tok.begin()+"Serializer";
					deserType = *tok.begin()+"Deserializer";
				}
				else
				{
					Poco::StringTokenizer::Iterator itClass = tok.end();
					--itClass;
					Poco::StringTokenizer::Iterator itNS = itClass;
					--itNS;
					std::string tmp = *itNS;
					if (*itClass != "AutoPtr" && *itClass != "SharedPtr" && *itNS != "Poco")
					{
						serType = *itClass+"Serializer";
						deserType = *itClass+"Deserializer";
					}
				}
			}
			if (!serType.empty())
				handleIncludeFile(includeBase+serType+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			if (!deserType.empty())
				handleIncludeFile(includeBase+deserType+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			return;
		}
		Poco::CppParser::Struct* pStruct = dynamic_cast<Poco::CppParser::Struct*>(pSym);
		if (pStruct)
		{
			std::string serType = SerializerGenerator::generateClassName(pStruct);
			std::string deserType = DeserializerGenerator::generateClassName(pStruct);
			handleIncludeFile(serType+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			handleIncludeFile(deserType+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			return;
		}

		// Note: the serializer class will probably not exist yet, so don't search in CppParser root namespace
		Poco::StringTokenizer tok(resType, "::", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		if (tok.count() == 1)
		{
			handleIncludeFile(*tok.begin()+"Serializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			handleIncludeFile(*tok.begin()+"Deserializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
		}
		else
		{
			Poco::StringTokenizer::Iterator itClass = tok.end();
			--itClass;
			Poco::StringTokenizer::Iterator itNS = itClass;
			--itNS;
			std::string tmp = *itNS;
			std::string fullNS = Poco::cat(std::string("::"), tok.begin(),itNS);
			if (*itClass != "AutoPtr" && *itClass != "SharedPtr" && *itNS != "Poco")
			{
				std::string incDir = includePath(cfg, fullNS);
				if (!incDir.empty())
				{
					Poco::Path aPath(incDir);
					aPath.makeDirectory();
					if (aPath.depth() > 0 && aPath.directory(aPath.depth() - 1).find("include") != std::string::npos)
					{
						tmp.clear();
					}
					else if (aPath.depth() > 0)
					{
						tmp.clear();
						while (aPath.depth() > 0 && aPath.directory(aPath.depth() - 1).find("include") == std::string::npos)
						{
							if (!tmp.empty())
								tmp = aPath.directory(aPath.depth() - 1) + "/" + tmp;
							else
								tmp = aPath.directory(aPath.depth() - 1);
							aPath = aPath.parent();
						}
					}
					else
						tmp.clear();
				}
				if (tmp.empty())
				{
					handleIncludeFile((*itClass)+"Serializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
					handleIncludeFile((*itClass)+"Deserializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
				}
				else
				{
					handleIncludeFile(tmp+"/"+(*itClass)+"Serializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
					handleIncludeFile(tmp+"/"+(*itClass)+"Deserializer"+Utility::HFILEEXTENSION, toHFile, writeDirectly);
				}
			}
		}
	}
}


void AbstractGenerator::handleIncludeTypes(const Poco::CppParser::Parameter* pParam, bool toHFile, bool writeDirectly)
{
	std::string resType(resolveType(pParam->declType()));

	size_t posVec = resType.find("vector");
	if (posVec != std::string::npos)
	{
		if (writeDirectly)
			_cppGen.writeSystemInclude("vector");
		else
			_cppGen.addSystemIncludeFile("vector");
	}
	std::vector<std::string> templType = GenUtility::getResolvedInnerTemplateTypes(_pStructIn, resType);
	if (!templType.empty())
	{
		for (int i = 0; i < templType.size(); ++i)
		{
			resType = templType[i];
			if (resType.find("std::") != 0 &&
				(BUILTIN_TYPES.find(resType) == BUILTIN_TYPES.end()))
			{
				Poco::StringTokenizer tok(resType, "::");
				if (tok.count() == 1)
					handleIncludeFile(*tok.begin()+Utility::HFILEEXTENSION, toHFile, writeDirectly);
				else
				{
					Poco::StringTokenizer::Iterator itClass = tok.end();
					--itClass;
					Poco::StringTokenizer::Iterator itNS = itClass;
					--itNS;
					handleIncludeFile((*itNS)+"/"+(*itClass)+Utility::HFILEEXTENSION, toHFile, writeDirectly);
				}
			}
		}
	}
	else
	{
		if (resType.find("std::") != 0 &&
			(BUILTIN_TYPES.find(resType) == BUILTIN_TYPES.end()))
		{
			Poco::StringTokenizer tok(resType, "::");
			if (tok.count() == 1)
				handleIncludeFile(*tok.begin()+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			else
			{
				Poco::StringTokenizer::Iterator itClass = tok.end();
				--itClass;
				Poco::StringTokenizer::Iterator itNS = itClass;
				--itNS;
				handleIncludeFile((*itNS)+"/"+(*itClass)+Utility::HFILEEXTENSION, toHFile, writeDirectly);
			}
		}
	}
}


void AbstractGenerator::handleIncludeFile(const std::string& iFile, bool toHFile, bool writeDirectly)
{
	std::string incFile = iFile;
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	bool flat = cfg.getBool("RemoteGen.output.flatIncludes", false);
	if (flat)
	{
		incFile = Poco::CodeGeneration::Utility::createInclude(iFile, true);
	}
	if (toHFile)
	{
		if (writeDirectly)
			_cppGen.writeInclude(incFile, toHFile);
		else
			_cppGen.addIncludeFile(incFile);
	}
	else
	{
		if (writeDirectly)
			_cppGen.writeSrcInclude(incFile);
		else
			_cppGen.addSrcIncludeFile(incFile);
	}
}


std::string AbstractGenerator::resolveType(const std::string& in)
{
	static int recursionCount = 0;
	Poco::CodeGeneration::RecursionGuard recursionGuard(recursionCount, Poco::RuntimeException("Recursion limit reached resolving type", in));

	std::string decl(in);
	Poco::CppParser::Symbol* pSym = _pStructIn->lookup(in);
	if (pSym)
	{
		if (pSym->kind() == Poco::CppParser::Symbol::SYM_TYPEDEF)
		{
			Poco::CppParser::TypeDef* pType = static_cast<Poco::CppParser::TypeDef*>(pSym);
			decl = pType->baseType();
			return resolveType(decl);
		}
		else if (pSym->kind() == Poco::CppParser::Symbol::SYM_TYPEALIAS)
		{
			Poco::CppParser::TypeAlias* pType = static_cast<Poco::CppParser::TypeAlias*>(pSym);
			decl = pType->baseType();
			return resolveType(decl);
		}
	}
	return Utility::resolveType(_pStructIn, decl);
}


std::string AbstractGenerator::includePath(const Poco::Util::AbstractConfiguration& config, const std::string& nameSpace)
{
	return config.getString(
		Poco::format("RemoteGen.output.include[@namespace=%s]", nameSpace), 
		config.getString("RemoteGen.output.include", "")
	);
}


std::string AbstractGenerator::includePath(const Poco::Util::AbstractConfiguration& config, const Poco::CppParser::Struct* pStruct)
{
	if (pStruct && pStruct->nameSpace())
		return includePath(config, pStruct->nameSpace()->fullName());
	else
		return includePath(config, std::string());
}


const std::set<std::string> AbstractGenerator::initBuiltinTypes()
{
	std::set<std::string> builtins;

	builtins.insert("long long");
	builtins.insert("unsigned long long");
	builtins.insert("signed long long");
	builtins.insert("long long int");
	builtins.insert("unsigned long long int");
	builtins.insert("signed long long int");
	builtins.insert("long");
	builtins.insert("unsigned long");
	builtins.insert("signed long");
	builtins.insert("long int");
	builtins.insert("unsigned long int");
	builtins.insert("signed long int");
	builtins.insert("int");
	builtins.insert("unsigned int");
	builtins.insert("signed int");
	builtins.insert("signed");
	builtins.insert("unsigned");
	builtins.insert("short");
	builtins.insert("unsigned short");
	builtins.insert("signed short");
	builtins.insert("short int");
	builtins.insert("unsigned short int");
	builtins.insert("signed short int");
	builtins.insert("char");
	builtins.insert("unsigned char");
	builtins.insert("signed char");
	builtins.insert("bool");
	builtins.insert("long double");
	builtins.insert("double");
	builtins.insert("float");
	builtins.insert("Poco::UInt8");
	builtins.insert("Poco::UInt16");
	builtins.insert("Poco::UInt32");
	builtins.insert("Poco::UInt64");
	builtins.insert("Poco::UIntPtr");
	builtins.insert("Poco::Int8");
	builtins.insert("Poco::Int16");
	builtins.insert("Poco::Int32");
	builtins.insert("Poco::Int64");
	builtins.insert("Poco::IntPtr");
	builtins.insert("UInt8");
	builtins.insert("UInt16");
	builtins.insert("UInt32");
	builtins.insert("UInt64");
	builtins.insert("UIntPtr");
	builtins.insert("Int8");
	builtins.insert("Int16");
	builtins.insert("Int32");
	builtins.insert("Int64");
	builtins.insert("IntPtr");
	builtins.insert("string");
	builtins.insert("std::string");
	builtins.insert("Poco::DateTime");
	builtins.insert("DateTime");
	builtins.insert("Poco::LocalDateTime");
	builtins.insert("LocalDateTime");
	builtins.insert("Poco::Timespan");
	builtins.insert("Timespan");
	builtins.insert("Poco::Timestamp");
	builtins.insert("Timestamp");
	builtins.insert("URI");
	builtins.insert("Poco::URI");
	builtins.insert("UUID");
	builtins.insert("Poco::UUID");
	builtins.insert("ULID");
	builtins.insert("Poco::ULID");

	return builtins;
}
