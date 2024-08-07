//
// Stringifier.cpp
//
// Library: JSON
// Package: JSON
// Module:  Stringifier
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"


using Poco::Dynamic::Var;


namespace Poco {
namespace JSON {


void Stringifier::stringify(const Var& any, std::ostream& out, unsigned int indent, int step, int options)
{
	bool escapeUnicode = ((options & Poco::JSON_ESCAPE_UNICODE) != 0);
	bool lowercaseHex = ((options & Poco::JSON_LOWERCASE_HEX) != 0);

	if (step == -1) step = indent;

	if (any.type() == typeid(Object))
	{
		auto& o = const_cast<Object&>(any.extract<Object>());
		o.setEscapeUnicode(escapeUnicode);
		o.setLowercaseHex(lowercaseHex);
		o.stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if (any.type() == typeid(Array))
	{
		auto& a = const_cast<Array&>(any.extract<Array>());
		a.setEscapeUnicode(escapeUnicode);
		a.setLowercaseHex(lowercaseHex);
		a.stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if (any.type() == typeid(Object::Ptr))
	{
		auto& o = const_cast<Object::Ptr&>(any.extract<Object::Ptr>());
		o->setEscapeUnicode(escapeUnicode);
		o->setLowercaseHex(lowercaseHex);
		o->stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if (any.type() == typeid(Array::Ptr))
	{
		auto& a = const_cast<Array::Ptr&>(any.extract<Array::Ptr>());
		a->setEscapeUnicode(escapeUnicode);
		a->setLowercaseHex(lowercaseHex);
		a->stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if (any.isEmpty())
	{
		out << "null";
	}
	else if (any.isNumeric() || any.isBoolean())
	{
		auto value = any.convert<std::string>();
		if ((Poco::icompare(value, "nan") == 0) ||
			(Poco::icompare(value, "inf") == 0)) value = "null";
		if (any.type() == typeid(char)) formatString(value, out, options);
		else out << value;
	}
	else if (any.isString() || any.isDateTime() || any.isDate() || any.isTime())
	{
		auto value = any.convert<std::string>();
		formatString(value, out, options);
	}
	else
	{
		out << any.convert<std::string>();
	}
}


void Stringifier::formatString(const std::string& value, std::ostream& out, int options)
{
	Poco::toJSON(value, out, options);
}


} }  // namespace Poco::JSON
