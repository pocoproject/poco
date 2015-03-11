//
// BasicFIFOBufferTest.h
//
// $Id: //poco/1.6/Foundation/testsuite/src/BasicFIFOBufferTest.h#1 $
//
// Definition of the BasicFIFOBufferTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef BasicFIFOBufferTest_INCLUDED
#define BasicFIFOBufferTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class BasicFIFOBufferTest: public CppUnit::TestCase
{
public:
	BasicFIFOBufferTest(const std::string& name);
	~BasicFIFOBufferTest();

	void testNextWrite();
	void testChar();
	void testInt();
	void testEOFAndError();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void onReadable(bool& b);
	void onWritable(bool& b);

private:
	int _notToReadable;
	int _notToWritable;
	int _readableToNot;
	int _writableToNot;
};


#endif // BasicFIFOBufferTest_INCLUDED
