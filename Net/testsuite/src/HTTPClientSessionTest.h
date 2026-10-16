//
// HTTPClientSessionTest.h
//
// Definition of the HTTPClientSessionTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPClientSessionTest_INCLUDED
#define HTTPClientSessionTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


namespace Poco::Net {

class HTTPClientSession;
class HTTPRequest;
class ServerSocket;

} // namespace Poco::Net


class HTTPClientSessionTest: public CppUnit::TestCase
{
public:
	HTTPClientSessionTest(const std::string& name);
	~HTTPClientSessionTest();

	void testGetSmall();
	void testGetSmallUnix();
	void testGetLarge();
	void testHead();
	void testPostSmallIdentity();
	void testPostLargeIdentity();
	void testPostSmallChunked();
	void testPostLargeChunked();
	void testKeepAlive();
	void testTrailer();
	void testProxy();
	void testProxyAuth();
	void testBypassProxy();
	void testExpectContinue();
	void testExpectContinueFail();
	void testProxyConfig();
	void testSetProxyProtocolValidation();
	void testSetProxyConfigValidation();
	void testProxySetters();
	void testProxyRequestPrefix();
	void testProxyNonTunnel();
	void testGlobalProxyConfig();
	void testBypassProxyExtended();
	void testChunkedFramingErrorReconnects();
	void testInvalidRequestKeepsConnection();
	void testIncompleteFixedLengthResponse();
	void testCompleteFixedLengthResponse();
	void testFixedLengthResponseInPartsNonBlocking();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void assertNewConnectionServes(Poco::Net::HTTPClientSession& session, Poco::Net::ServerSocket& server, Poco::Net::HTTPRequest& request);
		/// Sends request on session and checks that it arrives on a new
		/// connection from server and that the response sent there is read.
};


#endif // HTTPClientSessionTest_INCLUDED
