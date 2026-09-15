//
// SwaggerGenerator.h
//
// Definition of the SwaggerGenerator class.
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_SwaggerGenerator_INCLUDED
#define RemoteGen_SwaggerGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "Poco/JSON/Object.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include <map>


class SwaggerGenerator: public AbstractGenerator
	/// SwaggerGenerator generates an OpenAPI 3.0 "Swagger" JSON document describing the API
{
public:
	struct Info
	{
		std::string title;
		std::string description;
		std::string termsOfService;
		std::string contactName;
		std::string contactUrl;
		std::string contactEmail;
		std::string licenseName;
		std::string licenseUrl;
		std::string version;
	};

	struct Server
	{
		std::string url;
		std::string description;
		// Server variables are not supported.
	};

	using PropMap = std::map<std::string, CodeGenerator::Properties>;

	SwaggerGenerator(Poco::CodeGeneration::CppGenerator& generator, const Info& info, const std::vector<Server>& servers);
	~SwaggerGenerator();

	Poco::JSON::Object::Ptr json() const;

	// AbstractGenerator
	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
	void structEnd();
	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
	void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);
	std::string newClassName(const Poco::CppParser::Struct* pStruct);
	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

protected:
	static Poco::JSON::Object::Ptr createInfo(const Info& info);
	static Poco::JSON::Array::Ptr createServers(const std::vector<Server>& servers);
	static Poco::JSON::Object::Ptr createErrorSchema();
	static Poco::JSON::Object::Ptr createSchemaForBasicType(const std::string& basicType);
	static bool isBasicType(const std::string& type);
	static bool isValidOperation(const std::string& name);
	static bool operationCanHaveBody(const std::string& name);
	static std::string translateTypeName(const std::string& typeName);
	const Poco::CppParser::Symbol* resolveInnerType(const Poco::CppParser::NameSpace* pNamespace, const Poco::CppParser::Symbol* pSymbol, std::string& resolvedType);
	Poco::JSON::Object::Ptr createSchemaForType(const Poco::CppParser::NameSpace* pNamespace, const Poco::CppParser::Symbol* pSymbol, const std::string& resolvedType);
	Poco::JSON::Object::Ptr createSchemaForObject(const Poco::CppParser::Struct* pStruct);
	void createSchemaForObject(const Poco::CppParser::Struct* pStruct, Poco::JSON::Object::Ptr pSchema);
	Poco::JSON::Object::Ptr createOperation(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
	Poco::JSON::Object::Ptr createParameter(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& properties, const std::string& in);
	Poco::JSON::Object::Ptr createFormRequestBody(const Poco::CppParser::Function* pFunc, const std::vector<Poco::CppParser::Parameter*>& params, const PropMap& propMap, const CodeGenerator::Properties& funcProps);
	Poco::JSON::Object::Ptr createRequestBody(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& paramProps, const CodeGenerator::Properties& funcProps);
	Poco::JSON::Object::Ptr createResponseBodyContent(const std::string& type, const CodeGenerator::Properties& paramProps, const CodeGenerator::Properties& funcProps);
	Poco::JSON::Object::Ptr createErrorResponse(const std::string& description);
	void variableProperties(const Poco::CppParser::Variable* pVar, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType);
	static PropMap parseParameterProperties(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
	static CodeGenerator::Properties parseParameterProperties(const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& funcProps);
	void parameterProperties(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType);
	void typeProperties(const std::string& decl, bool& isVector, bool& isMandatory, bool& isNullable, std::string& resolvedType);
	void fillTypeAttributes(Poco::JSON::Object::Ptr pSchema, const CodeGenerator::Properties& properties, const std::string& name);

private:
	Poco::JSON::Object::Ptr _pSwagger;
	Poco::JSON::Object::Ptr _pPaths;
	Poco::JSON::Object::Ptr _pComponents;
	Poco::JSON::Object::Ptr _pSchemas;

	static const std::map<std::string, std::pair<std::string, std::string>> basicTypes;
};


//
// inlines
//
inline Poco::JSON::Object::Ptr SwaggerGenerator::json() const
{
	return _pSwagger;
}


inline bool SwaggerGenerator::isBasicType(const std::string& type)
{
	return basicTypes.find(type) != basicTypes.end();
}


#endif // RemoteGen_SwaggerGenerator_INCLUDED
