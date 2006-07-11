//
// TestRunnerDlg.h
//
// $Id: //poco/1.1.0/CppUnit/WinTestRunner/src/TestRunnerDlg.h#1 $
//


#ifndef TestRunnerDlg_INCLUDED
#define TestRunnerDlg_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_CppUnitException_INCLUDED
#include "CppUnit/CppUnitException.h"
#endif
#ifndef ActiveTest_INCLUDED
#include "ActiveTest.h"
#endif
#ifndef STD_VECTOR_INCLUDED
#include <vector>
#define STD_VECTOR_INCLUDED
#endif
#ifndef IDD_DIALOG_TESTRUNNER
#include "../res/Resource.h"
#endif
#include <afxwin.h>
#include "afxwin.h"


CppUnit_BEGIN


class ProgressBar;


class TestRunnerDlg: public CDialog
{
public:
	TestRunnerDlg(CWnd* pParent = NULL);
	~TestRunnerDlg();

    void setTests(const std::vector<Test*>& tests);

    void addError(TestResult* result, Test* test, CppUnitException* e);
    void addFailure(TestResult* result, Test* test, CppUnitException* e);
    void endTest(TestResult* result, Test* test);

    //{{AFX_DATA(TestRunnerDlg)
    enum { IDD = IDD_DIALOG_TESTRUNNER };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    //{{AFX_VIRTUAL(TestRunnerDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(TestRunnerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnRun();
    afx_msg void OnStop();
    virtual void OnOK();
    afx_msg void OnSelchangeComboTest();
	afx_msg void OnBnClickedAutorun();
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    void addListEntry(const std::string& type, TestResult* result, Test* test, CppUnitException* e);
    void beIdle();
    void beRunning();
    void beRunDisabled();
    void reset();
    void freeState();
    void updateCountsDisplay();
	void addTest(Test* pTest, int level);

	struct TestInfo
	{
		Test* pTest;
		int   level;
	};
    std::vector<TestInfo> _tests;
    ProgressBar* _testsProgress;
    Test*        _selectedTest;
    ActiveTest*  _activeTest;
    TestResult*  _result;
    int   _testsRun;
    int   _errors;
    int   _failures;
    DWORD _testStartTime;
    DWORD _testEndTime;
};


CppUnit_END


#endif // TestRunnerDlg_INCLUDED

