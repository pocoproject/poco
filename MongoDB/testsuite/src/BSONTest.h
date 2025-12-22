//
// BSONTest.h
//
// Definition of the BSONTest class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef BSONTest_INCLUDED
#define BSONTest_INCLUDED


#include "CppUnit/TestCase.h"


class BSONTest: public CppUnit::TestCase
{
public:
	BSONTest(const std::string& name);
	virtual ~BSONTest();

	void setUp();
	void tearDown();

	// Document tests
	void testDocumentAddGet();
	void testDocumentExists();
	void testDocumentRemove();
	void testDocumentClear();
	void testDocumentSize();
	void testDocumentElementNames();
	void testNestedDocuments();
	void testDuplicateDocumentMembers();

	// Array tests
	void testArray();
	void testArrayIndexAccess();
	void testArrayNested();

	// Type tests
	void testDouble();
	void testString();
	void testBool();
	void testInt32();
	void testInt64();
	void testTimestamp();
	void testNull();
	void testBSONTimestamp();

	// Binary tests
	void testBinaryGeneric();
	void testBinaryUUID();
	void testBinarySubtypes();

	// ObjectId tests
	void testObjectID();
	void testObjectIDTimestamp();

	// RegularExpression tests
	void testRegularExpression();

	// JavaScriptCode tests
	void testJavaScriptCode();

	// Serialization/Deserialization tests
	void testDocumentSerialization();
	void testDocumentDeserialization();
	void testArraySerialization();
	void testComplexDocumentSerialization();

	// toString tests
	void testSimpleDocumentToString();
	void testNestedDocumentToString();
	void testDocumentWithArrayToString();
	void testComplexDocumentToString();
	void testToStringIndentation();
	void testArrayToString();

	// Failure/Error tests
	void testGetNonExistent();
	void testBadCast();
	void testInvalidObjectID();
	void testEmptyDocument();

	static CppUnit::Test* suite();
};


#endif // BSONTest_INCLUDED
