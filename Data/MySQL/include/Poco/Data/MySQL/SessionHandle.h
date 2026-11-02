//
// SesssionHandle.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  SessionHandle
//
// Definition of the SessionHandle class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_MySQL_SessionHandle_INCLUDED
#define Data_MySQL_SessionHandle_INCLUDED


#include "Poco/Data/MySQL/MySQLException.h"
#include <mysql/mysql.h>
#include <atomic>
#include <memory>


namespace Poco::Data::MySQL {


class SessionHandle
	/// MySQL session handle
{
public:
	using Counter = std::atomic<Poco::UInt32>;
		/// The type of the counter that generation() returns.

	explicit SessionHandle(MYSQL* mysql);
		/// Creates session handle

	SessionHandle(const SessionHandle&) = delete;

	SessionHandle& operator=(const SessionHandle&) = delete;

	~SessionHandle();
		/// Destroy handle, close connection

	void init(MYSQL* mysql = nullptr);
		/// Initializes the handle iff not initialized.

	void options(mysql_option opt);
		/// Set connection options
		/// Throws NullPointerException if the handle is closed.

	void options(mysql_option opt, bool b);
		/// Set connection options
		/// Throws NullPointerException if the handle is closed.

	void options(mysql_option opt, const char* c);
		/// Set connection options
		/// Throws NullPointerException if the handle is closed.

	void options(mysql_option opt, unsigned int i);
		/// Set connection options
		/// Throws NullPointerException if the handle is closed.

	void connect(const char* host, const char* user, const char* password, const char* db, unsigned int port);
		/// Connect to server
		/// If the connection fails, the handle is closed.
		/// Throws NullPointerException if the handle is closed.

	void close();
		/// Close connection

	void startTransaction();
		/// Start transaction
		/// Throws NullPointerException if the handle is closed.

	void commit();
		/// Commit transaction
		/// Throws NullPointerException if the handle is closed.

	void rollback();
		/// Rollback transaction
		/// Throws NullPointerException if the handle is closed.

	void autoCommit(bool val);
		/// Set autocommit mode
		/// Throws NullPointerException if the handle is closed.

	void reset();
		/// Reset connection with dababase and clears session state, but without disconnecting
		/// Throws NullPointerException if the handle is closed.

	[[nodiscard]] bool ping();
		/// Checks if the connection is alive.
		/// Throws NullPointerException if the handle is closed.

	[[nodiscard]] std::shared_ptr<const Counter> generation() const;
		/// Returns the number of times the handle has been closed.
		/// mysql_close() detaches the statements of the connection,
		/// so a statement must not be used once the number has
		/// changed since it was created.

	[[nodiscard]] operator MYSQL* ();
		/// Throws NullPointerException if the handle is closed.

private:
	MYSQL*                   _pHandle;
	std::shared_ptr<Counter> _pGeneration;
};


//
// inlines
//

inline std::shared_ptr<const SessionHandle::Counter> SessionHandle::generation() const
{
	return _pGeneration;
}


inline SessionHandle::operator MYSQL* ()
{
	poco_check_ptr (_pHandle);
	return _pHandle;
}


} // namespace Poco::Data::MySQL


#endif // Data_MySQL_SessionHandle_INCLUDED
