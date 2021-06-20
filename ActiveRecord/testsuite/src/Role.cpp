//
// Role.cpp
//
// This file has been generated from ORM.xml. Do not edit.
//


#include "ORM/Role.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ORM {


Role::Role(ID id):
	Poco::ActiveRecord::ActiveRecord<Poco::Int16>(id)
{
}


Role::Role(const Role& other):
	Poco::ActiveRecord::ActiveRecord<Poco::Int16>(other),
	_name(other._name),
	_description(other._description)
{
}


Role::Ptr Role::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	Role::Ptr pObject(new Role);

	pContext->session()
		<< "SELECT id, name, description"
		<< "  FROM roles"
		<< "  WHERE id = " << pSPP->next(),
		into(pObject->mutableID()),
		into(*pObject),
		bind(id),
		now;

	return withContext(pObject, pContext);
}


void Role::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "INSERT INTO roles (id, name, description)"
		<< "  VALUES (NULL, " << pSPP->next() << ", " << pSPP->next() << ")",
		use(*this),
		now;
	updateID(context()->session());
}


void Role::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE roles"
		<< "  SET name = " << pSPP->next() << ", description = " << pSPP->next()
		<< "  WHERE id = " << pSPP->next(),
		use(*this),
		bind(id()),
		now;
}


void Role::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM roles"
		<< "  WHERE id = " << pSPP->next(),
		bind(id()),
		now;
}


const std::vector<std::string>& Role::columns()
{
	static const std::vector<std::string> cols =
	{
		"id"s,
		"name"s,
		"description"s,
	};

	return cols;
}


const std::string& Role::table()
{
	static const std::string t = "roles";
	return t;
}


} // namespace ORM
