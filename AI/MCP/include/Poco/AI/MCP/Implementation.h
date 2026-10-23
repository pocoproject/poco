//
// Implementation.h
//
// Library: AI/MCP
// Package: Core
// Module:  Implementation
//
// Definition of the Implementation struct.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Implementation_INCLUDED
#define AIMCP_Implementation_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/JSON/Object.h"
#include <string>


namespace Poco::AI::MCP {


struct AIMCP_API Implementation
	/// The name and version of a client or a server, as it reports itself
	/// in clientInfo or serverInfo. Self-reported: a peer must not base
	/// security decisions on it.
{
	std::string name;
	std::string version;
	std::string title;
		/// Optional name for display.
	std::string description;
		/// Optional.
	std::string websiteUrl;
		/// Optional.

	[[nodiscard]] bool empty() const;
		/// Returns true if no name is set.

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Returns the JSON form; the optional members only when set.

	[[nodiscard]] static Implementation fromObject(const Poco::JSON::Object::Ptr& object);
		/// Reads the JSON form. Members that are missing or not strings stay
		/// empty; a null object gives an empty Implementation.

	[[nodiscard]] static Implementation library();
		/// Returns how this library reports itself: the name poco-ai-mcp and
		/// the version of POCO. Client and Server report it until they are
		/// given another one.
};


//
// inlines
//
inline bool Implementation::empty() const
{
	return name.empty();
}


} // namespace Poco::AI::MCP


#endif // AIMCP_Implementation_INCLUDED
