//
// PostgreSQLStatementImpl.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQLStatementImpl
//
// Definition of the PostgreSQLStatementImpl class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_PostgreSQLStatementImpl_INCLUDED
#define SQL_PostgreSQL_PostgreSQLStatementImpl_INCLUDED

#include "Poco/SQL/PostgreSQL/PostgreSQL.h"
#include "Poco/SQL/PostgreSQL/SessionImpl.h"
#include "Poco/SQL/PostgreSQL/Binder.h"
#include "Poco/SQL/PostgreSQL/Extractor.h"
#include "Poco/SQL/PostgreSQL/StatementExecutor.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {
namespace SQL {
namespace PostgreSQL {


class PostgreSQL_API PostgreSQLStatementImpl: public Poco::SQL::StatementImpl
	/// Implements statement functionality needed for PostgreSQL
{
public:
	PostgreSQLStatementImpl(SessionImpl& aSessionImpl);
		/// Creates the PostgreSQLStatementImpl.

	~PostgreSQLStatementImpl();
		/// Destroys the PostgreSQLStatementImpl.

protected:

	virtual std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	virtual int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.

	virtual const MetaColumn& metaColumn(std::size_t aPosition, std::size_t aDataset) const;
		/// Returns column meta data.

	virtual bool hasNext();
		/// Returns true if a call to next() will return data.

	virtual std::size_t next();
		/// Retrieves the next row from the resultset.
		/// Will throw, if the resultset is empty.

	virtual bool canBind() const;
		/// Returns true if a valid statement is set and we can bind.

	virtual bool canCompile() const;
		/// Returns true if another compile is possible.

	virtual void compileImpl();
		/// Compiles the statement, doesn't bind yet

	virtual void bindImpl();
		/// Binds parameters

	virtual Poco::SQL::AbstractExtractor::Ptr extractor();
		/// Returns the concrete extractor used by the statement.

	virtual Poco::SQL::AbstractBinder::Ptr binder();
		/// Returns the concrete binder used by the statement.

private:
	enum NextState
	{
		NEXT_DONTKNOW,
		NEXT_TRUE,
		NEXT_FALSE
	};

	StatementExecutor _statementExecutor;
	Binder::Ptr       _pBinder;
	Extractor::Ptr    _pExtractor;
	NextState         _hasNext;
};


} } } // namespace Poco::SQL::PostgreSQL


#endif // SQL_PostgreSQL_PostgreSQLStatementImpl_INCLUDED
