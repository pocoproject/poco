//
// MailMessageTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MailMessageTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailStream.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/PartStore.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timestamp.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Stopwatch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


using Poco::Net::MailMessage;
using Poco::Net::MailInputStream;
using Poco::Net::MailOutputStream;
using Poco::Net::MailRecipient;
using Poco::Net::MessageHeader;
using Poco::Net::MessageException;
using Poco::Net::PartHandler;
using Poco::Net::MediaType;
using Poco::Net::StringPartSource;
using Poco::Net::FilePartStoreFactory;
using Poco::Net::FilePartStore;
using Poco::Timestamp;
using Poco::FileInputStream;
using Poco::replaceInPlace;
using Poco::icompare;
using Poco::TemporaryFile;


namespace
{
	class StringPartHandler: public PartHandler
	{
	public:
		StringPartHandler() = default;

		void handlePart(const MessageHeader& header, std::istream& stream) override
		{
			_disp.push_back(header["Content-Disposition"]);
			_type.push_back(header["Content-Type"]);
			std::string data;
			Poco::StreamCopier::copyToString(stream, data);
			_data.push_back(std::move(data));
		}

		[[nodiscard]] const std::vector<std::string>& data() const
		{
			return _data;
		}

		[[nodiscard]] const std::vector<std::string>& disp() const
		{
			return _disp;
		}

		[[nodiscard]] const std::vector<std::string>& type() const
		{
			return _type;
		}

	private:
		std::vector<std::string> _data;
		std::vector<std::string> _disp;
		std::vector<std::string> _type;
	};
}


MailMessageTest::MailMessageTest(const std::string& name): CppUnit::TestCase(name)
{
}


MailMessageTest::~MailMessageTest()
{
}


void MailMessageTest::testWriteQP()
{
	MailMessage message;
	MailRecipient r1(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe");
	MailRecipient r2(MailRecipient::CC_RECIPIENT, "jane.doe@no.where", "Jane Doe");
	MailRecipient r3(MailRecipient::BCC_RECIPIENT, "walter.foo@no.where", "Frank Foo");
	MailRecipient r4(MailRecipient::BCC_RECIPIENT, "bernie.bar@no.where", "Bernie Bar");
	message.addRecipient(r1);
	message.addRecipient(r2);
	message.addRecipient(r3);
	message.addRecipient(r4);
	message.setSubject("Test Message");
	message.setSender("poco@appinf.com");
	message.setContent(
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
		"To test the quoted-printable encoding, we'll put an extra long line here. This should be enough.\r\n"
		"And here is some more =fe.\r\n"
	);
	Timestamp ts(0);
	message.setDate(ts);

	assertTrue (!message.isMultipart());

	std::ostringstream str;
	message.write(str);
	std::string s = str.str();

	assertTrue (s ==
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"Subject: Test Message\r\n"
		"From: poco@appinf.com\r\n"
		"Content-Transfer-Encoding: quoted-printable\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"CC: Jane Doe <jane.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
		"To test the quoted-printable encoding, we'll put an extra long line here. T=\r\n"
		"his should be enough.\r\n"
		"And here is some more =3Dfe.\r\n"
	);
}


void MailMessageTest::testWrite8Bit()
{
	MailMessage message;
	MailRecipient r1(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe");
	message.addRecipient(r1);
	message.setSubject("Test Message");
	message.setSender("poco@appinf.com");
	message.setContent(
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n",
		MailMessage::ENCODING_8BIT
	);
	Timestamp ts(0);
	message.setDate(ts);

	std::ostringstream str;
	message.write(str);
	std::string s = str.str();
	assertTrue (s ==
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"Subject: Test Message\r\n"
		"From: poco@appinf.com\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);
}


void MailMessageTest::testWriteBase64()
{
	MailMessage message;
	MailRecipient r1(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe");
	message.addRecipient(r1);
	message.setSubject("Test Message");
	message.setSender("poco@appinf.com");
	message.setContent(
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n",
		MailMessage::ENCODING_BASE64
	);
	Timestamp ts(0);
	message.setDate(ts);

	std::ostringstream str;
	message.write(str);
	std::string s = str.str();
	assertTrue (s ==
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"Subject: Test Message\r\n"
		"From: poco@appinf.com\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"SGVsbG8sIHdvcmxkIQ0KVGhpcyBpcyBhIHRlc3QgZm9yIHRoZSBNYWlsTWVzc2FnZSBjbGFz\r\n"
		"cy4NCg=="
	);
}


void MailMessageTest::testWriteManyRecipients()
{
	MailMessage message;
	MailRecipient r1(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe");
	MailRecipient r2(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe");
	MailRecipient r3(MailRecipient::PRIMARY_RECIPIENT, "walter.foo@no.where", "Frank Foo");
	MailRecipient r4(MailRecipient::PRIMARY_RECIPIENT, "bernie.bar@no.where", "Bernie Bar");
	MailRecipient r5(MailRecipient::PRIMARY_RECIPIENT, "joe.spammer@no.where", "Joe Spammer");
	message.addRecipient(r1);
	message.addRecipient(r2);
	message.addRecipient(r3);
	message.addRecipient(r4);
	message.addRecipient(r5);
	message.setSubject("Test Message");
	message.setSender("poco@appinf.com");
	message.setContent(
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n",
		MailMessage::ENCODING_8BIT
	);
	Timestamp ts(0);
	message.setDate(ts);

	std::ostringstream str;
	message.write(str);
	std::string s = str.str();
	assertTrue (s ==
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"Subject: Test Message\r\n"
		"From: poco@appinf.com\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"To: John Doe <john.doe@no.where>, Jane Doe <jane.doe@no.where>, \r\n"
		"\tFrank Foo <walter.foo@no.where>, Bernie Bar <bernie.bar@no.where>, \r\n"
		"\tJoe Spammer <joe.spammer@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);
}


void MailMessageTest::testWriteMultiPart()
{
	MailMessage message;
	MailRecipient r1(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe");
	message.addRecipient(r1);
	message.setSubject("Test Message");
	message.setSender("poco@appinf.com");
	Timestamp ts(0);
	message.setDate(ts);
	message.addContent(new StringPartSource("Hello World!\r\n", "text/plain"), MailMessage::ENCODING_8BIT);
	StringPartSource* pSPS = new StringPartSource("This is some binary data. Really.", "application/octet-stream", "sample.dat");
	pSPS->headers().set("Content-ID", "abcd1234");
	message.addAttachment("sample", pSPS);

	assertTrue (message.isMultipart());

	std::ostringstream str;
	message.write(str);
	std::string s = str.str();
	std::string rawMsg(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: multipart/mixed; boundary=$\r\n"
		"Subject: Test Message\r\n"
		"From: poco@appinf.com\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"Mime-Version: 1.0\r\n"
		"\r\n"
		"--$\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Disposition: inline\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--$\r\n"
		"Content-ID: abcd1234\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--$--\r\n"
	);
	std::string::size_type p1 = s.find('=') + 1;
	std::string::size_type p2 = s.find('\r', p1);
	std::string boundary(s, p1, p2 - p1);
	std::string msg;
	for (std::string::const_iterator it = rawMsg.begin(); it != rawMsg.end(); ++it)
	{
		if (*it == '$')
			msg += boundary;
		else
			msg += *it;
	}

	assertTrue (s == msg);
}


void MailMessageTest::testReadQP()
{
	std::istringstream istr(
		"Content-Transfer-Encoding: quoted-printable\r\n"
		"Content-Type: text/plain\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
		"To test the quoted-printable encoding, we'll put an extra long line here. T=\r\n"
		"his should be enough.\r\n"
		"And here is some more =3Dfe.\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.getSender() == "poco@appinf.com");
	assertTrue (message.getContentType() == "text/plain");
	assertTrue (message.getContent() ==
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
		"To test the quoted-printable encoding, we'll put an extra long line here. This should be enough.\r\n"
		"And here is some more =fe.\r\n"
	);
}


void MailMessageTest::testReadDefaultTransferEncoding()
{
	std::istringstream istr(
		"Content-Type: text/plain\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.getSender() == "poco@appinf.com");
	assertTrue (message.getContentType() == "text/plain");
	assertTrue (message.getContent() ==
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);
}


void MailMessageTest::testRead8Bit()
{
	std::istringstream istr(
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.getSender() == "poco@appinf.com");
	assertTrue (message.getContentType() == "text/plain");
	assertTrue (message.getContent() ==
		"Hello, world!\r\n"
		"This is a test for the MailMessage class.\r\n"
	);
}


void MailMessageTest::testReadMultiPart()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	StringPartHandler handler;
	MailMessage message;
	message.read(istr, handler);

	assertTrue (handler.data().size() == 2);
	assertTrue (handler.data()[0] == "Hello World!\r\n");
	assertTrue (handler.type()[0] == "text/plain");
	assertTrue (handler.disp()[0] == "inline");

	assertTrue (handler.data()[1] == "This is some binary data. Really.");
	assertTrue (handler.type()[1] == "application/octet-stream; name=sample");
	assertTrue (handler.disp()[1] == "attachment; filename=sample.dat");
}


void MailMessageTest::testReadMultiPartWithAttachmentNames()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.parts().size() == 2);
	assertTrue (message.parts()[1].name == "sample");
	assertTrue (message.parts()[1].pSource->filename() == "sample.dat");
}


void MailMessageTest::testReadMultiPartPartParameters()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: Inline; FileName=\"note.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"my file; v1.dat\"\r\n"
		"Content-Type: application/octet-stream; NAME=\"my file.dat\"\r\n"
		"\r\n"
		"data\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: ATTACHMENT\r\n"
		"Content-Type: application/pdf; name=report.pdf\r\n"
		"\r\n"
		"data\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=data.bin\r\n"
		"Content-Type: application/octet-stream\r\n"
		"\r\n"
		"data\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.parts().size() == 4);
	assertTrue (message.parts()[0].disposition == MailMessage::CONTENT_INLINE);
	assertTrue (message.parts()[0].pSource->filename() == "note.txt");

	assertTrue (message.parts()[1].disposition == MailMessage::CONTENT_ATTACHMENT);
	assertTrue (message.parts()[1].name == "my file.dat");
	assertTrue (message.parts()[1].pSource->filename() == "my file; v1.dat");

	assertTrue (message.parts()[2].disposition == MailMessage::CONTENT_ATTACHMENT);
	assertTrue (message.parts()[2].name == "report.pdf");
	assertTrue (message.parts()[2].pSource->filename() == "report.pdf");

	assertTrue (message.parts()[3].disposition == MailMessage::CONTENT_ATTACHMENT);
	assertTrue (message.parts()[3].name == "data.bin");
	assertTrue (message.parts()[3].pSource->filename() == "data.bin");
}


void MailMessageTest::testReadMultiPartDefaultContentType()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	MailMessage message;
	message.read(istr);

	assertTrue (message.parts().size() == 1);
	assertTrue (message.parts()[0].disposition == MailMessage::CONTENT_INLINE);
	assertTrue (message.parts()[0].pSource->mediaType() == "text/plain; charset=us-ascii");
}


void MailMessageTest::testReadRecipients()
{
	const std::string msgin(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: \"Doe, Jane\" <jane.doe@no.where>, walter.foo@no.where (Walter Foo),\r\n"
		"\tJohn  \"Q.\" Public <john.public@no.where>\r\n"
		"CC: Team: Homer Simpson <homer@no.where>, bart@no.where;, Lisa Simpson, nobody,\r\n"
		"\t<>, <unterminated@no.where, Ned <ned@no.where> trailing text\r\n"
		"BCC: undisclosed-recipients:;, maggie@no.where\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);

	MailMessage message;
	std::istringstream istr(msgin);
	message.read(istr);

	const MailMessage::Recipients& recipients = message.recipients();
	assertEqual (6, recipients.size());
	assertTrue (recipients[0].getType() == MailRecipient::PRIMARY_RECIPIENT);
	assertEqual ("jane.doe@no.where", recipients[0].getAddress());
	assertEqual ("Doe, Jane", recipients[0].getRealName());
	assertTrue (recipients[1].getType() == MailRecipient::PRIMARY_RECIPIENT);
	assertEqual ("walter.foo@no.where", recipients[1].getAddress());
	assertTrue (recipients[1].getRealName().empty());
	assertTrue (recipients[2].getType() == MailRecipient::PRIMARY_RECIPIENT);
	assertEqual ("john.public@no.where", recipients[2].getAddress());
	assertEqual ("John Q. Public", recipients[2].getRealName());
	assertTrue (recipients[3].getType() == MailRecipient::CC_RECIPIENT);
	assertEqual ("homer@no.where", recipients[3].getAddress());
	assertEqual ("Homer Simpson", recipients[3].getRealName());
	assertTrue (recipients[4].getType() == MailRecipient::CC_RECIPIENT);
	assertEqual ("bart@no.where", recipients[4].getAddress());
	assertTrue (recipients[4].getRealName().empty());
	assertTrue (recipients[5].getType() == MailRecipient::BCC_RECIPIENT);
	assertEqual ("maggie@no.where", recipients[5].getAddress());
	assertEqual ("Hello, world!\r\n", message.getContent());

	std::istringstream istrAgain(msgin);
	message.read(istrAgain);
	assertEqual (6, message.recipients().size());
}


void MailMessageTest::testReadRecipientsEncodedWords()
{
	std::istringstream istr(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"To: =?UTF-8?Q?evil@x.example=2C?= <jane@no.where>\r\n"
		"CC: =?UTF-8?q?Doe=2C_J=C3=B6rg?= <j@no.where>\r\n"
		"BCC: a@no.where,\r\n"
		"\t=?UTF-8?q?B=2C_C?= <b@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	MailMessage message;
	message.read(istr);

	const MailMessage::Recipients& recipients = message.recipients();
	assertEqual (4, recipients.size());
	assertTrue (recipients[0].getType() == MailRecipient::PRIMARY_RECIPIENT);
	assertEqual ("jane@no.where", recipients[0].getAddress());
	assertEqual ("evil@x.example,", recipients[0].getRealName());
	assertTrue (recipients[1].getType() == MailRecipient::CC_RECIPIENT);
	assertEqual ("j@no.where", recipients[1].getAddress());
	assertEqual ("Doe, J\xC3\xB6rg", recipients[1].getRealName());
	assertTrue (recipients[2].getType() == MailRecipient::BCC_RECIPIENT);
	assertEqual ("a@no.where", recipients[2].getAddress());
	assertTrue (recipients[3].getType() == MailRecipient::BCC_RECIPIENT);
	assertEqual ("b@no.where", recipients[3].getAddress());
	assertEqual ("B, C", recipients[3].getRealName());

	assertEqual ("\"evil@x.example,\" <jane@no.where>", message.get(MailMessage::HEADER_TO));
	assertEqual ("\"evil@x.example,\" <jane@no.where>", message.getDecoded(MailMessage::HEADER_TO));
	assertEqual ("Hello, world!\r\n", message.getContent());

	std::istringstream istrUndecoded(
		"To: =?UTF-8?Q?evil@x.example=2C?= <jane@no.where>\n"
		"\n"
		"Hello, world!\n"
	);
	MailMessage messageUndecoded;
	messageUndecoded.setAutoDecode(false);
	messageUndecoded.read(istrUndecoded);
	assertEqual (1, messageUndecoded.recipients().size());
	assertEqual ("=?UTF-8?Q?evil@x.example=2C?=", messageUndecoded.recipients()[0].getRealName());
	assertEqual ("=?UTF-8?Q?evil@x.example=2C?= <jane@no.where>", messageUndecoded.get(MailMessage::HEADER_TO));
	assertEqual ("\"evil@x.example,\" <jane@no.where>", messageUndecoded.getDecoded(MailMessage::HEADER_TO));
	assertEqual ("Hello, world!\n", messageUndecoded.getContent());
}


void MailMessageTest::testReadWriteRecipientFieldsUnchanged()
{
	const std::string recipientFields[] = {
		"To: jane.doe@no.where\r\n",
		"To: \"Jane Doe\" <jane.doe@no.where>, walter.foo@no.where\r\n",
		"To: walter.foo@no.where (Walter Foo)\r\n",
		"To: Team: a@no.where, b@no.where;\r\n",
		"To: Lisa Simpson, bart@no.where\r\nCC: Homer <homer@no.where>, <broken\r\n"
	};
	for (const auto& fields: recipientFields)
	{
		const std::string msgin(
			"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
			"From: poco@appinf.com\r\n"
			"Subject: Test Message\r\n" + fields +
			"\r\n"
			"Hello, world!\r\n"
		);
		std::istringstream istr(msgin);
		MailMessage message;
		message.read(istr);
		assertTrue (!message.recipients().empty());

		std::ostringstream ostr;
		message.write(ostr);
		assertEqual (msgin, ostr.str());
	}
}


void MailMessageTest::testWriteOmitsBccFields()
{
	std::istringstream istr(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: jane.doe@no.where\r\n"
		"Bcc: walter.foo@no.where\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	MailMessage message;
	message.read(istr);
	assertEqual (2, message.recipients().size());
	assertTrue (message.recipients()[1].getType() == MailRecipient::BCC_RECIPIENT);
	assertEqual ("walter.foo@no.where", message.recipients()[1].getAddress());

	std::ostringstream ostr;
	message.write(ostr);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: jane.doe@no.where\r\n"
		"\r\n"
		"Hello, world!\r\n",
		ostr.str());
	assertEqual (2, message.recipients().size());

	MailMessage builtMessage;
	builtMessage.setDate(Timestamp(0));
	builtMessage.setSender("poco@appinf.com");
	builtMessage.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where"));
	builtMessage.set(MailMessage::HEADER_BCC, "walter.foo@no.where");
	builtMessage.setContent("Hello, world!\r\n", MailMessage::ENCODING_8BIT);

	std::ostringstream builtStr;
	builtMessage.write(builtStr);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"From: poco@appinf.com\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"To: <jane.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n",
		builtStr.str());

	builtMessage.set(MailMessage::HEADER_BCC, "walter.foo@no.where\r\nX-Injected: 1");
	builtMessage.validateHeaders();
}


void MailMessageTest::testReadHeaderLineTooLong()
{
	const std::string longLine = "To: " + std::string(20000, 'a') + "@no.where\r\n";
	std::istringstream istr(longLine + "\r\nHello, world!\r\n");
	MailMessage message;
	try
	{
		message.read(istr);
		fail("must throw");
	}
	catch (MessageException&)
	{
	}
	assertTrue (istr.tellg() < static_cast<std::streamoff>(longLine.size()));
}


void MailMessageTest::testReadAddRecipientEscapesRealName()
{
	const std::pair<std::string, std::string> cases[] = {
		{"To: \"x\\\" <evil@a.com>, \\\"y\" <orig@b.com>\r\n", "x\" <evil@a.com>, \"y"},
		{"To: =?utf-8?q?x=22_=3Cevil=40a.com=3E=2C_=22y?= <orig@b.com>\r\n", "x\" <evil@a.com>, \"y"},
		{"To: =?utf-8?q?x=22_=3Cevil=40a.com=3E,_=22y?= <orig@b.com>\r\n", "_=22y?="}
	};
	for (const auto& [field, realName]: cases)
	{
		std::istringstream istr(
			"From: poco@appinf.com\r\n" + field +
			"\r\n"
			"Hello, world!\r\n"
		);
		MailMessage message;
		message.read(istr);
		assertEqual (1, message.recipients().size());
		assertEqual (realName, message.recipients()[0].getRealName());
		message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe"));

		std::ostringstream ostr;
		message.write(ostr);
		std::istringstream istrWritten(ostr.str());
		MailMessage messageWritten;
		messageWritten.read(istrWritten);

		const MailMessage::Recipients& recipients = messageWritten.recipients();
		assertEqual (2, recipients.size());
		assertEqual ("orig@b.com", recipients[0].getAddress());
		assertEqual (realName, recipients[0].getRealName());
		assertEqual ("john.doe@no.where", recipients[1].getAddress());
		assertEqual ("John Doe", recipients[1].getRealName());
	}
}


void MailMessageTest::testReadAddRecipientRewritesFields()
{
	std::istringstream istr(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: jane.doe@no.where (Jane)\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	MailMessage message;
	message.read(istr);
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe"));

	std::ostringstream ostr;
	message.write(ostr);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: <jane.doe@no.where>, John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n",
		ostr.str());

	std::istringstream istrTwoFields(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: jane.doe@no.where\r\n"
		"To: w@no.where\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	MailMessage messageTwoFields;
	messageTwoFields.read(istrTwoFields);
	messageTwoFields.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "john.doe@no.where", "John Doe"));

	std::ostringstream ostrTwoFields;
	messageTwoFields.write(ostrTwoFields);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Subject: Test Message\r\n"
		"To: <jane.doe@no.where>, <w@no.where>, John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n",
		ostrTwoFields.str());
}


void MailMessageTest::testReadMultiPartDefaultTransferEncoding()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	StringPartHandler handler;
	MailMessage message;
	message.read(istr, handler);

	assertTrue (handler.data().size() == 2);
	assertTrue (handler.data()[0] == "Hello World!\r\n");
	assertTrue (handler.type()[0] == "text/plain");
	assertTrue (handler.disp()[0] == "inline");

	assertTrue (handler.data()[1] == "This is some binary data. Really.");
	assertTrue (handler.type()[1] == "application/octet-stream; name=sample");
	assertTrue (handler.disp()[1] == "attachment; filename=sample.dat");
}


void MailMessageTest::testReadMultiPartMixedCaseHeaders()
{
	std::istringstream istr(
		"Content-type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"content-disposition: inline\r\n"
		"Content-type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-DISposition: attachment; filename=sample.dat\r\n"
		"content-Transfer-encodING: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	MailMessage message;
	MailInputStream mis(istr);
	message.read(mis);

	assertTrue (message.isMultipart());
	assertTrue (message.parts().size() == 2);
	assertTrue (message.get(MailMessage::HEADER_CONTENT_TYPE) == "multipart/mixed; boundary=MIME_boundary_01234567");

	assertTrue (message.parts()[0].encoding == MailMessage::ContentTransferEncoding::ENCODING_7BIT);
	assertTrue (message.parts()[0].disposition == MailMessage::ContentDisposition::CONTENT_INLINE);
	assertTrue (message.parts()[0].pSource->headers().get(MailMessage::HEADER_CONTENT_TYPE) == "text/plain");

	assertTrue (message.parts()[1].encoding == MailMessage::ContentTransferEncoding::ENCODING_BASE64);
	assertTrue (message.parts()[1].disposition == MailMessage::ContentDisposition::CONTENT_ATTACHMENT);
	assertTrue (message.parts()[1].pSource->filename() == "sample.dat");
	assertTrue (message.parts()[1].pSource->headers().get(MailMessage::HEADER_CONTENT_TYPE) == "application/octet-stream; name=sample");
}


void MailMessageTest::testReadMultiPartInvalidContentDisposition()
{
		std::istringstream istr(
				"Content-type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
				"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
				"From: poco@appinf.com\r\n"
				"Mime-Version: 1.0\r\n"
				"Subject: Test Message\r\n"
				"To: John Doe <john.doe@no.where>\r\n"
				"\r\n"
				"\r\n"
				"Hello World!\r\n"
				"\r\n"
				"--MIME_boundary_01234567\r\n"
				"Content-Disposition: \r\n"
				"Content-Transfer-Encoding: base64\r\n"
				"Content-Type: application/octet-stream; name=sample\r\n"
				"\r\n"
				"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
				"--MIME_boundary_01234567--\r\n"
		);

		MailMessage message;
		MailInputStream mis(istr);
		message.read(mis);

		assertTrue (message.isMultipart());
		assertTrue (message.parts().size() == 1);
		assertTrue (message.get(MailMessage::HEADER_CONTENT_TYPE) == "multipart/mixed; boundary=MIME_boundary_01234567");

		assertTrue (message.parts()[0].encoding == MailMessage::ContentTransferEncoding::ENCODING_BASE64);
		assertTrue (message.parts()[0].disposition == MailMessage::ContentDisposition::CONTENT_INLINE);
		assertTrue (message.parts()[0].pSource->headers().get(MailMessage::HEADER_CONTENT_TYPE) == "application/octet-stream; name=sample");
}


void MailMessageTest::testReadMultiPartNoFinalBoundaryFromFile()
{
	std::string data(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
	);

	// The problem occurs during reading message from file.
	// (For stringstreams it works fine.)
	// https://github.com/pocoproject/poco/issues/2401
	TemporaryFile file;
	std::ofstream(file.path()) << data;
	std::ifstream istr(file.path());

	StringPartHandler handler;
	MailMessage message;
	message.read(istr, handler);

	assertTrue (handler.data().size() == 2);
	assertTrue (handler.data()[0] == "Hello World!\r\n");
	assertTrue (handler.type()[0] == "text/plain");
	assertTrue (handler.disp()[0] == "inline");

	assertTrue (handler.data()[1] == "This is some binary data. Really.");
	assertTrue (handler.type()[1] == "application/octet-stream; name=sample");
	assertTrue (handler.disp()[1] == "attachment; filename=sample.dat");
}


void MailMessageTest::testReadWriteMultiPart()
{
	std::string msgin(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_31E8A8D61DF53389\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"--MIME_boundary_31E8A8D61DF53389\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_31E8A8D61DF53389\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-ID: abcd1234\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_31E8A8D61DF53389--\r\n"
	);

	std::istringstream istr(msgin);
	std::ostringstream ostr;
	MailMessage message;

	message.read(istr);
	message.write(ostr);

	std::string msgout(ostr.str());
	assertTrue (msgout == msgin);
}


void MailMessageTest::testReadWriteMultiPartStore()
{
	std::string msgin(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_31E8A8D61DF53389\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Test Message\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"--MIME_boundary_31E8A8D61DF53389\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_31E8A8D61DF53389\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-ID: abcd1234\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_31E8A8D61DF53389--\r\n"
	);

	std::istringstream istr(msgin);
	std::ostringstream ostr;
	FilePartStoreFactory pfsf;
	MailMessage message(&pfsf);

	message.read(istr);

	MailMessage::PartVec::const_iterator it = message.parts().begin();
	MailMessage::PartVec::const_iterator end = message.parts().end();
	for (; it != end; ++it)
	{
		FilePartStore* fps = dynamic_cast<FilePartStore*>(it->pSource);
		if (fps && fps->filename().size())
		{
			std::string filename = fps->filename();
			assertTrue (filename == "sample.dat");
			std::string path = fps->path();
			// for security reasons, the filesystem temporary
			// filename is not the same as attachment name
			std::size_t sz = std::min(path.size(), filename.size());
			assertTrue (0 != icompare(path, path.size() - sz, sz, path));

			Poco::FileInputStream fis(path);
			assertTrue (fis.good());
			std::string read;
			std::string line;
			while (std::getline(fis, line)) read += line;

			assertTrue (!read.empty());
			assertTrue (read == "This is some binary data. Really.");
		}
	}

	message.write(ostr);
	std::string msgout(ostr.str());
	assertTrue (msgout == msgin);
}


void MailMessageTest::testEncodeWord()
{
	std::string plain("this is pure ASCII");
	std::string encoded = MailMessage::encodeWord(plain, "ISO-8859-1");
	assertTrue (encoded == plain);

	plain = "This text contains German Umlauts: \304\326";
	encoded = MailMessage::encodeWord(plain, "ISO-8859-1");
	assertTrue (encoded == "=?ISO-8859-1?q?This_text_contains_German_Umlauts=3A_=C4=D6?=");

	plain = "This text contains German Umlauts: \304\326. "
			"It is also a very long text. Longer than 75 "
			"characters. Long enough to become three lines "
			"after being word-encoded.";
	encoded = MailMessage::encodeWord(plain, "ISO-8859-1");
	assertTrue (encoded == "=?ISO-8859-1?q?This_text_contains_German_Umlauts=3A_=C4=D6=2E_It_?=\r\n"
					   " =?ISO-8859-1?q?is_also_a_very_long_text=2E_Longer_than_75_characters=2E_?=\r\n"
					   " =?ISO-8859-1?q?Long_enough_to_become_three_lines_after_being_word-encode?=\r\n"
					   " =?ISO-8859-1?q?d=2E?=");
}


void MailMessageTest::testReadMultiPartEmptyBoundary()
{
	// Empty boundary should throw MultipartException (#5288)
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"\r\n"
		"\r\n"
		"--\r\n"
		"Content-Disposition: inline\r\n"
		"\r\n"
		"Hello\r\n"
		"----\r\n"
	);

	MailMessage message;
	try
	{
		message.read(istr);
		fail("must throw on empty boundary");
	}
	catch (const Poco::Net::MultipartException&)
	{
	}
}


void MailMessageTest::testReadMultiPartWithContentLength()
{
	// A correct Content-Length in the part headers does not change the parts.
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"Subject: Content-Length Test\r\n"
		"To: John Doe <john.doe@no.where>\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 14\r\n"
		"\r\n"
		"Hello World!\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=sample.dat\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Type: application/octet-stream; name=sample\r\n"
		"Content-Length: 44\r\n"
		"\r\n"
		"VGhpcyBpcyBzb21lIGJpbmFyeSBkYXRhLiBSZWFsbHku\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	StringPartHandler handler;
	MailMessage message;
	message.read(istr, handler);

	assertTrue (handler.data().size() == 2);
	assertTrue (handler.data()[0] == "Hello World!\r\n");
	assertTrue (handler.type()[0] == "text/plain");
	assertTrue (handler.disp()[0] == "inline");

	assertTrue (handler.data()[1] == "This is some binary data. Really.");
	assertTrue (handler.type()[1] == "application/octet-stream; name=sample");
	assertTrue (handler.disp()[1] == "attachment; filename=sample.dat");
}


void MailMessageTest::testReadMultiPartWithZeroContentLength()
{
	// Content-Length: 0 for an empty part body
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: poco@appinf.com\r\n"
		"Mime-Version: 1.0\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 0\r\n"
		"\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: inline\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 6\r\n"
		"\r\n"
		"Hello!\r\n"
		"--MIME_boundary_01234567--\r\n"
	);

	StringPartHandler handler;
	MailMessage message;
	message.read(istr, handler);

	assertTrue (handler.data().size() == 2);
	assertTrue (handler.data()[0].empty());
	assertTrue (handler.data()[1] == "Hello!");
}


namespace
{
	std::string buildMultiPartMessage(int partCount, int partBodySize = 0)
	{
		const std::string boundary = "MIME_boundary_parts_test";
		std::string body(partBodySize > 0 ? partBodySize : 0, 'X');
		std::string msg;
		msg.reserve(partCount * (120 + partBodySize) + 200);
		msg += "Content-Type: multipart/mixed; boundary=" + boundary + "\r\n";
		msg += "Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n";
		msg += "From: poco@appinf.com\r\n";
		msg += "Mime-Version: 1.0\r\n";
		msg += "\r\n";
		for (int i = 0; i < partCount; ++i)
		{
			msg += "\r\n--" + boundary + "\r\n";
			msg += "Content-Disposition: inline\r\n";
			msg += "Content-Transfer-Encoding: 8bit\r\n";
			msg += "Content-Type: text/plain\r\n";
			std::string partContent = partBodySize > 0 ? body : "Part " + std::to_string(i);
			partContent += "\r\n";
			msg += "\r\n";
			msg += partContent;
		}
		msg += "\r\n--" + boundary + "--\r\n";
		return msg;
	}
}


void MailMessageTest::testReadMultiPartManyParts()
{
	// 60000 parts — well within the 100000 limit
	std::string raw = buildMultiPartMessage(60000);
	std::istringstream istr(raw);

	StringPartHandler handler;
	MailMessage message;

	Poco::Stopwatch sw;
	sw.start();
	message.read(istr, handler);
	sw.stop();
	std::cout << "60000 parts (" << raw.size() << " bytes): " << sw.elapsed() / 1000 << " ms" << std::endl;

	assertTrue (handler.data().size() == 60000);
	assertTrue (handler.data()[0] == "Part 0\r\n");
	assertTrue (handler.data()[59999] == "Part 59999\r\n");
}


void MailMessageTest::testReadMultiPartTooManyParts()
{
	const int count = MailMessage::MAX_PARTS + 1;
	std::string raw = buildMultiPartMessage(count);
	std::istringstream istr(raw);

	MailMessage message;

	Poco::Stopwatch sw;
	sw.start();
	try
	{
		message.read(istr);
		fail("must throw on too many parts");
	}
	catch (const Poco::Net::MultipartException&)
	{
	}
	sw.stop();
	std::cout << count << " parts (" << raw.size() << " bytes), rejected at "
	          << MailMessage::MAX_PARTS << ": " << sw.elapsed() / 1000 << " ms" << std::endl;
}


void MailMessageTest::testReadMultiPartLarge()
{
	// 200 parts with 500KB body each
	std::string raw = buildMultiPartMessage(200, 500000);
	std::istringstream istr(raw);

	StringPartHandler handler;
	MailMessage message;

	Poco::Stopwatch sw;
	sw.start();
	message.read(istr, handler);
	sw.stop();
	std::cout << "200 x 500KB parts (" << raw.size() / 1024 << " KB): "
	          << sw.elapsed() / 1000 << " ms" << std::endl;

	assertTrue (handler.data().size() == 200);
	assertTrue (handler.data()[0].size() == 500002); // 500000 'X' + "\r\n"
}


void MailMessageTest::testWriteFoldedField()
{
	MailMessage message;
	message.set("X-Field", "a\r\n b");
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nX-Field: a\r\n b\r\n") != std::string::npos);
}


void MailMessageTest::testValidateHeaders()
{
	MailMessage valid;
	valid.setSubject("Test Message");
	valid.addAttachment("a.txt", new StringPartSource("a", "text/plain", "a.txt"));
	valid.validateHeaders();

	MailMessage invalidSubject;
	invalidSubject.setSubject("Test\nBcc: x");
	MailMessage invalidFilename;
	invalidFilename.addAttachment("a.txt", new StringPartSource("a", "text/plain", "a\r\nBcc: x"));
	MailMessage invalidPartField;
	StringPartSource* pSource = new StringPartSource("a", "text/plain");
	pSource->headers().set("X-Field", "a\r\nBcc: x");
	invalidPartField.addContent(pSource);
	for (const MailMessage* pMessage: {&invalidSubject, &invalidFilename, &invalidPartField})
	{
		try
		{
			pMessage->validateHeaders();
			fail("invalid header field must throw");
		}
		catch (MessageException&)
		{
		}
	}
}


void MailMessageTest::testReadMultiPartEncodedFileName()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Mime-Version: 1.0\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?B?w6TDtsO8LnR4dA==?=\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"a\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?Q?a=0Ab.txt?=\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"b\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"a=?b.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"c\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?Q?a?= b.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"d\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?Q?a?= =?UTF-8?Q?b.txt?=\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"e\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?B?PT9VVEYtOD9RP2E9M0Q/PQ==?=\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"f\r\n"
		"--MIME_boundary_01234567--\r\n"
	);
	MailMessage message;
	message.read(istr);

	assertEqual (6, message.parts().size());
	assertEqual ("\xC3\xA4\xC3\xB6\xC3\xBC.txt", message.parts()[0].name);
	assertEqual ("\xC3\xA4\xC3\xB6\xC3\xBC.txt", message.parts()[0].pSource->filename());
	assertEqual ("=?UTF-8?Q?a=0Ab.txt?=", message.parts()[1].name);
	assertEqual ("=?UTF-8?Q?a=0Ab.txt?=", message.parts()[1].pSource->filename());
	assertEqual ("a=?b.txt", message.parts()[2].pSource->filename());
	assertEqual ("a b.txt", message.parts()[3].pSource->filename());
	assertEqual ("ab.txt", message.parts()[4].pSource->filename());
	assertEqual ("=?UTF-8?Q?a=3D?=", message.parts()[5].name);
	assertEqual ("=?UTF-8?Q?a=3D?=", message.parts()[5].pSource->filename());
	assertEqual ("attachment; filename=\"=?UTF-8?B?w6TDtsO8LnR4dA==?=\"", message.parts()[0].pSource->headers().get("Content-Disposition"));
}


void MailMessageTest::testWriteRejectsControlCharacterInEncodedText()
{
	MailMessage withRecipient;
	withRecipient.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "j@no.where", "J\xC3\xB6rg\r\nBcc: evil@x.example"));
	MailMessage withSender;
	withSender.setSender("J\xC3\xB6rg\r\nBcc: evil@x.example <j@no.where>");
	MailMessage withSubject;
	withSubject.setSubject("\xC3\xA4\r\nBcc: evil@x.example");
	MailMessage withGroup;
	withGroup.set("Reply-To", "T\xC3\xA4m\r\nBcc: evil@x.example: a@no.where;");
	for (const MailMessage* pMessage: {&withRecipient, &withSender, &withSubject, &withGroup})
	{
		std::ostringstream ostr;
		try
		{
			pMessage->write(ostr);
			fail("a control character must be rejected before the value is encoded");
		}
		catch (MessageException&)
		{
		}
		assertTrue (ostr.str().empty());
		try
		{
			pMessage->validateHeaders();
			fail("a control character must be rejected before the value is encoded");
		}
		catch (MessageException&)
		{
		}
	}
}


void MailMessageTest::testWriteEncodesGroupName()
{
	MailMessage message;
	message.setDate(Timestamp(0));
	message.set("Reply-To", "T\xC3\xA4m: a@no.where;");
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nReply-To: =?UTF-8?q?T=C3=A4m?=: a@no.where;\r\n") != std::string::npos);

	MailMessage invalid;
	invalid.set("Reply-To", "J\xC3\xB6rg <unterminated");
	std::ostringstream invalidStr;
	try
	{
		invalid.write(invalidStr);
		fail("a mailbox that cannot be parsed must not be written with non-ASCII text");
	}
	catch (MessageException&)
	{
	}
	assertTrue (invalidStr.str().empty());
	try
	{
		invalid.validateHeaders();
		fail("a mailbox that cannot be parsed must not be written with non-ASCII text");
	}
	catch (MessageException&)
	{
	}
}


void MailMessageTest::testReadWriteEncodedFieldsUnchanged()
{
	const std::string msgin(
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"From: =?UTF-8?q?Ferenc_B=C5=91di?= <f@no.where>\r\n"
		"Subject: =?UTF-8?B?R3LDvMOfZQ==?=\r\n"
		"To: =?UTF-8?q?Doe=2C_J=C3=B6rg?= <j@no.where>\r\n"
		"X-Raw: Gr\xC3\xBC\xC3\x9F" "e\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	std::istringstream istr(msgin);
	MailMessage message;
	message.read(istr);
	assertEqual ("Gr\xC3\xBC\xC3\x9F" "e", message.getSubject());
	assertEqual ("Ferenc B\xC5\x91" "di <f@no.where>", message.getSender());
	assertEqual (1, message.recipients().size());
	assertEqual ("Doe, J\xC3\xB6rg", message.recipients()[0].getRealName());

	std::ostringstream ostr;
	message.write(ostr);
	assertEqual (msgin, ostr.str());
}


void MailMessageTest::testWriteEncodesNonAsciiText()
{
	const std::string sender("Ferenc B\xC5\x91" "di Kov\xC3\xA1" "cs <ferenc@no.where>");
	const std::string subject("Gr\xC3\xBC\xC3\x9F" "e aus Ljubljana und Wien");
	const std::string realName("Ren\xC3\xA9 Doe-Smith");
	MailMessage message;
	message.setDate(Timestamp(0));
	message.setSender(sender);
	message.setSubject(subject);
	message.set("X-Note", "=?UTF-8?Q?a?=");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "rene@no.where", realName));
	message.setContent("Hello, world!\r\n", MailMessage::ENCODING_8BIT);

	std::ostringstream ostr;
	message.write(ostr);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"From: =?UTF-8?q?Ferenc_B=C5=91di_Kov=C3=A1cs?= <ferenc@no.where>\r\n"
		"Subject: =?UTF-8?q?Gr=C3=BC=C3=9Fe_aus_Ljubljana_und_Wien?=\r\n"
		"X-Note: =?UTF-8?Q?a?=\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"To: =?UTF-8?q?Ren=C3=A9_Doe-Smith?= <rene@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n",
		ostr.str());
	assertEqual (subject, message.getSubject());

	std::istringstream istr(ostr.str());
	MailMessage messageRead;
	messageRead.read(istr);
	assertEqual (subject, messageRead.getSubject());
	assertEqual (sender, messageRead.getSender());
	assertEqual (1, messageRead.recipients().size());
	assertEqual (realName, messageRead.recipients()[0].getRealName());

	message.setSubject("\xC3\xA4\r\nBcc: x");
	std::ostringstream invalidStr;
	try
	{
		message.write(invalidStr);
		fail("a control character must be rejected before the value is encoded");
	}
	catch (MessageException&)
	{
	}
	assertTrue (invalidStr.str().empty());
}


void MailMessageTest::testEncodeWordB()
{
	assertEqual ("this is pure ASCII", MailMessage::encodeWord("this is pure ASCII", "UTF-8", MailMessage::WordEncoding::B));
	assertEqual ("=?UTF-8?b?R3LDvMOfZQ==?=", MailMessage::encodeWord("Gr\xC3\xBC\xC3\x9F" "e", "UTF-8", MailMessage::WordEncoding::B));
	assertEqual ("=?ISO-8859-1?b?xNY=?=", MailMessage::encodeWord("\304\326", "ISO-8859-1", MailMessage::WordEncoding::B));
}


void MailMessageTest::testEncodeWordAuto()
{
	assertEqual ("this is pure ASCII", MailMessage::encodeWord("this is pure ASCII", "UTF-8", MailMessage::WordEncoding::AUTO));
	assertEqual ("=?UTF-8?q?Gr=C3=BC=C3=9Fe?=", MailMessage::encodeWord("Gr\xC3\xBC\xC3\x9F" "e"));
	assertEqual ("=?UTF-8?b?R3LDvMOfZQ==?=", MailMessage::encodeWord("Gr\xC3\xBC\xC3\x9F" "e", "UTF-8", MailMessage::WordEncoding::AUTO));
	assertEqual ("=?UTF-8?q?Gr=C3=BC=C3=9Fe_aus_Ljubljana_und_Wien?=",
		MailMessage::encodeWord("Gr\xC3\xBC\xC3\x9F" "e aus Ljubljana und Wien", "UTF-8", MailMessage::WordEncoding::AUTO));
	assertEqual ("=?UTF-8?q?Ferenc_B=C5=91di_Kov=C3=A1cs?=",
		MailMessage::encodeWord("Ferenc B\xC5\x91" "di Kov\xC3\xA1" "cs", "UTF-8", MailMessage::WordEncoding::AUTO));
}


void MailMessageTest::testEncodeWordLength()
{
	assertEqual ("=?UTF-8?q?" + std::string(61, 'a') + "?=\r\n =?UTF-8?q?=C3=A4?=", MailMessage::encodeWord(std::string(61, 'a') + "\xC3\xA4"));

	std::string text;
	for (int i = 0; i < 40; ++i) text += "\xE2\x82\xAC";
	for (const auto encoding: {MailMessage::WordEncoding::Q, MailMessage::WordEncoding::B})
	{
		const std::string encoded = MailMessage::encodeWord(text, "UTF-8", encoding);
		assertEqual (text, MessageHeader::decodeWord(encoded));
		std::string decodedWords;
		std::string::size_type start = 0;
		while (start < encoded.size())
		{
			const std::string::size_type end = std::min(encoded.find("\r\n ", start), encoded.size());
			const std::string word = encoded.substr(start, end - start);
			assertTrue (word.size() <= 75);
			std::string decoded;
			MessageHeader::decodeRFC2047(word, decoded);
			assertTrue (!decoded.empty() && decoded.size() % 3 == 0);
			assertEqual ("\xE2", decoded.substr(0, 1));
			decodedWords += decoded;
			start = end + 3;
		}
		assertEqual (text, decodedWords);
	}
}


void MailMessageTest::testWriteEncodesShorterWord()
{
	MailMessage message;
	message.setSubject("Gr\xC3\xBC\xC3\x9F" "e");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "j@no.where", "J\xC3\xB6rg Doe"));
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nSubject: =?UTF-8?b?R3LDvMOfZQ==?=\r\n") != std::string::npos);
	assertTrue (ostr.str().find("\r\nTo: =?UTF-8?b?SsO2cmcgRG9l?= <j@no.where>\r\n") != std::string::npos);
}


void MailMessageTest::testMove()
{
	MailMessage message;
	message.setSubject("Test Message");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where"));
	message.addContent(new StringPartSource("Hello, world!\r\n", "text/plain"));
	message.addAttachment("a.txt", new StringPartSource("a", "text/plain", "a.txt"));
	const MailMessage::PartVec parts = message.parts();

	MailMessage moved(std::move(message));
	assertTrue (message.empty());
	assertTrue (message.recipients().empty());
	assertTrue (message.parts().empty());
	assertEqual ("Test Message", moved.getSubject());
	assertEqual (1, moved.recipients().size());
	assertEqual (2, moved.parts().size());
	assertTrue (moved.parts()[0].pSource == parts[0].pSource);
	assertTrue (moved.parts()[1].pSource == parts[1].pSource);

	MailMessage assigned;
	assigned.addAttachment("b.txt", new StringPartSource("b", "text/plain", "b.txt"));
	assigned = std::move(moved);
	assertTrue (moved.empty());
	assertTrue (moved.recipients().empty());
	assertTrue (moved.parts().empty());
	assertEqual ("Test Message", assigned.getSubject());
	assertEqual (1, assigned.recipients().size());
	assertEqual (2, assigned.parts().size());
	assertTrue (assigned.parts()[0].pSource == parts[0].pSource);
	assertTrue (assigned.parts()[1].pSource == parts[1].pSource);
	std::ostringstream ostr;
	assigned.write(ostr);
	assertTrue (ostr.str().find("Hello, world!") != std::string::npos);

	FilePartStoreFactory factory;
	MailMessage messageWithStore(&factory);
	MailMessage movedWithStore(std::move(messageWithStore));
	std::unique_ptr<Poco::Net::PartSource> pStore(movedWithStore.createPartStore("a", "text/plain"));
	assertTrue (dynamic_cast<FilePartStore*>(pStore.get()) != nullptr);
	std::unique_ptr<Poco::Net::PartSource> pSource(messageWithStore.createPartStore("a", "text/plain"));
	assertTrue (dynamic_cast<FilePartStore*>(pSource.get()) == nullptr);
}


void MailMessageTest::testWriteKeepsNonUTF8Text()
{
	MailMessage message;
	message.setDate(Timestamp(0));
	message.setSender("J\xF6rg <j@no.where>");
	message.setSubject("Gr\xFC\xDF" "e");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "r@no.where", "Ren\xE9"));
	message.setContent("Hello, world!\r\n", MailMessage::ENCODING_8BIT);
	std::ostringstream ostr;
	message.write(ostr);
	assertEqual (
		"Date: Thu, 1 Jan 1970 00:00:00 GMT\r\n"
		"Content-Type: text/plain\r\n"
		"From: J\xF6rg <j@no.where>\r\n"
		"Subject: Gr\xFC\xDF" "e\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"To: \"Ren\xE9\" <r@no.where>\r\n"
		"\r\n"
		"Hello, world!\r\n",
		ostr.str());
}


void MailMessageTest::testReadRecipientsDomainLiteral()
{
	std::istringstream istr(
		"To: user@[IPv6:2001:db8::1], Name <n@[IPv6:2001:db8::2]>, a@[unterminated\r\n"
		"\r\n"
		"Hello, world!\r\n"
	);
	MailMessage message;
	message.read(istr);
	assertEqual (2, message.recipients().size());
	assertEqual ("user@[IPv6:2001:db8::1]", message.recipients()[0].getAddress());
	assertEqual ("n@[IPv6:2001:db8::2]", message.recipients()[1].getAddress());
	assertEqual ("Name", message.recipients()[1].getRealName());

	std::istringstream angleStr("To: Name <n@[a,b;c]>, x@y.example\r\n\r\n");
	MailMessage angle;
	angle.read(angleStr);
	assertEqual (2, angle.recipients().size());
	assertEqual ("n@[a,b;c]", angle.recipients()[0].getAddress());
	assertEqual ("Name", angle.recipients()[0].getRealName());
	assertEqual ("x@y.example", angle.recipients()[1].getAddress());

	std::istringstream addrSpecStr("To: n@[a,b;c], x@y.example\r\n\r\n");
	MailMessage addrSpec;
	addrSpec.read(addrSpecStr);
	assertEqual (2, addrSpec.recipients().size());
	assertEqual ("n@[a,b;c]", addrSpec.recipients()[0].getAddress());
	assertEqual ("x@y.example", addrSpec.recipients()[1].getAddress());
}


void MailMessageTest::testReadRecipientsMalformedDomainLiteral()
{
	for (const std::string to: {"<a@[x>, b@c.example", "a@[x, b@c.example", "a@[x[y], b@c.example"})
	{
		std::istringstream istr("To: " + to + "\r\n\r\n");
		MailMessage message;
		message.read(istr);
		assertEqual (1, message.recipients().size());
		assertEqual ("b@c.example", message.recipients()[0].getAddress());
	}
	for (const std::string prefix: {"", "=?UTF-8?Q?x?= "})
	{
		for (const std::string to: {"[", "<", "<a@[", "a@[", "[[", "<a@[x]"})
		{
			std::istringstream istr("To: " + prefix + to + "\r\n\r\n");
			MailMessage message;
			message.read(istr);
			assertTrue (message.recipients().empty());
		}
	}
}


void MailMessageTest::testWriteEncodesNameWithComment()
{
	MailMessage message;
	message.setSender("J\xC3\xB6rg (Sales) <j@x.example>");
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nFrom: =?UTF-8?b?SsO2cmc=?= (Sales) <j@x.example>\r\n") != std::string::npos);
}


void MailMessageTest::testWriteEncodesDomainLiteral()
{
	MailMessage message;
	message.setDate(Timestamp(0));
	message.setSender("J\xC3\xB6rg [a<b] <c@d.example>");
	message.set("Cc", "a@[x, J\xC3\xB6rg <b@c.example>");
	message.set("Reply-To", "<a@[x>, J\xC3\xB6rg <b@c.example>");
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nFrom: =?UTF-8?b?SsO2cmcgW2E8Yl0=?= <c@d.example>\r\n") != std::string::npos);
	assertTrue (ostr.str().find("\r\nCc: a@[x, =?UTF-8?b?SsO2cmc=?= <b@c.example>\r\n") != std::string::npos);
	assertTrue (ostr.str().find("\r\nReply-To: <a@[x>, =?UTF-8?b?SsO2cmc=?= <b@c.example>\r\n") != std::string::npos);
}


void MailMessageTest::testWriteEscapesPartParameters()
{
	std::istringstream istr(
		"Content-Type: multipart/mixed; boundary=MIME_boundary_01234567\r\n"
		"Mime-Version: 1.0\r\n"
		"\r\n"
		"--MIME_boundary_01234567\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?Q?a=22=3B_x=3D=22y?=\"\r\n"
		"Content-Type: application/octet-stream; name=\"=?UTF-8?Q?n=22=3B_boundary=3D=22Z?=\"\r\n"
		"\r\n"
		"a\r\n"
		"--MIME_boundary_01234567--\r\n"
	);
	MailMessage message;
	message.read(istr);
	assertEqual (1, message.parts().size());
	assertEqual ("n\"; boundary=\"Z", message.parts()[0].name);
	assertEqual ("a\"; x=\"y", message.parts()[0].pSource->filename());

	std::ostringstream ostr;
	message.write(ostr);
	const std::string written = ostr.str();
	assertTrue (written.find("\r\nContent-Type: application/octet-stream; name=\"n\\\"; boundary=\\\"Z\"\r\n") != std::string::npos);
	assertTrue (written.find("\r\nContent-Disposition: attachment; filename=\"a\\\"; x=\\\"y\"\r\n") != std::string::npos);

	std::istringstream istrWritten(written);
	MailMessage messageWritten;
	messageWritten.read(istrWritten);
	assertEqual (1, messageWritten.parts().size());
	assertEqual ("n\"; boundary=\"Z", messageWritten.parts()[0].name);
	assertEqual ("a\"; x=\"y", messageWritten.parts()[0].pSource->filename());
}


void MailMessageTest::testWriteRealNameQuotedPair()
{
	MailMessage message;
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "r@no.where", "a\"b\\c"));
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nTo: \"a\\\"b\\\\c\" <r@no.where>\r\n") != std::string::npos);
}


void MailMessageTest::testReadRecipientsQuotedLocalPart()
{
	const auto readRecipients = [](const std::string& to)
	{
		std::istringstream istr("To: " + to + "\r\n\r\n");
		MailMessage message;
		message.read(istr);
		return message.recipients();
	};

	const MailMessage::Recipients unbracketed = readRecipients("\"a,b\"@example.com, x@y.example");
	assertEqual (2, unbracketed.size());
	assertEqual ("\"a,b\"@example.com", unbracketed[0].getAddress());
	assertEqual ("x@y.example", unbracketed[1].getAddress());

	const MailMessage::Recipients atSign = readRecipients("\"a@evil.example\"@good.example");
	assertEqual (1, atSign.size());
	assertEqual ("\"a@evil.example\"@good.example", atSign[0].getAddress());

	// An '@' inside the quotes is part of the local part, not the start of a domain.
	const MailMessage::Recipients noDomain = readRecipients("\"a@b\", x@y.example");
	assertEqual (1, noDomain.size());
	assertEqual ("x@y.example", noDomain[0].getAddress());

	const MailMessage::Recipients quotedPair = readRecipients("\"a\\\"b\"@example.com");
	assertEqual (1, quotedPair.size());
	assertEqual ("\"a\\\"b\"@example.com", quotedPair[0].getAddress());

	const MailMessage::Recipients angle = readRecipients("Name <\"a,b\"@example.com>, x@y.example");
	assertEqual (2, angle.size());
	assertEqual ("\"a,b\"@example.com", angle[0].getAddress());
	assertEqual ("Name", angle[0].getRealName());
	assertEqual ("x@y.example", angle[1].getAddress());

	const MailMessage::Recipients displayName = readRecipients("\"Doe, J\" <j@x.example>");
	assertEqual (1, displayName.size());
	assertEqual ("j@x.example", displayName[0].getAddress());
	assertEqual ("Doe, J", displayName[0].getRealName());
}


void MailMessageTest::testWriteKeepsQuotedLocalPart()
{
	MailMessage message;
	message.setSender("\"a<b\"@c.example");
	message.set("Cc", "J\xC3\xB6rg <\"a<b\"@c.example>, \"x<y\"@d.example");
	std::ostringstream ostr;
	message.write(ostr);
	assertTrue (ostr.str().find("\r\nFrom: \"a<b\"@c.example\r\n") != std::string::npos);
	assertTrue (ostr.str().find("\r\nCc: =?UTF-8?b?SsO2cmc=?= <\"a<b\"@c.example>, \"x<y\"@d.example\r\n") != std::string::npos);
}


void MailMessageTest::setUp()
{
}


void MailMessageTest::tearDown()
{
}


CppUnit::Test* MailMessageTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MailMessageTest");

	CppUnit_addTest(pSuite, MailMessageTest, testWriteQP);
	CppUnit_addTest(pSuite, MailMessageTest, testWrite8Bit);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteBase64);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteManyRecipients);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteMultiPart);
	CppUnit_addTest(pSuite, MailMessageTest, testReadQP);
	CppUnit_addTest(pSuite, MailMessageTest, testReadDefaultTransferEncoding);
	CppUnit_addTest(pSuite, MailMessageTest, testRead8Bit);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPart);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartWithAttachmentNames);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartPartParameters);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartDefaultContentType);
	CppUnit_addTest(pSuite, MailMessageTest, testReadRecipients);
	CppUnit_addTest(pSuite, MailMessageTest, testReadRecipientsEncodedWords);
	CppUnit_addTest(pSuite, MailMessageTest, testReadWriteRecipientFieldsUnchanged);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteOmitsBccFields);
	CppUnit_addTest(pSuite, MailMessageTest, testReadHeaderLineTooLong);
	CppUnit_addTest(pSuite, MailMessageTest, testReadAddRecipientEscapesRealName);
	CppUnit_addTest(pSuite, MailMessageTest, testReadAddRecipientRewritesFields);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartDefaultTransferEncoding);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartMixedCaseHeaders);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartInvalidContentDisposition);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartNoFinalBoundaryFromFile);
	CppUnit_addTest(pSuite, MailMessageTest, testReadWriteMultiPart);
	CppUnit_addTest(pSuite, MailMessageTest, testReadWriteMultiPartStore);
	CppUnit_addTest(pSuite, MailMessageTest, testEncodeWord);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartEmptyBoundary);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartWithContentLength);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartWithZeroContentLength);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartManyParts);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartTooManyParts);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartLarge);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteFoldedField);
	CppUnit_addTest(pSuite, MailMessageTest, testValidateHeaders);
	CppUnit_addTest(pSuite, MailMessageTest, testReadMultiPartEncodedFileName);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteRejectsControlCharacterInEncodedText);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEncodesGroupName);
	CppUnit_addTest(pSuite, MailMessageTest, testReadWriteEncodedFieldsUnchanged);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEncodesNonAsciiText);
	CppUnit_addTest(pSuite, MailMessageTest, testEncodeWordB);
	CppUnit_addTest(pSuite, MailMessageTest, testEncodeWordAuto);
	CppUnit_addTest(pSuite, MailMessageTest, testEncodeWordLength);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEncodesShorterWord);
	CppUnit_addTest(pSuite, MailMessageTest, testMove);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteKeepsNonUTF8Text);
	CppUnit_addTest(pSuite, MailMessageTest, testReadRecipientsDomainLiteral);
	CppUnit_addTest(pSuite, MailMessageTest, testReadRecipientsMalformedDomainLiteral);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEncodesNameWithComment);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEncodesDomainLiteral);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteEscapesPartParameters);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteRealNameQuotedPair);
	CppUnit_addTest(pSuite, MailMessageTest, testReadRecipientsQuotedLocalPart);
	CppUnit_addTest(pSuite, MailMessageTest, testWriteKeepsQuotedLocalPart);

	return pSuite;
}
