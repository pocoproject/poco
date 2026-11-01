//
// UtilityTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UtilityTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestCase.h"
#include "CppUnit/TestSuite.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/TemporaryFile.h"
#include <filesystem>
#include <set>
#include <string>
#include <vector>


using Poco::CppParser::Utility;
using Poco::File;
using Poco::Path;
using Poco::TemporaryFile;
using namespace std::string_literals;


namespace {
#if defined(_WIN32)
	constexpr bool CASELESS_SEARCH = true;
#else
	constexpr bool CASELESS_SEARCH = false;
#endif


	class FileListUtility: public Utility
		/// Gives the tests the file list that Utility::parseDir() parses.
	{
	public:
		FileListUtility() = delete;
		~FileListUtility() = delete;

		using Utility::buildFileList;
	};


	[[nodiscard]] std::string lines(const std::set<std::string>& paths)
	{
		return Poco::cat("\n"s, paths.begin(), paths.end());
	}


	// A search that ignores the letter case also lists the directories that it passes through by a name.
	[[nodiscard]] std::string directoryLines(std::set<std::string> listed, const std::set<std::string>& passedByName)
	{
		if constexpr (CASELESS_SEARCH) listed.insert(passedByName.begin(), passedByName.end());
		return lines(listed);
	}
}


UtilityTest::UtilityTest(const std::string& name): CppUnit::TestCase(name)
{
}


UtilityTest::~UtilityTest() = default;


void UtilityTest::testCollectDependenciesWildcardFileName()
{
	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/*.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/B.h"), inRoot("inc/Skip.h"), inRoot("inc/X.h")}), lines(files));
	assertEqual (directoryLines({inRoot("inc/")}, {inRoot("")}), lines(directories));
}


void UtilityTest::testCollectDependenciesWildcardDirectory()
{
	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/*/A.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/one/A.h")}), lines(files));
	assertEqual (directoryLines({inRoot("inc/"), inRoot("inc/two/")}, {inRoot(""), inRoot("inc/one/")}),
		lines(directories));
}


void UtilityTest::testCollectDependenciesLiteralPaths()
{
	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/B.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/B.h")}), lines(files));
	assertEqual (directoryLines({}, {inRoot(""), inRoot("inc/")}), lines(directories));

	directories.clear();
	files.clear();
	Utility::collectDependencies({"missing/C.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("")}), lines(directories));

	directories.clear();
	Utility::collectDependencies({"inc/missing/C.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (directoryLines({inRoot("inc/")}, {inRoot("")}), lines(directories));
}


void UtilityTest::testCollectDependenciesExcludePattern()
{
	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/*.h"s}, {"*Skip.h"s}, files, directories);
	assertEqual (lines({inRoot("inc/B.h"), inRoot("inc/X.h")}), lines(files));
	assertEqual (directoryLines({inRoot("inc/")}, {inRoot("")}), lines(directories));
}


void UtilityTest::testCollectDependenciesParentDirectory()
{
	std::filesystem::current_path(inRoot("work/"));

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"../inc/*.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/B.h"), inRoot("inc/Skip.h"), inRoot("inc/X.h")}), lines(files));
	assertEqual (directoryLines({inRoot("inc/")}, {inRoot("")}), lines(directories));

	directories.clear();
	files.clear();
	Utility::collectDependencies({"../missing/C.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("")}), lines(directories));
}


void UtilityTest::testCollectDependenciesAbsolutePattern()
{
	// The search passes by name through every directory from the root of the file system to the temporary directory.
	Path passed(_root);
	std::set<std::string> passedByName{passed.toString()};
	while (passed.depth() > 0)
	{
		passed.popDirectory();
		passedByName.insert(passed.toString());
	}

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({inRoot("inc/*.h")}, {}, files, directories);
	assertEqual (lines({inRoot("inc/B.h"), inRoot("inc/Skip.h"), inRoot("inc/X.h")}), lines(files));
	assertEqual (directoryLines({inRoot("inc/")}, passedByName), lines(directories));
}


void UtilityTest::testCollectDependenciesDanglingLink()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::filesystem::create_directory_symlink("../target"s, "inc/link"s);
	std::filesystem::create_symlink("work/missing/A"s, "chain"s);
	std::filesystem::create_directory_symlink("../chain"s, "inc/chained"s);

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/link/A.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot(""), inRoot("inc/")}), lines(directories));

	directories.clear();
	Utility::collectDependencies({"inc/chained/A.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot(""), inRoot("inc/"), inRoot("work/")}), lines(directories));

	directories.clear();
	Utility::collectDependencies({"inc/*/A.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/one/A.h")}), lines(files));
	assertEqual (lines({inRoot(""), inRoot("inc/"), inRoot("inc/two/"), inRoot("work/")}), lines(directories));

	std::filesystem::create_directory("real"s);
	std::filesystem::create_directory_symlink("real"s, "alias"s);
	std::filesystem::create_directory_symlink("../alias/missing"s, "inc/through"s);

	directories.clear();
	files.clear();
	Utility::collectDependencies({"inc/through/A.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("alias/"), inRoot("inc/")}), lines(directories));

	std::filesystem::create_directories("sub/real"s);
	std::filesystem::create_directory_symlink("sub/real"s, "nested"s);
	std::filesystem::create_directory_symlink("../nested/../missing/x"s, "inc/updown"s);

	directories.clear();
	Utility::collectDependencies({"inc/updown/A.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("inc/"), inRoot("sub/")}), lines(directories));

	std::filesystem::create_symlink("loopB"s, "loopA"s);
	std::filesystem::create_symlink("loopA"s, "loopB"s);
	std::filesystem::create_directory_symlink("../loopA"s, "inc/looped"s);

	directories.clear();
	Utility::collectDependencies({"inc/looped/A.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot(""), inRoot("inc/")}), lines(directories));
#endif
}


void UtilityTest::testCollectDependenciesLinkTargetNotSearchable()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::filesystem::create_directory("locked"s);
	std::filesystem::create_directory_symlink("../locked/x/y"s, "inc/link"s);

	// The link is followed as far as the target path can be searched.
	std::filesystem::permissions("locked"s, std::filesystem::perms::owner_read);
	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/link/A.h"s}, {}, files, directories);
	std::filesystem::permissions("locked"s, std::filesystem::perms::owner_all);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("inc/"), inRoot("locked/")}), lines(directories));
#endif
}


void UtilityTest::testCollectDependenciesCaseOnlyRename()
{
#if defined(POCO_OS_FAMILY_UNIX)
	// Only a file system that ignores letter case, as macOS does by default, shows this limit.
	if (!File("inc/b.h"s).exists()) return;

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"inc/B.h"s}, {}, files, directories);
	assertEqual (lines({inRoot("inc/B.h")}), lines(files));
	assertEqual (""s, lines(directories));

	// The listed file is still found under its old name, and no directory is listed, but the list changes.
	File("inc/B.h"s).renameTo("inc/b.h"s);
	assertTrue (File(inRoot("inc/B.h")).exists());
	files.clear();
	Utility::collectDependencies({"inc/B.h"s}, {}, files, directories);
	assertEqual (""s, lines(files));
	assertEqual (lines({inRoot("inc/")}), lines(directories));
#endif
}


void UtilityTest::testCollectDependenciesHomeAndDeviceNames()
{
#if defined(POCO_OS_FAMILY_UNIX)
	// Parsed alone, the relative paths "~/A.h" and "dev:/A.h" would name a file in the home directory and on a device.
	for (const auto& name: {"~"s, "dev:"s})
	{
		std::filesystem::create_directory(name);
		assertTrue (File(name + "/A.h"s).createFile());
	}

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies({"*/A.h"s}, {}, files, directories);
	assertEqual (lines({_root + "dev:/A.h"s, _root + "~/A.h"s}), lines(files));
	assertEqual (lines({inRoot(""), inRoot("inc/"), inRoot("work/")}), lines(directories));
#endif
}


void UtilityTest::testCollectDependenciesFilesAsParsed()
{
	const std::vector<std::string> includePattern{"inc/*.h"s, "inc/*/A.h"s, "inc/B.h"s, "missing/C.h"s};
	const std::vector<std::string> excludePattern{"*Skip.h"s};
	std::set<std::string> parsed;
	FileListUtility::buildFileList(parsed, includePattern, excludePattern);
	std::set<std::string> parsedAbsolute;
	for (const auto& file: parsed)
	{
		parsedAbsolute.insert(Path(file).absolute().toString());
	}

	std::set<std::string> files;
	std::set<std::string> directories;
	Utility::collectDependencies(includePattern, excludePattern, files, directories);
	assertEqual (lines(parsedAbsolute), lines(files));
	assertEqual (lines({inRoot("inc/B.h"), inRoot("inc/X.h"), inRoot("inc/one/A.h")}), lines(files));
	assertEqual (directoryLines({inRoot(""), inRoot("inc/"), inRoot("inc/two/")}, {inRoot("inc/one/")}),
		lines(directories));
}


void UtilityTest::setUp()
{
	_savedCwd = Path::current();
	_tempDir = TemporaryFile::tempName();
	Path tempDir(_tempDir);
	tempDir.makeDirectory();
	File(Path(tempDir, Path("work/", Path::PATH_UNIX))).createDirectories();
	for (const auto& file: {"inc/B.h"s, "inc/X.h"s, "inc/Skip.h"s, "inc/one/A.h"s, "inc/two/Other.h"s})
	{
		const Path path(tempDir, Path(file, Path::PATH_UNIX));
		File(path.parent()).createDirectories();
		assertTrue (File(path).createFile());
	}
	std::filesystem::current_path(_tempDir);
	_root = Path::current();
}


void UtilityTest::tearDown()
{
	std::filesystem::current_path(_savedCwd);
	File tempDir(_tempDir);
	if (tempDir.exists()) tempDir.remove(true);
}


std::string UtilityTest::inRoot(const std::string& relativePath) const
{
	return Path(Path(_root), Path(relativePath, Path::PATH_UNIX)).toString();
}


CppUnit::Test* UtilityTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UtilityTest");

	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesWildcardFileName);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesWildcardDirectory);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesLiteralPaths);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesExcludePattern);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesParentDirectory);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesAbsolutePattern);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesDanglingLink);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesLinkTargetNotSearchable);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesCaseOnlyRename);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesHomeAndDeviceNames);
	CppUnit_addTest(pSuite, UtilityTest, testCollectDependenciesFilesAsParsed);

	return pSuite;
}
