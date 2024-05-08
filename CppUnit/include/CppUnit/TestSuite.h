//
// TestSuite.h
//


#ifndef CppUnit_TestSuite_INCLUDED
#define CppUnit_TestSuite_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/Guards.h"
#include "CppUnit/Test.h"
#include <vector>
#include <string>


namespace CppUnit {


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
	~TestSuite() override;

	void run(TestResult* result, const Test::Callback& callback = nullptr) override;
	int countTestCases() const override;
	void addTest(Test* test);
	std::string toString() const override;
	Test::Type getType() const override;

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
inline std::string TestSuite::toString() const
{
	return "suite " + _name;
}

// Returns the type of the test, see Test::Type
inline Test::Type TestSuite::getType() const
{
	return Test::Suite;
}

// Returns all tests
inline const std::vector<Test*> TestSuite::tests() const
{
	return _tests;
}


} // namespace CppUnit


#endif // CppUnit_TestSuite_INCLUDED
