#include "HTTPReactorServerSessionTest.h"
#include "Poco/Net/HTTPReactorServerSession.h"
#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCaller.h"
#include <chrono>
#include <future>
#include <memory>
#include <thread>


namespace {


enum class ParseOutcome
{
	COMPLETE,
	INCOMPLETE,
	MALFORMED,
	DID_NOT_RETURN
};


struct ParseResult
{
	ParseOutcome outcome{ParseOutcome::DID_NOT_RETURN};
	// Bytes left in the buffer once the completed request has been popped.
	std::size_t  remaining{0};
};


// Runs checkRequestComplete() under a bound, because a chunk size line that the
// parser mishandles makes it loop without end: a regression then fails an
// assertion instead of hanging the whole test suite. The buffer is shared with
// the worker so that it stays alive if that worker is left running.
ParseResult checkComplete(const std::string& request)
{
	const auto pBuf = std::make_shared<std::string>(request);
	std::packaged_task<ParseResult()> task([pBuf]()
		{
			ParseResult result;
			Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), *pBuf, nullptr);
			try
			{
				result.outcome = session.checkRequestComplete() ?
					ParseOutcome::COMPLETE : ParseOutcome::INCOMPLETE;
				if (result.outcome == ParseOutcome::COMPLETE)
				{
					session.popCompletedRequest();
					result.remaining = pBuf->size();
				}
			}
			catch (const Poco::Net::MessageException&)
			{
				result.outcome = ParseOutcome::MALFORMED;
			}
			return result;
		});
	std::future<ParseResult> done = task.get_future();
	std::thread worker(std::move(task));
	if (done.wait_for(std::chrono::seconds(5)) != std::future_status::ready)
	{
		worker.detach();
		return ParseResult();
	}
	worker.join();
	return done.get();
}


const std::string chunkedHead("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n");


} // namespace


HTTPReactorServerSessionTest::HTTPReactorServerSessionTest(const std::string& name) : CppUnit::TestCase(name)
{
}

HTTPReactorServerSessionTest::~HTTPReactorServerSessionTest()
{
}

void HTTPReactorServerSessionTest::setUp()
{
}

void HTTPReactorServerSessionTest::tearDown()
{
}

void HTTPReactorServerSessionTest::testCheckRequestComplete()
{
	std::string  buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunked()
{
	std::string   buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(!buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(!buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithTrailer()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTrailer: Content-MD5\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(!buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\ntrailer: content-md5\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(!buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithTrailerAndBody()
{
	std::string buf(
		"GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTrailer: Content-MD5\r\n\r\n0\r\nContent-MD5:xxxxx\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());

	buf.assign(
		"GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\ntrailer: content-md5\r\n\r\n0\r\ncontent-md5:xxxxx\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\n\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithStickyBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nHello\r\n5\r\nWorld\r\n0\r\n\r\n");
	int len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.length() == len);
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLength()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHello");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHello");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLengthIncomplete()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHel");
	int len = buf.length();
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.size() == len);

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHel");
	len = buf.length();
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.size() == len);
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLengthWithStickyBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHello");
	int len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.length() == len);

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHello");
	len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.length() == len);
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedMalformedSize()
{
	// The body reader ends the size line at the first LF, so a CR or an LF inside
	// the line, and any terminator it would read differently, must be refused.
	const std::string sizeLines[] = {"-6", "+5", "0x5", " 5", "123456789", "zz",
		"5\nHello", "5\rHello", "5;ext=1\nHello", "5\vHello"};
	for (const std::string& sizeLine: sizeLines)
	{
		const ParseResult result = checkComplete(chunkedHead + sizeLine + "\r\nHello\r\n0\r\n\r\n");
		if (result.outcome == ParseOutcome::DID_NOT_RETURN)
			fail("chunk size line made the parser loop: " + sizeLine, __LINE__, __FILE__);
		if (result.outcome != ParseOutcome::MALFORMED)
			fail("chunk size line not rejected: " + sizeLine, __LINE__, __FILE__);
	}
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedSizeExtension()
{
	const std::string sizeLines[] = {"5;ext=1", "5 ;ext=1", "5\t;ext=1"};
	for (const std::string& sizeLine: sizeLines)
	{
		const std::string request(chunkedHead + sizeLine + "\r\nHello\r\n0\r\n\r\n");
		const ParseResult result = checkComplete(request + request);
		assertTrue(result.outcome == ParseOutcome::COMPLETE);
		assertTrue(result.remaining == request.size());
	}
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedMissingCRLF()
{
	assertTrue(checkComplete(chunkedHead + "5\r\nHelloXX0\r\n\r\n").outcome == ParseOutcome::MALFORMED);
}

void HTTPReactorServerSessionTest::testChunkExtensionHighByte()
{
	// Where char is signed, a byte of value 0xFF must not read as end of file.
	std::string buf(chunkedHead + "5;a=\"\xff\"\r\nhello\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	Poco::Net::HTTPRequest request;
	Poco::Net::HTTPHeaderInputStream headerStream(session);
	request.read(headerStream);
	Poco::Net::HTTPChunkedInputStream bodyStream(session, nullptr);
	std::string body;
	Poco::StreamCopier::copyToString(bodyStream, body);
	assertTrue(!bodyStream.bad());
	assertTrue(body == "hello");
}

CppUnit::Test* HTTPReactorServerSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPReactorServerSessionTest");

	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestComplete);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunked);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithTrailer);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithTrailerAndBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithStickyBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLength);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLengthIncomplete);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLengthWithStickyBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedMalformedSize);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedSizeExtension);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedMissingCRLF);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testChunkExtensionHighByte);

	return pSuite;
}

