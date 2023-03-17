//
// TestRunner.cpp
//


#include "CppUnit/TestRunner.h"
#include "CppUnit/Test.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TextTestResult.h"
#include <iostream>
#include <fstream>


namespace CppUnit {


TestRunner::TestRunner():
	_ostr(std::cout)
{
}


TestRunner::TestRunner(std::ostream& ostr):
	_ostr(ostr)
{
}


TestRunner::~TestRunner()
{
	for (Mappings::iterator it = _mappings.begin(); it != _mappings.end(); ++it)
		delete it->second;
}


void TestRunner::printBanner()
{
    _ostr
		<< "Usage: driver [-all] [-ignore <file> ] [-long] [-print] [-wait] [name] ..." << std::endl
		<< "       where name is the name of a test case class" << std::endl;
}


bool TestRunner::run(const std::vector<std::string>& args, const Test::Callback& callback)
{
	std::string testCase;
	int numberOfTests = 0;
	bool success = true;
	bool all     = false;
	bool wait    = false;
	bool printed = false;
	bool longRunning = false;
	std::string ignore;

	std::vector<std::string>	setup;

	std::vector<Test*> tests;
	for (int i = 1; i < args.size(); i++)
	{
		const std::string& arg = args[i];
		if (arg == "-wait")
		{
			wait = true;
			continue;
		}
		else if (arg == "-all")
		{
			all = true;
			continue;
		}
		else if (arg == "-long")
		{
			longRunning = true;
			continue;
		}
		else if (arg == "-ignore")
		{
			ignore = args[++i];
			continue;
		}
		else if (arg == "-print")
		{
			for (Mappings::iterator it = _mappings.begin(); it != _mappings.end(); ++it)
			{
				print(it->first, it->second, 0);
			}
			printed = true;
			continue;
		}
		else if (arg == "-setup")
		{
			if (i + 1 < args.size())
				setup.push_back(args[++i]);
			continue;
		}

		if (!all)
		{
			testCase = arg;

			if (testCase == "")
			{
				printBanner();
				return false;
			}

			Test* testToRun = 0;
			for (Mappings::iterator it = _mappings.begin(); !testToRun && it != _mappings.end(); ++it)
			{
				testToRun = find(testCase, it->second, it->first);
			}
			if (testToRun)
			{
				collectAllTestCases(testToRun, tests);
			}

			if (!testToRun)
			{
				_ostr << "Test " << testCase << " not found." << std::endl;
				return false;
			}
		}
	}
	if (all)
	{
		tests.clear();
		for (Mappings::iterator it = _mappings.begin(); it != _mappings.end(); ++it)
		{
			collectAllTestCases(it->second, tests);
		}
	}

	TextTestResult result(_ostr, ignore);
	for (std::vector<Test*>::const_iterator it = tests.begin(); it != tests.end(); ++it)
	{
		Test* testToRun = *it;
		if(testToRun->getType() == Test::Long && !longRunning)
			continue;
		if (setup.size() > 0)
			testToRun->addSetup(setup);

		testToRun->run(&result, callback);
		numberOfTests++;
	}
	_ostr << result << std::endl;
	success = result.wasSuccessful();

	if (numberOfTests == 0 && !printed)
	{
		printBanner();
		return false;
	}

	if (wait)
	{
		_ostr << "<RETURN> to continue" << std::endl;
		std::cin.get();
	}

	return success;
}


void TestRunner::addTest(const std::string& name, Test* test)
{
	_mappings.push_back(Mapping(name, test));
}


void TestRunner::print(const std::string& name, Test* pTest, int indent)
{
	for (int i = 0; i < indent; ++i)
		_ostr << "    ";
	_ostr << name << std::endl;
	TestSuite* pSuite = dynamic_cast<TestSuite*>(pTest);
	if (pSuite)
	{
		const std::vector<Test*>& tests = pSuite->tests();
		for (std::vector<Test*>::const_iterator it = tests.begin(); it != tests.end(); ++it)
		{
			print((*it)->toString(), *it, indent + 1);
		}
	}
}


Test* TestRunner::find(const std::string& name, Test* pTest, const std::string& testName)
{
	if (testName.find(name) != std::string::npos)
	{
		return pTest;
	}
	else
	{
		TestSuite* pSuite = dynamic_cast<TestSuite*>(pTest);
		if (pSuite)
		{
			const std::vector<Test*>& tests = pSuite->tests();
			for (std::vector<Test*>::const_iterator it = tests.begin(); it != tests.end(); ++it)
			{
				Test* result = find(name, *it, (*it)->toString());
				if (result) return result;
			}
		}
		return 0;
	}
}


int TestRunner::collectAllTestCases(Test* pTest, std::vector<Test*>& testcases)
{
	int added = 0;
	if (pTest->getType() == Test::Suite)
	{
		TestSuite* pSuite = dynamic_cast<TestSuite*>(pTest);

		if (pSuite)
		{
			const std::vector<Test*>& tests = pSuite->tests();
			for (std::vector<Test*>::const_iterator it = tests.begin(); it != tests.end(); ++it)
			{
				added += collectAllTestCases(*it, testcases);
			}
		}
	}
	else
	{
		testcases.push_back(pTest);
		added = 1;
	}
	return added;
}


} // namespace CppUnit
