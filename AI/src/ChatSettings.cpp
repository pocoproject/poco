//
// ChatSettings.cpp
//
// Library: AI
// Package: Chat
// Module:  ChatSettings
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/ChatSettings.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"


namespace Poco::AI {


namespace
{
	std::string stringMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return std::string();
		if (!value.isString()) throw Poco::DataFormatException("Not a string", name);
		return value.extract<std::string>();
	}


	int intMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return 0;
		if (!value.isInteger() || value.isBoolean()) throw Poco::DataFormatException("Not an integer", name);
		try
		{
			return value.convert<int>();
		}
		catch (const Poco::RangeException&)
		{
			throw Poco::DataFormatException("Out of range", name);
		}
	}


	std::vector<std::string> stringsMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		std::vector<std::string> result;
		if (!pObject->has(name) || pObject->get(name).isEmpty()) return result;
		if (!pObject->isArray(name)) throw Poco::DataFormatException("Not an array", name);
		const Poco::JSON::Array::Ptr pArray = pObject->getArray(name);
		for (std::size_t i = 0; i < pArray->size(); ++i)
		{
			const Poco::Dynamic::Var element = pArray->get(static_cast<unsigned>(i));
			if (!element.isString()) throw Poco::DataFormatException("Not a string", name);
			result.push_back(element.extract<std::string>());
		}
		return result;
	}


	Poco::JSON::Array::Ptr toArray(const std::vector<std::string>& strings)
	{
		Poco::JSON::Array::Ptr pArray = new Poco::JSON::Array;
		for (const std::string& text: strings) pArray->add(text);
		return pArray;
	}
}


const std::vector<std::string>& ProviderSettings::kinds()
{
	static const std::vector<std::string> KINDS = {"anthropic", "openai", "ollama"};
	return KINDS;
}


std::string ProviderSettings::defaultBaseUrl(const std::string& kind)
{
	if (kind == "anthropic") return "https://api.anthropic.com/v1";
	if (kind == "openai") return "https://api.openai.com/v1";
	if (kind == "ollama") return "http://127.0.0.1:11434/v1";
	return std::string();
}


bool ProviderSettings::usesApiKey(const std::string& kind)
{
	return kind == "anthropic" || kind == "openai";
}


bool ProviderSettings::usesServers(const std::string& kind)
{
	return kind == "ollama";
}


std::string ProviderSettings::keyVariable(const std::string& kind)
{
	if (kind == "anthropic") return "ANTHROPIC_API_KEY";
	if (kind == "openai") return "OPENAI_API_KEY";
	return std::string();
}


Poco::JSON::Object::Ptr ChatSettings::toJSON() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("provider", provider);
	pObject->set("model", model);
	Poco::JSON::Array::Ptr pProviders = new Poco::JSON::Array;
	for (const ProviderSettings& settings: providers)
	{
		Poco::JSON::Object::Ptr pEntry = new Poco::JSON::Object;
		pEntry->set("name", settings.name);
		pEntry->set("enabled", settings.enabled);
		pEntry->set("baseUrl", settings.baseUrl);
		pEntry->set("servers", toArray(settings.servers));
		pEntry->set("models", toArray(settings.models));
		pEntry->set("maxTokens", settings.maxTokens);
		pEntry->set("timeout", settings.timeout);
		pEntry->set("probeTimeout", settings.probeTimeout);
		pEntry->set("tools", settings.tools);
		pEntry->set("apiKey", settings.apiKey);
		pProviders->add(pEntry);
	}
	pObject->set("providers", pProviders);
	return pObject;
}


ChatSettings ChatSettings::fromJSON(const Poco::JSON::Object::Ptr& pObject)
{
	ChatSettings settings;
	if (pObject.isNull()) return settings;
	settings.provider = stringMember(pObject, "provider");
	settings.model = stringMember(pObject, "model");
	if (!pObject->has("providers") || pObject->get("providers").isEmpty()) return settings;
	if (!pObject->isArray("providers")) throw Poco::DataFormatException("Not an array", "providers");
	const Poco::JSON::Array::Ptr pProviders = pObject->getArray("providers");
	for (std::size_t i = 0; i < pProviders->size(); ++i)
	{
		if (!pProviders->isObject(static_cast<unsigned>(i))) throw Poco::DataFormatException("Not an object", "providers");
		const Poco::JSON::Object::Ptr pEntry = pProviders->getObject(static_cast<unsigned>(i));
		ProviderSettings entry;
		entry.name = stringMember(pEntry, "name");
		const Poco::Dynamic::Var enabled = pEntry->get("enabled");
		if (!enabled.isEmpty() && !enabled.isBoolean()) throw Poco::DataFormatException("Not a boolean", "enabled");
		entry.enabled = !enabled.isEmpty() && enabled.extract<bool>();
		entry.baseUrl = stringMember(pEntry, "baseUrl");
		entry.servers = stringsMember(pEntry, "servers");
		entry.models = stringsMember(pEntry, "models");
		entry.maxTokens = intMember(pEntry, "maxTokens");
		entry.timeout = intMember(pEntry, "timeout");
		entry.probeTimeout = intMember(pEntry, "probeTimeout");
		const Poco::Dynamic::Var tools = pEntry->get("tools");
		if (!tools.isEmpty() && !tools.isBoolean()) throw Poco::DataFormatException("Not a boolean", "tools");
		entry.tools = tools.isEmpty() || tools.extract<bool>();
		entry.apiKey = stringMember(pEntry, "apiKey");
		settings.providers.push_back(std::move(entry));
	}
	return settings;
}


} // namespace Poco::AI
