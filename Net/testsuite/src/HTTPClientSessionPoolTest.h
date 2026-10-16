//
// HTTPClientSessionPoolTest.h
//
// Definition of the HTTPClientSessionPoolTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPClientSessionPoolTest_INCLUDED
#define HTTPClientSessionPoolTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"
#include <istream>
#include <string>


namespace Poco::Net {

class HTTPClientSession;
class HTTPRequest;
class ServerSocket;
class StreamSocket;

} // namespace Poco::Net


class HTTPClientSessionPoolTest: public CppUnit::TestCase
{
public:
	explicit HTTPClientSessionPoolTest(const std::string& name);
	~HTTPClientSessionPoolTest() override;

	void testReuseAfterCompleteResponse();
	void testReuseAfterChunkedResponse();
	void testUnreadResponseNotKept();
	void testIncompleteRequestBodyNotKept();
	void testConnectionCloseNotKept();
	void testSwitchingProtocolsNotReused();
	void testConnectTunnelNotReused();
	void testSettingsRestored();
	void testChangedProxyNotKept();
	void testChangedHostNotKept();
	void testChangedProxyCredentialsNotKept();
	void testNTLMProxySessionNotKept();
	void testConnectionAuthenticatedSessionNotKept();
	void testClosedIdleConnectionNotReused();
	void testExpiredIdleSessionRemoved();
	void testExpiredIdleSessionOutOfOrderRemoved();
	void testIdleSessionLimit();
	void testConcurrentAcquire();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
	std::istream& exchange(Poco::Net::HTTPClientSession& session, Poco::Net::ServerSocket& server, Poco::Net::StreamSocket& peer, const std::string& response);
		/// Sends a GET request with session, accepts the connection into peer
		/// if session was not connected, answers with response and returns
		/// the response body stream.

	std::istream& exchange(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::ServerSocket& server, Poco::Net::StreamSocket& peer, const std::string& response);
		/// Sends request with session and continues as the overload above.
};


#endif // HTTPClientSessionPoolTest_INCLUDED
