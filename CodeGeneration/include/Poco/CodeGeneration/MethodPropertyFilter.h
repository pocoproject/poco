//
// MethodPropertyFilter.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  MethodPropertyFilter
//
// Definition of the MethodPropertyFilter class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef CodeGeneration_MethodPropertyFilter_INCLUDED
#define CodeGeneration_MethodPropertyFilter_INCLUDED


#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CodeGeneration/CodeGenerator.h"


namespace Poco {
namespace CodeGeneration {


class CodeGeneration_API MethodPropertyFilter: public CodeGenerator
	/// A MethodPropertyFilter filters methods according to properties. It works as a decorator,
	/// checking if a property is set at the method. If not it will simply ignore all method calls
	/// related to that method. It will not filter variables, includes, only methods excl. constructor and destructor
	/// are checked.
{
public:
	MethodPropertyFilter(CodeGenerator* pGen, const CodeGenerator::Properties& properties);
		/// Creates a MethodPropertyFilter decorating another CodeGenerator. Takes ownership of the pointer which must not be 0.
		/// properties contains the properties that each method must match to be generated.

	MethodPropertyFilter(CodeGenerator* pGen, const std::string& prop);
		/// Creates a MethodPropertyFilter decorating another CodeGenerator. Takes ownership of the pointer which must not be 0.
		/// Convenience constructor which takes exactly one property.

	~MethodPropertyFilter();
		/// Destroys the MethodPropertyFilter.

	void addIncludeFile(const std::string& incFile);
		/// Adds an include file to the CodeGenerator. A call to this method is only meaningful until structStart was not called.
		/// Duplicate includes will be ignored. Contains the include path only, e.g. "include/MyClass.h" (without enclosing ")

	void addSystemIncludeFile(const std::string& incFile);
		/// Adds a system include file.

	void addFwdDecl(const std::string& inclFile, const std::string& className, const std::string namespacePrefix);
		/// Adds a fwd decl.

	void writeIncludes();
		/// Convenience Functions which calls for all include files writeInclude, also writes fwdDecls.

	void writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package);
		/// Writes the default header, e.g. copyright notice, no include namespace open or anything else is written
		/// This is always the first method that is called!

	void writeInclude(const std::string& include, bool toHeader);

	void writeSystemInclude(const std::string& include);

	void writeNameSpaceBegin(const std::string& ns);

	void writeFwdDecls(const CodeGenerator::FwdDecls& decl);

	void writeNameSpaceEnd(const std::string& ns);

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);

	void structEnd();

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);

	void writeMethodImplementation(const std::string& code);

	void writeImplementation(const std::string& code);

	void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);

	void variablesStart();

	void variable(const Poco::CppParser::Variable* pVar);

	void variablesEnd();

	const CodeGenerator::IncludeSet& includes() const;

	const CodeGenerator::IncludeSet& systemIncludes() const;

	const std::string& nameSpace() const;

	const std::string& libraryName() const;

	bool usePocoIncludeStyle() const;

	const std::string& copyright() const;

	void endFile();

	void writeTypeDef(const Poco::CppParser::TypeDef* pType);

	void writeUsing(const Poco::CppParser::TypeAlias* pType);

private:
	bool matches(const CodeGenerator::Properties& properties) const;
		/// Returns true if it is superset of the internal properties

private:
	CodeGenerator*            _pGen;
	CodeGenerator::Properties _properties;
	bool                      _forwardMethod;
	bool                      _inMethod;
};


inline void MethodPropertyFilter::addIncludeFile(const std::string& incFile)
{
	_pGen->addIncludeFile(incFile);
}


inline void MethodPropertyFilter::addSystemIncludeFile(const std::string& incFile)
{
	_pGen->addSystemIncludeFile(incFile);
}


inline void MethodPropertyFilter::addFwdDecl(const std::string& inclFile, const std::string& className, const std::string namespacePrefix)
{
	_pGen->addFwdDecl(inclFile, className, namespacePrefix);
}


inline void MethodPropertyFilter::writeIncludes()
{
	_pGen->writeIncludes();
}


inline void MethodPropertyFilter::writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package)
{
	_pGen->writeDefaultHeader(pStruct, className, libraryName, package);
}


inline void MethodPropertyFilter::writeInclude(const std::string& include, bool toheader)
{
	_pGen->writeInclude(include, toheader);
}


inline void MethodPropertyFilter::writeSystemInclude(const std::string& include)
{
	_pGen->writeSystemInclude(include);
}


inline void MethodPropertyFilter::writeImplementation(const std::string& code)
{
	_pGen->writeImplementation(code);
}


inline void MethodPropertyFilter::writeNameSpaceBegin(const std::string& ns)
{
	_pGen->writeNameSpaceBegin(ns);
}


inline void MethodPropertyFilter::writeFwdDecls(const FwdDecls& decl)
{
	_pGen->writeFwdDecls(decl);
}


inline void MethodPropertyFilter::writeNameSpaceEnd(const std::string& ns)
{
	_pGen->writeNameSpaceEnd(ns);
}


inline void MethodPropertyFilter::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pGen->structStart(pStruct, properties);
}


inline void MethodPropertyFilter::structEnd()
{
	_pGen->structEnd();
}


inline void MethodPropertyFilter::variablesStart()
{
	_pGen->variablesStart();
}


inline void MethodPropertyFilter::variable(const Poco::CppParser::Variable* pVar)
{
	_pGen->variable(pVar);
}


inline void MethodPropertyFilter::variablesEnd()
{
	_pGen->variablesEnd();
}


inline const CodeGenerator::IncludeSet& MethodPropertyFilter::includes() const
{
	return _pGen->includes();
}


inline const CodeGenerator::IncludeSet& MethodPropertyFilter::systemIncludes() const
{
	return _pGen->systemIncludes();
}


inline const std::string& MethodPropertyFilter::nameSpace() const
{
	return _pGen->nameSpace();
}


inline const std::string& MethodPropertyFilter::libraryName() const
{
	return _pGen->libraryName();
}


inline bool MethodPropertyFilter::usePocoIncludeStyle() const
{
	return _pGen->usePocoIncludeStyle();
}


inline void MethodPropertyFilter::endFile()
{
	_pGen->endFile();
}


inline const std::string& MethodPropertyFilter::copyright() const
{
	return _pGen->copyright();
}


inline void MethodPropertyFilter::writeTypeDef(const Poco::CppParser::TypeDef* pType)
{
	return _pGen->writeTypeDef(pType);
}


inline void MethodPropertyFilter::writeUsing(const Poco::CppParser::TypeAlias* pType)
{
	return _pGen->writeUsing(pType);
}


} } // namespace Poco::CodeGeneration


#endif // CodeGeneration_MethodPropertyFilter_INCLUDED
