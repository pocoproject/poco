//
// Implementation.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Implementation
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Implementation.h"


namespace Poco::AI::MCP {


namespace
{
	std::string stringMember(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
	{
		const Poco::Dynamic::Var value = pObject->get(key);
		return value.isString() ? value.extract<std::string>() : std::string();
	}
}


Poco::JSON::Object::Ptr Implementation::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("name", name);
	pObject->set("version", version);
	if (!title.empty()) pObject->set("title", title);
	if (!description.empty()) pObject->set("description", description);
	if (!websiteUrl.empty()) pObject->set("websiteUrl", websiteUrl);
	return pObject;
}


Implementation Implementation::fromObject(const Poco::JSON::Object::Ptr& object)
{
	Implementation impl;
	if (object == nullptr) return impl;
	impl.name = stringMember(object, "name");
	impl.version = stringMember(object, "version");
	impl.title = stringMember(object, "title");
	impl.description = stringMember(object, "description");
	impl.websiteUrl = stringMember(object, "websiteUrl");
	return impl;
}


} // namespace Poco::AI::MCP
