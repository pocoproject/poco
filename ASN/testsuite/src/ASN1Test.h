//
// ASN1Test.h
//
// $Id: //poco/1.4/Foundation/testsuite/src/ASN1Test.h#1 $
//
// Definition of the ASN1Test class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ASN1Test_INCLUDED
#define ASN1Test_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class ASN1Test: public CppUnit::TestCase
{
public:
	ASN1Test(const std::string& name);
	~ASN1Test();

	void testEncoder();
	void testDecoder();
	void testEncodeDecode();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // ASN1Test_INCLUDED
