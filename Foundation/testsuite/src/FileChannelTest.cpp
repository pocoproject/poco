//
// FileChannelTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FileChannelTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Exception.h"
#include "Poco/RotateStrategy.h"
#include "Poco/ArchiveStrategy.h"
#include "Poco/PurgeStrategy.h"
#include <vector>
#include <iostream>


using Poco::FileChannel;
using Poco::Message;
using Poco::AutoPtr;
using Poco::TemporaryFile;
using Poco::Thread;
using Poco::File;
using Poco::Path;
using Poco::Timestamp;
using Poco::NumberFormatter;
using Poco::DateTime;
using Poco::LocalDateTime;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::DirectoryIterator;
using Poco::InvalidArgumentException;


FileChannelTest::FileChannelTest(const std::string& name): CppUnit::TestCase(name)
{
}


FileChannelTest::~FileChannelTest()
{
}


void FileChannelTest::testRotateNever()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "never");
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


void FileChannelTest::testFlushing()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_FLUSH, "false");
		pChannel->open();
		Message msg("source", "01234567890123456789", Message::PRIO_INFORMATION);
		pChannel->log(msg);

		// File shall be there and have content after writing first message.
		File f(name);
		assertTrue (f.exists());
		assertTrue (f.getSize() >= 20);

		Timestamp::TimeDiff noFlushTime;
		{
			Timestamp start;
			for (int i = 0; i < 2000; ++i)
			{
				pChannel->log(msg);
			}
			pChannel->close();
			Timestamp end;
			noFlushTime = end-start;
		}
		Timestamp::TimeDiff flushTime;
		{
			pChannel->setProperty(FileChannel::PROP_FLUSH, "true");
			pChannel->open();
			Timestamp start;
			for (int i = 0; i < 2000; ++i)
			{
				pChannel->log(msg);
			}
			pChannel->close();
			Timestamp end;
			flushTime = end-start;
		}

		// Writing to channel with flushing is expected to be slower.
		assertTrue(flushTime > noFlushTime);
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateBySize()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "2 K");
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f(name + ".0");
		assertTrue (f.exists());
		f = name + ".1";
		assertTrue (f.exists());
		f = name + ".2";
		assertTrue (!f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateByAge()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "2 seconds");
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 15; ++i)
		{
			pChannel->log(msg);
			Thread::sleep(300);
		}
		File f(name + ".0");
		assertTrue (f.exists());
		f = name + ".1";
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeDayUTC()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "utc");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<DateTime>(DAY_HOUR_MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeDayLocal()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "local");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<LocalDateTime>(DAY_HOUR_MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeHourUTC()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "utc");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<DateTime>(HOUR_MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeHourLocal()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "local");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<LocalDateTime>(HOUR_MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeMinUTC()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "utc");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<DateTime>(MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testRotateAtTimeMinLocal()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_TIMES, "local");
		pChannel->setProperty(FileChannel::PROP_ROTATION, rotation<LocalDateTime>(MIN));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		int min = DateTime().minute();
		while (DateTime().minute() == min)
		{
			pChannel->log(msg);
			Thread::sleep(1000);
		}
		pChannel->log(msg);
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


class RotateByCustomStrategy : public Poco::RotateStrategy
	/// The file is rotated when the log file
	/// exceeds a given age.
	///
	/// For this to work reliably across all platforms and file systems
	/// (there are severe issues on most platforms finding out the real
	/// creation date of a file), the creation date of the file is
	/// written into the log file as the first entry.
{
public:
	bool mustRotate(Poco::LogFile* pFile)
	{
		return pFile->size() > 2000;
	}

private:
};


void FileChannelTest::testRotateByStrategy()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);

		// this test rotates at 2k just like testRotateBySize. Set the prop rotation to 50k to verify that the rotation strategy takes over
		pChannel->setProperty(FileChannel::PROP_ROTATION, "50 K");
		pChannel->setRotationStrategy(new RotateByCustomStrategy());
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f(name + ".0");
		assertTrue(f.exists());
		f = name + ".1";
		assertTrue(f.exists());
		f = name + ".2";
		assertTrue(!f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testArchive()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "2 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f(name + ".0");
		assertTrue (f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


class ArchiveByCustomNumberStrategy : public Poco::ArchiveStrategy
	/// A monotonic increasing number is appended to the
	/// log file name. The most recent archived file
	/// always has the number zero.
{
public:
	Poco::LogFile* open(Poco::LogFile* pFile)
	{
		return pFile;
	}


	Poco::LogFile* archive(Poco::LogFile* pFile)
	{
		std::string basePath = pFile->path();
		delete pFile;
		int n = -1;
		std::string path;
		do
		{
			path = basePath;
			path = path.substr(0, path.length() - 4);
			path.append("_");
			NumberFormatter::append(path, ++n);
			path.append(".log");
		} while (exists(path));

		while (n >= 0)
		{
			std::string oldPath = basePath;
			if (n > 0)
			{
				oldPath = oldPath.substr(0, oldPath.length() - 4);
				oldPath.append("_");
				NumberFormatter::append(oldPath, n - 1);
				oldPath.append(".log");
			}
			std::string newPath = basePath;
			newPath = newPath.substr(0, newPath.length() - 4);
			newPath.append("_");
			NumberFormatter::append(newPath, n);
			newPath.append(".log");
			moveFile(oldPath, newPath);
			--n;
		}
		return new Poco::LogFile(basePath);
	}
};


void FileChannelTest::testArchiveByStrategy()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "2 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");

		pChannel->setArchiveStrategy(new ArchiveByCustomNumberStrategy());

		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		name = name.substr(0, name.length() - 4);
		name.append("_0.log");
		
		File f(name);
		assertTrue(f.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testCompress()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_COMPRESS, "true");
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		Thread::sleep(3000); // allow time for background compression
		File f0(name + ".0.gz");
		assertTrue (f0.exists());
		File f1(name + ".1.gz");
		assertTrue (f1.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testCompressedRotation()
{
	static const uint32_t MAX_ROLLOVER_TIMES = 8;
	static const uint32_t LONG_MESSAGE_LENGTH = 1024;
	static const uint32_t LONG_MAX_FILESIZE = 1024;

	std::vector<uint8_t> longMessage(LONG_MESSAGE_LENGTH, '&');
	longMessage.push_back(0);

	Poco::Path logsPath(Poco::Path::current(), "logs");
	Poco::File logsDir(logsPath.toString());
	if (logsDir.exists())
		logsDir.remove(true);

	logsDir.createDirectory();
	logsPath.append("test.log");

	Poco::AutoPtr<Poco::FileChannel> fileChannel = new Poco::FileChannel("ABC");
	fileChannel->setProperty(Poco::FileChannel::PROP_PATH, logsPath.toString());
	fileChannel->setProperty(Poco::FileChannel::PROP_FLUSH, "false");
	fileChannel->setProperty(Poco::FileChannel::PROP_ROTATION, "1 M");
	fileChannel->setProperty(Poco::FileChannel::PROP_PURGECOUNT, "5");
	fileChannel->setProperty(Poco::FileChannel::PROP_ARCHIVE, "number");
	fileChannel->setProperty(Poco::FileChannel::PROP_TIMES, "local");
	fileChannel->setProperty(Poco::FileChannel::PROP_COMPRESS, "true");

	fileChannel->open();

	std::string text(longMessage.begin(), longMessage.end());

	for (uint32_t i = 1; i <= MAX_ROLLOVER_TIMES; ++i)
	{
	    for (uint32_t j = 0; j < LONG_MAX_FILESIZE; ++j)
	    {
	        Poco::Message message("ABC", text, Poco::Message::PRIO_INFORMATION);
	        fileChannel->log(message);
	    }
	}

	fileChannel->close();

	std::vector<std::string> files;
	logsDir.list(files);
	std::sort(files.begin(), files.end());

	for (const auto& f: files)
		std::cout << "log file: " << f << std::endl;

	assertEqual(5+1+1, files.size()); // 5+1 rotated files, current file
	assertEqual("test.log", files[0]);
	assertEqual("test.log.0.gz", files[1]);
	assertEqual("test.log.1.gz", files[2]);
	assertEqual("test.log.2.gz", files[3]);
	assertEqual("test.log.3.gz", files[4]);
	assertEqual("test.log.4.gz", files[5]);
	assertEqual("test.log.5.gz", files[6]);

	logsDir.remove(true);
}


void FileChannelTest::purgeAge(const std::string& pa)
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_PURGEAGE, pa);
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f0(name + ".0");
		assertTrue (f0.exists());
		File f1(name + ".1");
		assertTrue (f1.exists());
		File f2(name + ".2");
		assertTrue (f2.exists());

		Thread::sleep(5000);
		for (int i = 0; i < 50; ++i)
		{
			pChannel->log(msg);
		}

		assertTrue (!f2.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::noPurgeAge(const std::string& npa)
{
	std::string name = filename();

	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_PURGEAGE, npa);
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
		}
		File f0(name + ".0");
		assertTrue (f0.exists());
		File f1(name + ".1");
		assertTrue (f1.exists());
		File f2(name + ".2");
		assertTrue (f2.exists());

		Thread::sleep(5000);
		for (int i = 0; i < 50; ++i)
		{
			pChannel->log(msg);
		}

		assertTrue (f2.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testPurgeAge()
{
		purgeAge("5 seconds");
		try
		{
				noPurgeAge("0 seconds");
				fail ("must fail");
		} catch (InvalidArgumentException&)
		{
		}

		noPurgeAge("");
		noPurgeAge("none");
}


void FileChannelTest::purgeCount(const std::string& pc)
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_PURGECOUNT, pc);
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
			Thread::sleep(50);
		}
		File f0(name + ".0");
		assertTrue (f0.exists());
		File f1(name + ".1");
		assertTrue (f1.exists());
		File f2(name + ".2");
		assertTrue (!f2.exists());
	} catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::noPurgeCount(const std::string& npc)
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_PURGECOUNT, npc);
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
			Thread::sleep(50);
		}
		File f0(name + ".0");
		assertTrue (f0.exists());
		File f1(name + ".1");
		assertTrue (f1.exists());
		File f2(name + ".2");
		assertTrue (f2.exists());
	} catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::testPurgeCount()
{
	purgeCount("2");
	try
	{
		noPurgeCount("0");
		fail("must fail");
	} catch (InvalidArgumentException&)
	{
	}

	noPurgeCount("");
	noPurgeCount("none");
}


void FileChannelTest::testWrongPurgeOption()
{
	std::string name = filename();
	AutoPtr<FileChannel> pChannel = new FileChannel(name);
	pChannel->setProperty(FileChannel::PROP_PURGEAGE, "5 seconds");

	try
	{
		pChannel->setProperty(FileChannel::PROP_PURGEAGE, "peace");
		fail("must fail");
	} catch (InvalidArgumentException&)
	{
		assertTrue (pChannel->getProperty(FileChannel::PROP_PURGEAGE) == "5 seconds");
	}

	try
	{
		pChannel->setProperty(FileChannel::PROP_PURGECOUNT, "peace");
		fail("must fail");
	} catch (InvalidArgumentException&)
	{
		assertTrue (pChannel->getProperty(FileChannel::PROP_PURGEAGE) == "5 seconds");
	}

	remove(name);
}


void FileChannelTest::testPurgeByStrategy()
{
	std::string name = filename();
	try
	{
		AutoPtr<FileChannel> pChannel = new FileChannel(name);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "1 K");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");
		pChannel->setProperty(FileChannel::PROP_PURGECOUNT, "");
		// simpler to test the type that already exists. A true "custom" purge strategy might be time based or total size based
		pChannel->setPurgeStrategy(new Poco::PurgeByCountStrategy(2));
		pChannel->open();
		Message msg("source", "This is a log file entry", Message::PRIO_INFORMATION);
		for (int i = 0; i < 200; ++i)
		{
			pChannel->log(msg);
			Thread::sleep(50);
		}
		File f0(name + ".0");
		assertTrue(f0.exists());
		File f1(name + ".1");
		assertTrue(f1.exists());
		File f2(name + ".2");
		assertTrue(!f2.exists());
	}
	catch (...)
	{
		remove(name);
		throw;
	}
	remove(name);
}


void FileChannelTest::setUp()
{
}


void FileChannelTest::tearDown()
{
}


void FileChannelTest::remove(const std::string& baseName)
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
		} catch (...)
		{
		}
	}
}


std::string FileChannelTest::filename() const
{
	std::string name = "log_";
	name.append(DateTimeFormatter::format(Timestamp(), "%Y%m%d%H%M%S"));
	name.append(".log");
	return name;
}


template <class DT>
std::string FileChannelTest::rotation(TimeRotation rtype) const
{
	DT now;
	std::string rotation;

	int day  = now.dayOfWeek();
	int min  = now.minute();
	int hour = now.hour();
	if (++min == 60)
	{
		++hour;
		min = 0;
	}
	if (hour == 24)
	{
		hour = 0;
		++day;
		day %= 7;
	}

	switch (rtype)
	{
	case DAY_HOUR_MIN: // day,hh:m,
		rotation = DateTimeFormat::WEEKDAY_NAMES[day];
		rotation += ',' + NumberFormatter::format0(hour, 2) + ':' + NumberFormatter::format0(min, 2);
		break;
	case HOUR_MIN: // hh:mm
		rotation = NumberFormatter::format0(hour, 2) + ':' + NumberFormatter::format0(min, 2);
		break;
	case MIN: // mm
		rotation = ':' + NumberFormatter::format0(min, 2);
		break;
	default:
		rotation = "";
		break;
	}
	return rotation;
}


CppUnit::Test* FileChannelTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FileChannelTest");

	CppUnit_addTest(pSuite, FileChannelTest, testRotateNever);
	CppUnit_addTest(pSuite, FileChannelTest, testFlushing);
	CppUnit_addTest(pSuite, FileChannelTest, testRotateBySize);
	CppUnit_addTest(pSuite, FileChannelTest, testRotateByAge);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeDayUTC);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeDayLocal);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeHourUTC);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeHourLocal);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeMinUTC);
	CppUnit_addLongTest(pSuite, FileChannelTest, testRotateAtTimeMinLocal);
	CppUnit_addTest(pSuite, FileChannelTest, testRotateByStrategy);
	CppUnit_addTest(pSuite, FileChannelTest, testArchive);
	CppUnit_addTest(pSuite, FileChannelTest, testArchiveByStrategy);
	CppUnit_addTest(pSuite, FileChannelTest, testCompress);
	CppUnit_addTest(pSuite, FileChannelTest, testCompressedRotation);
	CppUnit_addLongTest(pSuite, FileChannelTest, testPurgeAge);
	CppUnit_addTest(pSuite, FileChannelTest, testPurgeCount);
	CppUnit_addTest(pSuite, FileChannelTest, testWrongPurgeOption);
	CppUnit_addTest(pSuite, FileChannelTest, testPurgeByStrategy);

	return pSuite;
}
