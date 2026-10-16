//
// HTTPChunkedStreamTest.h
//
// Definition of the HTTPChunkedStreamTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPChunkedStreamTest_INCLUDED
#define HTTPChunkedStreamTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class HTTPChunkedStreamTest: public CppUnit::TestCase
{
public:
	explicit HTTPChunkedStreamTest(const std::string& name);
	~HTTPChunkedStreamTest() override;

	void testValid();
	void testTrailer();
	void testInvalidChunkSize();
	void testBareLineFeed();
	void testMissingCRLFAfterData();
	void testTruncated();
	void testReadAfterTimeout();
	void testReadInPartsNonBlocking();
	void testTrailerTooLong();
	void testWriteFoldedTrailer();
	void testWriteCloseOnce();
	void testWriteTrailerFailure();
	void testWriteFailure();
	void testWriteFailureKeepsCause();
	void testWriteAfterPartialChunk();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
};


#endif // HTTPChunkedStreamTest_INCLUDED
