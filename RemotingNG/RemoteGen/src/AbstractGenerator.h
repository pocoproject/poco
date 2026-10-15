//
// AbstractGenerator.h
//
// Definition of the AbstractGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_AbstractGenerator_INCLUDED
#define RemoteGen_AbstractGenerator_INCLUDED


#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/BasicEvent.h"
#include <set>


namespace Poco::CodeGeneration {


class GeneratorEngine;
class CppGenerator;


} // namespace Poco::CodeGeneration


class AbstractGenerator: public Poco::CodeGeneration::CodeGenerator
	/// AbstractGenerator generates a remote interface for a given class definition. C++ specific.
	/// Has no members, a default constructor/destructor and only virtual = 0; methods
{
public:
	static const std::set<std::string> BUILTIN_TYPES;

	static Poco::BasicEvent<const std::string> EError;

	AbstractGenerator(Poco::CodeGeneration::CppGenerator& cppGen);
		/// Creates the AbstractGenerator.

	~AbstractGenerator() override;
		/// Destroys the AbstractGenerator.

	void writeDefaultHeader(const Poco::CppParser::Struct* pStruct, const std::string& className, const std::string& libraryName, const std::string& package) override;
		/// Writes the default header, e.g. copyright notice, no include namespace open or anything else is written
		/// This is always the first method that is called!

	void writeInclude(const std::string& include, bool toHeader) override;

	void writeSystemInclude(const std::string& include) override;

	void writeNameSpaceBegin(const std::string& namespacePrefix) override;
		/// Writes the namespacePrefix+"_BEGIN"

	void writeFwdDecls(const CodeGenerator::FwdDecls& decl) override;

	void writeNameSpaceEnd(const std::string& namespacePrefix) override;
		/// Writes the namespacePrefix+"_END"

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties) override;
		/// The default impl simply clones the incoming struct. Override this method.

	void structEnd() override;
		/// Must not write the namespace

	[[nodiscard]] virtual Poco::CppParser::Function* methodClone(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Clones the function

	[[nodiscard]] virtual Poco::CppParser::Struct* structClone(const Poco::CppParser::Struct* pStructIn, const std::string& newClassName, const std::vector<std::string>& baseClasses = std::vector<std::string>());
		/// Clones the struct without any methods or member variables!

	virtual void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e) = 0;

	[[nodiscard]] virtual std::string newClassName(const Poco::CppParser::Struct* pStruct) = 0;
		/// generates the new class name based on the old one

	[[nodiscard]] virtual std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct) = 0;
		/// generates the new base classes

	void writeMethodImplementation(const std::string& code) override;
		/// Only valid when inside a method. Simply forwards the code lines to the implementation file.

	void writeImplementation(const std::string& code) override;

	void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties) override;
		/// Closes the method

	void variablesStart() override;

	void variable(const Poco::CppParser::Variable* pVar) override;

	void variablesEnd() override;

	void endFile() override;

	void writeTypeDef(const Poco::CppParser::TypeDef*) override;

	void writeUsing(const Poco::CppParser::TypeAlias*) override;

	static void addDocumentation(const Poco::CppParser::Symbol* pSymOld,Poco::CppParser::Symbol* pNew);
		/// Takes the docu from the old symbol and adds it to the new one removing the remote property,
		/// but adding the generated property.

	static void addParamDocu(Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& functionProperties);
		/// Searches the docu for the parameter specific properties and sets them as docu at pParam

	[[nodiscard]] static std::string includePath(const Poco::Util::AbstractConfiguration& config, const std::string& nameSpace);
		/// Returns the include file output path for the given namespace.

	[[nodiscard]] static std::string includePath(const Poco::Util::AbstractConfiguration& config, const Poco::CppParser::Struct* pStruct);

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

	[[nodiscard]] std::string resolveType(const std::string& decl);
		/// Resolves typedefs.

	[[nodiscard]] Poco::CodeGeneration::CppGenerator& cppGen();
		/// Returns the cpp code generator

private:
	[[nodiscard]] static const std::set<std::string> initBuiltinTypes();

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
