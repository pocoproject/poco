//
// SimpleFileChannelTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SimpleFileChannel.h"
#include "Poco/Message.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "SimpleFileChannelTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <atomic>
#include <thread>


using Poco::SimpleFileChannel;
using Poco::Message;
using Poco::Path;
using Poco::File;
using Poco::DirectoryIterator;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::AutoPtr;
using Poco::InvalidArgumentException;


SimpleFileChannelTest::SimpleFileChannelTest(const std::string& name): CppUnit::TestCase(name)
{
}


SimpleFileChannelTest::~SimpleFileChannelTest()
{
}


void SimpleFileChannelTest::testRotate()
{
	std::string name = filename();
	try
	{
		AutoPtr<SimpleFileChannel> pChannel = new SimpleFileChannel(name);
		pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, "2 K");
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f(name);
		assertTrue (f.exists());
		f = name + ".0";
		assertTrue (f.exists());
		assertTrue (f.getSize() >= 2048);
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void SimpleFileChannelTest::testRotateBySizeUnits()
{
	std::string name = filename();
	try
	{
		AutoPtr<SimpleFileChannel> pChannel = new SimpleFileChannel(name);
		pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, "1 G");
		assertTrue (pChannel->getProperty(SimpleFileChannel::PROP_ROTATION) == "1 G");
		pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, "4 G");
		assertTrue (pChannel->getProperty(SimpleFileChannel::PROP_ROTATION) == "4 G");
		try
		{
			pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, "1 X");
			fail("must fail");
		}
		catch (InvalidArgumentException&)
		{
		}
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f(name);
		assertTrue (f.exists());
		f = name + ".0";
		assertTrue (!f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void SimpleFileChannelTest::testRotationOverflow()
{
	AutoPtr<SimpleFileChannel> pChannel = new SimpleFileChannel(filename());
	pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, "2 K");
	const auto assertRejected = [&](const std::string& rotation)
	{
		try
		{
			pChannel->setProperty(SimpleFileChannel::PROP_ROTATION, rotation);
			fail("must fail: " + rotation);
		}
		catch (InvalidArgumentException& exc)
		{
			assertTrue (exc.message().find(rotation) != std::string::npos);
		}
		assertTrue (pChannel->getProperty(SimpleFileChannel::PROP_ROTATION) == "2 K");
	};
	assertRejected("99999999999999999 K");
	assertRejected("99999999999999 M");
	assertRejected("99999999999 G");
	assertRejected("1234567890123456789012345");
}


void SimpleFileChannelTest::testPathWhileSetPath()
{
	// The paths are read while another thread sets them: every path read is
	// one that was set.
	// Only ThreadSanitizer shows the unlocked read reliably.
	constexpr int ROUNDS = 10000;
	const std::string first = "log_path_first.log";
	const std::string second = "log_path_second_with_a_longer_name.log";

	AutoPtr<SimpleFileChannel> pChannel = new SimpleFileChannel(first);
	pChannel->setProperty(SimpleFileChannel::PROP_SECONDARYPATH, second);
	std::atomic<bool> stop(false);
	std::thread setter([&]()
	{
		for (int i = 0; i < ROUNDS; ++i)
		{
			pChannel->setProperty(SimpleFileChannel::PROP_PATH, i % 2 == 0 ? second : first);
			pChannel->setProperty(SimpleFileChannel::PROP_SECONDARYPATH, i % 2 == 0 ? first : second);
		}
		stop = true;
	});

	const auto wasSet = [&](const std::string& path)
	{
		return path == first || path == second;
	};
	int unexpected = 0;
	while (!stop)
	{
		if (!wasSet(pChannel->path())) ++unexpected;
		if (!wasSet(pChannel->secondaryPath())) ++unexpected;
		if (!wasSet(pChannel->getProperty(SimpleFileChannel::PROP_PATH))) ++unexpected;
	}
	setter.join();

	assertEqual (0, unexpected);
	assertEqual (first, pChannel->path());
	assertEqual (second, pChannel->secondaryPath());
}


void SimpleFileChannelTest::testSizeWhileClosing()
{
	// size() and creationDate() are called while another thread opens and
	// closes the channel, which creates and deletes its LogFile.
	// Only ThreadSanitizer or AddressSanitizer show the use after free reliably.
	constexpr int ROUNDS = 2000;
	std::string name = filename();
	try
	{
		AutoPtr<SimpleFileChannel> pChannel = new SimpleFileChannel(name);
		std::atomic<bool> stop(false);
		std::thread writer([&]()
		{
			Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
			for (int i = 0; i < ROUNDS; ++i)
			{
				pChannel->log(msg);
				pChannel->close();
			}
			stop = true;
		});

		// A plausibility check of the values read, not of the use after free:
		// the log file only grows, so an open file never reports a smaller size
		// than an earlier read of an open file.
		Poco::UInt64 lastSize = 0;
		int shrunk = 0;
		while (!stop)
		{
			const Poco::UInt64 size = pChannel->size();
			const Timestamp created = pChannel->creationDate();
			if (size != 0 && created != 0)
			{
				if (size < lastSize) ++shrunk;
				lastSize = size;
			}
		}
		writer.join();

		assertEqual (0, shrunk);
		assertEqual (0, pChannel->size());
		assertTrue (pChannel->creationDate() == 0);
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void SimpleFileChannelTest::setUp()
{
}


void SimpleFileChannelTest::tearDown()
{
}


void SimpleFileChannelTest::remove(const std::string& baseName)
{
	DirectoryIterator it(Path::current());
	DirectoryIterator end;
	std::vector<std::string> files;
	while (it != end)
	{
		if (it.name().find(baseName) == 0)
		{
			files.push_back(it.name());
		}
		++it;
	}
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		try
		{
			File f(*it);
			f.remove();
		}
		catch (...)
		{
		}
	}
}


std::string SimpleFileChannelTest::filename() const
{
	std::string name = "log_";
	name.append(DateTimeFormatter::format(Timestamp(), "%Y%m%d%H%M%S"));
	name.append(".log");
	return name;
}


CppUnit::Test* SimpleFileChannelTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SimpleFileChannelTest");

	CppUnit_addTest(pSuite, SimpleFileChannelTest, testRotate);
	CppUnit_addTest(pSuite, SimpleFileChannelTest, testRotateBySizeUnits);
	CppUnit_addTest(pSuite, SimpleFileChannelTest, testRotationOverflow);
	CppUnit_addTest(pSuite, SimpleFileChannelTest, testPathWhileSetPath);
	CppUnit_addTest(pSuite, SimpleFileChannelTest, testSizeWhileClosing);

	return pSuite;
}
