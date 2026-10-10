//
// UtilityTest.h
//
// Definition of the UtilityTest class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef UtilityTest_INCLUDED
#define UtilityTest_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "CppUnit/TestCase.h"


class UtilityTest: public CppUnit::TestCase
{
public:
	UtilityTest(const std::string& name);
	~UtilityTest();

	void testRenderValue();
	void testBoundSQLQuestion();
	void testBoundSQLQuoteDoubling();
	void testBoundSQLPlaceholderInLiteral();
	void testBoundSQLQuestionTooFew();
	void testBoundSQLQuestionTooMany();
	void testBoundSQLDollar();
	void testBoundSQLDollarRepetition();
	void testBoundSQLDollarGap();
	void testBoundSQLDollarOutOfRange();
	void testBoundSQLDollarZero();
	void testBoundSQLMixedStyles();
	void testBoundSQLNamed();
	void testExecuteSQLSuccess();
	void testExecuteSQLConstraintFailure();
	void testExecuteSQLMalformed();
	void testExecuteSQLNullContext();
	void testExecuteSQLNullCallback();
	void testExecuteSQLNoBindings();
	void testToJSONSuccess();
	void testToJSONFailure();
	void testToJSONNoContext();
	void testToJSONEscapes();
	void testBoundSQLVectorScalar();
	void testBoundSQLVectorMaxRows3();
	void testBoundSQLCustomTypeHandler();
	void testBoundSQLFromStatementScalar();
	void testBoundSQLFromStatementVector();
	void testBoundSQLFromStatementVectorBulk();
	void testBoundSQLFromStatementCustomTypeHandler();
	void testBoundSQLFromStatementPreservesExecute();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // UtilityTest_INCLUDED
