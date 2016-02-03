//
// ZipTest.h
//
// $Id: //poco/1.4/Zip/testsuite/src/ZipTest.h#1 $
//
// Definition of the ZipTest class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef ZipTest_INCLUDED
#define ZipTest_INCLUDED


#include "Poco/Zip/Zip.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "CppUnit/TestCase.h"


class ZipTest: public CppUnit::TestCase
{
public:
	ZipTest(const std::string& name);
	~ZipTest();

	void testSkipSingleFile();
	void testDecompressSingleFile();
	void testDecompressSingleFileInDir();
	void testDecompress();
	void testCrcAndSizeAfterData();
	void testCrcAndSizeAfterDataWithArchive();

	void testDecompressFlat();

	static const Poco::UInt64 KB = 1024;
	static const Poco::UInt64 MB = 1024*KB;
	void verifyDataFile(const std::string& path, Poco::UInt64 size);
	void testDecompressZip64();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

	static std::string getTestFile(const std::string& directory, const std::string& type);

private:
	void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
	
	int _errCnt;
};


#endif // ZipTest_INCLUDED
