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


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/PostgreSQL/Binder.h"
#include "Poco/Data/PostgreSQL/StatementExecutor.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/AbstractExtractor.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API PostgreSQLStatementImpl: public Poco::Data::StatementImpl
	/// Implements statement functionality needed for PostgreSQL
{
public:
	PostgreSQLStatementImpl(SessionImpl& aSessionImpl);
		/// Creates the PostgreSQLStatementImpl.

	~PostgreSQLStatementImpl() override;
		/// Destroys the PostgreSQLStatementImpl.

protected:
	std::size_t columnsReturned() const override;
		/// Returns number of columns returned by query.

	int affectedRowCount() const override;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.

	const MetaColumn& metaColumn(std::size_t aPosition) const override;
		/// Returns column meta data.

	bool hasNext() override;
		/// Returns true if a call to next() will return data.

	std::size_t next() override;
		/// Retrieves the next row from the resultset.
		/// Will throw, if the resultset is empty.

	bool canBind() const override;
		/// Returns true if a valid statement is set and we can bind.

	bool canCompile() const override;
		/// Returns true if another compile is possible.

	void compileImpl() override;
		/// Compiles the statement, doesn't bind yet

	void bindImpl() override;
		/// Binds parameters

	Poco::Data::AbstractExtractor::Ptr extractor() override;
		/// Returns the concrete extractor used by the statement.

	Poco::Data::AbstractBinder::Ptr binder() override;
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
	AbstractExtractor::Ptr _pExtractor;
	NextState         _hasNext;
};


} } } // namespace Poco::Data::PostgreSQL


#endif // SQL_PostgreSQL_PostgreSQLStatementImpl_INCLUDED
