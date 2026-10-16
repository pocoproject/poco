//
// HTTPChunkedStreamTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPChunkedStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timespan.h"
#include <string>
#include <thread>


using Poco::Net::HTTPChunkedInputStream;
using Poco::Net::HTTPChunkedOutputStream;
using Poco::Net::HTTPClientSession;
using Poco::Net::MessageException;
using Poco::Net::MessageHeader;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Net::StreamSocketImpl;
using Poco::StreamCopier;
using Poco::TimeoutException;
using Poco::Timespan;


namespace
{
	enum class ReadResult
	{
		COMPLETE,
		TRUNCATED,
		REJECTED,
		OTHER
	};


	ReadResult readChunked(const std::string& raw, std::string& body, MessageHeader* pTrailer = nullptr)
		/// Reads raw as a chunked body from a loopback connection that the
		/// peer closes after raw. Returns COMPLETE if the stream ends without
		/// an error, TRUNCATED if it ends at end of file with a MessageException
		/// in the session, REJECTED if the stream is bad and the session holds
		/// a MessageException, and OTHER otherwise.
	{
		ServerSocket server(SocketAddress("127.0.0.1", 0));
		StreamSocket client(server.address());
		StreamSocket peer = server.acceptConnection();
		client.setReceiveTimeout(Timespan(5, 0));
		std::thread writer([&peer, &raw]()
		{
			try
			{
				peer.sendBytes(raw.data(), static_cast<int>(raw.size()));
				peer.shutdownSend();
			}
			catch (Poco::Exception&)
			{
			}
		});
		ReadResult result = ReadResult::OTHER;
		{
			HTTPClientSession session(client);
			HTTPChunkedInputStream istr(session, pTrailer);
			body.clear();
			StreamCopier::copyToString(istr, body);
			const bool messageException = dynamic_cast<const MessageException*>(session.networkException()) != nullptr;
			if (istr.bad())
			{
				if (messageException) result = ReadResult::REJECTED;
			}
			else if (session.networkException() == nullptr)
			{
				result = ReadResult::COMPLETE;
			}
			else if (messageException)
			{
				result = ReadResult::TRUNCATED;
			}
		}
		writer.join();
		return result;
	}


	StreamSocket acceptPeer(ServerSocket& server, StreamSocket& client)
		/// Connects client to server and returns the accepted socket,
		/// with a receive timeout of 5 seconds.
	{
		client.connect(server.address());
		StreamSocket peer = server.acceptConnection();
		peer.setReceiveTimeout(Timespan(5, 0));
		return peer;
	}


	std::string receiveAll(StreamSocket& peer)
		/// Returns the bytes received on peer until the connection is closed.
	{
		std::string raw;
		char buffer[256];
		int n = 0;
		while ((n = peer.receiveBytes(buffer, sizeof(buffer))) > 0) raw.append(buffer, n);
		return raw;
	}


	std::string writeChunked(const std::string& body, MessageHeader* pTrailer = nullptr)
		/// Writes body through an HTTPChunkedOutputStream, closes its stream
		/// buffer and then destroys the stream, and returns the bytes the
		/// peer receives.
	{
		ServerSocket server(SocketAddress("127.0.0.1", 0));
		StreamSocket client;
		StreamSocket peer = acceptPeer(server, client);
		{
			HTTPClientSession session(client);
			HTTPChunkedOutputStream ostr(session, pTrailer);
			ostr << body;
			ostr.rdbuf()->close();
		}
		return receiveAll(peer);
	}


	class ShortWriteSocketImpl: public StreamSocketImpl
		/// Sends one byte less than requested, as a non-blocking socket may.
	{
	public:
		using StreamSocketImpl::sendBytes;

		int sendBytes(const void* buffer, int length, int flags = 0) override
		{
			return StreamSocketImpl::sendBytes(buffer, length > 1 ? length - 1 : length, flags);
		}
	};


	class SendTimeoutSocketImpl: public ShortWriteSocketImpl
		/// Times out on the first send and sends one byte less than
		/// requested afterwards.
	{
	public:
		using ShortWriteSocketImpl::sendBytes;

		int sendBytes(const void* buffer, int length, int flags = 0) override
		{
			if (_timedOut) return ShortWriteSocketImpl::sendBytes(buffer, length, flags);
			_timedOut = true;
			throw TimeoutException();
		}

	private:
		bool _timedOut = false;
	};
}


HTTPChunkedStreamTest::HTTPChunkedStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPChunkedStreamTest::~HTTPChunkedStreamTest() = default;


void HTTPChunkedStreamTest::testValid()
{
	std::string body;
	assertTrue (readChunked("5\r\nhello\r\n6\r\n world\r\n0\r\n\r\n", body) == ReadResult::COMPLETE);
	assertTrue (body == "hello world");
	assertTrue (readChunked("00000005\r\nhello\r\nA\r\n0123456789\r\n0\r\n\r\nNEXT", body) == ReadResult::COMPLETE);
	assertTrue (body == "hello0123456789");
	assertTrue (readChunked("5 ;a=b\r\nhello\r\n0\r\n\r\n", body) == ReadResult::COMPLETE);
	assertTrue (body == "hello");
	assertTrue (readChunked("5;a=\"\xff\"\t\r\nhello\r\n0\r\n\r\n", body) == ReadResult::COMPLETE);
	assertTrue (body == "hello");
}


void HTTPChunkedStreamTest::testTrailer()
{
	std::string body;
	MessageHeader trailer;
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX-Checksum: abc\r\nX-Count: 1\r\n\r\n", body, &trailer) == ReadResult::COMPLETE);
	assertTrue (body == "hello");
	assertTrue (trailer.get("X-Checksum") == "abc");
	assertTrue (trailer.get("X-Count") == "1");
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX-Checksum: abc\r\n\r\n", body) == ReadResult::COMPLETE);
	assertTrue (body == "hello");
}


void HTTPChunkedStreamTest::testInvalidChunkSize()
{
	std::string body;
	assertTrue (readChunked("5x\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("0x5\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("+5\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("-5\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked(" 5\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("000000005\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5 x\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5;a\x01" "b\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5;" + std::string(5000, 'a') + "\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
}


void HTTPChunkedStreamTest::testBareLineFeed()
{
	std::string body;
	assertTrue (readChunked("5\nhello\n0\n\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\rx\r\nhello\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\r\nhello\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\r\nhello\r\n0\n\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\r\nhello\r\n0\r\n\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX: y\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX: y\r\n\n", body) == ReadResult::REJECTED);
}


void HTTPChunkedStreamTest::testMissingCRLFAfterData()
{
	std::string body;
	assertTrue (readChunked("5\r\nhelloAB\r\n" + std::string(171, 'x') + "\r\n0\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (readChunked("2\r\nxx450\r\n\r\nGET /victim HTTP/1.1\r\nHost: a\r\n\r\n", body) == ReadResult::REJECTED);
	assertTrue (body.find("GET /victim") == std::string::npos);
	assertTrue (readChunked("5\r\nhello0\r\n\r\n", body) == ReadResult::REJECTED);
}


void HTTPChunkedStreamTest::testTruncated()
{
	std::string body;
	assertTrue (readChunked("", body) == ReadResult::TRUNCATED);
	assertTrue (body.empty());
	assertTrue (readChunked("5", body) == ReadResult::TRUNCATED);
	assertTrue (body.empty());
	assertTrue (readChunked("5\r", body) == ReadResult::TRUNCATED);
	assertTrue (body.empty());
	assertTrue (readChunked("5\r\nhel", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hel");
	assertTrue (readChunked("5\r\nhello", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello");
	assertTrue (readChunked("5\r\nhello\r", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello");
	assertTrue (readChunked("5\r\nhello\r\n", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello");
	assertTrue (readChunked("5\r\nhello\r\n6\r\n wor", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello wor");
	assertTrue (readChunked("5\r\nhello\r\n6\r\n world\r\n0\r\n", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello world");
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX: y\r\n", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello");
	assertTrue (readChunked("5\r\nhello\r\n0\r\nX: y\r\n\r", body) == ReadResult::TRUNCATED);
	assertTrue (body == "hello");
}


void HTTPChunkedStreamTest::testReadAfterTimeout()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(server.address());
	StreamSocket peer = server.acceptConnection();
	client.setReceiveTimeout(Timespan(0, 200000));
	const std::string sizeStart("5");
	peer.sendBytes(sizeStart.data(), static_cast<int>(sizeStart.size()));
	HTTPClientSession session(client);
	HTTPChunkedInputStream istr(session, nullptr);
	std::string body;
	StreamCopier::copyToString(istr, body);
	assertTrue (istr.bad());
	assertTrue (dynamic_cast<const TimeoutException*>(session.networkException()) != nullptr);

	// The rest of the size line arrives after the timeout.
	const std::string rest("\r\nhello\r\n0\r\n\r\n");
	peer.sendBytes(rest.data(), static_cast<int>(rest.size()));
	assertTrue (istr.rdbuf()->sgetc() == std::char_traits<char>::eof());
	assertTrue (dynamic_cast<const TimeoutException*>(session.networkException()) != nullptr);
}


void HTTPChunkedStreamTest::testReadInPartsNonBlocking()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(server.address());
	StreamSocket peer = server.acceptConnection();
	client.setReceiveTimeout(Timespan(5, 0));
	const std::string first("a\r\n01234");
	peer.sendBytes(first.data(), static_cast<int>(first.size()));
	HTTPClientSession session(client);
	HTTPChunkedInputStream istr(session, nullptr);
	istr.peek();

	// The rest of the chunk data has not arrived yet.
	client.setBlocking(false);
	std::string body;
	StreamCopier::copyToString(istr, body);
	assertTrue (body == "01234");
	assertTrue (session.networkException() == nullptr);
	assertTrue (!istr.bad());

	istr.clear();
	client.setBlocking(true);
	const std::string rest("56789\r\n0\r\n\r\n");
	peer.sendBytes(rest.data(), static_cast<int>(rest.size()));
	StreamCopier::copyToString(istr, body);
	assertTrue (body == "0123456789");
	assertTrue (session.networkException() == nullptr);
	assertTrue (!istr.bad());
	assertTrue (istr.eof());
}


void HTTPChunkedStreamTest::testTrailerTooLong()
{
	std::string raw("5\r\nhello\r\n0\r\n");
	for (int i = 0; i < 80; ++i)
		raw += "X-Field-" + std::to_string(i) + ": " + std::string(1000, 'v') + "\r\n";
	raw += "\r\n";
	std::string body;
	assertTrue (readChunked(raw, body) == ReadResult::REJECTED);
}


void HTTPChunkedStreamTest::testWriteFoldedTrailer()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client;
	StreamSocket peer = acceptPeer(server, client);
	{
		HTTPClientSession session(client);
		MessageHeader trailer;
		trailer.set("X-Checksum", "abc\r\n def");
		HTTPChunkedOutputStream ostr(session, &trailer);
		ostr << "hello";
		try
		{
			ostr.rdbuf()->close();
			fail("folded trailer value must throw");
		}
		catch (MessageException&)
		{
		}
		assertTrue (dynamic_cast<const MessageException*>(session.networkException()) != nullptr);
	}
	assertTrue (receiveAll(peer) == "5\r\nhello\r\n");
}


void HTTPChunkedStreamTest::testWriteCloseOnce()
{
	assertTrue (writeChunked("hello") == "5\r\nhello\r\n0\r\n\r\n");
	MessageHeader trailer;
	trailer.set("X-Checksum", "abc");
	assertTrue (writeChunked("hello", &trailer) == "5\r\nhello\r\n0\r\nX-Checksum: abc\r\n\r\n");
}


void HTTPChunkedStreamTest::testWriteTrailerFailure()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(new ShortWriteSocketImpl);
	StreamSocket peer = acceptPeer(server, client);
	{
		HTTPClientSession session(client);
		MessageHeader trailer;
		trailer.set("X-Checksum", "abc");
		HTTPChunkedOutputStream ostr(session, &trailer);
		try
		{
			ostr.rdbuf()->close();
			fail("must throw MessageException");
		}
		catch (MessageException&)
		{
		}
		try
		{
			session.flushRequest();
			fail("flushRequest() must throw MessageException");
		}
		catch (MessageException&)
		{
		}
	}
	assertTrue (receiveAll(peer) == "0\r\nX-Checksum: abc\r\n\r");
}


void HTTPChunkedStreamTest::testWriteFailure()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(new ShortWriteSocketImpl);
	StreamSocket peer = acceptPeer(server, client);
	HTTPClientSession session(client);
	{
		HTTPChunkedOutputStream ostr(session);
		ostr << "hello" << std::flush;
		assertTrue (ostr.bad());
		try
		{
			ostr.rdbuf()->close();
			fail("must throw MessageException");
		}
		catch (MessageException&)
		{
		}
		assertTrue (dynamic_cast<const MessageException*>(session.networkException()) != nullptr);
	}
	{
		HTTPChunkedOutputStream ostr(session);
		try
		{
			ostr.rdbuf()->close();
			fail("must throw MessageException");
		}
		catch (MessageException&)
		{
		}
	}
}


void HTTPChunkedStreamTest::testWriteFailureKeepsCause()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(new SendTimeoutSocketImpl);
	StreamSocket peer = acceptPeer(server, client);
	HTTPClientSession session(client);
	{
		HTTPChunkedOutputStream ostr(session);
		ostr << "hello" << std::flush;
		assertTrue (ostr.bad());
		try
		{
			ostr.rdbuf()->close();
			fail("must throw MessageException");
		}
		catch (MessageException&)
		{
		}
	}
	assertTrue (dynamic_cast<const TimeoutException*>(session.networkException()) != nullptr);
	try
	{
		session.flushRequest();
		fail("flushRequest() must throw TimeoutException");
	}
	catch (TimeoutException&)
	{
	}
}


void HTTPChunkedStreamTest::setUp()
{
}


void HTTPChunkedStreamTest::tearDown()
{
}


CppUnit::Test* HTTPChunkedStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPChunkedStreamTest");

	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testValid);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testTrailer);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testInvalidChunkSize);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testBareLineFeed);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testMissingCRLFAfterData);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testTruncated);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testReadAfterTimeout);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testReadInPartsNonBlocking);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testTrailerTooLong);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testWriteFoldedTrailer);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testWriteCloseOnce);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testWriteTrailerFailure);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testWriteFailure);
	CppUnit_addTest(pSuite, HTTPChunkedStreamTest, testWriteFailureKeepsCause);

	return pSuite;
}
