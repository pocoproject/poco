#include "HTTPReactorServerTestSuite.h"
#include "CppUnit/TestSuite.h"
#include "HTTPReactorServerSessionTest.h"
#include "HTTPReactorServerTest.h"


CppUnit::Test* HTTPReactorServerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPReactorServerTestSuite");

	pSuite->addTest(HTTPReactorServerSessionTest::suite());
	pSuite->addTest(HTTPReactorServerTest::suite());

	return pSuite;
}
