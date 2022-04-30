//
// Employee.h
//
// This file has been generated from ORM.xml. Do not edit.
//


#ifndef ORM_Employee_INCLUDED
#define ORM_Employee_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "ORM/Role.h"


namespace ORM {


class Employee: public Poco::ActiveRecord::ActiveRecord<Poco::UUID>
{
public:
	using Ptr = Poco::AutoPtr<Employee>;

	explicit Employee(ID id);
	Employee() = default;
	Employee(const Employee& other);
	~Employee() = default;

	const std::string& name() const;
	Employee& name(const std::string& value);

	const std::string& ssn() const;
	Employee& ssn(const std::string& value);

	Role::Ptr role() const;
	Poco::Int16 roleID() const;
	Employee& role(Role::Ptr pObject);
	Employee& roleID(Poco::Int16 id);

	Employee::Ptr manager() const;
	const Poco::UUID& managerID() const;
	Employee& manager(Employee::Ptr pObject);
	Employee& managerID(const Poco::UUID& id);

	static Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	std::string _name;
	std::string _ssn;
	Poco::Int16 _role = Role::INVALID_ID;
	Poco::UUID _manager;

	friend class Poco::Data::TypeHandler<Employee>;
};


inline const std::string& Employee::name() const
{
	return _name;
}


inline Employee& Employee::name(const std::string& value)
{
	_name = value;
	return *this;
}


inline const std::string& Employee::ssn() const
{
	return _ssn;
}


inline Employee& Employee::ssn(const std::string& value)
{
	_ssn = value;
	return *this;
}


inline Poco::Int16 Employee::roleID() const
{
	return _role;
}


inline Employee& Employee::roleID(Poco::Int16 value)
{
	_role = value;
	return *this;
}


inline const Poco::UUID& Employee::managerID() const
{
	return _manager;
}


inline Employee& Employee::managerID(const Poco::UUID& value)
{
	_manager = value;
	return *this;
}


} // namespace ORM


namespace Poco {
namespace Data {


template <>
class TypeHandler<ORM::Employee>
{
public:
	static std::size_t size()
	{
		return 4;
	}

	static void bind(std::size_t pos, const ORM::Employee& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<std::string>::bind(pos++, ar._name, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, ar._ssn, pBinder, dir);
		TypeHandler<Poco::Int16>::bind(pos++, ar._role, pBinder, dir);
		TypeHandler<Poco::UUID>::bind(pos++, ar._manager, pBinder, dir);
}

	static void extract(std::size_t pos, ORM::Employee& ar, const ORM::Employee& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<std::string>::extract(pos++, ar._name, deflt._name, pExtr);
		TypeHandler<std::string>::extract(pos++, ar._ssn, deflt._ssn, pExtr);
		TypeHandler<Poco::Int16>::extract(pos++, ar._role, deflt._role, pExtr);
		TypeHandler<Poco::UUID>::extract(pos++, ar._manager, deflt._manager, pExtr);
}

	static void prepare(std::size_t pos, const ORM::Employee& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<std::string>::prepare(pos++, ar._name, pPrep);
		TypeHandler<std::string>::prepare(pos++, ar._ssn, pPrep);
		TypeHandler<Poco::Int16>::prepare(pos++, ar._role, pPrep);
		TypeHandler<Poco::UUID>::prepare(pos++, ar._manager, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ORM_Employee_INCLUDED
