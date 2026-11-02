//
// StatementExecutor.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  StatementExecutor
//
// Definition of the StatementExecutor class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_MySQL_StatementHandle_INCLUDED
#define Data_MySQL_StatementHandle_INCLUDED


#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Data/MySQL/SessionHandle.h"
#include <mysql/mysql.h>


namespace Poco::Data::MySQL {


class StatementExecutor
	/// MySQL statement executor.
{
public:
	enum State
	{
		STMT_INITED,
		STMT_COMPILED,
		STMT_EXECUTED
	};

	explicit StatementExecutor(SessionHandle& handle);
		/// Creates the StatementExecutor on the connection of the handle.
		/// Throws NullPointerException if the handle is closed.

	StatementExecutor(const StatementExecutor&) = delete;

	StatementExecutor& operator=(const StatementExecutor&) = delete;

	~StatementExecutor();
		/// Destroys the StatementExecutor.

	[[nodiscard]] int state() const;
		/// Returns the current state.

	void prepare(const std::string& query);
		/// Prepares the statement for execution.

	void bindParams(MYSQL_BIND* params, std::size_t count);
		/// Binds the params.

	void bindResult(MYSQL_BIND* result);
		/// Binds result.

	void execute();
		/// Executes the statement.

	[[nodiscard]] bool fetch();
		/// Fetches the data.

	[[nodiscard]] bool fetchColumn(std::size_t n, MYSQL_BIND *bind);
		/// Fetches the column.

	[[nodiscard]] int getAffectedRowCount() const;

	[[nodiscard]] bool isValid() const;
		/// Returns true if the connection on which the statement was created
		/// is still open. Once it has been closed, the statement is detached
		/// from it and must not be used, even if the session has been opened again.

	[[nodiscard]] operator MYSQL_STMT* ();
		/// Cast operator to native handle type.

private:
	using GenerationPtr = std::shared_ptr<const SessionHandle::Counter>;

	GenerationPtr _pGeneration;
	Poco::UInt32  _generation;
	MYSQL_STMT*   _pHandle;
	int           _state;
	std::size_t   _affectedRowCount;
	std::string   _query;
};


//
// inlines
//

inline bool StatementExecutor::isValid() const
{
	return *_pGeneration == _generation;
}


inline StatementExecutor::operator MYSQL_STMT* ()
{
	return _pHandle;
}


} // namespace Poco::Data::MySQL


#endif // Data_MySQL_StatementHandle_INCLUDED
