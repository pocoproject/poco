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


#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


Poco::Data::MetaColumn::ColumnDataType oidToColumnDataType(const Oid anOID)
{
	Poco::Data::MetaColumn::ColumnDataType cdt = Poco::Data::MetaColumn::FDT_UNKNOWN;

	switch (anOID)
	{
	// bool
	case BOOLOID:
		cdt = Poco::Data::MetaColumn::FDT_BOOL;
		break;

	// integers
	case INT2OID:
		cdt = Poco::Data::MetaColumn::FDT_INT16;
		break;
	case INT4OID:
		cdt = Poco::Data::MetaColumn::FDT_INT32;
		break;
	case INT8OID:
		cdt = Poco::Data::MetaColumn::FDT_INT64;
		break;

	// floating point
	case FLOAT8OID:
		cdt = Poco::Data::MetaColumn::FDT_DOUBLE;
		break;
	case FLOAT4OID:
		// cdt = Poco::Data::MetaColumn::FDT_FLOAT;  This a bug in Poco::Data:: as a 4 byte "float" can't be cast/ugraded to an 8 byte "double"
		cdt = Poco::Data::MetaColumn::FDT_DOUBLE;
		break;
	case NUMERICOID:
		cdt = Poco::Data::MetaColumn::FDT_DOUBLE;
		break;

	// character strings
	case CHAROID:
		cdt = Poco::Data::MetaColumn::FDT_STRING;
		break;
	case BPCHAROID:
		cdt = Poco::Data::MetaColumn::FDT_STRING;
		break;
	case VARCHAROID:
		cdt = Poco::Data::MetaColumn::FDT_STRING;
		break;

	// BLOB, CLOB
	case BYTEAOID:
		cdt = Poco::Data::MetaColumn::FDT_BLOB;
		break;
	case TEXTOID:
		cdt = Poco::Data::MetaColumn::FDT_CLOB;
		break;

	// date
	case DATEOID:
		cdt = Poco::Data::MetaColumn::FDT_DATE;
		break;

	// time
	case TIMEOID:
		cdt = Poco::Data::MetaColumn::FDT_TIME;
		break;
	case TIMETZOID:
		cdt = Poco::Data::MetaColumn::FDT_TIME;
		break;

	//timestamp
	case TIMESTAMPOID:
		cdt = Poco::Data::MetaColumn::FDT_TIMESTAMP;
		break;
	case TIMESTAMPZOID:
		cdt = Poco::Data::MetaColumn::FDT_TIMESTAMP;
		break;

	// everything else is a string
	default:
		cdt = Poco::Data::MetaColumn::FDT_STRING;
		break;
	}

	return cdt;
}


} } } // namespace Poco::Data::PostgreSQL
