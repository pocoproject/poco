//
// FIFOBufferTest.h
//
// $Id: //poco/1.6/Foundation/testsuite/src/FIFOBufferTest.h#1 $
//
// Definition of the FIFOBufferTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FIFOBufferTest_INCLUDED
#define FIFOBufferTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"


class FIFOBufferTest: public CppUnit::TestCase
{
public:
	FIFOBufferTest(const std::string& name);
	~FIFOBufferTest();

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


#endif // FIFOBufferTest_INCLUDED
