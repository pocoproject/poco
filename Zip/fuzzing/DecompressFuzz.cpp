//
// DecompressFuzz.cpp
//
// Feeds a Zip archive to Decompress, extracting into a private directory.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DirectoryIterator.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipException.h"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Zip;


namespace
{
	constexpr std::size_t maxInput = 16u * 1024u;


	template <typename F>
	void attempt(F f)
		/// Runs f and passes over what Zip and Path throw for bad archives
		/// or over-long names. Anything else is a finding.
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
		catch (const PathSyntaxException&)
		{
		}
	}


	const std::string& processRoot()
		/// One private directory for this process, removed on exit.
	{
		static const std::string root = []()
		{
			const std::string path = TemporaryFile::tempName();
			TemporaryFile::registerForDeletion(path);
			File(path).createDirectories();
			return path;
		}();
		return root;
	}


	void assertOnlyChild(const std::string& dir, const std::string& expected)
		/// Aborts if dir contains anything other than a single child named expected.
	{
		DirectoryIterator it(dir);
		DirectoryIterator end;
		int count = 0;
		for (; it != end; ++it)
		{
			if (it.name() != expected) std::abort();
			++count;
		}
		if (count != 1) std::abort();
	}


	Path prepareExtractDir(const std::string& runDir)
		/// Builds <runDir>/a/b/out and returns out. Parents must stay clean.
	{
		Path out(runDir);
		out.makeDirectory();
		out.pushDirectory("a");
		out.pushDirectory("b");
		out.pushDirectory("out");
		File(out).createDirectories();
		return out;
	}


	void verifyParents(const std::string& runDir)
	{
		assertOnlyChild(runDir, "a");

		Path a(runDir);
		a.makeDirectory();
		a.pushDirectory("a");
		assertOnlyChild(a.toString(), "b");

		Path b(a);
		b.pushDirectory("b");
		assertOnlyChild(b.toString(), "out");
	}


	void extractOnce(const std::string& bytes, const Path& outDir, bool flattenDirs)
	{
		std::istringstream in(bytes);
		Decompress dec(in, outDir, flattenDirs, false);
		(void) dec.decompressAllFiles(true);
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > maxInput) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	static std::size_t runCounter = 0;
	Path runPath(processRoot());
	runPath.makeDirectory();
	runPath.pushDirectory("run" + std::to_string(runCounter++));
	const std::string runDir = runPath.toString();

	attempt([&]
	{
		for (const bool flattenDirs : {false, true})
		{
			File prior(runDir);
			if (prior.exists()) prior.remove(true);
			const Path outDir = prepareExtractDir(runDir);
			extractOnce(input, outDir, flattenDirs);
			verifyParents(runDir);
		}
	});

	try
	{
		File leftover(runDir);
		if (leftover.exists()) leftover.remove(true);
	}
	catch (const Exception&)
	{
	}

	return 0;
}
