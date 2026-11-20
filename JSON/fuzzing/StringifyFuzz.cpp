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
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::JSON;


namespace
{
	const std::size_t maxInput = 64 * 1024;


	struct OptionSet
	{
		unsigned indent;
		int step;
		int options;
	};


	// Fixed option sets: condensed and indented, with and without unicode
	// escape and lowercase hex.
	const OptionSet optionSets[] =
	{
		{ 0, -1, JSON_WRAP_STRINGS },
		{ 0, -1, JSON_WRAP_STRINGS | JSON_ESCAPE_UNICODE },
		{ 0, -1, JSON_WRAP_STRINGS | JSON_ESCAPE_UNICODE | JSON_LOWERCASE_HEX },
		{ 2, 2, JSON_WRAP_STRINGS },
		{ 2, 2, JSON_WRAP_STRINGS | JSON_ESCAPE_UNICODE },
		{ 1, 1, JSON_WRAP_STRINGS | JSON_LOWERCASE_HEX }
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
		catch (const JSONException&)
		{
			abort();
		}
		catch (const SyntaxException&)
		{
			abort();
		}

		const std::string twice = stringifyOnce(again, set);
		if (twice != once)
			abort();
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > maxInput) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	Var var;
	try
	{
		Parser parser;
		var = parser.parse(input);
	}
	catch (const JSONException&)
	{
		return 0;
	}
	catch (const SyntaxException&)
	{
		return 0;
	}

	for (const auto& set : optionSets)
		roundTrip(var, set);

	return 0;
}
