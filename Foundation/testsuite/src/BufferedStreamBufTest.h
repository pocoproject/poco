//
// BufferedStreamBufTest.h
//
// Definition of the BufferedStreamBufTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef BufferedStreamBufTest_INCLUDED
#define BufferedStreamBufTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class BufferedStreamBufTest: public CppUnit::TestCase
	/// Tests how BufferedStreamBuf and BufferedBidirectionalStreamBuf
	/// flush their buffer to a device that writes only part of it.
{
public:
	BufferedStreamBufTest() = delete;
	explicit BufferedStreamBufTest(const std::string& name);
	~BufferedStreamBufTest() override;

	void testShortWrites();
	void testBidirectionalShortWrites();
	void testFailureAfterPartialWrite();
	void testBidirectionalFailureAfterPartialWrite();
	void testExceptionAfterPartialWrite();
	void testBidirectionalExceptionAfterPartialWrite();
	void testFIFOBufferStreamDrainedAfterFull();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
	template <typename BufferBase>
	void checkShortWrites();
		/// Writes more than the buffer holds through a device that accepts
		/// only a few bytes per call; the device receives all of it.

	template <typename BufferBase>
	void checkFailureAfterPartialWrite();
		/// The device fails after a partial write, reporting the failure
		/// with 0 and with -1; the buffer does not call it again, and after
		/// it recovers, a retried flush writes the rest exactly once.

	template <typename BufferBase>
	void checkExceptionAfterPartialWrite();
		/// The device throws after a partial write; the exception reaches
		/// the caller, and after recovery a retried flush writes the rest
		/// exactly once.
};


#endif // BufferedStreamBufTest_INCLUDED
