//
// ToolInput.h
//
// Library: AI
// Package: Providers
// Module:  ToolInput
//
// Parsing of the arguments of a tool call as a model returns them.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ToolInput_INCLUDED
#define AI_ToolInput_INCLUDED


#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/String.h"
#include <exception>
#include <string>


namespace Poco {
namespace AI {


inline bool parseToolInput(const std::string& json, Poco::JSON::Object& input)
	/// Parses the arguments of a tool call, which a model returns as JSON
	/// text. No text and JSON null both stand for no arguments. Returns
	/// false, with input left empty, when the text is anything but a JSON
	/// object: a tool must not run on arguments the model did not give, as
	/// it would if arguments cut short by the token limit became none.
{
	input.clear();
	const std::string text = Poco::trim(json);
	if (text.empty() || text == "null") return true;
	try
	{
		Poco::JSON::Parser parser;
		const Poco::Dynamic::Var parsed = parser.parse(text);
		if (parsed.type() != typeid(Poco::JSON::Object::Ptr)) return false;
		input = *parsed.extract<Poco::JSON::Object::Ptr>();
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}


} } // namespace Poco::AI


#endif // AI_ToolInput_INCLUDED
