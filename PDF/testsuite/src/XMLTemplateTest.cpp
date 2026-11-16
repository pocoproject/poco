//
// XMLTemplateTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "XMLTemplateTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/PDF/XMLTemplate.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/InflatingStream.h"
#include "Poco/NumberParser.h"
#include "Poco/Path.h"
#include "Poco/RegularExpression.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include <iterator>
#include <sstream>
#include <string>
#include <vector>


namespace
{
	struct ImagePlacement
		/// The position and size of an image on a page, in points.
	{
		double x = 0;
		double y = 0;
		double width = 0;
		double height = 0;
	};

	// A grayscale PNG image of 3 x 2 pixels.
	constexpr unsigned char PNG_IMAGE[] =
	{
		0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
		0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x1F, 0x39,
		0xC6, 0x00, 0x00, 0x00, 0x0E, 0x49, 0x44, 0x41, 0x54, 0x78, 0xDA, 0x63, 0x68, 0x68, 0x68, 0x60,
		0x00, 0x62, 0x00, 0x0C, 0x08, 0x03, 0x01, 0xD8, 0xC9, 0xF7, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x49,
		0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
	};

	// The PDF writer prints at least five significant digits.
	constexpr double POSITION_TOLERANCE = 0.001;

	[[nodiscard]] std::string inflateStreams(const std::string& pdf)
		/// Returns the decompressed data of all streams in the given PDF file content.
	{
		static const std::string STREAM_BEGIN("\nstream\r\n");
		static const std::string STREAM_END("\nendstream");

		std::string data;
		std::string::size_type pos = pdf.find(STREAM_BEGIN);
		while (pos != std::string::npos)
		{
			const std::string::size_type begin = pos + STREAM_BEGIN.size();
			const std::string::size_type end = pdf.find(STREAM_END, begin);
			if (end == std::string::npos) break;
			std::istringstream compressed(pdf.substr(begin, end - begin));
			Poco::InflatingInputStream inflater(compressed);
			Poco::StreamCopier::copyToString(inflater, data);
			pos = pdf.find(STREAM_BEGIN, end);
		}
		return data;
	}

	[[nodiscard]] ImagePlacement placeImage(const std::string& imageStyle)
		/// Creates a PDF file from a template with one <img> element with the given
		/// style and returns where the page content draws the image.
		///
		/// The PDF library cannot read a page back, so the transformation matrix
		/// that the page content sets before it draws the image is parsed instead.
	{
		Poco::TemporaryFile tempDir;
		Poco::Path dir(tempDir.path());
		dir.makeDirectory();
		Poco::File(dir).createDirectories();
		{
			Poco::FileOutputStream imageStream(Poco::Path(dir, "image.png").toString());
			imageStream << std::string(std::begin(PNG_IMAGE), std::end(PNG_IMAGE));
		}

		std::istringstream xml("<document><page><img src=\"image.png\" style=\"" + imageStyle + "\"/></page></document>");
		Poco::PDF::XMLTemplate pdfTemplate(xml, dir.toString());
		const std::string pdfPath = Poco::Path(dir, "image.pdf").toString();
		pdfTemplate.create(pdfPath);

		std::string pdf;
		Poco::FileInputStream pdfStream(pdfPath);
		Poco::StreamCopier::copyToString(pdfStream, pdf);

		// width 0 0 height x y cm, then the image XObject is drawn
		static const Poco::RegularExpression DRAW_IMAGE(R"((\S+) 0 0 (\S+) (\S+) (\S+) cm\n/\S+ Do)");
		constexpr int DRAW_IMAGE_FIELDS = 5; // the whole match and the four numbers
		std::vector<std::string> fields;
		if (DRAW_IMAGE.split(inflateStreams(pdf), fields) != DRAW_IMAGE_FIELDS)
			throw Poco::DataFormatException("the page content does not draw an image");

		ImagePlacement placement;
		placement.width = Poco::NumberParser::parseFloat(fields[1]);
		placement.height = Poco::NumberParser::parseFloat(fields[2]);
		placement.x = Poco::NumberParser::parseFloat(fields[3]);
		placement.y = Poco::NumberParser::parseFloat(fields[4]);
		return placement;
	}
}


XMLTemplateTest::XMLTemplateTest(const std::string& name): CppUnit::TestCase(name)
{
}


XMLTemplateTest::~XMLTemplateTest() = default;


void XMLTemplateTest::testImageKeepAspectRatioFromWidth()
{
	const ImagePlacement placement = placeImage("left: 100; bottom: 200; width: 100; keep-aspect-ratio: true");
	assertEqualDelta (100.0, placement.width, POSITION_TOLERANCE);
	assertEqualDelta (100.0*2/3, placement.height, POSITION_TOLERANCE);
	assertEqualDelta (100.0, placement.x, POSITION_TOLERANCE);
	assertEqualDelta (200.0, placement.y, POSITION_TOLERANCE);
}


void XMLTemplateTest::testImageKeepAspectRatioFromHeight()
{
	const ImagePlacement placement = placeImage("left: 10; bottom: 20; height: 50; scale: 2; keep-aspect-ratio: true");
	assertEqualDelta (150.0, placement.width, POSITION_TOLERANCE);
	assertEqualDelta (100.0, placement.height, POSITION_TOLERANCE);
	assertEqualDelta (10.0, placement.x, POSITION_TOLERANCE);
	assertEqualDelta (20.0, placement.y, POSITION_TOLERANCE);
}


void XMLTemplateTest::testImageKeepAspectRatioWithBothSizes()
{
	const ImagePlacement placement = placeImage("left: 10; bottom: 20; width: 40; height: 30; keep-aspect-ratio: true");
	assertEqualDelta (40.0, placement.width, POSITION_TOLERANCE);
	assertEqualDelta (30.0, placement.height, POSITION_TOLERANCE);
}


void XMLTemplateTest::testImageOffset()
{
	const ImagePlacement placement = placeImage("left: 100; bottom: 200; offset-x: 10.5; offset-y: -20");
	assertEqualDelta (110.5, placement.x, POSITION_TOLERANCE);
	assertEqualDelta (180.0, placement.y, POSITION_TOLERANCE);
	assertEqualDelta (3.0, placement.width, POSITION_TOLERANCE);
	assertEqualDelta (2.0, placement.height, POSITION_TOLERANCE);

	// A negative position counts from the right edge of the box; an offset does not.
	const ImagePlacement leftOfBox = placeImage("left: 5; bottom: 200; offset-x: -10");
	assertEqualDelta (-5.0, leftOfBox.x, POSITION_TOLERANCE);
}


void XMLTemplateTest::setUp()
{
}


void XMLTemplateTest::tearDown()
{
}


CppUnit::Test* XMLTemplateTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XMLTemplateTest");

	CppUnit_addTest(pSuite, XMLTemplateTest, testImageKeepAspectRatioFromWidth);
	CppUnit_addTest(pSuite, XMLTemplateTest, testImageKeepAspectRatioFromHeight);
	CppUnit_addTest(pSuite, XMLTemplateTest, testImageKeepAspectRatioWithBothSizes);
	CppUnit_addTest(pSuite, XMLTemplateTest, testImageOffset);

	return pSuite;
}
