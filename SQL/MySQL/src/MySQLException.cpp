//
// MySQLException.cpp
//
// Library: Data/MySQL
// Package: MySQL
// Module:  MySQLException
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/MySQL/MySQLException.h"
#include <mysql.h>
#include <stdio.h>

namespace Poco {
namespace SQL {
namespace MySQL {


MySQLException::MySQLException(const std::string& msg, int errCode) :
	Poco::SQL::SQLException(std::string("[MySQL]: ") + msg, errCode)
{
}


MySQLException::MySQLException(const MySQLException& exc) :
	Poco::SQL::SQLException(exc)
{
}


MySQLException::~MySQLException() throw()
{
}


/////
//
// ConnectionException
//
/////


ConnectionException::ConnectionException(const std::string& msg) : MySQLException(msg)
{
}


ConnectionException::ConnectionException(const std::string& text, MYSQL* h) :
	MySQLException(compose(text, h), mysql_errno(h))
{
}


std::string ConnectionException::compose(const std::string& text, MYSQL* h)
{
	std::string str;
	str += "[Comment]: ";
	str += text;
	str += "\t[mysql_error]: ";
	str += mysql_error(h);

	str += "\t[mysql_errno]: ";
	char buff[30];
	sprintf(buff, "%d", mysql_errno(h));
	str += buff;

	str += "\t[mysql_sqlstate]: ";
	str += mysql_sqlstate(h);
	return str;
}


/////
//
// TransactionException
//
/////


TransactionException::TransactionException(const std::string& msg) : ConnectionException(msg)
{
}


TransactionException::TransactionException(const std::string& text, MYSQL* h) : ConnectionException(text, h)
{
}


/////
//
// StatementException
//
/////


StatementException::StatementException(const std::string& msg) : MySQLException(msg)
{
}


StatementException::StatementException(const std::string& text, MYSQL_STMT* h, const std::string& stmt) :
	MySQLException(compose(text, h, stmt), mysql_stmt_errno(h))
{
}


std::string StatementException::compose(const std::string& text, MYSQL_STMT* h, const std::string& stmt)
{
	std::string str;
	str += "[Comment]: ";
	str += text;

	if (h != 0)
	{
		str += "\t[mysql_stmt_error]: ";
		str += mysql_stmt_error(h);

		str += "\t[mysql_stmt_errno]: ";
		char buff[30];
		sprintf(buff, "%d", mysql_stmt_errno(h));
		str += buff;

		str += "\t[mysql_stmt_sqlstate]: ";
		str += mysql_stmt_sqlstate(h);
	}

	if (stmt.length() > 0)
	{
		str += "\t[statement]: ";
		str += stmt;
	}

	return str;
}

} } } // namespace Poco::SQL::MySQL
