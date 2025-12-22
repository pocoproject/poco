//
// ActiveRecord.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  ActiveRecord
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_ActiveRecord_INCLUDED
#define ActiveRecord_ActiveRecord_INCLUDED


#include "Poco/ActiveRecord/ActiveRecordLib.h"
#include "Poco/ActiveRecord/Context.h"
#include "Poco/ActiveRecord/IDTraits.h"
#include "Poco/DateTime.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Types.h"
#include <limits>


namespace Poco {
namespace ActiveRecord {


class ActiveRecordLib_API ActiveRecordBase: public Poco::RefCountedObject
	/// The base class for the ActiveRecord class template.
{
public:
	using Ptr = Poco::AutoPtr<ActiveRecordBase>;

	virtual std::string toString() const = 0;
		/// Returns a string representation of the object for
		/// debugging purposes. The default implementation returns the ID.

	void create(Context::Ptr pContext);
		/// Attaches the given Context and calls insert().

	virtual void insert() = 0;
		/// Inserts a new row in the database with the content of this object.
		/// The ID must be 0, and after successful insert, a unique ID
		/// is assigned.

	virtual void update() = 0;
		/// Updates the row in the database with the content of this object.

	virtual void remove() = 0;
		/// Deletes the corresponding row in the database.

	void attach(Context::Ptr pContext);
		/// Attaches the object to a Context.

	void detach();
		/// Detaches the object from its Context.

	Context::Ptr context() const;
		/// Returns the Context this object is attached to,
		/// or a null pointer if the object has not been
		/// attached to a Context.

	virtual bool isValid() const;
		/// Returns true iff the object is valid ID, otherwise false.

	bool isAttached() const;
		/// Returns true iff the object has been attached to a Context, otherwise false.

protected:
	ActiveRecordBase() = default;
	~ActiveRecordBase() = default;

	template <typename T>
	static Poco::AutoPtr<T> withContext(Poco::AutoPtr<T> pObj, Context::Ptr pContext)
	{
		if (pObj && pObj->isValid())
		{
			pObj->attach(pContext);
			return pObj;
		}
		else return nullptr;
	}

private:
	ActiveRecordBase(const ActiveRecordBase&) = delete;
	ActiveRecordBase& operator = (const ActiveRecordBase&) = delete;

	Context::Ptr _pContext;
};


template <typename IDType>
class ActiveRecord: public ActiveRecordBase
	/// The base class for all database objects that
	/// implement the ActiveRecord pattern, with a
	/// single key column.
{
public:
	using Ptr = Poco::AutoPtr<ActiveRecord>;
	using ID = IDType;

	static const IDType INVALID_ID;

	ID id() const;
		/// Returns the unique ID of the object.

	// ActiveRecordBase
	std::string toString() const;
	bool isValid() const;

protected:
	ActiveRecord() = default;
	ActiveRecord(ID id): _id(id) {};
	~ActiveRecord() = default;

	ActiveRecord(const ActiveRecord& other):
		_id(other._id)
	{
	}

	ID& mutableID();

	void updateID(Poco::Data::Session& session);
		/// Updates the ID using lastInsertID().

	static ID lastInsertID(Poco::Data::Session& session);
		/// Returns the last inserted ID from the database session.
		/// Used for automatically incrementing keys.

	template <typename T>
	static Poco::AutoPtr<T> withContext(Poco::AutoPtr<T> pObj, Context::Ptr pContext)
	{
		if (pObj->isValid())
		{
			pObj->attach(pContext);
			return pObj;
		}
		else return nullptr;
	}

	template <typename AR>
	static void queryInto(Poco::Data::Statement& statement, AR& ar)
	{
		statement, Poco::Data::Keywords::into(ar._id), Poco::Data::Keywords::into(ar);
	}

private:
	ActiveRecord& operator = (const ActiveRecord&) = delete;

	ID _id = INVALID_ID;

	template <typename ActRec> friend class Query;
};


template <typename IDType>
const IDType ActiveRecord<IDType>::INVALID_ID = IDTraits<IDType>::INVALID_ID;


class ActiveRecordLib_API KeylessActiveRecord: public ActiveRecordBase
	/// The base class for all database objects that
	/// implement the ActiveRecord pattern, without
	/// a key column.
{
public:
	using Ptr = Poco::AutoPtr<KeylessActiveRecord>;

	// ActiveRecordBase
	std::string toString() const;

protected:
	template <typename AR>
	static void queryInto(Poco::Data::Statement& statement, AR& ar)
	{
		statement, Poco::Data::Keywords::into(ar);
	}

	template <typename ActRec> friend class Query;
};


//
// inlines
//


inline Context::Ptr ActiveRecordBase::context() const
{
	return _pContext;
}


inline bool ActiveRecordBase::isAttached() const
{
	return !_pContext.isNull();
}


template <typename IDType>
inline IDType ActiveRecord<IDType>::id() const
{
	return _id;
}


template <typename IDType>
inline IDType& ActiveRecord<IDType>::mutableID()
{
	return _id;
}


template <typename IDType>
inline bool ActiveRecord<IDType>::isValid() const
{
	return IDTraits<IDType>::isValid(_id);
}


template <typename IDType>
inline std::string ActiveRecord<IDType>::toString() const
{
	return IDTraits<IDType>::toString(_id);
}


template <typename IDType>
void ActiveRecord<IDType>::updateID(Poco::Data::Session& session)
{
	_id = lastInsertID(session);
}


template <typename IDType>
IDType ActiveRecord<IDType>::lastInsertID(Poco::Data::Session& session)
{
	using namespace Poco::Data::Keywords;
	using namespace std::string_literals;

	IDType id;
	if (Poco::icompare(session.connector(), "sqlite"s) == 0)
	{
		session
			<< "SELECT last_insert_rowid()",
			into(id),
			now;
	}
	else if (Poco::icompare(session.connector(), "postgresql"s) == 0)
	{
		session
			<< "SELECT lastval()",
			into(id),
			now;
	}
	else if (Poco::icompare(session.connector(), "mysql"s) == 0)
	{
		session
			<< "SELECT LAST_INSERT_ID()",
			into(id),
			now;
	}
	else
	{
		throw Poco::NotImplementedException("lastInsertID not implemented for connector", session.connector());
	}
	return id;
}


} } // namespace Poco::ActiveRecord


#endif // ActiveRecord_ActiveRecord_INCLUDED
