//
// EncodingsConvert.cpp
//
// Fuzzer for the Poco::Encodings registry and text conversion surface.
//
// Threat model:
// - Untrusted input: character-encoding names (Content-Type charset
//   parameters, email headers, file metadata, protocol fields) plus raw
//   bytes in an unknown legacy encoding that an application converts to
//   UTF-8 before further processing.
// - Trust boundary: the Poco::TextEncoding registry lookup
//   (TextEncoding::byName / find) backed by the ~50 DoubleByteEncoding and
//   single-byte tables of Poco::Encodings, and TextConverter's conversion
//   state machines (queryConvert/sequenceConvert table walks).
// - The whole Encodings module (27k source lines, mostly generated tables)
//   is linked by no fuzz target as of the 2026-11-10 report, so the table
//   walkers and the registry are dark. This pair adds the first fuzz target
//   linking Poco::Encodings (with -DENABLE_ENCODINGS=ON explicit) and
//   drives lookup -> convert -> round-trip with attacker-controlled names
//   and bytes.
// - What counts as a bug: memory-safety violations in multi-byte sequence
//   decoding (out-of-bounds reads while walking DBCS lead/trail tables),
//   crashes in the registry, hangs on pathological sequences.
// - Registration note: the extra encodings register themselves when
//   Poco::registerExtraEncodings() is called; the harness calls it once in
//   LLVMFuzzerInitialize, exactly as an application that links PocoEncodings
//   would.
//

#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include "Poco/Encodings.h"

#include <algorithm>
#include <cstdint>
#include <string>

using namespace Poco;


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


} // namespace


extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv)
{
	Poco::registerExtraEncodings();
	return 0;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][nameLen:1][name:nameLen][payload]
	// name is an attacker-supplied encoding name; payload is the byte text.
	if (size < 2) return 0;

	const std::uint8_t op = data[0];
	const std::size_t nameLen = std::min<std::size_t>(data[1], size - 2);
	const std::string name(reinterpret_cast<const char*>(data) + 2, nameLen);
	const std::string payload(reinterpret_cast<const char*>(data) + 2 + nameLen, size - 2 - nameLen);

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
	{
		// Registry lookup: find() returns null for unknown names; byName()
		// throws NotFoundException - both paths are attacker-reachable.
		TextEncoding::Ptr enc = TextEncoding::find(name);
		if (enc)
		{
			ignore([&] { [[maybe_unused]] const char* c = enc->canonicalName(); });
			ignore([&] { [[maybe_unused]] bool v = enc->isA(name); });
			ignore([&] { [[maybe_unused]] bool v = enc->isA("utf-8"); });

			// Forward: legacy encoding -> UTF-8.
			ignore([&]
			{
				TextEncoding& utf8 = TextEncoding::byName("UTF-8");
				TextConverter conv(*enc, utf8);
				std::string out;
				conv.convert(payload, out);
			});

			// Backward: UTF-8 -> legacy encoding (different table direction).
			ignore([&]
			{
				TextEncoding& utf8 = TextEncoding::byName("UTF-8");
				TextConverter back(utf8, *enc);
				std::string out;
				back.convert(payload, out);
			});

			// Round-trip with substitution default chars.
			ignore([&]
			{
				TextEncoding& utf8 = TextEncoding::byName("UTF-8");
				std::string out;
					TextConverter forward(*enc, utf8, '_');
				std::string mid;
				forward.convert(payload, mid);
				TextConverter backward(utf8, *enc, '_');
				backward.convert(mid, out);
				[[maybe_unused]] const std::string& v = out;
			});
		}
	}

	if (runAll || op & 0x02)
	{
		// Throw path of the registry for unknown names.
		ignore([&]
		{
			TextEncoding& enc = TextEncoding::byName(name);
			TextConverter conv(enc, TextEncoding::byName("UTF-8"));
			std::string out;
			conv.convert(payload, out);
		});
	}

	if (runAll || op & 0x04)
	{
		// Convert between two attacker-named encodings, both directions.
		const std::string name2 = payload.substr(0, std::min<std::size_t>(16, payload.size()));
		TextEncoding::Ptr a = TextEncoding::find(name);
		TextEncoding::Ptr b = TextEncoding::find(name2);
		if (a && b)
		{
			ignore([&]
			{
				TextConverter ab(*a, *b);
					std::string out2;
				std::string mid;
				ab.convert(payload, mid);
				TextConverter ba(*b, *a);
				ba.convert(mid, out2);
			});
		}
	}

	return 0;
}
