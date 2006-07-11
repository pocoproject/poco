//
// TestRunner.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TestRunner.h#1 $
//


#ifndef CppUnit_TestRunner_INCLUDED
#define CppUnit_TestRunner_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef STD_VECTOR_INCLUDED
#include <vector>
#define STD_VECTOR_INCLUDED
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
#define STD_STRING_INCLUDED
#endif


CppUnit_BEGIN


class Test;


/*
 * A command line based tool to run tests.
 * TestRunner expects as its only argument the name of a TestCase class.
 * TestRunner prints out a trace as the tests are executed followed by a
 * summary at the end.
 *
 * You can add to the tests that the TestRunner knows about by
 * making additional calls to "addTest (...)" in main.
 *
 * Here is the synopsis:
 *
 * TestRunner [-all] [-print] [-wait] ExampleTestCase
 *
 */
class CppUnit_API TestRunner
{
	typedef std::pair<std::string, Test*> Mapping;
	typedef std::vector<Mapping> Mappings;

public:
	TestRunner();
	~TestRunner();

	bool run(const std::vector<std::string>& args);
	void addTest(const std::string& name, Test* test);

protected:
	bool run(Test* test);
	void printBanner();
	void print(const std::string& name, Test* pTest, int indent);
	Test* find(const std::string& name, Test* pTest, const std::string& testName);

private:
	Mappings _mappings;
};


CppUnit_END


#define CppUnitMain(testCase) \
	int main(int ac, char **av)							\
	{													\
		std::vector<std::string> args;					\
		for (int i = 0; i < ac; ++i)					\
			args.push_back(std::string(av[i]));			\
		CppUnit::TestRunner runner;						\
		runner.addTest(#testCase, testCase::suite());	\
		return runner.run(args) ? 0 : 1;				\
	}


#endif // CppUnit_TestRunner_INCLUDED
