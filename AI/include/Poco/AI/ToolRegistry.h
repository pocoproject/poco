//
// ToolRegistry.h
//
// Library: AI
// Package: Agent
// Module:  ToolRegistry
//
// A thread-safe registry of the tools a model may call.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ToolRegistry_INCLUDED
#define AI_ToolRegistry_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Mutex.h"
#include <string>
#include <map>
#include <functional>


namespace Poco {
namespace AI {


class AI_API ToolRegistry
	/// A registry of tool definitions with their executors. A definition is
	/// a JSON object with "name", "description" and "parameters" (a JSON
	/// Schema); the providers translate it to their API's tool format.
	/// Thread-safe: tools may be registered and executed concurrently.
{
public:
	using ToolExecutor = std::function<std::string(const Poco::JSON::Object&)>;
		/// Executes a tool with the input the model supplied and returns the
		/// result text passed back to the model.

	ToolRegistry() = default;
	~ToolRegistry() = default;

	void registerTool(const Poco::JSON::Object& definition, ToolExecutor executor);
		/// Registers a tool, replacing one of the same name. The definition
		/// must have "name", "description" and "parameters".

	void unregisterTool(const std::string& name);
		/// Unregisters a tool by name.

	Poco::JSON::Array getToolDefinitions() const;
		/// Returns all tool definitions as a JSON array.

	std::string executeTool(const std::string& name, const Poco::JSON::Object& input);
		/// Executes a tool by name and returns its result. An unknown tool and
		/// an executor that throws yield a JSON error string, never an
		/// exception.

	bool hasTool(const std::string& name) const;
		/// Returns true if a tool with the given name is registered.

private:
	struct ToolEntry
	{
		Poco::JSON::Object definition;
		ToolExecutor executor;
	};

	mutable Poco::FastMutex _mutex;
	std::map<std::string, ToolEntry> _tools;
};


} } // namespace Poco::AI


#endif // AI_ToolRegistry_INCLUDED
