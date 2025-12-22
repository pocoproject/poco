//
// TestRunner.h
//


#ifndef CppUnit_TestRunner_INCLUDED
#define CppUnit_TestRunner_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/Test.h"
#include <vector>
#include <string>
#include <ostream>
#if defined(POCO_VXWORKS)
#include <cstdarg>
#endif
#include "Poco/Exception.h"


namespace CppUnit {


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
 * TestRunner [-all] [-long] [-print] [-wait] ExampleTestCase
 *
 */
class CppUnit_API TestRunner
{
	using Mapping = std::pair<std::string, Test *>;
	using Mappings = std::vector<Mapping>;

public:
	TestRunner();
	TestRunner(std::ostream& ostr);
	~TestRunner();

	bool run(const std::vector<std::string>& args, const Test::Callback& callback = nullptr);
	void addTest(const std::string& name, Test* test);

protected:
	void printBanner();
	void print(const std::string& name, Test* pTest, int indent);
	Test* find(const std::string& name, Test* pTest, const std::string& testName);
	int collectAllTestCases(Test* pTest, std::vector<Test*>& tests);

private:
	std::ostream& _ostr;
	Mappings _mappings;
};


} // namespace CppUnit


#if defined(POCO_VXWORKS)
#define CppUnitMain(testCase) \
	int testCase##Runner(const char* arg0, ...) \
	{ \
		std::vector<std::string> args; \
		args.push_back(#testCase "Runner"); \
		args.push_back(std::string(arg0)); \
		va_list vargs; \
		va_start(vargs, arg0); \
		const char* arg = va_arg(vargs, const char*); \
		while (arg) \
		{ \
			args.push_back(std::string(arg)); \
			arg = va_arg(vargs, const char*); \
		} \
		va_end(vargs); \
		CppUnit::TestRunner runner; \
		runner.addTest(#testCase, testCase::suite()); \
		return runner.run(args) ? 0 : 1; \
	}
#else
#define CppUnitPocoExceptionText(exc) \
	CppUnit::Test::Callback exc = [] (const std::exception& ex) \
	{ \
		std::string text; \
		const Poco::Exception* pEx = dynamic_cast<const Poco::Exception*>(&ex); \
		if (pEx) text = pEx->displayText(); \
		else text = ex.what(); \
		return text; \
	}

#define CppUnitMain(testCase) \
	int main(int ac, char **av)							\
	{													\
		std::vector<std::string> args;					\
		for (int i = 0; i < ac; ++i)					\
			args.push_back(std::string(av[i]));			\
		CppUnit::TestRunner runner;						\
		runner.addTest(#testCase, testCase::suite());	\
		CppUnitPocoExceptionText(exc);					\
		return runner.run(args, exc) ? 0 : 1;			\
	}
#endif


#endif // CppUnit_TestRunner_INCLUDED
