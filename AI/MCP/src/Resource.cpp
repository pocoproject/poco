//
// Resource.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Resource
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Resource.h"
#include "Poco/Exception.h"


namespace Poco::AI::MCP {


namespace
{
	std::string optionalString(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
	{
		const Poco::Dynamic::Var value = pObject->get(key);
		return value.isString() ? value.extract<std::string>() : std::string();
	}


	std::string requiredString(const Poco::JSON::Object::Ptr& pObject, const std::string& key, const std::string& what)
	{
		const Poco::Dynamic::Var value = pObject->get(key);
		if (!value.isString())
		{
			throw Poco::DataFormatException(what + " without a string " + key);
		}
		return value.extract<std::string>();
	}


	void checkObject(const Poco::JSON::Object::Ptr& pObject, const std::string& what)
	{
		if (pObject == nullptr)
		{
			throw Poco::DataFormatException(what + " is not an object");
		}
	}
}


Poco::JSON::Object::Ptr Resource::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("uri", uri);
	pObject->set("name", name);
	if (!title.empty()) pObject->set("title", title);
	if (!description.empty()) pObject->set("description", description);
	if (!mimeType.empty()) pObject->set("mimeType", mimeType);
	return pObject;
}


Resource Resource::fromObject(const Poco::JSON::Object::Ptr& object)
{
	checkObject(object, "Resource");
	Resource resource;
	resource.uri = requiredString(object, "uri", "Resource");
	resource.name = requiredString(object, "name", "Resource");
	resource.title = optionalString(object, "title");
	resource.description = optionalString(object, "description");
	resource.mimeType = optionalString(object, "mimeType");
	return resource;
}


Poco::JSON::Object::Ptr ResourceContents::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("uri", uri);
	if (!mimeType.empty()) pObject->set("mimeType", mimeType);
	if (binary || !blob.empty())
	{
		pObject->set("blob", blob);
	}
	else
	{
		pObject->set("text", text);
	}
	return pObject;
}


ResourceContents ResourceContents::fromObject(const Poco::JSON::Object::Ptr& object)
{
	checkObject(object, "Resource contents");
	ResourceContents contents;
	contents.uri = requiredString(object, "uri", "Resource contents");
	contents.mimeType = optionalString(object, "mimeType");
	const Poco::Dynamic::Var text = object->get("text");
	const Poco::Dynamic::Var blob = object->get("blob");
	if (text.isString())
	{
		contents.text = text.extract<std::string>();
	}
	else if (blob.isString())
	{
		contents.blob = blob.extract<std::string>();
		contents.binary = true;
	}
	else
	{
		throw Poco::DataFormatException("Resource contents without text or blob");
	}
	return contents;
}


Poco::JSON::Object::Ptr Prompt::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("name", name);
	if (!title.empty()) pObject->set("title", title);
	if (!description.empty()) pObject->set("description", description);
	if (!arguments.empty())
	{
		Poco::JSON::Array::Ptr pArguments = new Poco::JSON::Array;
		for (const PromptArgument& argument: arguments)
		{
			Poco::JSON::Object::Ptr pArgument = new Poco::JSON::Object;
			pArgument->set("name", argument.name);
			if (!argument.title.empty()) pArgument->set("title", argument.title);
			if (!argument.description.empty()) pArgument->set("description", argument.description);
			pArgument->set("required", argument.required);
			pArguments->add(pArgument);
		}
		pObject->set("arguments", pArguments);
	}
	return pObject;
}


Prompt Prompt::fromObject(const Poco::JSON::Object::Ptr& object)
{
	checkObject(object, "Prompt");
	Prompt prompt;
	prompt.name = requiredString(object, "name", "Prompt");
	prompt.title = optionalString(object, "title");
	prompt.description = optionalString(object, "description");
	if (object->isArray("arguments"))
	{
		const Poco::JSON::Array::Ptr pArguments = object->getArray("arguments");
		for (std::size_t i = 0; i < pArguments->size(); ++i)
		{
			const Poco::JSON::Object::Ptr pArgument = pArguments->getObject(static_cast<unsigned>(i));
			checkObject(pArgument, "Prompt argument");
			PromptArgument argument;
			argument.name = requiredString(pArgument, "name", "Prompt argument");
			argument.title = optionalString(pArgument, "title");
			argument.description = optionalString(pArgument, "description");
			const Poco::Dynamic::Var required = pArgument->get("required");
			argument.required = required.isBoolean() && required.extract<bool>();
			prompt.arguments.push_back(argument);
		}
	}
	return prompt;
}


} // namespace Poco::AI::MCP
