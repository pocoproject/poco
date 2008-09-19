//
// WinTestRunner.h
//
// $Id: //poco/1.3/CppUnit/WinTestRunner/include/WinTestRunner/WinTestRunner.h#2 $
//
// Application shell for CppUnit's TestRunner dialog.
//


#ifndef WinTestRunner_H_INCLUDED
#define WinTestRunner_H_INCLUDED


#if !defined(POCO_STATIC)
#if defined(WinTestRunner_EXPORTS)
#define WinTestRunner_API __declspec(dllexport)
#else
#define WinTestRunner_API __declspec(dllimport)
#endif
#else
#define WinTestRunner_API
#endif


#include "CppUnit/CppUnit.h"
#include <vector>
#include <afxwin.h>


namespace CppUnit {


class Test;


class WinTestRunner_API WinTestRunner
{
public:
	WinTestRunner();
	~WinTestRunner();

	void run();
	void addTest(Test* pTest);
	
private:
	std::vector<Test*> _tests;
};


class WinTestRunner_API WinTestRunnerApp: public CWinApp
	/// A simple application class that hosts the TestRunner dialog.
	/// Create a subclass and override the TestMain() method.
{
public:
	virtual BOOL InitInstance();

	virtual void TestMain() = 0;

	DECLARE_MESSAGE_MAP()
};


} // namespace CppUnit


#endif // WinTestRunner_H_INCLUDED

