//
// Test.h
//


#ifndef CppUnit_Test_INCLUDED
#define CppUnit_Test_INCLUDED


#include "CppUnit/CppUnit.h"
#include <string>
#include <vector>


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
	virtual ~Test() = 0;
	virtual void run(TestResult* result) = 0;
	virtual int countTestCases() = 0;
	virtual std::string toString() = 0;

	void addSetup(const std::vector<std::string>& setup);
	const std::vector<std::string>& setup() const;

private:
	std::vector<std::string>	_setup;
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


inline void Test::addSetup(const std::vector<std::string>& setup)
{
	_setup = setup;
}


inline const std::vector<std::string>& Test::setup() const
{
	return _setup;
}


} // namespace CppUnit


#endif // CppUnit_Test_INCLUDED
