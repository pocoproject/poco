//
// ToolRegistry.cpp
//
// Library: AI
// Package: Agent
// Module:  ToolRegistry
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/ToolRegistry.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Exception.h"
#include <exception>
#include <sstream>
#include <string>
#include <utility>


namespace Poco::AI {


namespace
{
	std::string json(const Poco::Dynamic::Var& value)
	{
		std::ostringstream os;
		Poco::JSON::Stringifier::stringify(value, os);
		return os.str();
	}


	void check(const Poco::AI::MCP::Tool& tool)
	{
		if (tool.name.empty())
		{
			throw Poco::InvalidArgumentException("A tool needs a name");
		}
		if (!tool.handler)
		{
			throw Poco::InvalidArgumentException("A tool needs a handler", tool.name);
		}
	}
}


std::string toolResultText(const Poco::AI::MCP::ToolResult& result)
{
	std::string text;
	bool hasText = false;
	const auto line = [&text](const std::string& part)
	{
		if (!text.empty()) text += '\n';
		text += part;
	};
	const Poco::JSON::Array::Ptr pContent = result.content();
	for (std::size_t i = 0; pContent != nullptr && i < pContent->size(); ++i)
	{
		const Poco::JSON::Object::Ptr pBlock = pContent->getObject(static_cast<unsigned>(i));
		if (pBlock == nullptr) continue;
		const std::string type = pBlock->optValue<std::string>("type", "");
		if (type == "text")
		{
			line(pBlock->optValue<std::string>("text", ""));
			hasText = true;
		}
		else if (type == "image" || type == "audio")
		{
			line("[" + type + " " + pBlock->optValue<std::string>("mimeType", "") + ", " + std::to_string(pBlock->optValue<std::string>("data", "").size()) + " bytes of base64]");
		}
		else if (type == "resource_link")
		{
			line("[resource " + pBlock->optValue<std::string>("uri", "") + "]");
		}
		else if (type == "resource" && pBlock->isObject("resource"))
		{
			const Poco::JSON::Object::Ptr pResource = pBlock->getObject("resource");
			if (pResource->has("text"))
			{
				line(pResource->optValue<std::string>("text", ""));
				hasText = true;
			}
			else
			{
				line("[resource " + pResource->optValue<std::string>("uri", "") + ", " + std::to_string(pResource->optValue<std::string>("blob", "").size()) + " bytes of base64]");
			}
		}
		else
		{
			line(json(pBlock));
		}
	}
	if (!hasText && result.hasStructuredContent()) return json(result.structuredContent());
	return text;
}


ToolRegistry::ToolRegistry()
{
}


ToolRegistry::~ToolRegistry()
{
}


void ToolRegistry::registerTool(const Poco::AI::MCP::Tool& tool)
{
	check(tool);
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools[tool.name] = tool;
}


bool ToolRegistry::registerToolIfAbsent(const Poco::AI::MCP::Tool& tool)
{
	check(tool);
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _tools.emplace(tool.name, tool).second;
}


void ToolRegistry::registerTool(const std::string& name, const std::string& description, const Poco::JSON::Object::Ptr& inputSchema, TextExecutor executor)
{
	if (!executor)
	{
		throw Poco::InvalidArgumentException("A tool needs an executor", name);
	}
	Poco::AI::MCP::Tool tool;
	tool.name = name;
	tool.description = description;
	tool.inputSchema = inputSchema;
	tool.handler = [executor = std::move(executor)](const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext&)
	{
		Poco::AI::MCP::ToolResult result;
		result.addText(executor(input));
		return result;
	};
	registerTool(tool);
}


void ToolRegistry::unregisterTool(const std::string& name)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools.erase(name);
}


std::vector<Poco::AI::MCP::Tool> ToolRegistry::tools() const
{
	std::vector<Poco::AI::MCP::Tool> result;
	Poco::FastMutex::ScopedLock lock(_mutex);
	result.reserve(_tools.size());
	for (const auto& entry: _tools)
	{
		result.push_back(entry.second);
	}
	return result;
}


Poco::AI::MCP::ToolResult ToolRegistry::executeTool(const std::string& name, const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext& context)
{
	Poco::AI::MCP::Tool tool;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		const auto it = _tools.find(name);
		if (it == _tools.end())
		{
			return Poco::AI::MCP::ToolResult::error("Unknown tool: " + name);
		}
		tool = it->second;
	}
	return execute(tool, input, context);
}


Poco::AI::MCP::ToolResult ToolRegistry::execute(const Poco::AI::MCP::Tool& tool, const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext& context)
{
	if (!tool.handler)
	{
		return Poco::AI::MCP::ToolResult::error("The tool has no handler: " + tool.name);
	}
	try
	{
		return tool.handler(input != nullptr ? input : Poco::JSON::Object::Ptr(new Poco::JSON::Object), context);
	}
	catch (const Poco::Exception& exc)
	{
		return Poco::AI::MCP::ToolResult::error("Tool execution failed: " + exc.displayText());
	}
	catch (const std::exception& exc)
	{
		return Poco::AI::MCP::ToolResult::error(std::string("Tool execution failed: ") + exc.what());
	}
}


bool ToolRegistry::hasTool(const std::string& name) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _tools.find(name) != _tools.end();
}


std::size_t ToolRegistry::size() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _tools.size();
}


} // namespace Poco::AI
