//
// ArchiveFuzz.cpp
//
// Feeds a Zip archive to ZipArchive and ZipInputStream in memory.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Exception.h"
#include "Poco/Zip/ParseCallback.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/ZipCommon.h"
#include "Poco/Zip/ZipException.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipStream.h"

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Zip;


namespace
{

	constexpr std::size_t MAX_INPUT = 64u * 1024u;
	constexpr std::size_t MAX_PER_ENTRY = 1u << 20;
	constexpr std::size_t MAX_TOTAL = 8u << 20;
	constexpr std::size_t COPY_BUFFER_SIZE = 4096;


	template <typename F>
	void attempt(F f)
		/// Runs f and passes over what Zip throws for bad archives.
		/// Anything else is a finding.
	{
		try
		{
			f();
		}
		catch (const ZipException&)
		{
		}
		catch (const IOException&)
		{
		}
		catch (const DataFormatException&)
		{
		}
	}


	std::size_t inflateCapped(std::istream& zipStream, const ZipLocalFileHeader& hdr, bool reposition, std::size_t& totalOut)
		/// Reads at most MAX_PER_ENTRY of inflated output from one entry, and at
		/// most MAX_TOTAL across the whole input. Returns bytes read from this entry.
	{
		if (totalOut >= MAX_TOTAL) return 0;

		ZipInputStream zin(zipStream, hdr, reposition);
		// A read error throws instead of failing silently, so its type is
		// checked by the per-entry attempt like every other finding.
		zin.exceptions(std::ios::badbit);
		char buf[COPY_BUFFER_SIZE];
		std::size_t entryOut = 0;
		while (zin.good() && !zin.eof() && entryOut < MAX_PER_ENTRY && totalOut < MAX_TOTAL)
		{
			const std::size_t roomEntry = MAX_PER_ENTRY - entryOut;
			const std::size_t roomTotal = MAX_TOTAL - totalOut;
			const std::size_t room = roomEntry < roomTotal ? roomEntry : roomTotal;
			const std::size_t chunk = room < COPY_BUFFER_SIZE ? room : COPY_BUFFER_SIZE;
			zin.read(buf, static_cast<std::streamsize>(chunk));
			const std::streamsize got = zin.gcount();
			if (got <= 0) break;
			entryOut += static_cast<std::size_t>(got);
			totalOut += static_cast<std::size_t>(got);
		}
		return entryOut;
	}


	class CountingCallback: public ParseCallback
		/// Checks each entry path and inflates into a counting sink. Returns
		/// false when the output cap ended the read, so the archive stops
		/// processing entries.
	{
	public:
		explicit CountingCallback(std::size_t& totalOut):
			_totalOut(totalOut)
		{
		}

		bool handleZipEntry(std::istream& zipStream, const ZipLocalFileHeader& hdr) override
		{
			if (!ZipCommon::isValidPath(hdr.getFileName()))
				return false;
			if (_totalOut >= MAX_TOTAL)
			{
				SkipCallback skip;
				return skip.handleZipEntry(zipStream, hdr);
			}
			inflateCapped(zipStream, hdr, false, _totalOut);
			if (_totalOut >= MAX_TOTAL)
				return false;
			return true;
		}

	private:
		std::size_t& _totalOut;
	};
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);
	std::size_t totalOut = 0;

	std::istringstream in(input);
	CountingCallback cb(totalOut);
	std::unique_ptr<ZipArchive> arch;
	// A parse failure is an expected finding; the archive object survives
	// for the consistency check and the second pass whenever it exists.
	attempt([&]
	{
		arch = std::make_unique<ZipArchive>(in, cb);
	});
	if (!arch) return 0;

	// checkConsistency in its own attempt: when it throws, the second pass
	// over the headers still runs.
	attempt([&]
	{
		arch->checkConsistency();
	});

	std::istringstream again(input);
	for (auto it = arch->headerBegin(); it != arch->headerEnd(); ++it)
	{
		if (totalOut >= MAX_TOTAL) break;
		// One attempt per entry, so the type of an exception thrown while
		// reading is checked like every other finding.
		attempt([&] { inflateCapped(again, it->second, true, totalOut); });
	}

	return 0;
}
