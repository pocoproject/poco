//
// CppParserFuzz.cpp
//
// Fuzzer for the Poco::CppParser declaration-parsing surface.
//
// Threat model:
// - Untrusted input: C/C++ header text - what IDE tooling, documentation
//   generators and code-analysis services feed to Poco::CppParser from
//   untrusted repositories and uploaded sources.
// - Trust boundary: the Tokenizer + Parser pair (StreamTokenizer token
//   classification and the recursive declaration parser) and, separately,
//   the AttributesParser for Poco attribute syntax.
// - The whole CppParser module (4.5k source lines) is linked by no fuzz
//   target as of the 2026-11-10 report, so the parser and the symbol model
//   are dark. This pair adds the first fuzz target linking Poco::CppParser
//   (with -DENABLE_CPPPARSER=ON explicit) and drives parse -> symbol-table
//   walk with attacker-controlled declarations.
// - What counts as a bug: memory-safety violations in token classification,
//   unbalanced recursion (stack exhaustion is bounded by libFuzzer's input
//   cap), crashes in symbol insertion/lookup under malformed declarations,
//   and crashes in the attributes grammar.
//

#include "Poco/CppParser/Attributes.h"
#include "Poco/CppParser/AttributesParser.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/Tokenizer.h"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>

using namespace Poco::CppParser;


namespace
{

template <typename F>
void ignore(const F& func)
{
	try
	{
		func();
	}
	catch (const std::exception&)
	{
	}
}


std::string bounded(const std::string& s, std::size_t pos, std::size_t len)
{
	const std::size_t start = std::min(pos, s.size());
	return s.substr(start, std::min(len, s.size() - start));
}


void walkSymbols(const NameSpace& ns)
{
	NameSpace::SymbolTable table;

	ignore([&] { ns.nameSpaces(table); });
	ignore([&] { ns.typeDefs(table); });
	ignore([&] { ns.enums(table); });
	ignore([&] { ns.classes(table); });
	ignore([&] { ns.functions(table); });
	ignore([&] { ns.variables(table); });

	// Touch a bounded slice of the collected symbols.
	int visited = 0;
	for (const auto& entry : table)
	{
		Symbol* sym = entry.second;
		ignore([&]
		{
			[[maybe_unused]] const std::string& n = sym->name();
			[[maybe_unused]] Symbol::Kind k = sym->kind();
		});
		if (++visited >= 32) break;
	}
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][payload] - payload is C++ header text (parse
	// direction) or an attribute string (attributes grammar).
	if (size < 1) return 0;

	const std::uint8_t op = data[0];
	std::string payload(reinterpret_cast<const char*>(data) + 1, size - 1);

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
	{
		// Fresh global symbol table, parse, walk the root namespace.
		NameSpace::SymbolTable gst;
		std::istringstream in(payload);
		ignore([&]
		{
			Parser parser(gst, "fuzz.h", in);
			parser.parse();
		});
		ignore([&] { walkSymbols(*NameSpace::root()); });
	}

	if (runAll || op & 0x02)
	{
		// Second parse into the SAME table exercises symbol collision and
		// insertion paths that a fresh table never sees.
		NameSpace::SymbolTable gst;
		for (int pass = 0; pass < 2; ++pass)
		{
			std::istringstream in(payload);
			ignore([&]
			{
				Parser parser(gst, "fuzz.h", in);
				parser.parse();
			});
		}
		ignore([&] { walkSymbols(*NameSpace::root()); });
	}

	if (runAll || op & 0x04)
	{
		// Poco attribute grammar: <attr>/<attr>("args"), comma separated.
		Attributes attrs;
		std::istringstream in(payload);
		ignore([&]
		{
			AttributesParser parser(attrs, in);
			parser.parse();
		});
		int visited = 0;
		for (const auto& attr : attrs)
		{
			[[maybe_unused]] const std::string& n = attr.first;
			if (++visited >= 16) break;
		}
	}

	return 0;
}
