//
// TextTestResult.cpp
//


#include "CppUnit/TextTestResult.h"
#include "CppUnit/CppUnitException.h"
#include "CppUnit/Test.h"
#include "CppUnit/estring.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <exception>


namespace CppUnit {


TextTestResult::TextTestResult() :
	_ostr(std::cout)
{
}


TextTestResult::TextTestResult(const std::string& ignore):
	_ostr(std::cout)
{
	if (!ignore.empty())
	{
		try
		{
			std::ifstream ifs(ignore);
			if (ifs.is_open())
			{
				char line[256];
				while (ifs.getline(line, sizeof(line)))
				{
					if (line[0] == '#')
						continue;
					std::string ignored(line);
					ignoring(ignored);
				}
				ifs.close();
			}
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

	}
	setup();
}


TextTestResult::TextTestResult(std::ostream& ostr):
	_ostr(ostr)
{
}


TextTestResult::TextTestResult(std::ostream& ostr, const std::string& ignore) :
	_ostr(ostr)
{
	if (!ignore.empty())
	{
		std::ifstream ifs(ignore);
		if (ifs.is_open())
		{
			char line[256];
			while (ifs.getline(line, sizeof(line)))
			{
				if (line[0] == '#')
					continue;
				std::string ignored(line);
				ignoring(ignored);
			}
			ifs.close();
		}
	}
	setup();
}


void TextTestResult::ignoring(const std::string ignore)
{
	std::string::const_iterator it = ignore.begin();
	std::string::const_iterator end = ignore.end();
	for (;;)
	{
		while (it != end && (std::isspace(*it) || *it == '"' || *it == ',' || *it == '\'')) ++it;
		if (it == end)
			break;

		std::string test;
		while (it != end && *it != ',' && *it != '"' && *it != '\'') test += *it++;
		if (!test.empty()) _ignored.insert(test.erase(test.find_last_not_of(" \n\r\t") + 1));
	}
}


void TextTestResult::setup()
{
	const char* env = std::getenv("CPPUNIT_IGNORE");
	if (env)
	{
		std::string ignored = env;
		ignoring(ignored);
	}
}


void TextTestResult::addError(Test* test, CppUnitException* e)
{
	if (_ignored.find(test->toString()) == _ignored.end())
	{
		TestResult::addError(test, e);
		_ostr << "ERROR" << std::flush;
	}
	else
	{
		_ostr << "ERROR (ignored)" << std::flush;
		delete e;
	}
}


void TextTestResult::addFailure(Test* test, CppUnitException* e)
{
	if (_ignored.find(test->toString()) == _ignored.end())
	{
		TestResult::addFailure(test, e);
		_ostr << "FAILURE" << std::flush;
	}
	else
	{
		_ostr << "FAILURE (ignored)" << std::flush;
		delete e;
	}
}


void TextTestResult::startTest(Test* test)
{
	TestResult::startTest(test);
	_ostr << "\n" << shortName(test->toString()) << ": " << std::flush;
}


void TextTestResult::printErrors(std::ostream& stream)
{
	if (testErrors() != 0)
	{
		stream << "\n";

		if (testErrors() == 1)
			stream << "There was " << testErrors() << " error: " << std::endl;
		else
			stream << "There were " << testErrors() << " errors: " << std::endl;

		int i = 1;
		for (std::vector<TestFailure*>::iterator it = errors().begin(); it != errors().end(); ++it)
		{
			TestFailure* failure = *it;
			CppUnitException* e = failure->thrownException();

			stream << std::setw(2) << i
			       << ": "
			       << failure->failedTest()->toString() << "\n"
			       << "    \"" << (e ? e->what() : "") << "\"\n"
			       << "    in \""
			       << (e ? e->fileName() : std::string())
			       << "\", line ";
			if (e == 0)
			{
				stream << "0";
			}
			else
			{
				stream << e->lineNumber();
				if (e->data2LineNumber() != CppUnitException::CPPUNIT_UNKNOWNLINENUMBER)
				{
					stream << " data lines " << e->data1LineNumber()
                                               << ", " << e->data2LineNumber();
				}
				else if (e->data1LineNumber() != CppUnitException::CPPUNIT_UNKNOWNLINENUMBER)
				{
					stream << " data line " << e->data1LineNumber();
				}
			}
			stream << std::endl;
			i++;
		}
	}
}


void TextTestResult::printFailures(std::ostream& stream)
{
	if (testFailures() != 0)
	{
		stream << "\n";
		if (testFailures() == 1)
			stream << "There was " << testFailures() << " failure: " << std::endl;
		else
			stream << "There were " << testFailures() << " failures: " << std::endl;

		int i = 1;

		for (std::vector<TestFailure*>::iterator it = failures().begin(); it != failures().end(); ++it)
		{
			TestFailure* failure = *it;
			CppUnitException* e = failure->thrownException();

			stream << std::setw(2) << i
			       << ": "
			       << failure->failedTest()->toString() << "\n"
			       << "    \"" << (e ? e->what() : "") << "\"\n"
			       << "    in \""
			       << (e ? e->fileName() : std::string())
			       << "\", line ";
			if (e == 0)
			{
				stream << "0";
			}
			else
			{
				stream << e->lineNumber();
				if (e->data2LineNumber() != CppUnitException::CPPUNIT_UNKNOWNLINENUMBER)
				{
					stream << " data lines "
					       << e->data1LineNumber()
                           << ", " << e->data2LineNumber();
				}
				else if (e->data1LineNumber() != CppUnitException::CPPUNIT_UNKNOWNLINENUMBER)
				{
					stream << " data line " << e->data1LineNumber();
				}
			}
			stream << std::endl;
			i++;
		}
	}
}


void TextTestResult::print(std::ostream& stream)
{
	printHeader(stream);
	printErrors(stream);
	printFailures(stream);
}


void TextTestResult::printHeader(std::ostream& stream)
{
	stream << "\n\n";
	if (wasSuccessful())
		stream << "OK ("
		          << runTests() << " tests)"
		          << std::endl;
	else
		stream << "!!!FAILURES!!!" << "\n"
		          << "Runs: "
		          << runTests ()
		          << "   Failures: "
		          << testFailures ()
		          << "   Errors: "
		          << testErrors ()
		          << std::endl;
}


std::string TextTestResult::shortName(const std::string& testName)
{
	std::string::size_type pos = testName.rfind('.');
	if (pos != std::string::npos)
		return std::string(testName, pos + 1);
	else
		return testName;
}


} // namespace CppUnit
