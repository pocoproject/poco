//
// FileTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FileTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include <fstream>
#include <set>


using Poco::File;
using Poco::TemporaryFile;
using Poco::Path;
using Poco::Exception;
using Poco::Timestamp;
using Poco::Thread;


FileTest::FileTest(const std::string& name): CppUnit::TestCase(name)
{
}


FileTest::~FileTest()
{
}


void FileTest::testFileAttributes1()
{
	File f("testfile.dat");
	assertTrue (!f.exists());

	try
	{
		bool POCO_UNUSED flag = f.canRead();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		bool POCO_UNUSED flag = f.canWrite();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		bool POCO_UNUSED flag = f.isFile();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		bool POCO_UNUSED flag = f.isDirectory();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Timestamp POCO_UNUSED ts = f.created();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Timestamp POCO_UNUSED ts = f.getLastModified();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Timestamp ts;
		f.setLastModified(ts);
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		File::FileSize POCO_UNUSED fs = f.getSize();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.setSize(0);
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.setWriteable();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.setReadOnly();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.copyTo("copy.dat");
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.moveTo("copy.dat");
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.renameTo("copy.dat");
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		f.remove();
		failmsg("file does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}
}


void FileTest::testCreateFile()
{
	File f("testfile.dat");
	bool created = f.createFile();
	assertTrue (created);
	assertTrue (!f.isHidden());
	created = f.createFile();
	assertTrue (!created);
}


void FileTest::testFileAttributes2()
{
	TemporaryFile f;
	bool created = f.createFile();
	Timestamp ts;
	assertTrue (created);

	assertTrue (f.exists());
	assertTrue (f.canRead());
	assertTrue (f.canWrite());
	assertTrue (f.isFile());
	assertTrue (!f.isDirectory());
	Timestamp tsc = f.created();
	Timestamp tsm = f.getLastModified();
	assertTrue (tsc - ts >= -2000000 && tsc - ts <= 2000000);
	assertTrue (tsm - ts >= -2000000 && tsm - ts <= 2000000);

	f.setWriteable(false);
	assertTrue (!f.canWrite());
	assertTrue (f.canRead());

	f.setReadOnly(false);
	assertTrue (f.canWrite());
	assertTrue (f.canRead());

	ts = Timestamp::fromEpochTime(1000000);
	f.setLastModified(ts);
	assertTrue (f.getLastModified() == ts);
}


void FileTest::testFileAttributes3()
{
#if defined(POCO_OS_FAMILY_UNIX)
#if POCO_OS==POCO_OS_CYGWIN
	File f("/dev/tty");
#else
 	File f("/dev/null");
#endif
#elif defined(POCO_OS_FAMILY_WINDOWS)
	File f("CON");
#endif

	assertTrue (f.isDevice());
	assertTrue (!f.isFile());
	assertTrue (!f.isDirectory());
}


void FileTest::testCompare()
{
	File f1("abc.txt");
	File f2("def.txt");
	File f3("abc.txt");

	assertTrue (f1 == f3);
	assertTrue (!(f1 == f2));
	assertTrue (f1 != f2);
	assertTrue (!(f1 != f3));
	assertTrue (!(f1 == f2));
	assertTrue (f1 < f2);
	assertTrue (f1 <= f2);
	assertTrue (!(f2 < f1));
	assertTrue (!(f2 <= f1));
	assertTrue (f2 > f1);
	assertTrue (f2 >= f1);
	assertTrue (!(f1 > f2));
	assertTrue (!(f1 >= f2));

	assertTrue (f1 <= f3);
	assertTrue (f1 >= f3);
}


void FileTest::testRootDir()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	File f1("/");
	File f2("c:/");
	File f3("c:\\");
	File f4("\\");
	assertTrue (f1.exists());
	assertTrue (f2.exists());
	assertTrue (f3.exists());
	assertTrue (f4.exists());
#else
	File f1("/");
	assertTrue (f1.exists());
#endif
}


void FileTest::testSwap()
{
	File f1("abc.txt");
	File f2("def.txt");
	f1.swap(f2);
	assertTrue (f1.path() == "def.txt");
	assertTrue (f2.path() == "abc.txt");
}


void FileTest::testSize()
{
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();
	File f("testfile.dat");
	assertTrue (f.getSize() > 0);
	f.setSize(0);
	assertTrue (f.getSize() == 0);
}


void FileTest::testDirectory()
{
	File d("testdir");
	try
	{
		d.remove(true);
	}
	catch (...)
	{
	}
	TemporaryFile::registerForDeletion("testdir");

	bool created = d.createDirectory();
	assertTrue (created);
	assertTrue (d.isDirectory());
	assertTrue (!d.isFile());
	std::vector<std::string> files;
	d.list(files);
	assertTrue (files.empty());

	File f = Path("testdir/file1", Path::PATH_UNIX);
	f.createFile();
	f = Path("testdir/file2", Path::PATH_UNIX);
	f.createFile();
	f = Path("testdir/file3", Path::PATH_UNIX);
	f.createFile();

	d.list(files);
	assertTrue (files.size() == 3);

	std::set<std::string> fs;
	fs.insert(files.begin(), files.end());
	assertTrue (fs.find("file1") != fs.end());
	assertTrue (fs.find("file2") != fs.end());
	assertTrue (fs.find("file3") != fs.end());

	File dd(Path("testdir/testdir2/testdir3", Path::PATH_UNIX));
	dd.createDirectories();
	assertTrue (dd.exists());
	assertTrue (dd.isDirectory());

	File ddd(Path("testdir/testdirB/testdirC/testdirD", Path::PATH_UNIX));
	ddd.createDirectories();
	assertTrue (ddd.exists());
	assertTrue (ddd.isDirectory());

	d.remove(true);
}


void FileTest::testCopy()
{
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	TemporaryFile f2;
	f1.setReadOnly().copyTo(f2.path());
	assertTrue (f2.exists());
	assertTrue (!f2.canWrite());
	assertTrue (f1.getSize() == f2.getSize());
	f1.setWriteable().remove();
}


void FileTest::testCopyFailIfDestinationFileExists()
{
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	TemporaryFile f2;
	f2.createFile();
	try {
		f1.setReadOnly().copyTo(f2.path(), File::OPT_FAIL_ON_OVERWRITE);
		failmsg("file exist - must throw exception");
	} catch (Exception&) {
	}
	f1.setWriteable().remove();
}


void FileTest::testMove()
{
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	File::FileSize sz = f1.getSize();
	TemporaryFile f2;
	f1.moveTo(f2.path());
	assertTrue (f2.exists());
	assertTrue (f2.getSize() == sz);
	assertTrue (f1.exists());
	assertTrue (f1 == f2);
}


void FileTest::testMoveFailIfDestinationFileExists() {
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	TemporaryFile f2;
	f2.createFile();
	try {
		f1.moveTo(f2.path(), File::OPT_FAIL_ON_OVERWRITE);
		failmsg("file exist - must throw exception");
	} catch (Exception&) {
	}
	f1.setWriteable().remove();
}


void FileTest::testCopyDirectory()
{
	Path pd1("testdir");
	File fd1(pd1);
	try
	{
		fd1.remove(true);
	}
	catch (...)
	{
	}
	fd1.createDirectories();
	Path pd2(pd1, "subdir");
	File fd2(pd2);
	fd2.createDirectories();
	Path pf1(pd1, "testfile1.dat");
	std::ofstream ostr1(pf1.toString().c_str());
	ostr1 << "Hello, world!" << std::endl;
	ostr1.close();
	Path pf2(pd1, "testfile2.dat");
	std::ofstream ostr2(pf2.toString().c_str());
	ostr2 << "Hello, world!" << std::endl;
	ostr2.close();
	Path pf3(pd2, "testfile3.dat");
	std::ofstream ostr3(pf3.toString().c_str());
	ostr3 << "Hello, world!" << std::endl;
	ostr3.close();

	File fd3("testdir2");

	try
	{
		fd3.remove(true);
	}
	catch (...)
	{
	}

	fd1.copyTo("testdir2");

	Path pd1t("testdir2");
	File fd1t(pd1t);
	assertTrue (fd1t.exists());
	assertTrue (fd1t.isDirectory());

	Path pd2t(pd1t, "subdir");
	File fd2t(pd2t);
	assertTrue (fd2t.exists());
	assertTrue (fd2t.isDirectory());

	Path pf1t(pd1t, "testfile1.dat");
	File ff1t(pf1t);
	assertTrue (ff1t.exists());
	assertTrue (ff1t.isFile());

	Path pf2t(pd1t, "testfile2.dat");
	File ff2t(pf2t);
	assertTrue (ff2t.exists());
	assertTrue (ff2t.isFile());

	Path pf3t(pd2t, "testfile3.dat");
	File ff3t(pf3t);
	assertTrue (ff3t.exists());
	assertTrue (ff3t.isFile());

	fd1.remove(true);
	fd3.remove(true);
}


void FileTest::testCopyDirectoryFailIfExists()
{
	Path pd1("testdir");
	File fd1(pd1);
	try {
		fd1.remove(true);
	} catch (...) {
	}
	fd1.createDirectories();
	Path pf1(pd1, "testfile1.dat");
	std::ofstream ostr1(pf1.toString().c_str());
	ostr1 << "Hello, world!" << std::endl;
	ostr1.close();
	Path pf2(pd1, "testfile2.dat");
	std::ofstream ostr2(pf2.toString().c_str());
	ostr2 << "Hello, world!" << std::endl;
	ostr2.close();

	Path pd2("destination");
	File fd2(pd2);
	try {
		fd2.remove(true);
	} catch (...) {
	}
	fd2.createDirectories();
	Path pd3(pd2, "testdir");
	File fd3(pd3);
	fd3.createDirectories();

	try {
		fd1.copyTo("testdir", File::OPT_FAIL_ON_OVERWRITE);
		failmsg("Destination Directory exists - must throw exception");
	} catch (Exception&) {
	}

	fd1.remove(true);
	fd2.remove(true);
}


void FileTest::testRename()
{
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	File f2("testfile2.dat");
	f1.renameTo(f2.path());

	assertTrue (f2.exists());
	assertTrue (f1.exists());
	assertTrue (f1 == f2);

	f2.remove();
}


void FileTest::testRenameFailIfExists() {
	std::ofstream ostr("testfile.dat");
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	File f1("testfile.dat");
	File f2("testfile2.dat");
	f2.createFile();

	try {
		f1.renameTo(f2.path(), File::OPT_FAIL_ON_OVERWRITE);
		failmsg("file exists - must throw exception");
	} catch (Exception&) {
	}

	f1.renameTo(f2.path());

	assertTrue(f2.exists());
	assertTrue(f1.exists());
	assertTrue(f1 == f2);

	f2.remove();
}


void FileTest::testLongPath()
{
#if defined(_WIN32)
	Poco::Path p("longpathtest");
	p.makeAbsolute();
	std::string longpath(p.toString());
	while (longpath.size() < MAX_PATH*4)
	{
		longpath.append("\\");
		longpath.append(64, 'x');
	}

	Poco::File d(longpath);
	d.createDirectories();

	assertTrue (d.exists());
	assertTrue (d.isDirectory());

	Poco::File f(p.toString());
	f.remove(true);
#endif
}

void FileTest::testUnixFileExtension()
{
	std::string filePath1 = "/a/b/c/.notextension";
	Poco::Path path1(filePath1, Poco::Path::Style::PATH_UNIX);

	assertEqual(".notextension", path1.getBaseName());
	assertEqual("", path1.getExtension());

	std::string filePath2 = "/a/b/c/emptyextension.";
	Poco::Path path2(filePath2, Poco::Path::Style::PATH_UNIX);

	assertEqual("emptyextension", path2.getBaseName());
	assertEqual("", path2.getExtension());
}


void FileTest::testTemporaryFile()
{
	const int COUNT = 10000;
	std::set<std::string> paths;
	for (int i = 0; i < COUNT; i++)
	{
		Poco::TemporaryFile f;
		paths.insert(f.path());
	}
	assertTrue (paths.size() == COUNT);
}


void FileTest::setUp()
{
	File f("testfile.dat");
	try
	{
		f.remove();
	}
	catch (...)
	{
	}
}


void FileTest::tearDown()
{
	File f("testfile.dat");
	try
	{
		f.remove();
	}
	catch (...)
	{
	}
}


CppUnit::Test* FileTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FileTest");

	CppUnit_addTest(pSuite, FileTest, testCreateFile);
	CppUnit_addTest(pSuite, FileTest, testFileAttributes1);
	CppUnit_addTest(pSuite, FileTest, testFileAttributes2);
	CppUnit_addTest(pSuite, FileTest, testFileAttributes3);
	CppUnit_addTest(pSuite, FileTest, testCompare);
	CppUnit_addTest(pSuite, FileTest, testSwap);
	CppUnit_addTest(pSuite, FileTest, testSize);
	CppUnit_addTest(pSuite, FileTest, testDirectory);
	CppUnit_addTest(pSuite, FileTest, testCopy);
	CppUnit_addTest(pSuite, FileTest, testCopyFailIfDestinationFileExists);
	CppUnit_addTest(pSuite, FileTest, testMove);
	CppUnit_addTest(pSuite, FileTest, testMoveFailIfDestinationFileExists);
	CppUnit_addTest(pSuite, FileTest, testCopyDirectory);
	CppUnit_addTest(pSuite, FileTest, testCopyDirectoryFailIfExists);
	CppUnit_addTest(pSuite, FileTest, testRename);
	CppUnit_addTest(pSuite, FileTest, testRenameFailIfExists);
	CppUnit_addTest(pSuite, FileTest, testRootDir);
	CppUnit_addTest(pSuite, FileTest, testLongPath);
	CppUnit_addTest(pSuite, FileTest, testUnixFileExtension);
	CppUnit_addTest(pSuite, FileTest, testTemporaryFile);

	return pSuite;
}
