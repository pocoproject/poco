//
// FileStreamTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FileStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FileStream.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Exception.h"


FileStreamTest::FileStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


FileStreamTest::~FileStreamTest()
{
}


void FileStreamTest::testRead()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	char tmp[]={'\xc3', '\x84', '\xc3', '\x96', '\xc3', '\x9c', '\xc3', '\xa4', '\xc3', '\xb6', '\xc3', '\xbc', '\0'};
	std::string file(tmp);
	file.append(".txt");
#else
	std::string file("testfile.txt");
#endif

	Poco::TemporaryFile::registerForDeletion(file);

	Poco::FileOutputStream fos(file, std::ios::binary);
	fos << "sometestdata";
	fos.close();

	Poco::FileInputStream fis(file);
	assertTrue (fis.good());
	std::string read;
	fis >> read;
	assertTrue (!read.empty());
}


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/UnicodeConverter.h"
#else
#include <fcntl.h>
#endif

void FileStreamTest::testWriteReadNativeHandle()
{
	Poco::FileOutputStream fos;
	Poco::FileInputStream fis;
	Poco::FileIOS::NativeHandle outHandle;

#if defined(POCO_OS_FAMILY_WINDOWS)
	char tmp[]={'\xc3', '\x84', '\xc3', '\x96', '\xc3', '\x9c', '\xc3', '\xa4', '\xc3', '\xb6', '\xc3', '\xbc', '\0'};
	std::string file(tmp);
	file.append(".txt");
	std::wstring utf16Path;
	Poco::UnicodeConverter::toUTF16(file, utf16Path);
    outHandle = CreateFileW(utf16Path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    assertTrue(outHandle != INVALID_HANDLE_VALUE);
#else
	std::string file("testfile.txt");
	outHandle = ::open(file.c_str(), O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
#endif

	Poco::TemporaryFile::registerForDeletion(file);

    fos.openHandle(outHandle, std::ios::binary | std::ios::out | std::ios::trunc);
	fos << "sometestdata";
	fos.close();

    Poco::FileIOS::NativeHandle inHandle;

#if defined(POCO_OS_FAMILY_WINDOWS)
    inHandle = CreateFileW(utf16Path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    assertTrue(inHandle != INVALID_HANDLE_VALUE);
#else
    inHandle = ::open(file.c_str(), O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
#endif

	fis.openHandle(inHandle, std::ios::in);
	assertTrue (fis.good());
	std::string read;
	fis >> read;
	assertTrue (!read.empty());
}


void FileStreamTest::testWrite()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	char tmp[]={'\xc3', '\x9f', '\xc3', '\x84', '\xc3', '\x96', '\xc3', '\x9c', '\xc3', '\xa4', '\xc3', '\xb6', '\xc3', '\xbc', '\0'};
	std::string file(tmp);
	file = "dummy_" + file + ".txt";
#else
	std::string file("dummy_file.txt");
#endif

	Poco::TemporaryFile::registerForDeletion(file);

	Poco::FileOutputStream fos(file);
	assertTrue (fos.good());
	fos << "hiho";
	fos.close();

	Poco::FileInputStream fis(file);
	assertTrue (fis.good());
	std::string read;
	fis >> read;
	assertTrue (read == "hiho");
}


void FileStreamTest::testReadWrite()
{
	char tmp[]={'\xc3', '\x9f', '\xc3', '\x84', '\xc3', '\x96', '\xc3', '\x9c', '\xc3', '\xa4', '\xc3', '\xb6', '\xc3', '\xbc', '\0'};
	std::string file(tmp);
	file = "dummy_" + file + ".txt";

	Poco::TemporaryFile::registerForDeletion(file);

	Poco::FileStream fos(file);
	assertTrue (fos.good());
	fos << "hiho";
	fos.seekg(0, std::ios::beg);
	std::string read;
	fos >> read;
	assertTrue (read == "hiho");
}


void FileStreamTest::testOpen()
{
	Poco::FileOutputStream ostr;
	ostr.open("test.txt", std::ios::out);
	assertTrue (ostr.good());
	ostr.close();
}


void FileStreamTest::testOpenModeIn()
{
	Poco::File f("nonexistent.txt");
	if (f.exists())
		f.remove();

	try
	{
		Poco::FileInputStream istr("nonexistent.txt");
		fail("nonexistent file - must throw");
	}
	catch (Poco::Exception&)
	{
	}

	f.createFile();
	Poco::FileInputStream istr("nonexistent.txt");
	assertTrue (istr.good());
}


void FileStreamTest::testOpenModeOut()
{
	Poco::File f("test.txt");
	if (f.exists())
		f.remove();

	Poco::FileOutputStream ostr1("test.txt");
	ostr1 << "Hello, world!";
	ostr1.close();

	assertTrue (f.exists());
	assertTrue (f.getSize() != 0);

	Poco::FileStream str1("test.txt");
	str1.close();

	assertTrue (f.exists());
	assertTrue (f.getSize() != 0);

	Poco::FileOutputStream ostr2("test.txt");
	ostr2.close();

	assertTrue (f.exists());
	assertTrue (f.getSize() == 0);

	f.remove();
}


void FileStreamTest::testOpenModeTrunc()
{
	Poco::File f("test.txt");
	if (f.exists())
		f.remove();

	Poco::FileOutputStream ostr1("test.txt");
	ostr1 << "Hello, world!";
	ostr1.close();

	assertTrue (f.exists());
	assertTrue (f.getSize() != 0);

	Poco::FileStream str1("test.txt", std::ios::trunc);
	str1.close();

	assertTrue (f.exists());
	assertTrue (f.getSize() == 0);

	f.remove();
}


void FileStreamTest::testOpenModeAte()
{
	Poco::FileOutputStream ostr("test.txt");
	ostr << "0123456789";
	ostr.close();

	Poco::FileStream str1("test.txt", std::ios::in | std::ios::ate);
	int c = str1.get();
	assertTrue (str1.eof());

	str1.clear();
	str1.seekg(0);
	c = str1.get();
	assertTrue (c == '0');

	str1.close();

	Poco::FileStream str2("test.txt", std::ios::in | std::ios::out | std::ios::ate);
	str2 << "abcdef";
	str2.seekg(0);
	std::string s;
	str2 >> s;
	assertTrue (s == "0123456789abcdef");
	str2.close();
}


void FileStreamTest::testOpenModeApp()
{
	Poco::FileOutputStream ostr("test.txt");
	ostr << "0123456789";
	ostr.close();

	Poco::FileStream str1("test.txt", std::ios::in | std::ios::out | std::ios::app);

	str1 << "abc";

	str1.seekp(0);

	str1 << "def";

	str1.close();

	Poco::FileInputStream istr("test.txt");
	std::string s;
	istr >> s;
	assertTrue (s == "0123456789abcdef");
	istr.close();
}


void FileStreamTest::testSeek()
{
	Poco::FileStream str("test.txt", std::ios::in | std::ios::out | std::ios::trunc);
	str << "0123456789abcdef";

	str.seekg(0);
	int c = str.get();
	assertTrue (c == '0');

	str.seekg(10);
	assertTrue (str.tellg() == std::streampos(10));
	c = str.get();
	assertTrue (c == 'a');
	assertTrue (str.tellg() == std::streampos(11));

	str.seekg(-1, std::ios::end);
	assertTrue (str.tellg() == std::streampos(15));
	c = str.get();
	assertTrue (c == 'f');
	assertTrue (str.tellg() == std::streampos(16));

	str.seekg(-1, std::ios::cur);
	assertTrue (str.tellg() == std::streampos(15));
	c = str.get();
	assertTrue (c == 'f');
	assertTrue (str.tellg() == std::streampos(16));

	str.seekg(-4, std::ios::cur);
	assertTrue (str.tellg() == std::streampos(12));
	c = str.get();
	assertTrue (c == 'c');
	assertTrue (str.tellg() == std::streampos(13));

	str.seekg(1, std::ios::cur);
	assertTrue (str.tellg() == std::streampos(14));
	c = str.get();
	assertTrue (c == 'e');
	assertTrue (str.tellg() == std::streampos(15));
}


void FileStreamTest::testMultiOpen()
{
	Poco::FileStream str("test.txt", std::ios::out | std::ios::trunc);
	str << "0123456789\n";
	str << "abcdefghij\n";
	str << "klmnopqrst\n";
	str.close();

	std::string s;
	str.open("test.txt", std::ios::in);
	std::getline(str, s);
	assertTrue (s == "0123456789");
	str.close();

	str.open("test.txt", std::ios::in);
	std::getline(str, s);
	assertTrue (s == "0123456789");
	str.close();
}


void FileStreamTest::setUp()
{
}


void FileStreamTest::tearDown()
{
}


CppUnit::Test* FileStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FileStreamTest");

	CppUnit_addTest(pSuite, FileStreamTest, testRead);
	CppUnit_addTest(pSuite, FileStreamTest, testWriteReadNativeHandle);
	CppUnit_addTest(pSuite, FileStreamTest, testWrite);
	CppUnit_addTest(pSuite, FileStreamTest, testReadWrite);
	CppUnit_addTest(pSuite, FileStreamTest, testOpen);
	CppUnit_addTest(pSuite, FileStreamTest, testOpenModeIn);
	CppUnit_addTest(pSuite, FileStreamTest, testOpenModeOut);
	CppUnit_addTest(pSuite, FileStreamTest, testOpenModeTrunc);
	CppUnit_addTest(pSuite, FileStreamTest, testOpenModeAte);
	CppUnit_addTest(pSuite, FileStreamTest, testOpenModeApp);
	CppUnit_addTest(pSuite, FileStreamTest, testSeek);
	CppUnit_addTest(pSuite, FileStreamTest, testMultiOpen);

	return pSuite;
}
