//
// TestSetup.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TestSetup.h#1 $
//


#ifndef CppUnit_TestSetup_INCLUDED
#define CppUnit_TestSetup_INCLUDED


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


class CppUnit_API TestSetup: public TestDecorator
{
	REFERENCEOBJECT (TestSetup)

public:
	TestSetup(Test* test): TestDecorator(test) 
	{
	}
	
	void run(TestResult* result);

protected:
	void setUp() 
	{
	}
	
	void tearDown()
	{
	}
};


inline void TestSetup::run(TestResult* result)
{
	setUp();
	TestDecorator::run(result); 
	tearDown();
}


CppUnit_END


#endif // CppUnit_TestSetup_INCLUDED
