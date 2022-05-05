//
// ActiveRecordTest.h
//
// Definition of the ActiveRecordTest class.
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordTest_INCLUDED
#define ActiveRecordTest_INCLUDED


#include "Poco/ActiveRecord/Context.h"
#include "CppUnit/TestCase.h"


class ActiveRecordTest: public CppUnit::TestCase
{
public:
	ActiveRecordTest(const std::string& name);
	~ActiveRecordTest();

	void testInsert();
	void testFind();
	void testUpdate();
	void testDelete();
	void testRelations();
	void testQuery();
	void testQueryWhere();
	void testQueryWhereBind();
	void testQueryOrderBy();
	void testQueryFilter();
	void testQueryPaging();

	void setUp();
	void tearDown();

	void createRoles(Poco::ActiveRecord::Context::Ptr pContext);

	static CppUnit::Test* suite();

	static const std::string CONNECTOR;
	static const std::string CONNECTION_STRING;
};


#endif // ActiveRecordTest_INCLUDED
