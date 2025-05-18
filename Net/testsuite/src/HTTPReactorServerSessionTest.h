#ifndef HTTPReactorServerSessionTest_INCLUDED
#define HTTPReactorServerSessionTest_INCLUDED

#include "CppUnit/TestCase.h"


class HTTPReactorServerSessionTest: public CppUnit::TestCase
{
public:
	HTTPReactorServerSessionTest(const std::string& name);
	~HTTPReactorServerSessionTest();

	void testCheckRequestComplete();
	void testCheckRequestCompleteChunked();
	void testCheckRequestCompleteChunkedWithTrailer();
	void testCheckRequestCompleteChunkedWithTrailerAndBody();
	void testCheckRequestCompleteChunkedWithBody();
	void testCheckRequestCompleteChunkedWithStickyBody();


	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};



#endif // HTTPReactorServerSessionTest.h_INCLUDED