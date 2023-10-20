//
// StatementExecutor.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  StatementExecutor
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PostgreSQL/StatementExecutor.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Format.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberParser.h"
#include "Poco/RegularExpression.h"
#include <algorithm>
#include <set>


namespace
{
	std::size_t countOfPlaceHoldersInSQLStatement(const std::string& aSQLStatement)
	{
		// Find unique placeholders.
		// Unique placeholders allow the same placeholder to be used multiple times in the same statement.

		// NON C++11 implementation

		//if (aSQLStatement.empty())
		//{
		//return 0;
		//}

		// set to hold the unique placeholders ($1, $2, $3, etc.).
		// A set is used because the same placeholder can be used muliple times
		std::set<std::string> placeholderSet;

		Poco::RegularExpression placeholderRE("[$][0-9]+");
		Poco::RegularExpression::Match match = { 0 , 0 }; // Match is a struct, not a class :-(

		std::size_t startingPosition = 0;
		while (match.offset != std::string::npos)
		{
			try
			{
				if (placeholderRE.match(aSQLStatement, startingPosition, match))
				{
					placeholderSet.insert(aSQLStatement.substr(match.offset, match.length));
					startingPosition = match.offset + match.length;
				}
			}
			catch (Poco::RegularExpressionException &)
			{
				break;
			}
		}

		return placeholderSet.size();
	}
} // namespace


namespace Poco {
namespace Data {
namespace PostgreSQL {


StatementExecutor::StatementExecutor(SessionHandle& sessionHandle, bool binaryExtraction):
	_sessionHandle(sessionHandle),
	_binaryExtraction(binaryExtraction),
	_state(STMT_INITED),
	_pResultHandle(0),
	_countPlaceholdersInSQLStatement(0),
	_currentRow(0),
	_affectedRowCount(0)
{
}


StatementExecutor::~StatementExecutor()
{
	try
	{
		// remove the prepared statement from the session
		if(_sessionHandle.isConnected() && _state >= STMT_COMPILED)
		{
			_sessionHandle.deallocatePreparedStatement(_preparedStatementName);
		}

		PQResultClear resultClearer(_pResultHandle);
	}
	catch (...)
	{
	}
}


StatementExecutor::State StatementExecutor::state() const
{
	return _state;
}


void StatementExecutor::prepare(const std::string& aSQLStatement)
{
	if (!_sessionHandle.isConnected()) throw NotConnectedException();
	if (_state >= STMT_COMPILED) return;

	// clear out the metadata.  One way or another it is now obsolete.
	_countPlaceholdersInSQLStatement = 0;
	_SQLStatement= std::string();
	_preparedStatementName   = std::string();
	_resultColumns.clear();

	// clear out any result data.  One way or another it is now obsolete.
	clearResults();

	// prepare parameters for the call to PQprepare
	const char* ptrCSQLStatement = aSQLStatement.c_str();
	std::size_t countPlaceholdersInSQLStatement = countOfPlaceHoldersInSQLStatement(aSQLStatement);

	Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
	Poco::UUID uuid(generator.create()); // time based
	std::string statementName = uuid.toString();
	statementName.insert(0, 1, 'p'); // prepared statement names can't start with a number
	std::replace(statementName.begin(), statementName.end(), '-', 'p');  // PostgreSQL doesn't like dashes in prepared statement names
	const char* pStatementName = statementName.c_str();

	PGresult* ptrPGResult = 0;

	{
		Poco::FastMutex::ScopedLock mutexLocker(_sessionHandle.mutex());

		// prepare the statement - temporary PGresult returned
		ptrPGResult = PQprepare(_sessionHandle, pStatementName, ptrCSQLStatement, (int)countPlaceholdersInSQLStatement, 0);
	}

	{
                // get the sqlState
                const char* pSQLState	= PQresultErrorField(ptrPGResult, PG_DIAG_SQLSTATE);		

		// setup to clear the result from PQprepare
		PQResultClear resultClearer(ptrPGResult);

                //
		if  (!ptrPGResult || PQresultStatus(ptrPGResult) != PGRES_COMMAND_OK )
		{
			throw StatementException(std::string("postgresql_stmt_prepare error: ") + PQresultErrorMessage (ptrPGResult) + " " + aSQLStatement,pSQLState);
		}

	}

	// Determine what the structure of a statement result will look like
	{
		Poco::FastMutex::ScopedLock mutexLocker(_sessionHandle.mutex());
		ptrPGResult = PQdescribePrepared(_sessionHandle, pStatementName);
	}

	{
 		// get the sqlState
                const char* pSQLState	= PQresultErrorField(ptrPGResult, PG_DIAG_SQLSTATE);
	
		PQResultClear resultClearer(ptrPGResult);

		if (!ptrPGResult || PQresultStatus(ptrPGResult) != PGRES_COMMAND_OK)
		{
			throw StatementException(std::string("postgresql_stmt_describe error: ") +
				PQresultErrorMessage (ptrPGResult) + " " + aSQLStatement,pSQLState);
		}

		// remember the structure of the statement result
		int fieldCount = PQnfields(ptrPGResult);
		if (fieldCount < 0) fieldCount = 0;

		for (int i = 0; i < fieldCount; ++i)
		{
			_resultColumns.push_back(MetaColumn(i, PQfname(ptrPGResult, i),
				oidToColumnDataType(PQftype(ptrPGResult, i)), 0, 0, true));
		}
	}

	_SQLStatement = aSQLStatement;
	_preparedStatementName = statementName;
	_countPlaceholdersInSQLStatement = countPlaceholdersInSQLStatement;
	_state = STMT_COMPILED;  // must be last
}


void StatementExecutor::bindParams(const InputParameterVector& anInputParameterVector)
{
	if (!_sessionHandle.isConnected()) throw NotConnectedException();

	if (_state < STMT_COMPILED) throw StatementException("Statement is not compiled yet");

	if (anInputParameterVector.size() != _countPlaceholdersInSQLStatement)
	{
		throw StatementException(std::string("incorrect bind parameters count for SQL Statement: ") +
			_SQLStatement);
	}

	// Just record the input vector for later execution
	_inputParameterVector = anInputParameterVector;
}


void StatementExecutor::execute()
{
	if (!_sessionHandle.isConnected()) throw NotConnectedException();

	if (_state < STMT_COMPILED) throw StatementException("Statement is not compiled yet");

	if (_countPlaceholdersInSQLStatement != 0 &&
		_inputParameterVector.size() != _countPlaceholdersInSQLStatement)
	{
		throw StatementException("Count of Parameters in Statement different than supplied parameters");
	}

	// "transmogrify" the _inputParameterVector to the C format required by PQexecPrepared

	/* - from example
		const char *paramValues[1];
		int paramLengths[1];
		int paramFormats[1];
	*/

	std::vector<const char *> pParameterVector;
	std::vector<int>  parameterLengthVector;
	std::vector<int>  parameterFormatVector;

	InputParameterVector::const_iterator cItr    = _inputParameterVector.begin();
	InputParameterVector::const_iterator cItrEnd = _inputParameterVector.end();

	for (; cItr != cItrEnd; ++cItr)
	{
		try
		{
			pParameterVector.push_back  (static_cast<const char*>(cItr->pInternalRepresentation()));
			parameterLengthVector.push_back((int)cItr->size());
			parameterFormatVector.push_back((int)cItr->isBinary() ? 1 : 0);
		}
		catch (std::bad_alloc&)
		{
			throw StatementException("Memory Allocation Error");
		}
	}

	// clear out any result data.  One way or another it is now obsolete.
	clearResults();

	PGresult* ptrPGResult = 0;
	{
		Poco::FastMutex::ScopedLock mutexLocker(_sessionHandle.mutex());

		ptrPGResult = PQexecPrepared(_sessionHandle,
			_preparedStatementName.c_str(), (int)_countPlaceholdersInSQLStatement,
			_inputParameterVector.size() != 0 ? &pParameterVector[ 0 ] : 0,
			_inputParameterVector.size() != 0 ? &parameterLengthVector[ 0 ] : 0,
			_inputParameterVector.size() != 0 ? &parameterFormatVector[ 0 ] : 0,
			_binaryExtraction ? 1 : 0);
	}

	// Don't setup to auto clear the result (ptrPGResult).  It is required to retrieve the results later.

	if (!ptrPGResult || (PQresultStatus(ptrPGResult) != PGRES_COMMAND_OK &&
		PQresultStatus(ptrPGResult) != PGRES_TUPLES_OK))
	{
		PQResultClear resultClearer(ptrPGResult);

		const char* pSeverity	= PQresultErrorField(ptrPGResult, PG_DIAG_SEVERITY);
		const char* pSQLState	= PQresultErrorField(ptrPGResult, PG_DIAG_SQLSTATE);
		const char* pDetail		= PQresultErrorField(ptrPGResult, PG_DIAG_MESSAGE_DETAIL);
		const char* pHint		= PQresultErrorField(ptrPGResult, PG_DIAG_MESSAGE_HINT);
		const char* pConstraint	= PQresultErrorField(ptrPGResult, PG_DIAG_CONSTRAINT_NAME);

                
		throw StatementException(std::string("postgresql_stmt_execute error: ")
			+ PQresultErrorMessage (ptrPGResult)
			+ " Severity: " + (pSeverity   ? pSeverity   : "N/A")
			+ " State: " + (pSQLState   ? pSQLState   : "N/A")
			+ " Detail: " + (pDetail ? pDetail : "N/A")
			+ " Hint: " + (pHint   ? pHint   : "N/A")
			+ " Constraint: " + (pConstraint ? pConstraint : "N/A"),pSQLState);
	}

	_pResultHandle = ptrPGResult;

	// are there any results?

	int affectedRowCount = 0;

	if (PGRES_TUPLES_OK == PQresultStatus(_pResultHandle))
	{
		affectedRowCount = PQntuples(_pResultHandle);

		if (affectedRowCount >= 0)
		{
			_affectedRowCount = static_cast<std::size_t>(affectedRowCount);
		}
	}
	else
	{	// non Select DML statments also have an affected row count.
		// unfortunately PostgreSQL offers up this count as a char * - go figure!
		const char * pNonSelectAffectedRowCountString = PQcmdTuples(_pResultHandle);
		if (0 != pNonSelectAffectedRowCountString)
		{
			if	(	Poco::NumberParser::tryParse(pNonSelectAffectedRowCountString, affectedRowCount)
				 && affectedRowCount >= 0
				)
			{
				_affectedRowCount = static_cast<std::size_t>(affectedRowCount);
				_currentRow = _affectedRowCount;  // no fetching on these statements!
			}
		}
	}

	_state = STMT_EXECUTED;
}


bool StatementExecutor::fetch()
{
	if (!_sessionHandle.isConnected())
	{
		throw NotConnectedException();
	}

	if (_state < STMT_EXECUTED)
	{
		throw StatementException("Statement is not yet executed");
	}

	std::size_t countColumns = columnsReturned();

	// first time to fetch?
	if (0 == _outputParameterVector.size())
	{
		// setup a output vector for the results
		_outputParameterVector.resize(countColumns);
	}

	// already retrieved last row?
	if (_currentRow == getAffectedRowCount())
	{
		return false;
	}

	if	(0 == countColumns || PGRES_TUPLES_OK != PQresultStatus(_pResultHandle))
	{
		return false;
	}

	for (int i = 0; i < countColumns; ++i)
	{
		int fieldLength = PQgetlength(_pResultHandle, static_cast<int> (_currentRow), static_cast<int> (i));

		Oid columnInternalDataType = PQftype(_pResultHandle, i);  // Oid of column

		_outputParameterVector.at(i).setValues(oidToColumnDataType(columnInternalDataType), // Poco::Data::MetaData version of the Column Data Type
			columnInternalDataType, // Postgres Version
			_currentRow, // the row number of the result
			PQgetvalue(_pResultHandle, (int)_currentRow, i), // a pointer to the data
			(-1 == fieldLength ? 0 : fieldLength), // the length of the data returned
			PQgetisnull(_pResultHandle, (int)_currentRow, i) == 1 ? true : false); // is the column value null?
	}

	++_currentRow;
	return true;
}


std::size_t StatementExecutor::getAffectedRowCount() const
{
	return _affectedRowCount;
}


std::size_t StatementExecutor::columnsReturned() const
{
	return static_cast<std::size_t> (_resultColumns.size());
}


const MetaColumn& StatementExecutor::metaColumn(std::size_t aPosition) const
{
	if (aPosition >= columnsReturned())
	{
		throw StatementException("Invalid column number for metaColumn");
	}

	return _resultColumns.at(aPosition);
}


const OutputParameter& StatementExecutor::resultColumn(std::size_t aPosition) const
{
	if (aPosition >= columnsReturned())
	{
		throw StatementException("Invalid column number for resultColumn");
	}

	return _outputParameterVector.at(aPosition);
}


void StatementExecutor::clearResults()
{
	// clear out any old result first
	{
		PQResultClear resultClearer(_pResultHandle);
	}

	_outputParameterVector.clear();
	_affectedRowCount	= 0;
	_currentRow			= 0;
}


} } } // Poco::Data::PostgreSQL
