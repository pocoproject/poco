//
// HTTPCredentialsTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPCredentialsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/HTTPDigestCredentials.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"


using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPBasicCredentials;
using Poco::Net::HTTPAuthenticationParams;
using Poco::Net::HTTPDigestCredentials;
using Poco::Net::HTTPCredentials;
using Poco::Net::NotAuthenticatedException;


HTTPCredentialsTest::HTTPCredentialsTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPCredentialsTest::~HTTPCredentialsTest()
{
}


void HTTPCredentialsTest::testBasicCredentials()
{
	HTTPRequest request;
	assertTrue (!request.hasCredentials());

	HTTPBasicCredentials cred("user", "secret");
	cred.authenticate(request);
	assertTrue (request.hasCredentials());
	std::string scheme;
	std::string info;
	request.getCredentials(scheme, info);
	assertTrue (scheme == "Basic");
	assertTrue (info == "dXNlcjpzZWNyZXQ=");

	HTTPBasicCredentials cred2(request);
	assertTrue (cred2.getUsername() == "user");
	assertTrue (cred2.getPassword() == "secret");
}


void HTTPCredentialsTest::testProxyBasicCredentials()
{
	HTTPRequest request;
	assertTrue (!request.hasProxyCredentials());

	HTTPBasicCredentials cred("user", "secret");
	cred.proxyAuthenticate(request);
	assertTrue (request.hasProxyCredentials());
	std::string scheme;
	std::string info;
	request.getProxyCredentials(scheme, info);
	assertTrue (scheme == "Basic");
	assertTrue (info == "dXNlcjpzZWNyZXQ=");
}


void HTTPCredentialsTest::testBadCredentials()
{
	HTTPRequest request;

	std::string scheme;
	std::string info;
	try
	{
		request.getCredentials(scheme, info);
		fail("no credentials - must throw");
	}
	catch (NotAuthenticatedException&)
	{
	}

	request.setCredentials("Test", "SomeData");
	request.getCredentials(scheme, info);
	assertTrue (scheme == "Test");
	assertTrue (info == "SomeData");

	try
	{
		HTTPBasicCredentials cred(request);
		fail("bad scheme - must throw");
	}
	catch (NotAuthenticatedException&)
	{
	}
}


void HTTPCredentialsTest::testAuthenticationParams()
{
	const std::string authInfo("nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", response=\"40e4889cfbd0e561f71e3107a2863bc4\", uri=\"/digest/\", username=\"user\"");
	HTTPAuthenticationParams params(authInfo);

	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] == "40e4889cfbd0e561f71e3107a2863bc4");
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params.size() == 5);
	assertTrue (params.toString() == authInfo);

	params.clear();
	HTTPRequest request;
	request.set("Authorization", "Digest " + authInfo);
	params.fromRequest(request);

	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] == "40e4889cfbd0e561f71e3107a2863bc4");
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params.size() == 5);

	params.clear();
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	params.fromResponse(response);

	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params.size() == 2);

	params.clear();
	response.set("WWW-Authenticate", "NTLM TlRMTVNTUAACAAAADAAMADAAAAABAoEAASNFZ4mrze8AAAAAAAAAAGIAYgA8AAAARABPAE0AQQBJAE4AAgAMAEQATwBNAEEASQBOAAEADABTAEUAUgBWAEUAUgAEABQAZABvAG0AYQBpAG4ALgBjAG8AbQADACIAcwBlAHIAdgBlAHIALgBkAG8AbQBhAGkAbgAuAGMAbwBtAAAAAAA");
	params.fromResponse(response);

	assertTrue (params["NTLM"] == "TlRMTVNTUAACAAAADAAMADAAAAABAoEAASNFZ4mrze8AAAAAAAAAAGIAYgA8AAAARABPAE0AQQBJAE4AAgAMAEQATwBNAEEASQBOAAEADABTAEUAUgBWAEUAUgAEABQAZABvAG0AYQBpAG4ALgBjAG8AbQADACIAcwBlAHIAdgBlAHIALgBkAG8AbQBhAGkAbgAuAGMAbwBtAAAAAAA");
	assertTrue (params.size() == 1);
}


void HTTPCredentialsTest::testAuthenticationParamsMultipleHeaders()
{
	HTTPResponse response;
	response.add("WWW-Authenticate", "Unsupported realm=\"TestUnsupported\"");
	response.add("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	HTTPAuthenticationParams params(response);

	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params.size() == 2);
}


void HTTPCredentialsTest::testDigestCredentials()
{
	HTTPDigestCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	creds.authenticate(request, response);
	std::string auth = request.get("Authorization");
	assertTrue (auth == "Digest username=\"user\", nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", uri=\"/digest/\", response=\"40e4889cfbd0e561f71e3107a2863bc4\"");
}


void HTTPCredentialsTest::testDigestCredentialsQoP()
{
	HTTPDigestCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth,auth-int\"");
	creds.authenticate(request, response);

	HTTPAuthenticationParams params(request);
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] != "40e4889cfbd0e561f71e3107a2863bc4");
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params["opaque"] == "opaque");
	assertTrue (params["cnonce"] != "");
	assertTrue (params["nc"] == "00000001");
	assertTrue (params["qop"] == "auth");
	assertTrue (params.size() == 9);

	std::string cnonce = params["cnonce"];
	std::string aresp = params["response"];

	params.clear();

	creds.updateAuthInfo(request);
	params.fromRequest(request);
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] != aresp);
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params["opaque"] == "opaque");
	assertTrue (params["cnonce"] == cnonce);
	assertTrue (params["nc"] == "00000002");
	assertTrue (params["qop"] == "auth");
	assertTrue (params.size() == 9);
}

void HTTPCredentialsTest::testDigestCredentialsQoPSHA256()
{
	HTTPDigestCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth,auth-int\", algorithm=SHA-256");
	creds.authenticate(request, response);
	
	HTTPAuthenticationParams params(request);
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] != "40e4889cfbd0e561f71e3107a2863bc4");
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params["opaque"] == "opaque");
	assertTrue (params["cnonce"] != "");
	assertTrue (params["nc"] == "00000001");
	assertTrue (params["qop"] == "auth");
	assertTrue (params["algorithm"] == "SHA-256");
	assertTrue (params.size() == 10);

	std::string cnonce = params["cnonce"];
	std::string aresp = params["response"];

	params.clear();

	creds.updateAuthInfo(request);
	params.fromRequest(request);
	assertTrue (params["nonce"] == "212573bb90170538efad012978ab811f%lu");
	assertTrue (params["realm"] == "TestDigest");
	assertTrue (params["response"] != aresp);
	assertTrue (params["uri"] == "/digest/");
	assertTrue (params["username"] == "user");
	assertTrue (params["opaque"] == "opaque");
	assertTrue (params["cnonce"] == cnonce);
	assertTrue (params["nc"] == "00000002");
	assertTrue (params["qop"] == "auth");
	assertTrue (params.size() == 10);
}

void HTTPCredentialsTest::testCredentialsBasic()
{
	HTTPCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/basic/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Basic realm=\"TestBasic\"");
	creds.authenticate(request, response);
	assertTrue (request.get("Authorization") == "Basic dXNlcjpzM2NyM3Q=");
}


void HTTPCredentialsTest::testProxyCredentialsBasic()
{
	HTTPCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/basic/");
	HTTPResponse response;
	response.set("Proxy-Authenticate", "Basic realm=\"TestBasic\"");
	creds.proxyAuthenticate(request, response);
	assertTrue (request.get("Proxy-Authorization") == "Basic dXNlcjpzM2NyM3Q=");
}


void HTTPCredentialsTest::testCredentialsDigest()
{
	HTTPCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	creds.authenticate(request, response);
	std::string auth = request.get("Authorization");
	assertTrue (auth == "Digest username=\"user\", nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", uri=\"/digest/\", response=\"40e4889cfbd0e561f71e3107a2863bc4\"");
}


void HTTPCredentialsTest::testCredentialsDigestMultipleHeaders()
{
	HTTPCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.add("WWW-Authenticate", "Unsupported realm=\"TestUnsupported\"");
	response.add("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	creds.authenticate(request, response);
	std::string auth = request.get("Authorization");
	assertTrue (auth == "Digest username=\"user\", nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", uri=\"/digest/\", response=\"40e4889cfbd0e561f71e3107a2863bc4\"");
}


void HTTPCredentialsTest::testProxyCredentialsDigest()
{
	HTTPCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("Proxy-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	creds.proxyAuthenticate(request, response);
	assertTrue (request.get("Proxy-Authorization") == "Digest username=\"user\", nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", uri=\"/digest/\", response=\"40e4889cfbd0e561f71e3107a2863bc4\"");
}


void HTTPCredentialsTest::testExtractCredentials()
{
	Poco::URI uri("http://user:s3cr3t@host.com/");
	std::string username;
	std::string password;
	HTTPCredentials::extractCredentials(uri, username, password);
	assertTrue (username == "user");
	assertTrue (password == "s3cr3t");
}


void HTTPCredentialsTest::testVerifyAuthInfo()
{
	HTTPDigestCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\"");
	creds.authenticate(request, response);
	assertTrue (creds.verifyAuthInfo(request));

	request.set("Authorization", "Digest nonce=\"212573bb90170538efad012978ab811f%lu\", realm=\"TestDigest\", response=\"xxe4889cfbd0e561f71e3107a2863bc4\", uri=\"/digest/\", username=\"user\"");
	assertTrue (!creds.verifyAuthInfo(request));
}


void HTTPCredentialsTest::testVerifyAuthInfoQoP()
{
	HTTPDigestCredentials creds("user", "s3cr3t");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse response;
	response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth,auth-int\"");
	creds.authenticate(request, response);
	assertTrue (creds.verifyAuthInfo(request));

	request.set("Authorization", "Digest cnonce=\"f9c80ffd1c3bc4ee47ed92b704ba75a4\", nc=00000001, nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth\", realm=\"TestDigest\", response=\"ff0e90b9aa019120ea0ed6e23ce95d9a\", uri=\"/digest/\", username=\"user\"");
	assertTrue (!creds.verifyAuthInfo(request));
}

void HTTPCredentialsTest::testVerifyAuthInfoQoPSHA256() {
	HTTPDigestCredentials sha256Creds("user", "s3cr3t");
	HTTPRequest sha256Request(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse sha256Response;
	sha256Response.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth,auth-int\", algorithm=SHA-256");
	sha256Creds.authenticate(sha256Request, sha256Response);
	assertTrue (sha256Creds.verifyAuthInfo(sha256Request));

	sha256Request.set("Authorization", "Digest cnonce=\"f9c80ffd1c3bc4ee47ed92b704ba75a4\", nc=00000001, nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth\", realm=\"TestDigest\", response=\"ff0e90b9aa019120ea0ed6e23ce95d9a\", uri=\"/digest/\", username=\"user\", algorithm=SHA-256");
	assertTrue (!sha256Creds.verifyAuthInfo(sha256Request));

	HTTPDigestCredentials sha256SessCreds("user", "s3cr3t");
	HTTPRequest sha256SessRequest(HTTPRequest::HTTP_GET, "/digest/");
	HTTPResponse sha256SessResponse;
	sha256SessResponse.set("WWW-Authenticate", "Digest realm=\"TestDigest\", nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth,auth-int\", algorithm=SHA-256-sess");
	sha256SessCreds.authenticate(sha256SessRequest, sha256SessResponse);
	assertTrue (sha256SessCreds.verifyAuthInfo(sha256SessRequest));

	sha256SessRequest.set("Authorization", "Digest cnonce=\"f9c80ffd1c3bc4ee47ed92b704ba75a4\", nc=00000001, nonce=\"212573bb90170538efad012978ab811f%lu\", opaque=\"opaque\", qop=\"auth\", realm=\"TestDigest\", response=\"ff0e90b9aa019120ea0ed6e23ce95d9a\", uri=\"/digest/\", username=\"user\", algorithm=SHA-256-sess");
	assertTrue (!sha256SessCreds.verifyAuthInfo(sha256SessRequest));
}

void HTTPCredentialsTest::testIsAlgorithmSupported() {
	HTTPDigestCredentials sha256Creds("user", "s3cr3t");

	assertTrue (sha256Creds.isAlgorithmSupported("MD5"));
	assertTrue (sha256Creds.isAlgorithmSupported("MD5-sess"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-sess"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-256"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-256-sess"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-512-256"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-512-256-sess"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-512"));
	assertTrue (sha256Creds.isAlgorithmSupported("SHA-512-sess"));
	assertFalse (sha256Creds.isAlgorithmSupported("random_algorithm"));
}

void HTTPCredentialsTest::setUp()
{
}


void HTTPCredentialsTest::tearDown()
{
}


CppUnit::Test* HTTPCredentialsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPCredentialsTest");

	CppUnit_addTest(pSuite, HTTPCredentialsTest, testBasicCredentials);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testProxyBasicCredentials);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testBadCredentials);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testAuthenticationParams);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testAuthenticationParamsMultipleHeaders);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testDigestCredentials);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testDigestCredentialsQoP);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testDigestCredentialsQoPSHA256);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testCredentialsBasic);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testProxyCredentialsBasic);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testCredentialsDigest);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testCredentialsDigestMultipleHeaders);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testProxyCredentialsDigest);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testExtractCredentials);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testVerifyAuthInfo);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testVerifyAuthInfoQoP);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testVerifyAuthInfoQoPSHA256);
	CppUnit_addTest(pSuite, HTTPCredentialsTest, testIsAlgorithmSupported);

	return pSuite;
}
