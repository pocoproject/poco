//
// CppWriter.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_CppWriter_H_INCLUDED
#define CodeGen_CppWriter_H_INCLUDED


#include "ClassInfo.h"
#include "SchemaInfo.h"
#include "Poco/Logger.h"
#include <vector>
#include <string>
#include <ostream>
#include <set>


class TypeInfo;


class CppWriter
{
public:
	static const std::string EXT_H;
	static const std::string EXT_CPP;
	
	enum Options
	{
		OPT_PRESERVE_OPTIONAL = 1,
		OPT_HEADER_TIMESTAMPS = 8,
		OPT_INCLUDE_NAMESPACE_IN_SOURCE_FILENAME = 16,
		OPT_INCLUDE_NAMESPACE_IN_HEADER_FILENAME = 32,
		OPT_ALWAYS_USE_OPTIONAL = 64,
		OPT_USE_STD = 128,
		OPT_INLINE_ALL = 256,
		OPT_AMALGAMATE_SOURCES = 512
	};

	CppWriter(const SchemaInfo& info, int options, const std::string& amalgamatedSourceFileName);
		/// Constructor

	~CppWriter();

	void updateIncludeDir(ClassInfo& cInfo) const;
		/// Updates the include dir to the file that will be generated later
		/// with generate

	void generate(const ClassInfo& cinfo);
		/// Generates code for the given class

	void generate(const std::map<std::string, ClassInfo>& classes);
		/// Generates code for the given classes

	void allIncludeFiles(const ClassInfo& info, std::set<std::string>& includes, std::set<std::string>& sysIncludes, std::set<std::string>& fwdDecl);
		/// Iterates over all variables/parameters and adds their includes to the appropriate set

	void allExtraIncludeFiles(const ClassInfo& info, std::set<std::string>& includes);
		/// Iterates over all forward-declared types and adds their headers for inclusion
		/// at the end of the header file to aid template instantiation.

	void writeHeader(std::ostream& out, const std::string& fileName);

private:
	std::string generateHeaderFile(std::ostream& out, const ClassInfo& cinfo);
		/// Returns the include guard, Doesn't close namespace and include guard
		/// in case inline methods will be written

	bool generateSourceFile(std::ostream& out, std::ostream& hdrFile, const ClassInfo& cinfo);

	void writeComment(std::ostream& out, const std::string& line, int indent = 0);

	void writeComment(std::ostream& out, const std::vector<std::string>& lines, int indent = 0);

	void writeInclude(std::ostream& out, const std::set<std::string>& includes, bool systemInc);

	void writeInclude(std::ostream& out, const std::string& include, bool systemInc);

	void writeFwdDecls(std::ostream& out, const std::set<std::string>& fwds);

	std::string writeIncludeGuardBegin(std::ostream& out, const TypeInfo& info);
		/// Returns the incGuard we need for writeIncludeGuardEnd

	void writeIncludeGuardEnd(std::ostream& out, const std::string& incGuard);

	void writeNamespaceBegin(std::ostream& out, const std::string& ns);

	void writeNamespaceEnd(std::ostream& out, const std::string& ns);

	void writeClassStart(std::ostream& out, const ClassInfo& cinfo);

	void writeClassEnd(std::ostream& out, const ClassInfo& cinfo);

	void writeAccessibility(std::ostream& out, Utility::Access access);

	void checkAccessibility(std::ostream& out, Utility::Access oldAccess, Utility::Access newAccess);

	void writeConstructorHeader(std::ostream& out, const Constructor& constr);

	void writeConstructorSrc(std::ostream& out, const Constructor& constr, const ClassInfo& info, bool doInline);

	void writeParameterList(std::ostream& out, const std::map<int, Parameter>& params, bool singleLine = true);

	void writeDestructorHeader(std::ostream& out, const Destructor& destr);

	void writeDestructorSrc(std::ostream& out, const Destructor& destr, bool doInline);

	void writeParameter(std::ostream& out, const Parameter& param, bool isRetParam = false);

	void writeVariable(std::ostream& out, const Variable& var);

	void writeTypeInfo(std::ostream& out, const TypeInfo& info);

	void condWriteNamespace(std::ostream& out, const std::string& nameSpace);
		/// Writes the namespace iff it is not equal to the current _classNameSpace

	void writeMethodHeader(std::ostream& out, const MethodInfo& mi);

	void writeMethodSrc(std::ostream& out, const ClassInfo& info, const MethodInfo& mi, bool doInline);

	void writeCode(std::ostream& out, const std::vector<std::string>& lines, int indent = 0);

	void writeCode(std::ostream& out, const std::string& line, int indent = 0);

	void writeRemotingAttributes(std::ostream& out, const std::map<std::string, std::string>& attrs, int indent = 0);

	void writeMethodRemotingAttributes(std::ostream& out, const MethodInfo& mi, int indent = 1);

	void writeRemotingAttribute(std::ostream& out, const std::string& line, int ind = 0);

	void writePrefixedLine(std::ostream& out, const std::string& prefix, const std::string& line, int ind = 0);

	bool canForwardDeclare(const Variable& var) const;

private:
	SchemaInfo _info;
	int _options;
	std::string _amalgamatedSourceFileName;
	std::string _classNameSpace;
	std::string _sharedPtr; 
	std::string _optional;
	Poco::Logger& _logger;
};


#endif // CodeGen_CppWriter_H_INCLUDED
