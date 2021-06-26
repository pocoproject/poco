//
// Enum.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Enum
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Enum.h"
#include "Poco/CppParser/EnumValue.h"
#include "Poco/NumberFormatter.h"
#include <sstream>


using Poco::NumberFormatter;


namespace Poco {
namespace CppParser {


int Enum::_count = 0;


Enum::Enum(const std::string& name, NameSpace* pNameSpace, int flags):
	Symbol(processName(name), pNameSpace),
	_flags(flags)
{
}


Enum::Enum(const std::string& name, NameSpace* pNameSpace, const std::string& baseType, int flags):
	Symbol(processName(name), pNameSpace),
	_baseType(baseType),
	_flags(flags)
{
}


Enum::~Enum()
{
}


void Enum::addValue(EnumValue* pValue)
{
	poco_check_ptr (pValue);

	_values.push_back(pValue);
}


Enum::Iterator Enum::begin() const
{
	return _values.begin();
}


Enum::Iterator Enum::end() const
{
	return _values.end();
}


std::string Enum::processName(const std::string& name)
{
	if (name.empty())
	{
		std::string result("#AnonEnum");
		result.append(NumberFormatter::format0(_count++, 4));
		return result;
	}
	else return name;
}


Symbol::Kind Enum::kind() const
{
	return Symbol::SYM_ENUM;
}


std::string Enum::toString() const
{
	std::ostringstream ostr;
	ostr << "enum ";
	if (_flags & ENUM_IS_CLASS) ostr << "class ";
	ostr << name();
	if (!_baseType.empty()) ostr << ": " << _baseType;
	ostr << "\n{\n";
	for (Iterator it = begin(); it != end(); ++it)
	{
		ostr << "\t" << (*it)->toString() << "\n";
	}
	ostr << "};";
	return ostr.str();
}


} } // namespace Poco::CppParser
