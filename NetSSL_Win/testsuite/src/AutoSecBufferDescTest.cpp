//
// AutoSecBufferDescTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


// A program that uses SSPI itself may define SECURITY_WIN32 before it includes
// any header. Poco/Net/AutoSecBufferDesc.h must declare the SSPI types it uses
// in that case as well, or this file does not compile.
#define SECURITY_WIN32
#include "AutoSecBufferDescTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/AutoSecBufferDesc.h"


using Poco::Net::AutoSecBufferDesc;


AutoSecBufferDescTest::AutoSecBufferDescTest(const std::string& name): CppUnit::TestCase(name)
{
}


AutoSecBufferDescTest::~AutoSecBufferDescTest() = default;


void AutoSecBufferDescTest::testSecurityWin32Defined()
{
	SecurityFunctionTableW* pSec = ::InitSecurityInterfaceW();
	assertTrue (pSec != nullptr);

	char data[16] = {};
	AutoSecBufferDesc<2> desc(pSec, false);
	desc.setSecBufferData(0, data, sizeof(data));

	assertEqual (2ul, desc.cBuffers);
	assertEqual (static_cast<unsigned long>(SECBUFFER_VERSION), desc.ulVersion);
	assertTrue (desc[0].pvBuffer == data);
	assertEqual (static_cast<unsigned long>(sizeof(data)), desc[0].cbBuffer);
	assertEqual (static_cast<unsigned long>(SECBUFFER_DATA), desc[0].BufferType);
	assertEqual (static_cast<unsigned long>(SECBUFFER_EMPTY), desc[1].BufferType);
}


void AutoSecBufferDescTest::setUp()
{
}


void AutoSecBufferDescTest::tearDown()
{
}


CppUnit::Test* AutoSecBufferDescTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AutoSecBufferDescTest");

	CppUnit_addTest(pSuite, AutoSecBufferDescTest, testSecurityWin32Defined);

	return pSuite;
}
