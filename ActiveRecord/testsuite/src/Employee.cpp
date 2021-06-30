//
// Employee.cpp
//
// This file has been generated from ORM.xml. Do not edit.
//


#include "ORM/Employee.h"
#include "Poco/UUIDGenerator.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ORM {


Employee::Employee(ID id):
	Poco::ActiveRecord::ActiveRecord<Poco::UUID>(id)
{
}


Employee::Employee(const Employee& other):
	Poco::ActiveRecord::ActiveRecord<Poco::UUID>(other),
	_name(other._name),
	_ssn(other._ssn),
	_role(other._role),
	_manager(other._manager)
{
}


Role::Ptr Employee::role() const
{
	return Role::find(context(), _role);
}


Employee& Employee::role(Role::Ptr pObject)
{
	if (pObject)
		_role = pObject->id();
	else
		_role = Role::INVALID_ID;
	return *this;
}


Employee::Ptr Employee::manager() const
{
	return Employee::find(context(), _manager);
}


Employee& Employee::manager(Employee::Ptr pObject)
{
	if (pObject)
		_manager = pObject->id();
	else
		_manager = Employee::INVALID_ID;
	return *this;
}


Employee::Ptr Employee::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	Employee::Ptr pObject(new Employee);

	pContext->session()
		<< "SELECT id, name, ssn, role, manager"
		<< "  FROM employees"
		<< "  WHERE id = " << pSPP->next(),
		into(pObject->mutableID()),
		into(*pObject),
		bind(id),
		now;

	return withContext(pObject, pContext);
}


void Employee::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	if (id().isNull())
	{
		mutableID() = Poco::UUIDGenerator().createRandom();
	}

	context()->session()
		<< "INSERT INTO employees (id, name, ssn, role, manager)"
		<< "  VALUES (" << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		bind(id()),
		use(*this),
		now;
}


void Employee::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE employees"
		<< "  SET name = " << pSPP->next() << ", ssn = " << pSPP->next() << ", role = " << pSPP->next() << ", manager = " << pSPP->next()
		<< "  WHERE id = " << pSPP->next(),
		use(*this),
		bind(id()),
		now;
}


void Employee::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM employees"
		<< "  WHERE id = " << pSPP->next(),
		bind(id()),
		now;
}


const std::vector<std::string>& Employee::columns()
{
	static const std::vector<std::string> cols =
	{
		"id"s,
		"name"s,
		"ssn"s,
		"role"s,
		"manager"s,
	};

	return cols;
}


const std::string& Employee::table()
{
	static const std::string t = "employees";
	return t;
}


} // namespace ORM
