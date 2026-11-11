//
// ODBCStatementImpl.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ODBCStatementImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/AbstractPreparation.h"
#include "Poco/Data/DataException.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <vector>


#ifdef POCO_OS_FAMILY_WINDOWS
	#pragma warning(disable:4312)// 'type cast' : conversion from 'std::size_t' to 'SQLPOINTER' of greater size
#endif


using Poco::DataFormatException;


namespace Poco::Data::ODBC {


const std::string ODBCStatementImpl::INVALID_CURSOR_STATE = "24000";


ODBCStatementImpl::ODBCStatementImpl(SessionImpl& rSession):
	Poco::Data::StatementImpl(rSession),
	_rConnection(rSession.dbc()),
	_stmt(rSession.dbc()),
	_stepCalled(false),
	_nextResponse(0),
	_prepared(false),
	_affectedRowCount(0),
	_canCompile(true)
{
	int queryTimeout = rSession.queryTimeout();
	int rc = 0;
	if (queryTimeout >= 0)
	{
		SQLULEN uqt = static_cast<SQLULEN>(queryTimeout);
		rc = SQLSetStmtAttr(_stmt,
			SQL_ATTR_QUERY_TIMEOUT,
			(SQLPOINTER) uqt,
			0);
		if (Utility::isError(rc))
		{
			throw ODBC::StatementException(_stmt,
				Poco::format("SQLSetStmtAttr(SQL_ATTR_QUERY_TIMEOUT, %d)", queryTimeout));
		}
	}
}


ODBCStatementImpl::~ODBCStatementImpl()
{
	// Bindings can outlive this statement and share ownership of the Binder,
	// which holds a reference to _stmt. Detach it while the handle is still
	// valid, so a later reset through a surviving binding never touches it.
	if (!_pBinder.isNull()) _pBinder->detach();

	ColumnPtrVecVec::iterator it = _columnPtrs.begin();
	ColumnPtrVecVec::iterator end = _columnPtrs.end();
	for (; it != end; ++it)
	{
		ColumnPtrVec::iterator itC = it->begin();
		ColumnPtrVec::iterator endC = it->end();
		for (; itC != endC; ++itC) delete *itC;
	}
}


void ODBCStatementImpl::compileImpl()
{
	checkConnection();
	if (!_canCompile) return;

	_stepCalled = false;
	_nextResponse = 0;

	if (_preparations.size())
		PreparatorVec().swap(_preparations);

	addPreparator();

	Binder::ParameterBinding bind = session().getFeature("autoBind") ?
		Binder::PB_IMMEDIATE : Binder::PB_AT_EXEC;

	const TypeInfo* pDT = nullptr;
	try
	{
		Poco::Any dti = session().getProperty("dataTypeInfo");
		pDT = AnyCast<const TypeInfo*>(dti);
	}
	catch (NotSupportedException&)
	{
	}

	std::size_t maxFieldSize;
	try
	{
		maxFieldSize = AnyCast<std::size_t>(session().getProperty("maxFieldSize"));
	}
	catch (Poco::BadCastException&)
	{
		maxFieldSize = AnyCast<int>(session().getProperty("maxFieldSize"));
	}

	_pBinder = new Binder(_stmt, maxFieldSize, bind, pDT, TextEncoding::find("UTF-8"),
		TextEncoding::find(Poco::RefAnyCast<std::string>(session().getProperty("dbEncoding"))));

	makeInternalExtractors();
	doPrepare();

	 _canCompile = false;
}


void ODBCStatementImpl::resetImpl()
{
	checkConnection();

	_prepared = false;
	_stepCalled = false;
	_nextResponse = 0;
	_canCompile = true;
	_affectedRowCount = 0;
	_errorInfo.clear();

	SQLRETURN rc = SQLFreeStmt(_stmt, SQL_CLOSE);
	checkError(rc, "ODBCStatementImpl::resetImpl():SQLFreeStmt(SQL_CLOSE)");

	for (auto& row: _columnPtrs)
	{
		for (auto* pCol: row) delete pCol;
	}
	_columnPtrs.clear();

	PreparatorVec().swap(_preparations);
	ExtractorVec().swap(_extractors);
}


void ODBCStatementImpl::makeInternalExtractors()
{
	if (hasData() && !extractions().size())
	{
		try
		{
			fillColumns();
		}
		catch (DataFormatException&)
		{
			if (isStoredProcedure()) return;
			throw;
		}

		makeExtractors(columnsReturned());
		fixupExtraction();
	}
}


void ODBCStatementImpl::addPreparator()
{
	if (0 == _preparations.size())
	{
		std::string statement(toString());
		if (statement.empty())
			throw ODBCException("Empty statements are illegal");

		Preparator::DataExtraction ext = session().getFeature("autoExtract") ?
			Preparator::DE_BOUND : Preparator::DE_MANUAL;

		std::size_t maxFieldSize;
		try
		{
			maxFieldSize = AnyCast<std::size_t>(session().getProperty("maxFieldSize"));
		}
		catch (Poco::BadCastException&)
		{
			maxFieldSize = AnyCast<int>(session().getProperty("maxFieldSize"));
		}

		_preparations.push_back(new Preparator(_stmt, statement, maxFieldSize, ext));
	}
	else
		_preparations.push_back(new Preparator(*_preparations[0]));

	_extractors.push_back(new Extractor(_stmt, _preparations.back(),
		TextEncoding::find(Poco::RefAnyCast<std::string>(session().getProperty("dbEncoding"))),
		TextEncoding::find("UTF-8")));
}


void ODBCStatementImpl::doPrepare()
{
	if (session().getFeature("autoExtract") && hasData())
	{
		std::size_t curDataSet = currentDataSet();
		poco_check_ptr (_preparations[curDataSet]);

		Extractions& extracts = extractions();
		Extractions::iterator it    = extracts.begin();
		Extractions::iterator itEnd = extracts.end();

		// SQLFetch() writes SQL_ATTR_ROW_ARRAY_SIZE rows into every bound column:
		// a non-bulk extraction is bound to one row, a bulk extraction to
		// getLimit() rows. The rowset size is set here, so every extraction of
		// the data set is checked here, whatever order Statement accepted them in.
		const bool bulk = (it != itEnd) && (*it)->isBulk();
		std::size_t rowsetSize = 1;
		if (bulk)
		{
			rowsetSize = getExtractionLimit();
			if (rowsetSize == Limit::LIMIT_UNLIMITED)
				throw InvalidArgumentException("Bulk operation not allowed without limit.");
		}
		for (Extractions::iterator itExt = it; itExt != itEnd; ++itExt)
		{
			if ((*itExt)->isBulk() != bulk)
				throw InvalidAccessException("Bulk and non-bulk extraction modes can not be mixed.");
			if (bulk && (*itExt)->getLimit() != rowsetSize)
				throw InvalidArgumentException("Bulk extraction size differs from the statement limit.");
		}
		// A later data set must inherit neither the rowset size nor the column
		// bindings of an earlier one: SQLMoreResults() keeps the bindings, so a
		// column this data set does not bind again would receive rowsetSize rows
		// into an earlier data set's buffers.
		if (curDataSet > 0)
			checkError(SQLFreeStmt(_stmt, SQL_UNBIND), "SQLFreeStmt(SQL_UNBIND)");
		if (bulk || curDataSet > 0)
		{
			checkError(Poco::Data::ODBC::SQLSetStmtAttr(_stmt, SQL_ATTR_ROW_ARRAY_SIZE, reinterpret_cast<SQLPOINTER>(static_cast<SQLULEN>(rowsetSize)), 0),
					"SQLSetStmtAttr(SQL_ATTR_ROW_ARRAY_SIZE)");
		}

		AbstractPreparation::Ptr pAP = nullptr;
		Poco::Data::AbstractPreparator::Ptr pP = _preparations[curDataSet];
		for (std::size_t pos = 0; it != itEnd; ++it)
		{
			pAP = (*it)->createPreparation(pP, pos);
			pAP->prepare();
			pos += (*it)->numOfColumnsHandled();
		}

		_prepared = true;
	}
}


bool ODBCStatementImpl::canBind() const
{
	if (!bindings().empty())
		return (*bindings().begin())->canBind();

	return false;
}


void ODBCStatementImpl::doBind()
{
	this->clear();
	Bindings& binds = bindings();

	// This is the first bind of an execution (as opposed to a later row of a
	// container binding): bind() sets ST_BOUND only after this returns.
	const bool first = (ST_COMPILED == getState());

	bool unbound = true;
	bool bulk = false;
	if (!binds.empty())
	{
		// The first bulk binding sets SQL_ATTR_PARAMSET_SIZE, and the driver then
		// reads (and for output parameters writes) that many values for every
		// parameter: scalar and bulk bindings, or bulk bindings of different
		// sizes, must not be mixed. The Binder binds every container at its
		// current size, not at bulkSize(), its size when use() ran.
		bulk = binds.front()->isBulk();
		const std::size_t rows = binds.front()->numOfRowsHandled();
		if (bulk && 0 == rows)
			throw InvalidArgumentException("Zero size containers not allowed.");
		for (const auto& pBinding: binds)
		{
			if (pBinding->isBulk() != bulk)
				throw InvalidAccessException("Bulk and non-bulk binding modes can not be mixed.");
			if (bulk && pBinding->numOfRowsHandled() != rows)
				throw InvalidArgumentException("Bulk bindings of different sizes can not be mixed.");
			unbound = unbound && pBinding->canBind();
		}
	}

	// Reset the Binder at the first bind of an execution, and whenever every
	// parameter rebinds (a later row of container bindings, or bindings that
	// replaced the earlier ones). Otherwise a binding that does not hold the
	// Binder, or that another statement reset last, would leave it with the
	// parameter-set size, length arrays and parameter bindings of an earlier
	// execution: the driver would read that many values from the new buffers,
	// and a marker that is not bound again would keep its old, freed ones. The
	// reset also runs when there are no bindings left, so a removed parameter
	// does not keep its binding. A later row that does not rebind every
	// parameter (a scalar reused across the rows of a container) keeps them.
	if (!_pBinder.isNull() && (first || unbound))
		_pBinder->reset();

	if (!binds.empty())
	{
		Bindings::iterator it    = binds.begin();
		Bindings::iterator itEnd = binds.end();

		// An empty non-bulk container at the start of an execution binds no
		// parameter, which would leave its marker reading the reset buffers.
		if (first && !bulk && !(*it)->canBind())
			throw InvalidArgumentException("Empty container not allowed.");

		if (it != itEnd && 0 == _affectedRowCount)
			_affectedRowCount = static_cast<std::size_t>((*it)->numOfRowsHandled());

		for (std::size_t pos = 0; it != itEnd && (*it)->canBind(); ++it)
		{
			(*it)->bind(pos);
			pos += (*it)->numOfColumnsHandled();
		}
	}
}


void ODBCStatementImpl::addErrors()
{
	SQLSMALLINT i = 0;
	SQLSMALLINT len;
	SQLRETURN ret;
	do
	{
		_errorInfo.push_back({});
		ret = SQLGetDiagRec(SQL_HANDLE_STMT, _stmt, ++i,
			_errorInfo.back().state, &_errorInfo.back().native, _errorInfo.back().text,
			sizeof(_errorInfo.back().text), &len);
		if (!SQL_SUCCEEDED(ret) && _errorInfo.size())
			_errorInfo.pop_back();
	} while( ret == SQL_SUCCESS );
}


void ODBCStatementImpl::printErrors(std::ostream& os) const
{
	if (_errorInfo.size())
	{
		os << "Errors\n==================";
		for (const auto& e : _errorInfo)
		{
			os << "\nstate: " << e.state << "\nnative: "
				<< e.native << "\ntext: " << e.text << '\n';
		}
		os << "==================\n";
	}
}


void ODBCStatementImpl::bindImpl()
{
	checkConnection();
	doBind();

	SQLRETURN rc = SQLExecute(_stmt);

	if (SQL_NEED_DATA == rc) putData();
	else checkError(rc, "ODBCStatementImpl::bindImpl():SQLExecute()");

	_pBinder->synchronize();
}


void ODBCStatementImpl::execDirectImpl(const std::string& query)
{
	checkConnection();
	// drop any parameter bindings a previous execution left in the driver
	if (!_pBinder.isNull()) _pBinder->reset();
	SQLCHAR * statementText = (SQLCHAR*) query.c_str();
	SQLINTEGER textLength = static_cast<SQLINTEGER>(query.size());
	SQLRETURN rc = SQLExecDirect(_stmt,statementText,textLength);

	checkError(rc, "SQLExecute()");
}


void ODBCStatementImpl::putData()
{
	SQLPOINTER pParam = nullptr;
	SQLINTEGER dataSize = 0;
	SQLRETURN rc;

	while (SQL_NEED_DATA == (rc = SQLParamData(_stmt, &pParam)))
	{
		if (pParam)
		{
			dataSize = (SQLINTEGER) _pBinder->parameterSize(pParam);

			if (Utility::isError(SQLPutData(_stmt, pParam, dataSize)))
				throw StatementException(_stmt, "ODBCStatementImpl::putData():SQLPutData()");
		}
		else // if pParam is null pointer, do a dummy call
		{
			char dummy = 0;
			if (Utility::isError(SQLPutData(_stmt, &dummy, 0)))
				throw StatementException(_stmt, "ODBCStatementImpl::putData():SQLPutData()");
		}
	}

	checkError(rc, "SQLParamData()");
}


void ODBCStatementImpl::clear()
{
	_stepCalled = false;
	_affectedRowCount = 0;
	_errorInfo.clear();
	SQLRETURN rc = SQLFreeStmt(_stmt, SQL_CLOSE);
	if (Utility::isError(rc))
		throw StatementException(_stmt, "ODBCStatementImpl::putData():SQLFreeStmt(SQL_CLOSE)");
}


bool ODBCStatementImpl::hasNext()
{
	checkConnection();
	if (hasData())
	{
		if (!extractions().size())
			makeInternalExtractors();

		if (!_prepared) doPrepare();

		if (_stepCalled)
			return _stepCalled = nextRowReady();

		makeStep();

		if (!nextRowReady())
		{
			if (!hasMoreDataSets()) return false;

			SQLRETURN rc = SQLMoreResults(_stmt);
			if (SQL_NO_DATA == rc) return false;
			checkError(rc, "SQLMoreResults()");

			(void)activateNextDataSet();
			addPreparator();
			doPrepare();
			fixupExtraction();
			makeStep();
		}
		else if (Utility::isError(_nextResponse))
			checkError(_nextResponse, "SQLFetch()");

		return true;
	}

	return false;
}


void ODBCStatementImpl::makeStep()
{
	_extractors[currentDataSet()]->reset();
	_nextResponse = SQLFetch(_stmt);
	// workaround for SQL Server drivers 17, 18, ...
	// stored procedure calls may produce additional data,
	// causing SQLFetch error 24000 (invalid cursor state);
	// when it happens, SQLMoreResults() is called to
	// force SQL_NO_DATA response
	if (Utility::isError(_nextResponse))
	{
		StatementError se(_stmt);
		const StatementDiagnostics& sd = se.diagnostics();

		for (int i = 0; i < sd.count(); ++i)
		{
			if (sd.sqlState(i) == INVALID_CURSOR_STATE)
			{
				_nextResponse = SQLMoreResults(_stmt);
				break;
			}
		}
	}
	checkError(_nextResponse);
	_stepCalled = true;
}


std::size_t ODBCStatementImpl::next()
{
	checkConnection();
	std::size_t count = 0;

	if (nextRowReady())
	{
		Extractions& extracts = extractions();
		poco_assert (extracts.size());
		Extractions::iterator it    = extracts.begin();
		Extractions::iterator itEnd = extracts.end();
		std::size_t prevCount = 0;
		for (std::size_t pos = 0; it != itEnd; ++it)
		{
			count = (*it)->extract(pos);
			if (prevCount && count != prevCount)
				throw IllegalStateException("Different extraction counts");
			prevCount = count;
			pos += (*it)->numOfColumnsHandled();
		}
		_stepCalled = false;
	}
	else
	{
		throw StatementException(_stmt,
			"ODBCStatementImpl::next():Next row not available.");
	}

	return count;
}


std::string ODBCStatementImpl::nativeSQL()
{
	poco_check_ptr (_rConnection);
	const std::string statement = toString();

	constexpr SQLINTEGER maxLength = (std::numeric_limits<SQLINTEGER>::max)() / 2 - 1;
	if (statement.size() > static_cast<std::size_t>(maxLength))
		throw LengthExceededException("ODBCStatementImpl::nativeSQL(): statement too long");

	SQLINTEGER length = static_cast<SQLINTEGER>(statement.size()) * 2 + 1;
	for (;;)
	{
		// Some drivers (SQLite ODBC 0.99991) write a terminator into the input,
		// at the position of the output buffer length, so the driver gets a
		// writable copy of the statement that is as long as the output buffer.
		std::vector<SQLCHAR> input((std::max)(statement.size(), static_cast<std::size_t>(length)) + 1, 0);
		std::memcpy(input.data(), statement.data(), statement.size());
		// the last byte is not given to the driver and terminates the output
		std::vector<SQLCHAR> native(static_cast<std::size_t>(length) + 1, 0);
		SQLINTEGER nativeLength = 0;
		if (Utility::isError(SQLNativeSql(_rConnection,
			input.data(),
			static_cast<SQLINTEGER>(statement.size()),
			native.data(),
			length,
			&nativeLength)))
		{
			throw ConnectionException(_rConnection, "SQLNativeSql()");
		}

		if (nativeLength < length || nativeLength > maxLength)
		{
			const auto end = std::find(native.begin(), native.end(), 0);
			return std::string(native.begin(), end);
		}
		length = nativeLength + 1;
	}
}


void ODBCStatementImpl::checkError(SQLRETURN rc, const std::string& msg)
{
	if (SQL_NO_DATA == rc) return;

	if (Utility::isError(rc))
	{
		std::ostringstream os;
		os << std::endl << "Requested SQL statement: " << toString() << std::endl;
		os << "Native SQL statement: ";
		try
		{
			os << nativeSQL();
		}
		catch (Poco::Exception&)
		{
			// the statement error is reported, not the failure to describe it
		}
		os << std::endl;
		std::string str(msg); str += os.str();

		throw StatementException(_stmt, str);
	}
	else if (SQL_SUCCESS_WITH_INFO == rc) addErrors();
}


void ODBCStatementImpl::checkConnection() const
{
	if (!_stmt.isValid())
		throw NotConnectedException(session().connectorName());
}


void ODBCStatementImpl::fillColumns()
{
	std::size_t colCount = columnsReturned();
	std::size_t curDataSet = currentDataSet();
	if (curDataSet >= _columnPtrs.size())
		_columnPtrs.resize(curDataSet + 1);

	for (std::size_t i = 0; i < colCount; ++i)
		_columnPtrs[curDataSet].push_back(new ODBCMetaColumn(_stmt, i));
}


bool ODBCStatementImpl::isStoredProcedure() const
{
	std::string str = toString();
	if (trimInPlace(str).size() < 2) return false;

	return ('{' == str[0] && '}' == str[str.size()-1]);
}


const MetaColumn& ODBCStatementImpl::metaColumn(std::size_t pos) const
{
	std::size_t curDataSet = currentDataSet();
	if (curDataSet < _columnPtrs.size())
	{
		std::size_t sz = _columnPtrs[curDataSet].size();

		if (0 == sz || pos > sz - 1)
			throw InvalidAccessException(format("Invalid column number: %u", pos));

		return *_columnPtrs[curDataSet][pos];
	}
	else throw Poco::IllegalStateException("currentDataSet index out of range");
}


int ODBCStatementImpl::affectedRowCount() const
{
	if (0 == _affectedRowCount && _stmt.isValid())
	{
		SQLLEN rows = 0;
		if (!Utility::isError(SQLRowCount(_stmt, &rows)))
			_affectedRowCount = static_cast<std::size_t>(rows);
	}

	return static_cast<int>(_affectedRowCount);
}


} // namespace Poco::Data::ODBC
