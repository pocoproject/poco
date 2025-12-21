//
// TestRunner.cpp
//


#include "CppUnit/TestRunner.h"
#include "CppUnit/Test.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TextTestResult.h"
#include <iostream>


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
	for (auto & _mapping : _mappings)
		delete _mapping.second;
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
	for (std::size_t i = 1; i < args.size(); i++)
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
			for (auto& _mapping : _mappings)
			{
				print(_mapping.first, _mapping.second, 0);
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

			std::vector<Test*> matchedTests;
			for (auto& mapping : _mappings)
			{
				findAll(testCase, mapping.second, mapping.first, matchedTests);
			}

			if (!matchedTests.empty())
			{
				for (auto* matchedTest : matchedTests)
				{
					collectAllTestCases(matchedTest, tests);
				}
			}
			else
			{
				_ostr << "Test " << testCase << " not found." << std::endl;
				return false;
			}
		}
	}
	if (all)
	{
		tests.clear();
		for (auto& _mapping : _mappings)
		{
			collectAllTestCases(_mapping.second, tests);
		}
	}

	TextTestResult result(_ostr, ignore);
	for (auto testToRun : tests)
	{
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
	_mappings.emplace_back(name, test);
}


void TestRunner::print(const std::string& name, Test* pTest, int indent, const std::string& parentName)
{
	for (int i = 0; i < indent; ++i)
		_ostr << "    ";

	// For non-suite tests (individual test cases), print as ParentClass::testName
	TestSuite* pSuite = dynamic_cast<TestSuite*>(pTest);
	if (!pSuite && !parentName.empty())
	{
		// Convert "TestCaller<ClassName>.methodName" to "ClassName::methodName"
		// On MSVC, typeid().name() includes "class " prefix, so handle "TestCaller<class ClassName>" too
		std::string displayName = name;
		std::string::size_type callerPos = displayName.find("TestCaller<");
		if (callerPos != std::string::npos)
		{
			std::string::size_type endPos = displayName.find(">.", callerPos);
			if (endPos != std::string::npos)
			{
				std::string className = displayName.substr(callerPos + 11, endPos - callerPos - 11);
				std::string methodName = displayName.substr(endPos + 2);

				// Strip "class " or "struct " prefix added by MSVC's typeid().name()
				if (className.compare(0, 6, "class ") == 0)
					className = className.substr(6);
				else if (className.compare(0, 7, "struct ") == 0)
					className = className.substr(7);

				displayName = className + "::" + methodName;
			}
		}
		_ostr << displayName << std::endl;
	}
	else
	{
		_ostr << name << std::endl;
	}

	if (pSuite)
	{
		const std::vector<Test*>& tests = pSuite->tests();
		for (auto* test : tests)
		{
			print(test->toString(), test, indent + 1, name);
		}
	}
}


bool TestRunner::matchesName(const std::string& searchName, const std::string& testName)
{
	// Normalize :: to . in search name
	// e.g., "FastLoggerTest::testLogger" -> "FastLoggerTest.testLogger"
	std::string normalizedSearch = searchName;
	std::string::size_type pos = 0;
	while ((pos = normalizedSearch.find("::", pos)) != std::string::npos)
	{
		normalizedSearch.replace(pos, 2, ".");
		pos += 1;
	}

	// Check if search contains a separator (Class.method or Class::method format)
	std::string::size_type sepPos = normalizedSearch.find('.');
	bool hasMethodSeparator = (sepPos != std::string::npos);

	if (hasMethodSeparator)
	{
		// Full "Class.method" match
		// Search: "FastLoggerTest.testLogger"
		// testName might be: "CppUnit::TestCaller<FastLoggerTest>.testLogger" or "TestCaller<FastLoggerTest>.testLogger"
		// On MSVC: "TestCaller<class FastLoggerTest>.testLogger" (typeid includes "class " prefix)
		std::string searchClass = normalizedSearch.substr(0, sepPos);
		std::string searchMethod = normalizedSearch.substr(sepPos + 1);

		// Build patterns to match (with and without CppUnit:: namespace, with and without "class " prefix)
		std::string pattern1 = "TestCaller<" + searchClass + ">." + searchMethod;
		std::string pattern2 = "CppUnit::TestCaller<" + searchClass + ">." + searchMethod;
		std::string pattern3 = "TestCaller<class " + searchClass + ">." + searchMethod;  // MSVC
		std::string pattern4 = "CppUnit::TestCaller<class " + searchClass + ">." + searchMethod;  // MSVC

		if (testName == pattern1 || testName == pattern2 || testName == pattern3 || testName == pattern4)
			return true;

		// Also check if testName ends with the pattern (handles namespace variations)
		if (testName.length() >= pattern1.length() &&
		    testName.substr(testName.length() - pattern1.length()) == pattern1)
			return true;
		if (testName.length() >= pattern3.length() &&
		    testName.substr(testName.length() - pattern3.length()) == pattern3)
			return true;

		// Also check for direct substring match (for other test formats)
		if (testName.find(normalizedSearch) != std::string::npos)
			return true;
	}
	else
	{
		// Class name or method name only - need smarter matching
		// testName format is typically "CppUnit::TestCaller<ClassName>.methodName" or just "ClassName"
		// or "suite SuiteName" for test suites
		// On MSVC: "TestCaller<class ClassName>.methodName"

		// Check for exact suite name match (top-level)
		if (testName == normalizedSearch)
			return true;

		// Check for suite name match (TestSuite::toString() returns "suite <name>")
		if (testName == "suite " + normalizedSearch)
			return true;

		// Check for class name match in TestCaller format: "TestCaller<ClassName>." or "CppUnit::TestCaller<ClassName>."
		// Also handle MSVC's "class " prefix
		std::string callerPattern = "TestCaller<" + normalizedSearch + ">.";
		std::string callerPatternMsvc = "TestCaller<class " + normalizedSearch + ">.";
		if (testName.find(callerPattern) != std::string::npos || testName.find(callerPatternMsvc) != std::string::npos)
			return true;

		// Check for exact method name match (after the dot)
		std::string::size_type dotPos = testName.rfind('.');
		if (dotPos != std::string::npos)
		{
			std::string methodName = testName.substr(dotPos + 1);
			if (methodName == normalizedSearch)
				return true;
		}
	}

	return false;
}


Test* TestRunner::find(const std::string& name, Test* pTest, const std::string& testName)
{
	if (matchesName(name, testName))
	{
		return pTest;
	}
	else
	{
		auto* pSuite = dynamic_cast<TestSuite*>(pTest);
		if (pSuite)
		{
			const std::vector<Test*>& tests = pSuite->tests();
			for (auto* test : tests)
			{
				Test* result = find(name, test, test->toString());
				if (result) return result;
			}
		}
		return nullptr;
	}
}


void TestRunner::findAll(const std::string& name, Test* pTest, const std::string& testName, std::vector<Test*>& results)
{
	if (matchesName(name, testName))
	{
		results.push_back(pTest);
		// Don't search children - collectAllTestCases will expand suites
		return;
	}

	auto* pSuite = dynamic_cast<TestSuite*>(pTest);
	if (pSuite)
	{
		const std::vector<Test*>& tests = pSuite->tests();
		for (auto* test : tests)
		{
			findAll(name, test, test->toString(), results);
		}
	}
}


int TestRunner::collectAllTestCases(Test* pTest, std::vector<Test*>& testcases)
{
	int added = 0;
	if (pTest->getType() == Test::Suite)
	{
		auto* pSuite = dynamic_cast<TestSuite*>(pTest);

		if (pSuite)
		{
			const std::vector<Test*>& tests = pSuite->tests();
			for (auto* test : tests)
			{
				added += collectAllTestCases(test, testcases);
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
