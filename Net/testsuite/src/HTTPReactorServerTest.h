#ifndef HTTPReactorServerTest_INCLUDED
#define HTTPReactorServerTest_INCLUDED

#include "CppUnit/TestCase.h"

class HTTPReactorServerTest: public CppUnit::TestCase
{
public:
	HTTPReactorServerTest(const std::string& name);
	~HTTPReactorServerTest();

	void testBodyWithContentLength();
	void testBodyWithChunkedTransferEncoding();
	void testBodyReadTwice();
	void testMultipleRequests();
	void testKeepAliveRequests();
	void testStickyBufferWithBody();
	void testEmptyBody();
	void testLargeBody();
	void testChunkedMultipleChunks();
	void testMixedRequests();
	void testContentLengthZero();
	void testChunkedWithEmptyChunks();
	void testConcurrentRequests();
	void testUseSelfReactor();
	void testNotImplementedResponseWithKeepAlive();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};

#endif // HTTPReactorServerTest_INCLUDED
