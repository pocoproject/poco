//
// TestSuite.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TestSuite.h#1 $
//


#ifndef CppUnit_TestSuite_INCLUDED
#define CppUnit_TestSuite_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_Guards_INCLUDED
#include "CppUnit/Guards.h"
#endif
#ifndef CppUnit_Test_INCLUDED
#include "CppUnit/Test.h"
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


class TestResult;


/*
 * A TestSuite is a Composite of Tests.
 * It runs a collection of test cases. Here is an example.
 *
 * TestSuite *suite= new TestSuite();
 * suite->addTest(new TestCaller<MathTest> ("testAdd", testAdd));
 * suite->addTest(new TestCaller<MathTest> ("testDivideByZero", testDivideByZero));
 *
 * Note that TestSuites assume lifetime
 * control for any tests added to them.
 *
 * see Test and TestCaller
 */
class CppUnit_API TestSuite: public Test
{
	REFERENCEOBJECT (TestSuite)

public:
	TestSuite(const std::string& name = "");
	~TestSuite();

	void run(TestResult* result);
	int countTestCases();
	void addTest(Test* test);
	std::string toString();

	virtual void deleteContents();
	
	const std::vector<Test*> tests() const;

private:
	std::vector<Test*> _tests;
	const std::string _name;
};


// Default constructor
inline TestSuite::TestSuite(const std::string& name): _name(name)
{
}


// Destructor
inline TestSuite::~TestSuite()
{
	deleteContents();
}


// Adds a test to the suite.
inline void TestSuite::addTest(Test* test)
{
	_tests.push_back(test);
}


// Returns a std::string representation of the test suite.
inline std::string TestSuite::toString()
{
	return "suite " + _name; 
}


// Returns all tests
inline const std::vector<Test*> TestSuite::tests() const
{
	return _tests;
}


CppUnit_END


#endif // CppUnit_TestSuite_INCLUDED
