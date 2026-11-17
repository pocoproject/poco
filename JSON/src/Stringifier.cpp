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


namespace Poco::JSON {


void Stringifier::stringify(const Var& any, std::ostream& out, unsigned int indent, int step, int options)
{
	if (step == -1) step = static_cast<int>(indent);

	if (any.type() == typeid(Object))
	{
		any.extract<Object>().stringify(out, indent, step, options);
	}
	else if (any.type() == typeid(Array))
	{
		any.extract<Array>().stringify(out, indent, step, options);
	}
	else if (any.type() == typeid(Object::Ptr))
	{
		any.extract<Object::Ptr>()->stringify(out, indent, step, options);
	}
	else if (any.type() == typeid(Array::Ptr))
	{
		any.extract<Array::Ptr>()->stringify(out, indent, step, options);
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


} // namespace Poco::JSON
