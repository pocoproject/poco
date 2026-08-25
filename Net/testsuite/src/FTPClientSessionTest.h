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


#ifndef FTPClientSessionTest_INCLUDED
#define FTPClientSessionTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


namespace Poco::Net {

class FTPClientSession;
class DialogSocket;

} // namespace Poco::Net

class DialogServer;

class FTPClientSessionTest: public CppUnit::TestCase
{
public:
	FTPClientSessionTest(const std::string& name);
	~FTPClientSessionTest();

	void testLogin1();
	void testLogin2();
	void testLogin3();
	void testLoginFailed1();
	void testLoginFailed2();
	void testWelcomeMessageRead();
	void testWelcomeMessageNotRead();
	void testConstructorFailureClosesSocket1();
	void testConstructorFailureClosesSocket2();
	void testCommands();
	void testDownloadPORT();
	void testDownloadEPRT();
	void testDownloadPASV();
	void testDownloadEPSV();
	void testUpload();
	void testList();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void login(DialogServer& server, Poco::Net::FTPClientSession& session);
	void assertConnectionClosed(Poco::Net::DialogSocket& peer);
};


#endif // FTPClientSessionTest_INCLUDED
