//
// CodeGenerator.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CodeGenerator.h"
#include "Poco/StringTokenizer.h"
#include <set>


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


CodeGenerator::CodeGenerator(const std::string& source, std::ostream& stream):
	_source(source),
	_stream(stream)
{
}


void CodeGenerator::writeBeginNameSpace(const std::string& nameSpace) const
{
	if (!nameSpace.empty())
	{
		auto ns = splitNameSpace(nameSpace);
		for (const auto& s: ns)
		{
			_stream << "namespace " << s << " {\n";
		}
	}
}


void CodeGenerator::writeEndNameSpace(const std::string& nameSpace) const
{
	if (!nameSpace.empty())
	{
		auto ns = splitNameSpace(nameSpace);
		for (const auto& s: ns)
		{
			(void) s; // avoid unused variable warning
			_stream << "} ";
		}
		_stream << "// namespace " << nameSpace << "\n";
	}
}


void CodeGenerator::writeHeaderComment(const std::string& fileName) const
{
	_stream
		<< "//\n"
		<< "// " << fileName << "\n"
		<< "//\n"
		<< "// This file has been generated from " << _source << ". Do not edit.\n"
		<< "//\n\n\n";
}


void CodeGenerator::writeInclude(const std::string& nameSpace, const std::string& name) const
{
	_stream << "#include \"";
	auto ns = splitNameSpace(nameSpace);
	for (const auto& s: ns)
	{
		_stream << s << '/';
	}
	_stream << name << ".h\"\n";
}


std::vector<std::string> CodeGenerator::splitNameSpace(const std::string& nameSpace)
{
	std::vector<std::string> result;
	Poco::StringTokenizer tok(nameSpace, ":"s, Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	result.insert(result.end(), tok.begin(), tok.end());
	return result;
}


std::string CodeGenerator::propertyType(const Property& prop) const
{
	std::string type;
	if (prop.nullable) type += "Poco::Nullable<";
	type += prop.type;
	if (prop.nullable) type += ">";
	return type;
}


std::string CodeGenerator::paramType(const Property& prop) const
{
	std::string type;
	if (!prop.nullable && isSimpleType(prop.type))
	{
		type = propertyType(prop);
	}
	else
	{
		type += "const ";
		type += propertyType(prop);
		type += "&";
	}
	return type;
}


std::string CodeGenerator::keyType(const Class& clazz) const
{
	for (const auto& p: clazz.properties)
	{
		if (p.name == clazz.key)
		{
			return propertyType(p);
		}
	}
	return ""s;
}


bool CodeGenerator::isSimpleType(const std::string& type)
{
	static const std::set<std::string> simpleTypes =
	{
		"bool"s,
		"char"s,
		"Poco::UInt8"s,
		"Poco::Int8"s,
		"Poco::UInt16"s,
		"Poco::Int16"s,
		"Poco::UInt32"s,
		"Poco::Int32"s,
		"Poco::UInt64"s,
		"Poco::Int64"s,
		"float"s,
		"double"s
	};

	return simpleTypes.find(type) != simpleTypes.end();
}


std::string CodeGenerator::fullClassName(const Class& clazz) const
{
	std::string fullName;
	auto ns = splitNameSpace(clazz.nameSpace);
	for (const auto& n: ns)
	{
		fullName += n;
		fullName += "::";
	}
	fullName += clazz.name;
	return fullName;
}


} } } // namespace Poco::ActiveRecord::Compiler
