//
// SQLTest.h
//
// Definition of the SQLTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLTest_INCLUDED
#define SQLTest_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/SQL/Row.h"
#include "Poco/CppUnit/TestCase.h"


class SQLTest: public CppUnit::TestCase
{
public:
	SQLTest(const std::string& name);
	~SQLTest();

	void testSession();
	void testStatementFormatting();
	void testFeatures();
	void testProperties();
	void testLOB();
	void testCLOB();
	void testCLOBStreams();
	void testColumnVector();
	void testColumnVectorBool();
	void testColumnDeque();
	void testColumnList();
	void testRow();
	void testRowSort();
	void testSimpleRowFormatter();
	void testJSONRowFormatter();
	void testDateAndTime();
	void testExternalBindingAndExtraction();

	void testStdTuple();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void testRowStrictWeak(const Poco::SQL::Row& row1,
		const Poco::SQL::Row& row2,
		const Poco::SQL::Row& row3);
		/// Strict weak ordering requirement for sorted containers
		/// as described in Josuttis "The Standard C++ Library"
		/// chapter 6.5. pg. 176.
		/// For this to pass, the following condition must be satisifed:
		/// row1 < row2 < row3

	void writeToCLOB(Poco::BinaryWriter& writer);
	void readFromCLOB(Poco::BinaryReader& reader);
};


#endif // SQLTest_INCLUDED
