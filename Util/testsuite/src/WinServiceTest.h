#ifndef WinServiceTest_INCLUDED
#define WinServiceTest_INCLUDED


#include "Poco/Util/Util.h"
#include "CppUnit/TestCase.h"


class WinServiceTest : public CppUnit::TestCase {
public:
	WinServiceTest(const std::string& name);
	~WinServiceTest();

	void testServiceCouldCreatedWithExistingConnection();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // WinServiceTest_INCLUDED
