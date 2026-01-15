//
// PipeTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PipeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Pipe.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <atomic>
#include <cstring>


using Poco::Pipe;
using Poco::Thread;
using Poco::Runnable;


PipeTest::PipeTest(const std::string& name): CppUnit::TestCase(name)
{
}


PipeTest::~PipeTest()
{
}


void PipeTest::testReadWrite()
{
	Pipe pipe;
	const char* data = "Hello, Pipe!";
	int len = static_cast<int>(std::strlen(data));

	int written = pipe.writeBytes(data, len);
	assertEqual(len, written);

	char buffer[64] = {0};
	int read = pipe.readBytes(buffer, sizeof(buffer));
	assertEqual(len, read);
	assertEqual(std::string(data), std::string(buffer, read));
}


void PipeTest::testClose()
{
	Pipe pipe;
	const char* data = "test";
	int len = static_cast<int>(std::strlen(data));

	int written = pipe.writeBytes(data, len);
	assertEqual(len, written);

	pipe.close();

	// After close, readBytes should return 0
	char buffer[64];
	int read = pipe.readBytes(buffer, sizeof(buffer));
	assertEqual(0, read);

	// After close, writeBytes should return 0
	written = pipe.writeBytes(data, len);
	assertEqual(0, written);
}


void PipeTest::testCloseRead()
{
	Pipe pipe;

	pipe.close(Pipe::CLOSE_READ);

	// After closing read end, readBytes should return 0
	char buffer[64];
	int read = pipe.readBytes(buffer, sizeof(buffer));
	assertEqual(0, read);
}


void PipeTest::testCloseWrite()
{
	Pipe pipe;
	const char* data = "test";
	int len = static_cast<int>(std::strlen(data));

	// Write some data first
	int written = pipe.writeBytes(data, len);
	assertEqual(len, written);

	pipe.close(Pipe::CLOSE_WRITE);

	// After closing write end, writeBytes should return 0
	written = pipe.writeBytes(data, len);
	assertEqual(0, written);

	// Reading existing data should still work
	char buffer[64];
	int read = pipe.readBytes(buffer, sizeof(buffer));
	assertEqual(len, read);
}


void PipeTest::testCopy()
{
	Pipe pipe1;
	Pipe pipe2(pipe1);

	const char* data = "shared pipe";
	int len = static_cast<int>(std::strlen(data));

	// Write through pipe1
	int written = pipe1.writeBytes(data, len);
	assertEqual(len, written);

	// Read through pipe2 (same underlying pipe)
	char buffer[64] = {0};
	int read = pipe2.readBytes(buffer, sizeof(buffer));
	assertEqual(len, read);
	assertEqual(std::string(data), std::string(buffer, read));
}


namespace
{
	class PipeReader: public Runnable
	{
	public:
		PipeReader(Pipe& pipe, std::atomic<int>& readCount, std::atomic<bool>& stop):
			_pipe(pipe),
			_readCount(readCount),
			_stop(stop)
		{
		}

		void run()
		{
			char buffer[64];
			while (!_stop.load(std::memory_order_acquire))
			{
				int n = _pipe.readBytes(buffer, sizeof(buffer));
				if (n > 0)
					_readCount.fetch_add(n, std::memory_order_relaxed);
				else if (n == 0)
					break; // Pipe closed
			}
		}

	private:
		Pipe& _pipe;
		std::atomic<int>& _readCount;
		std::atomic<bool>& _stop;
	};
}


void PipeTest::testCloseRace()
{
	// Test concurrent close and read - this should not crash or hang
	for (int i = 0; i < 100; ++i)
	{
		Pipe pipe;
		std::atomic<int> readCount{0};
		std::atomic<bool> stop{false};

		PipeReader reader(pipe, readCount, stop);
		Thread readerThread;
		readerThread.start(reader);

		// Write some data
		const char* data = "x";
		for (int j = 0; j < 10; ++j)
			pipe.writeBytes(data, 1);

		// Small delay to let reader start
		Thread::sleep(1);

		// Close the pipe while reader might be reading
		pipe.close();

		// Wait for reader to finish
		readerThread.join();
	}
}


void PipeTest::setUp()
{
}


void PipeTest::tearDown()
{
}


CppUnit::Test* PipeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PipeTest");

	CppUnit_addTest(pSuite, PipeTest, testReadWrite);
	CppUnit_addTest(pSuite, PipeTest, testClose);
	CppUnit_addTest(pSuite, PipeTest, testCloseRead);
	CppUnit_addTest(pSuite, PipeTest, testCloseWrite);
	CppUnit_addTest(pSuite, PipeTest, testCopy);
	CppUnit_addTest(pSuite, PipeTest, testCloseRace);

	return pSuite;
}
