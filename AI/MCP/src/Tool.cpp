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
