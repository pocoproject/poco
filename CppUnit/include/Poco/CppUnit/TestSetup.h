//
// TestSetup.h
//
// $Id: //poco/1.4/CppUnit/include/CppUnit/TestSetup.h#1 $
//


#ifndef Poco_CppUnit_TestSetup_INCLUDED
#define Poco_CppUnit_TestSetup_INCLUDED


#include "Poco/CppUnit/CppUnit.h"
#include "Poco/CppUnit/Guards.h"
#include "Poco/CppUnit/TestDecorator.h"


namespace CppUnit {


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


} // namespace CppUnit


#endif // Poco_CppUnit_TestSetup_INCLUDED
