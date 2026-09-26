//
// BuiltinTypes.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
	static BuiltinTypes& instance();
		/// Returns an instance of the singleton

	bool tryGet(const std::string& xsdType, TypeInfo& result) const;
		/// If it finds a type, the method returns true and sets result

	const TypeInfo& get(const std::string& xsdType) const;
		/// Like tryGet but throws an exception if the type is not found

	bool isKnownTypeInfo(const TypeInfo& info) const;
		/// returns true if the info is from the set of primitve cpp types

	bool isStringType(const TypeInfo& info) const;
		/// Returns true if we can initialize the type with a string

	std::string generateInitializeValue(ClassInfo& ci, 
		Constructor& constr, 
		const Variable& var,
		const std::string& xsdString) const;
		/// Converts a value from a schema to an initialization value for a cpp type
private:
	void add(const std::string& key, const TypeInfo& val, bool isString);

	BuiltinTypes();
	BuiltinTypes(const BuiltinTypes&);
	BuiltinTypes& operator=(const BuiltinTypes&);
	~BuiltinTypes();

private:
	std::map<std::string, TypeInfo> _types;
	std::set<TypeInfo> _typesCpp;
	std::set<TypeInfo> _stringTypes;
};


#endif // CodeGen_BuiltinTypes_H_INCLUDED
