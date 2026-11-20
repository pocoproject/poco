//
// UtilConfig.cpp
//
// Fuzzer for the Poco::Util configuration parsing surface.
//
// Threat model:
// - Untrusted input: configuration files in Poco properties, INI, JSON and
//   XML dialects - files an operator or a remote management plane feeds to
//   an application built on Poco::Util.
// - Trust boundary: the istream constructors of PropertyFileConfiguration,
//   IniFileConfiguration, JSONConfiguration and XMLConfiguration, plus the
//   AbstractConfiguration query API (key expansion, typed getters, views).
// - The whole Util module (7.5k source lines) is linked by no fuzz target as
//   of the 2026-11-10 report, so its parsers are dark. This pair adds the
//   first fuzz target linking Poco::Util (with -DENABLE_UTIL=ON explicit in
//   the OSS-Fuzz build script) and drives parse -> query -> save -> reparse
//   round-trips with attacker-controlled documents and keys.
// - What counts as a bug: memory-safety violations in key/value splitting,
//   escape decoding or variable substitution (ASan/UBSan findings), crashes
//   in typed getter conversions, crashes in the XML/JSON config mappers.
//

#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/XMLConfiguration.h"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace Poco::Util;


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


// Drives the AbstractConfiguration contract shared by all backends.
void queryConfig(AbstractConfiguration* cfg, const std::string& key, const std::string& prefix)
{
	ignore([&] { [[maybe_unused]] bool v = cfg->has(key); });
	ignore([&] { [[maybe_unused]] std::string v = cfg->getString(key, "dflt"); });
	ignore([&] { [[maybe_unused]] int v = cfg->getInt(key, 0); });
	ignore([&] { [[maybe_unused]] double v = cfg->getDouble(key, 0.0); });
	ignore([&] { [[maybe_unused]] bool v = cfg->getBool(key, false); });
	ignore([&] { [[maybe_unused]] Poco::Int64 v = cfg->getInt64(key, 0); });

	std::vector<std::string> ks;
	ignore([&] { cfg->keys(ks); });
	ignore([&] { cfg->keys(prefix, ks); });

	ignore([&]
	{
		AbstractConfiguration::Ptr view = cfg->createView(prefix);
		[[maybe_unused]] std::string v = view->getString(key, "view");
		std::vector<std::string> vks;
		view->keys(vks);
	});
}


void fuzzProperty(const std::string& bytes, const std::string& key, const std::string& prefix)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		Poco::AutoPtr<PropertyFileConfiguration> cfg = new PropertyFileConfiguration(in);
		queryConfig(cfg.get(), key, prefix);
		std::ostringstream out;
		cfg->save(out);
		// Round-trip: re-parse what we serialized.
		std::istringstream back(out.str());
		Poco::AutoPtr<PropertyFileConfiguration> again = new PropertyFileConfiguration(back);
		queryConfig(again.get(), key, prefix);
	});
}


void fuzzIni(const std::string& bytes, const std::string& key, const std::string& prefix)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		Poco::AutoPtr<IniFileConfiguration> cfg = new IniFileConfiguration(in);
		queryConfig(cfg.get(), key, prefix);
	});
}


void fuzzJson(const std::string& bytes, const std::string& key, const std::string& prefix)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		Poco::AutoPtr<JSONConfiguration> cfg = new JSONConfiguration(in);
		queryConfig(cfg.get(), key, prefix);
	});
}


void fuzzXml(const std::string& bytes, const std::string& key, const std::string& prefix)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		Poco::AutoPtr<XMLConfiguration> cfg = new XMLConfiguration(in);
		queryConfig(cfg.get(), key, prefix);
	});
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][keyLen:1][key:keyLen][payload]
	// key and prefix are raw attacker bytes; payload is the config document.
	if (size < 2) return 0;

	const std::uint8_t op = data[0];
	const std::size_t keyLen = std::min<std::size_t>(data[1], size - 2);
	const std::string key(reinterpret_cast<const char*>(data) + 2, keyLen);
	std::string payload(reinterpret_cast<const char*>(data) + 2 + keyLen, size - 2 - keyLen);
	const std::string prefix = payload.substr(0, std::min<std::size_t>(16, payload.size()));

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
		fuzzProperty(payload, key, prefix);

	if (runAll || op & 0x02)
		fuzzIni(payload, key, prefix);

	if (runAll || op & 0x04)
		fuzzJson(payload, key, prefix);

	if (runAll || op & 0x08)
		fuzzXml(payload, key, prefix);

	return 0;
}
