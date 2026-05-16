//
// FTPClientSessionTest.h
//
// Definition of the FTPClientSessionTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FTPSClientSessionTest_INCLUDED
#define FTPSClientSessionTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


namespace Poco::Net {

class FTPSClientSession;

} // namespace Poco::Net


class DialogServer;


class FTPSClientSessionTest: public CppUnit::TestCase
{
public:
	FTPSClientSessionTest(const std::string& name);
	~FTPSClientSessionTest();

	void testLogin1();
	void testLogin2();
	void testLogin3();
	void testLoginFailed1();
	void testLoginFailed2();
	void testCommands();
	void testDownloadPORT();
	void testDownloadEPRT();
	void testDownloadPASV();
	void testDownloadEPSV();
	void testUpload();
	void testList();
	void testUploadSSL();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void login(DialogServer& server, Poco::Net::FTPSClientSession& session);
};


#endif // FTPSClientSessionTest_INCLUDED
