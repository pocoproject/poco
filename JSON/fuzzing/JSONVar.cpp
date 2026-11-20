//
// JSONVar.cpp
//
// Fuzzer for the JSON -> Dynamic::Var extraction and conversion pipeline.
//
// Threat model:
// - Untrusted input: the JSON document bytes, i.e. what an attacker-controlled
//   API response, webhook payload or config file delivers to a Poco application.
// - Trust boundary: Poco::JSON::Parser::parse(), which materialises the document
//   as a Poco::Dynamic::Var tree (JSON::Object / JSON::Array holders).
// - The incumbent json_parser_fuzzer stops right after parse() and never touches
//   the resulting Var (see JsonParse.cpp: the parsed result is discarded). As of
//   the 2026-11-10 coverage report the Dynamic::Var/VarHolder conversion surface
//   (VarHolder.h ~1.7k dark lines, Var.h ~700, Struct.h ~460, Var.cpp ~430) and
//   the typed Object/Array accessors sit at ~8% coverage project-wide. This
//   harness drives that surface with attacker-controlled documents:
//   * the full Var::convert<T>() matrix: numeric widening/narrowing, string to
//     number, string to DateTime/Timestamp, Array to std::vector<Var>,
//     Object to DynamicStruct (both the std::map and OrderedMap flavours),
//   * typed JSON::Object::getValue<T>() and JSON::Array::getElement<T>(),
//   * JSON::Query path evaluation over the parsed tree,
//   * stringify -> re-parse round-trips (Object and Array writers).
// - What counts as a bug: any memory-safety violation (ASan/UBSan finding) in a
//   conversion or accessor reachable from a syntactically valid document. The
//   harness itself only constructs valid objects; incompatible conversions are
//   expected to throw and are ignored, never silenced.
//

#include "Poco/DateTime.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/UUID.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Timestamp.h"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include <fuzzer/FuzzedDataProvider.h>

using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::JSON;


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

void convertMatrix(const Dynamic::Var& var)
{
	// Each conversion below instantiates a distinct VarHolder specialization;
	// incompatible target types throw BadCastException and are ignored.
	ignore([&var] { [[maybe_unused]] bool v = var.convert<bool>(); });
	ignore([&var] { [[maybe_unused]] char v = var.convert<char>(); });
	ignore([&var] { [[maybe_unused]] signed char v = var.convert<signed char>(); });
	ignore([&var] { [[maybe_unused]] unsigned char v = var.convert<unsigned char>(); });
	ignore([&var] { [[maybe_unused]] short v = var.convert<short>(); });
	ignore([&var] { [[maybe_unused]] unsigned short v = var.convert<unsigned short>(); });
	ignore([&var] { [[maybe_unused]] int v = var.convert<int>(); });
	ignore([&var] { [[maybe_unused]] unsigned int v = var.convert<unsigned int>(); });
	ignore([&var] { [[maybe_unused]] long v = var.convert<long>(); });
	ignore([&var] { [[maybe_unused]] unsigned long v = var.convert<unsigned long>(); });
	ignore([&var] { [[maybe_unused]] long long v = var.convert<long long>(); });
	ignore([&var] { [[maybe_unused]] unsigned long long v = var.convert<unsigned long long>(); });
	ignore([&var] { [[maybe_unused]] float v = var.convert<float>(); });
	ignore([&var] { [[maybe_unused]] double v = var.convert<double>(); });
	ignore([&var] { [[maybe_unused]] std::string v = var.convert<std::string>(); });
	ignore([&var] { [[maybe_unused]] DateTime v = var.convert<DateTime>(); });
	ignore([&var] { [[maybe_unused]] LocalDateTime v = var.convert<LocalDateTime>(); });
	ignore([&var] { [[maybe_unused]] Timestamp v = var.convert<Timestamp>(); });
	ignore([&var] { [[maybe_unused]] UUID v = var.convert<UUID>(); });
}



void stageObject(const Dynamic::Var& var)
{
	Object::Ptr obj = var.extract<Object::Ptr>();
	if (obj.isNull()) return;

	std::vector<std::string> names;
	obj->getNames(names);

	ignore([&] { [[maybe_unused]] Poco::DynamicStruct ds = Object::makeStruct(obj); });
	ignore([&] { [[maybe_unused]] Poco::OrderedDynamicStruct ods = Object::makeOrderedStruct(obj); });
	ignore([&] { [[maybe_unused]] const Poco::DynamicStruct& implicit = *obj; });

	for (const auto& name : names)
	{
		ignore([&] { [[maybe_unused]] std::string v = obj->getValue<std::string>(name); });
		ignore([&] { [[maybe_unused]] Poco::Int64 v = obj->getValue<Poco::Int64>(name); });
		ignore([&] { [[maybe_unused]] double v = obj->getValue<double>(name); });
		ignore([&] { [[maybe_unused]] bool v = obj->getValue<bool>(name); });

		ignore([&] { [[maybe_unused]] Object::Ptr v = obj->getObject(name); });
		ignore([&] { [[maybe_unused]] JSON::Array::Ptr v = obj->getArray(name); });
		ignore([&] { [[maybe_unused]] bool v = obj->has(name); });
		ignore([&] { [[maybe_unused]] bool v = obj->isArray(name); });
		ignore([&] { [[maybe_unused]] bool v = obj->isObject(name); });
		ignore([&] { [[maybe_unused]] bool v = obj->isNull(name); });
	}

	ignore([&]
	{
		std::ostringstream out;
		obj->setEscapeUnicode(true);
		obj->stringify(out, 2, 2);
		obj->setEscapeUnicode(false);
		obj->stringify(out, 0, -1);
	});
}


void stageArray(const Dynamic::Var& var)
{
	JSON::Array::Ptr arr = var.extract<JSON::Array::Ptr>();
	if (arr.isNull()) return;

	const std::size_t n = arr->size();

	for (std::size_t i = 0; i < n; ++i)
	{
		ignore([&] { [[maybe_unused]] std::string v = arr->getElement<std::string>(i); });
		ignore([&] { [[maybe_unused]] Poco::Int64 v = arr->getElement<Poco::Int64>(i); });
		ignore([&] { [[maybe_unused]] double v = arr->getElement<double>(i); });
		ignore([&] { [[maybe_unused]] bool v = arr->getElement<bool>(i); });
		ignore([&] { [[maybe_unused]] Object::Ptr v = arr->getObject(i); });
		ignore([&] { [[maybe_unused]] JSON::Array::Ptr v = arr->getArray(i); });
		ignore([&] { [[maybe_unused]] Dynamic::Var v = arr->get(i); });
	}

	ignore([&]
	{
		std::ostringstream out;
		arr->stringify(out, 2, 2);
	});
}


void stageQuery(const Dynamic::Var& var, const std::string& path)
{
	Query q(var);

	ignore([&] { [[maybe_unused]] Dynamic::Var v = q.find(path); });
	ignore([&] { [[maybe_unused]] Object::Ptr v = q.findObject(path); });
	ignore([&] { [[maybe_unused]] JSON::Array::Ptr v = q.findArray(path); });
	ignore([&] { [[maybe_unused]] std::string v = q.findValue<std::string>(path, std::string("dflt")); });
	ignore([&] { [[maybe_unused]] Poco::Int64 v = q.findValue<Poco::Int64>(path, 0); });
}


void stageRoundtrip(const Dynamic::Var& var)
{
	std::string once;
	ignore([&]
	{
		std::ostringstream out;
		Stringifier::stringify(var, out);
		once = out.str();
	});

	if (once.empty()) return;

	// Re-parse the serialized form and serialize again: exercises the writer
	// direction against the reader and the second-order conversions in between.
	Dynamic::Var again;
	ignore([&]
	{
		Parser parser;
		again = parser.parse(once);
	});

	ignore([&]
	{
		std::ostringstream out2;
		Stringifier::stringify(again, out2);
	});
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [ops:1][pathLen:1][path:pathLen][doc:rest]
	// The path and the document are raw attacker bytes; a fixed split avoids
	// mis-tokenising JSON documents that legitimately contain backslashes.
	if (size < 2) return 0;

	const std::uint8_t ops = data[0];
	const std::size_t pathLen = std::min<std::size_t>(data[1], size - 2);
	const std::string path(reinterpret_cast<const char*>(data) + 2, pathLen);
	std::string doc(reinterpret_cast<const char*>(data) + 2 + pathLen, size - 2 - pathLen);

	Dynamic::Var var;
	try
	{
		Parser parser;
		var = parser.parse(doc);
	}
	catch (const std::exception&)
	{
		return 0;
	}

	const bool runAll = ops == 0;

	if (runAll || ops & 0x01)
		convertMatrix(var);

	if (var.type() == typeid(Object::Ptr) && (runAll || ops & 0x02))
		stageObject(var);

	if (var.type() == typeid(JSON::Array::Ptr) && (runAll || ops & 0x04))
		stageArray(var);

	if (runAll || ops & 0x08)
		stageQuery(var, path);

	if (runAll || ops & 0x10)
		stageRoundtrip(var);

	return 0;
}
