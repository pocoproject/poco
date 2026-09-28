//
// BuiltinTypes.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_BuiltinTypes_H_INCLUDED
#define CodeGen_BuiltinTypes_H_INCLUDED


#include "TypeInfo.h"
#include <string>
#include <map>
#include <set>


class ClassInfo;
class Constructor;
class Variable;


class BuiltinTypes
	/// Singleton containing the mappings of xsd types to cpp types
{
public:
	BuiltinTypes(const BuiltinTypes&) = delete;
	BuiltinTypes& operator=(const BuiltinTypes&) = delete;

	[[nodiscard]] static BuiltinTypes& instance();
		/// Returns an instance of the singleton

	[[nodiscard]] bool tryGet(const std::string& xsdType, TypeInfo& result) const;
		/// If it finds a type, the method returns true and sets result

	[[nodiscard]] const TypeInfo& get(const std::string& xsdType) const;
		/// Like tryGet but throws an exception if the type is not found

	[[nodiscard]] bool isKnownTypeInfo(const TypeInfo& info) const;
		/// returns true if the info is from the set of primitve cpp types

	[[nodiscard]] bool isStringType(const TypeInfo& info) const;
		/// Returns true if we can initialize the type with a string

	[[nodiscard]] std::string generateInitializeValue(ClassInfo& ci, 
		Constructor& constr, 
		const Variable& var,
		const std::string& xsdName,
		const std::string& xsdString) const;
		/// Converts the default or fixed value xsdString of the schema declaration xsdName
		/// to an initialization value for the cpp type of var, and adds the includes it needs to ci.
		/// For a DateTime, adds the assignment to constr instead and returns an empty string.
		/// Throws a Poco::DataFormatException if the value is not valid for the type, and a
		/// Poco::NotImplementedException if the type has no conversion.
private:
	void add(const std::string& key, const TypeInfo& val, bool isString);

	BuiltinTypes();
	~BuiltinTypes();

private:
	std::map<std::string, TypeInfo> _types;
	std::set<TypeInfo> _typesCpp;
	std::set<TypeInfo> _stringTypes;
};


#endif // CodeGen_BuiltinTypes_H_INCLUDED
