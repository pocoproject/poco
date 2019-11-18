#include "WinServiceTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

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

void WinServiceTest::testServiceReturnsTrueIfStopped() {
	spoolerService_.stop();
	assertTrue(spoolerService_.isStopped());
}

void WinServiceTest::testServiceReturnsFailureActionConfigured() {
	auto failureActions = spoolerService_.getFailureAction();
	assertEqual(3, failureActions.size());

	assertEqual(WinService::SVC_RESTART, failureActions[0]);
	assertEqual(WinService::SVC_RESTART, failureActions[1]);
	assertEqual(WinService::SVC_NONE, failureActions[2]);
}


void WinServiceTest::setUp() {
}


void WinServiceTest::tearDown() {
	if (spoolerService_.isStopped()) {
		spoolerService_.start();
	}
}


CppUnit::Test* WinServiceTest::suite() {
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WinServiceTest");

	CppUnit_addTest(pSuite, WinServiceTest, testServiceCouldCreatedWithExistingConnection);
	CppUnit_addTest(pSuite, WinServiceTest, testServiceReturnsTrueIfStopped);
	CppUnit_addTest(pSuite, WinServiceTest, testServiceReturnsFailureActionConfigured);

	return pSuite;
}
