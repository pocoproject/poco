//
// PDFTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PDFTest.h"
#include "Poco/PDF/Document.h"
#include "Poco/PDF/PDFException.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Exception.h"
#include <fstream>
#include <sstream>
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


PDFTest::PDFTest(const std::string& name): CppUnit::TestCase(name)
{
}


PDFTest::~PDFTest()
{
}


void PDFTest::testDocument()
{
	// createNew() frees the pages, fonts and outlines held by libharu
	Poco::PDF::Document doc;
	assertEqual (1, doc.pageCount());
	(void) doc.font("Helvetica");
	doc.createNew();
	assertEqual (0, doc.pageCount());
	Poco::PDF::Page& page = const_cast<Poco::PDF::Page&>(doc.addPage());
	page.setFont(doc.font("Helvetica"), 12);
	assertEqual (1, doc.pageCount());
}


void PDFTest::testInsertPage()
{
	Poco::PDF::Document doc(3);
	assertEqual (3, doc.pageCount());

	const Poco::PDF::Page& middle = doc.insertPage(1);
	assertEqual (4, doc.pageCount());
	assertTrue (&middle == &doc.getPage(1));

	const Poco::PDF::Page& first = doc.insertPage(0);
	assertEqual (5, doc.pageCount());
	assertTrue (&first == &doc.getPage(0));
}


void PDFTest::testPage()
{
	warn("not implemented");
}


void PDFTest::testImage()
{
	static const char png[] =
		"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAADElEQVR4nGP4//8/AAX+Av4N70a4AAAAAElFTkSuQmCC";

	Poco::TemporaryFile dir;
	(void) dir.createDirectory();
	const std::string fileA = dir.path() + "/a_logo.png";
	const std::string fileB = dir.path() + "/b_logo.png";
	for (const auto& name: {fileA, fileB})
	{
		std::istringstream istr(png);
		Poco::Base64Decoder decoder(istr);
		std::ofstream ostr(name, std::ios::binary);
		Poco::StreamCopier::copyStream(decoder, ostr);
	}

	Poco::PDF::Document doc;
	const Poco::PDF::Image& image = doc.loadPNGImage(fileA);
	assertTrue (&doc.loadPNGImage(fileA) == &image);
	assertTrue (&doc.loadPNGImage(fileB) != &image);

	// same base name in different directories
	Poco::TemporaryFile dir2;
	(void) dir2.createDirectory();
	const std::string fileC = dir2.path() + "/a_logo.png";
	std::istringstream istr(png);
	Poco::Base64Decoder decoder(istr);
	std::ofstream ostr(fileC, std::ios::binary);
	Poco::StreamCopier::copyStream(decoder, ostr);
	ostr.close();
	assertTrue (&doc.loadPNGImage(fileC) != &image);
}


void PDFTest::testFont()
{
	Poco::PDF::Document doc;
	const Poco::PDF::Font& f1 = doc.loadFont("Helvetica", "WinAnsiEncoding");
	assertTrue (&doc.loadFont("Helvetica", "WinAnsiEncoding") == &f1);
	assertTrue (&doc.font("Helvetica", "WinAnsiEncoding") == &f1);

	// the encoding is part of the cache key
	const Poco::PDF::Font& f2 = doc.font("Helvetica", "CP1250");
	assertTrue (&f2 != &f1);
	assertTrue (f1.encodingName() == "WinAnsiEncoding");
	assertTrue (f2.encodingName() == "CP1250");
}


void PDFTest::testEncoding()
{
	Poco::PDF::Document doc;
	const Poco::PDF::Encoder& enc = doc.loadEncoder("WinAnsiEncoding");
	assertTrue (enc.name() == "WinAnsiEncoding");
	assertTrue (enc.getType() == Poco::PDF::Encoder::ENCODER_TYPE_SINGLE_BYTE);

	const Poco::PDF::Encoder& again = doc.loadEncoder("WinAnsiEncoding");
	assertTrue (&again == &enc);

	const Poco::PDF::Encoder& current = doc.setCurrentEncoder("WinAnsiEncoding");
	assertTrue (current.name() == "WinAnsiEncoding");
	assertTrue (&doc.getCurrentEncoder() == &enc);
}


void PDFTest::testNoCurrentEncoder()
{
	Poco::PDF::Document doc;
	try
	{
		(void) doc.getCurrentEncoder();
		fail("no current encoder must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void PDFTest::testOutline()
{
	warn("not implemented");
}


void PDFTest::testDestination()
{
	warn("not implemented");
}


void PDFTest::testAnnotation()
{
	warn("not implemented");
}


void PDFTest::setUp()
{
}


void PDFTest::tearDown()
{
}


CppUnit::Test* PDFTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PDFTest");

	CppUnit_addTest(pSuite, PDFTest, testDocument);
	CppUnit_addTest(pSuite, PDFTest, testInsertPage);
	CppUnit_addTest(pSuite, PDFTest, testPage);
	CppUnit_addTest(pSuite, PDFTest, testImage);
	CppUnit_addTest(pSuite, PDFTest, testFont);
	CppUnit_addTest(pSuite, PDFTest, testEncoding);
	CppUnit_addTest(pSuite, PDFTest, testNoCurrentEncoder);
	CppUnit_addTest(pSuite, PDFTest, testOutline);
	CppUnit_addTest(pSuite, PDFTest, testDestination);
	CppUnit_addTest(pSuite, PDFTest, testAnnotation);

	return pSuite;
}
