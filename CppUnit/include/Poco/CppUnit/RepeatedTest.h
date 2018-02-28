//
// RepeatedTest.h
//


#ifndef Poco_CppUnit_RepeatedTest_INCLUDED
#define Poco_CppUnit_RepeatedTest_INCLUDED


#include "Poco/CppUnit/CppUnit.h"
#include "Poco/CppUnit/Guards.h"
#include "Poco/CppUnit/TestDecorator.h"
#include "CppUnit/TestResult.h"


namespace CppUnit {


class Test;


/*
 * A decorator that runs a test repeatedly.
 * Does not assume ownership of the test it decorates
 *
 */
class CppUnit_API RepeatedTest: public TestDecorator
{
	REFERENCEOBJECT (RepeatedTest)

public:
	RepeatedTest(Test* test, int timesRepeat): TestDecorator (test), _timesRepeat (timesRepeat)
	{
	}

	int countTestCases();
	std::string toString();
	void run(TestResult *result);

private:
	const int _timesRepeat;
};


// Counts the number of test cases that will be run by this test.
inline int RepeatedTest::countTestCases ()
{
	return TestDecorator::countTestCases() * _timesRepeat;
}


// Returns the name of the test instance.
inline std::string RepeatedTest::toString()
{
	return TestDecorator::toString() + " (repeated)";
}


// Runs a repeated test
inline void RepeatedTest::run(TestResult *result)
{
	for (int n = 0; n < _timesRepeat; n++)
	{
		if (result->shouldStop())
			break;

		TestDecorator::run(result);
	}
}


} // namespace CppUnit


#endif // Poco_CppUnit_RepeatedTest_INCLUDED
