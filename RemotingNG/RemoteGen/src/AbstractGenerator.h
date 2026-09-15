//
// AbstractGenerator.h
//
// Definition of the AbstractGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_AbstractGenerator_INCLUDED
#define RemoteGen_AbstractGenerator_INCLUDED


#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/BasicEvent.h"
#include <set>


namespace Poco {
namespace CodeGeneration {


class GeneratorEngine;
class CppGenerator;


} }


class AbstractGenerator: public Poco::CodeGeneration::CodeGenerator
	/// AbstractGenerator generates a remote interface for a given class definition. C++ specific.
	/// Has no members, a default constructor/destructor and only virtual = 0; methods
{
public:
	static const std::set<std::string> BUILTIN_TYPES;

	static Poco::BasicEvent<const std::string> EError;

	AbstractGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the AbstractGenerator.

	virtual ~AbstractGenerator();
		/// Destroys the AbstractGenerator.

	void writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package);
		/// Writes the default header, e.g. copyright notice, no include namespace open or anything else is written
		/// This is always the first method that is called!

	void writeInclude(const std::string& include, bool toHeader);

	void writeSystemInclude(const std::string& include);

	void writeNameSpaceBegin(const std::string& namespacePrefix);
		/// Writes the namespacePrefix+"_BEGIN"

	void writeFwdDecls(const CodeGenerator::FwdDecls& decl);

	void writeNameSpaceEnd(const std::string& namespacePrefix);
		/// Writes the namespacePrefix+"_END"

	virtual void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// The default impl simply clones the incoming struct. Override this method.

	virtual void structEnd();
		/// Must not write the namespace

	virtual Poco::CppParser::Function* methodClone(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Clones the function

	virtual Poco::CppParser::Struct* structClone(const Poco::CppParser::Struct* pStructIn, const std::string& newClassName, const std::vector<std::string>& baseClasses = std::vector<std::string>());
		/// Clones the struct without any methods or member variables!

	virtual void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e) = 0;

	virtual std::string newClassName(const Poco::CppParser::Struct* pStruct) = 0;
		/// generates the new class name based on the old one

	virtual std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct) = 0;
		/// generates the new base classes

	void writeMethodImplementation(const std::string& code);
		/// Only valid when inside a method. Simply forwards the code lines to the implementation file.

	void writeImplementation(const std::string& code);

	virtual void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Closes the method

	void variablesStart();

	void variable(const Poco::CppParser::Variable* pVar);

	void variablesEnd();

	void endFile();

	void writeTypeDef(const Poco::CppParser::TypeDef*);

	void writeUsing(const Poco::CppParser::TypeAlias*);

	static void addDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew);
		/// Takes the docu from the old symbol and adds it to the new one removing the remote property,
		/// but adding the generated property.

	static void addParamDocu(Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& functionProperties);
		/// Searches the docu for the parameter specific properties and sets them as docu at pParam

	static std::string includePath(const Poco::Util::AbstractConfiguration& config, const std::string& nameSpace);
		/// Returns the include file output path for the given namespace.

	static std::string includePath(const Poco::Util::AbstractConfiguration& config, const Poco::CppParser::Struct* pStruct);

protected:
	void handleParentFunctions(const Poco::CppParser::Struct* pStruct);
		/// fwds all the methods of all parents to the methodStart method of the generator.

	void handleParentFunctionsImpl(const Poco::CppParser::Struct* pStruct, std::set<std::string>& methodSigs, bool root = true);
		/// fwds all the methods of all parents to the methodStart method of the generator.

	void includeTypeSerializers(const Poco::CppParser::Function* pFunc, bool toHFile, bool writeDirectly);
		/// Includes serializers/deserializers for the parameters of a function.

	void handleIncludeTypeSerializers(const Poco::CppParser::Variable* pVar, bool toHFile, bool writeDirectly);
		/// Includes serializers/deserializers for a member variable

	void handleIncludeTypeSerializersImpl(const std::string& complexResolvedType, bool toHFile, bool writeDirectly);
		/// Includes serializers/deserializers for a complex type

	void includeTypes(const Poco::CppParser::Function* pFunc, bool toHFile, bool writeDirectly);
		/// Includes files for data types. Catches cases where data is only fwd declared and the generated code is using it

	static void typeIdCodeGen(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Struct* pStruct, CodeGenerator& gen, void* addParam);
		/// Code generator for the getTypeId method

	std::string resolveType(const std::string& decl);
		/// Resolves typedefs.

	Poco::CodeGeneration::CppGenerator& cppGen();
		/// Returns the cpp code generator

private:
	static const std::set<std::string> initBuiltinTypes();

	void handleIncludeTypeSerializers(const Poco::CppParser::Parameter* pParam, bool toHFile, bool writeDirectly);
		/// Includes serializers/deserializers for the parameters of a function.

	void handleIncludeTypeSerializers(const std::string& simpleType, bool toHFile, bool writeDirectly);
		/// Includes serializers/deserializers for simple types

	void handleIncludeTypes(const Poco::CppParser::Parameter* pParam, bool toHFile, bool writeDirectly);
		/// Includes files for data types. Catches cases where data is only fwd declared and the generated code is using it

	void handleIncludeFile(const std::string& incFile, bool toHFile, bool writeDirectly);


protected:
	Poco::CodeGeneration::CppGenerator& _cppGen;
	Poco::CppParser::Struct* _pStruct;
	const Poco::CppParser::Struct* _pStructIn;
	Poco::CppParser::NameSpace* _pNs;
};


//
// inlines
//
inline void AbstractGenerator::writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package)
{
}


inline void AbstractGenerator::writeInclude(const std::string& include, bool)
{
}


inline void AbstractGenerator::writeSystemInclude(const std::string& include)
{
}


inline void AbstractGenerator::writeNameSpaceBegin(const std::string& namespacePrefix)
{
}


inline void AbstractGenerator::writeFwdDecls(const CodeGenerator::FwdDecls& decl)
{
}


inline void AbstractGenerator::writeNameSpaceEnd(const std::string& namespacePrefix)
{
}


inline void AbstractGenerator::writeMethodImplementation(const std::string& code)
{
}


inline void AbstractGenerator::writeImplementation(const std::string& code)
{
}


inline void AbstractGenerator::methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
}


inline void AbstractGenerator::variablesStart()
{
}


inline void AbstractGenerator::variable(const Poco::CppParser::Variable*)
{
}


inline void AbstractGenerator::variablesEnd()
{
}


inline void AbstractGenerator::endFile()
{
}


inline void AbstractGenerator::writeTypeDef(const Poco::CppParser::TypeDef*)
{
}


inline void AbstractGenerator::writeUsing(const Poco::CppParser::TypeAlias*)
{
}


inline Poco::CodeGeneration::CppGenerator& AbstractGenerator::cppGen()
{
	return _cppGen;
}


#endif // RemoteGen_AbstractGenerator_INCLUDED
