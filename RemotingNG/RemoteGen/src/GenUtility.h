//
// GenUtility.h
//
// Definition of the GenUtility class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_GenUtility_INCLUDED
#define RemoteGen_GenUtility_INCLUDED


#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Variable.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/Exception.h"
#include <vector>


#define RemoteGen_API
POCO_DECLARE_EXCEPTION(RemoteGen_API, CodeGenerationException, Poco::ApplicationException)


class GenUtility
	/// GenUtility functions
{
public:
	static const std::string ATTR_HEADER;
	static const std::string ATTR_RETURN;

	static bool isRemoteMethod(const Poco::CppParser::Function* pFunc, const Poco::CodeGeneration::CodeGenerator::Properties& properties);

	static bool isRemoteEvent(const Poco::CppParser::Variable* pVar, const Poco::CodeGeneration::CodeGenerator::Properties& properties);

	static std::string getMethodName(const Poco::CppParser::Function* pFunc);

	static std::string getRequestMethodName(const Poco::CppParser::Function* pFunc);

	static std::string getReplyMethodName(const Poco::CppParser::Function* pFunc);

	static std::string getParameterName(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam);

	static std::string getVariableName(const Poco::CppParser::Variable* pVar);

	static Poco::UInt32 getVariableOrder(const Poco::CppParser::Variable* pVar, Poco::UInt32 defValue);

	static Poco::UInt32 getParameterOrder(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pVar, Poco::UInt32 defValue);

	static bool getIsMandatory(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam);

	static bool getIsMandatory(const Poco::CppParser::Variable* pVar);

	static bool getIsMandatory(const Poco::CodeGeneration::CodeGenerator::Properties& props);

	static bool getIsInHeader(const Poco::CppParser::Function* pFunc, const Poco::CppParser::Parameter* pParam);

	static bool getInlineReturnParam(const Poco::CppParser::Function* pFunc);

	static std::string getReturnParameterName(const Poco::CppParser::Function* pFunc);

	static std::string cleanVariableName(const std::string& origName);
	/// Extracts the core name from a variable name, e.g.: m_id -> id, _ixyz -> xyz

	static std::string getReturnParameterType(const Poco::CppParser::Function* pFunc);
		/// Returns Poco::CodeGeneration::Utility::TYPE_VOID for void data types, otherwise returns the type of the return parameter.

	static std::string getResolvedReturnParameterType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Function* pFunc);
		/// Returns Poco::CodeGeneration::Utility::TYPE_VOID for void data types, otherwise returns the type of the return parameter.
		/// The returned type is already resolved

	static bool isVectorType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam);
		/// a vector type is everything that can be mapped directly to an XML schema collection type with maxOccurs=unbounded

	static bool isVectorType(const std::string& resolvedType);

	static bool isUniqueVectorType(const std::string& resolvedType);
		/// returns true if type is std::set or std::unordered_set

	static bool isArrayType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam);
		/// returns true if type is Poco::Array or std::array.

	static bool isArrayType(const std::string& resolvedType);

	static bool isTemplateType(const std::string& resolvedType);

	static bool isPtrType(const std::string& type);
		// returns true if the type is either an AutoPtr or a SharedPtr

	static bool isNullableType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam);
		/// returns true if the type is Poco::Nullable

	static bool isNullableType(const std::string& type);
		/// returns true if the type is Poco::Nullable

	static bool isOptionalType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam);
		/// returns true if the type is Poco::Optional or std::optional

	static bool isStdOptionalType(const Poco::CppParser::NameSpace* pNS, const Poco::CppParser::Parameter* pParam);
		/// returns true if the type is std::optional

	static bool isOptionalType(const std::string& type);
		/// returns true if the type is Poco::Optional or std::optional

	static bool isStdOptionalType(const std::string& type);
		/// returns true if the type is std::optional

	static std::vector<std::string> getInnerTemplateTypes(const std::string& typeDecl);
		/// Returns a vector containing the types used to instantiate the template

	static std::vector<std::string> getResolvedInnerTemplateTypes(const Poco::CppParser::NameSpace* pNS, const std::string& typeDecl);
		/// Returns a vector containing the types used to instantiate the template

	static bool mustBeSerializable(const std::string& name);
		/// Checks whether a (parent) class must be serializable or can be ignored

	static bool hasAnyRemoteParent(const Poco::CppParser::Struct* pStruct);

	static Poco::UInt64 parseExpireTime(const std::string& expireTimeStr);
		///Parses an expire time str. Is an integer value followed by a unit (e.g.:12ms, 3 min)
		///Returns a microseconds value

	static bool hasEvents(const Poco::CppParser::Struct* pService);
		/// Check if the Struct or any of its parent classes have events

	static bool parentsHaveEvents(const Poco::CppParser::Struct* pStruct);
		/// Checks only the parent classes for exporting events

	static void checkFunctionParams(const Poco::CppParser::Function* pFunction);
		/// Checks for unnamed parameters.

	static bool isOverride(const std::string& funcName, const Poco::CppParser::Struct* pStruct);
		/// Returns true if the function with the given name is an overrided virtual function.

private:
	GenUtility();
	~GenUtility();
	GenUtility(const GenUtility&);
	GenUtility& operator = (const GenUtility&);

	static const std::string ATTR_INLINE;
	static const std::string ATTR_NAME;
	static const std::string ATTR_REPLYNAME;
	static const std::string VAL_REQUEST;
	static const std::string VAL_REPLY;

	enum
	{
		KEYS_VECTOR_SIZE = 8,
		KEYS_UNIQUE_VECTOR_SIZE = 4
	};

	static const std::string KEYS_VECTOR[KEYS_VECTOR_SIZE];
	static const std::string KEYS_UNIQUE_VECTOR[KEYS_UNIQUE_VECTOR_SIZE];
};


#endif // RemoteGen_GenUtility_INCLUDED
