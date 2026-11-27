//
// MultipartReaderTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MultipartReaderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MultipartReader.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include <sstream>
#include <vector>


using Poco::Net::MultipartReader;
using Poco::Net::MessageHeader;
using Poco::Net::MultipartException;
using Poco::StreamCopier;


namespace
{
	[[nodiscard]] std::vector<std::string> readParts(const std::string& message, const std::string& boundary = "MIME_boundary_01234567")
		/// Returns the content of the parts of a message.
	{
		std::istringstream istr(message);
		MultipartReader r(istr, boundary);
		std::vector<std::string> parts;
		while (r.hasNextPart())
		{
			MessageHeader h;
			r.nextPart(h);
			StreamCopier::copyToString(r.stream(), parts.emplace_back());
		}
		return parts;
	}


	[[nodiscard]] std::string twoPartsWithContentLength(const std::string& contentLength)
		/// Returns a message of two parts; the first one has a Content-Length
		/// header field with the given value.
	{
		return "\r\n--MIME_boundary_01234567\r\nContent-Length: " + contentLength + "\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567\r\n\r\nthis is part 2\r\n--MIME_boundary_01234567--\r\n";
	}
}


MultipartReaderTest::MultipartReaderTest(const std::string& name): CppUnit::TestCase(name)
{
}


MultipartReaderTest::~MultipartReaderTest()
{
}


void MultipartReaderTest::testReadOnePart()
{
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.boundary() == "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (!r.hasNextPart());
	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testReadTwoParts()
{
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567\r\n\r\nthis is part 2\r\n\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (r.hasNextPart());
	r.nextPart(h);
	assertTrue (h.empty());
	std::istream& ii = r.stream();
	part.clear();
	ch = ii.get();
	while (ch >= 0)
	{
		part += (char) ch;
		ch = ii.get();
	}
	assertTrue (part == "this is part 2\r\n");

	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testReadEmptyLines()
{
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is\r\npart 1\r\n\r\n--MIME_boundary_01234567\r\n\r\nthis\r\n\r\nis part 2\r\n\r\n\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is\r\npart 1\r\n");
	assertTrue (r.hasNextPart());
	r.nextPart(h);
	assertTrue (h.empty());
	std::istream& ii = r.stream();
	part.clear();
	ch = ii.get();
	while (ch >= 0)
	{
		part += (char) ch;
		ch = ii.get();
	}
	assertTrue (part == "this\r\n\r\nis part 2\r\n\r\n");

	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testReadLongPart()
{
	std::string longPart(3000, 'X');
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\n");
	s.append(longPart);
	s.append("\r\n--MIME_boundary_01234567\r\n\r\nthis is part 2\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == longPart);
	assertTrue (r.hasNextPart());
	r.nextPart(h);
	assertTrue (h.empty());
	std::istream& ii = r.stream();
	part.clear();
	ch = ii.get();
	while (ch >= 0)
	{
		part += (char) ch;
		ch = ii.get();
	}
	assertTrue (part == "this is part 2");

	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testGuessBoundary()
{
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr);
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (r.boundary() == "MIME_boundary_01234567");
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (!r.hasNextPart());
	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testPreamble()
{
	std::string s("this is the\r\npreamble\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (!r.hasNextPart());
	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testBadBoundary()
{
	std::string s("\r\n--MIME_boundary_01234567\r\nname1: value1\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567--\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_7654321");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	try
	{
		r.nextPart(h);
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testEmptyBoundary()
{
	std::string s("--\r\nname1: value1\r\n\r\nthis is part 1\r\n----\r\n");
	std::istringstream istr(s);
	MultipartReader r(istr);
	MessageHeader h;
	try
	{
		r.nextPart(h);
		fail("empty boundary - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testRobustness()
{
	std::string s("--MIME_boundary_01234567\rname1: value1\r\n\nthis is part 1\n--MIME_boundary_01234567--");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (!r.hasNextPart());
	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testUnixLineEnds()
{
	std::string s("\n--MIME_boundary_01234567\nname1: value1\n\nthis is part 1\n--MIME_boundary_01234567\n\nthis is part 2\n\n--MIME_boundary_01234567--\n");
	std::istringstream istr(s);
	MultipartReader r(istr, "MIME_boundary_01234567");
	assertTrue (r.hasNextPart());
	MessageHeader h;
	r.nextPart(h);
	assertTrue (h.size() == 1);
	assertTrue (h["name1"] == "value1");
	std::istream& i = r.stream();
	int ch = i.get();
	std::string part;
	while (ch >= 0)
	{
		part += (char) ch;
		ch = i.get();
	}
	assertTrue (part == "this is part 1");
	assertTrue (r.hasNextPart());
	r.nextPart(h);
	assertTrue (h.empty());
	std::istream& ii = r.stream();
	part.clear();
	ch = ii.get();
	while (ch >= 0)
	{
		part += (char) ch;
		ch = ii.get();
	}
	assertTrue (part == "this is part 2\n");

	try
	{
		r.nextPart(h);
		fail("no more parts - must throw");
	}
	catch (MultipartException&)
	{
	}
}


void MultipartReaderTest::testBoundaryAtWindowEnd()
{
	// The sizes of the long part move the delimiter after it across the end of
	// the first and of the second read window (32768 bytes).
	const std::string twoParts("--MIME_boundary_01234567\r\nname1: value1\r\n\r\n%s\r\n--MIME_boundary_01234567\r\n\r\nthis is part 2\r\n--MIME_boundary_01234567--\r\n");
	const std::string twoPartsUnix(Poco::replace(twoParts, "\r\n", "\n"));
	const std::string onePart("--MIME_boundary_01234567\r\nname1: value1\r\n\r\n%s\r\n--MIME_boundary_01234567--\r\n");
	for (const int firstSize: {32600, 64800})
	{
		for (int size = firstSize; size <= firstSize + 200; ++size)
		{
			const std::string longPart(static_cast<std::size_t>(size), 'X');
			const std::vector<std::string> both{longPart, "this is part 2"};
			const std::vector<std::string> single{longPart};
			loop_1_assert (size, readParts(Poco::format(twoParts, longPart)) == both);
			loop_1_assert (size, readParts(Poco::format(twoPartsUnix, longPart)) == both);
			loop_1_assert (size, readParts(Poco::format(onePart, longPart)) == single);
		}
	}
}


void MultipartReaderTest::testDelimiterLookalikes()
{
	// Prefixes of the dash-boundary, lines that differ from it in one byte and
	// runs of dashes are content, also for boundaries that repeat characters.
	for (const std::string& boundary: {std::string("MIME_boundary_01234567"), std::string("---------------------------974767299852498929531610575"), std::string("b")})
	{
		const std::string dashBoundary("--" + boundary);
		std::string body(std::string(100, '-').append("\r\n"));
		for (std::size_t k = 0; k < dashBoundary.size(); ++k)
		{
			std::string lookalike(dashBoundary);
			lookalike[k] = 'x';
			body.append(dashBoundary, 0, k).append("\r\n").append(lookalike).append("\r\n").append(lookalike).append("\n");
		}
		const std::vector<std::string> parts{body, "this is part 2"};
		const std::string crlf(dashBoundary + "\r\n\r\n" + body + "\r\n" + dashBoundary + "\r\n\r\nthis is part 2\r\n" + dashBoundary + "--\r\n");
		const std::string lf(dashBoundary + "\n\n" + body + "\n" + dashBoundary + "\n\nthis is part 2\n" + dashBoundary + "--\n");
		assertTrue (readParts(crlf, boundary) == parts);
		assertTrue (readParts(lf, boundary) == parts);
	}
}


void MultipartReaderTest::testGuessBoundaryEmptyArgument()
{
	// An empty boundary given to the constructor is replaced by the guessed one.
	std::istringstream istr("--MIME_boundary_01234567\r\n\r\nthis is part 1\r\n--MIME_boundary_01234567\r\n\r\nthis is part 2\r\n--MIME_boundary_01234567--\r\n");
	MultipartReader r(istr, "");
	std::vector<std::string> parts;
	while (r.hasNextPart())
	{
		MessageHeader h;
		r.nextPart(h);
		StreamCopier::copyToString(r.stream(), parts.emplace_back());
	}
	const std::vector<std::string> expected{"this is part 1", "this is part 2"};
	assertTrue (parts == expected);
	assertEqual (std::string("MIME_boundary_01234567"), r.boundary());
}


void MultipartReaderTest::testPartContentLengthLargerThanBody()
{
	const std::vector<std::string> parts{"this is part 1", "this is part 2"};
	assertTrue (readParts(twoPartsWithContentLength("73")) == parts);
}


void MultipartReaderTest::testPartContentLengthZero()
{
	const std::vector<std::string> parts{"this is part 1", "this is part 2"};
	assertTrue (readParts(twoPartsWithContentLength("0")) == parts);
}


void MultipartReaderTest::testPartContentLengthSmallerThanBody()
{
	const std::vector<std::string> parts{"this is part 1", "this is part 2"};
	assertTrue (readParts(twoPartsWithContentLength("4")) == parts);
}


void MultipartReaderTest::setUp()
{
}


void MultipartReaderTest::tearDown()
{
}


CppUnit::Test* MultipartReaderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MultipartReaderTest");

	CppUnit_addTest(pSuite, MultipartReaderTest, testReadOnePart);
	CppUnit_addTest(pSuite, MultipartReaderTest, testReadTwoParts);
	CppUnit_addTest(pSuite, MultipartReaderTest, testReadEmptyLines);
	CppUnit_addTest(pSuite, MultipartReaderTest, testReadLongPart);
	CppUnit_addTest(pSuite, MultipartReaderTest, testGuessBoundary);
	CppUnit_addTest(pSuite, MultipartReaderTest, testPreamble);
	CppUnit_addTest(pSuite, MultipartReaderTest, testBadBoundary);
	CppUnit_addTest(pSuite, MultipartReaderTest, testEmptyBoundary);
	CppUnit_addTest(pSuite, MultipartReaderTest, testRobustness);
	CppUnit_addTest(pSuite, MultipartReaderTest, testUnixLineEnds);
	CppUnit_addTest(pSuite, MultipartReaderTest, testBoundaryAtWindowEnd);
	CppUnit_addTest(pSuite, MultipartReaderTest, testDelimiterLookalikes);
	CppUnit_addTest(pSuite, MultipartReaderTest, testGuessBoundaryEmptyArgument);
	CppUnit_addTest(pSuite, MultipartReaderTest, testPartContentLengthLargerThanBody);
	CppUnit_addTest(pSuite, MultipartReaderTest, testPartContentLengthZero);
	CppUnit_addTest(pSuite, MultipartReaderTest, testPartContentLengthSmallerThanBody);

	return pSuite;
}
