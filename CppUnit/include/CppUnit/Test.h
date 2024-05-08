//
// Test.h
//


#ifndef CppUnit_Test_INCLUDED
#define CppUnit_Test_INCLUDED


#include "CppUnit/CppUnit.h"
#include <string>
#include <vector>
#include <functional>


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
		Suite, // Only set on CppUnit::TestSuite
		Normal, // Default TestCase always run
		Long // Such TestCase will only be run if the `-long` command line argument is set
	};

public:
	using Callback = std::function<std::string(const std::exception&)>;

	virtual ~Test() = 0;
	virtual void run(TestResult* result, const Callback& callback = nullptr) = 0;
	virtual int countTestCases() const = 0;
	virtual std::string toString() const = 0;
	virtual Test::Type getType() const = 0;

	void addSetup(const std::vector<std::string>& setup);
	const std::vector<std::string>& setup() const;

private:
	std::vector<std::string> _setup;
};


inline Test::~Test() = default;


// Runs a test and collects its result in a TestResult instance.
inline void Test::run(TestResult*, const Callback&)
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
	return Test::Normal;
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
