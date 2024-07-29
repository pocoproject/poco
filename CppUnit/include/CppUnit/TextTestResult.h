//
// TextTestResult.h
//


#ifndef CppUnit_TextTestResult_INCLUDED
#define CppUnit_TextTestResult_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/TestResult.h"
#include <set>
#include <ostream>


namespace CppUnit {


class CppUnit_API TextTestResult: public TestResult
{
public:
	TextTestResult();
	TextTestResult(const std::string& ignore);
	TextTestResult(std::ostream& ostr);
	TextTestResult(std::ostream& ostr, const std::string& ignore);

	void addError(Test* test, CppUnitException* e) override;
	void addFailure(Test* test, CppUnitException* e) override;
	void startTest(Test* test) override;
	virtual void print(std::ostream& stream);
	virtual void printErrors(std::ostream& stream);
	virtual void printFailures(std::ostream& stream);
	virtual void printHeader(std::ostream& stream);

protected:
	std::string shortName(const std::string& testName);
	void setup();
	void ignoring(const std::string ignore);

private:
	std::ostream& _ostr;
	std::set<std::string> _ignored;
};


/* insertion operator for easy output */
inline std::ostream& operator<< (std::ostream& stream, TextTestResult& result)
{
	result.print(stream);
	return stream;
}


} // namespace CppUnit


#endif // CppUnit_TextTestResult_INCLUDED
