//
// TestCaller.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TestCaller.h#1 $
//


#ifndef CppUnit_TestCaller_INCLUDED
#define CppUnit_TestCaller_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_Guards_INCLUDED
#include "Guards.h"
#endif
#ifndef CppUnit_TestCase_INCLUDED
#include "TestCase.h"
#endif
#ifndef STD_MEMORY_INCLUDED
#include <memory>
#define STD_MEMORY_INCLUDED
#endif


CppUnit_BEGIN


/*
 * A test caller provides access to a test case method
 * on a test case class.  Test callers are useful when
 * you want to run an individual test or add it to a
 * suite.
 *
 * Here is an example:
 *
 * class MathTest : public TestCase {
 *         ...
 *     public:
 *         void         setUp ();
 *         void         tearDown ();
 *
 *         void         testAdd ();
 *         void         testSubtract ();
 * };
 *
 * Test *MathTest::suite () {
 *     TestSuite *suite = new TestSuite;
 *
 *     suite->addTest (new TestCaller<MathTest> ("testAdd", testAdd));
 *     return suite;
 * }
 *
 * You can use a TestCaller to bind any test method on a TestCase
 * class, as long as it returns accepts void and returns void.
 *
 * See TestCase
 */
template <class Fixture>
class TestCaller: public TestCase
{
	REFERENCEOBJECT (TestCaller)

	typedef void (Fixture::*TestMethod)();

public:
	TestCaller(const std::string& name, TestMethod test): TestCase(name), _fixture(new Fixture(name)), _test(test)
	{
	}

protected:
	void runTest()
	{
		(_fixture.get()->*_test)();
	}

	void setUp()
	{
		_fixture.get()->setUp();
	}

	void tearDown()
	{
		_fixture.get()->tearDown();
	}

private:
	TestMethod             _test;
	std::auto_ptr<Fixture> _fixture;
};


CppUnit_END


#define CppUnit_addTest(suite, cls, mth) \
	suite->addTest(new CppUnit::TestCaller<cls>(#mth, &cls::mth))


#endif // CppUnit_TestCaller_INCLUDED
