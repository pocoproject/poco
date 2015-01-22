//
// HTTPClientTest.h
//
// $Id: //poco/1.4/Net/testsuite/src/HTTPClientTest.h#1 $
//
// Definition of the HTTPClientTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPClientTest_INCLUDED
#define HTTPClientTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPEventArgs.h"
#include "CppUnit/TestCase.h"


class HTTPClientTest: public CppUnit::TestCase
{
public:
	HTTPClientTest(const std::string& name);
	~HTTPClientTest();

	void testGetSmall();
	void testGetLarge();
	void testHead();
	void testPostSmallIdentity();
	void testPostLargeIdentity();
	void testPostSmallChunked();
	void testPostLargeChunked();
	void testPostSmallClose();
	void testPostLargeClose();
	void testKeepAlive();
	void testProxy();
	void testProxyAuth();
	void testBypassProxy();

	void setUp();
	void tearDown();

	void HTTPClientTest::onResponse(const void* pSender, Poco::Net::HTTPEventArgs& args);
	void HTTPClientTest::onError(const void* pSender, Poco::Net::HTTPEventArgs& args);

	static CppUnit::Test* suite();

private:
	std::string _body;
	std::string _contentType;
	int         _contentLength;
	bool        _chunked;
	bool        _keepAlive;
	std::string _error;
	bool        _done;
};


#endif // HTTPClientTest_INCLUDED
