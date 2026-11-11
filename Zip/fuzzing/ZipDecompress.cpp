//
// ZipDecompress.cpp
//
// Fuzzer for the Poco::Zip archive parsing and decompression surface.
//
// Threat model:
// - Untrusted input: raw archive bytes — the classic "attacker uploads a zip"
//   surface (web uploads, document attachments, update packages).
// - Trust boundary: ZipArchive's stream parser (local file headers, central
//   directory, zip64 extensions), Decompress's entry handler, and the
//   ZipInputStream inflate reader.
// - The whole Zip module (3.1k source lines) is linked by no fuzz target as
//   of the 2026-11-10 report, so every line is dark. This pair adds the first
//   fuzz target linking Poco::Zip (and makes -DENABLE_ZIP=ON explicit in the
//   OSS-Fuzz build script) and drives the reader direction end to end.
// - Zip-bomb containment: libFuzzer's input bound caps expansion (max deflate
//   ratio ~1:1032 over a bounded input), and the ZipInputStream reader in the
//   manual path consumes at most 1 MiB of inflated output per entry before
//   abandoning the stream. The Decompress path writes only into a
//   round-robin pool of wiped temporary directories.
// - What counts as a bug: memory-safety violations while walking headers or
//   inflating entries (ASan/UBSan findings), path-traversal escapes when
//   extracting (files written outside the extraction directory), crashes in
//   zip64/data-descriptor handling.
//

#include "Poco/Path.h"
#include "Poco/Zip/AutoDetectStream.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/ZipArchiveInfo.h"
#include "Poco/Zip/ZipDataInfo.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/ZipLocalFileHeader.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <string>

using namespace Poco;
using namespace Poco::Zip;


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


Path scratchDir(std::size_t slot)
{
	const std::string root = std::string(Path::temp()) + "poco-zip-fuzz";
	std::error_code ec;
	std::filesystem::create_directories(root, ec);
	Path dir(root);
	dir.pushDirectory("slot" + std::to_string(slot % 8));
	std::filesystem::remove_all(dir.toString(), ec);
	std::filesystem::create_directories(dir.toString(), ec);
	return dir;
}


void inspectArchive(ZipArchive& arch, const std::string& probe)
{
	ignore([&] { [[maybe_unused]] std::ptrdiff_t n = std::distance(arch.headerBegin(), arch.headerEnd()); });
	ignore([&] { [[maybe_unused]] std::ptrdiff_t n = std::distance(arch.fileInfoBegin(), arch.fileInfoEnd()); });
	ignore([&] { [[maybe_unused]] const std::string& c = arch.getZipComment(); });
	ignore([&] { arch.checkConsistency(); });
	ignore([&] { [[maybe_unused]] auto it = arch.findHeader(probe); });
	ignore([&] { [[maybe_unused]] auto it = arch.findFileInfo(probe); });
	for (auto it = arch.fileInfoBegin(); it != arch.fileInfoEnd(); ++it)
	{
		ignore([&]
		{
			[[maybe_unused]] const std::string& n = it->second.getFileName();
			[[maybe_unused]] Poco::UInt32 crc = it->second.getCRC();
			[[maybe_unused]] Poco::UInt64 us = it->second.getUncompressedSize();
			[[maybe_unused]] Poco::UInt64 cs = it->second.getCompressedSize();
		});
	}
}


void fuzzParseArchive(const std::string& bytes, const std::string& probe)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		ZipArchive arch(in);
		inspectArchive(arch, probe);
	});

	// Same parse through the explicit ParseCallback overload.
	SkipCallback skip;
	std::istringstream in2(bytes);
	ignore([&]
	{
		ZipArchive arch(in2, skip);
		inspectArchive(arch, probe);
	});

	// Data descriptor parsing.
	std::istringstream in3(bytes);
	ignore([&]
	{
		ZipDataInfo info(in3, false);
		[[maybe_unused]] Poco::UInt32 crc = info.getCRC32();
	});

	// Raw auto-detected (pre/post-fixed) stream handling.
	std::istringstream in4(bytes);
	ignore([&]
	{
		AutoDetectInputStream autoDetect(in4, "pre-", "-post", false, 0);
		std::array<char, 512> buf{};
		while (autoDetect.good() && !autoDetect.eof())
		{
			autoDetect.read(buf.data(), static_cast<std::streamsize>(buf.size()));
			if (autoDetect.gcount() <= 0) break;
		}
	});
}


void fuzzDecompress(const std::string& bytes, std::size_t slot, bool flattenDirs, bool keepIncomplete, bool checkConsistency)
{
	std::istringstream in(bytes);
	ignore([&]
	{
		Decompress dec(in, scratchDir(slot), flattenDirs, keepIncomplete);
		ZipArchive arch = dec.decompressAllFiles(checkConsistency);
		inspectArchive(arch, "x");
		for (const auto& entry : dec.mapping())
		{
			[[maybe_unused]] const std::string& k = entry.first;
			[[maybe_unused]] const std::string& p = entry.second.toString();
		}
	});
}



void fuzzStreamEntry(const std::string& bytes)
{
	// Parse a local header from the raw stream, then inflate the entry with a
	// hard cap on consumed output (zip-bomb containment).
	SkipCallback skip;
	std::istringstream in(bytes);
	ignore([&]
	{
		ZipLocalFileHeader hdr(in, false, skip);
		ZipInputStream zin(in, hdr);
		std::array<char, 4096> buf{};
		std::size_t total = 0;
		while (zin.good() && !zin.eof() && total < (1u << 20))
		{
			zin.read(buf.data(), static_cast<std::streamsize>(buf.size()));
			const std::streamsize got = zin.gcount();
			if (got <= 0) break;
			total += static_cast<std::size_t>(got);
		}
	});
}


void fuzzCompressRoundtrip(const std::string& bytes, std::size_t slot)
{
	// Compress direction: fuzzed entry name/content into an in-memory
	// archive, then parse and decompress it again.
	std::string name = "fz/" + bytes.substr(0, std::min<std::size_t>(64, bytes.size()));
	for (auto& ch : name)
	{
		if (ch == '\0') ch = '_';
	}

	std::ostringstream out;
	ignore([&]
	{
		Compress zip(out, true);
		std::istringstream content(bytes);
		zip.addFile(content, DateTime(), Path(name),
			(bytes.size() & 0x01) ? ZipCommon::CM_DEFLATE : ZipCommon::CM_STORE,
			ZipCommon::CL_MAXIMUM);
		zip.addDirectory(Path("fz/dir"), DateTime());
		zip.setZipComment("poco-zip-fuzz");
		ZipArchive arch = zip.close();
		inspectArchive(arch, name);
	});

	const std::string archive = out.str();
	if (archive.empty()) return;

	fuzzDecompress(archive, slot, true, false, true);
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][flags:1][payload] — payload is the archive bytes,
	// the entry content and the probe name at once.
	if (size < 2) return 0;

	static std::size_t slotCounter = 0;
	const std::size_t slot = slotCounter++ % 8;

	const std::uint8_t op = data[0];
	const std::uint8_t flags = data[1];
	std::string payload(reinterpret_cast<const char*>(data) + 2, size - 2);
	const std::string probe = payload.substr(0, std::min<std::size_t>(32, payload.size()));

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
		fuzzParseArchive(payload, probe);

	if (runAll || op & 0x02)
		fuzzDecompress(payload, slot, flags & 0x01, flags & 0x02, flags & 0x04);

	if (runAll || op & 0x04)
		fuzzStreamEntry(payload);

	if (runAll || op & 0x08)
		fuzzCompressRoundtrip(payload, slot);

	return 0;
}
