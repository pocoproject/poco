//
// SerializerTest.h
//
// Definition of the SerializerTest class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SerializerTest_INCLUDED
#define SerializerTest_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "CppUnit/TestCase.h"


class SerializerTest: public CppUnit::TestCase
{
public:
	SerializerTest(const std::string& name);
	~SerializerTest();

	void testRequestNoParams11();
	void testRequestNoParams11DeclareEncNS();
	void testRequestNoParams11NoEncodingStyle();
	void testRequestTwoParams11();
	void testReplyNoParams11();
	void testReplyTwoParams11();
	void testFault11();

	void testRequestNoParams12();
	void testRequestTwoParams12();
	void testReplyNoParams12();
	void testReplyTwoParams12();
	void testFault12();

	void testRequestNoParamsDL();
	void testRequestTwoParamsDL();
	void testReplyNoParamsDL();
	void testReplyTwoParamsDL();

	void testRequestTwoAttrs();
	void testRequestWithAttrs();
	void testComplexRequest();
	void testComplexRequestNS();
	void testVector();
	void testSubElemAttrs();
	void testHeader();
	void testHeaderAttr();

	void testCharVector();
	void testCharVectorNil();
	void testCharVectorNotNil();
	void testNilNS();
	void testMTOM();

	void testStructAsRoot();

	void testDateTime();
	void testLocalDateTime();

	void testInterceptor();
	void testInterceptorComplex();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SerializerTest_INCLUDED
