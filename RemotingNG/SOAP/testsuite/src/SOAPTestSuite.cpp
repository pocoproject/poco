//
// SOAPTestSuite.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "SOAPTestSuite.h"
#include "SerializerTest.h"
#include "DeserializerTest.h"
#include "RemotingTest.h"


CppUnit::Test* SOAPTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SOAPTestSuite");

	pSuite->addTest(SerializerTest::suite());
	pSuite->addTest(DeserializerTest::suite());
	pSuite->addTest(RemotingTestSOAP11::suite());
	pSuite->addTest(RemotingTestSOAP12::suite());
	pSuite->addTest(RemotingTestSOAP12Compressed::suite());
	pSuite->addTest(RemotingTestSOAP11NonChunked::suite());
	pSuite->addTest(RemotingTestSOAP12MTOM::suite());
	pSuite->addTest(RemotingTestSOAP12Auth::suite());
	pSuite->addTest(RemotingTestSOAP12AuthChunked::suite());

	return pSuite;
}
