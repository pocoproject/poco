//
// ToolResult.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  ToolResult
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/Exception.h"


namespace Poco::AI::MCP {


namespace
{
	bool isBlock(const Poco::JSON::Object::Ptr& pBlock)
	{
		return pBlock != nullptr && pBlock->get("type").isString();
	}
}


ToolResult::ToolResult():
	_pContent(new Poco::JSON::Array),
	_isError(false)
{
}


ToolResult ToolResult::error(const std::string& text)
{
	ToolResult result;
	result.addText(text).setError();
	return result;
}


ToolResult& ToolResult::addText(const std::string& text)
{
	Poco::JSON::Object::Ptr pBlock = new Poco::JSON::Object;
	pBlock->set("type", "text");
	pBlock->set("text", text);
	_pContent->add(pBlock);
	return *this;
}


ToolResult& ToolResult::addImage(const std::string& data, const std::string& mimeType)
{
	Poco::JSON::Object::Ptr pBlock = new Poco::JSON::Object;
	pBlock->set("type", "image");
	pBlock->set("data", data);
	pBlock->set("mimeType", mimeType);
	_pContent->add(pBlock);
	return *this;
}


ToolResult& ToolResult::addAudio(const std::string& data, const std::string& mimeType)
{
	Poco::JSON::Object::Ptr pBlock = new Poco::JSON::Object;
	pBlock->set("type", "audio");
	pBlock->set("data", data);
	pBlock->set("mimeType", mimeType);
	_pContent->add(pBlock);
	return *this;
}


ToolResult& ToolResult::addResourceLink(const std::string& uri, const std::string& name, const std::string& mimeType)
{
	Poco::JSON::Object::Ptr pBlock = new Poco::JSON::Object;
	pBlock->set("type", "resource_link");
	pBlock->set("uri", uri);
	pBlock->set("name", name);
	if (!mimeType.empty()) pBlock->set("mimeType", mimeType);
	_pContent->add(pBlock);
	return *this;
}


ToolResult& ToolResult::addResource(const ResourceContents& contents)
{
	Poco::JSON::Object::Ptr pBlock = new Poco::JSON::Object;
	pBlock->set("type", "resource");
	pBlock->set("resource", contents.toObject());
	_pContent->add(pBlock);
	return *this;
}


ToolResult& ToolResult::addContent(const Poco::JSON::Object::Ptr& block)
{
	if (!isBlock(block))
	{
		throw Poco::InvalidArgumentException("A content block needs a string type");
	}
	_pContent->add(block);
	return *this;
}


ToolResult& ToolResult::setError(bool isError)
{
	_isError = isError;
	return *this;
}


ToolResult& ToolResult::setStructuredContent(const Poco::Dynamic::Var& structured)
{
	_structuredContent = structured;
	return *this;
}


Poco::JSON::Object::Ptr ToolResult::toObject() const
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("content", _pContent);
	if (!_structuredContent.isEmpty())
	{
		pObject->set("structuredContent", _structuredContent);
	}
	pObject->set("isError", _isError);
	return pObject;
}


ToolResult ToolResult::fromObject(const Poco::JSON::Object::Ptr& object)
{
	if (object == nullptr)
	{
		throw Poco::DataFormatException("Tool result is not an object");
	}
	if (!object->isArray("content"))
	{
		throw Poco::DataFormatException("Tool result without a content array");
	}
	const Poco::JSON::Array::Ptr pContent = object->getArray("content");
	for (std::size_t i = 0; i < pContent->size(); ++i)
	{
		if (!isBlock(pContent->getObject(static_cast<unsigned>(i))))
		{
			throw Poco::DataFormatException("Tool result content block without a string type");
		}
	}
	const Poco::Dynamic::Var isError = object->get("isError");
	if (!isError.isEmpty() && !isError.isBoolean())
	{
		throw Poco::DataFormatException("Tool result with an isError that is not a boolean");
	}

	ToolResult result;
	result._pContent = pContent;
	result._structuredContent = object->get("structuredContent");
	result._isError = !isError.isEmpty() && isError.extract<bool>();
	return result;
}


} // namespace Poco::AI::MCP
