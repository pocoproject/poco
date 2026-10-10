//
// BuiltinTypes.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BuiltinTypes.h"
#include "ClassInfo.h"
#include "Utility.h"
#include "Poco/Exception.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/RegularExpression.h"
#include "Poco/String.h"
#include <limits>


using Poco::XSD::Types::TypesManager;


namespace
{
	enum class IntegerValue
		/// The result of checking a default or fixed value of an integer type.
	{
		Invalid,
		Literal,
		Minimum
	};


	struct IntegerType
		/// The value check, C++ type and literal suffix of an XSD integer type.
	{
		IntegerValue (*format)(const std::string& value, std::string& literal);
		const char* cppType;
		const char* suffix;
	};


	bool isIntegerLexical(const std::string& value)
		/// Returns true if value is an optional "+" or "-" followed by one or more ASCII digits.
	{
		const std::size_t firstDigit = (!value.empty() && (value[0] == '+' || value[0] == '-')) ? 1 : 0;
		return value.size() > firstDigit && value.find_first_not_of("0123456789", firstDigit) == std::string::npos;
	}


	template <typename T>
	IntegerValue formatInteger(const std::string& value, std::string& literal)
		/// Checks that value is an XSD integer literal whose value fits into T and writes the value
		/// to literal in canonical decimal form. Returns Minimum for the minimum of a signed T, which
		/// has no literal: -2147483648 negates a value that does not fit into int.
	{
		// NumberParser also accepts forms that are not XSD integers, such as "+-5"
		if (!isIntegerLexical(value))
			return IntegerValue::Invalid;
		constexpr char noThousandSeparator = 0;
		if constexpr (std::numeric_limits<T>::is_signed)
		{
			Poco::Int64 number = 0;
			if (!Poco::NumberParser::tryParse64(value, number, noThousandSeparator)
				|| number < std::numeric_limits<T>::min() || number > std::numeric_limits<T>::max())
				return IntegerValue::Invalid;
			if (number == std::numeric_limits<T>::min())
				return IntegerValue::Minimum;
			literal = Poco::NumberFormatter::format(number);
		}
		else
		{
			// nonNegativeInteger allows a minus sign before a lexical form that denotes zero
			const bool negative = (value[0] == '-');
			Poco::UInt64 number = 0;
			if (!Poco::NumberParser::tryParseUnsigned64(negative ? value.substr(1) : value, number, noThousandSeparator)
				|| number > std::numeric_limits<T>::max() || (negative && number != 0))
				return IntegerValue::Invalid;
			literal = Poco::NumberFormatter::format(number);
		}
		return IntegerValue::Literal;
	}


	[[noreturn]] void throwInvalidValue(const std::string& xsdName, const std::string& value)
	{
		throw Poco::DataFormatException("invalid default or fixed value for " + xsdName, value);
	}
}


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


BuiltinTypes::~BuiltinTypes() = default;


BuiltinTypes& BuiltinTypes::instance()
{
	static BuiltinTypes types;
	return types;
}


bool BuiltinTypes::tryGet(const std::string& xsdType, TypeInfo& result) const
{
	auto it = _types.find(xsdType);
	bool found = (it != _types.end());
	if (found)
		result = it->second;
	return found;
}


const TypeInfo& BuiltinTypes::get(const std::string& xsdType) const
{
	auto it = _types.find(xsdType);
	if (it == _types.end())
	{
		throw Poco::NotFoundException(xsdType);
	}

	return it->second;
}


void BuiltinTypes::add(const std::string& key, const TypeInfo& val, bool isString)
{
	_types.try_emplace(key, val);
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


std::string BuiltinTypes::generateInitializeValue(ClassInfo& ci, Constructor& constr, const Variable& var, const std::string& xsdName, const std::string& xsdString) const
{
	static const std::map<std::string, IntegerType> integerTypes = {
		{"int", {&formatInteger<int>, "int", ""}},
		{"Int8", {&formatInteger<Poco::Int8>, "Poco::Int8", ""}},
		{"Int16", {&formatInteger<Poco::Int16>, "Poco::Int16", ""}},
		{"Int32", {&formatInteger<Poco::Int32>, "Poco::Int32", ""}},
		{"Int64", {&formatInteger<Poco::Int64>, "Poco::Int64", "ll"}},
		{"UInt8", {&formatInteger<Poco::UInt8>, "Poco::UInt8", ""}},
		{"UInt16", {&formatInteger<Poco::UInt16>, "Poco::UInt16", ""}},
		{"UInt32", {&formatInteger<Poco::UInt32>, "Poco::UInt32", ""}},
		{"UInt64", {&formatInteger<Poco::UInt64>, "Poco::UInt64", "ull"}}
	};

	const TypeInfo& info = var.getType();
	std::string value = Poco::trim(xsdString);
	if (const auto itInteger = integerTypes.find(info.name()); itInteger != integerTypes.end())
	{
		const IntegerType& type = itInteger->second;
		std::string literal;
		const IntegerValue result = type.format(value, literal);
		if (result == IntegerValue::Invalid)
			throwInvalidValue(xsdName, xsdString);
		if (result == IntegerValue::Minimum)
		{
			ci.addSrcInclude("limits", true);
			return std::string("std::numeric_limits<") + type.cppType + ">::min()";
		}
		return literal + type.suffix;
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
		code += Utility::cppStringLiteral(xsdString) + ", " + intName + ");";
		constr.addCode(code);
		return "";
	}
	if (info.name() == "URI")
	{
		return "Poco::URI(" + Utility::cppStringLiteral(xsdString) + ")";
	}
	if (isStringType(info))
		return Utility::cppStringLiteral(xsdString);
	if (info.name() == "bool")
	{
		if (value == "true" || value == "1")
			return "true";
		if (value == "false" || value == "0")
			return "false";
		throwInvalidValue(xsdName, xsdString);
	}
	if (info.name() == "float" || info.name() == "double")
	{
		if (value == "INF" || value == "-INF" || value == "NaN")
		{
			ci.addSrcInclude("limits", true);
			const std::string limits("std::numeric_limits<" + info.name() + ">::");
			if (value == "NaN")
				return limits + "quiet_NaN()";
			return (value == "INF" ? "" : "-") + limits + "infinity()";
		}
		static const Poco::RegularExpression floatPattern("^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?$");
		if (!floatPattern.match(value))
			throwInvalidValue(xsdName, xsdString);
		return value;
	}
	throw Poco::NotImplementedException("default or fixed value for type " + info.name());
}
