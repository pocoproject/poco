//
// OptionProcessorTest.cpp
//
// $Id: //poco/1.4/Util/testsuite/src/OptionProcessorTest.cpp#2 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OptionProcessorTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/OptionException.h"


using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionProcessor;


OptionProcessorTest::OptionProcessorTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


OptionProcessorTest::~OptionProcessorTest()
{
}


void OptionProcessorTest::testUnix()
{
	OptionSet set;
	set.addOption(
		Option("include-dir", "I", "specify a search path for locating header files")
			.required(false)
			.repeatable(true)
			.argument("path"));
			
	set.addOption(
		Option("library-dir", "L", "specify a search path for locating library files")
			.required(false)
			.repeatable(true)
			.argument("path"));

	set.addOption(
		Option("output", "o", "specify the output file", true)
			.argument("file", true));

	set.addOption(
		Option("verbose", "v")
		.description("enable verbose mode")
		.required(false)
		.repeatable(false));
		
	set.addOption(
		Option("optimize", "O")
		.description("enable optimization")
		.required(false)
		.repeatable(false)
		.argument("level", false)
		.group("mode"));
		
	set.addOption(
		Option("debug", "g")
		.description("generate debug information")
		.required(false)
		.repeatable(false)
		.group("mode"));

	set.addOption(
		Option("info", "i")
		.description("print information")
		.required(false)
		.repeatable(false));

	OptionProcessor p1(set);
	std::string optionName;
	std::string value;
	
	assert (p1.process("-I/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("--include:/usr/local/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/local/include");
	
	assert (p1.process("-I", optionName, value));
	assert (optionName.empty());
	assert (value.empty());
	assert (p1.process("/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("-I", optionName, value));
	assert (optionName.empty());
	assert (value.empty());
	assert (p1.process("-L", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "-L");

	assert (p1.process("--lib=/usr/local/lib", optionName, value));
	assert (optionName == "library-dir");
	assert (value == "/usr/local/lib");
	
	assert (p1.process("-ofile", optionName, value));
	assert (optionName == "output");
	assert (value == "file");
	
	assert (!p1.process("src/file.cpp", optionName, value));
	assert (!p1.process("/src/file.cpp", optionName, value));
	
	try
	{
		p1.process("--output:file", optionName, value);
		fail("duplicate - must throw");
	}
	catch (Poco::Util::DuplicateOptionException&)
	{
	}
	
	assert (p1.process("-g", optionName, value));
	assert (optionName == "debug");
	assert (value == "");
	
	try
	{
		p1.process("--optimize", optionName, value);
		fail("incompatible - must throw");
	}
	catch (Poco::Util::IncompatibleOptionsException&)
	{
	}
	
	try
	{
		p1.process("-x", optionName, value);
		fail("unknown option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		p1.process("--in", optionName, value);
		fail("ambiguous option - must throw");
	}
	catch (Poco::Util::AmbiguousOptionException&)
	{
	}
}


void OptionProcessorTest::testDefault()
{
	OptionSet set;
	set.addOption(
		Option("include-dir", "I", "specify a search path for locating header files")
			.required(false)
			.repeatable(true)
			.argument("path"));
			
	set.addOption(
		Option("library-dir", "L", "specify a search path for locating library files")
			.required(false)
			.repeatable(true)
			.argument("path"));

	set.addOption(
		Option("output", "o", "specify the output file", true)
			.argument("file", true));

	set.addOption(
		Option("verbose", "v")
		.description("enable verbose mode")
		.required(false)
		.repeatable(false));
		
	set.addOption(
		Option("optimize", "O")
		.description("enable optimization")
		.required(false)
		.repeatable(false)
		.argument("level", false)
		.group("mode"));
		
	set.addOption(
		Option("debug", "g")
		.description("generate debug information")
		.required(false)
		.repeatable(false)
		.group("mode"));

	set.addOption(
		Option("info", "i")
		.description("print information")
		.required(false)
		.repeatable(false));

	OptionProcessor p1(set);
	p1.setUnixStyle(false);
	std::string optionName;
	std::string value;
	
	assert (p1.process("/Inc:/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("/include:/usr/local/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/local/include");

	assert (p1.process("/Inc", optionName, value));
	assert (optionName.empty());
	assert (value.empty());
	assert (p1.process("/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("/lib=/usr/local/lib", optionName, value));
	assert (optionName == "library-dir");
	assert (value == "/usr/local/lib");
	
	assert (p1.process("/out:file", optionName, value));
	assert (optionName == "output");
	assert (value == "file");
	
	assert (!p1.process("src/file.cpp", optionName, value));
	assert (!p1.process("\\src\\file.cpp", optionName, value));
	
	try
	{
		p1.process("/output:file", optionName, value);
		fail("duplicate - must throw");
	}
	catch (Poco::Util::DuplicateOptionException&)
	{
	}
	
	assert (p1.process("/debug", optionName, value));
	assert (optionName == "debug");
	assert (value == "");
	
	try
	{
		p1.process("/OPT", optionName, value);
		fail("incompatible - must throw");
	}
	catch (Poco::Util::IncompatibleOptionsException&)
	{
	}
	
	try
	{
		p1.process("/x", optionName, value);
		fail("unknown option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		p1.process("/in", optionName, value);
		fail("ambiguous option - must throw");
	}
	catch (Poco::Util::AmbiguousOptionException&)
	{
	}
}


void OptionProcessorTest::testRequired()
{
	OptionSet set;
	set.addOption(
		Option("option", "o")
			.required(true)
			.repeatable(true));

	OptionProcessor p1(set);
	std::string optionName;
	std::string value;
	
	try
	{
		p1.checkRequired();
		fail("no options specified - must throw");
	}
	catch (Poco::Util::MissingOptionException&)
	{
	}
	
	assert (p1.process("-o", optionName, value));
	p1.checkRequired();
}


void OptionProcessorTest::testArgs()
{
	OptionSet set;
	set.addOption(
		Option("include-dir", "I", "specify a search path for locating header files")
			.required(false)
			.repeatable(true)
			.argument("path"));
		
	set.addOption(
		Option("optimize", "O")
		.description("enable optimization")
		.required(false)
		.repeatable(true)
		.argument("level", false));

	OptionProcessor p1(set);
	std::string optionName;
	std::string value;
	
	assert (p1.process("-I/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("--include:/usr/local/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/local/include");
	
	assert (p1.process("-I", optionName, value));
	assert (optionName.empty());
	assert (value.empty());
	assert (p1.process("/usr/include", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "/usr/include");

	assert (p1.process("-I", optionName, value));
	assert (optionName.empty());
	assert (value.empty());
	assert (p1.process("-L", optionName, value));
	assert (optionName == "include-dir");
	assert (value == "-L");

	assert (p1.process("-O", optionName, value));
	assert (optionName == "optimize");
	assert (value.empty());
	
	assert (p1.process("-O2", optionName, value));
	assert (optionName == "optimize");
	assert (value == "2");
}


void OptionProcessorTest::setUp()
{
}


void OptionProcessorTest::tearDown()
{
}


CppUnit::Test* OptionProcessorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OptionProcessorTest");

	CppUnit_addTest(pSuite, OptionProcessorTest, testUnix);
	CppUnit_addTest(pSuite, OptionProcessorTest, testDefault);
	CppUnit_addTest(pSuite, OptionProcessorTest, testRequired);
	CppUnit_addTest(pSuite, OptionProcessorTest, testArgs);

	return pSuite;
}
