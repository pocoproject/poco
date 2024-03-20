//
// URITest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "URITest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/URI.h"
#include "Poco/Path.h"


using Poco::URI;
using Poco::Path;


URITest::URITest(const std::string& name): CppUnit::TestCase(name)
{
}


URITest::~URITest()
{
}


void URITest::testConstruction()
{
	URI uri;
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());

	uri.setScheme("ftp");
	assertTrue (uri.getScheme() == "ftp");
	assertTrue (uri.getPort() == 21);

	uri.setScheme("HTTP");
	assertTrue (uri.getScheme() == "http");

	uri.setAuthority("www.appinf.com");
	assertTrue (uri.getAuthority() == "www.appinf.com");
	assertTrue (uri.getPort() == 80);
	assertTrue (uri.getSpecifiedPort() == 0);

	uri.setAuthority("user@services.appinf.com:8000");
	assertTrue (uri.getUserInfo() == "user");
	assertTrue (uri.getHost() == "services.appinf.com");
	assertTrue (uri.getPort() == 8000);
	assertTrue (uri.getSpecifiedPort() == 8000);

	uri.setPath("/index.html");
	assertTrue (uri.getPath() == "/index.html");

	uri.setPath("/file%20with%20spaces.html");
	assertTrue (uri.getPath() == "/file with spaces.html");

	uri.setPathEtc("/query.cgi?query=foo");
	assertTrue (uri.getPath() == "/query.cgi");
	assertTrue (uri.getQuery() == "query=foo");
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.getPathEtc() == "/query.cgi?query=foo");
	assertTrue (uri.getPathAndQuery() == "/query.cgi?query=foo");

	uri.setPathEtc("/query.cgi?query=bar#frag");
	assertTrue (uri.getPath() == "/query.cgi");
	assertTrue (uri.getQuery() == "query=bar");
	assertTrue (uri.getFragment() == "frag");
	assertTrue (uri.getPathEtc() == "/query.cgi?query=bar#frag");
	assertTrue (uri.getPathAndQuery() == "/query.cgi?query=bar");

	uri.setQuery("query=test");
	assertTrue (uri.getQuery() == "query=test");

	uri.setFragment("result");
	assertTrue (uri.getFragment() == "result");

	URI uri2("file", "/home/guenter/foo.bar");
	assertTrue (uri2.getScheme() == "file");
	assertTrue (uri2.getPath() == "/home/guenter/foo.bar");

	URI uri3("http", "www.appinf.com", "/index.html");
	assertTrue (uri3.getScheme() == "http");
	assertTrue (uri3.getAuthority() == "www.appinf.com");
	assertTrue (uri3.getPath() == "/index.html");

	URI uri4("http", "www.appinf.com:8000", "/index.html");
	assertTrue (uri4.getScheme() == "http");
	assertTrue (uri4.getAuthority() == "www.appinf.com:8000");
	assertTrue (uri4.getPath() == "/index.html");

	URI uri5("http", "user@www.appinf.com:8000", "/index.html");
	assertTrue (uri5.getScheme() == "http");
	assertTrue (uri5.getUserInfo() == "user");
	assertTrue (uri5.getHost() == "www.appinf.com");
	assertTrue (uri5.getPort() == 8000);
	assertTrue (uri5.getAuthority() == "user@www.appinf.com:8000");
	assertTrue (uri5.getPath() == "/index.html");

	URI uri6("http", "user@www.appinf.com:80", "/index.html");
	assertTrue (uri6.getScheme() == "http");
	assertTrue (uri6.getUserInfo() == "user");
	assertTrue (uri6.getHost() == "www.appinf.com");
	assertTrue (uri6.getPort() == 80);
	assertTrue (uri6.getSpecifiedPort() == 80);
	assertTrue (uri6.getAuthority() == "user@www.appinf.com");
	assertTrue (uri6.getPath() == "/index.html");

	URI uri7("http", "user@www.appinf.com:", "/index.html");
	assertTrue (uri7.getScheme() == "http");
	assertTrue (uri7.getUserInfo() == "user");
	assertTrue (uri7.getHost() == "www.appinf.com");
	assertTrue (uri7.getPort() == 80);
	assertTrue (uri7.getSpecifiedPort() == 0);
	assertTrue (uri7.getAuthority() == "user@www.appinf.com");
	assertTrue (uri7.getPath() == "/index.html");

	URI uri8("http", "www.appinf.com", "/index.html", "query=test");
	assertTrue (uri8.getScheme() == "http");
	assertTrue (uri8.getAuthority() == "www.appinf.com");
	assertTrue (uri8.getPath() == "/index.html");
	assertTrue (uri8.getQuery() == "query=test");

	URI uri9("http", "www.appinf.com", "/index.html", "query=test", "fragment");
	assertTrue (uri9.getScheme() == "http");
	assertTrue (uri9.getAuthority() == "www.appinf.com");
	assertTrue (uri9.getPath() == "/index.html");
	assertTrue (uri9.getPathEtc() == "/index.html?query=test#fragment");
	assertTrue (uri9.getQuery() == "query=test");
	assertTrue (uri9.getFragment() == "fragment");

	uri9.clear();
	assertTrue (uri9.getScheme().empty());
	assertTrue (uri9.getAuthority().empty());
	assertTrue (uri9.getUserInfo().empty());
	assertTrue (uri9.getHost().empty());
	assertTrue (uri9.getPort() == 0);
	assertTrue (uri9.getPath().empty());
	assertTrue (uri9.getQuery().empty());
	assertTrue (uri9.getFragment().empty());

	URI uri10("ldap", "[2001:db8::7]", "/c=GB?objectClass?one");
	assertTrue (uri10.getScheme() == "ldap");
	assertTrue (uri10.getUserInfo().empty());
	assertTrue (uri10.getHost() == "2001:db8::7");
	assertTrue (uri10.getPort() == 389);
	assertTrue (uri10.getSpecifiedPort() == 0);
	assertTrue (uri10.getAuthority() == "[2001:db8::7]");
	assertTrue (uri10.getPathEtc() == "/c=GB?objectClass?one");

	URI uri11("http", "www.appinf.com", "/index.html?query=test#fragment");
	assertTrue (uri11.getScheme() == "http");
	assertTrue (uri11.getAuthority() == "www.appinf.com");
	assertTrue (uri11.getPath() == "/index.html");
	assertTrue (uri11.getPathEtc() == "/index.html?query=test#fragment");
	assertTrue (uri11.getQuery() == "query=test");
	assertTrue (uri11.getFragment() == "fragment");
}


void URITest::testParse()
{
	URI uri("http://www.appinf.com");
	assertTrue (uri.getScheme() == "http");
	assertTrue (uri.getAuthority() == "www.appinf.com");
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "http://www.appinf.com/";
	assertTrue (uri.getScheme() == "http");
	assertTrue (uri.getAuthority() == "www.appinf.com");
	assertTrue (uri.getPath() == "/");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "ftp://anonymous@ftp.appinf.com/pub/";
	assertTrue (uri.getScheme() == "ftp");
	assertTrue (uri.getUserInfo() == "anonymous");
	assertTrue (uri.getHost() == "ftp.appinf.com");
	assertTrue (uri.getPort() == 21);
	assertTrue (uri.getAuthority() == "anonymous@ftp.appinf.com");
	assertTrue (uri.getPath() == "/pub/");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());
	assertTrue (!uri.isRelative());

	uri = "https://www.appinf.com/index.html#top";
	assertTrue (uri.getScheme() == "https");
	assertTrue (uri.getHost() == "www.appinf.com");
	assertTrue (uri.getPort() == 443);
	assertTrue (uri.getPath() == "/index.html");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment() == "top");
	assertTrue (!uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
	assertTrue (uri.getScheme() == "http");
	assertTrue (uri.getHost() == "www.appinf.com");
	assertTrue (uri.getPort() == 80);
	assertTrue (uri.getPath() == "/search.cgi");
	assertTrue (uri.getQuery() == "keyword=test&scope=all");
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
	assertTrue (uri.getScheme() == "http");
	assertTrue (uri.getHost() == "www.appinf.com");
	assertTrue (uri.getPort() == 80);
	assertTrue (uri.getPath() == "/search.cgi");
	assertTrue (uri.getQuery() == "keyword=test&scope=all");
	assertTrue (uri.getFragment() == "result");
	assertTrue (!uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
	assertTrue (uri.getScheme() == "http");
	assertTrue (uri.getHost() == "www.appinf.com");
	assertTrue (uri.getPort() == 80);
	assertTrue (uri.getPath() == "/search.cgi");
	assertTrue (uri.getQuery() == "keyword=test encoded&scope=all");
	assertTrue (uri.getFragment() == "result");
	assertTrue (!uri.isRelative());

	uri = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	assertTrue (uri.getScheme() == "ldap");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost() == "2001:db8::7");
	assertTrue (uri.getPort() == 389);
	assertTrue (uri.getAuthority() == "[2001:db8::7]");
	assertTrue (uri.getPath() == "/c=GB");
	assertTrue (uri.getQuery() == "objectClass?one");
	assertTrue (uri.getFragment().empty());

	uri = "mailto:John.Doe@example.com";
	assertTrue (uri.getScheme() == "mailto");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getPath() == "John.Doe@example.com");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());

	uri = "tel:+1-816-555-1212";
	assertTrue (uri.getScheme() == "tel");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getPath() == "+1-816-555-1212");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());

	uri = "telnet://192.0.2.16:80";
	assertTrue (uri.getScheme() == "telnet");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost() == "192.0.2.16");
	assertTrue (uri.getPort() == 80);
	assertTrue (uri.getAuthority() == "192.0.2.16:80");
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());

	uri = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	assertTrue (uri.getScheme() == "urn");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());

	uri = "";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.empty());

	// relative references

	uri = "/foo/bar";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "/foo/bar");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "./foo/bar";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "./foo/bar");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "../foo/bar";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "../foo/bar");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "index.html";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "index.html");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "index.html#frag";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "index.html");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment() == "frag");
	assertTrue (uri.isRelative());

	uri = "?query=test";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery() == "query=test");
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "?query=test#frag";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery() == "query=test");
	assertTrue (uri.getFragment() == "frag");
	assertTrue (uri.isRelative());

	uri = "#frag";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment() == "frag");
	assertTrue (uri.isRelative());

	uri = "#";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath().empty());
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "file:///a/b/c";
	assertTrue (uri.getScheme() == "file");
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "/a/b/c");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "file://localhost/a/b/c";
	assertTrue (uri.getScheme() == "file");
	assertTrue (uri.getAuthority() == "localhost");
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost() == "localhost");
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "/a/b/c");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "file:///c:/Windows/system32/";
	assertTrue (uri.getScheme() == "file");
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "/c:/Windows/system32/");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (!uri.isRelative());

	uri = "./c:/Windows/system32/";
	assertTrue (uri.getScheme().empty());
	assertTrue (uri.getAuthority().empty());
	assertTrue (uri.getUserInfo().empty());
	assertTrue (uri.getHost().empty());
	assertTrue (uri.getPort() == 0);
	assertTrue (uri.getPath() == "./c:/Windows/system32/");
	assertTrue (uri.getQuery().empty());
	assertTrue (uri.getFragment().empty());
	assertTrue (uri.isRelative());

	uri = "ws://www.appinf.com/ws";
	assertTrue (uri.getScheme() == "ws");
	assertTrue (uri.getPort() == 80);

	uri = "wss://www.appinf.com/ws";
	assertTrue (uri.getScheme() == "wss");
	assertTrue (uri.getPort() == 443);
}


void URITest::testToString()
{
	URI uri("http://www.appinf.com");
	assertTrue (uri.toString() == "http://www.appinf.com");

	uri = "http://www.appinf.com/";
	assertTrue (uri.toString() == "http://www.appinf.com/");

	uri = "ftp://anonymous@ftp.appinf.com/pub/";
	assertTrue (uri.toString() == "ftp://anonymous@ftp.appinf.com/pub/");

	uri = "https://www.appinf.com/index.html#top";
	assertTrue (uri.toString() == "https://www.appinf.com/index.html#top");

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
	assertTrue (uri.toString() == "http://www.appinf.com/search.cgi?keyword=test&scope=all");

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
	assertTrue (uri.toString() == "http://www.appinf.com/search.cgi?keyword=test&scope=all#result");

	uri = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
	assertTrue (uri.toString() == "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");

	uri = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	assertTrue (uri.toString() == "ldap://[2001:db8::7]/c=GB?objectClass?one");

	uri = "mailto:John.Doe@example.com";
	assertTrue (uri.toString() == "mailto:John.Doe@example.com");

	uri = "tel:+1-816-555-1212";
	assertTrue (uri.toString() == "tel:+1-816-555-1212");

	uri = "telnet://192.0.2.16:80";
	assertTrue (uri.toString() == "telnet://192.0.2.16:80");

	uri = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	assertTrue (uri.toString() == "urn:oasis:names:specification:docbook:dtd:xml:4.1.2");

	uri = "";
	assertTrue (uri.toString() == "");

	// relative references

	uri = "/foo/bar";
	assertTrue (uri.toString() == "/foo/bar");

	uri = "./foo/bar";
	assertTrue (uri.toString() == "./foo/bar");

	uri = "../foo/bar";
	assertTrue (uri.toString() == "../foo/bar");

	uri = "//foo/bar";
	assertTrue (uri.toString() == "//foo/bar");

	uri = "index.html";
	assertTrue (uri.toString() == "index.html");

	uri = "index.html#frag";
	assertTrue (uri.toString() == "index.html#frag");

	uri = "?query=test";
	assertTrue (uri.toString() == "?query=test");

	uri = "?query=test#frag";
	assertTrue (uri.toString() == "?query=test#frag");

	uri = "#frag";
	assertTrue (uri.toString() == "#frag");

	uri = "#";
	assertTrue (uri.toString() == "");

	uri = "file:///a/b/c";
	assertTrue (uri.toString() == "file:///a/b/c");

	uri = "file://localhost/a/b/c";
	assertTrue (uri.toString() == "file://localhost/a/b/c");

	uri = "file:///c:/Windows/system32/";
	assertTrue (uri.toString() == "file:///c:/Windows/system32/");

	uri = "./c:/Windows/system32/";
	assertTrue (uri.toString() == "./c:/Windows/system32/");

	uri = "http://www.appinf.com";
	uri.setRawQuery("query=test");
	assertTrue (uri.toString() == "http://www.appinf.com/?query=test");
}


void URITest::testCompare()
{
	URI uri1("http://www.appinf.com");
	URI uri2("HTTP://www.appinf.com:80");
	assertTrue (uri1 == uri2);
	assertTrue (uri1 == "http://www.appinf.com:");
	assertTrue (uri1 != "http://www.google.com");

	uri1 = "/foo/bar";
	assertTrue (uri1 == "/foo/bar");
	assertTrue (uri1 != "/foo/baz");

	uri1 = "?query";
	assertTrue (uri1 == "?query");
	assertTrue (uri1 != "?query2");

	uri1 = "#frag";
	assertTrue (uri1 == "#frag");
	assertTrue (uri1 != "#frag2");

	uri1 = "/index.html#frag";
	assertTrue (uri1 == "/index.html#frag");
	assertTrue (uri1 != "/index.html");
}


void URITest::testNormalize()
{
	URI uri("http://www.appinf.com");
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com");

	uri = "http://www.appinf.com/";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/");

	uri = "http://www.appinf.com/foo/bar/./index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/bar/index.html");

	uri = "http://www.appinf.com/foo/bar/../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/index.html");

	uri = "http://www.appinf.com/foo/./bar/../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/index.html");

	uri = "http://www.appinf.com/foo/./bar/../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/index.html");

	uri = "http://www.appinf.com/foo/bar/../../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/index.html");

	uri = "http://www.appinf.com/foo/bar/../../../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/index.html");

	uri = "http://www.appinf.com/foo/bar/.././../index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/index.html");

	uri = "http://www.appinf.com/./foo/bar/index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/bar/index.html");

	uri = "http://www.appinf.com/../foo/bar/index.html";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/bar/index.html");

	uri = "http://www.appinf.com/../foo/bar/";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/foo/bar/");

	uri = "http://www.appinf.com/../foo/../";
	uri.normalize();
	assertTrue (uri.toString() == "http://www.appinf.com/");

	uri = "file:///c:/Windows/system32/";
	uri.normalize();
	assertTrue (uri.toString() == "file:///c:/Windows/system32/");

	uri.clear();
	uri.setPath("c:/windows/system32/");
	uri.normalize();
	assertTrue (uri.toString() == "./c:/windows/system32/");
}


void URITest::testResolve()
{
	URI uri("http://www.appinf.com");

	uri.resolve("/index.html");
	assertTrue (uri.toString() == "http://www.appinf.com/index.html");

	uri.resolve("#frag");
	assertTrue (uri.toString() == "http://www.appinf.com/index.html#frag");

	uri = "http://www.appinf.com/html";
	uri.resolve("../images/foo.gif");
	assertTrue (uri.toString() == "http://www.appinf.com/images/foo.gif");

	uri = "http://www.appinf.com/html/";
	uri.resolve(".");
	assertTrue (uri.toString() == "http://www.appinf.com/html/");

	uri = "http://www.appinf.com/html/";
	uri.resolve(".");
	assertTrue (uri.toString() == "http://www.appinf.com/html/");

	uri = "http://www.appinf.com/html/";
	uri.resolve("..");
	assertTrue (uri.toString() == "http://www.appinf.com/");

	uri = "http://www.appinf.com/html/";
	uri.resolve("index.html");
	assertTrue (uri.toString() == "http://www.appinf.com/html/index.html");

	uri = "http://www.appinf.com/html/";
	uri.resolve("/index.html");
	assertTrue (uri.toString() == "http://www.appinf.com/index.html");

	uri = "/a/b/c/d/e";
	uri.resolve("./../../f/./g");
	assertTrue (uri.toString() == "/a/b/f/g");

	uri = "/a/b/../c/";
	uri.resolve("../d");
	assertTrue (uri.toString() == "/a/d");

	uri = "/a/b/../c/";
	uri.resolve("../d/");
	assertTrue (uri.toString() == "/a/d/");

	uri = "/a/b/c/";
	uri.resolve("../../../../d/");
	assertTrue (uri.toString() == "/d/");

	uri = "/a/b/c/";
	uri.resolve("%2e%2e/%2e%2e/%2e%2e/../d/");
	assertTrue (uri.toString() == "/d/");

	uri = "/a/b/c/";
	uri.resolve("");
	assertTrue (uri.toString() == "/a/b/c/");

	uri = "/a/b/c/";
	uri.resolve("/d/");
	assertTrue (uri.toString() == "/d/");

	uri = "/a/b/c";
	uri.resolve("");
	assertTrue (uri.toString() == "/a/b/c");

	uri = "/a/b/c";
	uri.resolve("?query=test");
	assertTrue (uri.toString() == "/a/b/c?query=test");

	uri = "/a/b/c";
	uri.resolve("#frag");
	assertTrue (uri.toString() == "/a/b/c#frag");

	uri = "http://www.appinf.com/html/";
	uri.resolve("http://www.google.com/");
	assertTrue (uri.toString() == "http://www.google.com/");

	uri = "http://www.appinf.com/";
	URI uri2(uri, "index.html");
	assertTrue (uri2.toString() == "http://www.appinf.com/index.html");

	uri = "index.html";
	URI uri3(uri, "search.html");
	assertTrue (uri3.toString() == "search.html");
}


void URITest::testSwap()
{
	URI uri1("http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
	URI uri2("mailto:John.Doe@example.com");

	uri1.swap(uri2);
	assertTrue (uri1.toString() == "mailto:John.Doe@example.com");
	assertTrue (uri2.toString() == "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
}


void URITest::testOther()
{
	// The search string is "hello%world"; google happens to ignore the '%'
	// character, so it finds lots of hits for "hello world" programs. This is
	// a convenient reproduction case, and a URL that actually works.
	Poco::URI uri("http://google.com/search?q=hello%25world#frag%20ment");

	assertTrue (uri.getHost() == "google.com");
	assertTrue (uri.getPath() == "/search");
	assertTrue (uri.getQuery() == "q=hello%world");
	assertTrue (uri.getRawQuery() == "q=hello%25world");
	assertTrue (uri.getFragment() == "frag ment");
	assertTrue (uri.getRawFragment() == "frag%20ment");
	assertTrue (uri.toString() == "http://google.com/search?q=hello%25world#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=hello%25world#frag%20ment");

	uri.setQuery("q=foo&bar");
	assertTrue (uri.getQuery() == "q=foo&bar");
	assertTrue (uri.getRawQuery() == "q=foo&bar");
	assertTrue (uri.toString() == "http://google.com/search?q=foo&bar#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=foo&bar#frag%20ment");

	uri.setQuery("q=foo/bar");
	assertTrue (uri.getQuery() == "q=foo/bar");
	assertTrue (uri.getRawQuery() == "q=foo%2Fbar");
	assertTrue (uri.toString() == "http://google.com/search?q=foo%2Fbar#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=foo%2Fbar#frag%20ment");

	uri.setQuery("q=goodbye cruel world");
	assertTrue (uri.getQuery() == "q=goodbye cruel world");
	assertTrue (uri.getRawQuery() == "q=goodbye%20cruel%20world");
	assertTrue (uri.toString() == "http://google.com/search?q=goodbye%20cruel%20world#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=goodbye%20cruel%20world#frag%20ment");

	uri.setRawQuery("q=pony%7eride");
	assertTrue (uri.getQuery() == "q=pony~ride");
	assertTrue (uri.getRawQuery() == "q=pony%7eride");
	assertTrue (uri.toString() == "http://google.com/search?q=pony%7eride#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=pony%7eride#frag%20ment");

	uri.addQueryParameter("pa=ra&m1");
	assertTrue (uri.getRawQuery() == "q=pony%7eride&pa%3Dra%26m1=");
	assertTrue (uri.getQuery() == "q=pony~ride&pa=ra&m1=");
	uri.addQueryParameter("pa=ra&m2", "val&ue");
	assertTrue (uri.getRawQuery() == "q=pony%7eride&pa%3Dra%26m1=&pa%3Dra%26m2=val%26ue");
	assertTrue (uri.getQuery() == "q=pony~ride&pa=ra&m1=&pa=ra&m2=val&ue");

	uri = "http://google.com/search?q=hello+world#frag%20ment";
	assertTrue (uri.getHost() == "google.com");
	assertTrue (uri.getPath() == "/search");
	assertTrue (uri.getQuery() == "q=hello+world");
	assertTrue (uri.getRawQuery() == "q=hello+world");
	assertTrue (uri.getFragment() == "frag ment");
	assertTrue (uri.getRawFragment() == "frag%20ment");
	assertTrue (uri.toString() == "http://google.com/search?q=hello+world#frag%20ment");
	assertTrue (uri.getPathEtc() == "/search?q=hello+world#frag%20ment");

	uri.setFragment("foo/bar");
	assertTrue (uri.getFragment() == "foo/bar");
	assertTrue (uri.getRawFragment() == "foo/bar");
	assertTrue (uri.toString() == "http://google.com/search?q=hello+world#foo/bar");
	assertTrue (uri.getPathEtc() == "/search?q=hello+world#foo/bar");
	uri.setRawFragment("foo%2Fbar");
	assertTrue (uri.getFragment() == "foo/bar");
	assertTrue (uri.getRawFragment() == "foo%2Fbar");
	assertTrue (uri.toString() == "http://google.com/search?q=hello+world#foo%2Fbar");
	assertTrue (uri.getPathEtc() == "/search?q=hello+world#foo%2Fbar");

	uri = "http://ServerSocket.com/index.html";
	assertTrue (uri.toString() == "http://serversocket.com/index.html");

	uri = "http+unix://%2Ftmp%2FServerSocket/index.html";
	assertTrue (uri.toString() == "http+unix://%2Ftmp%2FServerSocket/index.html");
	std::string decoded;
	uri.decode("http+unix://%2Ftmp%2FServerSocket/index.html", decoded);
	assertTrue (decoded == "http+unix:///tmp/ServerSocket/index.html");
}


void URITest::testEncodeDecode()
{
	std::string str;
	URI::encode("http://google.com/search?q=hello+world#frag ment", "+#?", str);
	assertTrue (str == "http://google.com/search%3Fq=hello%2Bworld%23frag%20ment");

	str = "";
	URI::encode("http://google.com/search?q=hello+world#frag ment", "", str);
	assertTrue (str == "http://google.com/search?q=hello+world#frag%20ment");

	str = "";
	URI::decode("http://google.com/search?q=hello+world#frag%20ment", str, true);
	assertTrue (str == "http://google.com/search?q=hello world#frag ment");

	str = "";
	URI::decode("http://google.com/search?q=hello%2Bworld#frag%20ment", str);
	assertTrue (str == "http://google.com/search?q=hello+world#frag ment");
}


void URITest::testFromPath()
{
	Path path1("/var/www/site/index.html", Path::PATH_UNIX);
	URI uri1(path1);
	assertTrue (uri1.toString() == "file:///var/www/site/index.html");

	Path path2("/var/www/site/with space.html", Path::PATH_UNIX);
	URI uri2(path2);
	assertTrue (uri2.toString() == "file:///var/www/site/with%20space.html");

	Path path3("c:\\www\\index.html", Path::PATH_WINDOWS);
	URI uri3(path3);
	assertTrue (uri3.toString() == "file:///c:/www/index.html");
}


void URITest::testQueryParameters()
{
	Poco::URI uri("http://google.com/search?q=hello+world&client=safari");
	URI::QueryParameters params = uri.getQueryParameters();
	assertTrue (params.size() == 2);
	assertTrue (params[0].first == "q");
	assertTrue (params[0].second == "hello world");
	assertTrue (params[1].first == "client");
	assertTrue (params[1].second == "safari");

	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=hello%20world&client=safari");

	uri = "http://google.com/search?q=&client&";
	params = uri.getQueryParameters();
	assertTrue (params.size() == 2);
	assertTrue (params[0].first == "q");
	assertTrue (params[0].second == "");
	assertTrue (params[1].first == "client");
	assertTrue (params[1].second == "");

	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=&client=");

	params[0].second = "foo/bar?";
	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=foo%2Fbar%3F&client=");

	params[0].second = "foo&bar";
	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=foo%26bar&client=");
}


void URITest::testQueryParametersPlus()
{
	Poco::URI uri("http://google.com/search?q=hello+world&client=safari");
	URI::QueryParameters params = uri.getQueryParameters(false);
	assertTrue (params.size() == 2);
	assertTrue (params[0].first == "q");
	assertTrue (params[0].second == "hello+world");
	assertTrue (params[1].first == "client");
	assertTrue (params[1].second == "safari");

	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=hello%2Bworld&client=safari");

	uri = "http://google.com/search?q=&client&";
	params = uri.getQueryParameters();
	assertTrue (params.size() == 2);
	assertTrue (params[0].first == "q");
	assertTrue (params[0].second == "");
	assertTrue (params[1].first == "client");
	assertTrue (params[1].second == "");

	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=&client=");

	params[0].second = "foo/bar?";
	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=foo%2Fbar%3F&client=");

	params[0].second = "foo&bar";
	uri.setQueryParameters(params);
	assertTrue (uri.toString() == "http://google.com/search?q=foo%26bar&client=");
}


void URITest::setUp()
{
}


void URITest::tearDown()
{
}


CppUnit::Test* URITest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("URITest");

	CppUnit_addTest(pSuite, URITest, testConstruction);
	CppUnit_addTest(pSuite, URITest, testParse);
	CppUnit_addTest(pSuite, URITest, testToString);
	CppUnit_addTest(pSuite, URITest, testCompare);
	CppUnit_addTest(pSuite, URITest, testNormalize);
	CppUnit_addTest(pSuite, URITest, testResolve);
	CppUnit_addTest(pSuite, URITest, testSwap);
	CppUnit_addTest(pSuite, URITest, testEncodeDecode);
	CppUnit_addTest(pSuite, URITest, testOther);
	CppUnit_addTest(pSuite, URITest, testFromPath);
	CppUnit_addTest(pSuite, URITest, testQueryParameters);
	CppUnit_addTest(pSuite, URITest, testQueryParametersPlus);

	return pSuite;
}
