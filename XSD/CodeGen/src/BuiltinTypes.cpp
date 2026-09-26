//
// BuiltinTypes.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "BuiltinTypes.h"
#include "ClassInfo.h"
#include "Poco/Exception.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"


using Poco::XSD::Types::TypesManager;


BuiltinTypes::BuiltinTypes():
	_types(),
	_typesCpp(),
	_stringTypes()
{
	//name, c++ns, xsdns, inc, sysInc, vec
	add(TypesManager::XSD_TYPE_STRING, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_STRING, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_BOOLEAN, TypeInfo("bool", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_BOOLEAN, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_DECIMAL, TypeInfo("double", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_DECIMAL, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_FLOAT, TypeInfo("float", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_FLOAT, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_DOUBLE, TypeInfo("double", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_DOUBLE, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_DURATION, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_DURATION, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_DATETIME, TypeInfo("DateTime", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_DATETIME, "Poco/DateTime.h", false, false, false), true);
	add(TypesManager::XSD_TYPE_TIME, TypeInfo("DateTime", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_TIME, "Poco/DateTime.h", false, false, false), true);
	add(TypesManager::XSD_TYPE_DATE, TypeInfo("DateTime", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_DATE, "Poco/DateTime.h", false, false, false), true);
	add(TypesManager::XSD_TYPE_GYEARMONTH, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_GYEARMONTH, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_GYEAR, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_GYEAR, "string", true, false, true), false);
	add(TypesManager::XSD_TYPE_GMONTHDAY, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_GMONTHDAY, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_GDAY, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_GDAY, "string", true, false, true), false);
	add(TypesManager::XSD_TYPE_GMONTH, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_GMONTH, "string", true, false, true), false);
	add(TypesManager::XSD_TYPE_HEX_BINARY, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_HEX_BINARY, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_BASE64BINARY, TypeInfo("vector<char>", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_BASE64BINARY, "vector", true, false, false), true);
	add(TypesManager::XSD_TYPE_ANYURI, TypeInfo("URI", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ANYURI, "Poco/URI.h", false, false, false), true);
	add(TypesManager::XSD_TYPE_ANYTYPE, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ANYTYPE, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_ANYSIMPLETYPE, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ANYSIMPLETYPE, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_QNAME, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_QNAME, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NOTATION, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NOTATION, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NORMALIZEDSTRING, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NORMALIZEDSTRING, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_INTEGER, TypeInfo("int", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_INTEGER, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_TOKEN, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_TOKEN, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NONPOSITIVEINTEGER, TypeInfo("Int32", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NONPOSITIVEINTEGER, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_LONG, TypeInfo("Int64", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_LONG, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_NONNEGATIVEINTEGER, TypeInfo("UInt32", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NONNEGATIVEINTEGER, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_LANGUAGE, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_LANGUAGE, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NAME, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NAME, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NMTOKEN, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NMTOKEN, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NEGATIVEINTEGER, TypeInfo("Int32", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NEGATIVEINTEGER, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_INT, TypeInfo("int", "", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_INT, "", true, false, true), false);
	add(TypesManager::XSD_TYPE_UNSIGNEDLONG, TypeInfo("UInt64", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_UNSIGNEDLONG, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_POSITIVEINTEGER, TypeInfo("UInt32", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_POSITIVEINTEGER, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_NCNAME, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NCNAME, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_NMTOKENS, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_NMTOKENS, "string", true, true, false), true);
	add(TypesManager::XSD_TYPE_SHORT, TypeInfo("Int16", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_SHORT, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_UNSIGNEDINT, TypeInfo("UInt32", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_UNSIGNEDINT, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_ID, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ID, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_IDREF, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_IDREF, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_ENTITY, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ENTITY, "string", true, false, false), true);
	add(TypesManager::XSD_TYPE_BYTE, TypeInfo("Int8", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_BYTE, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_UNSIGNEDSHORT, TypeInfo("UInt16", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_UNSIGNEDSHORT, "Poco/Types.h", false, false, true), false);
	add(TypesManager::XSD_TYPE_IDREFS, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_IDREFS, "string", true, true, false), true);
	add(TypesManager::XSD_TYPE_ENTITIES, TypeInfo("string", "std", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_ENTITIES, "string", true, true, false), true);
	add(TypesManager::XSD_TYPE_UNSIGNEDBYTE, TypeInfo("UInt8", "Poco", TypesManager::XSD_NAMESPACE, TypesManager::XSD_TYPE_UNSIGNEDBYTE, "Poco/Types.h", false, false, true), false);
}


BuiltinTypes::~BuiltinTypes()
{
}


BuiltinTypes& BuiltinTypes::instance()
{
	static BuiltinTypes types;
	return types;
}


bool BuiltinTypes::tryGet(const std::string& xsdType, TypeInfo& result) const
{
	std::map<std::string, TypeInfo>::const_iterator it = _types.find(xsdType);
	bool found = (it != _types.end());
	if (found)
		result = it->second;
	return found;
}


const TypeInfo& BuiltinTypes::get(const std::string& xsdType) const
{
	std::map<std::string, TypeInfo>::const_iterator it = _types.find(xsdType);
	if (it == _types.end())
	{
		throw Poco::NotFoundException(xsdType);
	}

	return it->second;
}


void BuiltinTypes::add(const std::string& key, const TypeInfo& val, bool isString)
{
	_types.insert(std::make_pair(key, val));
	_typesCpp.insert(val);
	if (isString)
		_stringTypes.insert(val);
}


bool BuiltinTypes::isKnownTypeInfo(const TypeInfo& info) const
{
	if (info.getFullName().compare(0, 12, "std::vector<") == 0)
	{
		std::string fullName = info.getFullName();
		std::string::size_type p = fullName.find_last_of('>');
		std::string baseType(fullName, 12, p - 12);
		if (baseType == "char") return true;
		for (const auto& t: _typesCpp)
		{
			if (t.getFullName() == baseType || t.name() == baseType) return true;
		}
		return false;
	}
	else return _typesCpp.find(info) != _typesCpp.end();
}


bool BuiltinTypes::isStringType(const TypeInfo& info) const
{
	return _stringTypes.find(info) != _stringTypes.end();
}


std::string BuiltinTypes::generateInitializeValue(ClassInfo& ci, Constructor& constr, const Variable& var, const std::string& xsdString) const
{
	const TypeInfo& info = var.getType();
	// if we have a Int64 or UInt64, we must guarantee that the const is notlarger than 32 bit, otherwise 
	// we init with a calculated value
	if (info.name() == "UInt64")
	{
		Poco::UInt64 val = Poco::NumberParser::parseUnsigned64(xsdString);
		return Poco::NumberFormatter::format(val) + "ull";
	}
	if (info.name() == "Int64")
	{
		Poco::Int64 val = Poco::NumberParser::parse64(xsdString);
		return Poco::NumberFormatter::format(val) + "ll";
	}
	if (info.name() == "DateTime")
	{
		ci.addSrcInclude("Poco/DateTimeParser.h", false);
		ci.addSrcInclude("Poco/DateTimeFormat.h", false);
		// can't init in constructor header do in code
		std::string fmt;
		if (info.xsdType() == "date")
			fmt = "\"%Y-%m-%d\"";
		else if (info.xsdType() == "time")
			fmt = "\"%H:%M:%s\"";
		else
			fmt = "Poco::DateTimeFormat::ISO8601_FORMAT";
		std::string intName("ltz" + var.getName());
		constr.addCode("int " + intName + "(0);");
		std::string code(var.getName() + " = Poco::DateTimeParser::parse(" + fmt + ", ");
		code += "\"" + xsdString + "\", " + intName + ");";
		constr.addCode(code);
		return "";
	}
	if (info.name() == "URI")
	{
		return std::string("Poco::URI(\"") + xsdString + "\")";
	}
	if (isStringType(info))
		return std::string("\"") + xsdString + "\"";
	return xsdString;
}
