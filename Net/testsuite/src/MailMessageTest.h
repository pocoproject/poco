//
// MailMessageTest.h
//
// Definition of the MailMessageTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MailMessageTest_INCLUDED
#define MailMessageTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class MailMessageTest: public CppUnit::TestCase
{
public:
	MailMessageTest(const std::string& name);
	~MailMessageTest();

	void testWriteQP();
	void testWrite8Bit();
	void testWriteBase64();
	void testWriteManyRecipients();
	void testWriteMultiPart();
	void testReadWriteMultiPart();
	void testReadWriteMultiPartStore();
	void testReadDefaultTransferEncoding();
	void testReadQP();
	void testRead8Bit();
	void testReadMultiPart();
	void testReadMultiPartWithAttachmentNames();
	void testReadMultiPartPartParameters();
	void testReadMultiPartDefaultContentType();
	void testReadRecipients();
	void testReadRecipientsEncodedWords();
	void testReadWriteRecipientFieldsUnchanged();
	void testWriteOmitsBccFields();
	void testReadHeaderLineTooLong();
	void testReadAddRecipientEscapesRealName();
	void testReadAddRecipientRewritesFields();
	void testReadMultiPartDefaultTransferEncoding();
	void testReadMultiPartMixedCaseHeaders();
	void testReadMultiPartInvalidContentDisposition();
	void testReadMultiPartNoFinalBoundaryFromFile();
	void testEncodeWord();
	void testReadMultiPartEmptyBoundary();
	void testReadMultiPartWithContentLength();
	void testReadMultiPartWithZeroContentLength();
	void testReadMultiPartManyParts();
	void testReadMultiPartTooManyParts();
	void testReadMultiPartLargeWithContentLength();
	void testReadMultiPartLargeWithoutContentLength();
	void testWriteFoldedField();
	void testValidateHeaders();
	void testReadMultiPartEncodedFileName();
	void testWriteRejectsControlCharacterInEncodedText();
	void testWriteEncodesGroupName();
	void testReadWriteEncodedFieldsUnchanged();
	void testWriteEncodesNonAsciiText();
	void testWriteKeepsNonUTF8Text();
	void testReadRecipientsDomainLiteral();
	void testReadRecipientsMalformedDomainLiteral();
	void testWriteEncodesNameWithComment();
	void testWriteEncodesDomainLiteral();
	void testReadRecipientsQuotedLocalPart();
	void testWriteKeepsQuotedLocalPart();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // MailMessageTest_INCLUDED
