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
#include <sstream>


namespace Poco {
namespace AI {


namespace
{
	std::string errorResult(const std::string& message)
		/// Returns the JSON text of a tool result that reports an error.
	{
		Poco::JSON::Object error;
		error.set("error", message);
		std::ostringstream stream;
		error.stringify(stream);
		return stream.str();
	}
}


void ToolRegistry::registerTool(const Poco::JSON::Object& definition, ToolExecutor executor)
{
	std::string name = definition.getValue<std::string>("name");
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools[name] = {definition, std::move(executor)};
}


void ToolRegistry::unregisterTool(const std::string& name)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools.erase(name);
}


Poco::JSON::Array ToolRegistry::getToolDefinitions() const
{
	Poco::JSON::Array result;
	Poco::FastMutex::ScopedLock lock(_mutex);
	for (const auto& [name, entry] : _tools)
	{
		result.add(Poco::JSON::Object::Ptr(new Poco::JSON::Object(entry.definition)));
	}
	return result;
}


std::string ToolRegistry::executeTool(const std::string& name, const Poco::JSON::Object& input)
{
	ToolExecutor executor;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		auto it = _tools.find(name);
		if (it == _tools.end())
		{
			return errorResult("Unknown tool: " + name);
		}
		executor = it->second.executor;
	}
	try
	{
		return executor(input);
	}
	catch (const Poco::Exception& e)
	{
		return errorResult("Tool execution failed: " + e.displayText());
	}
	catch (const std::exception& e)
	{
		return errorResult(std::string("Tool execution failed: ") + e.what());
	}
}


bool ToolRegistry::hasTool(const std::string& name) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _tools.find(name) != _tools.end();
}


} } // namespace Poco::AI
