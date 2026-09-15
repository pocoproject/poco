//
// CodeGenerator.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  CodeGenerator
//
// Definition of the CodeGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGeneration_CodeGenerator_INCLUDED
#define CodeGeneration_CodeGenerator_INCLUDED


#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Variable.h"
#include "Poco/CppParser/TypeDef.h"
#include <set>
#include <map>


namespace Poco {
namespace CodeGeneration {


class CodeGeneration_API CodeGenerator
	/// A CodeGenerator defines an interface for code generators.
{
public:
	struct FwdDecl
	{
		std::string includeFile;
		std::string className;
		std::string fullNameSpace;
		FwdDecl(const std::string& inc, const std::string& cN, const std::string& ns);
		bool operator<(const FwdDecl& other) const;
		/// Must guarantee that the primary order is by namespace, then by class
	};

	typedef std::set<std::string> IncludeSet;
	typedef std::map<std::string, std::string> Properties;
	/// Maps a property name to its value.
	typedef std::set<FwdDecl> FwdDecls;

	CodeGenerator(const std::string& nameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright);
		/// Creates the CodeGenerator. The nameSpace declares in which nameSpace the generated class will be,
		/// the library name contaisn the name of the project

	virtual ~CodeGenerator();
		/// Destroys the CodeGenerator.

	virtual void addIncludeFile(const std::string& incFile);
		/// Adds an include file to the CodeGenerator. A call to this method is only meaningful until structStart was not called.
		/// Duplicate includes will be ignored. Contains the include path only, e.g. "include/MyClass.h" (without enclosing ")

	virtual void addSrcIncludeFile(const std::string& incFile);
		/// Include file which will be written to an implementation file.

	virtual void addSystemIncludeFile(const std::string& incFile);
		/// Adds a system include file

	virtual void addFwdDecl(const std::string& inclFile, const std::string& className, const std::string namespacePrefix);
		/// Adds a fwd decl.

	virtual void writeIncludes();
		/// Convenience Functions which calls for all include files writeInclude, also writes fwdDecls.

	virtual void writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package) = 0;
		/// Writes the default header, e.g. copyright notice, no include namespace open or anything else is written
		/// This is always the first method that is called!

	virtual void endFile() = 0;
		/// Closes the file.

	virtual void writeInclude(const std::string& include, bool toHeader) = 0;
		/// Writes the include either to the header or the src file.

	virtual void writeSystemInclude(const std::string& include) = 0;
		/// Writes the system include file to the header.

	virtual void writeNameSpaceBegin(const std::string& fullNameSpace) = 0;
		/// Writes the enclosing namespace

	virtual void writeFwdDecls(const FwdDecls& decl) = 0;
		/// Writes the fwd decl to the header file and the include file to the src file.

	virtual void writeNameSpaceEnd(const std::string& fullNameSpace) = 0;
		/// Closes the namespace

	virtual void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties) = 0;
		/// Writes the class declaration.

	virtual void structEnd() = 0;
		/// Closes the class declaration.

	virtual void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties) = 0;
		/// Writes the method header. Properties contains the properties found in the functions docuementation and in the class documentation.

	virtual void writeMethodImplementation(const std::string& code) = 0;
		/// Only valid when inside a method. Simply forwards the code lines to the implementation file.

	virtual void writeImplementation(const std::string& code) = 0;
		/// Writes other non-method code.

	virtual void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties) = 0;
		/// Closes the method.

	virtual void variablesStart() = 0;
		/// Indicates that variables wil be written.

	virtual void variable(const Poco::CppParser::Variable* pVar) = 0;
		/// Writes a single variable.

	virtual void writeTypeDef(const Poco::CppParser::TypeDef* pType) = 0;
		/// Writes a single typedef

	virtual void writeUsing(const Poco::CppParser::TypeAlias* pType) = 0;
		/// Writes a single using

	virtual void variablesEnd() = 0;
		/// Ends variable definitions.

	virtual const CodeGenerator::IncludeSet& includes() const;
		/// Returns all includes that the CodeGenerator consists of.

	virtual const CodeGenerator::IncludeSet& systemIncludes() const;
		/// Returns all system includes that the CodeGenerator consists of.

	virtual const std::string& nameSpace() const;
		/// The namespace of the file

	virtual const std::string& libraryName() const;
		/// The prefix assigned to the namespace.

	virtual bool usePocoIncludeStyle() const;
		/// Returns which include style should be used

	virtual const std::string& copyright() const;
		/// Returns the copyright notice, can be empty.

private:
	std::string _nameSpace;
	std::string _libraryName;
	bool        _usePocoIncludeStyle;
	std::string _copyright;
	IncludeSet  _includes;
	IncludeSet  _systemIncludes;
	IncludeSet  _srcIncludes;
	FwdDecls    _fwdDecls;
};


inline void CodeGenerator::addIncludeFile(const std::string& incFile)
{
	_includes.insert(incFile);
}


inline void CodeGenerator::addSrcIncludeFile(const std::string& incFile)
{
	_srcIncludes.insert(incFile);
}


inline void CodeGenerator::addSystemIncludeFile(const std::string& incFile)
{
	_systemIncludes.insert(incFile);
}


inline const CodeGenerator::IncludeSet& CodeGenerator::includes() const
{
	return _includes;
}


inline const CodeGenerator::IncludeSet& CodeGenerator::systemIncludes() const
{
	return _systemIncludes;
}


inline const std::string& CodeGenerator::nameSpace() const
{
	return _nameSpace;
}


inline const std::string& CodeGenerator::libraryName() const
{
	return _libraryName;
}


inline bool CodeGenerator::usePocoIncludeStyle() const
{
	return _usePocoIncludeStyle;
}


inline const std::string& CodeGenerator::copyright() const
{
	return _copyright;
}


} } // namespace Poco::CodeGeneration


#endif // CodeGeneration_CodeGenerator_INCLUDED
