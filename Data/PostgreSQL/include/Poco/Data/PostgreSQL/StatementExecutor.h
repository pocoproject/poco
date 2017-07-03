//
// StatementExecutor.h
//
// $Id: //poco/1.5/Data/PostgreSQL/include/Poco/Data/PostgreSQL/StatementExecutor.h#1 $
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  StatementExecutor
//
// Definition of the StatementExecutor class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_PostgreSQL_StatementHandle_INCLUDED
#define Data_PostgreSQL_StatementHandle_INCLUDED

#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/MetaColumn.h"

#include <libpq-fe.h>

#include <string>
#include <vector>

namespace Poco {
namespace Data {
namespace PostgreSQL {


class StatementExecutor
	/// PostgreSQL statement executor.
{
public:
	enum State
	{
		STMT_INITED,
		STMT_COMPILED,
		STMT_EXECUTED
	};

	explicit StatementExecutor(SessionHandle& aSessionHandle);
		/// Creates the StatementExecutor.

	~StatementExecutor();
		/// Destroys the StatementExecutor.

	State state() const;
		/// Returns the current state.

	void prepare(const std::string& aSQLStatement);
		/// Prepares the statement for execution.

	void bindParams(const InputParameterVector& anInputParameterVector);
		/// Binds the params - REQUIRED if the statment has input parameters/placeholders
		/// Pointer and list elements must stay valid for the lifetime of the StatementExecutor!

	void execute();
		/// Executes the statement.

	bool fetch();
		/// Fetches the data for the current row

	std::size_t getAffectedRowCount() const;
		/// get the count of rows affected by the statement

	std::size_t columnsReturned() const;
		/// get the count of columns returned by the statement

	const MetaColumn& metaColumn(std::size_t aPosition) const;
		/// Returns the reference to the specified metacolumn - 0 based

	const OutputParameter& resultColumn(std::size_t aPosition) const;
		/// Returns the reference to the specified result - 0 based

	operator PGresult* ();
		/// Cast operator to native result handle type.

private:
	
	void clearResults();
	
	StatementExecutor(const StatementExecutor&);
	StatementExecutor& operator= (const StatementExecutor&);

private:
	SessionHandle&				_sessionHandle;
	State						_state;

	PGresult*					_pResultHandle;
	std::string					_SQLStatement;
	std::string					_preparedStatementName;	// UUID based to allow multiple prepared statements per transaction.
	std::size_t					_countPlaceholdersInSQLStatement;
	std::vector<MetaColumn>		_resultColumns;

	InputParameterVector		_inputParameterVector;
	OutputParameterVector		_outputParameterVector;
	std::size_t					_currentRow;			// current row of the result
	std::size_t					_affectedRowCount;
};


//
// inlines
//

inline StatementExecutor::operator PGresult* ()
{
	return _pResultHandle;
}
	
}}}

#endif // Data_PostgreSQL_StatementHandle_INCLUDED
