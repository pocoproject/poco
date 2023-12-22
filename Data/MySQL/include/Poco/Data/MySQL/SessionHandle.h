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


namespace Poco {
namespace Data {
namespace MySQL {


class SessionHandle
	/// MySQL session handle
{
public:
	explicit SessionHandle(MYSQL* mysql);
		/// Creates session handle

	~SessionHandle();
		/// Destroy handle, close connection

	void init(MYSQL* mysql = 0);
		/// Initializes the handle iff not initialized.

	void options(mysql_option opt);
		/// Set connection options

	void options(mysql_option opt, bool b);
		/// Set connection options

	void options(mysql_option opt, const char* c);
		/// Set connection options

	void options(mysql_option opt, unsigned int i);
		/// Set connection options

	void connect(const char* host, const char* user, const char* password, const char* db, unsigned int port);
		/// Connect to server

	void close();
		/// Close connection

	void startTransaction();
		/// Start transaction

	void commit();
		/// Commit transaction

	void rollback();
		/// Rollback transaction

	void autoCommit(bool val);
		/// Set autocommit mode

	void reset();
		/// Reset connection with dababase and clears session state, but without disconnecting

	bool ping();
		/// Checks if the connection is alive.

	operator MYSQL* ();

private:
	SessionHandle(const SessionHandle&);
	SessionHandle& operator=(const SessionHandle&);

private:
	MYSQL* _pHandle;
};


//
// inlines
//

inline SessionHandle::operator MYSQL* ()
{
	return _pHandle;
}


} } } // namespace Poco::Data::MySQL


#endif // Data_MySQL_SessionHandle_INCLUDED
