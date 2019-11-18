#include "WinServiceTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/WinService.h"

using Poco::Util::WinService;


WinServiceTest::WinServiceTest(const std::string& name) : CppUnit::TestCase(name) {
}


WinServiceTest::~WinServiceTest() {
}


void WinServiceTest::testServiceCouldCreatedWithExistingConnection() {

	SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	assertTrue(scmHandle);

	WinService spoolerService{ scmHandle, "Spooler" };

	assertTrue(spoolerService.isRegistered());	
}


void WinServiceTest::setUp() {
}


void WinServiceTest::tearDown() {
}


CppUnit::Test* WinServiceTest::suite() {
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WinServiceTest");

	CppUnit_addTest(pSuite, WinServiceTest, testServiceCouldCreatedWithExistingConnection);

	return pSuite;
}
