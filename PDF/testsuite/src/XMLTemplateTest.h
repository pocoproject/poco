//
// XMLTemplateTest.h
//
// Definition of the XMLTemplateTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XMLTemplateTest_INCLUDED
#define XMLTemplateTest_INCLUDED


#include "Poco/PDF/PDF.h"
#include "CppUnit/TestCase.h"


class XMLTemplateTest: public CppUnit::TestCase
	/// Tests of Poco::PDF::XMLTemplate.
{
public:
	XMLTemplateTest() = delete;
		/// Not supported; a test case needs a name.

	explicit XMLTemplateTest(const std::string& name);
		/// Creates the XMLTemplateTest.

	~XMLTemplateTest() override;
		/// Destroys the XMLTemplateTest.

	void testImageKeepAspectRatioFromWidth();
		/// Checks that an image with only a width keeps its aspect ratio.

	void testImageKeepAspectRatioFromHeight();
		/// Checks that an image with only a height keeps its aspect ratio.

	void testImageKeepAspectRatioWithBothSizes();
		/// Checks that an image with width and height keeps both.

	void testImageOffset();
		/// Checks that offset-x and offset-y move the drawn image.

	void setUp() override;
		/// Does nothing.

	void tearDown() override;
		/// Does nothing.

	static CppUnit::Test* suite();
		/// Returns the suite of all tests of this class.
};


#endif // XMLTemplateTest_INCLUDED
