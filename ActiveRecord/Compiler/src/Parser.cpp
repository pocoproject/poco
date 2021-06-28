//
// Parser.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Parser.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/SAX/Locator.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/Format.h"
#include "Poco/Ascii.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


Parser::Parser()
{
}


ClassMap Parser::parse(const std::string& systemId, std::istream& stream)
{
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, false);
	parser.setContentHandler(this);

	Poco::XML::InputSource inputSource(stream);
	inputSource.setSystemId(systemId);

	parser.parse(&inputSource);

	poco_assert (_elemStack.empty());

	return _classes;
}


void Parser::setDocumentLocator(const Poco::XML::Locator* pLocator)
{
	_pLocator = pLocator;
}


void Parser::startElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes)
{
	if (qname == "project")
	{
		if (_elemStack.empty())
		{
			_elemStack.push_back(qname);
			handleProject(attributes);
		}
		else throw Poco::SyntaxException(Poco::format("%s: project element must be at document root"s, where()));
	}
	else if (qname == "class")
	{
		if (_elemStack.size() == 1 && _elemStack.back() == "project")
		{
			_elemStack.push_back(qname);
			handleClass(attributes);
		}
		else throw Poco::SyntaxException(Poco::format("%s: class element must be within project element"s, where()));
	}
	else if (qname == "property")
	{
		if (_elemStack.size() == 2 && _elemStack.back() == "class")
		{
			_elemStack.push_back(qname);
			handleProperty(attributes);
		}
		else throw Poco::SyntaxException(Poco::format("%s: property element must be within class element"s, where()));
	}
	else throw Poco::SyntaxException(Poco::format("%s: invalid element: %s"s, where(), qname));
}


void Parser::endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname)
{
	poco_assert (_elemStack.size() > 0);

	if (qname == "class")
	{
		_classes[_class.name] = _class;
	}
	_elemStack.pop_back();
}


void Parser::handleProject(const Poco::XML::Attributes& attributes)
{
	_nameSpace = attributes.getValue("namespace"s);
	_convertCamelCase = parseBool("convertCamelCase"s, attributes.getValue("convertCamelCase"s));
}


void Parser::handleClass(const Poco::XML::Attributes& attributes)
{
	_class.name = attributes.getValue("name"s);
	_class.nameSpace = _nameSpace;
	_class.table = attributes.getValue("table"s);
	if (_class.table.empty()) _class.table = toDatabaseName(_class.name);
	_class.key = attributes.getValue("key"s);
	_class.autoIncrementID = parseBool("autoIncrementID"s, attributes.getValue("autoIncrementID"s), false);
	_class.properties.clear();
	_class.references.clear();
}


void Parser::handleProperty(const Poco::XML::Attributes& attributes)
{
	Property prop;
	prop.name = attributes.getValue("name"s);
	prop.column = attributes.getValue("column"s);
	if (prop.column.empty()) prop.column = toDatabaseName(prop.name);
	prop.type = parseType(attributes.getValue("type"s));
	prop.referencedClass = attributes.getValue("references"s);
	prop.foreignKey = attributes.getValue("foreignKey"s);
	prop.cardinality = parseCardinality(attributes.getValue("cardinality"));
	prop.nullable = parseBool("nullable"s, attributes.getValue("nullable"s), false);
	_class.properties.push_back(prop);

	if (!prop.referencedClass.empty())
	{
		if (prop.cardinality == Property::CARD_ZERO_OR_ONE)
		{
			prop.nullable = true;
		}
	}

	if (prop.referencedClass.empty() && !prop.foreignKey.empty())
	{
		throw Poco::InvalidArgumentException(Poco::format("%s: foreign key specified, but no referenced class"s, where()));
	}

	if (!prop.referencedClass.empty() && prop.referencedClass != _class.name && std::find(_class.references.begin(), _class.references.end(), prop.referencedClass) == _class.references.end())
	{
		_class.references.push_back(prop.referencedClass);
	}

	if (_class.key.empty() && prop.name == "id")
	{
		_class.key = prop.name;
	}
}


std::string Parser::where() const
{
	if (_pLocator)
	{
		if (_pLocator->getSystemId().empty())
			return Poco::format("Line %d, column %d"s,
				_pLocator->getLineNumber(),
				_pLocator->getColumnNumber());
		else
			return Poco::format("File '%s', line %d, column %d"s,
				_pLocator->getSystemId(),
				_pLocator->getLineNumber(),
				_pLocator->getColumnNumber());
	}
	else return "<unknown>";
}


std::string Parser::parseType(const std::string& type) const
{
	static const std::map<std::string, std::string> typeMap =
	{
		{"bool"s,      "bool"s},
		{"char"s,      "char"s},
		{"int8"s,      "Poco::Int8"s},
		{"uint8"s,     "Poco::UInt8"s},
		{"int16"s,     "Poco::Int16"s},
		{"uint16"s,    "Poco::UInt16"s},
		{"int32"s,     "Poco::Int32"s},
		{"uint32"s,    "Poco::UInt32"s},
		{"int64"s,     "Poco::Int64"s},
		{"uint64"s,    "Poco::UInt64"s},
		{"float"s,     "float"s},
		{"double"s,    "double"s},
		{"dateTime"s,  "Poco::DateTime"s},
		{"timestamp"s, "Poco::Timestamp"s},
		{"time"s,      "Poco::Data::Time"s},
		{"date"s,      "Poco::Data::Date"s},
		{"uuid"s,      "Poco::UUID"s},
		{"string"s,    "std::string"s}
	};

	auto it = typeMap.find(type);
	if (it == typeMap.end())
		throw Poco::InvalidArgumentException(Poco::format("%s: invalid type: %s", where(), type));
	else
		return it->second;
}


char Parser::parseCardinality(const std::string& cardinality) const
{
	if (cardinality.empty())
	{
		return Property::CARD_ONE;
	}
	else if (cardinality.size() == 1)
	{
		switch (cardinality[0])
		{
		case Property::CARD_ZERO_OR_ONE:
		case Property::CARD_ONE:
		case Property::CARD_ZERO_OR_MORE:
		case Property::CARD_ONE_OR_MORE:
			return cardinality[0];
		default:
			break;
		}
	}
	throw Poco::InvalidArgumentException(Poco::format("%s: cardinality must be one of ?, 1, *, +", where()));
}


bool Parser::parseBool(const std::string& name, const std::string& value, bool deflt) const
{
	if (value == "true")
		return true;
	else if (value == "false")
		return false;
	else if (value.empty())
		return deflt;
	else throw Poco::InvalidArgumentException(Poco::format("%s: %s value must be 'true' or 'false'", name, where()));
}


std::string Parser::convertCamelCase(const std::string& name)
{
	std::string result;
	for (const char c: name)
	{
		if (Poco::Ascii::isUpper(c))
		{
			if (!(result.empty() || result.back() == '_'))
			{
				result += '_';
			}
			result += Poco::Ascii::toLower(c);
		}
		else
		{
			result += c;
		}
	}
	return result;
}


std::string Parser::toDatabaseName(const std::string& name)
{
	if (_convertCamelCase)
		return convertCamelCase(name);
	else
		return name;
}


} } } // namespace Poco::ActiveRecord::Compiler
