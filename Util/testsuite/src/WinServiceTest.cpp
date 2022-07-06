#include "WinServiceTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


using Poco::Util::WinService;


WinServiceTest::WinServiceTest(const std::string& name) : CppUnit::TestCase(name)
{
}


WinServiceTest::~WinServiceTest()
{
}


void WinServiceTest::testServiceCouldCreatedWithExistingConnection()
{
	SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	assertTrue(scmHandle);

	WinService spoolerService{scmHandle, "Spooler"};

	assertTrue(spoolerService.isRegistered());
}


void WinServiceTest::testServiceReturnsTrueIfStopped()
{
	WinService spoolerService{"Spooler"};

	spoolerService.stop();
	assertTrue(spoolerService.isStopped());
}


void WinServiceTest::testServiceReturnsFailureActionConfigured()
{
	WinService spoolerService{"Spooler"};

	auto failureActions = spoolerService.getFailureActions();
	assertEqual(3, static_cast<int>(failureActions.size()));

	assertTrue(WinService::SVC_RESTART == failureActions[0]);
	assertTrue(WinService::SVC_RESTART == failureActions[1]);
	assertTrue(WinService::SVC_NONE == failureActions[2]);
}


void WinServiceTest::setUp()
{
}


void WinServiceTest::tearDown()
{
	try
	{
		WinService spoolerService{"Spooler"};

		if (spoolerService.isStopped())
		{
			spoolerService.start();
		}
	}
	catch (...)
	{
	}
}


CppUnit::Test* WinServiceTest::suite() {
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WinServiceTest");

	CppUnit_addTest(pSuite, WinServiceTest, testServiceCouldCreatedWithExistingConnection);
	CppUnit_addTest(pSuite, WinServiceTest, testServiceReturnsTrueIfStopped);
	CppUnit_addTest(pSuite, WinServiceTest, testServiceReturnsFailureActionConfigured);

	return pSuite;
}
