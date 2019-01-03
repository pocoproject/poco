//
// SQLiteStatementImpl.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  SQLiteStatementImpl
//
// Definition of the SQLiteStatementImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_SQLite_SQLiteStatementImpl_INCLUDED
#define SQL_SQLite_SQLiteStatementImpl_INCLUDED


#include "Poco/SQL/SQLite/SQLite.h"
#include "Poco/SQL/SQLite/Binder.h"
#include "Poco/SQL/SQLite/Extractor.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/MetaColumn.h"
#include "Poco/SharedPtr.h"


extern "C"
{
	typedef struct sqlite3 sqlite3;
	typedef struct sqlite3_stmt sqlite3_stmt;
}


namespace Poco {
namespace SQL {
namespace SQLite {


class SQLite_API SQLiteStatementImpl: public Poco::SQL::StatementImpl
	/// Implements statement functionality needed for SQLite
{
public:
	SQLiteStatementImpl(Poco::SQL::SessionImpl& rSession, sqlite3* pDB);
		/// Creates the SQLiteStatementImpl.

	~SQLiteStatementImpl();
		/// Destroys the SQLiteStatementImpl.

protected:
	std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert, delete or update.
		/// All changes are counted, even if they are later undone by a ROLLBACK or ABORT.
		/// Changes associated with creating and dropping tables are not counted.

	const MetaColumn& metaColumn(std::size_t pos, std::size_t dataSet) const;
		/// Returns column meta data.

	bool hasNext();
		/// Returns true if a call to next() will return data.

	std::size_t next();
		/// Retrieves the next row from the resultset and returns 1.
		/// Will throw, if the resultset is empty.

	bool canBind() const;
		/// Returns true if a valid statement is set and we can bind.

	bool canCompile() const;
		/// Returns true if statement can compile.

	void compileImpl();
		/// Compiles the statement, doesn't bind yet.
		/// Returns true if the statement was successfully compiled.
		/// The way SQLite handles batches of statements is by compiling
		/// one at a time and returning a pointer to the next one.
		/// The remainder of the statement is kept in a string
		/// buffer pointed to by _pLeftover member.

	void bindImpl();
		/// Binds parameters

	AbstractExtraction::ExtractorPtr extractor();
		/// Returns the concrete extractor used by the statement.

	AbstractBinding::BinderPtr binder();
		/// Returns the concrete binder used by the statement.

private:
	void clear();
		/// Removes the _pStmt

	typedef Poco::SharedPtr<Binder>             BinderPtr;
	typedef Poco::SharedPtr<Extractor>          ExtractorPtr;
	typedef Poco::SQL::AbstractBindingVec      Bindings;
	typedef Poco::SQL::AbstractExtractionVec   Extractions;
	typedef std::vector<Poco::SQL::MetaColumn> MetaColumnVec;
	typedef std::vector<MetaColumnVec>          MetaColumnVecVec;
	typedef Poco::SharedPtr<std::string>        StrPtr;
	typedef Bindings::iterator                  BindIt;

	sqlite3*         _pDB;
	sqlite3_stmt*    _pStmt;
	bool             _stepCalled;
	int              _nextResponse;
	BinderPtr        _pBinder;
	ExtractorPtr     _pExtractor;
	MetaColumnVecVec _columns;
	int              _affectedRowCount;
	StrPtr           _pLeftover;
	BindIt           _bindBegin;
	bool             _canBind;
	bool             _isExtracted;
	bool             _canCompile;

	static const int POCO_SQLITE_INV_ROW_CNT;
};


//
// inlines
//
inline AbstractExtraction::ExtractorPtr SQLiteStatementImpl::extractor()
{
	return _pExtractor;
}


inline AbstractBinding::BinderPtr SQLiteStatementImpl::binder()
{
	return _pBinder;
}


inline bool SQLiteStatementImpl::canBind() const
{
	return _canBind;
}


inline bool SQLiteStatementImpl::canCompile() const
{
	return _canCompile;
}


} } } // namespace Poco::SQL::SQLite


#endif // Data_SQLite_SQLiteStatementImpl_INCLUDED
