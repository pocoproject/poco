//
// WinTestRunner.cpp
//
// $Id: //poco/1.3/CppUnit/WinTestRunner/src/WinTestRunner.cpp#1 $
//


#include "WinTestRunner/WinTestRunner.h"
#include "TestRunnerDlg.h"


namespace CppUnit {


WinTestRunner::WinTestRunner()
{
}


WinTestRunner::~WinTestRunner()
{
	for (std::vector<Test*>::iterator it = _tests.begin(); it != _tests.end(); ++it)
		delete *it;
}


void WinTestRunner::run()
{
	TestRunnerDlg dlg;

	dlg.setTests(_tests);
	dlg.DoModal();
}


void WinTestRunner::addTest(Test* pTest)
{
	_tests.push_back(pTest);
}


BEGIN_MESSAGE_MAP(WinTestRunnerApp, CWinApp)
END_MESSAGE_MAP()


BOOL WinTestRunnerApp::InitInstance()
{
	AllocConsole();
	SetConsoleTitle("CppUnit WinTestRunner Console");
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);
	TestMain();
	FreeConsole();
	return FALSE;
}


void WinTestRunnerApp::TestMain()
{
}


} // namespace CppUnit
