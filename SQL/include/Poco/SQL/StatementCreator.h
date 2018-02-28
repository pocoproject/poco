//
// StatementCreator.h
//
// Library: Data
// Package: DataCore
// Module:  StatementCreator
//
// Definition of the StatementCreator class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_StatementCreator_INCLUDED
#define SQL_StatementCreator_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/SessionImpl.h"
#include "Poco/SQL/Statement.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace SQL {


class Poco_SQL_API StatementCreator
	/// A StatementCreator creates Statements.
{
public:
	StatementCreator();
		/// Creates an unitialized StatementCreator.

	StatementCreator(SessionImpl::Ptr ptrImpl);
		/// Creates a StatementCreator.

	StatementCreator(const StatementCreator& other);
		/// Creates a StatementCreator by copying another one.

	~StatementCreator();
		/// Destroys the StatementCreator.

	StatementCreator& operator = (const StatementCreator& other);
		/// Assignment operator.

	StatementCreator& operator = (Poco::AutoPtr<SessionImpl> ptrImpl);
		/// Assignment operator.

	void swap(StatementCreator& other);
		/// Swaps the StatementCreator with another one.

	template <typename T>
	Statement operator << (const T& t)
		/// Creates a Statement.
	{
		if (!_ptrImpl->isConnected())
			throw NotConnectedException(_ptrImpl->connectionString());

		Statement stmt(_ptrImpl->createStatementImpl());
		stmt << t;
		return stmt;
	}

private:
	Poco::AutoPtr<SessionImpl> _ptrImpl;
};


} } // namespace Poco::SQL


#endif // Data_StatementCreator_INCLUDED
