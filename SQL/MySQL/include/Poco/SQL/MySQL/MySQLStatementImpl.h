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


#ifndef SQL_MySQL_MySQLStatementImpl_INCLUDED
#define SQL_MySQL_MySQLStatementImpl_INCLUDED

#include "Poco/SQL/MySQL/MySQL.h"
#include "Poco/SQL/MySQL/SessionImpl.h"
#include "Poco/SQL/MySQL/Binder.h"
#include "Poco/SQL/MySQL/Extractor.h"
#include "Poco/SQL/MySQL/StatementExecutor.h"
#include "Poco/SQL/MySQL/ResultMetadata.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {
namespace SQL {
namespace MySQL {


class MySQL_API MySQLStatementImpl: public Poco::SQL::StatementImpl
	/// Implements statement functionality needed for MySQL
{
public:
	MySQLStatementImpl(SessionImpl& s);
		/// Creates the MySQLStatementImpl.
		
	~MySQLStatementImpl();
		/// Destroys the MySQLStatementImpl.
		
protected:

	virtual std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	virtual int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.
	
	virtual const MetaColumn& metaColumn(std::size_t pos, std::size_t dataSet) const;
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


} } } // namespace Poco::SQL::MySQL


#endif // Data_MySQL_MySQLStatementImpl_INCLUDED
