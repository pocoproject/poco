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
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Zip;


namespace
{
	constexpr std::size_t maxInput = 64u * 1024u;
	constexpr std::size_t maxPerEntry = 1u << 20;
	constexpr std::size_t maxTotal = 8u << 20;


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
		catch (const IllegalStateException&)
		{
		}
	}


	std::size_t inflateCapped(std::istream& zipStream, const ZipLocalFileHeader& hdr, bool reposition, std::size_t& totalOut)
		/// Reads at most maxPerEntry of inflated output from one entry, and at
		/// most maxTotal across the whole input. Returns bytes read from this entry.
	{
		if (totalOut >= maxTotal) return 0;

		ZipInputStream zin(zipStream, hdr, reposition);
		char buf[4096];
		std::size_t entryOut = 0;
		while (zin.good() && !zin.eof() && entryOut < maxPerEntry && totalOut < maxTotal)
		{
			const std::size_t roomEntry = maxPerEntry - entryOut;
			const std::size_t roomTotal = maxTotal - totalOut;
			const std::size_t room = roomEntry < roomTotal ? roomEntry : roomTotal;
			const std::size_t chunk = room < sizeof(buf) ? room : sizeof(buf);
			zin.read(buf, static_cast<std::streamsize>(chunk));
			const std::streamsize got = zin.gcount();
			if (got <= 0) break;
			entryOut += static_cast<std::size_t>(got);
			totalOut += static_cast<std::size_t>(got);
		}
		return entryOut;
	}


	class CountingCallback: public ParseCallback
		/// Checks each entry path and inflates into a counting sink.
	{
	public:
		explicit CountingCallback(std::size_t& totalOut):
			_totalOut(totalOut)
		{
		}

		bool handleZipEntry(std::istream& zipStream, const ZipLocalFileHeader& hdr) override
		{
			(void) ZipCommon::isValidPath(hdr.getFileName());
			if (_totalOut >= maxTotal)
			{
				SkipCallback skip;
				return skip.handleZipEntry(zipStream, hdr);
			}
			inflateCapped(zipStream, hdr, false, _totalOut);
			return true;
		}

	private:
		std::size_t& _totalOut;
	};
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > maxInput) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);
	std::size_t totalOut = 0;

	attempt([&]
	{
		std::istringstream in(input);
		CountingCallback cb(totalOut);
		ZipArchive arch(in, cb);
		arch.checkConsistency();

		std::istringstream again(input);
		for (auto it = arch.headerBegin(); it != arch.headerEnd(); ++it)
		{
			if (totalOut >= maxTotal) break;
			inflateCapped(again, it->second, true, totalOut);
		}
	});

	return 0;
}
