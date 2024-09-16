//
// FileStreamRWLockTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FileStreamRWLockTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FileStreamRWLock.h"
#include "Poco/FileStream.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/ProcessRunner.h"
#include "Poco/Path.h"
#include "Poco/Stopwatch.h"


using Poco::FileStreamRWLock;
using Poco::Thread;
using Poco::Runnable;
using Poco::FileStream;
using Poco::TemporaryFile;
using Poco::ProcessRunner;
using Poco::Path;
using Poco::Stopwatch;

class FileStreamRWLockRunnable: public Runnable
{
public:
	FileStreamRWLockRunnable(std::string fileLock): _fileLock(std::move(fileLock)) 
	{
		std::string name("TestApp");
#if defined(_DEBUG)
		Poco::File testapp(name);
		if (!testapp.exists())
		{
			name += "d";
		}
#endif
		_cmd = name;
		
	}

	void run() override
	{
		std::string pid = Poco::format("%spid%ld.file", Path::tempHome(), Thread::currentOsTid());
		std::vector<std::string> args;
		args.push_back(std::string("--lock-file=").append(_fileLock));
		args.push_back(std::string("--pidfile=").append(pid));
		ProcessRunner pr(_cmd, args, pid, 0, 10, true);
		while (pr.running())
		{
			Poco::Thread::sleep(100);
		}
		int result = pr.result();
		_ok = (result >= 0);
	}

	bool ok() const
	{
		return _ok;
	}

private:
	std::string _fileLock;
	bool _ok{false};
	std::string _cmd;
};


class FileStreamRWTryLockRunnable: public Runnable
{
public:

	FileStreamRWTryLockRunnable(std::string fileLock): _fileLock(std::move(fileLock))
	{
		std::string name("TestApp");
#if defined(_DEBUG)
		Poco::File testapp(name);
		if (!testapp.exists())
		{
			name += "d";
		}
#endif
		_cmd = name;
		
	}

	void run() override
	{
		std::string pid = Poco::format("%spid%ld.file", Path::tempHome(), Thread::currentOsTid());
		std::vector<std::string> args;
		args.push_back(std::string("--trylock-file=").append(_fileLock));
		args.push_back(std::string("--pidfile=").append(pid));
		ProcessRunner pr(_cmd, args, pid, 0, 10, true);
		while (pr.running())
		{
			Poco::Thread::sleep(100);
		}
		int result = pr.result();
		_ok = (result >= 0);
	}

	bool ok() const
	{
		return _ok;
	}

private:
	std::string _fileLock;
	bool _ok{false};
	std::string _cmd;
};


FileStreamRWLockTest::FileStreamRWLockTest(const std::string& name): CppUnit::TestCase(name)
{
}


FileStreamRWLockTest::~FileStreamRWLockTest()
{
}


void FileStreamRWLockTest::testFSLock()
{
	TemporaryFile fl;
	FileStream fs(fl.path(), std::ios::out | std::ios::in | std::ios::binary);
	Poco::Int32 i32 = 0;
	fs.seekp(0, std::ios::beg);
	fs.write((const char *)&i32, sizeof(i32));
	fs.flushToDisk();

	const auto &path = fl.path();
	FileStreamRWLockRunnable r1(path);
	FileStreamRWLockRunnable r2(path);
	FileStreamRWLockRunnable r3(path);
	FileStreamRWLockRunnable r4(path);
	FileStreamRWLockRunnable r5(path);
	Thread t1;
	Thread t2;
	Thread t3;
	Thread t4;
	Thread t5;
	t1.start(r1);
	t2.start(r2);
	t3.start(r3);
	t4.start(r4);
	t5.start(r5);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	fs.seekp(0, std::ios::beg);
	fs.read((char *)&i32, sizeof(i32));
	assertEquals(50000, i32);
	assertTrue (r1.ok());
	assertTrue (r2.ok());
	assertTrue (r3.ok());
	assertTrue (r4.ok());
	assertTrue (r5.ok());
}


void FileStreamRWLockTest::testFSTryLock()
{
	TemporaryFile fl;
	FileStream fs(fl.path(), std::ios::out | std::ios::in | std::ios::binary);
	Poco::Int32 i32 = 0;
	fs.seekp(0, std::ios::beg);
	fs.write((const char *)&i32, sizeof(i32));
	fs.flushToDisk();
	
	const auto &path = fl.path();
	FileStreamRWTryLockRunnable r1(path);
	FileStreamRWTryLockRunnable r2(path);
	FileStreamRWTryLockRunnable r3(path);
	FileStreamRWTryLockRunnable r4(path);
	FileStreamRWTryLockRunnable r5(path);
	Thread t1;
	Thread t2;
	Thread t3;
	Thread t4;
	Thread t5;
	t1.start(r1);
	t2.start(r2);
	t3.start(r3);
	t4.start(r4);
	t5.start(r5);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	
	fs.seekp(0, std::ios::beg);
	fs.read((char *)&i32, sizeof(i32));
	assertTrue (i32 == 50000);
	assertTrue (r1.ok());
	assertTrue (r2.ok());
	assertTrue (r3.ok());
	assertTrue (r4.ok());
	assertTrue (r5.ok());
}


void FileStreamRWLockTest::setUp()
{
}


void FileStreamRWLockTest::tearDown()
{
}


CppUnit::Test* FileStreamRWLockTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FileStreamRWLockTest");

	CppUnit_addLongTest(pSuite, FileStreamRWLockTest, testFSLock);
	CppUnit_addLongTest(pSuite, FileStreamRWLockTest, testFSTryLock);

	return pSuite;
}
