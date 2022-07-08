//
// OptionTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OptionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"


using Poco::Util::Option;


OptionTest::OptionTest(const std::string& name): CppUnit::TestCase(name)
{
}


OptionTest::~OptionTest()
{
}


void OptionTest::testOption()
{
	Option incOpt = Option("include-dir", "I", "specify an include search path")
		.required(false)
		.repeatable(true)
		.argument("path");

	Option libOpt = Option("library-dir", "L", "specify a library search path", false)
		.repeatable(true)
		.argument("path");

	Option outOpt = Option("output", "o", "specify the output file", true)
		.argument("file", true);

	Option vrbOpt = Option("verbose", "v")
		.description("enable verbose mode")
		.required(false)
		.repeatable(false);

	Option optOpt = Option("optimize", "O")
		.description("enable optimization")
		.required(false)
		.repeatable(false)
		.argument("level", false);

	assertTrue (incOpt.shortName() == "I");
	assertTrue (incOpt.fullName() == "include-dir");
	assertTrue (incOpt.repeatable());
	assertTrue (!incOpt.required());
	assertTrue (incOpt.argumentName() == "path");
	assertTrue (incOpt.argumentRequired());
	assertTrue (incOpt.takesArgument());

	assertTrue (libOpt.shortName() == "L");
	assertTrue (libOpt.fullName() == "library-dir");
	assertTrue (libOpt.repeatable());
	assertTrue (!libOpt.required());
	assertTrue (libOpt.argumentName() == "path");
	assertTrue (libOpt.argumentRequired());
	assertTrue (incOpt.takesArgument());

	assertTrue (outOpt.shortName() == "o");
	assertTrue (outOpt.fullName() == "output");
	assertTrue (!outOpt.repeatable());
	assertTrue (outOpt.required());
	assertTrue (outOpt.argumentName() == "file");
	assertTrue (outOpt.argumentRequired());
	assertTrue (incOpt.takesArgument());

	assertTrue (vrbOpt.shortName() == "v");
	assertTrue (vrbOpt.fullName() == "verbose");
	assertTrue (!vrbOpt.repeatable());
	assertTrue (!vrbOpt.required());
	assertTrue (!vrbOpt.argumentRequired());
	assertTrue (!vrbOpt.takesArgument());

	assertTrue (optOpt.shortName() == "O");
	assertTrue (optOpt.fullName() == "optimize");
	assertTrue (!optOpt.repeatable());
	assertTrue (!optOpt.required());
	assertTrue (optOpt.argumentName() == "level");
	assertTrue (optOpt.takesArgument());
	assertTrue (!optOpt.argumentRequired());
}


void OptionTest::testMatches1()
{
	Option incOpt = Option("include-dir", "I", "specify an include search path")
		.required(false)
		.repeatable(true)
		.argument("path");

	assertTrue (incOpt.matchesShort("Iinclude"));
	assertTrue (incOpt.matchesPartial("include:include"));
	assertTrue (incOpt.matchesPartial("include-dir:include"));
	assertTrue (incOpt.matchesPartial("inc=include"));
	assertTrue (incOpt.matchesPartial("INCLUDE=include"));
	assertTrue (incOpt.matchesPartial("include"));
	assertTrue (incOpt.matchesShort("I"));
	assertTrue (incOpt.matchesPartial("i"));

	assertTrue (incOpt.matchesFull("include-dir:include"));
	assertTrue (incOpt.matchesFull("INClude-dir:include"));
	assertTrue (!incOpt.matchesFull("include:include"));
	assertTrue (!incOpt.matchesFull("include-dir2:include"));

	assertTrue (!incOpt.matchesPartial("include-dir2=include"));
	assertTrue (!incOpt.matchesShort("linclude"));
}


void OptionTest::testMatches2()
{
	Option incOpt = Option("include-dir", "", "specify an include search path")
		.required(false)
		.repeatable(true)
		.argument("path");

	assertTrue (!incOpt.matchesShort("Iinclude"));
	assertTrue (incOpt.matchesPartial("include:include"));
	assertTrue (incOpt.matchesPartial("include-dir:include"));
	assertTrue (incOpt.matchesPartial("inc=include"));
	assertTrue (incOpt.matchesPartial("INCLUDE=include"));
	assertTrue (incOpt.matchesPartial("I"));
	assertTrue (incOpt.matchesPartial("i"));

	assertTrue (incOpt.matchesFull("include-dir:include"));
	assertTrue (incOpt.matchesFull("INClude-dir:include"));
	assertTrue (!incOpt.matchesFull("include:include"));
	assertTrue (!incOpt.matchesFull("include-dir2:include"));

	assertTrue (!incOpt.matchesFull("include-dir2=include"));
	assertTrue (!incOpt.matchesShort("linclude"));
}


void OptionTest::testProcess1()
{
	Option incOpt = Option("include-dir", "I", "specify an include search path")
		.required(false)
		.repeatable(true)
		.argument("path");

	std::string arg;
	incOpt.process("Iinclude", arg);
	assertTrue (arg == "include");
	incOpt.process("I/usr/include", arg);
	assertTrue (arg == "/usr/include");
	incOpt.process("include:/usr/local/include", arg);
	assertTrue (arg == "/usr/local/include");
	incOpt.process("include=/proj/include", arg);
	assertTrue (arg == "/proj/include");
	incOpt.process("include-dir=/usr/include", arg);
	assertTrue (arg == "/usr/include");
	incOpt.process("Include-dir:/proj/include", arg);
	assertTrue (arg == "/proj/include");

	try
	{
		incOpt.process("I", arg);
		fail("argument required - must throw");
	}
	catch (Poco::Util::MissingArgumentException&)
	{
	}

	try
	{
		incOpt.process("Include", arg);
		fail("argument required - must throw");
	}
	catch (Poco::Util::MissingArgumentException&)
	{
	}

	try
	{
		incOpt.process("Llib", arg);
		fail("wrong option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	Option vrbOpt = Option("verbose", "v")
		.description("enable verbose mode")
		.required(false)
		.repeatable(false);

	vrbOpt.process("v", arg);
	assertTrue (arg.empty());
	vrbOpt.process("verbose", arg);
	assertTrue (arg.empty());

	try
	{
		vrbOpt.process("v2", arg);
		fail("no argument expected - must throw");
	}
	catch (Poco::Util::UnexpectedArgumentException&)
	{
	}

	try
	{
		vrbOpt.process("verbose:2", arg);
		fail("no argument expected - must throw");
	}
	catch (Poco::Util::UnexpectedArgumentException&)
	{
	}

	Option optOpt = Option("optimize", "O")
		.description("enable optimization")
		.required(false)
		.repeatable(false)
		.argument("level", false);

	optOpt.process("O", arg);
	assertTrue (arg.empty());
	optOpt.process("O2", arg);
	assertTrue (arg == "2");
	optOpt.process("optimize:1", arg);
	assertTrue (arg == "1");
	optOpt.process("opt", arg);
	assertTrue (arg.empty());
	optOpt.process("opt=3", arg);
	assertTrue (arg == "3");
	optOpt.process("opt=", arg);
	assertTrue (arg.empty());
}


void OptionTest::testProcess2()
{
	Option incOpt = Option("include-dir", "", "specify an include search path")
		.required(false)
		.repeatable(true)
		.argument("path");

	std::string arg;
	incOpt.process("include:/usr/local/include", arg);
	assertTrue (arg == "/usr/local/include");
	incOpt.process("include=/proj/include", arg);
	assertTrue (arg == "/proj/include");
	incOpt.process("include-dir=/usr/include", arg);
	assertTrue (arg == "/usr/include");
	incOpt.process("Include-dir:/proj/include", arg);
	assertTrue (arg == "/proj/include");

	try
	{
		incOpt.process("Iinclude", arg);
		fail("unknown option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		incOpt.process("I", arg);
		fail("argument required - must throw");
	}
	catch (Poco::Util::MissingArgumentException&)
	{
	}

	try
	{
		incOpt.process("Include", arg);
		fail("argument required - must throw");
	}
	catch (Poco::Util::MissingArgumentException&)
	{
	}

	try
	{
		incOpt.process("Llib", arg);
		fail("wrong option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	Option vrbOpt = Option("verbose", "")
		.description("enable verbose mode")
		.required(false)
		.repeatable(false);

	vrbOpt.process("v", arg);
	assertTrue (arg.empty());
	vrbOpt.process("verbose", arg);
	assertTrue (arg.empty());

	try
	{
		vrbOpt.process("v2", arg);
		fail("no argument expected - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		vrbOpt.process("verbose:2", arg);
		fail("no argument expected - must throw");
	}
	catch (Poco::Util::UnexpectedArgumentException&)
	{
	}
}


void OptionTest::setUp()
{
}


void OptionTest::tearDown()
{
}


CppUnit::Test* OptionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OptionTest");

	CppUnit_addTest(pSuite, OptionTest, testOption);
	CppUnit_addTest(pSuite, OptionTest, testMatches1);
	CppUnit_addTest(pSuite, OptionTest, testMatches2);
	CppUnit_addTest(pSuite, OptionTest, testProcess1);
	CppUnit_addTest(pSuite, OptionTest, testProcess2);

	return pSuite;
}
