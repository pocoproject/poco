//
// DeserializerTest.h
//
// Definition of the DeserializerTest class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef DeserializerTest_INCLUDED
#define DeserializerTest_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "CppUnit/TestCase.h"


class DeserializerTest: public CppUnit::TestCase
{
public:
	DeserializerTest(const std::string& name);
	~DeserializerTest();

	void testRequestNoParams11();
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
	void testRequestWithAttrsInc();
	void testRequestWithAttrsOptional();
	void testVector();
	void testHeader();
	void testHeaderAttr();
	void testHeaderAttrOptional();
	
	void testCharVector();
	void testCharVectorNil1();
	void testCharVectorNil2();
	void testCharVectorNotNil();
	void testMTOM();
	
	void testStructAsRoot();

	void testDateTime();
	void testLocalDateTime();

	void testInterceptor();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // DeserializerTest_INCLUDED
