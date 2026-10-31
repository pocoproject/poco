//
// DataTest.h
//
// Definition of the DataTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataTest_INCLUDED
#define DataTest_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/Data/Row.h"
#include "CppUnit/TestCase.h"


class DataTest: public CppUnit::TestCase
{
public:
	DataTest(const std::string& name);
	~DataTest();

	void testSession();
	void testStatementFormatting();
	void testStatementMoveAssignment();
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
	void testTranscode();
	void testSQLParse();
	void testSQLChannel();
	void testNullableExtract();
	void testTransactionAutoCommit();
	void testExecuteDirectNotImplemented();
	void testSessionNotConnected();
	void testSessionNullImpl();
	void testRecordSetMovedFrom();
	void testRecordSetMoveFilter();
	void testNotConnectedMessage();
	void testTransactionNotConnected();
	void testTransactorFailure();
	void testStatementAsyncKeepsImpl();
	void testSQLChannelUnreachable();
	void testArchiveStrategyReopen();
	void testStatementAsyncError();
	void testStatementSetAsyncMovedFrom();
	void testTransactionNotConnectedLog();
	void testTransactorFailureLog();
	void testSQLChannelReconnectNotDelayed();
	void testSQLChannelWaitStopped();
	void testSQLChannelBackOff();
	void testSQLChannelArchiveNotConnected();
	void testSQLChannelDrainFallback();
	void testArchiveStrategyOpenReleasesStatements();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void testRowStrictWeak(const Poco::Data::Row& row1,
		const Poco::Data::Row& row2,
		const Poco::Data::Row& row3);
		/// Strict weak ordering requirement for sorted containers
		/// as described in Josuttis "The Standard C++ Library"
		/// chapter 6.5. pg. 176.
		/// For this to pass, the following condition must be satisifed:
		/// row1 < row2 < row3

	void writeToCLOB(Poco::BinaryWriter& writer);
	void readFromCLOB(Poco::BinaryReader& reader);
};


#endif // DataTest_INCLUDED
