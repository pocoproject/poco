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
class StreamSocket;

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
	void testUnreadFixedLengthResponseReconnects();
	void testUnreadBufferedFixedLengthResponseKeepsConnection();
	void testStrayResponseAfterBodyReconnects();
	void testServerCloseAfterResponseReconnects();
	void testUnreadChunkedResponseReconnects();
	void testMalformedUnreadChunkReconnects();
	void testUnreadCloseDelimitedResponseReconnects();
	void testUnreadChunkedTerminatorKeepsConnection();
	void testProxyTunnelIgnoresPreviousResponse();
	void testMalformedContentLengthResponse();
	void testDuplicateContentLengthResponse();
	void testDuplicateTransferEncodingResponse();
	void testContentLengthAndTransferEncodingResponse();
	void testNonChunkedTransferEncodingResponseReadsToClose();
	void testHeadResponseIgnoresFramingHeaders();
	void testConnectResponseIgnoresFramingHeaders();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void assertNewConnectionServes(Poco::Net::HTTPClientSession& session, Poco::Net::ServerSocket& server, Poco::Net::HTTPRequest& request);
		/// Sends request on session and checks that it arrives on a new
		/// connection from server and that the response sent there is read.

	std::string receiveHeader(Poco::Net::StreamSocket& peer);
		/// Returns the bytes received from peer up to and including the empty line
		/// that ends a request header. Fails if it does not arrive within 5 seconds.

	void assertReconnectAfterPartialRead(const std::string& firstResponse, std::streamsize bodyBytes);
		/// Reads bodyBytes of the body of firstResponse and checks
		/// that the next request is sent on a new connection without
		/// waiting for the rest of the body.

	void assertFramingRejected(const std::string& headers);
		/// Checks that a response with the given header fields is
		/// refused with a MessageException, that the session is closed,
		/// and that the next request is sent on a new connection.

	void assertFramingIgnored(const std::string& method, const std::string& uri);
		/// Checks that a 200 response with two Content-Length headers and
		/// a Transfer-Encoding header to a request with the given method
		/// is accepted with an empty body.
};


#endif // HTTPClientSessionTest_INCLUDED
