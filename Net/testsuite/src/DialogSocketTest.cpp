//
// DialogSocketTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DialogSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "EchoServer.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/SocketAddress.h"
#include <cstring>


using Poco::Net::DialogSocket;
using Poco::Net::SocketAddress;


DialogSocketTest::DialogSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


DialogSocketTest::~DialogSocketTest()
{
}


void DialogSocketTest::testDialogSocket()
{
	EchoServer echoServer;
	DialogSocket ds;
	ds.connect(SocketAddress("127.0.0.1", echoServer.port()));

	ds.sendMessage("Hello, world!");
	std::string str;
	ds.receiveMessage(str);
	assertTrue (str == "Hello, world!");

	ds.sendString("Hello, World!\n");
	ds.receiveMessage(str);
	assertTrue (str == "Hello, World!");
	
	ds.sendMessage("EHLO", "appinf.com");
	ds.receiveMessage(str);
	assertTrue (str == "EHLO appinf.com");
	
	ds.sendMessage("PUT", "local.txt", "remote.txt");
	ds.receiveMessage(str);
	assertTrue (str == "PUT local.txt remote.txt");

	ds.sendMessage("220 Hello, world!");
	int status = ds.receiveStatusMessage(str);
	assertTrue (status == 220);
	assertTrue (str == "220 Hello, world!");
	
	ds.sendString("220-line1\r\n220 line2\r\n");
	status = ds.receiveStatusMessage(str);
	assertTrue (status == 220);
	assertTrue (str == "220-line1\n220 line2");
	
	ds.sendString("220-line1\r\nline2\r\n220 line3\r\n");
	status = ds.receiveStatusMessage(str);
	assertTrue (status == 220);
	assertTrue (str == "220-line1\nline2\n220 line3");

	ds.sendMessage("Hello, world!");
	status = ds.receiveStatusMessage(str);
	assertTrue (status == 0);
	assertTrue (str == "Hello, world!");
	
	ds.sendString("Header\nMore Bytes");
	status = ds.receiveStatusMessage(str);
	assertTrue (status == 0);
	assertTrue (str == "Header");
	char buffer[16];
	int n = ds.receiveRawBytes(buffer, sizeof(buffer));
	assertTrue (n == 10);
	assertTrue (std::memcmp(buffer, "More Bytes", 10) == 0);

	ds.sendString("Even More Bytes");
	n = ds.receiveRawBytes(buffer, sizeof(buffer));
	assertTrue (n == 15);
	assertTrue (std::memcmp(buffer, "Even More Bytes", 15) == 0);
}


void DialogSocketTest::setUp()
{
}


void DialogSocketTest::tearDown()
{
}


CppUnit::Test* DialogSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DialogSocketTest");

	CppUnit_addTest(pSuite, DialogSocketTest, testDialogSocket);

	return pSuite;
}
