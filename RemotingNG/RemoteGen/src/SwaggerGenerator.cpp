//
// SwaggerGenerator.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "SwaggerGenerator.h"
#include "SerializerGenerator.h"
#include "GenUtility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Function.h"
#include "Poco/Exception.h"
#include "Poco/String.h"


using namespace std::string_literals;


const std::map<std::string, std::pair<std::string, std::string>> SwaggerGenerator::basicTypes =
{
	{"int"s, {"integer"s, "int32"s}},
	{"signed int"s, {"integer"s, "int32"s}},
	{"unsigned int"s, {"integer"s, "int32"s}},
	{"short"s, {"integer"s, "int32"s}},
	{"signed short"s, {"integer"s, "int32"s}},
	{"unsigned short"s, {"integer"s, "int32"s}},
	{"short int"s, {"integer"s, "int32"s}},
	{"signed short int"s, {"integer"s, "int32"s}},
	{"unsigned short int"s, {"integer"s, "int32"s}},
	{"long"s, {"integer"s, "int64"s}},
	{"signed long"s, {"integer"s, "int64"s}},
	{"unsigned long"s, {"integer"s, "int64"s}},
	{"long int"s, {"integer"s, "int64"s}},
	{"signed long int"s, {"integer"s, "int64"s}},
	{"unsigned long int"s, {"integer"s, "int64"s}},
	{"long long"s, {"integer"s, "int64"s}},
	{"signed long long"s, {"integer"s, "int64"s}},
	{"unsigned long long"s, {"integer"s, "int64"s}},
	{"long long int"s, {"integer"s, "int64"s}},
	{"signed long long int"s, {"integer"s, "int64"s}},
	{"unsigned long long int"s, {"integer"s, "int64"s}},
	{"char"s, {"string"s, ""s}},
	{"signed char"s, {"integer"s, "int32"s}},
	{"unsigned char"s, {"integer"s, "int32"s}},
	{"bool"s, {"boolean"s, ""s}},
	{"Int8"s, {"integer"s, "int32"s}},
	{"UInt8"s, {"integer"s, "int32"s}},
	{"Int16"s, {"integer"s, "int32"s}},
	{"UInt16"s, {"integer"s, "int32"s}},
	{"Int32"s, {"integer"s, "int32"s}},
	{"UInt32"s, {"integer"s, "int32"s}},
	{"Int64"s, {"integer"s, "int64"s}},
	{"UInt64"s, {"integer"s, "int64"s}},
	{"Poco::Int8"s, {"integer"s, "int32"s}},
	{"Poco::UInt8"s, {"integer"s, "int32"s}},
	{"Poco::Int16"s, {"integer"s, "int32"s}},
	{"Poco::UInt16"s, {"integer"s, "int32"s}},
	{"Poco::Int32"s, {"integer"s, "int32"s}},
	{"Poco::UInt32"s, {"integer"s, "int32"s}},
	{"Poco::Int64"s, {"integer"s, "int64"s}},
	{"Poco::UInt64"s, {"integer"s, "int64"s}},
	{"float"s, {"number"s, "float"s}},
	{"double"s, {"number"s, "double"s}},
	{"long double"s, {"number"s, "double"s}},
	{"Poco::DateTime"s, {"string"s, "date-time"s}},
	{"DateTime"s, {"string"s, "date-time"s}},
	{"Poco::LocalDateTime"s, {"string"s, "date-time"s}},
	{"LocalDateTime"s, {"string"s, "date-time"s}},
	{"Poco::Timestamp"s, {"string"s, "date-time"s}},
	{"Timestamp"s, {"string"s, "date-time"s}},
	{"Poco::Timespan"s, {"integer"s, "int64"s}},
	{"Timespan"s, {"integer"s, "int64"s}},
	{"std::string"s, {"string"s, ""s}},
	{"string"s, {"string"s, ""s}},
	{"URI"s, {"string"s, ""s}},
	{"Poco::URI"s, {"string"s, ""s}},
	{"UUID"s, {"string"s, ""s}},
	{"Poco::UUID"s, {"string"s, ""s}},
	{"vector < char >"s, {"string"s, "byte"s}},
	{"std::vector < char >"s, {"string"s, "byte"s}}
};


SwaggerGenerator::SwaggerGenerator(Poco::CodeGeneration::CppGenerator& generator, const Info& info, const std::vector<Server>& servers):
	AbstractGenerator(generator),
	_pSwagger(new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER)),
	_pPaths(new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER)),
	_pComponents(new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER)),
	_pSchemas(new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER))
{
	_pSwagger->set("openapi"s, "3.0.0"s);
	_pSwagger->set("info"s, createInfo(info));
	if (!servers.empty())
	{
		_pSwagger->set("servers"s, createServers(servers));
	}
	_pSwagger->set("paths"s, _pPaths);
	_pSchemas->set("Poco.RemotingNG.Error"s, createErrorSchema());
	_pComponents->set("schemas"s, _pSchemas);
	_pSwagger->set("components"s, _pComponents);
}


SwaggerGenerator::~SwaggerGenerator()
{
}


Poco::JSON::Object::Ptr SwaggerGenerator::createInfo(const Info& info)
{
	Poco::JSON::Object::Ptr pInfo = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pInfo->set("title"s, info.title);
	if (!info.description.empty())
	{
		pInfo->set("description"s, info.description);
	}
	pInfo->set("version"s, info.version);
	if (!info.termsOfService.empty())
	{
		pInfo->set("termsOfService"s, info.termsOfService);
	}

	Poco::JSON::Object::Ptr pContact = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	if (!info.contactName.empty())
	{
		pContact->set("name"s, info.contactName);
	}
	if (!info.contactUrl.empty())
	{
		pContact->set("url"s, info.contactUrl);
	}
	if (!info.contactEmail.empty())
	{
		pContact->set("email"s, info.contactEmail);
	}
	if (pContact->size() > 0)
	{
		pInfo->set("contact"s, pContact);
	}

	Poco::JSON::Object::Ptr pLicense = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	if (!info.licenseName.empty())
	{
		pLicense->set("name"s, info.licenseName);
	}
	if (!info.licenseUrl.empty())
	{
		pLicense->set("url"s, info.licenseUrl);
	}
	if (pLicense->size() > 0)
	{
		pInfo->set("license"s, pLicense);
	}
	return pInfo;
}


Poco::JSON::Array::Ptr SwaggerGenerator::createServers(const std::vector<Server>& servers)
{
	Poco::JSON::Array::Ptr pServers = new Poco::JSON::Array;
	for (const auto& server: servers)
	{
		Poco::JSON::Object::Ptr pServer = new Poco::JSON::Object;
		pServer->set("url"s, server.url);
		if (!server.description.empty())
		{
			pServer->set("description"s, server.description);
		}
		pServers->add(pServer);
	}
	return pServers;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createErrorSchema()
{
	Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pSchema->set("type"s, "object"s);

	Poco::JSON::Object::Ptr pProperties = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pProperties->set("error"s, createSchemaForBasicType("std::string"s));
	pProperties->set("detail"s, createSchemaForBasicType("std::string"s));
	pProperties->set("code"s, createSchemaForBasicType("int"s));

	pSchema->set("properties"s, pProperties);
	return pSchema;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createSchemaForBasicType(const std::string& basicType)
{
	Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	auto it = basicTypes.find(basicType);
	if (it != basicTypes.end())
	{
		pSchema->set("type"s, it->second.first);
		if (!it->second.second.empty())
		{
			pSchema->set("format"s, it->second.second);
		}
	}
	else throw Poco::NotFoundException("basic type", basicType);
	return pSchema;
}


const Poco::CppParser::Symbol* SwaggerGenerator::resolveInnerType(const Poco::CppParser::NameSpace* pNamespace, const Poco::CppParser::Symbol* pSymbol, std::string& resolvedType)
{
	std::vector<std::string> tt = GenUtility::getInnerTemplateTypes(resolvedType);
	if (tt.size() == 1 || (tt.size() == 2 && GenUtility::isNullableType(tt[0])))
	{
		resolvedType = tt[tt.size() - 1];
		pSymbol = pNamespace->lookup(resolvedType);
		if (!pSymbol)
			pSymbol = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
	}
	else if (tt.size() > 0)
	{
		throw CodeGenerationException("Unsupported template type", resolvedType + "<" + tt[0] + ">");
	}
	else throw CodeGenerationException("Unparsable template type", resolvedType);
	return pSymbol;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createSchemaForType(const Poco::CppParser::NameSpace* pNamespace, const Poco::CppParser::Symbol* pSymbol, const std::string& type)
{
	Poco::JSON::Object::Ptr pSchema;

	std::string resolvedType = type;
	bool isNullableType = GenUtility::isNullableType(resolvedType);
	bool isPtrType = GenUtility::isPtrType(resolvedType);
	bool isOptionalType = GenUtility::isOptionalType(resolvedType);
	bool isVectorType = GenUtility::isVectorType(resolvedType);

	if (isPtrType || isNullableType || isOptionalType || isVectorType)
	{
		pSymbol = resolveInnerType(pNamespace, pSymbol, resolvedType);
		pSchema = createSchemaForType(pNamespace, pSymbol, resolvedType);
	}
	else if (isBasicType(resolvedType))
	{
		pSchema = createSchemaForBasicType(resolvedType);
	}
	else
	{
		const Poco::CppParser::Struct* pStruct = dynamic_cast<const Poco::CppParser::Struct*>(pSymbol);
		if (pStruct)
		{
			// create ref to schema
			pSchema = new Poco::JSON::Object;
			pSchema->set("$ref"s, "#/components/schemas/"s + translateTypeName(pStruct->fullName()));
		}
		else if (pSymbol && pSymbol->kind() == Poco::CppParser::Symbol::SYM_ENUM)
		{
			pSchema = createSchemaForBasicType("int"s);
		}
		else throw CodeGenerationException("Missing class", resolvedType);
	}
	if (isNullableType || isPtrType)
	{
		pSchema->set("nullable"s, true);
	}
	if (isOptionalType)
	{
		pSchema->set("required"s, false);
	}
	if (isVectorType)
	{
		Poco::JSON::Object::Ptr pArraySchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pArraySchema->set("type"s, "array"s);
		pArraySchema->set("items"s, pSchema);
		if (GenUtility::isUniqueVectorType(resolvedType))
		{
			pArraySchema->set("uniqueItems"s, true);
		}
		pSchema = pArraySchema;
	}
	return pSchema;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createSchemaForObject(const Poco::CppParser::Struct* pStruct)
{
	Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	std::string description = Poco::trim(pStruct->getDocumentation());
	if (!description.empty())
	{
		pSchema->set("description"s, description);
	}
	createSchemaForObject(pStruct, pSchema);
	return pSchema;
}


void SwaggerGenerator::createSchemaForObject(const Poco::CppParser::Struct* pStruct, Poco::JSON::Object::Ptr pSchema)
{
	auto itBase = pStruct->baseBegin();
	auto itBaseEnd = pStruct->baseEnd();
	for (; itBase != itBaseEnd; ++itBase)
	{
		const Poco::CppParser::Struct::Base& base = *itBase;
		if (GenUtility::mustBeSerializable(base.name))
		{
			if (!base.pClass)
				throw CodeGenerationException("Missing base class", base.name);
			createSchemaForObject(base.pClass, pSchema);
		}
	}

	SerializerGenerator::VarGet varMatch;
	SerializerGenerator::OrderedVars attrs;
	SerializerGenerator::OrderedVars elems;
	SerializerGenerator::matchVarsWithFunctions(pStruct, varMatch);
	SerializerGenerator::doElemAttrSplit(varMatch, attrs, elems);

	// Merge elems and attributes.
	SerializerGenerator::OrderedVars members;
	for (const auto& p: attrs) members.insert(p);
	for (const auto& p: elems) members.insert(p);

	pSchema->set("type"s, "object"s);

	Poco::JSON::Array::Ptr pRequired;
	if (pSchema->has("required"s))
		pRequired = pSchema->getArray("required"s);
	else
		pRequired = new Poco::JSON::Array;

	Poco::JSON::Object::Ptr pProperties;
	if (pSchema->has("properties"s))
		pProperties = pSchema->getObject("properties"s);
	else
		pProperties = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	pSchema->set("required"s, pRequired);
	pSchema->set("properties"s, pProperties);

	for (const auto& member: members)
	{
		std::string propertyName;
		bool isVector = false;
		bool isMandatory = true;
		bool isNullable = false;
		std::string resolvedType;
		Poco::CodeGeneration::CodeGenerator::Properties varProps;
		Poco::CodeGeneration::GeneratorEngine::parseProperties(member.second->second.first, varProps);
		variableProperties(member.second->second.first, isVector, isMandatory, isNullable, propertyName, resolvedType);
		const Poco::CppParser::Symbol* pSymbol = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
		if (!pSymbol)
		{
			pSymbol = pStruct->lookup(resolvedType); // for inner classes
			if (pSymbol)
				resolvedType = pSymbol->fullName();
		}

		if (isMandatory && !GenUtility::isOptionalType(resolvedType))
		{
			pRequired->add(propertyName);
		}

		Poco::JSON::Object::Ptr pPropSchema = createSchemaForType(pStruct, pSymbol, resolvedType);

		if (isVector)
		{
			Poco::JSON::Object::Ptr pArraySchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
			pArraySchema->set("type"s, "array"s);
			pArraySchema->set("items"s, pPropSchema);
			if (GenUtility::isUniqueVectorType(resolvedType))
			{
				pArraySchema->set("uniqueItems"s, true);
			}
			pPropSchema = pArraySchema;
		}

		if (isNullable)
		{
			pPropSchema->set("nullable"s, true);
		}

		std::string description = Poco::trim(member.second->second.first->getDocumentation());
		if (!description.empty())
		{
			pPropSchema->set("description"s, description);
		}

		fillTypeAttributes(pPropSchema, varProps, propertyName);

		pProperties->set(propertyName, pPropSchema);
	}

	// Empty "required" array is not allowed
	if (pRequired->size() == 0)
	{
		pSchema->remove("required"s);
	}
}


template <class FilterExpr>
class ParameterFilter
{
public:
	using PropsMap = std::map<std::string, AbstractGenerator::CodeGenerator::Properties>;
	ParameterFilter(const PropsMap& propsMap, const std::string& param, const FilterExpr& expr):
		_propsMap(propsMap),
		_param(param),
		_expr(expr)
	{
	}

	bool operator () (const Poco::CppParser::Parameter* pParam)
	{
		AbstractGenerator::CodeGenerator::Properties props;
		auto it = _propsMap.find(pParam->name());
		if (it != _propsMap.end()) props = it->second;
		std::string value;
		Poco::CodeGeneration::GeneratorEngine::getStringProperty(props, _param, value);
		return _expr(value);
	}

private:
	const PropsMap& _propsMap;
	std::string _param;
	FilterExpr _expr;
};


template <typename FilterExpr>
ParameterFilter<FilterExpr> makeParameterFilter(const typename ParameterFilter<FilterExpr>::PropsMap& propsMap, const std::string& param, const FilterExpr& expr)
{
	return ParameterFilter<FilterExpr>(propsMap, param, expr);
}


Poco::JSON::Object::Ptr SwaggerGenerator::createOperation(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	Poco::JSON::Object::Ptr pOperation = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	std::string description = Poco::trim(pFunc->getDocumentation());
	if (!description.empty())
	{
		pOperation->set("description"s, description);
	}

	const auto* pNamespace = pFunc->nameSpace();
	if (pNamespace && !pNamespace->name().empty())
	{
		Poco::JSON::Array::Ptr pTags = new Poco::JSON::Array;
		pTags->add(pFunc->nameSpace()->name());
		pOperation->set("tags"s, pTags);
	}

	Poco::JSON::Array::Ptr pParameters = new Poco::JSON::Array;

	auto propsMap = parseParameterProperties(pFunc, properties);

	std::vector<Poco::CppParser::Parameter*> params;
	std::copy_if(pFunc->begin(), pFunc->end(), std::back_inserter(params), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::DIRECTION,
			[](const std::string& dir)
			{
				return dir != "out";
			}
		));

	const std::vector<std::string> ins = {"path"s, "query"s, "header"s};
	for (const auto& in: ins)
	{
		std::vector<Poco::CppParser::Parameter*> selectedParams;
		std::copy_if(params.begin(), params.end(), std::back_inserter(selectedParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::IN,
			[&in](const std::string& what)
			{
				return in == what;
			}
		));

		for (const auto pParam: selectedParams)
		{
			Poco::JSON::Object::Ptr pParameter = createParameter(pFunc, pParam, propsMap[pParam->name()], in);
			pParameters->add(pParameter);
		}
	}

	if (pParameters->size() > 0)
	{
		pOperation->set("parameters"s, pParameters);
	}

	std::vector<Poco::CppParser::Parameter*> formParams;
	std::copy_if(params.begin(), params.end(), std::back_inserter(formParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::IN,
		[](const std::string& in)
		{
			return in == "form";
		}
	));

	std::vector<Poco::CppParser::Parameter*> bodyParams;
	std::copy_if(params.begin(), params.end(), std::back_inserter(bodyParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::IN,
		[](const std::string& in)
		{
			return in.empty() || in == "body";
		}
	));

	if ((formParams.size() > 0 || bodyParams.size() > 0) && !operationCanHaveBody(pFunc->name()))
	{
		throw CodeGenerationException(Poco::format("Operation %s in %s cannot have form or body parameters.",
			pFunc->name(), _pStructIn->fullName()));
	}

	if (formParams.size() > 0 && bodyParams.size() > 0)
	{
		throw CodeGenerationException(Poco::format("Operation %s in %s cannot have both form and body parameters.",
			pFunc->name(), _pStructIn->fullName()));
	}

	if (bodyParams.size() > 1)
	{
		throw CodeGenerationException(Poco::format("Operation %s in %s cannot have more than one body parameter.",
			pFunc->name(), _pStructIn->fullName()));
	}

	if (formParams.size() > 0)
	{
		pOperation->set("requestBody"s, createFormRequestBody(pFunc, formParams, propsMap, properties));
	}

	if (bodyParams.size() > 0)
	{
		pOperation->set("requestBody"s, createRequestBody(pFunc, bodyParams[0], propsMap[bodyParams[0]->name()], properties));
	}

	Poco::JSON::Object::Ptr pResponses = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	Poco::JSON::Object::Ptr pDefaultResponse = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pDefaultResponse->set("description"s, "The response to a successful request."s);
	pResponses->set("default"s, pDefaultResponse);
	pResponses->set("4XX"s, createErrorResponse("The response to an invalid request."s));
	pResponses->set("5XX"s, createErrorResponse("The response to a failed request."s));

	std::vector<Poco::CppParser::Parameter*> responseParams;
	std::copy_if(pFunc->begin(), pFunc->end(), std::back_inserter(responseParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::DIRECTION,
			[](const std::string& dir)
			{
				return dir == "out" || dir == "inout";
			}
		));

	std::vector<Poco::CppParser::Parameter*> headerResponseParams;
	std::copy_if(responseParams.begin(), responseParams.end(), std::back_inserter(headerResponseParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::IN,
		[](const std::string& in)
		{
			return in == "header";
		}
	));

	if (headerResponseParams.size() > 0)
	{
		Poco::JSON::Object::Ptr pHeaders = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
		for (const auto pParam: headerResponseParams)
		{
			bool isVector = false;
			bool isMandatory = false;
			bool isNullable = false;
			std::string name;
			std::string resolvedType;
			parameterProperties(pFunc, pParam, isVector, isMandatory, isNullable, name, resolvedType);

			if (Poco::icompare(name, "Content-Type"s) != 0)
			{
				if (!isBasicType(resolvedType))
				{
					throw CodeGenerationException(Poco::format("Type %s cannot be used in header response parameter %s in operation %s in %s.",
						resolvedType, pParam->name(), pFunc->name(), _pStructIn->fullName()));
				}

				Poco::JSON::Object::Ptr pHeader = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
				pHeader->set("schema"s, createSchemaForType(_pStructIn, nullptr, resolvedType));
				pHeaders->set(name, pHeader);
			}
		}

		pDefaultResponse->set("headers"s, pHeaders);
	}

	std::vector<Poco::CppParser::Parameter*> bodyResponseParams;
	std::copy_if(responseParams.begin(), responseParams.end(), std::back_inserter(bodyResponseParams), makeParameterFilter(propsMap, Poco::CodeGeneration::Utility::IN,
		[](const std::string& in)
		{
			return in.empty() || in == "body";
		}
	));

	std::string returnType = GenUtility::getResolvedReturnParameterType(_pStructIn, pFunc);
	std::size_t bodyResponseParamsCount = bodyResponseParams.size();
	if (returnType != Poco::CodeGeneration::Utility::TYPE_VOID)
	{
		bodyResponseParamsCount++;
	}
	if (bodyResponseParamsCount > 1)
	{
		throw CodeGenerationException(Poco::format("Cannot have more than one body response parameter in operation %s in %s.",
			pFunc->name(), _pStructIn->fullName()));
	}

	if (returnType != Poco::CodeGeneration::Utility::TYPE_VOID)
	{
		CodeGenerator::Properties returnProps;
		std::string returnPropsStr;
		Poco::CodeGeneration::GeneratorEngine::getStringProperty(properties, GenUtility::ATTR_RETURN, returnPropsStr);
		if (!returnPropsStr.empty())
		{
			Poco::CodeGeneration::GeneratorEngine::parseElementProperties(returnPropsStr, returnProps);
		}

		pDefaultResponse->set("content"s, createResponseBodyContent(returnType, returnProps, properties));
	}
	else if (bodyResponseParams.size() > 0)
	{
		CodeGenerator::Properties paramProps = parseParameterProperties(bodyResponseParams[0], properties);
		bool isVector = false;
		bool isMandatory = false;
		bool isNullable = false;
		std::string name;
		std::string resolvedType;
		parameterProperties(pFunc, bodyResponseParams[0], isVector, isMandatory, isNullable, name, resolvedType);

		pDefaultResponse->set("content"s, createResponseBodyContent(resolvedType, paramProps, properties));
	}

	pOperation->set("responses"s, pResponses);

	return pOperation;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createParameter(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& properties, const std::string& in)
{
	Poco::JSON::Object::Ptr pParamObject = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	bool isVector = false;
	bool isMandatory = false;
	bool isNullable = false;
	std::string name;
	std::string resolvedType;
	parameterProperties(pFunc, pParam, isVector, isMandatory, isNullable, name, resolvedType);

	pParamObject->set("name"s, name);
	pParamObject->set("in"s, in);
	if (in == "path" || isMandatory)
	{
		pParamObject->set("required"s, true);
	}

	std::string description = Poco::trim(pParam->getDocumentation());
	if (!description.empty())
	{
		pParamObject->set("description"s, description);
	}

	if (!isBasicType(resolvedType))
	{
		throw CodeGenerationException(Poco::format("Type %s cannot be used in %s parameter %s in operation %s in %s.",
			resolvedType, in, pParam->name(), pFunc->name(), _pStructIn->fullName()));
	}

	Poco::JSON::Object::Ptr pSchema = createSchemaForType(_pStructIn, nullptr, resolvedType);
	fillTypeAttributes(pSchema, properties, name);
	pParamObject->set("schema"s, pSchema);

	return pParamObject;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createFormRequestBody(const Poco::CppParser::Function* pFunc, const std::vector<Poco::CppParser::Parameter*>& params, const PropMap& propMap, const CodeGenerator::Properties& funcProps)
{
	Poco::JSON::Object::Ptr pRequestBody = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	Poco::JSON::Object::Ptr pContent = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	pRequestBody->set("content"s, pContent);

	std::string consumes;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, Poco::CodeGeneration::Utility::CONSUMES, consumes);
	if (consumes.empty()) consumes = "application/x-www-form-urlencoded";

	if (consumes != "multipart/form-data" && consumes != "application/x-www-form-urlencoded")
	{
		throw CodeGenerationException(Poco::format("consumes value \"%s\" for operation %s in %s is invalid. Must be \"application/x-www-form-urlencoded\" or \"multipart/form-data\".",
			consumes, pFunc->name(), _pStructIn->fullName()));
	}

	Poco::JSON::Object::Ptr pContentType = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pContent->set(consumes, pContentType);

	Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pSchema->set("type"s, "object"s);

	Poco::JSON::Array::Ptr pRequired = new Poco::JSON::Array;
	pSchema->set("required"s, pRequired);

	Poco::JSON::Object::Ptr pProperties = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pSchema->set("properties"s, pProperties);

	Poco::JSON::Object::Ptr pEncoding = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	for (const auto pParam: params)
	{
		bool isVector = false;
		bool isMandatory = false;
		bool isNullable = false;
		std::string name;
		std::string resolvedType;
		parameterProperties(pFunc, pParam, isVector, isMandatory, isNullable, name, resolvedType);

		if (!isBasicType(resolvedType))
		{
			throw CodeGenerationException(Poco::format("Type %s cannot be used in form parameter %s in operation %s in %s.",
				resolvedType, pParam->name(), pFunc->name(), _pStructIn->fullName()));
		}

		Poco::JSON::Object::Ptr pParamSchema = createSchemaForType(_pStructIn, nullptr, resolvedType);
		pProperties->set(name, pParamSchema);

		if (isMandatory)
		{
			pRequired->add(name);
		}

		if (consumes == "multipart/form-data")
		{
			CodeGenerator::Properties paramProps;
			auto it = propMap.find(pParam->name());
			if (it != propMap.end()) paramProps = it->second;

			std::string contentType;
			Poco::CodeGeneration::GeneratorEngine::getStringProperty(paramProps, Poco::CodeGeneration::Utility::CONTENT_TYPE, contentType);
			if (!contentType.empty())
			{
				Poco::JSON::Object::Ptr pParamEncoding = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
				pParamEncoding->set("contentType"s, contentType);
				pEncoding->set(name, pParamEncoding);
			}

			if (pParamSchema->optValue("type"s, ""s) == "string")
			{
				std::string format;
				Poco::CodeGeneration::GeneratorEngine::getStringProperty(paramProps, Poco::CodeGeneration::Utility::FORMAT, format);
				if (pParamSchema->optValue("format"s, ""s) == "byte" || format == "raw" || format == "binary")
				{
					pParamSchema->set("format"s, "binary"s);
				}
			}
		}
	}
	if (pRequired->size() == 0)
	{
		pSchema->remove("required"s);
	}

	pContentType->set("schema"s, pSchema);

	if (pEncoding->size() > 0)
	{
		pContentType->set("encoding"s, pEncoding);
	}

	return pRequestBody;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createRequestBody(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& paramProps, const CodeGenerator::Properties& funcProps)
{
	Poco::JSON::Object::Ptr pRequestBody = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	Poco::JSON::Object::Ptr pContent = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	pRequestBody->set("content"s, pContent);

	std::string format;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(paramProps, Poco::CodeGeneration::Utility::FORMAT, format);

	std::string consumes;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, Poco::CodeGeneration::Utility::CONSUMES, consumes);
	if (consumes.empty())
	{
		if (format == "json" || format.empty())
			consumes = "application/json";
		else
			consumes = "*/*";
	}

	Poco::JSON::Object::Ptr pContentType = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pContent->set(consumes, pContentType);

	bool isVector = false;
	bool isMandatory = false;
	bool isNullable = false;
	std::string name;
	std::string resolvedType;
	parameterProperties(pFunc, pParam, isVector, isMandatory, isNullable, name, resolvedType);

	const Poco::CppParser::Symbol* pSymbol = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
	if (!pSymbol)
	{
		pSymbol = _pStructIn->lookup(resolvedType); // for inner classes
		if (pSymbol)
			resolvedType = pSymbol->fullName();
	}

	Poco::JSON::Object::Ptr pSchema = createSchemaForType(_pStructIn, pSymbol, resolvedType);
	if (pSchema->optValue("format"s, ""s) == "byte" || format == "raw" || format == "binary")
	{
		pSchema->set("format"s, "binary"s);
	}
	pContentType->set("schema"s, pSchema);

	return pRequestBody;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createResponseBodyContent(const std::string& type, const CodeGenerator::Properties& paramProps, const CodeGenerator::Properties& funcProps)
{
	Poco::JSON::Object::Ptr pContent = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	std::string format;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(paramProps, Poco::CodeGeneration::Utility::FORMAT, format);

	std::string produces;
	Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, Poco::CodeGeneration::Utility::PRODUCES, produces);
	if (produces.empty())
	{
		if (format == "json" || format.empty())
			produces = "application/json";
		else
			produces = "*/*";
	}

	Poco::JSON::Object::Ptr pContentType = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pContent->set(produces, pContentType);

	bool isVector = false;
	bool isMandatory = false;
	bool isNullable = false;
	std::string resolvedType;
	typeProperties(type, isVector, isMandatory, isNullable, resolvedType);

	const Poco::CppParser::Symbol* pSymbol = Poco::CppParser::NameSpace::root()->lookup(resolvedType);
	if (!pSymbol)
	{
		pSymbol = _pStructIn->lookup(resolvedType); // for inner classes
		if (pSymbol)
			resolvedType = pSymbol->fullName();
	}

	Poco::JSON::Object::Ptr pSchema = createSchemaForType(_pStructIn, pSymbol, resolvedType);
	if (pSchema->optValue("format"s, ""s) == "byte" || format == "raw" || format == "binary")
	{
		pSchema->set("format"s, "binary"s);
	}

	if (isVector)
	{
		Poco::JSON::Object::Ptr pArraySchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pArraySchema->set("type"s, "array"s);
		pArraySchema->set("items"s, pSchema);
		if (GenUtility::isUniqueVectorType(resolvedType))
		{
			pArraySchema->set("uniqueItems"s, true);
		}
		pSchema = pArraySchema;
	}

	pContentType->set("schema"s, pSchema);

	return pContent;
}


Poco::JSON::Object::Ptr SwaggerGenerator::createErrorResponse(const std::string& description)
{
	Poco::JSON::Object::Ptr pErrorResponse = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pErrorResponse->set("description"s, description);
	Poco::JSON::Object::Ptr pContent = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	Poco::JSON::Object::Ptr pContentType = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pSchema->set("$ref"s, "#/components/schemas/Poco.RemotingNG.Error"s);
	pContentType->set("schema"s, pSchema);
	pContent->set("application/json"s, pContentType);
	pErrorResponse->set("content"s, pContent);
	return pErrorResponse;
}


SwaggerGenerator::PropMap SwaggerGenerator::parseParameterProperties(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	PropMap propsMap;

	for (const auto& pParam: *pFunc)
	{
		propsMap[pParam->name()] = parseParameterProperties(pParam, properties);
	}

	return propsMap;
}


AbstractGenerator::CodeGenerator::Properties SwaggerGenerator::parseParameterProperties(const Poco::CppParser::Parameter* pParam, const CodeGenerator::Properties& funcProps)
{
	CodeGenerator::Properties paramProps;

	std::string propName("$"s + pParam->name());
	auto itProp = funcProps.find(propName);

	if (itProp != funcProps.end())
	{
		Poco::CodeGeneration::GeneratorEngine::parseElementProperties(itProp->second, paramProps);
	}
	return paramProps;
}


void SwaggerGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStructIn = pStruct;

	bool serialize = false;
	Poco::CodeGeneration::GeneratorEngine::getBoolProperty(properties, Poco::CodeGeneration::Utility::SERIALIZE, serialize);
	if (serialize)
	{
		Poco::JSON::Object::Ptr pObject = createSchemaForObject(pStruct);
		_pSchemas->set(translateTypeName(pStruct->fullName()), pObject);
	}
}


void SwaggerGenerator::structEnd()
{
}


void SwaggerGenerator::methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
	if (pFunc->getAccess() == Poco::CppParser::Variable::ACC_PUBLIC)
	{
		bool remote = false;
		Poco::CodeGeneration::GeneratorEngine::getBoolProperty(properties, Poco::CodeGeneration::Utility::REMOTE, remote);
		if (remote && isValidOperation(pFunc->name()))
		{
			std::string path;
			Poco::CodeGeneration::GeneratorEngine::getStringProperty(properties, Poco::CodeGeneration::Utility::PATH, path);
			if (!path.empty())
			{
				std::string operation = pFunc->name();
				if (operation == "delete_") operation = "delete";

				Poco::JSON::Object::Ptr pPathObject = _pPaths->getObject(path);
				if (!pPathObject)
				{
					pPathObject = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
					_pPaths->set(path, pPathObject);
				}
				pPathObject->set(operation, createOperation(pFunc, properties));
			}
		}
	}
}


void SwaggerGenerator::methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties)
{
}


void SwaggerGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
}


std::string SwaggerGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return pStruct->name();
}


std::vector<std::string> SwaggerGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;
	return bases;
}


std::string SwaggerGenerator::translateTypeName(const std::string& typeName)
{
	return Poco::replace(typeName, "::"s, "."s);
}


bool SwaggerGenerator::isValidOperation(const std::string& name)
{
	return
		name == "get" ||
		name == "put" ||
		name == "post" ||
		name == "delete_" ||
		name == "options" ||
		name == "head" ||
		name == "patch" ||
		name == "trace";
}


bool SwaggerGenerator::operationCanHaveBody(const std::string& name)
{
	return
		name == "put" ||
		name == "post" ||
		name == "patch";
}


void SwaggerGenerator::variableProperties(const Poco::CppParser::Variable* pVar, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType)
{
	const std::string& decl = pVar->declType();

	typeProperties(decl, isVector, isMandatory, isNullable, resolvedType);

	isMandatory = isMandatory && GenUtility::getIsMandatory(pVar);
	name = GenUtility::getVariableName(pVar);
}


void SwaggerGenerator::parameterProperties(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType)
{
	const std::string& decl = pParam->declType();

	typeProperties(decl, isVector, isMandatory, isNullable, resolvedType);

	isMandatory = isMandatory && GenUtility::getIsMandatory(pFunc, pParam);
	name = GenUtility::getParameterName(pFunc, pParam);
}


void SwaggerGenerator::typeProperties(const std::string& decl, bool& isVector, bool& isMandatory, bool& isNullable, std::string& resolvedType)
{
	resolvedType = decl;
	std::string tmp;
	poco_assert (!resolvedType.empty());
	do
	{
		tmp = resolveType(resolvedType);
		if (tmp != resolvedType)
		{
			resolvedType = tmp;
			tmp.clear();
		}
	}
	while (tmp != resolvedType); // resolve recursive typedefs

	poco_assert (!resolvedType.empty());
	if (GenUtility::isPtrType(resolvedType))
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(resolvedType);
		if (types.size() != 1)
			throw CodeGenerationException("Unsupported template type", decl);
		resolvedType = types[0];
	}
	poco_assert (!resolvedType.empty());
	isVector = GenUtility::isVectorType(resolvedType);
	isNullable = GenUtility::isNullableType(resolvedType);
	isMandatory = !GenUtility::isOptionalType(resolvedType);
	bool isVectorOrOptional = isVector || !isMandatory || isNullable;
	if (isVectorOrOptional)
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(resolvedType);
		if (types.size() == 1 || (types.size() == 2 && GenUtility::isNullableType(types[0])))
		{
			resolvedType = types[types.size() - 1];
		}
		else throw CodeGenerationException("Unsupported template type", decl);
	}
	else if (GenUtility::isTemplateType(resolvedType))
	{
		std::vector<std::string> types = GenUtility::getInnerTemplateTypes(resolvedType);
		if (types.size() != 1)
			throw CodeGenerationException("Unsupported template type", decl);
	}

	do
	{
		tmp = resolveType(resolvedType);
		if (tmp != resolvedType)
		{
			resolvedType = tmp;
			tmp.clear();
		}
	}
	while (tmp != resolvedType); // resolve recursive typedefs

	poco_assert (!resolvedType.empty());
}


void SwaggerGenerator::fillTypeAttributes(Poco::JSON::Object::Ptr pSchema, const CodeGenerator::Properties& properties, const std::string& name)
{
	struct AttrInfo
	{
		std::string name;
		char type;
	};

	static const std::map<std::string, AttrInfo> ATTRS = {
		{"min"s, {"minimum"s, 'f'}},
		{"max"s, {"maximum"s, 'f'}},
		{"exclusiveMin"s, {"exclusiveMinimum"s, 'b'}},
		{"exclusiveMax"s, {"exclusiveMaximum"s, 'b'}},
		{"minLength"s, {"minLength"s, 'i'}},
		{"maxLength"s, {"maxLength"s, 'i'}},
		{"pattern"s, {"pattern"s, 's'}},
		{"minItems"s, {"minItems"s, 'i'}},
		{"maxItems"s, {"maxItems"s, 'i'}},
		{"multipleOf"s, {"multipleOf"s, 'f'}}
	};

	for (const auto& attr: ATTRS)
	{
		const auto it = properties.find(attr.first);
		if (it != properties.end())
		{
			try
			{
				switch (attr.second.type)
				{
				case 'i':
					pSchema->set(attr.second.name, Poco::NumberParser::parse64(it->second));
					break;
				case 'f':
					pSchema->set(attr.second.name, Poco::NumberParser::parseFloat(it->second));
					break;
				case 'b':
					pSchema->set(attr.second.name, Poco::NumberParser::parseBool(it->second));
					break;
				case 's':
					pSchema->set(attr.second.name, it->second);
					break;
				}
			}
			catch (Poco::Exception& exc)
			{
				throw Poco::InvalidArgumentException(Poco::format("Invalid value for attribute %s of %s: %s", it->first, name, exc.displayText()));
			}
		}
	}
}
