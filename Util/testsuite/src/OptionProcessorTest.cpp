//
// OptionProcessorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OptionProcessorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/OptionException.h"


using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionProcessor;


OptionProcessorTest::OptionProcessorTest(const std::string& name): CppUnit::TestCase(name)
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
	std::string name;
	std::string value;
	
	assertTrue (p1.process("-I/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("--include:/usr/local/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/local/include");
	
	assertTrue (p1.process("-I", name, value));
	assertTrue (name.empty());
	assertTrue (value.empty());
	assertTrue (p1.process("/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("-I", name, value));
	assertTrue (name.empty());
	assertTrue (value.empty());
	assertTrue (p1.process("-L", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "-L");

	assertTrue (p1.process("--lib=/usr/local/lib", name, value));
	assertTrue (name == "library-dir");
	assertTrue (value == "/usr/local/lib");
	
	assertTrue (p1.process("-ofile", name, value));
	assertTrue (name == "output");
	assertTrue (value == "file");
	
	assertTrue (!p1.process("src/file.cpp", name, value));
	assertTrue (!p1.process("/src/file.cpp", name, value));
	
	try
	{
		p1.process("--output:file", name, value);
		fail("duplicate - must throw");
	}
	catch (Poco::Util::DuplicateOptionException&)
	{
	}
	
	assertTrue (p1.process("-g", name, value));
	assertTrue (name == "debug");
	assertTrue (value == "");
	
	try
	{
		p1.process("--optimize", name, value);
		fail("incompatible - must throw");
	}
	catch (Poco::Util::IncompatibleOptionsException&)
	{
	}
	
	try
	{
		p1.process("-x", name, value);
		fail("unknown option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		p1.process("--in", name, value);
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
	std::string name;
	std::string value;
	
	assertTrue (p1.process("/Inc:/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("/include:/usr/local/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/local/include");

	assertTrue (p1.process("/Inc", name, value));
	assertTrue (name.empty());
	assertTrue (value.empty());
	assertTrue (p1.process("/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("/lib=/usr/local/lib", name, value));
	assertTrue (name == "library-dir");
	assertTrue (value == "/usr/local/lib");
	
	assertTrue (p1.process("/out:file", name, value));
	assertTrue (name == "output");
	assertTrue (value == "file");
	
	assertTrue (!p1.process("src/file.cpp", name, value));
	assertTrue (!p1.process("\\src\\file.cpp", name, value));
	
	try
	{
		p1.process("/output:file", name, value);
		fail("duplicate - must throw");
	}
	catch (Poco::Util::DuplicateOptionException&)
	{
	}
	
	assertTrue (p1.process("/debug", name, value));
	assertTrue (name == "debug");
	assertTrue (value == "");
	
	try
	{
		p1.process("/OPT", name, value);
		fail("incompatible - must throw");
	}
	catch (Poco::Util::IncompatibleOptionsException&)
	{
	}
	
	try
	{
		p1.process("/x", name, value);
		fail("unknown option - must throw");
	}
	catch (Poco::Util::UnknownOptionException&)
	{
	}

	try
	{
		p1.process("/in", name, value);
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
	std::string name;
	std::string value;
	
	try
	{
		p1.checkRequired();
		fail("no options specified - must throw");
	}
	catch (Poco::Util::MissingOptionException&)
	{
	}
	
	assertTrue (p1.process("-o", name, value));
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
	std::string name;
	std::string value;
	
	assertTrue (p1.process("-I/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("--include:/usr/local/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/local/include");
	
	assertTrue (p1.process("-I", name, value));
	assertTrue (name.empty());
	assertTrue (value.empty());
	assertTrue (p1.process("/usr/include", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "/usr/include");

	assertTrue (p1.process("-I", name, value));
	assertTrue (name.empty());
	assertTrue (value.empty());
	assertTrue (p1.process("-L", name, value));
	assertTrue (name == "include-dir");
	assertTrue (value == "-L");

	assertTrue (p1.process("-O", name, value));
	assertTrue (name == "optimize");
	assertTrue (value.empty());
	
	assertTrue (p1.process("-O2", name, value));
	assertTrue (name == "optimize");
	assertTrue (value == "2");
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
