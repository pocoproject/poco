//
// StringifyFuzz.cpp
//
// Feeds a plain JSON document through parse, stringify with fixed option
// sets, re-parse and a second stringify; aborts on a broken round trip.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSONString.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>


using Poco::Dynamic::Var;
using Poco::JSON::Parser;
using Poco::JSON::Stringifier;


namespace
{
	constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;


	struct OptionSet
	{
		unsigned indent;
		int step;
		int options;
	};


	// Fixed option sets: condensed and indented, with and without unicode
	// escape and lowercase hex.
	constexpr OptionSet OPTION_SETS[] =
	{
		{ 0, -1, Poco::JSON_WRAP_STRINGS },
		{ 0, -1, Poco::JSON_WRAP_STRINGS | Poco::JSON_ESCAPE_UNICODE },
		{ 0, -1, Poco::JSON_WRAP_STRINGS | Poco::JSON_ESCAPE_UNICODE | Poco::JSON_LOWERCASE_HEX },
		{ 2, 2, Poco::JSON_WRAP_STRINGS },
		{ 2, 2, Poco::JSON_WRAP_STRINGS | Poco::JSON_ESCAPE_UNICODE },
		{ 1, 1, Poco::JSON_WRAP_STRINGS | Poco::JSON_LOWERCASE_HEX }
	};


	std::string stringifyOnce(const Var& var, const OptionSet& set)
	{
		std::ostringstream out;
		Stringifier::stringify(var, out, set.indent, set.step, set.options);
		return out.str();
	}


	void roundTrip(const Var& var, const OptionSet& set)
	{
		const std::string once = stringifyOnce(var, set);

		Var again;
		try
		{
			Parser parser;
			again = parser.parse(once);
		}
		catch (const Poco::JSON::JSONException&)
		{
			std::cerr << "JSON-stringify: stringify output failed to re-parse (JSONException)" << std::endl;
			std::abort();
		}
		catch (const Poco::SyntaxException&)
		{
			std::cerr << "JSON-stringify: stringify output failed to re-parse (SyntaxException)" << std::endl;
			std::abort();
		}

		const std::string twice = stringifyOnce(again, set);
		if (twice != once)
		{
			std::cerr << "JSON-stringify: second stringify differs from the first" << std::endl;
			std::abort();
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	Var var;
	try
	{
		Parser parser;
		var = parser.parse(input);
	}
	catch (const Poco::JSON::JSONException&)
	{
		return 0;
	}
	catch (const Poco::SyntaxException&)
	{
		return 0;
	}

	for (const auto& set : OPTION_SETS)
		roundTrip(var, set);

	return 0;
}
