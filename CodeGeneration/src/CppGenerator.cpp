//
// CppGenerator.cpp
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  CppGenerator
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/String.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Path.h"


namespace Poco {
namespace CodeGeneration {


CppGenerator::CppGenerator(const std::string& nameSpace, const std::string& libraryName, bool usePocoIncludes, const std::string& copyright, std::ostream& hOut, std::ostream& cppOut):
	CodeGenerator(nameSpace, libraryName, usePocoIncludes, copyright),
	_hOut(hOut),
	_cppOut(cppOut),
	_className(),
	_inInlineFct(false),
	_isTemplate(false),
	_inPureVirtualFct(false),
	_lastAcc(Poco::CppParser::Symbol::ACC_PUBLIC),
	_pStruct(0),
	_pFunc(0),
	_attributesEnabled(false),
	_timestampsEnabled(true)
{
}


CppGenerator::~CppGenerator()
{
}


void CppGenerator::enableAttributes(bool enable)
{
	_attributesEnabled = enable;
}


void CppGenerator::enableTimestamps(bool enable)
{
	_timestampsEnabled = enable;
}


void CppGenerator::writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& packageName)
{
	_isTemplate = (pStruct->flags() & Poco::CppParser::Struct::FN_TEMPLATE);
	Poco::Path path(pStruct->getFile());
	Utility::writeStdHeader(_hOut, path.getBaseName() + Utility::HFILEEXTENSION, libraryName, packageName, className, copyright(), _timestampsEnabled);
	if (pStruct->flags() & Poco::CppParser::Struct::FN_TEMPLATE_SPECIALIZATION)
	{
		// write a different include guard which depends on the specialized type too
		const std::string& decl = pStruct->declaration();
		std::size_t templTypeStart = decl.rfind("<");
		std::size_t templTypeEnd = decl.rfind(">");
		poco_assert (templTypeStart != std::string::npos && templTypeEnd != std::string::npos);
		std::string templParam = decl.substr(templTypeStart+1, templTypeEnd-templTypeStart-1);
		Poco::trimInPlace(templParam);
		templParam = Poco::CppParser::replace(templParam, "::", "_");
		poco_assert (!templParam.empty());
		_includeGuard = Utility::createIncludeGuard(libraryName, className+"_"+templParam);
		Utility::writeIncludeGuard(_hOut, libraryName, className+"_"+templParam);
	}
	else
	{
		_includeGuard = Utility::createIncludeGuard(libraryName, className);
		Utility::writeIncludeGuard(_hOut, libraryName, className);
	}
	if (!_isTemplate)
	{
		Utility::writeStdHeader(_cppOut, path.getBaseName() + Utility::CPPFILEEXTENSION, libraryName, packageName, className, copyright(), _timestampsEnabled);
		std::string incPath = Utility::createInclude(pStruct, usePocoIncludeStyle());
		_cppOut << "#include \"" << incPath << "\"\n";
	}
}


void CppGenerator::writeInclude(const std::string& include, bool toHeader)
{
	if (toHeader)
		_hOut << "#include \"" << include << "\"\n";
	else
		writeSrcInclude(include);
}


void CppGenerator::writeSrcInclude(const std::string& include)
{
	_cppOut << "#include \"" << include << "\"\n";
}


void CppGenerator::writeSystemInclude(const std::string& include)
{
	_hOut << "#include <" << include << ">\n";
}


void CppGenerator::writeNameSpaceBegin(const std::string& nameSpace)
{
	writeNameSpaceBegin(nameSpace, false);
}


void CppGenerator::writeNameSpaceBegin(const std::string& nameSpace, bool toHeaderOnly)
{
	_hOut << "\n\n";

	if (!nameSpace.empty())
	{
		Poco::StringTokenizer aTok(nameSpace, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		int nsCnt = (int)aTok.count();
		if (!_isTemplate && !toHeaderOnly)
		{
			_cppOut << "\n\n";
		}
		for (int i = 0; i < nsCnt; ++i)
		{
			std::string nsDecl("namespace " + aTok[i] + " {\n");
			_hOut << nsDecl;
			if (!_isTemplate && !toHeaderOnly)
			{
				_cppOut << nsDecl;
			}
		}
		_hOut << "\n\n";
		if (!_isTemplate && !toHeaderOnly)
		{
			_cppOut << "\n\n";
		}
	}
	else
	{
		if (!_isTemplate && !toHeaderOnly)
			_cppOut << "\n\n";
	}
}


void CppGenerator::writeFwdDecls(const CodeGenerator::FwdDecls& decl)
{
	std::string lastNS;
	bool nsOpen = false;
	bool firstDecl = true;
	CodeGenerator::FwdDecls::const_iterator itFwd = decl.begin();
	CodeGenerator::FwdDecls::const_iterator itFwdEnd = decl.end();
	for (; itFwd != itFwdEnd; ++itFwd)
	{
		// only write fwd decl if the file is not yet included
		if (includes().find(itFwd->includeFile) == includes().end() &&
			systemIncludes().find(itFwd->includeFile) == systemIncludes().end())
		{
			if (_isTemplate)
			{
				// fwd decl in template? not always possible/meaningful
				_hOut << "#include \"" << itFwd->includeFile << "\"\n";
			}
			else
			{
				if (lastNS != itFwd->fullNameSpace)
				{
					// a new namespace
					if (nsOpen)
					{
						_hOut << "\n\n";
						writeNameSpaceEnd(lastNS, true);
					}
					if (firstDecl)
					{
						_hOut << "\n\n";
					}
					writeNameSpaceBegin(itFwd->fullNameSpace, true);
					lastNS = itFwd->fullNameSpace;
					nsOpen = true;
				}
				else if (firstDecl)
				{
					_hOut << "\n\n";
				}
				_hOut << "class " << itFwd->className << ";\n";
				_cppOut << "#include \"" << itFwd->includeFile << "\"\n";
				firstDecl = false;
			}
		}
	}
	if (nsOpen)
	{
		_hOut << "\n\n";
		writeNameSpaceEnd(lastNS, true);
	}
}


void CppGenerator::writeNameSpaceEnd(const std::string& nameSpace)
{
	writeNameSpaceEnd(nameSpace, false);
}


void CppGenerator::writeNameSpaceEnd(const std::string& nameSpace, bool toHeaderOnly)
{
	if (!nameSpace.empty())
	{
		Poco::StringTokenizer aTok(nameSpace, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
		int nsCnt = (int)aTok.count();
		for (int i = 0; i < nsCnt; ++i)
		{
			_hOut << "} // namespace " << aTok[nsCnt-1-i] << "\n";
			if (!_isTemplate && !toHeaderOnly)
			{
				_cppOut << "} // namespace " << aTok[nsCnt-1-i] << "\n";
			}
		}
		_hOut << "\n\n";
		if (!_isTemplate && !toHeaderOnly)
		{
			_cppOut << "\n";
		}
	}
	else
		_hOut << "\n\n";
}


void CppGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStruct = pStruct;
	_className = pStruct->name();
	std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;

	// now write the decl of the class
	Utility::writeClassDeclaration(out, libraryName(), pStruct, _attributesEnabled);
	Utility::writeAccess(out, Poco::CppParser::Symbol::ACC_PUBLIC);
	out << ":\n";
	_lastAcc = Poco::CppParser::Symbol::ACC_PUBLIC;
}


void CppGenerator::structEnd()
{
	std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;

	out << "};\n\n\n";
	// now write the inline methods
	std::vector<InlineFunctionInfo>::const_iterator it = _inlineFunctions.begin();
	std::vector<InlineFunctionInfo>::const_iterator itEnd = _inlineFunctions.end();
	for (; it != itEnd; ++it)
	{
		writeMethodStartCpp(it->pFunc, it->props, out);
		std::vector<std::string>::const_iterator itCode = it->codeLines.begin();
		std::vector<std::string>::const_iterator itCodeEnd = it->codeLines.end();
		for (; itCode != itCodeEnd; ++itCode)
		{
			writeMethodImplementationImpl(*itCode, out);
		}
		methodEndImpl(it->pFunc, it->props, out);
	}
	_className.clear();
	_inlineFunctions.clear();
}


void CppGenerator::methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	handleAccessChange (pFunc->getAccess());
	_pFunc = pFunc;
	_inPureVirtualFct = (pFunc->flags() & Poco::CppParser::Function::FN_PURE_VIRTUAL) != 0;
	if (_isTemplate || _pStruct->isInline() || pFunc->flags() & Poco::CppParser::Function::FN_TEMPLATE)
	{
		writeMethodStartTemplate(pFunc, properties);
	}
	else
	{
		writeMethodStartH(pFunc, properties);
		_inInlineFct = ((pFunc->flags() & Poco::CppParser::Function::FN_INLINE) != 0);

		if (!_inInlineFct && !_inPureVirtualFct)
			writeMethodStartCpp(pFunc, properties, _cppOut);
		else if (!_inPureVirtualFct)
		{
			InlineFunctionInfo info;
			info.pFunc = pFunc;
			info.props = properties;
			_inlineFunctions.push_back(info);
		}
	}
}


void CppGenerator::writeMethodStartCpp(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties, std::ostream& out)
{
	poco_assert_dbg(!_inPureVirtualFct);
	if (pFunc->flags() & Poco::CppParser::Function::FN_INLINE)
	{
		out << "inline ";
	}
	if (!pFunc->getReturnParameter().empty())
	{
		out << pFunc->getReturnParameter() << " ";
	}
	out << _className << "::" << pFunc->name() << "(";
	/// now write the parameters
	Poco::CppParser::Function::Iterator itF = pFunc->begin();
	Poco::CppParser::Function::Iterator itFEnd = pFunc->end();
	if (itF != itFEnd)
	{
		writeParameter(*itF, false, out);
		++itF;
	}
	while (itF != itFEnd)
	{
		out << ", ";
		writeParameter(*itF, false, out);
		++itF;
	}
	if (pFunc->isConstructor())
	{
		out << ")";
		handleMemberInitInConstructor(pFunc, out);
		out << "\n{\n";
	}
	else
	{
		if (pFunc->flags() & Poco::CppParser::Function::FN_CONST)
		{
			out << ") const\n{\n";
		}
		else
		{
			out << ")\n{\n";
		}
	}
}


void CppGenerator::writeMethodStartH(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	writeFunctionHeader(pFunc, true, _hOut);
	_hOut << ";\n";
	if (!pFunc->getDocumentation().empty())
	{
		_hOut << "\t\t///";
		_hOut << Poco::CppParser::replace(pFunc->getDocumentation(), "\n", "\n\t\t///") << "\n\n";
	}
	else
		_hOut << "\n";
}


void CppGenerator::writeFunctionHeader(const Poco::CppParser::Function* pFunc, bool inHFile, std::ostream& out, const std::string& className)
{
	// write properties if in hFile
	if (!inHFile && _inPureVirtualFct)
		return;

	if (inHFile)
	{
		if (_attributesEnabled)
		{
			Utility::writeAttributes(out, pFunc->attrs(), 1);
		}
		out << "\t"; //indentation
		if ( (pFunc->flags() & Poco::CppParser::Function::FN_VIRTUAL) || _inPureVirtualFct)
			out << Utility::VIRTUAL << " ";

		if (pFunc->flags() & Poco::CppParser::Function::FN_STATIC)
			out << Utility::STATIC << " ";
	}

	// write the return type
	if (!pFunc->getReturnParameter().empty())
	{
		out << pFunc->getReturnParameter() << " ";
	}

	if (!className.empty() && !inHFile)
		out << className << "::";

	out << pFunc->name() << "(";

	// now write the parameters
	Poco::CppParser::Function::Iterator itF = pFunc->begin();
	Poco::CppParser::Function::Iterator itFEnd = pFunc->end();
	if (itF != itFEnd)
	{
		writeParameter(*itF, inHFile, out);
		++itF;
	}
	while (itF != itFEnd)
	{
		out << ", ";
		writeParameter(*itF, inHFile, out);
		++itF;
	}
	if (pFunc->flags() & Poco::CppParser::Function::FN_CONST)
		out << ") const";
	else
		out << ")";

	if (inHFile && _inPureVirtualFct)
	{
		out << " = 0";
	}

}


void CppGenerator::writeMethodStartTemplate(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;
	writeFunctionHeader(pFunc, true, out);
	out << "\n";
	if (!pFunc->getDocumentation().empty())
	{
		out << "\t\t///";
		out << Poco::CppParser::replace(pFunc->getDocumentation(), "\n", "\n\t\t///") << "\n";
	}
	out << "\t{\n";
}


void CppGenerator::writeMethodImplementation(const std::string& code)
{
	poco_assert (_pFunc);
	if(_inPureVirtualFct)
		return;

	if (_isTemplate || _pStruct->isInline() || _pFunc->flags() & Poco::CppParser::Function::FN_TEMPLATE)
	{
		std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;
		out << "\t\t";
		out << Poco::CppParser::replace(code, "\n", "\n\t\t");
		out << "\n";
	}
	else if (!_inInlineFct)
	{
		writeMethodImplementationImpl(code, _cppOut);
	}
	else
	{
		_inlineFunctions.back().codeLines.push_back(code);
	}

}

void CppGenerator::writeImplementation(const std::string& code)
{
	if (_isTemplate)
	{
		_hOut << code << "\n";
	}
	else
	{
		_cppOut << code << "\n";
	}
}


void CppGenerator::writeMethodImplementationImpl(const std::string& code, std::ostream& out)
{
	out << "\t";
	out << Poco::CppParser::replace(code, "\n", "\n\t");
	out << "\n";
}


void CppGenerator::methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	if (_isTemplate || _pStruct->isInline() || _pFunc->flags() & Poco::CppParser::Function::FN_TEMPLATE)
	{
		std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;
		out << "\t}\n\n";
	}
	else if (!_inInlineFct && !_inPureVirtualFct)
		methodEndImpl(pFunc, properties, _cppOut);
	_inInlineFct = false;
	_inPureVirtualFct = false;
	_pFunc = 0;
}


void CppGenerator::methodEndImpl(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties, std::ostream& out)
{
	out << "}\n\n\n";
	_inInlineFct = false;
}


void CppGenerator::variablesStart()
{
}


void CppGenerator::variable(const Poco::CppParser::Variable* pVar)
{
	handleAccessChange(pVar->getAccess());
	std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;

	if (_attributesEnabled)
	{
		const Poco::CppParser::Attributes& attrs = pVar->attrs();
		Utility::writeAttributes(out, attrs, 1);
	}
	out << "\t" << pVar->declaration() << ";\n";
}


void CppGenerator::variablesEnd()
{
}


void CppGenerator::writeTypeDef(const Poco::CppParser::TypeDef* pType)
{
	if (!pType)
	{
		_hOut << std::endl;
		return;
	}
	handleAccessChange(pType->getAccess());
	_hOut << "\t" << pType->declaration() << ";" << std::endl;
}


void CppGenerator::writeUsing(const Poco::CppParser::TypeAlias* pType)
{
	if (!pType)
	{
		_hOut << std::endl;
		return;
	}
	handleAccessChange(pType->getAccess());
	_hOut << "\t" << pType->declaration() << ";" << std::endl;
}


void CppGenerator::writeParameter(const Poco::CppParser::Parameter* pParam)
{
	if (!_pStruct->isInline())
	{
		_hOut << Poco::trim(pParam->declaration());
		if (pParam->hasDefaultValue())
			_hOut << " = " << Utility::resolveType(_pStruct, pParam->declType()) << "(" << pParam->defaultValue() << ")";
	}
	_cppOut << Poco::trim(pParam->declaration());
	if (_pStruct->isInline())
	{
		if (pParam->hasDefaultValue())
			_cppOut << " = " << Utility::resolveType(_pStruct, pParam->declType()) << "(" << pParam->defaultValue() << ")";
	}
}


void CppGenerator::writeParameter(const Poco::CppParser::Parameter* pParam, bool inHFile, std::ostream& out)
{
	out << pParam->declaration();
	if (_pStruct->isInline() || inHFile)
	{
		if (pParam->hasDefaultValue())
			out << " = " << Utility::resolveType(_pStruct, pParam->declType()) << "(" << pParam->defaultValue() << ")";
	}
}


void CppGenerator::handleAccessChange(Poco::CppParser::Symbol::Access newAcc)
{
	if (newAcc != _lastAcc)
	{
		std::ostream& out = _pStruct->isInline()? _cppOut : _hOut;
		Utility::writeAccess(out, newAcc);
		out << ":\n";
		_lastAcc = newAcc;
	}
}


void CppGenerator::handleMemberInitInConstructor(const Poco::CppParser::Function* pFunc, std::ostream& out)
{
	poco_assert_dbg(_pStruct);
	// the question is: do we have any member variables that we can initialize?
	Poco::CppParser::NameSpace::SymbolTable varsAll;
	_pStruct->variables(varsAll);
	// remove static vars! and store the remaining in vars
	Poco::CppParser::NameSpace::SymbolTable vars;
	Poco::CppParser::NameSpace::SymbolTable::iterator it = varsAll.begin();
	Poco::CppParser::NameSpace::SymbolTable::iterator itEnd = varsAll.end();
	for (; it != itEnd; ++it)
	{
		Poco::CppParser::Variable* pVar = static_cast<Poco::CppParser::Variable*>(it->second);
		if ((pVar->flags() & Poco::CppParser::Variable::VAR_STATIC) == 0)
			vars.insert(std::make_pair(it->first, it->second));
	}

	Poco::CppParser::Struct::BaseIterator itB = _pStruct->baseBegin();
	Poco::CppParser::Struct::BaseIterator itBEnd = _pStruct->baseEnd();

	it = vars.begin();
	itEnd = vars.end();
	if (itB != itBEnd || it != itEnd)
		out << ":\n";
	for (; itB != itBEnd; ++itB)
	{
		// call for each base class the correct constructor,
		// we assume that the parameters do have the same name in the subclass than
		// in the super class, so basically it CAN work, but there is NO guarantee
		// that it will always work...

		out << "\t" << itB->name << "(";
		Poco::CppParser::Struct::BaseIterator itBBeforeEnd = _pStruct->baseEnd();
		--itBBeforeEnd;
		// now do parameter magic
		// take the decl of the super class constructor, use it. if we find more than one, guess.
		// when using virtual inheritance always take the default constructor!
		if (itB->pClass && !itB->isVirtual)
		{
			Poco::CppParser::Struct* pParent = itB->pClass;
			Poco::CppParser::Struct::Functions fct;
			pParent->constructors(fct);
			Poco::CppParser::Struct::Functions::const_iterator itF = fct.begin();
			Poco::CppParser::Struct::Functions::const_iterator itBestMatch = fct.end();
			int bestMatchCnt = 0;
			Poco::CppParser::Struct::Functions::const_iterator itFEnd = fct.end();
			for (; itF != itFEnd; ++itF)
			{
				const Poco::CppParser::Function* pParentConstr = *itF;
				Poco::CppParser::Function::Iterator itP = pFunc->begin();
				Poco::CppParser::Function::Iterator itPEnd = pFunc->end();
				int cnt = 0;
				for (; itP != itPEnd; ++itP)
				{
					// only compare parameter decl!
					if (findMatchingDecl(pParentConstr, (*itP)))
						cnt++;
				}
				if (cnt > bestMatchCnt)
					itBestMatch = itF;
			}
			if (itBestMatch != fct.end())
			{
				const Poco::CppParser::Function* pBestConstr = *itBestMatch;
				Poco::CppParser::Function::Iterator itP = pBestConstr->begin();
				Poco::CppParser::Function::Iterator itPEnd = pBestConstr->end();
				Poco::CppParser::Function::Iterator itPBeforeEnd = itPEnd;
				--itPBeforeEnd;

				for (; itP != itPEnd; ++itP)
				{
					out << (*itP)->name();
					if (itP != itPBeforeEnd)
						out << ", ";
				}
			}
		}

		out << ")";
		if (itB != itBBeforeEnd)
			out << ",\n";
		else
		{
			if (vars.empty())
				out << "\n";
			else
				out << ",\n";
		}
	}

	// now search for each member in pFunc, the matching member in vars

	bool first = true;
	for (; it != itEnd; ++it)
	{
		if (!first)
		{
			out << ",\n";
		}
		const Poco::CppParser::Variable* pVar = static_cast<const Poco::CppParser::Variable*>(it->second);

		out << "\t" << pVar->name() << "(";
		// is there a matching member in pFunc?
		// we assume that the only difference between member vars and init vars name is a "_"
		// remove the "_" from the var name if there is one
		// we never init a member which doesn't begin with _
		ConstructorMapping::const_iterator itMap = _constrMapping.find(pVar);
		SimpleConstructorMapping::const_iterator itMapS = _simpleConstrMapping.find(pVar);
		if (itMap != _constrMapping.end() || itMapS != _simpleConstrMapping.end())
		{
			if (itMap != _constrMapping.end())
			{
				const Poco::CppParser::Parameter* pParam = itMap->second;
				out << pParam->name();
			}
			else
			{
				poco_assert_dbg (itMapS != _simpleConstrMapping.end());
				out << itMapS->second;
			}
		}
		else if(pVar->name()[0]=='_')
		{
			std::string expectedParamName(pVar->name().substr(1));
			// if we find the parameter init with the paramname, otherwise init empty
			const Poco::CppParser::Parameter* pParam = findParameter(pFunc, expectedParamName);
			if (pParam)
			{
				out << pParam->name();
			}
			else if (pVar->isPointer())
				out << "0";
		}
		else if (pVar->isPointer())
			out << "0";
		out << ")";

		first = false;
	}
}


const Poco::CppParser::Parameter* CppGenerator::findParameter(const Poco::CppParser::Function* pFunc, const std::string& paramName)
{
	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->name() == paramName)
			return *it;
	}
	return 0;
}


bool CppGenerator::findMatchingDecl(const Poco::CppParser::Function* pParentFunc, const Poco::CppParser::Parameter* pParam)
{
	const std::string& dN = pParam->name();
	std::string resDT(Utility::resolveType(_pStruct->nameSpace(), pParam->declType()));
	Poco::CppParser::Function::Iterator it = pParentFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pParentFunc->end();

	for (; it != itEnd; ++it)
	{
		std::string res(Utility::resolveType(_pStruct, (*it)->declType()));
		if (res == resDT && (*it)->name() == dN)
			return true;
	}

	return false;
}


void CppGenerator::endFile()
{
	_hOut << "#endif";
	if (!_includeGuard.empty())
	{
		_hOut << " // " << _includeGuard;
		_includeGuard.clear();
	}
	_hOut << "\n\n";
}


} } // namespace Poco::CodeGeneration
