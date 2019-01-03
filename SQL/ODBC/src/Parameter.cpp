//
// Parameter.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Parameter
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/ODBC/Parameter.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/Error.h"
#include "Poco/SQL/ODBC/ODBCException.h"


namespace Poco {
namespace SQL {
namespace ODBC {


Parameter::Parameter(const StatementHandle& rStmt, std::size_t colNum) :
	_rStmt(rStmt),
	_number(colNum)
{
	init();
}

	
Parameter::~Parameter()
{
}


void Parameter::init()
{
	if (Utility::isError(SQLDescribeParam(_rStmt,
		(SQLUSMALLINT) _number + 1,
		&_dataType,
		&_columnSize,
		&_decimalDigits,
		&_isNullable)))
	{
		throw StatementException(_rStmt);
	}
}


} } } // namespace Poco::SQL::ODBC
