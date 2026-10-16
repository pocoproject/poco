//
// HTTPServerTest.h
//
// Definition of the HTTPServerTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPServerTest_INCLUDED
#define HTTPServerTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class HTTPServerTest: public CppUnit::TestCase
{
public:
	HTTPServerTest(const std::string& name);
	~HTTPServerTest();

	void testIdentityRequest();
	void testFramingConflicts();
	void testPutIdentityRequest();
	void testChunkedRequest();
	void testIdentityRequestKeepAlive();
	void testChunkedRequestKeepAlive();
	void testMaxKeepAlive();
	void testKeepAliveTimeout();
	void test100Continue();
	void testRedirect();
	void testAuth();
	void testNotImpl();
	void testBuffer();
	void testFile();
	void testFileKeepAlive();
	void testEmptyFileKeepAlive();
	void testChunkedTrailer();
	void testChunkedFramingError();
	void testUnreadRequestBody();
	void testUnreadChunkedRequestBody();
	void testLargeUnreadRequestBody();
	void testResponseBeforeUnreadRequestBody();
	void testNonBlockingHandlerClosesConnection();
	void testHandlerNonBlockingReadThenBlocking();
	void testIncompleteFixedLengthRequest();
	void testSocketTimeouts();
	void testFieldNameWhitespace();
	void testRedirectLineBreak();
	void testSendLineBreak();
	void testEchoDecodedLineBreak();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // HTTPServerTest_INCLUDED
