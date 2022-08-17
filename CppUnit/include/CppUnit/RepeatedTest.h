//
// RepeatedTest.h
//


#ifndef CppUnit_RepeatedTest_INCLUDED
#define CppUnit_RepeatedTest_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/Guards.h"
#include "CppUnit/TestDecorator.h"


namespace CppUnit {


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
	void run(TestResult* result, const Test::Callback& callback = nullptr);

private:
	const int _timesRepeat;
};

} // namespace CppUnit


#endif // CppUnit_RepeatedTest_INCLUDED
