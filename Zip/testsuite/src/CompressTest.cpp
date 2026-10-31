//
// CompressTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "CompressTest.h"
#include "ZipTest.h"
#include "Poco/Buffer.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Zip/ZipManipulator.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/ZipUtil.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>


using namespace Poco::Zip;


CompressTest::CompressTest(const std::string& name): CppUnit::TestCase(name)
{
}


CompressTest::~CompressTest()
{
}


void CompressTest::testSingleFile()
{
	Poco::FileOutputStream out(Poco::Path::temp() + "appinf.zip");
	Poco::Path theFile(ZipTest::getTestFile("data", "test.zip"));
	Compress c(out, true);
	c.addFile(theFile, theFile.getFileName());
	ZipArchive a(c.close());
}


void CompressTest::testDirectory()
{
	Poco::FileOutputStream out(Poco::Path::temp() + "pocobin.zip");
	Poco::File aFile("some/");
	if (aFile.exists()) aFile.remove(true);
	Poco::File aDir("some/recursive/dir/");
	aDir.createDirectories();
	Poco::File aDir2("some/other/recursive/dir/");
	aDir2.createDirectories();
	Poco::File aF("some/recursive/dir/test.file");
	(void) aF.createFile();
	Poco::FileOutputStream fos(aF.path());
	fos << "just some test data";
	fos.close();

	Poco::Path theFile(aFile.path());
	theFile.makeDirectory();
	Compress c(out, true);
	c.addRecursive(theFile, ZipCommon::CL_MAXIMUM, false, theFile);
	ZipArchive a(c.close());
	Poco::File(aFile).remove(true);
}


void CompressTest::testManipulator()
{
	{
		Poco::FileOutputStream out(Poco::Path::temp() + "appinf.zip");
		Poco::Path theFile(ZipTest::getTestFile("data", "test.zip"));
		Compress c(out, true);
		c.addFile(theFile, theFile.getFileName());
		ZipArchive a(c.close());
	}
	ZipManipulator zm(Poco::Path::temp() + "appinf.zip", true);
	zm.renameFile("test.zip", "renamedtest.zip");
	zm.addFile("doc/othertest.zip", ZipTest::getTestFile("data", "test.zip"));
	ZipArchive archive=zm.commit();
	assertTrue (archive.findHeader("doc/othertest.zip") != archive.headerEnd());
}


void CompressTest::testManipulatorDel()
{
	{
		Poco::FileOutputStream out(Poco::Path::temp() + "appinf.zip");
		Poco::Path theFile(ZipTest::getTestFile("data", "test.zip"));
		Compress c(out, true);
		c.addFile(theFile, theFile.getFileName());
		ZipArchive a(c.close());
	}
	ZipManipulator zm(Poco::Path::temp() + "appinf.zip", true);
	zm.deleteFile("test.zip");
	zm.addFile("doc/data.zip", ZipTest::getTestFile("data", "data.zip"));
	ZipArchive archive=zm.commit();
	assertTrue (archive.findHeader("test.zip") == archive.headerEnd());
	assertTrue (archive.findHeader("doc/data.zip") != archive.headerEnd());
}


void CompressTest::testManipulatorReplace()
{
	{
		Poco::FileOutputStream out(Poco::Path::temp() + "appinf.zip");
		Poco::Path theFile(ZipTest::getTestFile("data", "test.zip"));
		Compress c(out, true);
		c.addFile(theFile, theFile.getFileName());
		ZipArchive a(c.close());
	}
	ZipManipulator zm(Poco::Path::temp() + "appinf.zip", true);
	zm.replaceFile("test.zip", ZipTest::getTestFile("data", "doc.zip"));

	ZipArchive archive=zm.commit();
	assertTrue (archive.findHeader("test.zip") != archive.headerEnd());
	assertTrue (archive.findHeader("doc.zip") == archive.headerEnd());
}


void CompressTest::testManipulatorZip64DataDescriptor()
{
	Poco::TemporaryFile tempFile;
	const std::string zipFile(tempFile.path());
	{
		// A non-seekable output with forced Zip64 writes a Zip64 data descriptor after each entry.
		Poco::FileOutputStream out(zipFile, std::ios::trunc);
		Compress c(out, false, true);
		c.addFile(ZipTest::getTestFile("data", "test.zip"), "first.zip");
		c.addFile(ZipTest::getTestFile("data", "data.zip"), "second.zip");
		ZipArchive a(c.close());
	}
	{
		ZipManipulator zm(zipFile, false);
		zm.renameFile("first.zip", "renamed.zip");
		ZipArchive archive = zm.commit();
	}
	Poco::FileInputStream in(zipFile);
	ZipArchive archive(in);
	int entries = 0;
	for (ZipArchive::FileInfos::const_iterator it = archive.fileInfoBegin(); it != archive.fileInfoEnd(); ++it)
	{
		// The central directory must point at the local header of each entry.
		char signature[ZipCommon::HEADER_SIZE];
		in.clear();
		in.seekg(static_cast<std::streamoff>(it->second.getOffset()));
		in.read(signature, ZipCommon::HEADER_SIZE);
		assertTrue (in.gcount() == ZipCommon::HEADER_SIZE);
		assertTrue (std::memcmp(signature, ZipLocalFileHeader::HEADER, ZipCommon::HEADER_SIZE) == 0);
		++entries;
	}
	assertEqual (2, entries);
}


void CompressTest::testManipulatorCopiesEntriesWithExtraField()
{
	// Info-ZIP writes the extra fields UT and ux for every entry of extra-field.zip.
	struct Entry
	{
		std::string sourceName;
		std::string name;
		std::string content;
	};
	const Entry entries[] =
	{
		{"kept.txt", "kept.txt", "kept kept kept kept kept kept kept kept kept kept kept kept\n"},
		{"renamed.txt", "new.txt", "renamed renamed renamed renamed renamed renamed renamed renamed\n"}
	};
	const std::string sourceFile(ZipTest::getTestFile("data", "extra-field.zip"));
	Poco::TemporaryFile zipFile;
	Poco::File(sourceFile).copyTo(zipFile.path());
	{
		ZipManipulator zm(zipFile.path(), false);
		zm.renameFile("renamed.txt", "new.txt");
		zm.deleteFile("deleted.txt");
		zm.commit();
	}
	{
		Poco::FileInputStream sourceIn(sourceFile);
		ZipArchive source(sourceIn);
		Poco::FileInputStream in(zipFile.path());
		ZipArchive archive(in);
		archive.checkConsistency();
		assertTrue (archive.findHeader("deleted.txt") == archive.headerEnd());
		for (const auto& entry: entries)
		{
			auto itSource = source.findHeader(entry.sourceName);
			assertTrue (itSource != source.headerEnd());
			assertTrue (itSource->second.hasExtraField());
			auto it = archive.findHeader(entry.name);
			assertTrue (it != archive.headerEnd());
			ZipInputStream zipIn(in, it->second);
			std::ostringstream out(std::ios::binary);
			Poco::StreamCopier::copyStream(zipIn, out);
			assertEqual (entry.content, out.str());
			assertEqual (itSource->second.getExtraField(), it->second.getExtraField());
		}
	}
}


void CompressTest::testManipulatorKeepsAttributesOfCopiedEntries()
{
	// Positions in the central directory header; the version made by holds the host system in its upper byte.
	constexpr Poco::UInt32 VERSION_MADE_BY_POS = 4;
	constexpr Poco::UInt32 INTERNAL_FILE_ATTRIBUTES_POS = 36;
	constexpr Poco::UInt32 EXTERNAL_FILE_ATTRIBUTES_POS = 38;
	constexpr Poco::UInt32 UNIX_FILE_TYPE_MASK = 0170000;
	constexpr Poco::UInt32 UNIX_SYMBOLIC_LINK = 0120000;

	// Info-ZIP on Unix wrote attributes.zip with "zip -y": link.txt is a symbolic link to target.txt
	// and tool.sh has the mode 0755.
	const std::pair<std::string, std::string> entries[] =
	{
		{"link.txt", "link.txt"},
		{"tool.sh", "renamed-tool.sh"}
	};
	const std::string sourceFile(ZipTest::getTestFile("data", "attributes.zip"));
	Poco::TemporaryFile zipFile;
	Poco::File(sourceFile).copyTo(zipFile.path());
	{
		ZipManipulator zm(zipFile.path(), false);
		zm.renameFile("tool.sh", "renamed-tool.sh");
		zm.deleteFile("deleted.txt");
		zm.commit();
	}
	Poco::FileInputStream sourceIn(sourceFile);
	ZipArchive source(sourceIn);
	Poco::FileInputStream in(zipFile.path());
	ZipArchive archive(in);
	assertTrue (archive.findFileInfo("deleted.txt") == archive.fileInfoEnd());
	for (const auto& [sourceName, name]: entries)
	{
		const auto itSource = source.findFileInfo(sourceName);
		assertTrue (itSource != source.fileInfoEnd());
		const auto it = archive.findFileInfo(name);
		assertTrue (it != archive.fileInfoEnd());
		const std::string sourceHeader = itSource->second.createHeader();
		const std::string header = it->second.createHeader();
		assertEqual (ZipUtil::get16BitValue(sourceHeader.data(), VERSION_MADE_BY_POS), ZipUtil::get16BitValue(header.data(), VERSION_MADE_BY_POS));
		assertEqual (ZipUtil::get16BitValue(sourceHeader.data(), INTERNAL_FILE_ATTRIBUTES_POS), ZipUtil::get16BitValue(header.data(), INTERNAL_FILE_ATTRIBUTES_POS));
		assertEqual (ZipUtil::get32BitValue(sourceHeader.data(), EXTERNAL_FILE_ATTRIBUTES_POS), ZipUtil::get32BitValue(header.data(), EXTERNAL_FILE_ATTRIBUTES_POS));
	}
	const auto itLink = archive.findFileInfo("link.txt");
	assertTrue (itLink != archive.fileInfoEnd());
	const std::string linkHeader = itLink->second.createHeader();
	const Poco::UInt32 linkMode = ZipUtil::get32BitValue(linkHeader.data(), EXTERNAL_FILE_ATTRIBUTES_POS) >> 16;
	assertEqual (UNIX_SYMBOLIC_LINK, linkMode & UNIX_FILE_TYPE_MASK);
}


void CompressTest::testManipulatorCopiesEntriesWithZip64DataDescriptor()
{
	struct Entry
	{
		std::string sourceName;
		std::string name;
		std::string content;
	};
	const Entry entries[] =
	{
		{"kept.txt", "kept.txt", "kept kept kept kept kept kept kept kept kept kept kept kept\n"},
		{"renamed.txt", "new.txt", "renamed renamed renamed renamed renamed renamed renamed renamed\n"},
		{"last.txt", "last.txt", "last last last last last last last last last last last last\n"}
	};
	Poco::TemporaryFile zipFile;
	{
		// Without seeking, Compress writes the CRC and the sizes of every entry into a data
		// descriptor after the entry data, and forceZip64 makes it a ZIP64 data descriptor.
		Poco::FileOutputStream out(zipFile.path());
		Compress c(out, false, true);
		for (const auto& entry: entries)
		{
			std::istringstream in(entry.content);
			c.addFile(in, Poco::DateTime(), entry.sourceName);
		}
		c.close();
	}
	{
		ZipManipulator zm(zipFile.path(), false);
		zm.renameFile("renamed.txt", "new.txt");
		zm.commit();
	}
	Poco::FileInputStream in(zipFile.path());
	ZipArchive archive(in);
	archive.checkConsistency();
	for (const auto& entry: entries)
	{
		auto it = archive.findHeader(entry.name);
		assertTrue (it != archive.headerEnd());
		assertTrue (it->second.searchCRCAndSizesAfterData());
		assertTrue (it->second.needsZip64());
		// The entry data is found by the position from the central directory; ZipArchive leaves its
		// stream in the failed state, so every entry is read from a stream of its own.
		Poco::FileInputStream entryIn(zipFile.path());
		ZipInputStream zipIn(entryIn, it->second);
		std::ostringstream out(std::ios::binary);
		Poco::StreamCopier::copyStream(zipIn, out);
		assertEqual (entry.content, out.str());
	}
}


void CompressTest::testSetZipComment()
{
	std::string comment("Testing...123...");
	Poco::FileOutputStream out(Poco::Path::temp() + "comment.zip");
	Poco::Path theFile(ZipTest::getTestFile("data", "test.zip"));
	Compress c(out, true);
	c.addFile(theFile, theFile.getFileName());
	c.setZipComment(comment);
	ZipArchive a(c.close());
	assertTrue (a.getZipComment() == comment);
}


void CompressTest::createDataFile(const std::string& path, Poco::UInt64 size)
{
	Poco::FileOutputStream out(path.c_str(), std::ios::trunc);
	assertTrue ( ! out.fail() );
	Poco::Buffer<char> buffer(MB);
	for(int i = 0; size != 0; i++) {
		std::memset(buffer.begin(), i, buffer.size());
		Poco::UInt64 bytesToWrite = std::min(size, static_cast<Poco::UInt64>(buffer.size()));
		out.write(buffer.begin(), bytesToWrite);
		assertTrue ( ! out.fail() );
		size -= bytesToWrite;
	}
	out.flush();
	assertTrue ( ! out.fail() );
	out.close();
	assertTrue ( ! out.fail() );
}


void CompressTest::testZip64()
{
	using FileMap = std::map<std::string, Poco::UInt64>;

	std::cout << std::endl;
	FileMap files;
	files["data1.bin"] = static_cast<Poco::UInt64>(KB)*4096+1;
	files["data2.bin"] = static_cast<Poco::UInt64>(KB)*16;
	files["data3.bin"] = static_cast<Poco::UInt64>(KB)*4096-1;

	for(FileMap::const_iterator it = files.begin(); it != files.end(); it++)
	{
		std::cout << '\t' << "createDataFile(" << it->first << ", " << it->second << ");" << std::endl;
		createDataFile(it->first, it->second);
	}
	Poco::FileOutputStream out(Poco::Path::temp() + "zip64.zip", std::ios::trunc);
	Compress c(out, true, true);
	for(FileMap::const_iterator it = files.begin(); it != files.end(); it++)
	{
		const std::string& path = it->first;
		std::cout << '\t' << "addFile(" << path <<  ");" << std::endl;
		c.addFile(path, path, ZipCommon::CM_STORE);
	}
	ZipArchive a(c.close());
	for(FileMap::const_iterator it = files.begin(); it != files.end(); it++)
	{
		const std::string& path = it->first;
		Poco::UInt64 size = it->second;
		ZipArchive::FileHeaders::const_iterator it2 = a.findHeader(path);
		assertTrue (it2 != a.headerEnd());
		const Poco::Zip::ZipLocalFileHeader& file = it2->second;
		assertTrue (file.getUncompressedSize() == size);
		assertTrue (file.getCompressedSize() == size);
	}
	for (FileMap::const_iterator it = files.begin(); it != files.end(); it++)
	{
		Poco::File(it->first).remove();
	}
}


void CompressTest::setUp()
{
}


void CompressTest::tearDown()
{
}


CppUnit::Test* CompressTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CompressTest");

	CppUnit_addTest(pSuite, CompressTest, testSingleFile);
	CppUnit_addTest(pSuite, CompressTest, testDirectory);
	CppUnit_addTest(pSuite, CompressTest, testManipulator);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorDel);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorReplace);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorZip64DataDescriptor);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorCopiesEntriesWithExtraField);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorKeepsAttributesOfCopiedEntries);
	CppUnit_addTest(pSuite, CompressTest, testManipulatorCopiesEntriesWithZip64DataDescriptor);
	CppUnit_addTest(pSuite, CompressTest, testSetZipComment);
	CppUnit_addTest(pSuite, CompressTest, testZip64);

	return pSuite;
}
