//
// TestDecorator.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TestDecorator.h#1 $
//


#ifndef CppUnit_TestDecorator_INCLUDED
#define CppUnit_TestDecorator_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_Guards_INCLUDED
#include "CppUnit/Guards.h"
#endif
#ifndef CppUnit_Test_INCLUDED
#include "CppUnit/Test.h"
#endif


CppUnit_BEGIN


class TestResult;


/*
 * A Decorator for Tests
 *
 * Does not assume ownership of the test it decorates
 *
 */
class CppUnit_API TestDecorator: public Test
{
	REFERENCEOBJECT(TestDecorator)

public:
	TestDecorator(Test* test);

	virtual ~TestDecorator();

	int countTestCases();

	void run(TestResult* result);

	std::string toString();

protected:
	Test* _test;
};


CppUnit_END


#endif // CppUnit_TestDecorator_INCLUDED
