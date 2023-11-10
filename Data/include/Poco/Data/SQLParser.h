//
// SQLParser.h
//
// Library: Data
// Package: SQLParser
// Module:  SQLParser
//
// Forward header for the SQLParser class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SQLParser_INCLUDED
#define Data_SQLParser_INCLUDED

#include "Poco/Config.h"


#ifndef POCO_DATA_NO_SQL_PARSER


#include "sql-parser/src/SQLParser.h"
#include "sql-parser/src/SQLParserResult.h"
#include "sql-parser/src/util/sqlhelper.h"


namespace Poco {
namespace Data {

	namespace Parser = hsql; // namespace Poco::Data::Parser

} } // namespace Poco::Data


#endif //  POCO_DATA_NO_SQL_PARSER


#endif // Data_SQLParser_INCLUDED
