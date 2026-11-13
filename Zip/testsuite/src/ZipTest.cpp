//
// ZipTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "ZipTest.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ParseCallback.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipException.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipCommon.h"
#include "Poco/Zip/ZipFileInfo.h"
#include "Poco/Zip/ZipArchiveInfo.h"
#include "Poco/Zip/ZipDataInfo.h"
#include "Poco/Zip/ZipManipulator.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/URI.h"
#include "Poco/Path.h"
#include "Poco/Delegate.h"
#include "Poco/StreamCopier.h"
#include "Poco/Environment.h"
#include "Poco/BinaryWriter.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Exception.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


using namespace Poco::Zip;


namespace
{
	// The archives built below hold one entry, a.txt, stored with the content "hello".
	constexpr std::string_view ENTRY_NAME = "a.txt";
	constexpr std::string_view ENTRY_DATA = "hello";
	constexpr Poco::UInt32 ENTRY_CRC = 0x3610A686;
	constexpr Poco::UInt32 ENTRY_SIZE = static_cast<Poco::UInt32>(ENTRY_DATA.size());
	constexpr Poco::UInt16 ENTRY_NAME_LENGTH = static_cast<Poco::UInt16>(ENTRY_NAME.size());
	constexpr Poco::UInt16 ENTRY_TIME = 0;
	constexpr Poco::UInt16 ENTRY_DATE = 0x0021; // 1980-01-01
	constexpr Poco::UInt16 ENTRY_COUNT = 1;
	constexpr Poco::UInt32 CENTRAL_DIRECTORY_SIZE = 46 + ENTRY_NAME_LENGTH; // fixed fields and the name

	// ZipLocalFileHeader reads the upper byte of the version needed to extract as the host system.
	constexpr Poco::UInt16 VERSION_20 = 0x0014;
	constexpr Poco::UInt16 VERSION_45 = 0x002D;

	constexpr Poco::UInt16 DATA_DESCRIPTOR_FLAG = 0x0008; // CRC-32 and sizes follow the data

	constexpr Poco::UInt64 OUT_OF_RANGE = static_cast<Poco::UInt64>(1) << 62; // above the limit for sizes and offsets


	std::string zip64ExtraField(std::initializer_list<Poco::UInt64> values)
		/// Returns a ZIP64 extra field that holds the given values.
	{
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer << ZipCommon::ZIP64_EXTRA_ID << static_cast<Poco::UInt16>(values.size() * ZipCommon::ZIP64_VALUE_SIZE);
		for (const Poco::UInt64 value: values)
		{
			writer << value;
		}
		return ostr.str();
	}


	struct Entry
		/// The fields of a local file header and a central directory entry that the tests change.
	{
		Poco::UInt16 versionNeeded = VERSION_20;
		Poco::UInt16 flags = 0;
		ZipCommon::CompressionMethod method = ZipCommon::CM_STORE;
		std::string_view name = ENTRY_NAME;
		std::string_view data = ENTRY_DATA;          // written after the local file header
		Poco::UInt32 compressedSize = ENTRY_SIZE;
		Poco::UInt32 uncompressedSize = ENTRY_SIZE;
		Poco::UInt32 localHeaderOffset = 0;          // central directory entry only
		std::string extraField{};
		std::string_view comment{};                  // central directory entry only
	};


	std::string localFile(const Entry& entry)
		/// Returns the local file header of the entry, followed by its data.
	{
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer.writeRaw(ZipLocalFileHeader::HEADER, ZipCommon::HEADER_SIZE);
		writer << entry.versionNeeded << entry.flags << static_cast<Poco::UInt16>(entry.method);
		writer << ENTRY_TIME << ENTRY_DATE << ENTRY_CRC << entry.compressedSize << entry.uncompressedSize;
		writer << static_cast<Poco::UInt16>(entry.name.size()) << static_cast<Poco::UInt16>(entry.extraField.size());
		writer.writeRaw(entry.name.data(), entry.name.size());
		writer.writeRaw(entry.extraField.data(), entry.extraField.size());
		writer.writeRaw(entry.data.data(), entry.data.size());
		return ostr.str();
	}


	std::string centralDirectoryEntry(const Entry& entry)
		/// Returns the central directory header of the entry.
	{
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer.writeRaw(ZipFileInfo::HEADER, ZipCommon::HEADER_SIZE);
		writer << entry.versionNeeded << entry.versionNeeded << entry.flags << static_cast<Poco::UInt16>(entry.method);
		writer << ENTRY_TIME << ENTRY_DATE << ENTRY_CRC << entry.compressedSize << entry.uncompressedSize;
		writer << static_cast<Poco::UInt16>(entry.name.size()) << static_cast<Poco::UInt16>(entry.extraField.size());
		writer << static_cast<Poco::UInt16>(entry.comment.size());
		writer.writeRaw(std::string(8, '\0'));                 // disk, attributes
		writer << entry.localHeaderOffset;
		writer.writeRaw(entry.name.data(), entry.name.size());
		writer.writeRaw(entry.extraField.data(), entry.extraField.size());
		writer.writeRaw(entry.comment.data(), entry.comment.size());
		return ostr.str();
	}


	std::string endOfCentralDirectory(Poco::UInt32 directoryOffset, std::string_view comment = {})
		/// Returns the end of central directory record of disk 0 with the given archive comment.
	{
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer.writeRaw(ZipArchiveInfo::HEADER, ZipCommon::HEADER_SIZE);
		writer.writeRaw(std::string(4, '\0'));                 // number of this disk, disk with the directory
		writer << ENTRY_COUNT << ENTRY_COUNT << CENTRAL_DIRECTORY_SIZE << directoryOffset;
		writer << static_cast<Poco::UInt16>(comment.size());
		writer.writeRaw(comment.data(), comment.size());
		return ostr.str();
	}


	std::string zip64EndOfCentralDirectory(Poco::UInt64 recordOffset, Poco::UInt64 directoryOffset)
		/// Returns the ZIP64 end of central directory record of disk 0, which starts at
		/// recordOffset, followed by its locator.
	{
		constexpr Poco::UInt64 RECORD_SIZE = 44; // the record without its signature and this field
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer.writeRaw(ZipArchiveInfo64::HEADER, ZipCommon::HEADER_SIZE);
		writer << RECORD_SIZE << VERSION_45 << VERSION_45;
		writer.writeRaw(std::string(8, '\0'));                 // number of this disk, disk with the directory
		writer << static_cast<Poco::UInt64>(ENTRY_COUNT) << static_cast<Poco::UInt64>(ENTRY_COUNT);
		writer << static_cast<Poco::UInt64>(CENTRAL_DIRECTORY_SIZE) << directoryOffset;
		writer.writeRaw(ZipArchiveInfo64::LOCATOR_HEADER, ZipCommon::HEADER_SIZE);
		writer.writeRaw(std::string(4, '\0'));                 // disk with the ZIP64 record
		writer << recordOffset << static_cast<Poco::UInt32>(1); // total number of disks
		return ostr.str();
	}


	std::string singleEntryArchive(ZipCommon::CompressionMethod method)
		/// Returns a complete archive that holds the entry.
	{
		std::string data = localFile({.method = method});
		const auto directoryOffset = static_cast<Poco::UInt32>(data.size());
		data += centralDirectoryEntry({.method = method});
		data += endOfCentralDirectory(directoryOffset);
		return data;
	}


	class SkipDataCallback: public ParseCallback
		/// Skips the data of the entry and leaves its data descriptor to the header.
	{
	public:
		bool handleZipEntry(std::istream& zipStream, const ZipLocalFileHeader&) override
		{
			zipStream.seekg(ENTRY_SIZE, std::ios_base::cur);
			return true;
		}
	};
}


ZipTest::ZipTest(const std::string& name): CppUnit::TestCase(name)
{
}


ZipTest::~ZipTest()
{
}


void ZipTest::testSkipSingleFile()
{
	std::string testFile = getTestFile("data", "test.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	SkipCallback skip;
	ZipLocalFileHeader hdr(inp, false, skip);
	assertTrue (ZipCommon::HS_FAT == hdr.getHostSystem());
	int major = hdr.getMajorVersionNumber();
	(void) hdr.getMinorVersionNumber();
	assertTrue (major <= 2);
	std::size_t hdrSize = hdr.getHeaderSize();
	assertTrue (hdrSize > 30);
	(void) hdr.getCompressionMethod();
	assertTrue (!hdr.isEncrypted());
	Poco::DateTime aDate = hdr.lastModifiedAt();
	(void) hdr.getCompressedSize();
	(void) hdr.getUncompressedSize();
	(void) hdr.getFileName();
}


void ZipTest::testCrcAndSizeAfterDataEncapsulated()
{
	// touch empty.txt
	// zip -fd foo.zip empty.txt
	// zip -fd encapsulated.zip foo.zip
	std::string testFile = getTestFile("data", "encapsulated.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue(inp.good());

	ZipArchive arch(inp);
	ZipArchive::FileHeaders::const_iterator it = arch.findHeader("foo.zip");
	assertTrue(it != arch.headerEnd());
	inp.clear(); // inp eof(), should clear

	ZipInputStream zipin(inp, it->second);
	std::ostringstream out(std::ios::binary);
	Poco::StreamCopier::copyStream(zipin, out);

	std::string result = out.str();
	// sub zip
	std::istringstream istr(result);
	ZipArchive subArch(istr);
	it = subArch.findHeader("empty.txt");
	assertTrue(it != subArch.headerEnd());
	assertTrue(it->second.getCompressedSize() == 0);
	assertTrue(it->second.getUncompressedSize() == 0);
}


void ZipTest::testDecompressSingleFile()
{
	std::string testFile = getTestFile("data", "test.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	ZipArchive arch(inp);
	ZipArchive::FileHeaders::const_iterator it = arch.findHeader("testfile.txt");
	assertTrue (it != arch.headerEnd());
	ZipInputStream zipin (inp, it->second);
	std::ostringstream out(std::ios::binary);
	Poco::StreamCopier::copyStream(zipin, out);
	assertTrue (!out.str().empty());
}


void ZipTest::testDecompressSingleFileInDir()
{
	std::string testFile = getTestFile("data","test.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	ZipArchive arch(inp);
	ZipArchive::FileHeaders::const_iterator it = arch.findHeader("testdir/testfile.txt");
	assertTrue (it != arch.headerEnd());
	ZipInputStream zipin (inp, it->second);
	std::ostringstream out(std::ios::binary);
	Poco::StreamCopier::copyStream(zipin, out);
	assertTrue (!out.str().empty());
}


void ZipTest::testCrcAndSizeAfterData()
{
	std::string testFile = getTestFile("data", "data.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Decompress dec(inp, Poco::Path::temp());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertTrue (_errCnt == 0);
	assertTrue (!dec.mapping().empty());
}


void ZipTest::testCrcAndSizeAfterDataWithArchive()
{
	std::string testFile = getTestFile("data", "data.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Poco::Zip::ZipArchive zip(inp);
	inp.clear();
	inp.seekg(0);
	Poco::Zip::ZipArchive::FileHeaders::const_iterator it = zip.headerBegin();
	for ( ; it!=zip.headerEnd(); ++it)
	{
		Poco::Zip::ZipInputStream zipis(inp,it->second);
		Poco::Path path(it->second.getFileName());
		if (path.isFile())
		{
			Poco::FileOutputStream os(Poco::Path::temp() + "test.dat");
			Poco::StreamCopier::copyStream(zipis,os);
		}
	}
}


std::string ZipTest::getTestFile(const std::string& directory, const std::string& file)
{
	std::ostringstream ostr;
	ostr << directory << '/' << file;
	std::string validDir(ostr.str());
	Poco::Path pathPattern(validDir);
	if (Poco::File(pathPattern).exists())
	{
		return validDir;
	}

	ostr.str("");
	ostr << "/Zip/testsuite/" << directory << '/' << file;
	validDir = Poco::Environment::get("POCO_BASE") + ostr.str();
	pathPattern = validDir;

	if (!Poco::File(pathPattern).exists())
	{
		std::cout << "Can't find " << validDir << std::endl;
		throw Poco::NotFoundException("cannot locate directory containing valid Zip test files");
	}
	return validDir;
}


void ZipTest::testDecompress()
{
	std::string testFile = getTestFile("data", "test.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Decompress dec(inp, Poco::Path::temp());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertTrue (_errCnt == 0);
	assertTrue (!dec.mapping().empty());
}


void ZipTest::testDecompressFlat()
{
	std::string testFile = getTestFile("data", "test.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Decompress dec(inp, Poco::Path::temp(), true);
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertTrue (_errCnt == 0);
	assertTrue (!dec.mapping().empty());
}


void ZipTest::testDecompressVuln()
{
	std::string testFile = getTestFile("data", "vuln.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Decompress dec(inp, Poco::Path::temp());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertTrue (_errCnt == 1);
	assertTrue (dec.mapping().empty());
}


void ZipTest::testDecompressFlatVuln()
{
	std::string testFile = getTestFile("data", "vuln.zip");
	Poco::FileInputStream inp(testFile);
	assertTrue (inp.good());
	Decompress dec(inp, Poco::Path::temp(), true);
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	dec.decompressAllFiles();
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertTrue (_errCnt == 0);
	assertTrue (!dec.mapping().empty());
}


void ZipTest::verifyDataFile(const std::string& path, Poco::UInt64 size)
{
	Poco::FileInputStream in(path);
	assertTrue ( ! in.fail() );
	Poco::Buffer<char> buffer1(MB);
	Poco::Buffer<char> buffer2(MB);
	for (int i = 0; size != 0; i++)
	{
		std::memset(buffer1.begin(), i, buffer1.size());
		std::memset(buffer2.begin(), 0, buffer2.size());
		Poco::UInt64 bytesToRead = std::min(size, static_cast<Poco::UInt64>(buffer2.size()));
		in.read(buffer2.begin(), bytesToRead);
		assertTrue (!in.fail() );
		assertTrue (std::memcmp(buffer1.begin(), buffer2.begin(), static_cast<std::size_t>(bytesToRead)) == 0);
		size -= bytesToRead;
	}
	char c;
	in.read(&c, 1);
	assertTrue ( in.eof() );
}


void ZipTest::testDecompressZip64()
{
	std::map<std::string, Poco::UInt64> files;
	files[Poco::Path::temp() + "data1.bin"] = static_cast<Poco::UInt64>(KB)*4096+1;
	files[Poco::Path::temp() + "data2.bin"] = static_cast<Poco::UInt64>(KB)*16;
	files[Poco::Path::temp() + "data3.bin"] = static_cast<Poco::UInt64>(KB)*4096-1;

	for(std::map<std::string, Poco::UInt64>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		Poco::File file(it->first);
		if(file.exists())
			file.remove();
	}
	Poco::FileInputStream in(Poco::Path::temp() + "zip64.zip");
	Decompress c(in, Poco::Path::temp());
	c.decompressAllFiles();
	for(std::map<std::string, Poco::UInt64>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		verifyDataFile(it->first, it->second);
	}
}


void ZipTest::testDecompressConsistency()
{
	// test archives with borked file headers, but correct directory
	const std::string testArchives[] =
	{
		"consistency-crc32.zip",
		"consistency-uncompressed.zip"
	};


	for (const std::string &archive : testArchives)
	{
		//
		// test decompressing all files
		//
		{
			Poco::FileInputStream inputStream(getTestFile("data", archive));
			assertTrue (inputStream.good());

			Decompress dec(inputStream, Poco::Path::temp());

			try {
				dec.decompressAllFiles();
				assertTrue (false);
			}
			catch (const ZipException &e)
			{
			}
		}

		
		//
		// test decompressing all files (ignore consistency check)
		//
		{
			Poco::FileInputStream inputStream(getTestFile("data", archive));
			assertTrue (inputStream.good());

			Decompress dec(inputStream, Poco::Path::temp());

			// should not throw since consistency checks are ignored
			try {
				dec.decompressAllFiles(false);
			}
			catch (const ZipException &e)
			{
				assertTrue (false);
			}
		}


		//
		// test decompressing single file
		//
		{
			Poco::FileInputStream inputStream(getTestFile("data", archive));
			assertTrue (inputStream.good());

			ZipArchive archive(inputStream);

			// File decompression code is skipped since
			// we are not expecting archive to be processed further
			try
			{
				archive.checkConsistency();
				assertTrue (false);
			}
			catch (const ZipException &e)
			{
			}
		}
	};
}

void ZipTest::testValidPath()
{
	assertTrue (ZipCommon::isValidPath("."));
	assertTrue (ZipCommon::isValidPath("file.txt"));
	assertTrue (ZipCommon::isValidPath(".file.txt"));
	assertTrue (ZipCommon::isValidPath("..file.txt"));
	assertTrue (ZipCommon::isValidPath("file.txt.."));
	assertTrue (ZipCommon::isValidPath(".file..txt"));
	assertTrue (ZipCommon::isValidPath("~file..txt"));
	assertTrue (ZipCommon::isValidPath("~file/~"));
	assertTrue (ZipCommon::isValidPath("dir/~"));
	assertTrue (ZipCommon::isValidPath("some"));
	assertTrue (ZipCommon::isValidPath("some/dir"));
	assertTrue (ZipCommon::isValidPath("some/dir/or/another"));
	assertTrue (ZipCommon::isValidPath("some/dir/./another"));
	assertTrue (ZipCommon::isValidPath("some/dir/or/another/file.txt"));
	assertTrue (ZipCommon::isValidPath("s~me\\d.r\\.or..\\an..her\\file.txt"));
	assertTrue (ZipCommon::isValidPath("some\\dir\\or\\another"));
	assertTrue (ZipCommon::isValidPath("some\\dir\\or\\another\\file.txt"));
	assertTrue (ZipCommon::isValidPath("s~me\\d.r/.or..\\an..her\\file.txt"));

	assertTrue (!ZipCommon::isValidPath("/../"));
	assertTrue (!ZipCommon::isValidPath("/"));
	assertTrue (!ZipCommon::isValidPath("\\..\\"));
	assertTrue (!ZipCommon::isValidPath("/..\\"));
	assertTrue (!ZipCommon::isValidPath("\\../"));
	assertTrue (!ZipCommon::isValidPath(".."));
	assertTrue (!ZipCommon::isValidPath("~/"));
	assertTrue (!ZipCommon::isValidPath("~/~"));
	assertTrue (!ZipCommon::isValidPath("/~"));
	assertTrue (!ZipCommon::isValidPath("/file.txt"));
	assertTrue (!ZipCommon::isValidPath("~/file.txt"));
	assertTrue (!ZipCommon::isValidPath("some/dir/or/../another/file.txt"));
	assertTrue (!ZipCommon::isValidPath("C:\\Windows\\system32"));
}


void ZipTest::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
{
	++_errCnt;
}


void ZipTest::setUp()
{
	_errCnt = 0;
}


void ZipTest::tearDown()
{
}


void ZipTest::testMalformedZip64()
{
	// A ZIP64 end of central directory record whose declared size is far larger
	// than the data present must not drive an allocation from that number, and a
	// truncated ZIP64 extra field must not be read past the bytes available.
	{
		std::string data;
		data.append("PK\x06\x06", 4);                        // ZIP64 end of central directory
		const char hugeSize[8] = { '\xff', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff', '\x7f' };
		data.append(hugeSize, 8);                             // record size: ~2^63
		data.append(48, '\0');                                // the fixed part and a little payload, then EOF
		std::istringstream istr(data);
		try
		{
			ZipArchive arch(istr);
			fail("malformed ZIP64 record must be rejected");
		}
		catch (Poco::Exception&)
		{
		}
	}

	// A central directory header whose ZIP64 extra field declares more bytes
	// than the field actually holds must not be read past that field.
	{
		std::string data;
		data.append("PK\x01\x02", 4);                          // central directory header
		data.append(2, '\0');                                  // version made by
		data.append("\x2d\x00", 2);                            // version needed 4.5
		data.append(2, '\0');                                  // general purpose flags
		data.append(2, '\0');                                  // compression method
		data.append(4, '\0');                                  // time and date
		data.append(4, '\0');                                  // crc32
		const char magic[4] = { '\xff', '\xff', '\xff', '\xff' };
		data.append(magic, 4);                                 // compressed size
		data.append(magic, 4);                                 // uncompressed size
		data.append("\x01\x00", 2);                            // file name length
		data.append("\x08\x00", 2);                            // extra field length: 8
		data.append(2, '\0');                                  // file comment length
		data.append(2, '\0');                                  // disk number start
		data.append(2, '\0');                                  // internal attributes
		data.append(4, '\0');                                  // external attributes
		data.append(magic, 4);                                 // local header offset
		data.append("a", 1);                                   // file name
		data.append("\x01\x00", 2);                            // extra field id: ZIP64
		data.append("\x18\x00", 2);                            // declared size 24, 4 present
		data.append(4, '\0');
		std::istringstream istr(data);
		try
		{
			ZipArchive arch(istr);
		}
		catch (Poco::Exception&)
		{
			// Rejecting the archive is fine, reading out of bounds is not. Under
			// AddressSanitizer this is what catches a regression.
		}
	}

	// The same mismatch in a local file header. The compressed size is left at
	// zero so the entry is skipped normally and the extra field is the only
	// thing under test.
	{
		std::string data;
		data.append("PK\x03\x04", 4);
		data.append("\x2d\x00", 2);                            // version needed 4.5
		data.append(2, '\0');                                  // general purpose flags
		data.append(2, '\0');                                  // compression method: store
		data.append(4, '\0');                                  // time and date
		data.append(4, '\0');                                  // crc32
		data.append(4, '\0');                                  // compressed size: 0
		const char magic[4] = { '\xff', '\xff', '\xff', '\xff' };
		data.append(magic, 4);                                 // uncompressed size: magic
		data.append("\x01\x00", 2);                            // file name length
		data.append("\x08\x00", 2);                            // extra field length: 8
		data.append("a", 1);                                   // file name
		data.append("\x01\x00", 2);                            // extra field id: ZIP64
		data.append("\x10\x00", 2);                            // declared size 16, 4 present
		data.append(4, '\0');
		std::istringstream istr(data);
		try
		{
			ZipArchive arch(istr);
			// The declared ZIP64 size exceeds the field, so no value may have
			// been taken from beyond it: the header value stays untouched.
			auto it = arch.headerBegin();
			if (it != arch.headerEnd())
				assertTrue (it->second.getUncompressedSize() == ZipCommon::ZIP64_MAGIC);
		}
		catch (Poco::Exception&)
		{
		}
	}

}


void ZipTest::testUnknownMethodAndHostSystem()
{
	// A compression method or host system beyond the enumerators of ZipCommon is
	// read as it is, and the entry is rejected as unsupported.
	std::string data;
	data.append("PK\x03\x04", 4);                          // local file header
	data.append("\x14\x00", 2);                            // version needed 2.0, host FAT
	data.append(2, '\0');                                  // general purpose flags
	data.append("\x6f\x6d", 2);                            // compression method 0x6d6f
	data.append(4, '\0');                                  // time and date
	data.append(4, '\0');                                  // crc32
	data.append(4, '\0');                                  // compressed size
	data.append(4, '\0');                                  // uncompressed size
	data.append("\x01\x00", 2);                            // file name length
	data.append(2, '\0');                                  // extra field length
	data.append("a", 1);                                   // file name
	data.append("PK\x01\x02", 4);                          // central directory header
	data.append("\x14\x00", 2);                            // version made by
	data.append("\x14\x7f", 2);                            // version needed 2.0, host system 0x7f
	data.append(2, '\0');                                  // general purpose flags
	data.append("\x6f\x6d", 2);                            // compression method 0x6d6f
	data.append(4, '\0');                                  // time and date
	data.append(4, '\0');                                  // crc32
	data.append(4, '\0');                                  // compressed size
	data.append(4, '\0');                                  // uncompressed size
	data.append("\x01\x00", 2);                            // file name length
	data.append(2, '\0');                                  // extra field length
	data.append(2, '\0');                                  // file comment length
	data.append(2, '\0');                                  // disk number start
	data.append(2, '\0');                                  // internal attributes
	data.append(4, '\0');                                  // external attributes
	data.append(4, '\0');                                  // local header offset
	data.append("a", 1);                                   // file name
	std::istringstream istr(data);
	ZipArchive arch(istr);

	ZipArchive::FileHeaders::const_iterator it = arch.findHeader("a");
	assertTrue (it != arch.headerEnd());
	const ZipCommon::CompressionMethod method = it->second.getCompressionMethod();
	assertEqual (0x6d6f, static_cast<int>(method));
	assertTrue (!it->second.hasSupportedCompressionMethod());

	ZipArchive::FileInfos::const_iterator info = arch.findFileInfo("a");
	assertTrue (info != arch.fileInfoEnd());
	const ZipCommon::CompressionMethod infoMethod = info->second.getCompressionMethod();
	const ZipCommon::HostSystem hostSystem = info->second.getHostSystem();
	assertEqual (0x6d6f, static_cast<int>(infoMethod));
	assertEqual (0x7f, static_cast<int>(hostSystem));

	istr.clear();
	try
	{
		ZipInputStream zipin(istr, it->second);
		fail("an unsupported compression method must be rejected");
	}
	catch (ZipException&)
	{
	}
}


void ZipTest::testUnsupportedVersionNeeded()
{
	const Poco::UInt16 unsupportedVersions[] =
	{
		0x1214, // host system 18, version 2.0
		0x0033, // version 5.1
		0x00FF  // version 25.5
	};
	for (const Poco::UInt16 versionNeeded: unsupportedVersions)
	{
		std::istringstream istr(localFile({.versionNeeded = versionNeeded}));
		try
		{
			ZipArchive arch(istr);
			fail("unsupported version needed to extract must be rejected");
		}
		catch (const ZipException&)
		{
		}
	}

	std::istringstream istr(localFile({.versionNeeded = VERSION_45}));
	ZipArchive arch(istr);
	assertEqual (1, std::distance(arch.headerBegin(), arch.headerEnd()));
}


void ZipTest::testDuplicateEndOfCentralDirectory()
{
	{
		std::string data = localFile({});
		const auto directoryOffset = static_cast<Poco::UInt32>(data.size());
		data += centralDirectoryEntry({});
		data += endOfCentralDirectory(directoryOffset);
		data += endOfCentralDirectory(directoryOffset);
		std::istringstream istr(data);
		try
		{
			ZipArchive arch(istr);
			fail("a second end of central directory record for disk 0 must be rejected");
		}
		catch (const ZipException&)
		{
		}
	}

	{
		std::string data = localFile({});
		const auto directoryOffset = static_cast<Poco::UInt32>(data.size());
		data += centralDirectoryEntry({});
		const Poco::UInt64 firstRecordOffset = data.size();
		data += zip64EndOfCentralDirectory(firstRecordOffset, directoryOffset);
		const Poco::UInt64 secondRecordOffset = data.size();
		data += zip64EndOfCentralDirectory(secondRecordOffset, directoryOffset);
		data += endOfCentralDirectory(directoryOffset);
		std::istringstream istr(data);
		try
		{
			ZipArchive arch(istr);
			fail("a second ZIP64 end of central directory record for disk 0 must be rejected");
		}
		catch (const ZipException&)
		{
		}
	}
}


void ZipTest::testIllegalHeader()
{
	// ZipUtil::sync() stops at a data descriptor signature, but no record starts with it.
	std::string garbage("XXXX");
	garbage.append(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);

	{
		std::istringstream istr(garbage);
		try
		{
			ZipArchive arch(istr);
			fail("an archive without a known record must be rejected");
		}
		catch (const ZipException& exc)
		{
			assertMessageEqual ("Illegal header in zip file", exc.message());
		}
	}

	{
		std::istringstream istr(singleEntryArchive(ZipCommon::CM_STORE) + garbage);
		try
		{
			ZipArchive arch(istr);
			fail("garbage after the end of central directory record must be rejected");
		}
		catch (const ZipException& exc)
		{
			assertMessageEqual ("Garbage after directory header", exc.message());
		}
	}
}


void ZipTest::testReplaceUnsupportedMethod()
{
	Poco::TemporaryFile zipFile;
	Poco::TemporaryFile replacementFile;
	{
		Poco::FileOutputStream out(zipFile.path());
		out << singleEntryArchive(ZipCommon::CM_BZIP2);
	}
	{
		Poco::FileOutputStream out(replacementFile.path());
		out << "replacement";
	}
	// commit() writes the new archive to this file and leaves it behind when it fails.
	Poco::TemporaryFile::registerForDeletion(zipFile.path() + ".tmp");

	ZipManipulator zm(zipFile.path(), false);
	zm.replaceFile(std::string(ENTRY_NAME), replacementFile.path());
	try
	{
		zm.commit();
		fail("an entry must not be written with an unsupported compression method");
	}
	catch (const ZipException&)
	{
	}
}


void ZipTest::testDecompressFailedEntryWithDataDescriptor()
{
	// Decompress cannot extract the entry and declines it; the central directory after
	// the entry data and its data descriptor must still be found.
	std::ostringstream ostr;
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(localFile({.flags = DATA_DESCRIPTOR_FLAG, .method = ZipCommon::CM_BZIP2}));
	writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	writer << ENTRY_CRC << ENTRY_SIZE << ENTRY_SIZE;
	const auto directoryOffset = static_cast<Poco::UInt32>(ostr.str().size());
	writer.writeRaw(centralDirectoryEntry({.flags = DATA_DESCRIPTOR_FLAG, .method = ZipCommon::CM_BZIP2}));
	writer.writeRaw(endOfCentralDirectory(directoryOffset));
	std::istringstream istr(ostr.str());

	Poco::TemporaryFile outputDir;
	Decompress dec(istr, outputDir.path());
	dec.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	// The CRC-32 of a declined entry with a data descriptor is not read, so the
	// consistency check with the central directory would fail.
	const ZipArchive arch = dec.decompressAllFiles(false);
	dec.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>>(this, &ZipTest::onDecompressError);
	assertEqual (1, _errCnt);
	assertTrue (arch.findFileInfo(std::string(ENTRY_NAME)) != arch.fileInfoEnd());
	assertTrue (dec.mapping().empty());
	std::vector<std::string> files;
	outputDir.list(files);
	assertTrue (files.empty());
}


void ZipTest::testSizeOrOffsetOutOfRange()
{
	const std::string outOfRangeField = zip64ExtraField({OUT_OF_RANGE});
	const std::string compressedSizeArchive = localFile({.versionNeeded = VERSION_45, .compressedSize = ZipCommon::ZIP64_MAGIC, .extraField = outOfRangeField});

	// A compressed size that moves the position after the entry back to the start of its local file header.
	const std::size_t headerSize = compressedSizeArchive.size() - ENTRY_DATA.size();
	const Poco::UInt64 wrappingSize = std::numeric_limits<Poco::UInt64>::max() - headerSize + 1;

	const auto directoryEntryArchive = [](const Entry& directoryEntry)
	{
		std::string data = localFile({});
		const auto directoryOffset = static_cast<Poco::UInt32>(data.size());
		data += centralDirectoryEntry(directoryEntry);
		data += endOfCentralDirectory(directoryOffset);
		return data;
	};

	std::ostringstream descriptorArchive;
	Poco::BinaryWriter writer(descriptorArchive, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(localFile({.versionNeeded = VERSION_45, .flags = DATA_DESCRIPTOR_FLAG, .compressedSize = 0, .uncompressedSize = 0, .extraField = zip64ExtraField({0, 0})}));
	writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	writer << ENTRY_CRC << static_cast<Poco::UInt64>(ENTRY_SIZE) << OUT_OF_RANGE;

	const std::string archives[] =
	{
		compressedSizeArchive,
		localFile({.versionNeeded = VERSION_45, .compressedSize = ZipCommon::ZIP64_MAGIC, .extraField = zip64ExtraField({wrappingSize})}),
		localFile({.versionNeeded = VERSION_45, .uncompressedSize = ZipCommon::ZIP64_MAGIC, .extraField = outOfRangeField}),
		directoryEntryArchive({.compressedSize = ZipCommon::ZIP64_MAGIC, .extraField = outOfRangeField}),
		directoryEntryArchive({.uncompressedSize = ZipCommon::ZIP64_MAGIC, .extraField = outOfRangeField}),
		directoryEntryArchive({.localHeaderOffset = ZipCommon::ZIP64_MAGIC, .extraField = outOfRangeField}),
		descriptorArchive.str()
	};
	for (const std::string& archive: archives)
	{
		std::istringstream istr(archive);
		try
		{
			ZipArchive arch(istr);
			fail("a size or an offset out of range must be rejected");
		}
		catch (const ZipException&)
		{
		}
	}

	std::string data = localFile({.versionNeeded = VERSION_45, .compressedSize = ZipCommon::ZIP64_MAGIC, .uncompressedSize = ZipCommon::ZIP64_MAGIC, .extraField = zip64ExtraField({ENTRY_SIZE, ENTRY_SIZE})});
	const auto zip64DirectoryOffset = static_cast<Poco::UInt32>(data.size());
	data += centralDirectoryEntry({.compressedSize = ZipCommon::ZIP64_MAGIC, .uncompressedSize = ZipCommon::ZIP64_MAGIC, .localHeaderOffset = ZipCommon::ZIP64_MAGIC, .extraField = zip64ExtraField({ENTRY_SIZE, ENTRY_SIZE, 0})});
	data += endOfCentralDirectory(zip64DirectoryOffset);
	std::istringstream istr(data);
	ZipArchive arch(istr);
	const auto itHeader = arch.findHeader(std::string(ENTRY_NAME));
	assertTrue (itHeader != arch.headerEnd());
	assertEqual (ENTRY_SIZE, itHeader->second.getCompressedSize());
	assertEqual (ENTRY_SIZE, itHeader->second.getUncompressedSize());
	const auto itInfo = arch.findFileInfo(std::string(ENTRY_NAME));
	assertTrue (itInfo != arch.fileInfoEnd());
	assertEqual (ENTRY_SIZE, itInfo->second.getCompressedSize());
	assertEqual (ENTRY_SIZE, itInfo->second.getUncompressedSize());
	assertEqual (0, itInfo->second.getOffset());
}


void ZipTest::testKeepEntryBeyondEndOfArchive()
{
	Poco::TemporaryFile zipFile;
	{
		Poco::FileOutputStream out(zipFile.path());
		out << localFile({.compressedSize = ENTRY_SIZE + 1});
	}
	// commit() writes the new archive to this file and leaves it behind when it fails.
	Poco::TemporaryFile::registerForDeletion(zipFile.path() + ".tmp");

	ZipManipulator zm(zipFile.path(), false);
	try
	{
		zm.commit();
		fail("an entry whose data ends after the end of the archive must not be copied");
	}
	catch (const Poco::IOException&)
	{
	}
}


void ZipTest::testDataDescriptorEntryAbove4GiB()
{
	constexpr Poco::UInt64 OFFSET_ABOVE_4GIB = static_cast<Poco::UInt64>(1) << 32; // the local file header is at 0
	constexpr std::string_view DEFLATED_DATA("\xcb\x48\xcd\xc9\xc9\x07\x00", 7); // raw deflate stream of ENTRY_DATA
	const Entry entries[] =
	{
		{.flags = DATA_DESCRIPTOR_FLAG},
		{.flags = DATA_DESCRIPTOR_FLAG, .method = ZipCommon::CM_DEFLATE, .data = DEFLATED_DATA, .compressedSize = static_cast<Poco::UInt32>(DEFLATED_DATA.size())}
	};
	for (const Entry& entry: entries)
	{
		std::ostringstream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer.writeRaw(localFile(entry));
		// The entry needs ZIP64 at its offset, so ZipInputStream expects a ZIP64 data descriptor.
		writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
		writer << ENTRY_CRC << static_cast<Poco::UInt64>(entry.compressedSize) << static_cast<Poco::UInt64>(entry.uncompressedSize);
		const auto directoryOffset = static_cast<Poco::UInt32>(ostr.str().size());
		Entry directoryEntry = entry;
		directoryEntry.localHeaderOffset = ZipCommon::ZIP64_MAGIC;
		directoryEntry.extraField = zip64ExtraField({OFFSET_ABOVE_4GIB});
		writer.writeRaw(centralDirectoryEntry(directoryEntry));
		writer.writeRaw(endOfCentralDirectory(directoryOffset));
		std::istringstream istr(ostr.str());

		ZipArchive arch(istr);
		const auto it = arch.findHeader(std::string(ENTRY_NAME));
		assertTrue (it != arch.headerEnd());
		istr.clear();
		ZipInputStream zipin(istr, it->second, true);
		std::string data;
		Poco::StreamCopier::copyToString(zipin, data);
		// From the start position cut to 32 bits, the stream would return the data of the entry at offset 0;
		// the entry itself is at 2^32, past the end of the stream.
		assertTrue (data != ENTRY_DATA);
	}
}


void ZipTest::testEmptyFileName()
{
	const Entry entry{.name = "", .uncompressedSize = 0};
	std::string data = localFile(entry);
	const auto directoryOffset = static_cast<Poco::UInt32>(data.size());
	data += centralDirectoryEntry(entry);
	data += endOfCentralDirectory(directoryOffset);
	std::istringstream istr(data);

	ZipArchive arch(istr);
	const auto itHeader = arch.findHeader("");
	assertTrue (itHeader != arch.headerEnd());
	assertTrue (!itHeader->second.isDirectory());
	assertTrue (itHeader->second.isFile());
	const auto itInfo = arch.findFileInfo("");
	assertTrue (itInfo != arch.fileInfoEnd());
	assertTrue (!itInfo->second.isDirectory());
	assertTrue (itInfo->second.isFile());
}


void ZipTest::testDuplicateFileNames()
{
	const std::string names[] = { "a.txt", "a.txt.duplicate-2", "a.txt", "a.txt", "a.txt" };
	std::string data;
	for (const std::string& name: names)
	{
		data += localFile({.name = name});
	}
	std::istringstream istr(data);

	ZipArchive arch(istr);
	std::vector<std::string> headerNames;
	for (auto it = arch.headerBegin(); it != arch.headerEnd(); ++it)
	{
		headerNames.push_back(it->first);
	}
	const std::vector<std::string> expected = { "a.txt", "a.txt.duplicate-1", "a.txt.duplicate-2", "a.txt.duplicate-3", "a.txt.duplicate-4" };
	assertTrue (headerNames == expected);
}


void ZipTest::verifyTruncatedArchive(const std::vector<std::string>& records, std::string_view fileComment, std::string_view archiveComment)
{
	std::string data;
	std::set<std::size_t> acceptedLengths;
	for (const std::string& record: records)
	{
		// The parser takes a record signature that ends early for the end of the archive.
		for (std::size_t i = 0; i < ZipCommon::HEADER_SIZE; ++i)
		{
			acceptedLengths.insert(data.size() + i);
		}
		data += record;
	}
	acceptedLengths.insert(data.size());

	for (std::size_t length = 0; length <= data.size(); ++length)
	{
		std::istringstream istr(data.substr(0, length));
		bool accepted = true;
		try
		{
			ZipArchive arch(istr);
			for (auto it = arch.headerBegin(); it != arch.headerEnd(); ++it)
			{
				assertTrue (it->second.getFileName() == ENTRY_NAME);
			}
			for (auto it = arch.fileInfoBegin(); it != arch.fileInfoEnd(); ++it)
			{
				assertTrue (it->second.getFileName() == ENTRY_NAME);
				assertTrue (it->second.getFileComment() == fileComment);
			}
			// The end of central directory record is the last record of each archive.
			assertTrue (arch.getZipComment() == (length == data.size() ? archiveComment : std::string_view()));
		}
		catch (const Poco::IOException&)
		{
			accepted = false;
		}
		if (accepted != acceptedLengths.contains(length))
		{
			fail("archive of " + std::to_string(data.size()) + " bytes cut after " + std::to_string(length) + " bytes: " + (accepted ? "accepted" : "rejected"));
		}
	}
}


void ZipTest::testTruncatedArchive()
{
	constexpr std::string_view FILE_COMMENT = "file comment";
	constexpr std::string_view ARCHIVE_COMMENT = "archive comment";
	const std::string extraField("UT\0\0", 4); // an extended timestamp field without data
	const std::string entry = localFile({.extraField = extraField});
	verifyTruncatedArchive(
		{
			entry,
			centralDirectoryEntry({.extraField = extraField, .comment = FILE_COMMENT}),
			endOfCentralDirectory(static_cast<Poco::UInt32>(entry.size()), ARCHIVE_COMMENT)
		},
		FILE_COMMENT,
		ARCHIVE_COMMENT);

	const std::string plainEntry = localFile({});
	const std::string plainDirectory = centralDirectoryEntry({});
	const auto plainDirectoryOffset = static_cast<Poco::UInt32>(plainEntry.size());
	verifyTruncatedArchive(
		{
			plainEntry,
			plainDirectory,
			// The parser reads a ZIP64 end of central directory record together with its locator.
			zip64EndOfCentralDirectory(plainEntry.size() + plainDirectory.size(), plainDirectoryOffset),
			endOfCentralDirectory(plainDirectoryOffset)
		});

	std::ostringstream descriptorEntry;
	Poco::BinaryWriter writer(descriptorEntry, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(localFile({.flags = DATA_DESCRIPTOR_FLAG}));
	writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	writer << ENTRY_CRC << ENTRY_SIZE << ENTRY_SIZE;
	verifyTruncatedArchive(
		{
			descriptorEntry.str(),
			centralDirectoryEntry({.flags = DATA_DESCRIPTOR_FLAG}),
			endOfCentralDirectory(static_cast<Poco::UInt32>(descriptorEntry.str().size()))
		});
}


void ZipTest::testTruncatedDataDescriptor()
{
	// The callback leaves the data descriptor to ZipLocalFileHeader.
	SkipDataCallback callback;

	std::ostringstream entry;
	Poco::BinaryWriter writer(entry, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(localFile({.flags = DATA_DESCRIPTOR_FLAG}));
	const std::size_t descriptorOffset = entry.str().size();
	writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	writer << ENTRY_CRC << ENTRY_SIZE << ENTRY_SIZE;

	std::ostringstream zip64Entry;
	Poco::BinaryWriter zip64Writer(zip64Entry, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	zip64Writer.writeRaw(localFile({.versionNeeded = VERSION_45, .flags = DATA_DESCRIPTOR_FLAG, .compressedSize = 0, .uncompressedSize = 0, .extraField = zip64ExtraField({0, 0})}));
	const std::size_t zip64DescriptorOffset = zip64Entry.str().size();
	zip64Writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	zip64Writer << ENTRY_CRC << static_cast<Poco::UInt64>(ENTRY_SIZE) << static_cast<Poco::UInt64>(ENTRY_SIZE);

	const std::pair<std::string, std::size_t> entries[] =
	{
		{entry.str(), descriptorOffset},
		{zip64Entry.str(), zip64DescriptorOffset}
	};
	for (const auto& [data, offset]: entries)
	{
		for (std::size_t length = offset; length < data.size(); ++length)
		{
			std::istringstream istr(data.substr(0, length));
			try
			{
				ZipLocalFileHeader header(istr, false, callback);
				fail("a truncated data descriptor must be rejected");
			}
			catch (const Poco::IOException&)
			{
			}
		}

		std::istringstream istr(data);
		ZipLocalFileHeader header(istr, false, callback);
		assertEqual (ENTRY_CRC, header.getCRC());
		assertEqual (ENTRY_SIZE, header.getCompressedSize());
		assertEqual (ENTRY_SIZE, header.getUncompressedSize());
	}
}


void ZipTest::testDataDescriptorCompressedSizeOutOfRange()
{
	SkipDataCallback callback;

	std::ostringstream ostr;
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(localFile({.versionNeeded = VERSION_45, .flags = DATA_DESCRIPTOR_FLAG, .compressedSize = 0, .uncompressedSize = 0, .extraField = zip64ExtraField({0, 0})}));
	writer.writeRaw(ZipDataInfo::HEADER, ZipCommon::HEADER_SIZE);
	writer << ENTRY_CRC << OUT_OF_RANGE << static_cast<Poco::UInt64>(ENTRY_SIZE);
	std::istringstream istr(ostr.str());
	try
	{
		// Read through ZipArchive, the entry is rejected earlier, when SkipCallback fails to seek past its data.
		ZipLocalFileHeader header(istr, false, callback);
		fail("a compressed size out of range in a data descriptor must be rejected");
	}
	catch (const ZipException&)
	{
	}
}


void ZipTest::testSyncAtEndOfArchive()
{
	// ZipUtil::sync() finds the first two bytes of a signature and the archive ends after one more.
	std::istringstream istr("XXXXPK\x03");
	ZipArchive arch(istr);
	assertTrue (arch.headerBegin() == arch.headerEnd());
}


CppUnit::Test* ZipTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ZipTest");

	CppUnit_addTest(pSuite, ZipTest, testSkipSingleFile);
	CppUnit_addTest(pSuite, ZipTest, testDecompressSingleFile);
	CppUnit_addTest(pSuite, ZipTest, testDecompressSingleFileInDir);
	CppUnit_addTest(pSuite, ZipTest, testDecompress);
	CppUnit_addTest(pSuite, ZipTest, testDecompressFlat);
	CppUnit_addTest(pSuite, ZipTest, testDecompressVuln);
	CppUnit_addTest(pSuite, ZipTest, testDecompressFlatVuln);
	CppUnit_addTest(pSuite, ZipTest, testCrcAndSizeAfterData);
	CppUnit_addTest(pSuite, ZipTest, testCrcAndSizeAfterDataWithArchive);
	CppUnit_addTest(pSuite, ZipTest, testCrcAndSizeAfterDataEncapsulated);
	CppUnit_addTest(pSuite, ZipTest, testDecompressZip64);
	CppUnit_addTest(pSuite, ZipTest, testValidPath);
	CppUnit_addTest(pSuite, ZipTest, testMalformedZip64);
	CppUnit_addTest(pSuite, ZipTest, testUnknownMethodAndHostSystem);
	CppUnit_addTest(pSuite, ZipTest, testDecompressConsistency);
	CppUnit_addTest(pSuite, ZipTest, testUnsupportedVersionNeeded);
	CppUnit_addTest(pSuite, ZipTest, testDuplicateEndOfCentralDirectory);
	CppUnit_addTest(pSuite, ZipTest, testIllegalHeader);
	CppUnit_addTest(pSuite, ZipTest, testReplaceUnsupportedMethod);
	CppUnit_addTest(pSuite, ZipTest, testDecompressFailedEntryWithDataDescriptor);
	CppUnit_addTest(pSuite, ZipTest, testSizeOrOffsetOutOfRange);
	CppUnit_addTest(pSuite, ZipTest, testKeepEntryBeyondEndOfArchive);
	CppUnit_addTest(pSuite, ZipTest, testDataDescriptorEntryAbove4GiB);
	CppUnit_addTest(pSuite, ZipTest, testEmptyFileName);
	CppUnit_addTest(pSuite, ZipTest, testDuplicateFileNames);
	CppUnit_addTest(pSuite, ZipTest, testTruncatedArchive);
	CppUnit_addTest(pSuite, ZipTest, testTruncatedDataDescriptor);
	CppUnit_addTest(pSuite, ZipTest, testDataDescriptorCompressedSizeOutOfRange);
	CppUnit_addTest(pSuite, ZipTest, testSyncAtEndOfArchive);

	return pSuite;
}
