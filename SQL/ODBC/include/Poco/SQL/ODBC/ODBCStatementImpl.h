//
// ODBCStatementImpl.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ODBCStatementImpl
//
// Definition of the ODBCStatementImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_ODBC_ODBCStatementImpl_INCLUDED
#define SQL_ODBC_ODBCStatementImpl_INCLUDED


#include "Poco/SQL/ODBC/ODBC.h"
#include "Poco/SQL/ODBC/SessionImpl.h"
#include "Poco/SQL/ODBC/Binder.h"
#include "Poco/SQL/ODBC/Extractor.h"
#include "Poco/SQL/ODBC/Preparator.h"
#include "Poco/SQL/ODBC/ODBCMetaColumn.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/Column.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"
#include <sstream>
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace SQL {
namespace ODBC {


class ODBC_API ODBCStatementImpl: public Poco::SQL::StatementImpl
	/// Implements statement functionality needed for ODBC
{
public:
	ODBCStatementImpl(SessionImpl& rSession);
		/// Creates the ODBCStatementImpl.

	~ODBCStatementImpl();
		/// Destroys the ODBCStatementImpl.

protected:
	std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert or update.

	const MetaColumn& metaColumn(std::size_t pos, size_t dataSet) const;
		/// Returns column meta data.

	bool hasNext();
		/// Returns true if a call to next() will return data.

	std::size_t next();
		/// Retrieves the next row or set of rows from the resultset.
		/// Returns the number of rows retrieved.
		/// Will throw, if the resultset is empty.

	bool canBind() const;
		/// Returns true if a valid statement is set and we can bind.

	bool canCompile() const;
		/// Returns true if another compile is possible.

	void compileImpl();
		/// Compiles the statement, doesn't bind yet.
		/// Does nothing if the statement has already been compiled.

	void bindImpl();
		/// Binds all parameters and executes the statement.

	virtual bool canMakeExtractors();
		/// Returns true if extractors can be created.

	virtual void makeExtractors(std::size_t count);
		/// Create extractors for the specified dataset

	AbstractExtraction::ExtractorPtr extractor();
		/// Returns the concrete extractor used by the statement.

	AbstractBinding::BinderPtr binder();
		/// Returns the concrete binder used by the statement.

	std::string nativeSQL();
		/// Returns the SQL string as modified by the driver.

protected:

	virtual void insertHint();

private:
	typedef Poco::SQL::AbstractBindingVec    Bindings;
	typedef Poco::SharedPtr<Binder>           BinderPtr;
	typedef Poco::SQL::AbstractExtractionVec Extractions;
	typedef Poco::SharedPtr<Preparator>       PreparatorPtr;
	typedef std::vector<PreparatorPtr>        PreparatorVec;
	typedef Poco::SharedPtr<Extractor>        ExtractorPtr;
	typedef std::vector<ExtractorPtr>         ExtractorVec;
	typedef std::vector<ODBCMetaColumn*>      ColumnPtrVec;
	typedef std::vector<ColumnPtrVec>         ColumnPtrVecVec;
	
	static const std::string INVALID_CURSOR_STATE;

	void clear();
		/// Closes the cursor and resets indicator variables.
	
	void doBind();
		/// Binds parameters.

	bool isStoredProcedure() const;
		/// Returns true if SQL is a stored procedure call.

	void doPrepare();
		/// Prepares placeholders for data returned by statement.
		/// It is called during statement compilation for SQL statements
		/// returning data. For stored procedures returning datasets,
		/// it is called upon the first check for data availability
		/// (see hasNext() function).

	bool hasData() const;
		/// Returns true if statement returns data.

	void makeStep();
		/// Fetches the next row of data.

	bool nextRowReady() const;
		/// Returns true if there is a row fetched but not yet extracted.

	void putData();
		/// Called whenever SQLExecute returns SQL_NEED_DATA. This is expected
		/// behavior for PB_AT_EXEC binding mode.

	void getData();

	bool addPreparator(bool addAlways = true);
	void fillColumns(size_t dataSetPos);
	void checkError(SQLRETURN rc, const std::string& msg="");
	bool nextResultSet();

	const SQLHDBC&        _rConnection;
	const StatementHandle _stmt;
	PreparatorVec         _preparations;
	BinderPtr             _pBinder;
	ExtractorVec          _extractors;
	bool                  _stepCalled;
	int                   _nextResponse;
	ColumnPtrVecVec       _columnPtrs;
	bool                  _prepared;
	mutable std::size_t   _affectedRowCount;
	bool                  _canCompile;
	bool                  _insertHint;
};


//
// inlines
//
inline AbstractExtraction::ExtractorPtr ODBCStatementImpl::extractor()
{
	poco_assert_dbg (currentDataSet() < _extractors.size());
	poco_assert_dbg (_extractors[currentDataSet()]);
	return _extractors[currentDataSet()];
}


inline AbstractBinding::BinderPtr ODBCStatementImpl::binder()
{
	poco_assert_dbg (!_pBinder.isNull());
	return _pBinder;
}


inline std::size_t ODBCStatementImpl::columnsReturned() const
{
	poco_assert_dbg (currentDataSet() < _preparations.size());
	poco_assert_dbg (_preparations[currentDataSet()]);
	return static_cast<std::size_t>(_preparations[currentDataSet()]->columns());
}


inline bool ODBCStatementImpl::hasData() const
{
	return (columnsReturned() > 0);
}


inline bool ODBCStatementImpl::nextRowReady() const
{
	return (!Utility::isError(static_cast<SQLRETURN>(_nextResponse)));
}


inline bool ODBCStatementImpl::canCompile() const
{
	return _canCompile;
}


} } } // namespace Poco::SQL::ODBC


#endif // Data_ODBC_ODBCStatementImpl_INCLUDED
