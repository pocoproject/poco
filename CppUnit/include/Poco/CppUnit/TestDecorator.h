//
// TestDecorator.h
//
// $Id: //poco/1.4/CppUnit/include/CppUnit/TestDecorator.h#1 $
//


#ifndef Poco_CppUnit_TestDecorator_INCLUDED
#define Poco_CppUnit_TestDecorator_INCLUDED


#include "Poco/CppUnit/CppUnit.h"
#include "Poco/CppUnit/Guards.h"
#include "Poco/CppUnit/Test.h"


namespace CppUnit {


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


} // namespace CppUnit


#endif // Poco_CppUnit_TestDecorator_INCLUDED
