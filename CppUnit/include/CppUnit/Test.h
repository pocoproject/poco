//
// Test.h
//


#ifndef CppUnit_Test_INCLUDED
#define CppUnit_Test_INCLUDED


#include "CppUnit/CppUnit.h"
#include <string>


namespace CppUnit {


class TestResult;


/*
 * A Test can be run and collect its results.
 * See TestResult.
 *
 */
class CppUnit_API Test
{
public:
	enum Type {
		Normal, // Normal tests are always run
		Long // Long tests are run only if the -long option is specified
	};

public:
	virtual ~Test() = 0;
	virtual void run(TestResult* result) = 0;
	virtual int countTestCases() const = 0;
	virtual std::string toString() const = 0;
	virtual Test::Type getType() const = 0;
};


inline Test::~Test()
{
}


// Runs a test and collects its result in a TestResult instance.
inline void Test::run(TestResult *result)
{
}


// Counts the number of test cases that will be run by this test.
inline int Test::countTestCases() const
{
	return 0; 
}


// Returns the name of the test instance.
inline std::string Test::toString() const
{
	return "";
}

// Returns the type of the test, see Test::Type
inline Test::Type Test::getType() const
{
	return Test::Type::Normal;
}

} // namespace CppUnit


#endif // CppUnit_Test_INCLUDED
