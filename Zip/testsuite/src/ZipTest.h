//
// ZipTest.h
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
#include <string_view>
#include <vector>


class ZipTest: public CppUnit::TestCase
{
public:
	ZipTest(const std::string& name);
	~ZipTest();

	void testSkipSingleFile();
	void testDecompressSingleFile();
	void testDecompressSingleFileInDir();
	void testDecompress();
	void testDecompressConsistency();
	void testDecompressFlat();
	void testDecompressVuln();
	void testDecompressFlatVuln();
	void testCrcAndSizeAfterData();
	void testCrcAndSizeAfterDataWithArchive();
	void testCrcAndSizeAfterDataEncapsulated();

	static constexpr Poco::UInt64 KB = 1024;
	static constexpr Poco::UInt64 MB = 1024*KB;
	void verifyDataFile(const std::string& path, Poco::UInt64 size);
	void testDecompressZip64();
	void testValidPath();
	void testMalformedZip64();
	void testUnknownMethodAndHostSystem();
	void testUnsupportedVersionNeeded();
	void testDuplicateEndOfCentralDirectory();
	void testIllegalHeader();
	void testReplaceUnsupportedMethod();
	void testDecompressFailedEntryWithDataDescriptor();
	void testSizeOrOffsetOutOfRange();
	void testKeepEntryBeyondEndOfArchive();
	void testDataDescriptorEntryAbove4GiB();
	void testEmptyFileName();
	void testDuplicateFileNames();
	void testTruncatedArchive();
	void testTruncatedDataDescriptor();
	void testDataDescriptorCompressedSizeOutOfRange();
	void testSyncAtEndOfArchive();
	void testNulCharacterInName();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

	static std::string getTestFile(const std::string& directory, const std::string& type);

private:
	void verifyTruncatedArchive(const std::vector<std::string>& records, std::string_view fileComment = {}, std::string_view archiveComment = {});
		/// Checks that a prefix of the archive made of the records is read, with the entry, the file
		/// comment and as much of the archive comment as it holds, exactly when it ends at a record
		/// boundary, within a record signature or within the archive comment, and that every other
		/// prefix is rejected with an IOException. The last record has the archive comment.

	void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);

	int _errCnt;
};


#endif // ZipTest_INCLUDED
