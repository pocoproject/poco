//
// Tool.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Tool
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Tool.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"
#include "Poco/String.h"


namespace Poco::AI::MCP {


namespace
{
	Poco::Dynamic::Var copyValue(const Poco::Dynamic::Var& value);


	Poco::JSON::Object::Ptr copyObject(const Poco::JSON::Object& object)
		/// Copies an object in the order of its names: insertion order when
		/// the object keeps it.
	{
		Poco::JSON::Object::Ptr pCopy = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
		for (const std::string& name: object.getNames())
		{
			pCopy->set(name, copyValue(object.get(name)));
		}
		return pCopy;
	}


	Poco::JSON::Array::Ptr copyArray(const Poco::JSON::Array& array)
	{
		Poco::JSON::Array::Ptr pCopy = new Poco::JSON::Array;
		for (const auto& element: array)
		{
			pCopy->add(copyValue(element));
		}
		return pCopy;
	}


	Poco::Dynamic::Var copyValue(const Poco::Dynamic::Var& value)
		/// Copies a JSON value; objects and arrays recursively.
	{
		if (value.type() == typeid(Poco::JSON::Object::Ptr))
		{
			const Poco::JSON::Object::Ptr& pObject = value.extract<Poco::JSON::Object::Ptr>();
			return pObject.isNull() ? value : Poco::Dynamic::Var(copyObject(*pObject));
		}
		if (value.type() == typeid(Poco::JSON::Array::Ptr))
		{
			const Poco::JSON::Array::Ptr& pArray = value.extract<Poco::JSON::Array::Ptr>();
			return pArray.isNull() ? value : Poco::Dynamic::Var(copyArray(*pArray));
		}
		if (value.type() == typeid(Poco::JSON::Object))
		{
			return copyObject(value.extract<Poco::JSON::Object>());
		}
		if (value.type() == typeid(Poco::JSON::Array))
		{
			return copyArray(value.extract<Poco::JSON::Array>());
		}
		return value;
	}


	std::string optionalString(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
	{
		const Poco::Dynamic::Var value = pObject->get(key);
		return value.isString() ? value.extract<std::string>() : std::string();
	}


	bool hasAnnotation(const Poco::Dynamic::Var& value)
		/// Returns true if an x-mcp-header annotation sits anywhere in value.
	{
		if (value.type() == typeid(Poco::JSON::Object::Ptr))
		{
			const Poco::JSON::Object::Ptr& pObject = value.extract<Poco::JSON::Object::Ptr>();
			if (pObject.isNull()) return false;
			for (const std::string& key: pObject->getNames())
			{
				if (key == "x-mcp-header" || hasAnnotation(pObject->get(key))) return true;
			}
		}
		else if (value.type() == typeid(Poco::JSON::Array::Ptr))
		{
			const Poco::JSON::Array::Ptr& pArray = value.extract<Poco::JSON::Array::Ptr>();
			if (pArray.isNull()) return false;
			for (const auto& element: *pArray)
			{
				if (hasAnnotation(element)) return true;
			}
		}
		return false;
	}


	bool isToken(const std::string& name)
		/// Returns true if name is a token of HTTP: a header name.
	{
		static const std::string SPECIALS("!#$%&'*+-.^_`|~");
		if (name.empty()) return false;
		for (const char c: name)
		{
			const bool alnum = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
			if (!alnum && SPECIALS.find(c) == std::string::npos) return false;
		}
		return true;
	}


	void collectAnnotations(const Poco::JSON::Object::Ptr& pSchema, std::vector<std::string>& path, std::vector<Tool::HeaderAnnotation>& annotations)
		/// Collects the x-mcp-header annotations of a schema object reached
		/// from the root through properties keys only.
	{
		for (const std::string& key: pSchema->getNames())
		{
			const Poco::Dynamic::Var value = pSchema->get(key);
			if (key == "x-mcp-header")
			{
				if (path.empty())
				{
					throw Poco::DataFormatException("x-mcp-header at the root of the input schema");
				}
				if (!value.isString() || !isToken(value.extract<std::string>()))
				{
					throw Poco::DataFormatException("x-mcp-header that is not a header name");
				}
				const Poco::Dynamic::Var type = pSchema->get("type");
				const std::string typeName = type.isString() ? type.extract<std::string>() : std::string();
				if (typeName != "string" && typeName != "integer" && typeName != "boolean")
				{
					throw Poco::DataFormatException("x-mcp-header on a parameter that is not a string, an integer or a boolean");
				}
				const std::string& name = value.extract<std::string>();
				for (const Tool::HeaderAnnotation& annotation: annotations)
				{
					if (Poco::icompare(annotation.name, name) == 0)
					{
						throw Poco::DataFormatException("x-mcp-header name used twice", name);
					}
				}
				annotations.push_back(Tool::HeaderAnnotation{name, path, typeName});
			}
			else if (key == "properties" && pSchema->isObject(key))
			{
				const Poco::JSON::Object::Ptr pProperties = pSchema->getObject(key);
				for (const std::string& property: pProperties->getNames())
				{
					if (!pProperties->isObject(property)) continue;
					path.push_back(property);
					collectAnnotations(pProperties->getObject(property), path, annotations);
					path.pop_back();
				}
			}
			else if (hasAnnotation(value))
			{
				// Below items, the composition and conditional keywords and $ref,
				// a parameter is not statically reachable.
				throw Poco::DataFormatException("x-mcp-header on a parameter that is not reached through properties alone");
			}
		}
	}
}


Poco::JSON::Object::Ptr Tool::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("name", name);
	if (!title.empty()) pObject->set("title", title);
	if (!description.empty()) pObject->set("description", description);
	if (inputSchema != nullptr)
	{
		pObject->set("inputSchema", copyObject(*inputSchema));
	}
	else
	{
		Poco::JSON::Object::Ptr pSchema = new Poco::JSON::Object;
		pSchema->set("type", "object");
		pObject->set("inputSchema", pSchema);
	}
	if (outputSchema != nullptr) pObject->set("outputSchema", copyObject(*outputSchema));
	if (annotations != nullptr) pObject->set("annotations", copyObject(*annotations));
	return pObject;
}


std::vector<Tool::HeaderAnnotation> Tool::headerAnnotations() const
{
	std::vector<HeaderAnnotation> annotations;
	if (inputSchema.isNull()) return annotations;
	std::vector<std::string> path;
	collectAnnotations(inputSchema, path, annotations);
	return annotations;
}


Tool Tool::fromObject(const Poco::JSON::Object::Ptr& object)
{
	if (object == nullptr)
	{
		throw Poco::DataFormatException("Tool is not an object");
	}
	const Poco::Dynamic::Var name = object->get("name");
	if (!name.isString() || name.extract<std::string>().empty())
	{
		throw Poco::DataFormatException("Tool without a name");
	}
	if (!object->isObject("inputSchema"))
	{
		throw Poco::DataFormatException("Tool without an input schema object", name.extract<std::string>());
	}
	Tool tool;
	tool.name = name.extract<std::string>();
	tool.title = optionalString(object, "title");
	tool.description = optionalString(object, "description");
	tool.inputSchema = object->getObject("inputSchema");
	if (object->isObject("outputSchema")) tool.outputSchema = object->getObject("outputSchema");
	if (object->isObject("annotations")) tool.annotations = object->getObject("annotations");
	return tool;
}


} // namespace Poco::AI::MCP
