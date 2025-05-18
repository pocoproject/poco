#include "HTTPReactorServerTestSuite.h"
#include "CppUnit/TestSuite.h"
#include "HTTPReactorServerSessionTest.h"


CppUnit::Test* HTTPReactorServerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPReactorServerTestSuite");

	pSuite->addTest(HTTPReactorServerSessionTest::suite());

	return pSuite;
}