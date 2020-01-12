#ifndef WinServiceTest_INCLUDED
#define WinServiceTest_INCLUDED


#include "Poco/Util/Util.h"
#include "CppUnit/TestCase.h"
#include "Poco/Util/WinService.h"


class WinServiceTest : public CppUnit::TestCase {
public:
	WinServiceTest(const std::string& name);
	~WinServiceTest();

	void testServiceCouldCreatedWithExistingConnection();
	void testServiceReturnsTrueIfStopped();
	void testServiceReturnsFailureActionConfigured();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // WinServiceTest_INCLUDED
