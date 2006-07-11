//
// RepeatedTest.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/RepeatedTest.h#1 $
//


#ifndef CppUnit_RepeatedTest_INCLUDED
#define CppUnit_RepeatedTest_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_Guards_INCLUDED
#include "CppUnit/Guards.h"
#endif
#ifndef CppUnit_TestDecorator_INCLUDED
#include "CppUnit/TestDecorator.h"
#endif


CppUnit_BEGIN


class Test;
class TestResult;


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
inline RepeatedTest::countTestCases ()
{
	return TestDecorator::countTestCases () * _timesRepeat;
}


// Returns the name of the test instance.
inline std::string RepeatedTest::toString()
{
	return TestDecorator::toString () + " (repeated)";
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


CppUnit_END


#endif // CppUnit_RepeatedTest_INCLUDED
