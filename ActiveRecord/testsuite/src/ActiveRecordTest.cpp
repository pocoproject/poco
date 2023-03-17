//
// ActiveRecordTest.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActiveRecordTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ActiveRecord/Context.h"
#include "Poco/ActiveRecord/Query.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Statement.h"
#include "Poco/UUIDGenerator.h"
#include "ORM/Employee.h"
#include "ORM/Role.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;
using Poco::ActiveRecord::Context;
using Poco::ActiveRecord::Query;
using ORM::Employee;
using ORM::Role;


const std::string ActiveRecordTest::CONNECTOR("SQLite");
const std::string ActiveRecordTest::CONNECTION_STRING("ORM.sqlite");


ActiveRecordTest::ActiveRecordTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveRecordTest::~ActiveRecordTest()
{
}


void ActiveRecordTest::testInsert()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	Role::Ptr pDeveloper = new Role;
	pDeveloper->name("Developer").description("Developer role");

	assertTrue (!pDeveloper->isValid());

	pDeveloper->create(pContext);

	assertTrue (pDeveloper->isValid());

	int n = 0;
	session << "SELECT COUNT(*) FROM roles", into(n), now;
	assertTrue (n == 1);

	assertTrue (pDeveloper->id() == 1);

	Role::Ptr pSeniorDeveloper = new Role;
	pSeniorDeveloper->name("Senior Developer").description("Senior developer role");

	pSeniorDeveloper->create(pContext);

	session << "SELECT COUNT(*) FROM roles", into(n), now;
	assertTrue (n == 2);

	assertTrue (pSeniorDeveloper->id() == 2);
}


void ActiveRecordTest::testFind()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Role::Ptr pRole = Role::find(pContext, 1);
	assertTrue (!pRole.isNull());
	assertTrue (pRole->name() == "Developer");
	assertTrue (pRole->description() == "Developer role");

	pRole = Role::find(pContext, 2);
	assertTrue (!pRole.isNull());
	assertTrue (pRole->name() == "Senior Developer");
	assertTrue (pRole->description() == "Senior developer role");

	pRole = Role::find(pContext, 3);
	assertTrue (!pRole.isNull());
	assertTrue (pRole->name() == "Manager");
	assertTrue (pRole->description() == "Manager role");

	pRole = Role::find(pContext, 4);
	assertTrue (pRole.isNull());
}


void ActiveRecordTest::testUpdate()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Role::Ptr pRole = Role::find(pContext, 1);
	assertTrue (!pRole.isNull());
	pRole->name("Junior Developer").description("Junior developer role");
	pRole->update();

	pRole = Role::find(pContext, 1);
	assertTrue (!pRole.isNull());
	assertTrue (pRole->name() == "Junior Developer");
	assertTrue (pRole->description() == "Junior developer role");
}


void ActiveRecordTest::testDelete()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Role::Ptr pRole = Role::find(pContext, 1);
	assertTrue (!pRole.isNull());

	pRole->remove();

	pRole = Role::find(pContext, 1);
	assertTrue (pRole.isNull());
}


void ActiveRecordTest::testRelations()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Employee::Ptr pManager = new Employee(Poco::UUIDGenerator().createOne());
	pManager->name("Bill Lumbergh").ssn("23452343").roleID(3);
	pManager->create(pContext);

	Role::Ptr pManagerRole = pManager->role();
	assertFalse (pManagerRole.isNull());
	assertTrue (pManagerRole->id() == 3);

	Employee::Ptr pEmployee = new Employee(Poco::UUIDGenerator().createOne());
	pEmployee->name("Michael Bolton").ssn("123987123").roleID(2).manager(pManager);
	pEmployee->create(pContext);

	assertTrue (pEmployee->managerID() == pManager->id());
}


void ActiveRecordTest::testQuery()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);

	auto result = query.execute();
	assertTrue (result.size() == 3);
}


void ActiveRecordTest::testQueryWhere()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);
	query.where("name = 'Senior Developer'");

	auto result = query.execute();
	assertTrue (result.size() == 1);
	assertTrue (result[0]->name() == "Senior Developer");
}


void ActiveRecordTest::testQueryWhereBind()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);
	query.where("name = ?").bind("Senior Developer"s);

	auto result = query.execute();
	assertTrue (result.size() == 1);
	assertTrue (result[0]->name() == "Senior Developer");
}


void ActiveRecordTest::testQueryFilter()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);
	query.filter(
		[](const Role& role)
		{
			return role.name() == "Senior Developer";
		}
	);

	auto result = query.execute();
	assertTrue (result.size() == 1);
	assertTrue (result[0]->name() == "Senior Developer");
}


void ActiveRecordTest::testQueryOrderBy()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);
	query.orderBy("id DESC");

	auto result = query.execute();
	assertTrue (result.size() == 3);
	assertTrue (result[0]->name() == "Manager");
}


void ActiveRecordTest::testQueryPaging()
{
	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);
	Context::Ptr pContext = new Context(session);

	createRoles(pContext);

	Query<Role> query(pContext);
	auto result = query.orderBy("id").offset(0).limit(2).execute();
	assertTrue (result.size() == 2);
	assertTrue (result[0]->name() == "Developer");
	assertTrue (result[1]->name() == "Senior Developer");

	query.reset();
	result = query.orderBy("id").offset(1).limit(2).execute();
	assertTrue (result.size() == 2);
	assertTrue (result[0]->name() == "Senior Developer");
	assertTrue (result[1]->name() == "Manager");
}


void ActiveRecordTest::setUp()
{
	Poco::Data::SQLite::Connector::registerConnector();

	Poco::Data::Session session(CONNECTOR, CONNECTION_STRING);

	session << "DROP TABLE IF EXISTS employees", now;
	session << "DROP TABLE IF EXISTS roles", now;
	session
		<< "CREATE TABLE employees ("
		<< "  id CHAR(36),"
		<< "  name VARCHAR(64),"
		<< "  ssn VARCHAR(32),"
		<< "  role INTEGER,"
		<< "  manager CHAR(36)"
		<< ")",
		now;
	session
		<< "CREATE TABLE roles ("
		<< "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
		<< "  name VARCHAR(64),"
		<< "  description VARCHAR(256)"
		<< ")",
		now;
}


void ActiveRecordTest::tearDown()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}


void ActiveRecordTest::createRoles(Poco::ActiveRecord::Context::Ptr pContext)
{
	Role::Ptr pDeveloper = new Role;
	pDeveloper->name("Developer").description("Developer role");
	pDeveloper->create(pContext);

	Role::Ptr pSeniorDeveloper = new Role;
	pSeniorDeveloper->name("Senior Developer").description("Senior developer role");
	pSeniorDeveloper->create(pContext);

	Role::Ptr pManager = new Role;
	pManager->name("Manager").description("Manager role");
	pManager->create(pContext);
}


CppUnit::Test* ActiveRecordTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActiveRecordTest");

	CppUnit_addTest(pSuite, ActiveRecordTest, testInsert);
	CppUnit_addTest(pSuite, ActiveRecordTest, testFind);
	CppUnit_addTest(pSuite, ActiveRecordTest, testUpdate);
	CppUnit_addTest(pSuite, ActiveRecordTest, testDelete);
	CppUnit_addTest(pSuite, ActiveRecordTest, testRelations);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQuery);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQueryWhere);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQueryWhereBind);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQueryOrderBy);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQueryFilter);
	CppUnit_addTest(pSuite, ActiveRecordTest, testQueryPaging);

	return pSuite;
}
