//
// GlobTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GlobTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <fstream>


using Poco::Glob;
using Poco::File;
using Poco::Path;


GlobTest::GlobTest(const std::string& name): CppUnit::TestCase(name)
{
}


GlobTest::~GlobTest()
{
}


void GlobTest::testMatchChars()
{
	Glob g1("a");
	assertTrue (g1.match("a"));
	assertTrue (!g1.match("b"));
	assertTrue (!g1.match("aa"));
	assertTrue (!g1.match(""));

	Glob g2("ab");
	assertTrue (g2.match("ab"));
	assertTrue (!g2.match("aab"));
	assertTrue (!g2.match("abab"));
}


void GlobTest::testMatchQM()
{
	Glob g1("?");
	assertTrue (g1.match("a"));
	assertTrue (g1.match("b"));
	assertTrue (!g1.match("aa"));
	assertTrue (g1.match("."));

	Glob g2("\\?");
	assertTrue (g2.match("?"));
	assertTrue (!g2.match("a"));
	assertTrue (!g2.match("ab"));

	Glob g3("a?");
	assertTrue (g3.match("aa"));
	assertTrue (g3.match("az"));
	assertTrue (!g3.match("a"));
	assertTrue (!g3.match("aaa"));

	Glob g4("??");
	assertTrue (g4.match("aa"));
	assertTrue (g4.match("ab"));
	assertTrue (!g4.match("a"));
	assertTrue (!g4.match("abc"));

	Glob g5("?a?");
	assertTrue (g5.match("aaa"));
	assertTrue (g5.match("bac"));
	assertTrue (!g5.match("bbc"));
	assertTrue (!g5.match("ba"));
	assertTrue (!g5.match("ab"));

	Glob g6("a\\?");
	assertTrue (g6.match("a?"));
	assertTrue (!g6.match("az"));
	assertTrue (!g6.match("a"));

	Glob g7("?", Glob::GLOB_DOT_SPECIAL);
	assertTrue (g7.match("a"));
	assertTrue (g7.match("b"));
	assertTrue (!g7.match("aa"));
	assertTrue (!g7.match("."));
}


void GlobTest::testMatchAsterisk()
{
	Glob g1("*");
	assertTrue (g1.match(""));
	assertTrue (g1.match("a"));
	assertTrue (g1.match("ab"));
	assertTrue (g1.match("abc"));
	assertTrue (g1.match("."));

	Glob g2("a*");
	assertTrue (g2.match("a"));
	assertTrue (g2.match("aa"));
	assertTrue (g2.match("abc"));
	assertTrue (!g2.match("b"));
	assertTrue (!g2.match("ba"));

	Glob g3("ab*");
	assertTrue (g3.match("ab"));
	assertTrue (g3.match("abc"));
	assertTrue (g3.match("abab"));
	assertTrue (!g3.match("ac"));
	assertTrue (!g3.match("baab"));

	Glob g4("*a");
	assertTrue (g4.match("a"));
	assertTrue (g4.match("ba"));
	assertTrue (g4.match("aa"));
	assertTrue (g4.match("aaaaaa"));
	assertTrue (g4.match("bbbbba"));
	assertTrue (!g4.match("b"));
	assertTrue (!g4.match("ab"));
	assertTrue (!g4.match("aaab"));

	Glob g5("a*a");
	assertTrue (g5.match("aa"));
	assertTrue (g5.match("aba"));
	assertTrue (g5.match("abba"));
	assertTrue (!g5.match("aab"));
	assertTrue (!g5.match("aaab"));
	assertTrue (!g5.match("baaaa"));

	Glob g6("a*b*c");
	assertTrue (g6.match("abc"));
	assertTrue (g6.match("aabbcc"));
	assertTrue (g6.match("abcbbc"));
	assertTrue (g6.match("aaaabbbbcccc"));
	assertTrue (!g6.match("aaaabbbcb"));

	Glob g7("a*b*");
	assertTrue (g7.match("aaabbb"));
	assertTrue (g7.match("abababab"));
	assertTrue (g7.match("ab"));
	assertTrue (g7.match("aaaaab"));
	assertTrue (!g7.match("a"));
	assertTrue (!g7.match("aa"));
	assertTrue (!g7.match("aaa"));

	Glob g8("**");
	assertTrue (g1.match(""));
	assertTrue (g1.match("a"));
	assertTrue (g1.match("ab"));
	assertTrue (g1.match("abc"));

	Glob g9("a\\*");
	assertTrue (g9.match("a*"));
	assertTrue (!g9.match("aa"));
	assertTrue (!g9.match("a"));

	Glob g10("a*\\*");
	assertTrue (g10.match("a*"));
	assertTrue (g10.match("aaa*"));
	assertTrue (!g10.match("a"));
	assertTrue (!g10.match("aa"));

	Glob g11("*", Glob::GLOB_DOT_SPECIAL);
	assertTrue (g11.match(""));
	assertTrue (g11.match("a"));
	assertTrue (g11.match("ab"));
	assertTrue (g11.match("abc"));
	assertTrue (!g11.match("."));
}


void GlobTest::testMatchRange()
{
	Glob g1("[a]");
	assertTrue (g1.match("a"));
	assertTrue (!g1.match("b"));
	assertTrue (!g1.match("aa"));

	Glob g2("[ab]");
	assertTrue (g2.match("a"));
	assertTrue (g2.match("b"));
	assertTrue (!g2.match("c"));
	assertTrue (!g2.match("ab"));

	Glob g3("[abc]");
	assertTrue (g3.match("a"));
	assertTrue (g3.match("b"));
	assertTrue (g3.match("c"));
	assertTrue (!g3.match("ab"));

	Glob g4("[a-z]");
	assertTrue (g4.match("a"));
	assertTrue (g4.match("z"));
	assertTrue (!g4.match("A"));

	Glob g5("[!a]");
	assertTrue (g5.match("b"));
	assertTrue (g5.match("c"));
	assertTrue (!g5.match("a"));
	assertTrue (!g5.match("bb"));

	Glob g6("[!a-z]");
	assertTrue (g6.match("A"));
	assertTrue (!g6.match("a"));
	assertTrue (!g6.match("z"));

	Glob g7("[0-9a-zA-Z_]");
	assertTrue (g7.match("0"));
	assertTrue (g7.match("1"));
	assertTrue (g7.match("8"));
	assertTrue (g7.match("9"));
	assertTrue (g7.match("a"));
	assertTrue (g7.match("b"));
	assertTrue (g7.match("z"));
	assertTrue (g7.match("A"));
	assertTrue (g7.match("Z"));
	assertTrue (g7.match("_"));
	assertTrue (!g7.match("-"));

	Glob g8("[1-3]");
	assertTrue (g8.match("1"));
	assertTrue (g8.match("2"));
	assertTrue (g8.match("3"));
	assertTrue (!g8.match("0"));
	assertTrue (!g8.match("4"));

	Glob g9("[!1-3]");
	assertTrue (g9.match("0"));
	assertTrue (g9.match("4"));
	assertTrue (!g9.match("1"));
	assertTrue (!g9.match("2"));
	assertTrue (!g9.match("3"));

	Glob g10("[\\!a]");
	assertTrue (g10.match("!"));
	assertTrue (g10.match("a"));
	assertTrue (!g10.match("x"));

	Glob g11("[a\\-c]");
	assertTrue (g11.match("a"));
	assertTrue (g11.match("c"));
	assertTrue (g11.match("-"));
	assertTrue (!g11.match("b"));

	Glob g12("[\\]]");
	assertTrue (g12.match("]"));
	assertTrue (!g12.match("["));

	Glob g13("[[\\]]");
	assertTrue (g13.match("["));
	assertTrue (g13.match("]"));
	assertTrue (!g13.match("x"));

	Glob g14("\\[]");
	assertTrue (g14.match("[]"));
	assertTrue (!g14.match("[["));

	Glob g15("a[bc]");
	assertTrue (g15.match("ab"));
	assertTrue (g15.match("ac"));
	assertTrue (!g15.match("a"));
	assertTrue (!g15.match("aa"));

	Glob g16("[ab]c");
	assertTrue (g16.match("ac"));
	assertTrue (g16.match("bc"));
	assertTrue (!g16.match("a"));
	assertTrue (!g16.match("b"));
	assertTrue (!g16.match("c"));
	assertTrue (!g16.match("aa"));
}


void GlobTest::testMisc()
{
	Glob g1("*.cpp");
	assertTrue (g1.match("Glob.cpp"));
	assertTrue (!g1.match("Glob.h"));

	Glob g2("*.[hc]");
	assertTrue (g2.match("foo.c"));
	assertTrue (g2.match("foo.h"));
	assertTrue (!g2.match("foo.i"));

	Glob g3("*.*");
	assertTrue (g3.match("foo.cpp"));
	assertTrue (g3.match("foo.h"));
	assertTrue (g3.match("foo."));
	assertTrue (!g3.match("foo"));

	Glob g4("File*.?pp");
	assertTrue (g4.match("File.hpp"));
	assertTrue (g4.match("File.cpp"));
	assertTrue (g4.match("Filesystem.hpp"));
	assertTrue (!g4.match("File.h"));

	Glob g5("File*.[ch]*");
	assertTrue (g5.match("File.hpp"));
	assertTrue (g5.match("File.cpp"));
	assertTrue (g5.match("Filesystem.hpp"));
	assertTrue (g5.match("File.h"));
	assertTrue (g5.match("Filesystem.cp"));
}


void GlobTest::testCaseless()
{
	Glob g1("*.cpp", Glob::GLOB_CASELESS);
	assertTrue (g1.match("Glob.cpp"));
	assertTrue (!g1.match("Glob.h"));
	assertTrue (g1.match("Glob.CPP"));
	assertTrue (!g1.match("Glob.H"));

	Glob g2("*.[hc]", Glob::GLOB_CASELESS);
	assertTrue (g2.match("foo.c"));
	assertTrue (g2.match("foo.h"));
	assertTrue (!g2.match("foo.i"));
	assertTrue (g2.match("foo.C"));
	assertTrue (g2.match("foo.H"));
	assertTrue (!g2.match("foo.I"));

	Glob g4("File*.?pp", Glob::GLOB_CASELESS);
	assertTrue (g4.match("file.hpp"));
	assertTrue (g4.match("FILE.CPP"));
	assertTrue (g4.match("filesystem.hpp"));
	assertTrue (g4.match("FILESYSTEM.HPP"));
	assertTrue (!g4.match("FILE.H"));
	assertTrue (!g4.match("file.h"));

	Glob g5("File*.[ch]*", Glob::GLOB_CASELESS);
	assertTrue (g5.match("file.hpp"));
	assertTrue (g5.match("FILE.HPP"));
	assertTrue (g5.match("file.cpp"));
	assertTrue (g5.match("FILE.CPP"));
	assertTrue (g5.match("filesystem.hpp"));
	assertTrue (g5.match("FILESYSTEM.HPP"));
	assertTrue (g5.match("file.h"));
	assertTrue (g5.match("FILE.H"));
	assertTrue (g5.match("filesystem.cp"));
	assertTrue (g5.match("FILESYSTEM.CP"));

	Glob g6("[abc]", Glob::GLOB_CASELESS);
	assertTrue (g6.match("a"));
	assertTrue (g6.match("b"));
	assertTrue (g6.match("c"));
	assertTrue (g6.match("A"));
	assertTrue (g6.match("B"));
	assertTrue (g6.match("C"));

	Glob g7("[a-f]", Glob::GLOB_CASELESS);
	assertTrue (g7.match("a"));
	assertTrue (g7.match("b"));
	assertTrue (g7.match("f"));
	assertTrue (!g7.match("g"));
	assertTrue (g7.match("A"));
	assertTrue (g7.match("B"));
	assertTrue (g7.match("F"));
	assertTrue (!g7.match("G"));

	Glob g8("[A-F]", Glob::GLOB_CASELESS);
	assertTrue (g8.match("a"));
	assertTrue (g8.match("b"));
	assertTrue (g8.match("f"));
	assertTrue (!g8.match("g"));
	assertTrue (g8.match("A"));
	assertTrue (g8.match("B"));
	assertTrue (g8.match("F"));
	assertTrue (!g8.match("G"));
}


void GlobTest::testGlob()
{
	createFile("globtest/Makefile");
	createFile("globtest/.hidden");
	createFile("globtest/include/one.h");
	createFile("globtest/include/two.h");
	createFile("globtest/src/one.c");
	createFile("globtest/src/two.c");
	createFile("globtest/src/main.c");
	createFile("globtest/testsuite/src/test.h");
	createFile("globtest/testsuite/src/test.c");
	createFile("globtest/testsuite/src/main.c");

	std::set<std::string> files;
	Glob::glob("globtest/*", files);
	translatePaths(files);
	assertTrue (files.size() == 5);
	assertTrue (files.find("globtest/Makefile") != files.end());
	assertTrue (files.find("globtest/.hidden") != files.end());
	assertTrue (files.find("globtest/include/") != files.end());
	assertTrue (files.find("globtest/src/") != files.end());
	assertTrue (files.find("globtest/testsuite/") != files.end());

	files.clear();
	Glob::glob("GlobTest/*", files, Glob::GLOB_CASELESS);
	translatePaths(files);
	assertTrue (files.size() == 5);
	assertTrue (files.find("globtest/Makefile") != files.end());
	assertTrue (files.find("globtest/.hidden") != files.end());
	assertTrue (files.find("globtest/include/") != files.end());
	assertTrue (files.find("globtest/src/") != files.end());
	assertTrue (files.find("globtest/testsuite/") != files.end());

	files.clear();
	Glob::glob("globtest/*/*.[hc]", files);
	translatePaths(files);
	assertTrue (files.size() == 5);
	assertTrue (files.find("globtest/include/one.h") != files.end());
	assertTrue (files.find("globtest/include/two.h") != files.end());
	assertTrue (files.find("globtest/src/one.c") != files.end());
	assertTrue (files.find("globtest/src/one.c") != files.end());
	assertTrue (files.find("globtest/src/main.c") != files.end());

	files.clear();
	Glob::glob("gl?bt?st/*/*/*.c", files);
	translatePaths(files);
	assertTrue (files.size() == 2);
	assertTrue (files.find("globtest/testsuite/src/test.c") != files.end());
	assertTrue (files.find("globtest/testsuite/src/main.c") != files.end());

	files.clear();
	Glob::glob("Gl?bT?st/*/*/*.C", files, Glob::GLOB_CASELESS);
	translatePaths(files);
	assertTrue (files.size() == 2);
	assertTrue (files.find("globtest/testsuite/src/test.c") != files.end());
	assertTrue (files.find("globtest/testsuite/src/main.c") != files.end());

	files.clear();
	Glob::glob("globtest/*/src/*", files);
	translatePaths(files);
	assertTrue (files.size() == 3);
	assertTrue (files.find("globtest/testsuite/src/test.h") != files.end());
	assertTrue (files.find("globtest/testsuite/src/test.c") != files.end());
	assertTrue (files.find("globtest/testsuite/src/main.c") != files.end());

	files.clear();
	Glob::glob("globtest/*/", files);
	translatePaths(files);
	assertEqual (3, files.size());
	assertTrue (files.find("globtest/include/") != files.end());
	assertTrue (files.find("globtest/src/") != files.end());
	assertTrue (files.find("globtest/testsuite/") != files.end());

	files.clear();
	Glob::glob("globtest/testsuite/src/*", "globtest/testsuite/", files);
	translatePaths(files);
	assertEqual (3, files.size());
	assertTrue (files.find("globtest/testsuite/src/test.h") != files.end());
	assertTrue (files.find("globtest/testsuite/src/test.c") != files.end());
	assertTrue (files.find("globtest/testsuite/src/main.c") != files.end());

	// won't work if current directory is root dir
	files.clear();
	Glob::glob("globtest/../*/testsuite/*/", files);
	translatePaths(files);
	assertEqual (1, files.size());

	File dir("globtest");
	dir.remove(true);
}


void GlobTest::testMatchEmptyPattern()
{
	// Run the empty pattern against a number of subjects with all different match options
	const std::string empty;

	assertTrue (!Glob(empty, Glob::GLOB_DEFAULT).match("subject"));
	assertTrue (Glob(empty, Glob::GLOB_DEFAULT).match(empty));

	assertTrue (!Glob(empty, Glob::GLOB_DOT_SPECIAL).match("subject"));
	assertTrue (Glob(empty, Glob::GLOB_DOT_SPECIAL).match(empty));

	assertTrue (!Glob(empty, Glob::GLOB_CASELESS).match("subject"));
	assertTrue (Glob(empty, Glob::GLOB_CASELESS).match(empty));
}


void GlobTest::createFile(const std::string& path)
{
	Path p(path, Path::PATH_UNIX);
	File dir(p.parent());
	dir.createDirectories();
	std::ofstream ostr(path.c_str());
	ostr << path << std::endl;
}


void GlobTest::translatePaths(std::set<std::string>& paths)
{
	std::set<std::string> translated;
	for (std::set<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path p(*it);
		std::string tp(p.toString(Path::PATH_UNIX));
		translated.insert(tp);
	}
	paths = translated;
}


void GlobTest::setUp()
{
}


void GlobTest::tearDown()
{
}


CppUnit::Test* GlobTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("GlobTest");

	CppUnit_addTest(pSuite, GlobTest, testMatchChars);
	CppUnit_addTest(pSuite, GlobTest, testMatchQM);
	CppUnit_addTest(pSuite, GlobTest, testMatchAsterisk);
	CppUnit_addTest(pSuite, GlobTest, testMatchRange);
	CppUnit_addTest(pSuite, GlobTest, testMisc);
	CppUnit_addTest(pSuite, GlobTest, testCaseless);
	CppUnit_addTest(pSuite, GlobTest, testGlob);
	CppUnit_addTest(pSuite, GlobTest, testMatchEmptyPattern);

	return pSuite;
}
