//
// HTTPSClientTest.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/HTTPSClientTest.h#1 $
//
// Definition of the HTTPSClientTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPSClientTest_INCLUDED
#define HTTPSClientTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPEventArgs.h"
#include "CppUnit/TestCase.h"


class HTTPSClientTest: public CppUnit::TestCase
{
public:
	HTTPSClientTest(const std::string& name);
	~HTTPSClientTest();

	void onResponse(const void* pSender, Poco::Net::HTTPEventArgs& args);
	void onError(const void* pSender, Poco::Net::HTTPEventArgs& args);

	void testGetSmall();
	void testGetLarge();
	void testHead();
	void testPostSmallIdentity();
	void testPostLargeIdentity();
	void testPostSmallChunked();
	void testPostLargeChunked();
	/*
	void testPostLargeChunkedKeepAlive();
	void testKeepAlive();
	void testInterop();
	void testProxy();
	void testCachedSession();
	void testUnknownContentLength();
	void testServerAbort();
*/

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	typedef Poco::Net::HTTPResponse::HTTPStatus Status;

	std::string _body;
	std::string _contentType;
	int         _contentLength;
	bool        _chunked;
	bool        _keepAlive;
	Status      _error;
	bool        _done;
};


#endif // HTTPSClientTest_INCLUDED
