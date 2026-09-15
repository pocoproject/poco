//
// CppGenerator.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  CppGenerator
//
// Definition of the CppGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef CodeGeneration_CppGenerator_INCLUDED
#define CodeGeneration_CppGenerator_INCLUDED


#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CppParser/Parameter.h"
#include <ostream>


namespace Poco {
namespace CodeGeneration {


class CodeGeneration_API CppGenerator: public CodeGenerator
	/// Cpp implementation of the code generator
{
public:
	CppGenerator(const std::string& nameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright, std::ostream& hOut, std::ostream& cppOut);
		/// Creates the CppGenerator.

	~CppGenerator();
		/// Destroys the CppGenerator.

	void enableAttributes(bool enable = true);
		/// Enable or disable (default) writing of attributes.

	void enableTimestamps(bool enable = true);
		/// Enable (default) or disable timestamps in file headers.

	void registerConstructorHint(const Poco::CppParser::Variable* pVar, const Poco::CppParser::Parameter* pParam);
		/// Register a hint which tells us which parameter of the constructor should be used to init
		/// a given member variable

	void registerConstructorHint(const Poco::CppParser::Variable* pVar, const std::string& val);
		/// Register a hint which tells us which constant value should be used to init
		/// a given member variable.

	void writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package);
		/// Writes the default header, plus include guard

	void endFile();
		/// Closes the include guard

	void writeInclude(const std::string& include, bool toHeader);

	void writeSrcInclude(const std::string& include);

	void writeSystemInclude(const std::string& include);

	void writeNameSpaceBegin(const std::string& fullNameSpace);

	void writeFwdDecls(const CodeGenerator::FwdDecls& decl);

	void writeNameSpaceEnd(const std::string& fullNameSpace);

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);

	void structEnd();

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);

	void writeMethodImplementation(const std::string& code);

	void writeImplementation(const std::string& code);

	void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);

	void variablesStart();

	void variable(const Poco::CppParser::Variable* pVar);

	void variablesEnd();

	void writeTypeDef(const Poco::CppParser::TypeDef* pType);

	void writeUsing(const Poco::CppParser::TypeAlias* pType);

	void writeToHeader(const std::string& jsCode);

private:
	void writeNameSpaceBegin(const std::string& fullNameSpace, bool toHeaderOnly);

	void writeNameSpaceEnd(const std::string& fullNameSpace, bool toHeaderOnly);

	void writeParameter(const Poco::CppParser::Parameter* pParam);
		///Writes parameters to h and cpp file

	void writeParameter(const Poco::CppParser::Parameter* pParam, bool inHFile, std::ostream& out);
		///Writes a single parameter to h and cpp file

	void handleAccessChange(Poco::CppParser::Symbol::Access newAcc);
		/// Handles access change in h file.

	void handleMemberInitInConstructor(const Poco::CppParser::Function* pFunc, std::ostream& out);
		/// Magically initializes the member variables with the correct constructor parameter.

	void writeMethodStartCpp(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties, std::ostream& out);
		/// Writes method implementation header to the defined output stream.

	void writeMethodStartH(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes method implementation header to the h file.

	void writeFunctionHeader(const Poco::CppParser::Function* pFunc, bool inHFile, std::ostream& out, const std::string& className=std::string());
		/// Writes the function header. If a className is specified, the function is written qualified.

	void writeMethodStartTemplate(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Writes method templates.

	void writeMethodImplementationImpl(const std::string& code, std::ostream& out);
		/// Implementation of writeMethodImplementation.

	void methodEndImpl(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties, std::ostream& out);
		/// Cloes the method.

	const Poco::CppParser::Parameter* findParameter(const Poco::CppParser::Function* pFunc, const std::string& paramName);
		/// Finds the parameter in the function or returns 0.

	bool findMatchingDecl(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam);
		/// Matches fucntion declaration to a parameter declaration. Returns true on success,

private:
	typedef std::map<const Poco::CppParser::Variable*, const Poco::CppParser::Parameter*> ConstructorMapping;
	typedef std::map<const Poco::CppParser::Variable*, std::string> SimpleConstructorMapping;

	struct InlineFunctionInfo
	{
		const Poco::CppParser::Function* pFunc;
		CodeGenerator::Properties  props;
		std::vector<std::string> codeLines;
	};

		///Defines if include paths are formatted poco style.
	std::ostream&             _hOut;
	std::ostream&             _cppOut;
	std::string               _className;
	bool                      _inInlineFct;
	bool                      _isTemplate;
	bool                      _inPureVirtualFct;
	Poco::CppParser::Symbol::Access _lastAcc;
	const Poco::CppParser::Struct*  _pStruct;
	const Poco::CppParser::Function* _pFunc;
	std::vector<InlineFunctionInfo> _inlineFunctions;
	ConstructorMapping        _constrMapping;
	SimpleConstructorMapping  _simpleConstrMapping;
	std::string               _includeGuard;
	bool                      _attributesEnabled;
	bool                      _timestampsEnabled;
};


inline void CppGenerator::registerConstructorHint(const Poco::CppParser::Variable* pVar, const Poco::CppParser::Parameter* pParam)
{
	_constrMapping.insert(std::make_pair (pVar, pParam));
}


inline void CppGenerator::registerConstructorHint(const Poco::CppParser::Variable* pVar, const std::string& val)
{
	_simpleConstrMapping.insert(std::make_pair (pVar, val));
}


inline void CppGenerator::writeToHeader(const std::string& jsCode)
{
	_hOut << jsCode << std::endl;
}


} } // namespace Poco::CodeGeneration


#endif // CodeGeneration_CppGenerator_INCLUDED
