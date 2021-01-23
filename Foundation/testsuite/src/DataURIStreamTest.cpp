//
// DataURIStreamTest.cpp
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DataURIStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DataURIStream.h"
#include "Poco/Exception.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::DataFormatException;
using Poco::DataURIStream;
using Poco::StreamCopier;
using Poco::URI;


DataURIStreamTest::DataURIStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


DataURIStreamTest::~DataURIStreamTest()
{
}


void DataURIStreamTest::testWithBase64()
{
	{
		DataURIStream ds(URI("data:;base64,SGVsbG8sIFdvcmxkIQ%3D%3D"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str() == "Hello, World!");
	}
	{
		DataURIStream ds(URI("data:text/vnd-example+xyz;foo=bar;base64,R0lGODdh"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str() == "GIF87a");
	}
	{
		DataURIStream ds(URI("data:application/octet-stream;base64,A+b/7A=="));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str() == "\x03\xE6\xFF\xEC");
	}
}


void DataURIStreamTest::testWithoutBase64()
{
	{
		DataURIStream ds(URI("data:,Hello%2C%20World!"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str() == "Hello, World!");
	}
	{
		DataURIStream ds(URI("data:text/plain;charset=UTF-8;page=21,the%20data:1234,5678"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str() == "the data:1234,5678");
	}
}


void DataURIStreamTest::testZeroLengthData()
{
	{
		DataURIStream ds(URI("data:text/plain;base64,"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str().empty());
	}
	{
		DataURIStream ds(URI("data:,"));
		std::ostringstream ostr;
		StreamCopier::copyStream(ds, ostr);
		assertTrue (ostr.str().empty());
	}
}


void DataURIStreamTest::setUp()
{
}


void DataURIStreamTest::tearDown()
{
}


CppUnit::Test* DataURIStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DataURIStreamTest");

	CppUnit_addTest(pSuite, DataURIStreamTest, testWithBase64);
	CppUnit_addTest(pSuite, DataURIStreamTest, testWithoutBase64);
	CppUnit_addTest(pSuite, DataURIStreamTest, testZeroLengthData);

	return pSuite;
}
