//
// PostgreSQLTypes.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQLTypes
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PostgreSQL/PostgreSQLTypes.h"

namespace Poco {
namespace SQL {
namespace PostgreSQL {

Poco::SQL::MetaColumn::ColumnDataType oidToColumnDataType(const Oid anOID)
{
	Poco::SQL::MetaColumn::ColumnDataType cdt = Poco::SQL::MetaColumn::FDT_UNKNOWN;
	
	switch (anOID)
	{
	// bool
	case BOOLOID:
		cdt = Poco::SQL::MetaColumn::FDT_BOOL;
		break;

	// integers
	case INT2OID:
		cdt = Poco::SQL::MetaColumn::FDT_INT16;
		break;
	case INT4OID:
		cdt = Poco::SQL::MetaColumn::FDT_INT32;
		break;
	case INT8OID:
		cdt = Poco::SQL::MetaColumn::FDT_INT64;
		break;

	// floating point
	case FLOAT8OID:
		cdt = Poco::SQL::MetaColumn::FDT_DOUBLE;
		break;
	case FLOAT4OID:
		// cdt = Poco::SQL::MetaColumn::FDT_FLOAT;  This a bug in Poco::SQL as a 4 byte "float" can't be cast/ugraded to an 8 byte "double"
		cdt = Poco::SQL::MetaColumn::FDT_DOUBLE;
		break;
	case NUMERICOID:
		cdt = Poco::SQL::MetaColumn::FDT_DOUBLE;
		break;

	// character strings
	case CHAROID:
		cdt = Poco::SQL::MetaColumn::FDT_STRING;
		break;
	case BPCHAROID:
		cdt = Poco::SQL::MetaColumn::FDT_STRING;
		break;
	case VARCHAROID:
		cdt = Poco::SQL::MetaColumn::FDT_STRING;
		break;

	// BLOB, CLOB
	case BYTEAOID:
		cdt = Poco::SQL::MetaColumn::FDT_BLOB;
		break;
	case TEXTOID:
		cdt = Poco::SQL::MetaColumn::FDT_CLOB;
		break;

	// date
	case DATEOID:
		cdt = Poco::SQL::MetaColumn::FDT_DATE;
		break;

	// time
	case TIMEOID:
		cdt = Poco::SQL::MetaColumn::FDT_TIME;
		break;
	case TIMETZOID:
		cdt = Poco::SQL::MetaColumn::FDT_TIME;
		break;

	//timestamp
	case TIMESTAMPOID:
		cdt = Poco::SQL::MetaColumn::FDT_TIMESTAMP;
		break;
	case TIMESTAMPZOID:
		cdt = Poco::SQL::MetaColumn::FDT_TIMESTAMP;
		break;

	// everything else is a string
	default:
		cdt = Poco::SQL::MetaColumn::FDT_STRING;
		break;
	}

	return cdt;
}

} } } // namespace Poco::SQL::PostgreSQL

