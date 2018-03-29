//
// SMTPClientSessionTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SMTPClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "DialogServer.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/NetException.h"


using Poco::Net::SMTPClientSession;
using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::SMTPException;


SMTPClientSessionTest::SMTPClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


SMTPClientSessionTest::~SMTPClientSessionTest()
{
}


void SMTPClientSessionTest::testLoginEHLO()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");
	std::string cmd = server.popCommand();
	assertTrue (cmd == "EHLO localhost");
	session.close();
	cmd = server.popCommand();
	assertTrue (cmd == "QUIT");
}


void SMTPClientSessionTest::testLoginHELO()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("500 EHLO not understood");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");
	std::string cmd = server.popCommand();
	assertTrue (cmd == "EHLO localhost");
	cmd = server.popCommand();
	assertTrue (cmd == "HELO localhost");
	session.close();
	cmd = server.popCommand();
	assertTrue (cmd == "QUIT");
}


void SMTPClientSessionTest::testLoginFailed()
{
	DialogServer server;
	server.addResponse("500 No SMTP service here");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	try
	{
		session.login("localhost");
		fail("cannot login - must throw");
	}
	catch (SMTPException&)
	{
	}
	session.close();
}


void SMTPClientSessionTest::testSend()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("250 OK");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");
	server.clearCommands();
	session.sendMessage(message);
	std::string cmd = server.popCommandWait();
	assertTrue (cmd == "MAIL FROM: <john.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "DATA");
	cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "Date");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Type: text/plain");
	cmd = server.popCommandWait();
	assertTrue (cmd == "From: john.doe@no.where");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Subject: Test Message");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Transfer-Encoding: quoted-printable");
	cmd = server.popCommandWait();
	assertTrue (cmd == "To: Jane Doe <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Hello");
	cmd = server.popCommandWait();
	assertTrue (cmd == "blah blah");
	cmd = server.popCommandWait();
	assertTrue (cmd == "John");
	cmd = server.popCommandWait();
	assertTrue (cmd == ".");

	session.close();
}


void SMTPClientSessionTest::testSendMultiRecipient()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	MailMessage::Recipients msgRecipients;
	msgRecipients.push_back(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	msgRecipients.push_back(MailRecipient(MailRecipient::CC_RECIPIENT, "jack.doe@no.where", "Jack Doe"));
	msgRecipients.push_back(MailRecipient(MailRecipient::BCC_RECIPIENT, "joe.doe@no.where", "Joe Doe"));
	message.setRecipients(msgRecipients);
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");

	server.clearCommands();
	session.sendMessage(message);
	std::string cmd = server.popCommandWait();
	assertTrue (cmd == "MAIL FROM: <john.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <jack.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <joe.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "DATA");
	cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "Date");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Type: text/plain");
	cmd = server.popCommandWait();
	assertTrue (cmd == "From: john.doe@no.where");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Subject: Test Message");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Transfer-Encoding: quoted-printable");
	cmd = server.popCommandWait();
	assertTrue (cmd == "To: Jane Doe <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "CC: Jack Doe <jack.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Hello");
	cmd = server.popCommandWait();
	assertTrue (cmd == "blah blah");
	cmd = server.popCommandWait();
	assertTrue (cmd == "John");
	cmd = server.popCommandWait();
	assertTrue (cmd == ".");

	session.close();
}


void SMTPClientSessionTest::testMultiSeparateRecipient()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	MailMessage::Recipients msgRecipients;
	msgRecipients.push_back(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	msgRecipients.push_back(MailRecipient(MailRecipient::CC_RECIPIENT, "jack.doe@no.where", "Jack Doe"));
	msgRecipients.push_back(MailRecipient(MailRecipient::CC_RECIPIENT, "joe.doe@no.where", "Joe Doe"));
	message.setRecipients(msgRecipients);
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");

	SMTPClientSession::Recipients recipients;
	recipients.push_back("jill.doe@no.where");
	recipients.push_back("josh.doe@no.where");
	recipients.push_back("jake.doe@no.where");

	server.clearCommands();
	session.sendMessage(message, recipients);
	std::string cmd = server.popCommandWait();
	assertTrue (cmd == "MAIL FROM: <john.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <jill.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <josh.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "RCPT TO: <jake.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "DATA");
	cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "Date");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Type: text/plain");
	cmd = server.popCommandWait();
	assertTrue (cmd == "From: john.doe@no.where");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Subject: Test Message");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Content-Transfer-Encoding: quoted-printable");
	cmd = server.popCommandWait();
	assertTrue (cmd == "To: Jane Doe <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "CC: Jack Doe <jack.doe@no.where>, Joe Doe <joe.doe@no.where>");
	cmd = server.popCommandWait();
	assertTrue (cmd == "Hello");
	cmd = server.popCommandWait();
	assertTrue (cmd == "blah blah");
	cmd = server.popCommandWait();
	assertTrue (cmd == "John");
	cmd = server.popCommandWait();
	assertTrue (cmd == ".");

	session.close();
}


void SMTPClientSessionTest::testSendFailed()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("500 Error");
	server.addResponse("221 Bye");
	SMTPClientSession session("127.0.0.1", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");
	server.clearCommands();
	try
	{
		session.sendMessage(message);
		fail("internal error - must throw");
	}
	catch (SMTPException&)
	{
	}
	
	session.close();
}


void SMTPClientSessionTest::setUp()
{
}


void SMTPClientSessionTest::tearDown()
{
}


CppUnit::Test* SMTPClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SMTPClientSessionTest");

	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginEHLO);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginHELO);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginFailed);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testSend);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testSendMultiRecipient);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testMultiSeparateRecipient);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testSendFailed);

	return pSuite;
}
