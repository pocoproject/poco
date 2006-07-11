//
// TextTestResult.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/TextTestResult.h#1 $
//


#ifndef CppUnit_TextTestResult_INCLUDED
#define CppUnit_TextTestResult_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef CppUnit_TestResult_INCLUDED
#include "CppUnit/TestResult.h"
#endif
#ifndef STD_OSTREAM_INCLUDED
#include <ostream>
#define STD_OSTREAM_INCLUDED
#endif


CppUnit_BEGIN


class CppUnit_API TextTestResult: public TestResult
{
public:
	virtual void addError(Test* test, CppUnitException* e);
	virtual void addFailure(Test* test, CppUnitException* e);
	virtual void startTest(Test* test);
	virtual void print(std::ostream& stream);
	virtual void printErrors(std::ostream& stream);
	virtual void printFailures(std::ostream& stream);
	virtual void printHeader(std::ostream& stream);
	
protected:
	std::string shortName(const std::string& testName);
};


/* insertion operator for easy output */
inline std::ostream& operator<< (std::ostream& stream, TextTestResult& result)
{
	result.print(stream);
	return stream;
}


CppUnit_END


#endif // CppUnit_TextTestResult_INCLUDED
