//
// Test.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/Test.h#1 $
//


#ifndef CppUnit_Test_INCLUDED
#define CppUnit_Test_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
#define STD_STRING_INCLUDED
#endif


CppUnit_BEGIN


class TestResult;


/*
 * A Test can be run and collect its results.
 * See TestResult.
 *
 */
class CppUnit_API Test
{
public:
	virtual ~Test() = 0;
	virtual void run(TestResult* result) = 0;
	virtual int countTestCases() = 0;
	virtual std::string toString() = 0;
};


inline Test::~Test()
{
}


// Runs a test and collects its result in a TestResult instance.
inline void Test::run(TestResult *result)
{
}


// Counts the number of test cases that will be run by this test.
inline int Test::countTestCases()
{
	return 0; 
}


// Returns the name of the test instance.
inline std::string Test::toString()
{
	return "";
}


CppUnit_END


#endif // CppUnit_Test_INCLUDED
