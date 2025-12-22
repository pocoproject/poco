//
// StringTokenizerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#include <iostream>

#include "StringTokenizerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Exception.h"


using Poco::StringTokenizer;
using Poco::RangeException;
using Poco::NotFoundException;


StringTokenizerTest::StringTokenizerTest(const std::string& name): CppUnit::TestCase(name)
{
}


StringTokenizerTest::~StringTokenizerTest()
{
}


void StringTokenizerTest::testStringTokenizer()
{
	{
		StringTokenizer st("", "");
		assertTrue (st.begin() == st.end());
	}
	{
		StringTokenizer st("", "", StringTokenizer::TOK_IGNORE_EMPTY);
		assertTrue (st.begin() == st.end());
	}
	{
		StringTokenizer st("", "", StringTokenizer::TOK_TRIM);
		assertTrue (st.begin() == st.end());
	}
	{
		StringTokenizer st("", "", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		assertTrue (st.begin() == st.end());
	}
	{
		StringTokenizer st("abc", "");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc ", "", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("  abc  ", "", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("  abc", "", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "b");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (st.find("c") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "b", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (st.find("c") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "bc");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (st.find("") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "bc", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (st.find("") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "bc", StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "bc", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc", "bc", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("a a,c c", ",");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a a") == 0);
		assertTrue (st.find("c c") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("a a,c c", ",", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a a") == 0);
		assertTrue (st.find("c c") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(" a a , , c c ", ",");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find(" a a ") == 0);
		assertTrue (st.find(" ") == 1);
		assertTrue (st.find(" c c ") == 2);
		assertTrue (it != st.end());
		assertTrue (*it++ == " a a ");
		assertTrue (it != st.end());
		assertTrue (*it++ == " ");
		assertTrue (it != st.end());
		assertTrue (*it++ == " c c ");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(" a a , , c c ", ",", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a a") == 0);
		assertTrue (st.find("") == 1);
		assertTrue (st.find("c c") == 2);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(" a a , , c c ", ",", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a a") == 0);
		assertTrue (st.find("c c") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "c c");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc,def,,ghi , jk,  l ", ",", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (st.find("def") == 1);
		assertTrue (st.find("ghi") == 2);
		assertTrue (st.find("jk") == 3);
		assertTrue (st.find("l") == 4);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it != st.end());
		assertTrue (*it++ == "def");
		assertTrue (it != st.end());
		assertTrue (*it++ == "ghi");
		assertTrue (it != st.end());
		assertTrue (*it++ == "jk");
		assertTrue (it != st.end());
		assertTrue (*it++ == "l");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("abc,def,,ghi // jk,  l ", ",/", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("abc") == 0);
		assertTrue (st.find("def") == 1);
		assertTrue (st.find("ghi") == 2);
		assertTrue (st.find("jk") == 3);
		assertTrue (st.find("l") == 4);
		assertTrue (it != st.end());
		assertTrue (*it++ == "abc");
		assertTrue (it != st.end());
		assertTrue (*it++ == "def");
		assertTrue (it != st.end());
		assertTrue (*it++ == "ghi");
		assertTrue (it != st.end());
		assertTrue (*it++ == "jk");
		assertTrue (it != st.end());
		assertTrue (*it++ == "l");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("a/bc,def,,ghi // jk,  l ", ",/", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("a") == 0);
		assertTrue (st.find("bc") == 1);
		assertTrue (st.find("def") == 2);
		assertTrue (st.find("ghi") == 3);
		assertTrue (st.find("jk") == 4);
		assertTrue (st.find("l") == 5);
		assertTrue (it != st.end());
		assertTrue (*it++ == "a");
		assertTrue (it != st.end());
		assertTrue (*it++ == "bc");
		assertTrue (it != st.end());
		assertTrue (*it++ == "def");
		assertTrue (it != st.end());
		assertTrue (*it++ == "ghi");
		assertTrue (it != st.end());
		assertTrue (*it++ == "jk");
		assertTrue (it != st.end());
		assertTrue (*it++ == "l");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(",ab,cd,", ",");
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("") == 0);
		assertTrue (st.find("ab") == 1);
		assertTrue (st.find("cd") == 2);
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it != st.end());
		assertTrue (*it++ == "ab");
		assertTrue (it != st.end());
		assertTrue (*it++ == "cd");
		assertTrue (it != st.end());
		assertTrue (*it++ == "");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(",ab,cd,", ",", StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("ab") == 0);
		assertTrue (st.find("cd") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "ab");
		assertTrue (it != st.end());
		assertTrue (*it++ == "cd");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st(" , ab , cd , ", ",", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		StringTokenizer::Iterator it = st.begin();
		assertTrue (st.find("ab") == 0);
		assertTrue (st.find("cd") == 1);
		assertTrue (it != st.end());
		assertTrue (*it++ == "ab");
		assertTrue (it != st.end());
		assertTrue (*it++ == "cd");
		assertTrue (it == st.end());
	}
	{
		StringTokenizer st("1 : 2 , : 3 ", ":,", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);
		assertTrue (st.count() == 3);
		assertTrue (st[0] == "1");
		assertTrue (st[1] == "2");
		assertTrue (st[2] == "3");
		assertTrue (st.find("1") == 0);
		assertTrue (st.find("2") == 1);
		assertTrue (st.find("3") == 2);
	}

	{
		Poco::StringTokenizer st(" 2- ","-", Poco::StringTokenizer::TOK_TRIM);
		assertTrue (st.count() == 2);
		assertTrue (st[0] == "2");
		assertTrue (st[1] == "");
	}
}


void StringTokenizerTest::testFind()
{
	StringTokenizer st("0,1,2,3,3,2,1,0", ",", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);

	assertTrue (st.count() == 8);
	assertTrue (2 == st.count("0"));
	assertTrue (2 == st.count("1"));
	assertTrue (2 == st.count("2"));
	assertTrue (2 == st.count("3"));
	assertTrue (0 == st.count("4"));
	assertTrue (0 == st.count("5"));
	assertTrue (0 == st.count("6"));
	assertTrue (0 == st.count("7"));

	assertTrue (st[0] == "0");
	assertTrue (st[1] == "1");
	assertTrue (st[2] == "2");
	assertTrue (st[3] == "3");
	assertTrue (st[4] == "3");
	assertTrue (st[5] == "2");
	assertTrue (st[6] == "1");
	assertTrue (st[7] == "0");

	assertTrue (st.has("0"));
	assertTrue (st.has("1"));
	assertTrue (st.has("2"));
	assertTrue (st.has("3"));
	assertTrue (!st.has("4"));
	assertTrue (!st.has("5"));
	assertTrue (!st.has("6"));
	assertTrue (!st.has("7"));

	assertTrue (st.find("0") == 0);
	assertTrue (st.find("1") == 1);
	assertTrue (st.find("2") == 2);
	assertTrue (st.find("3") == 3);

	assertTrue (st.find("0", 1) == 7);
	assertTrue (st.find("1", 2) == 6);
	assertTrue (st.find("2", 3) == 5);
	assertTrue (st.find("3", 4) == 4);

	try
	{
		std::size_t POCO_UNUSED p = st.find("4");
		fail ("must fail");
	}
	catch (NotFoundException&) { }

	try
	{
		std::string POCO_UNUSED s = st[8];
		fail ("must fail");
	}
	catch (RangeException&) { }

	st[0] = "1";
	st[7] = "1";
	assertTrue (st[0] == "1");
	assertTrue (st[7] == "1");
	assertTrue (0 == st.count("0"));
	assertTrue (4 == st.count("1"));

	st.replace("2", "5");
	assertTrue (0 == st.count("2"));
	assertTrue (2 == st.count("5"));

	st.replace("3", "6", 4);
	assertTrue (1 == st.count("3"));
	assertTrue (1 == st.count("6"));
}


void StringTokenizerTest::setUp()
{
}


void StringTokenizerTest::tearDown()
{
}


CppUnit::Test* StringTokenizerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StringTokenizerTest");

	CppUnit_addTest(pSuite, StringTokenizerTest, testStringTokenizer);
	CppUnit_addTest(pSuite, StringTokenizerTest, testFind);

	return pSuite;
}
