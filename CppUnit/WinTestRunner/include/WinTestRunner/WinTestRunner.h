//
// WinTestRunner.h
//
// $Id: //poco/1.1.0/CppUnit/WinTestRunner/include/WinTestRunner/WinTestRunner.h#1 $
//
// Application shell for CppUnit's TestRunner dialog.
//


#ifndef WinTestRunner_H_INCLUDED
#define WinTestRunner_H_INCLUDED


#if defined(WinTestRunner_EXPORTS)
	#define WinTestRunner_API __declspec(dllexport)
#else
	#define WinTestRunner_API __declspec(dllimport)
#endif


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef STD_VECTOR_INCLUDED
#include <vector>
#define STD_VECTOR_INCLUDED
#endif
#include <afxwin.h>


CppUnit_BEGIN


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


CppUnit_END


#endif // WinTestRunner_H_INCLUDED

