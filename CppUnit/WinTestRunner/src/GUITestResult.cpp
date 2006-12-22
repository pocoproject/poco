//
// GUITestResult.cpp
//
// $Id: //poco/Main/CppUnit/WinTestRunner/src/GUITestResult.cpp#6 $
//


#include "TestRunnerDlg.h"
#include "GUITestResult.h"


namespace CppUnit {


void GUITestResult::addError(Test *test, CppUnitException *e)
{
    ExclusiveZone zone(_syncObject);

    TestResult::addError(test, e);
    _runner->addError(this, test, e);
}


void GUITestResult::addFailure(Test *test, CppUnitException *e)
{
    ExclusiveZone zone(_syncObject);

    TestResult::addFailure(test, e);
    _runner->addFailure(this, test, e);
}


void GUITestResult::endTest(Test *test)
{
    ExclusiveZone zone(_syncObject);

    TestResult::endTest(test);
    _runner->endTest(this, test);
}


} // namespace CppUnit

