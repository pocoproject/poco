//
// Content.cpp
//
// Library: AIMCP
// Package: Core
// Module:  Content
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Content.h"


namespace Poco {
namespace AI {
namespace MCP {


ToolResult::ToolResult():
	_content(new Poco::JSON::Array),
	_isError(false)
{
}


ToolResult& ToolResult::addText(const std::string& text)
{
	Poco::JSON::Object::Ptr block = new Poco::JSON::Object;
	block->set("type", "text");
	block->set("text", text);
	_content->add(block);
	return *this;
}


ToolResult& ToolResult::setError(bool isError)
{
	_isError = isError;
	return *this;
}


ToolResult& ToolResult::setStructuredContent(const Poco::JSON::Object::Ptr& structured)
{
	_structuredContent = structured;
	return *this;
}


Poco::JSON::Object::Ptr ToolResult::toObject() const
{
	Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
	o->set("content", _content);
	if (_structuredContent)
	{
		o->set("structuredContent", _structuredContent);
	}
	o->set("isError", _isError);
	return o;
}


ToolResult ToolResult::fromObject(const Poco::JSON::Object::Ptr& object)
{
	ToolResult result;
	if (!object)
	{
		return result;
	}
	if (object->isArray("content"))
	{
		result._content = object->getArray("content");
	}
	if (object->isObject("structuredContent"))
	{
		result._structuredContent = object->getObject("structuredContent");
	}
	result._isError = object->optValue<bool>("isError", false);
	return result;
}


} } } // namespace Poco::AI::MCP
