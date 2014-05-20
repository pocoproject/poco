//
// Stringifier.cpp
//
// $Id$
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
#include <iomanip>


using Poco::Dynamic::Var;


namespace Poco {
namespace JSON {


void Stringifier::stringify(const Var& any, std::ostream& out, unsigned int indent, int step, bool preserveInsertionOrder)
{
	if (step == -1) step = indent;

	if ( any.type() == typeid(Object) )
	{
		const Object& o = any.extract<Object>();
		o.stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if ( any.type() == typeid(Array) )
	{
		const Array& a = any.extract<Array>();
		a.stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if ( any.type() == typeid(Object::Ptr) )
	{
		const Object::Ptr& o = any.extract<Object::Ptr>();
		o->stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if ( any.type() == typeid(Array::Ptr) )
	{
		const Array::Ptr& a = any.extract<Array::Ptr>();
		a->stringify(out, indent == 0 ? 0 : indent, step);
	}
	else if ( any.isEmpty() )
	{
		out << "null";
	}
	else if ( any.isString() )
	{
		std::string value = any.convert<std::string>();
		formatString(value, out);
	}
	else
	{
		out << any.convert<std::string>();
	}
}


void Stringifier::formatString(const std::string& value, std::ostream& out)
{
	out << '"';
	for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		if (*it == 0x20 ||
			*it == 0x21 ||
			(*it >= 0x23 && *it <= 0x2E) ||
			(*it >= 0x30 && *it <= 0x5B) ||
			(*it >= 0x5D && *it <= 0xFF))
			out << *it;
		else if (*it == '"')
			out << "\\\"";
		else if (*it == '\\')
			out << "\\\\";
		else if (*it == '\b')
			out << "\\b";
		else if (*it == '\f')
			out << "\\f";
		else if (*it == '\n')
			out << "\\n";
		else if (*it == '\r')
			out << "\\r";
		else if (*it == '\t')
			out << "\\t";
		else if ( *it == '\0' )
			out << "\\u0000";
		else
		{
			const char *hexdigits = "0123456789ABCDEF";
			unsigned long u = (std::min)(static_cast<unsigned long>(static_cast<unsigned char>(*it)), 0xFFFFul);
			int d1 = u / 4096; u -= d1 * 4096;
			int d2 = u / 256; u -= d2 * 256;
			int d3 = u / 16; u -= d3 * 16;
			int d4 = u;
			out << "\\u";
			out << hexdigits[d1];
			out << hexdigits[d2];
			out << hexdigits[d3];
			out << hexdigits[d4];
		}
	}
	out << '"';
}

} }  // Namespace Poco::JSON
