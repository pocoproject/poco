//
// AttributesFuzz.cpp
//
// Feeds arbitrary bytes as an attribute list to AttributesParser, which
// parses POCO-style C++ attributes of the form
// <name>[=<value>][,<name>[=<value>]...] and must reject malformed input
// with a SyntaxException.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Attributes.h"
#include "Poco/CppParser/AttributesParser.h"
#include "Poco/Exception.h"
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>


using Poco::CppParser::Attributes;
using Poco::CppParser::AttributesParser;


namespace
{

constexpr std::size_t MAX_INPUT_SIZE = 4096;


template <typename F>
void attempt(F f)
	/// Runs f and passes over the SyntaxException that AttributesParser
	/// throws for a malformed attribute list. Anything else is a finding.
{
	try
	{
		f();
	}
	catch (const Poco::SyntaxException&)
	{
	}
}

} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, std::size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	attempt([&input]
	{
		Attributes attrs;
		std::istringstream in(input);
		AttributesParser parser(attrs, in);
		parser.parse();
	});

	return 0;
}
