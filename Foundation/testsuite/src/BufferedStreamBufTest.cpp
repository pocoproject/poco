//
// BufferedStreamBufTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BufferedStreamBufTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/BufferedStreamBuf.h"
#include "Poco/BufferedBidirectionalStreamBuf.h"
#include "Poco/FIFOBuffer.h"
#include "Poco/FIFOBufferStream.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <optional>
#include <ostream>


using Poco::BufferedStreamBuf;
using Poco::BufferedBidirectionalStreamBuf;
using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;


namespace
{
	constexpr std::streamsize BUFFER_SIZE = 64;
	constexpr std::size_t DATA_SIZE = 200;
	constexpr std::streamsize MAX_WRITES[] = {1, 7, 10};

	// Fits the buffer, so the stream holds all of it until the flush.
	constexpr std::size_t PARTIAL_DATA_SIZE = 50;
	constexpr std::streamsize PARTIAL_MAX_WRITE = 7;
	constexpr std::streamsize ACCEPTED_BEFORE_FAILURE = 10;
	// Poco devices such as FileStreamBuf report a failure with -1.
	constexpr std::streamsize FAILURE_RESULTS[] = {0, -1};

	constexpr std::size_t FIFO_SIZE = 16;
	// FIFOBufferStreamBuf's buffer is 4 bytes larger than the FIFO, so these
	// bytes stay buffered until the flush and do not fit into the FIFO.
	constexpr std::size_t FIFO_EXCESS = 2;


	template <typename BufferBase>
	class ShortWriteStreamBuf: public BufferBase
		/// An output streambuf with a BUFFER_SIZE byte buffer whose device
		/// accepts at most a given number of bytes per writeToDevice() call
		/// and collects them in a string. The device can be made to fail
		/// after a number of bytes until recover() is called.
	{
	public:
		ShortWriteStreamBuf() = delete;

		explicit ShortWriteStreamBuf(std::streamsize maxWrite);
			/// Creates the streambuf; the device accepts at most maxWrite
			/// bytes per call.

		~ShortWriteStreamBuf() override = default;
			/// Destroys the streambuf.

		void failAfter(std::streamsize bytes, std::streamsize result);
			/// After accepting the given number of bytes, writeToDevice()
			/// returns result, which must be 0 or negative; a further call
			/// before recover() throws a Poco::IllegalStateException.

		void throwAfter(std::streamsize bytes);
			/// After accepting the given number of bytes, writeToDevice()
			/// throws a Poco::IOException.

		void recover();
			/// Makes the device accept data again.

		[[nodiscard]] const std::string& data() const;
			/// Returns the bytes the device accepted.

		[[nodiscard]] bool calledAfterFailure() const;
			/// Returns true if writeToDevice() was called again after it had
			/// returned the failAfter() result and before recover() was called.

	private:
		std::streamsize writeToDevice(const char* buffer, std::streamsize length) override;

		std::streamsize _maxWrite;
		std::optional<std::streamsize> _budget;
		std::streamsize _failResult = 0;
		bool _throw = false;
		bool _failureReported = false;
		bool _calledAfterFailure = false;
		std::string _data;
	};


	template <typename BufferBase>
	ShortWriteStreamBuf<BufferBase>::ShortWriteStreamBuf(std::streamsize maxWrite):
		BufferBase(BUFFER_SIZE, std::ios::out),
		_maxWrite(maxWrite)
	{
	}


	template <typename BufferBase>
	void ShortWriteStreamBuf<BufferBase>::failAfter(std::streamsize bytes, std::streamsize result)
	{
		_budget = bytes;
		_failResult = result;
		_throw = false;
		_failureReported = false;
	}


	template <typename BufferBase>
	void ShortWriteStreamBuf<BufferBase>::throwAfter(std::streamsize bytes)
	{
		_budget = bytes;
		_throw = true;
	}


	template <typename BufferBase>
	void ShortWriteStreamBuf<BufferBase>::recover()
	{
		_budget.reset();
		_failureReported = false;
	}


	template <typename BufferBase>
	const std::string& ShortWriteStreamBuf<BufferBase>::data() const
	{
		return _data;
	}


	template <typename BufferBase>
	bool ShortWriteStreamBuf<BufferBase>::calledAfterFailure() const
	{
		return _calledAfterFailure;
	}


	template <typename BufferBase>
	std::streamsize ShortWriteStreamBuf<BufferBase>::writeToDevice(const char* buffer, std::streamsize length)
	{
		std::streamsize n = std::min(length, _maxWrite);
		if (_budget.has_value())
		{
			if (*_budget == 0)
			{
				if (_throw) throw Poco::IOException("device failure");
				if (_failureReported)
				{
					// A buffer that ignores the failure would call the device forever.
					_calledAfterFailure = true;
					throw Poco::IllegalStateException("writeToDevice() called again after a failure");
				}
				_failureReported = true;
				return _failResult;
			}
			n = std::min(n, *_budget);
			*_budget -= n;
		}
		_data.append(buffer, static_cast<std::size_t>(n));
		return n;
	}


	[[nodiscard]] std::string makeData(std::size_t size)
		/// Returns size bytes of printable test data.
	{
		std::string data;
		for (std::size_t i = 0; i < size; ++i)
		{
			data += static_cast<char>('a' + i % ('z' - 'a' + 1));
		}
		return data;
	}


	[[nodiscard]] std::string drain(FIFOBuffer& fifo)
		/// Removes the bytes in the FIFO and returns them.
	{
		std::string data(fifo.used(), '\0');
		data.resize(fifo.read(data.data(), data.size()));
		return data;
	}
}


BufferedStreamBufTest::BufferedStreamBufTest(const std::string& name): CppUnit::TestCase(name)
{
}


BufferedStreamBufTest::~BufferedStreamBufTest() = default;


void BufferedStreamBufTest::testShortWrites()
{
	checkShortWrites<BufferedStreamBuf>();
}


void BufferedStreamBufTest::testBidirectionalShortWrites()
{
	checkShortWrites<BufferedBidirectionalStreamBuf>();
}


void BufferedStreamBufTest::testFailureAfterPartialWrite()
{
	checkFailureAfterPartialWrite<BufferedStreamBuf>();
}


void BufferedStreamBufTest::testBidirectionalFailureAfterPartialWrite()
{
	checkFailureAfterPartialWrite<BufferedBidirectionalStreamBuf>();
}


void BufferedStreamBufTest::testExceptionAfterPartialWrite()
{
	checkExceptionAfterPartialWrite<BufferedStreamBuf>();
}


void BufferedStreamBufTest::testBidirectionalExceptionAfterPartialWrite()
{
	checkExceptionAfterPartialWrite<BufferedBidirectionalStreamBuf>();
}


void BufferedStreamBufTest::testFIFOBufferStreamDrainedAfterFull()
{
	FIFOBuffer fifo(FIFO_SIZE);
	FIFOBufferStream stream(fifo);
	const std::string input = makeData(FIFO_SIZE + FIFO_EXCESS);

	stream << input;
	stream.flush();
	assertTrue (stream.bad());
	assertTrue (fifo.isFull());

	std::string received = drain(fifo);
	stream.clear();
	stream.flush();
	assertTrue (stream.good());
	received += drain(fifo);
	assertEqual (input, received);
}


template <typename BufferBase>
void BufferedStreamBufTest::checkShortWrites()
{
	const std::string input = makeData(DATA_SIZE);
	for (const auto maxWrite: MAX_WRITES)
	{
		ShortWriteStreamBuf<BufferBase> buf(maxWrite);
		std::ostream os(&buf);
		os << input;
		os.flush();
		assertTrue (os.good());
		assertEqual (input, buf.data());
	}
}


template <typename BufferBase>
void BufferedStreamBufTest::checkFailureAfterPartialWrite()
{
	const std::string input = makeData(PARTIAL_DATA_SIZE);
	for (const auto failResult: FAILURE_RESULTS)
	{
		ShortWriteStreamBuf<BufferBase> buf(PARTIAL_MAX_WRITE);
		buf.failAfter(ACCEPTED_BEFORE_FAILURE, failResult);
		std::ostream os(&buf);
		os << input;
		os.flush();
		assertTrue (os.bad());
		assertFalse (buf.calledAfterFailure());
		assertEqual (input.substr(0, static_cast<std::size_t>(ACCEPTED_BEFORE_FAILURE)), buf.data());

		buf.recover();
		os.clear();
		os.flush();
		assertTrue (os.good());
		assertEqual (input, buf.data());
	}
}


template <typename BufferBase>
void BufferedStreamBufTest::checkExceptionAfterPartialWrite()
{
	const std::string input = makeData(PARTIAL_DATA_SIZE);
	ShortWriteStreamBuf<BufferBase> buf(PARTIAL_MAX_WRITE);
	buf.throwAfter(ACCEPTED_BEFORE_FAILURE);
	std::ostream os(&buf);
	os.exceptions(std::ios::badbit);
	os << input;
	try
	{
		os.flush();
		fail("flush must throw");
	}
	catch (const Poco::IOException&)
	{
	}
	assertTrue (os.bad());
	assertEqual (input.substr(0, static_cast<std::size_t>(ACCEPTED_BEFORE_FAILURE)), buf.data());

	buf.recover();
	os.clear();
	os.flush();
	assertTrue (os.good());
	assertEqual (input, buf.data());
}


void BufferedStreamBufTest::setUp()
{
}


void BufferedStreamBufTest::tearDown()
{
}


CppUnit::Test* BufferedStreamBufTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BufferedStreamBufTest");

	CppUnit_addTest(pSuite, BufferedStreamBufTest, testShortWrites);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testBidirectionalShortWrites);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testFailureAfterPartialWrite);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testBidirectionalFailureAfterPartialWrite);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testExceptionAfterPartialWrite);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testBidirectionalExceptionAfterPartialWrite);
	CppUnit_addTest(pSuite, BufferedStreamBufTest, testFIFOBufferStreamDrainedAfterFull);

	return pSuite;
}
