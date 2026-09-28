//
// CppWriter.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppWriter.h"
#include "BuiltinTypes.h"
#include "Poco/File.h"
#include "Poco/String.h"
#include "Poco/StringTokenizer.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberParser.h"
#include <fstream>


const std::string CppWriter::EXT_H("h");
const std::string CppWriter::EXT_CPP("cpp");


CppWriter::CppWriter(const SchemaInfo& si, int options, const std::string& amalgamatedSourceFileName):
	_info(si),
	_options(options),
	_amalgamatedSourceFileName(amalgamatedSourceFileName),
	_logger(Poco::Logger::get("CppWriter"))
{
	if (options & OPT_USE_STD)
	{
		_sharedPtr = "std::shared_ptr";
		_optional = "std::optional";
	}
	else
	{
		_sharedPtr = "Poco::SharedPtr";
		_optional = "Poco::Optional";
	}
}


CppWriter::~CppWriter()
{
}


void CppWriter::updateIncludeDir(ClassInfo& cInfo) const
{
	std::string incFile = _info.createInclude(cInfo.name());
	cInfo.setIncludeFile(incFile, false);
}


void CppWriter::generate(const std::map<std::string, ClassInfo>& classes)
{
	std::map<std::string, ClassInfo>::const_iterator it = classes.begin();
	for (; it != classes.end(); ++it)
		generate(it->second);
}


void CppWriter::generate(const ClassInfo& cinfo)
{
	static BuiltinTypes& built = BuiltinTypes::instance();
	if (built.isKnownTypeInfo(cinfo.getTypeInfo()))
		return;

	if (cinfo.name().find('<') != std::string::npos) return;

	_logger.debug("generating class " + cinfo.name());

	if (cinfo.getVariables().empty())
	{
		_logger.debug("class has no members: " + cinfo.name());
	}

	Poco::File srcFile(_info.sourceDir());
	srcFile.createDirectories();

	Poco::File incFile(_info.includeDir());
	incFile.createDirectories();

	Poco::Path cppFile(_info.sourceDir());
	cppFile.makeDirectory();
	if (_options & OPT_AMALGAMATE_SOURCES)
	{
		cppFile.setBaseName(_amalgamatedSourceFileName);
	}
	else if (_options & OPT_INCLUDE_NAMESPACE_IN_SOURCE_FILENAME)
	{
		std::string name = cinfo.getNameSpace();
		Poco::replaceInPlace(name, std::string("::"), std::string("_"));
		name += "_";
		name += cinfo.name();
		cppFile.setBaseName(name);
	}
	else
	{
		cppFile.setBaseName(cinfo.name());
	}
	cppFile.setExtension(EXT_CPP);

	Poco::Path hFile(_info.includeDir());
	hFile.makeDirectory();
	if (_options & OPT_INCLUDE_NAMESPACE_IN_HEADER_FILENAME)
	{
		std::string name = cinfo.getNameSpace();
		Poco::replaceInPlace(name, std::string("::"), std::string("_"));
		name += "_";
		name += cinfo.name();
		hFile.setBaseName(name);
	}
	else
	{
		hFile.setBaseName(cinfo.name());
	}
	hFile.setExtension(EXT_H);

	std::ofstream hStr(hFile.toString().c_str());
	if (!hStr.good()) throw Poco::OpenFileException(hFile.toString());
	std::ofstream cppStr(cppFile.toString().c_str(), (_options & OPT_AMALGAMATE_SOURCES) != 0 ? std::ios::app : std::ios::trunc);
	if (!cppStr.good()) throw Poco::OpenFileException(cppFile.toString());

	_classNameSpace = cinfo.getTypeInfo().getNameSpace();
	writeHeader(hStr, hFile.getFileName());
	if ((_options & OPT_AMALGAMATE_SOURCES) == 0)
	{
		writeHeader(cppStr, cppFile.getFileName());
	}
	std::string incGuard = generateHeaderFile(hStr, cinfo);
	bool cppHasContent = generateSourceFile(cppStr, hStr, cinfo);
	writeNamespaceEnd(hStr, cinfo.getNameSpace());

	std::set<std::string> extraIncludes;
	allExtraIncludeFiles(cinfo, extraIncludes);
	if (!extraIncludes.empty())
	{
		hStr << "// The following headers are required for template instantiation." << std::endl;
		writeInclude(hStr, extraIncludes, false);
		hStr << std::endl << std::endl;
	}

	writeIncludeGuardEnd(hStr, incGuard);

	hStr.close();
	cppStr.close();

	if ((_options & OPT_AMALGAMATE_SOURCES) == 0 && !cppHasContent)
	{
		Poco::File f(cppFile.toString());
		f.remove();
	}
}


std::string CppWriter::generateHeaderFile(std::ostream& out, const ClassInfo& cinfo)
{
	std::string incGuard = writeIncludeGuardBegin(out, cinfo.getTypeInfo());
	std::set<std::string> includes(_info.extraIncludes());
	std::set<std::string> sysIncludes;
	if (_options & OPT_USE_STD)
		sysIncludes.insert("optional");
	else
		includes.insert("Poco/Optional.h");
	includes.insert("Poco/Nullable.h");
	if (_options & OPT_USE_STD)
		sysIncludes.insert("memory");
	else
		includes.insert("Poco/SharedPtr.h");
	sysIncludes.insert("string");
	sysIncludes.insert("vector");
	std::set<std::string> fwdDeclares;
	allIncludeFiles(cinfo, includes, sysIncludes, fwdDeclares);
	writeInclude(out, includes, false);
	writeInclude(out, sysIncludes, true);
	out << std::endl << std::endl;
	writeFwdDecls(out, fwdDeclares);
	if (!fwdDeclares.empty()) out << std::endl << std::endl;
	writeNamespaceBegin(out, cinfo.getNameSpace());
	// now write class
	writeClassStart(out, cinfo);
	// write method visibility
	bool visWritten = false;
	Utility::Access lastAccess = Utility::AC_PRIVATE;
	const std::vector<Constructor>& constr = cinfo.getConstructors();
	std::vector<Constructor>::const_iterator itC = constr.begin();
	for (; itC != constr.end(); ++itC)
	{
		if (!visWritten)
		{
			visWritten = true;
			writeAccessibility(out, itC->getAccess());
			lastAccess = itC->getAccess();
		}
		checkAccessibility(out, lastAccess, itC->getAccess());
		writeConstructorHeader(out, *itC);
		lastAccess = itC->getAccess();
	}

	if (!visWritten)
	{
		visWritten = true;
		writeAccessibility(out, cinfo.getDestructor().getAccess());
		lastAccess = cinfo.getDestructor().getAccess();
	}

	checkAccessibility(out, lastAccess, cinfo.getDestructor().getAccess());
	writeDestructorHeader(out, cinfo.getDestructor());
	lastAccess = cinfo.getDestructor().getAccess();

	const std::multimap<std::string, MethodInfo>& methods = cinfo.getMethods();
	std::multimap<std::string, MethodInfo>::const_iterator itM = methods.begin();
	for (; itM != methods.end(); ++itM)
	{
		if (!visWritten)
		{
			visWritten = true;
			writeAccessibility(out, itM->second.getAccess());
			lastAccess = itM->second.getAccess();
		}
		if (itM != methods.begin())
			out << std::endl;
		checkAccessibility(out, lastAccess, itM->second.getAccess());
		writeMethodHeader(out, itM->second);
		lastAccess = itM->second.getAccess();
	}

	const std::map<int, Variable>& vars = cinfo.getVariables();
	std::map<int, Variable>::const_iterator itV = vars.begin();
	for (; itV != vars.end(); ++itV)
	{
		if (!visWritten)
		{
			visWritten = true;
			writeAccessibility(out, itV->second.getAccess());
			lastAccess = itV->second.getAccess();
		}

		checkAccessibility(out, lastAccess, itV->second.getAccess());
		writeRemotingAttributes(out, itV->second.getAll(), 1);
		out << "\t";
		writeVariable(out, itV->second);
		out << ";" << std::endl << std::endl;
		lastAccess = itV->second.getAccess();
	}

	writeClassEnd(out, cinfo);
	return incGuard;
}


bool CppWriter::generateSourceFile(std::ostream& out, std::ostream& hStr, const ClassInfo& cinfo)
{
	bool hasContent = false;
	writeInclude(out, cinfo.getIncludeFile(), false);
	writeInclude(out, cinfo.getSrcIncludes(), false);
	writeInclude(out, cinfo.getSrcSystemIncludes(), true);

	out << std::endl << std::endl;
	writeNamespaceBegin(out, cinfo.getNameSpace());

	const std::vector<Constructor>& constr = cinfo.getConstructors();
	std::vector<Constructor>::const_iterator itC = constr.begin();
	for (; itC != constr.end(); ++itC)
	{
		if (_options & OPT_INLINE_ALL)
		{
			writeConstructorSrc(hStr, *itC, cinfo, true);
		}
		else
		{
			hasContent = true;
			writeConstructorSrc(out, *itC, cinfo, false);
		}
	}

	if (_options & OPT_INLINE_ALL)
	{
		writeDestructorSrc(hStr, cinfo.getDestructor(), true);
	}
	else
	{
		hasContent = true;
		writeDestructorSrc(out, cinfo.getDestructor(), false);
	}

	const std::multimap<std::string, MethodInfo>& methods = cinfo.getMethods();
	std::multimap<std::string, MethodInfo>::const_iterator itM = methods.begin();
	for (; itM != methods.end(); ++itM)
	{
		if (itM->second.getCode().size() < 2 || (_options & OPT_INLINE_ALL) != 0)
		{
			writeMethodSrc(hStr, cinfo, itM->second, true);
		}
		else
		{
			hasContent = true;
			writeMethodSrc(out, cinfo, itM->second, false);
		}
	}

	writeNamespaceEnd(out, cinfo.getNameSpace());
	return hasContent;
}


void CppWriter::writeComment(std::ostream& out, const std::string& line, int ind)
{
	static const std::string prefix("// ");
	writePrefixedLine(out, prefix, line, ind);
}


void CppWriter::writeComment(std::ostream& out, const std::vector<std::string>& lines, int indent)
{
	std::vector<std::string>::const_iterator it = lines.begin();
	for (; it != lines.end(); ++it)
	{
		writeComment(out, *it);
	}
}


void CppWriter::writeHeader(std::ostream& out, const std::string& fileName)
{
	Poco::DateTime now;
	writeComment(out, "");
	writeComment(out, fileName);
	writeComment(out, "");
	if (_options & OPT_HEADER_TIMESTAMPS)
	{
		writeComment(out, Poco::DateTimeFormatter::format(now, "This file has been generated on %Y-%m-%d %H:%M:%S UTC."));
	}
	else
	{
		writeComment(out, "This file has been generated.");
	}
	writeComment(out, "Warning: All changes to this will be lost when the file is re-generated.");
	writeComment(out, "");
	writeComment(out, _info.copyright());
	writeComment(out, "");
	out << std::endl << std::endl;
}


void CppWriter::writeInclude(std::ostream& out, const std::set<std::string>& includes, bool systemInc)
{
	std::set<std::string>::const_iterator it = includes.begin();
	for (; it != includes.end(); ++it)
	{
		writeInclude(out, *it, systemInc);
	}
}


void CppWriter::writeInclude(std::ostream& out, const std::string& include, bool systemInc)
{
	if (include.empty())
		return;
	out << "#include ";
	if (systemInc)
		out << "<";
	else
		out << "\"";

	out << include;

	if (systemInc)
		out << ">";
	else
		out << "\"";
	out << std::endl;
}


void CppWriter::writeFwdDecls(std::ostream& out, const std::set<std::string>& fwds)
{
	std::set<std::string>::const_iterator it = fwds.begin();
	std::string lastNS;
	std::string closeNSLine;

	for (; it != fwds.end(); ++it)
	{
		const std::string& fwd = *it;
		std::string::size_type pos = fwd.rfind("::");
		std::string ns;
		std::string name;
		if (pos != std::string::npos)
		{
			ns = fwd.substr(0, pos);
			name = fwd.substr(pos+2);
		}
		else
			name = fwd;

		if (ns != lastNS)
		{
			if (!closeNSLine.empty())
				out << closeNSLine << std::endl;
			closeNSLine.clear();
			Poco::StringTokenizer tok(ns, ":", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
			Poco::StringTokenizer::Iterator it = tok.begin();
			for (; it != tok.end(); ++it)
			{
				out << "namespace " << *it << " {" << std::endl;
				closeNSLine += "} ";
			}
			lastNS = ns;
		}
		out << "class " << name << ";" << std::endl;
	}
	if (!closeNSLine.empty())
		out << closeNSLine << std::endl;
}


std::string CppWriter::writeIncludeGuardBegin(std::ostream& out, const TypeInfo& info)
{
	std::string incString = Poco::replace(info.getNameSpace(), "::", "_");
	if (!incString.empty())
		incString.append("_");
	incString.append(info.name());
	incString.append("_INCLUDED");
	out << "#ifndef " << incString << std::endl;
	out << "#define " << incString << std::endl;
	out << std::endl << std::endl;
	return incString;
}


void CppWriter::writeIncludeGuardEnd(std::ostream& out, const std::string& incGuard)
{
	out << "#endif // " << incGuard << std::endl;
}


void CppWriter::allIncludeFiles(const ClassInfo& info, std::set<std::string>& includes, std::set<std::string>& sysIncludes, std::set<std::string>& fwdDecl)
{
	if (!info.getParent().getIncludeFile().empty())
	{
		Poco::StringTokenizer tok(info.getParent().getIncludeFile(),",;", Poco::StringTokenizer::TOK_IGNORE_EMPTY| Poco::StringTokenizer::TOK_TRIM);
		if (info.getParent().isSystemInclude())
		{
			poco_assert(tok.count() == 1);
			sysIncludes.insert (info.getParent().getIncludeFile());
		}
		else
		{
			Poco::StringTokenizer::Iterator itTok = tok.begin();
			for (; itTok != tok.end(); ++itTok)
			{
				if (*itTok == "string")
					sysIncludes.insert(*itTok);
				else
					includes.insert(*itTok);
			}
		}
	}

	const std::map<int, Variable>& vars = info.getVariables();
	std::map<int, Variable>::const_iterator it = vars.begin();
	for (; it != vars.end(); ++it)
	{
		// builtin types often don't need an include (like float)
		if (!it->second.getType().getIncludeFile().empty())
		{
			if (canForwardDeclare(it->second))
			{
				fwdDecl.insert(it->second.getType().getFullName());
			}
			else
			{
				Poco::StringTokenizer tok(it->second.getType().getIncludeFile(),",;", Poco::StringTokenizer::TOK_IGNORE_EMPTY| Poco::StringTokenizer::TOK_TRIM);
				if (it->second.getType().isSystemInclude())
				{
					poco_assert (tok.count() == 1);
					sysIncludes.insert(it->second.getType().getIncludeFile());
				}
				else
				{
					Poco::StringTokenizer::Iterator itTok = tok.begin();
					for (; itTok != tok.end(); ++itTok)
					{
						if (*itTok == "string")
							sysIncludes.insert(*itTok);
						else
							includes.insert(*itTok);
					}
				}
			}
		}
		if (it->second.isVector())
			sysIncludes.insert("vector");
	}

	const std::multimap<std::string, MethodInfo>& methods = info.getMethods();
	std::multimap<std::string, MethodInfo>::const_iterator itM = methods.begin();
	for (; itM != methods.end(); ++itM)
	{
		const MethodInfo& mi = itM->second;
		const std::map<int, Parameter>& params = mi.getParameters();
		Poco::SharedPtr<Parameter> pRet = mi.getReturnParameter();
		if (pRet)
		{
			if (canForwardDeclare(*pRet))
				fwdDecl.insert (pRet->getType().getFullName());
			else
			{
				// builtin types often don't need an include (like float)
				if (!pRet->getType().getIncludeFile().empty())
				{
					//Problem are list types XSDVEctor<Poco::URI> -> need to include two files!
					Poco::StringTokenizer tok(pRet->getType().getIncludeFile(),",;", Poco::StringTokenizer::TOK_IGNORE_EMPTY| Poco::StringTokenizer::TOK_TRIM);
					if (pRet->getType().isSystemInclude())
					{
						poco_assert (tok.count() == 1);
						sysIncludes.insert(pRet->getType().getIncludeFile());
					}
					else
					{
						Poco::StringTokenizer::Iterator itTok = tok.begin();
						for (; itTok != tok.end(); ++itTok)
						{
							if (*itTok == "string")
								sysIncludes.insert(*itTok);
							else
								includes.insert(*itTok);
						}
					}
				}
			}

			if (pRet->isVector())
				sysIncludes.insert("vector");
		}
		std::map<int, Parameter>::const_iterator itP = params.begin();
		for (; itP != params.end(); ++itP)
		{
			if (canForwardDeclare(itP->second))
				fwdDecl.insert (itP->second.getType().getFullName());
			else
			{
				// builtin types often don't need an include (like float)
				if (!itP->second.getType().getIncludeFile().empty())
				{
					//Problem are list types XSDVEctor<Poco::URI> -> need to include two files!
					Poco::StringTokenizer tok(itP->second.getType().getIncludeFile(),",;", Poco::StringTokenizer::TOK_IGNORE_EMPTY| Poco::StringTokenizer::TOK_TRIM);
					if (itP->second.getType().isSystemInclude())
					{
						poco_assert (tok.count() == 1);
						sysIncludes.insert(itP->second.getType().getIncludeFile());
					}
					else
					{
						Poco::StringTokenizer::Iterator itTok = tok.begin();
						for (; itTok != tok.end(); ++itTok)
						{
							if (*itTok == "string")
								sysIncludes.insert(*itTok);
							else
								includes.insert(*itTok);
						}
					}
				}
			}

			if (itP->second.isVector())
				sysIncludes.insert("vector");
		}
	}
}


void CppWriter::allExtraIncludeFiles(const ClassInfo& info, std::set<std::string>& includes)
{
	const std::map<int, Variable>& vars = info.getVariables();
	std::map<int, Variable>::const_iterator it = vars.begin();
	for (; it != vars.end(); ++it)
	{
		// builtin types often don't need an include (like float)
		if (!it->second.getType().getIncludeFile().empty())
		{
			if (canForwardDeclare(it->second))
			{
				includes.insert(it->second.getType().getIncludeFile());
			}
		}
	}
}


void CppWriter::writeNamespaceBegin(std::ostream& out, const std::string& ns)
{
	if (ns.empty())
		return;

	Poco::StringTokenizer tok(ns, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY|Poco::StringTokenizer::TOK_TRIM);
	Poco::StringTokenizer::Iterator it = tok.begin();
	for (; it != tok.end(); ++it)
	{
		out << "namespace " << *it << " {" << std::endl;
	}
	out << std::endl << std::endl;
}


void CppWriter::writeNamespaceEnd(std::ostream& out, const std::string& ns)
{
	if (ns.empty())
		return;

	Poco::StringTokenizer tok(ns, ":", Poco::StringTokenizer::TOK_IGNORE_EMPTY|Poco::StringTokenizer::TOK_TRIM);
	for (std::size_t i = 0; i < tok.count(); ++i)
	{
		out << "} ";
	}
	out << "// " << ns << std::endl;
	out << std::endl << std::endl;
}


void CppWriter::writeClassStart(std::ostream& out, const ClassInfo& cinfo)
{
	// write remoting attributes
	writeRemotingAttributes(out, cinfo.getAll(), 0);
	out << "class ";
	if (!cinfo.getDllExportMacro().empty())
		out << cinfo.getDllExportMacro() << " ";
	out << cinfo.name();
	if (!cinfo.getParent().name().empty())
	{
		out << ": public ";
		writeTypeInfo(out, cinfo.getParent());
	}
	out << std::endl << "{" << std::endl;
}


void CppWriter::writeClassEnd(std::ostream& out, const ClassInfo& cinfo)
{
	out << "};";
	out << std::endl << std::endl << std::endl;
}


void CppWriter::writeAccessibility(std::ostream& out, Utility::Access access)
{
	if (access == Utility::AC_PUBLIC)
	{
		out << "public:" << std::endl;
	}
	else if (access == Utility::AC_PROTECTED)
	{
		out << "protected:" << std::endl;
	}
	else
	{
		out << "private:" << std::endl;
	}
}


void CppWriter::checkAccessibility(std::ostream& out, Utility::Access oldAccess, Utility::Access newAccess)
{
	if (oldAccess != newAccess)
	{
		out << std::endl;
		writeAccessibility(out, newAccess);
	}
}


void CppWriter::writeConstructorHeader(std::ostream& out, const Constructor& constr)
{
	out << "\t" << constr.name() << "(";
	writeParameterList(out, constr.getParameters(), false);
	out << ");" << std::endl;
	if (!constr.getDocu().empty())
	{
		writeComment(out, constr.getDocu(), 2);
	}
	out << std::endl;
}


void CppWriter::writeConstructorSrc(std::ostream& out, const Constructor& constr, const ClassInfo& info, bool doInline)
{
	if (doInline)
		out << "inline ";
	
	out << constr.name() << "::" << constr.name() << "(";
	writeParameterList(out, constr.getParameters());
	out << ")";
	if (constr.getInitializationCode().empty())
	{
		const std::map<int, Variable>& vars = info.getVariables();
		if (!vars.empty())
		{
			out << ":" << std::endl;
			bool writeColon = false;
			std::map<int, Variable>::const_iterator it = vars.begin();
			for (; it != vars.end(); ++it)
			{
				if (writeColon)
				{
					out << "," << std::endl;
				}
				out << "\t" << it->second.getName() + "()";
				writeColon = true;
			}
		}
	}
	else
	{
		out << ":" << std::endl;

		// we have to make sure that the init order matches the var order
		// and fill possible gaps
		std::vector<std::string> code = constr.getInitializationCode();
		bool writeColon = false;
		if (code.size() > 0)
		{
			// super constructor call
			if (code[0][0] != '_')
			{
				out << "\t" << code[0];
				writeColon = true;
				code.erase(code.begin());
			}
		}
		const std::map<int, Variable>& vars = info.getVariables();
		std::map<int, Variable>::const_iterator it = vars.begin();
		for (; it != vars.end(); ++it)
		{
			if (writeColon)
			{
				out << "," << std::endl;
			}
			std::vector<std::string>::iterator itC = code.begin();
			while(itC != code.end() && itC->find(it->second.getName()))
				++itC;
			if (itC != code.end())
			{
				out << "\t" << *itC;
				code.erase(itC);
			}
			else
				out << "\t" << it->second.getName() + "()";

			writeColon = true;
		}
		poco_assert (code.empty());
	}
	out << std::endl;
	out << "{" << std::endl;
	writeCode(out, constr.getCode(), 1);
	out << "}" << std::endl << std::endl << std::endl;
}


void CppWriter::writeParameterList(std::ostream& out, const std::map<int, Parameter>& params, bool singleLine)
{
	std::map<int, Parameter>::const_iterator it = params.begin();
	for (; it != params.end(); ++it)
	{
		if (it != params.begin())
		{
			out << ", ";
		}
		if (!singleLine && params.size() > 1)
		{
			out << "\n\t\t";
		}
		writeParameter(out, it->second);
	}
}


void CppWriter::writeDestructorHeader(std::ostream& out, const Destructor& destr)
{
	out << "\t";
	if (destr.isVirtual())
		out << "virtual ";
	out << destr.name() << "();" << std::endl;
	if (!destr.getDocu().empty())
	{
		writeComment(out, destr.getDocu(), 2);
	}
	out << std::endl;
}


void CppWriter::writeDestructorSrc(std::ostream& out, const Destructor& destr, bool doInline)
{
	if (doInline)
		out << "inline ";
	out << destr.name().substr(1) << "::" << destr.name() << "()" << std::endl;
	out << "{" << std::endl;
	writeCode(out, destr.getCode());
	out << "}" << std::endl << std::endl << std::endl;
}


void CppWriter::writeParameter(std::ostream& out, const Parameter& var, bool isRetParam)
{
	bool isOptional = var.isOptional() && !var.isVector() && (!var.isNillable() || (_options & OPT_PRESERVE_OPTIONAL) != 0);
	bool isNillable = var.isNillable() || (var.isOptional() && !var.isVector() && (_options & OPT_PRESERVE_OPTIONAL) == 0 && !isOptional);
	bool isPointer = var.isPointer();

	if (var.isConst() && !var.isRVRef())
		out << "const ";
	if (var.isVector())
		out << "std::vector<";
	if (isPointer)
	{
		out << _sharedPtr << "<";
	}
	else if (isOptional)
	{
		if ((_options & OPT_ALWAYS_USE_OPTIONAL) || BuiltinTypes::instance().isKnownTypeInfo(var.getType()))
			out << _optional << "<";
		else
			out << _sharedPtr << "<";
	}
	else if (isNillable)
	{
		if (BuiltinTypes::instance().isKnownTypeInfo(var.getType()))
			out << "Poco::Nullable<";
		else
			out << _sharedPtr << "<";
	}

	writeTypeInfo(out, var.getType());

	if (isNillable || isOptional || isPointer)
		out << ">";
	if (var.isVector())
		out << ">";
	if (var.isRVRef())
		out << "&& ";
	else if (var.isRef())
		out << "& ";
	else
		out << " ";

	if (!isRetParam)
		out << var.getName();
}


void CppWriter::writeVariable(std::ostream& out, const Variable& var)
{
	bool isOptional = var.isOptional() && !var.isVector() && (!var.isNillable() || (_options & OPT_PRESERVE_OPTIONAL) != 0);
	bool isNillable = var.isNillable() || (var.isOptional() && !var.isVector() && (_options & OPT_PRESERVE_OPTIONAL) == 0 && !isOptional);
	bool isPointer = var.isPointer();

	if (var.isConst())
		out << "const ";
	if (var.isVector())
		out << "std::vector<";
	if (isPointer)
		out << _sharedPtr << "<";
	else if (isOptional)
	{
		if ((_options & OPT_ALWAYS_USE_OPTIONAL) || BuiltinTypes::instance().isKnownTypeInfo(var.getType()))
			out << _optional << "<";
		else
			out << _sharedPtr << "<";
	}
	else if (isNillable)
	{
		if (BuiltinTypes::instance().isKnownTypeInfo(var.getType()))
			out << "Poco::Nullable<";
		else
			out << _sharedPtr << "<";
	}

	writeTypeInfo(out, var.getType());

	if (isNillable || isOptional || isPointer)
		out << ">";
	if (var.isVector())
		out << ">";
	if (var.isRef())
		out << "& ";
	else
		out << " ";
	out << var.getName();
}


void CppWriter::writeTypeInfo(std::ostream& out, const TypeInfo& info)
{
	condWriteNamespace(out, info.getNameSpace());
	out << info.name();
}


void CppWriter::condWriteNamespace(std::ostream& out, const std::string& nameSpace)
{
	if (nameSpace != _classNameSpace && !nameSpace.empty())
		out << nameSpace << "::";
}


void CppWriter::writeMethodHeader(std::ostream& out, const MethodInfo& mi)
{
	writeMethodRemotingAttributes(out, mi, 1);

	out << "\t";
	if (mi.isStatic())
		out << "static ";
	if (mi.isAbstract() || mi.isVirtual())
		out << "virtual ";
	Poco::SharedPtr<Parameter> pRet = mi.getReturnParameter();
	if (pRet)
		writeParameter(out, *pRet, true);
	else
		out << "void ";
	out << mi.name() << "(";
	writeParameterList(out, mi.getParameters(), false);
	out << ")";
	if (mi.isConst())
	{
		out << " const";
	}
	if (mi.isAbstract())
		out << " = 0;" << std::endl;
	else
		out << ";" << std::endl;
	if (!mi.getDocu().empty())
	{
		writeComment(out, mi.getDocu(), 2);
	}
}


void CppWriter::writeMethodSrc(std::ostream& out, const ClassInfo& info, const MethodInfo& mi, bool writeInline)
{
	if (mi.isAbstract())
		return;
	if (writeInline)
		out << "inline ";
	Poco::SharedPtr<Parameter> pRet = mi.getReturnParameter();
	if (pRet)
		writeParameter(out, *pRet, true);
	else
		out << "void ";
	out << info.name() << "::" << mi.name() << "(";
	writeParameterList(out, mi.getParameters());
	out << ")";
	if (mi.isConst())
		out << " const";
	out << std::endl << "{" << std::endl;
	writeCode(out, mi.getCode(), 1);
	out << "}" << std::endl << std::endl << std::endl;
}


void CppWriter::writeCode(std::ostream& out, const std::vector<std::string>& code, int indent)
{
	std::vector<std::string>::const_iterator it = code.begin();
	for (; it != code.end(); ++it)
	{
		writeCode(out, *it, indent);
	}
}


void CppWriter::writeCode(std::ostream& out, const std::string& line, int ind)
{
	static const std::string prefix;
	writePrefixedLine(out, prefix, line, ind);
}


void CppWriter::writePrefixedLine(std::ostream& out, const std::string& prefix, const std::string& line, int ind)
{
	static const std::string indent[] = {"", "\t", "\t\t", "\t\t\t", "\t\t\t\t",
		"\t\t\t\t\t", "\t\t\t\t\t\t", "\t\t\t\t\t\t\t", "\t\t\t\t\t\t\t\t",
		"\t\t\t\t\t\t\t\t\t", "\t\t\t\t\t\t\t\t\t\t"};
	if (ind >= 0)
	{
		out << indent[ind%11] << prefix;
	}
	out << Poco::trim(line);
	if (ind >= 0 && (line.empty() || line[line.size() - 1] != '{'))
	{
		out << std::endl;
	}
}


void CppWriter::writeRemotingAttribute(std::ostream& out, const std::string& line, int ind)
{
	if (!_info.remotingAttributes())
		return;
	static const std::string prefix("//@ ");
	writePrefixedLine(out, prefix, line, ind);
}


void CppWriter::writeRemotingAttributes(std::ostream& out, const std::map<std::string, std::string>& attrs, int indent)
{
	if (!_info.remotingAttributes())
		return;
	std::map<std::string, std::string>::const_iterator it = attrs.begin();
	for (; it != attrs.end(); ++it)
	{
		std::string name = it->first;
		std::string value = it->second;
		if (value.empty())
		{
			if (indent == -1 && it != attrs.begin()) out << ", ";
			writeRemotingAttribute(out, name, indent);
		}
		else
		{
			bool mustQuote = false;
			if (value != "\"\"")
			{
				for (std::string::const_iterator itv = value.begin(); itv != value.end(); ++itv)
				{
					bool isAlNum = (*itv >= '0' && *itv <= '9')
						|| (*itv >= 'a' && *itv <= 'z')
						|| (*itv >= 'A' && *itv <= 'Z')
						|| (*itv == '_');
					if (!isAlNum) mustQuote = true;
				}
			}
			std::string quote(mustQuote ? "\"" : "");
			if (indent == -1 && it != attrs.begin()) out << ", ";
			writeRemotingAttribute(out, name + "=" + quote + value + quote, indent);
		}
	}
}


void CppWriter::writeMethodRemotingAttributes(std::ostream& out, const MethodInfo& mi, int indent)
{
	if (!_info.remotingAttributes())
		return;
	// first write global attributes of methodinfo
	// then for each parameter
	writeRemotingAttributes(out, mi.getAll(), indent);
	Poco::SharedPtr<Parameter> pReturn = mi.getReturnParameter();
	if (pReturn)
	{
		if (!pReturn->getAll().empty())
		{
			writeRemotingAttribute(out, "return={", indent);
			writeRemotingAttributes(out, pReturn->getAll(), -1);
			out << "}" << std::endl;
		}
	}
	const std::map<int, Parameter>& params = mi.getParameters();
	std::map<int, Parameter>::const_iterator it = params.begin();
	for (; it != params.end(); ++it)
	{
		if (!it->second.getAll().empty())
		{
			std::string attr("$");
			attr += it->second.getName();
			attr += "={";
			writeRemotingAttribute(out, attr, indent);
			writeRemotingAttributes(out, it->second.getAll(), -1);
			out << "}" << std::endl;
		}
	}
}


bool CppWriter::canForwardDeclare(const Variable& var) const
{
	bool isOptional = var.isOptional() && !var.isVector() && (!var.isNillable() || (_options & OPT_PRESERVE_OPTIONAL) != 0);
	bool isNillable = var.isNillable() || (var.isOptional() && !var.isVector() && (_options & OPT_PRESERVE_OPTIONAL) == 0 && !isOptional);

	if (isNillable || (isOptional && !(_options & OPT_ALWAYS_USE_OPTIONAL)))
		return !BuiltinTypes::instance().isKnownTypeInfo(var.getType());
	else
		return false;
}
