//
// MySQLstatementImpl.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  MySQLstatementImpl
//
// Definition of the MySQLStatementImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_MySQL_MySQLStatementImpl_INCLUDED
#define Data_MySQL_MySQLStatementImpl_INCLUDED


#include "Poco/Data/MySQL/MySQL.h"
#include "Poco/Data/MySQL/SessionImpl.h"
#include "Poco/Data/MySQL/Binder.h"
#include "Poco/Data/MySQL/Extractor.h"
#include "Poco/Data/MySQL/StatementExecutor.h"
#include "Poco/Data/MySQL/ResultMetadata.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {
namespace Data {
namespace MySQL {


class MySQL_API MySQLStatementImpl: public Poco::Data::StatementImpl
	/// Implements statement functionality needed for MySQL
{
public:
	MySQLStatementImpl(SessionImpl& s);
		/// Creates the MySQLStatementImpl.

	~MySQLStatementImpl() override;
		/// Destroys the MySQLStatementImpl.

protected:
	[[nodiscard]]
	std::size_t columnsReturned() const override;
		/// Returns number of columns returned by query.

	[[nodiscard]]
	int affectedRowCount() const override;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.

	[[nodiscard]]
	const MetaColumn& metaColumn(std::size_t pos) const override;
		/// Returns column meta data.

	[[nodiscard]]
	bool hasNext() override;
		/// Returns true if a call to next() will return data.

	[[nodiscard]]
	std::size_t next() override;
		/// Retrieves the next row from the resultset.
		/// Will throw, if the resultset is empty.

	[[nodiscard]]
	bool canBind() const override;
		/// Returns true if a valid statement is set and we can bind.

	[[nodiscard]]
	bool canCompile() const override;
		/// Returns true if another compile is possible.

	void compileImpl() override;
		/// Compiles the statement, doesn't bind yet

	void bindImpl() override;
		/// Binds parameters

	[[nodiscard]]
	Poco::Data::AbstractExtractor::Ptr extractor() override;
		/// Returns the concrete extractor used by the statement.

	[[nodiscard]]
	Poco::Data::AbstractBinder::Ptr binder() override;
		/// Returns the concrete binder used by the statement.

private:
	enum
	{
		NEXT_DONTKNOW,
		NEXT_TRUE,
		NEXT_FALSE
	};

	StatementExecutor _stmt;
	ResultMetadata    _metadata;
	Binder::Ptr       _pBinder;
	Extractor::Ptr    _pExtractor;
	int               _hasNext;
};


} } } // namespace Poco::Data::MySQL


#endif // Data_MySQL_MySQLStatementImpl_INCLUDED
