//
// TypeInfo.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  TypeInfo
//
// Definition of TypeInfo.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_DataTypes_INCLUDED
#define Data_ODBC_DataTypes_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/NamedTuple.h"
#include "Poco/Dynamic/Var.h"
#include <vector>
#include <map>
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqlext.h>


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API TypeInfo
	/// Datatypes mapping utility class.
	///
	/// This class provides mapping between C and SQL datatypes as well
	/// as datatypes supported by the underlying database. In order for database
	/// types to be available, a valid conection handle must be supplied at either
	/// object construction time, or at a later point in time, through call to
	/// fillTypeInfo member function.
	///
	/// Class also provides a convenient debugging function that prints
	/// tabulated data to an output stream.
{
public:
	typedef std::map<int, int> DataTypeMap;
	typedef DataTypeMap::value_type ValueType;
	typedef Poco::NamedTuple<std::string,
		SQLSMALLINT,
		SQLINTEGER,
		std::string,
		std::string,
		std::string,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		std::string,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLINTEGER,
		SQLSMALLINT> TypeInfoTup;
	typedef std::vector<TypeInfoTup> TypeInfoVec;

	explicit TypeInfo(SQLHDBC* pHDBC=0);
		/// Creates the TypeInfo.

	~TypeInfo();
		/// Destroys the TypeInfo.

	SQLSMALLINT cDataType(SQLSMALLINT sqlDataType) const;
		/// Returns C data type corresponding to supplied SQL data type.

	template <SQLSMALLINT T>
	static constexpr SQLSMALLINT cDataType()
		/// Returns C data type corresponding to supplied SQL data type.
	{
		static_assert (
			(T == SQL_CHAR          ) ||
			(T == SQL_CHAR          ) ||
			(T == SQL_VARCHAR       ) ||
			(T == SQL_LONGVARCHAR   ) ||
			(T == SQL_DECIMAL       ) ||
			(T == SQL_NUMERIC       ) ||
			(T == SQL_BIT           ) ||
			(T == SQL_TINYINT       ) ||
			(T == SQL_SMALLINT      ) ||
			(T == SQL_INTEGER       ) ||
			(T == SQL_BIGINT        ) ||
			(T == SQL_REAL          ) ||
			(T == SQL_FLOAT         ) ||
			(T == SQL_DOUBLE        ) ||
			(T == SQL_BINARY        ) ||
			(T == SQL_VARBINARY     ) ||
			(T == SQL_LONGVARBINARY ) ||
			(T == SQL_TYPE_DATE     ) ||
			(T == SQL_TYPE_TIME     ) ||
			(T == SQL_TYPE_TIMESTAMP), "TypeInfo::cDataType(): Unknown SQL type."
		);

		if constexpr(T == SQL_CHAR          ) return SQL_C_CHAR;
		if constexpr(T == SQL_CHAR          ) return SQL_C_CHAR;
		if constexpr(T == SQL_VARCHAR       ) return SQL_C_CHAR;
		if constexpr(T == SQL_LONGVARCHAR   ) return SQL_C_CHAR;
		if constexpr(T == SQL_DECIMAL       ) return SQL_C_DOUBLE;
		if constexpr(T == SQL_NUMERIC       ) return SQL_C_DOUBLE;
		if constexpr(T == SQL_BIT           ) return SQL_C_BIT;
		if constexpr(T == SQL_TINYINT       ) return SQL_C_STINYINT;
		if constexpr(T == SQL_SMALLINT      ) return SQL_C_SSHORT;
		if constexpr(T == SQL_INTEGER       ) return SQL_C_SLONG;
		if constexpr(T == SQL_BIGINT        ) return SQL_C_SBIGINT;
		if constexpr(T == SQL_REAL          ) return SQL_C_FLOAT;
		if constexpr(T == SQL_FLOAT         ) return SQL_C_DOUBLE;
		if constexpr(T == SQL_DOUBLE        ) return SQL_C_DOUBLE;
		if constexpr(T == SQL_BINARY        ) return SQL_C_BINARY;
		if constexpr(T == SQL_VARBINARY     ) return SQL_C_BINARY;
		if constexpr(T == SQL_LONGVARBINARY ) return SQL_C_BINARY;
		if constexpr(T == SQL_TYPE_DATE     ) return SQL_C_TYPE_DATE;
		if constexpr(T == SQL_TYPE_TIME     ) return SQL_C_TYPE_TIME;
		if constexpr(T == SQL_TYPE_TIMESTAMP) return SQL_C_TYPE_TIMESTAMP;
		return 0;
	}

	SQLSMALLINT sqlDataType(SQLSMALLINT cDataType) const;
		/// Returns SQL data type corresponding to supplied C data type.

	template <SQLSMALLINT T>
	static constexpr SQLSMALLINT sqlDataType()
		/// Returns SQL data type corresponding to supplied C data type.
	{
		static_assert (
			(T == SQL_C_CHAR          ) ||
			(T == SQL_C_WCHAR         ) ||
			(T == SQL_C_BIT           ) ||
			(T == SQL_C_TINYINT       ) ||
			(T == SQL_C_STINYINT      ) ||
			(T == SQL_C_UTINYINT      ) ||
			(T == SQL_C_SHORT         ) ||
			(T == SQL_C_SSHORT        ) ||
			(T == SQL_C_USHORT        ) ||
			(T == SQL_C_LONG          ) ||
			(T == SQL_C_SLONG         ) ||
			(T == SQL_C_ULONG         ) ||
			(T == SQL_C_SBIGINT       ) ||
			(T == SQL_C_UBIGINT       ) ||
			(T == SQL_C_FLOAT         ) ||
			(T == SQL_C_DOUBLE        ) ||
			(T == SQL_C_BINARY        ) ||
			(T == SQL_C_TYPE_DATE     ) ||
			(T == SQL_C_TYPE_TIME     ) ||
			(T == SQL_C_TYPE_TIMESTAMP), "TypeInfo::sqlDataType(): Unknown C type."
		);

		if constexpr(T == SQL_C_CHAR          ) return SQL_VARCHAR;
		if constexpr(T == SQL_C_WCHAR         ) return SQL_WVARCHAR;
		if constexpr(T == SQL_C_BIT           ) return SQL_BIT;
		if constexpr(T == SQL_C_TINYINT       ) return SQL_TINYINT;
		if constexpr(T == SQL_C_STINYINT      ) return SQL_TINYINT;
		if constexpr(T == SQL_C_UTINYINT      ) return SQL_TINYINT;
		if constexpr(T == SQL_C_SHORT         ) return SQL_SMALLINT;
		if constexpr(T == SQL_C_SSHORT        ) return SQL_SMALLINT;
		if constexpr(T == SQL_C_USHORT        ) return SQL_SMALLINT;
		if constexpr(T == SQL_C_LONG          ) return SQL_INTEGER;
		if constexpr(T == SQL_C_SLONG         ) return SQL_INTEGER;
		if constexpr(T == SQL_C_ULONG         ) return SQL_INTEGER;
		if constexpr(T == SQL_C_SBIGINT       ) return SQL_BIGINT;
		if constexpr(T == SQL_C_UBIGINT       ) return SQL_BIGINT;
		if constexpr(T == SQL_C_FLOAT         ) return SQL_REAL;
		if constexpr(T == SQL_C_DOUBLE        ) return SQL_DOUBLE;
		if constexpr(T == SQL_C_BINARY        ) return SQL_LONGVARBINARY;
		if constexpr(T == SQL_C_TYPE_DATE     ) return SQL_TYPE_DATE;
		if constexpr(T == SQL_C_TYPE_TIME     ) return SQL_TYPE_TIME;
		if constexpr(T == SQL_C_TYPE_TIMESTAMP) return SQL_TYPE_TIMESTAMP;
		return 0;
	}

	void fillTypeInfo(const SQLHDBC* pHDBC);
		/// Fills the data type info structure for the database.

	Dynamic::Var getInfo(SQLSMALLINT type, const std::string& param) const;
		/// Returns information about specified data type as specified by parameter 'type'.
		/// The requested information is specified by parameter 'param'.
		/// Will fail with a Poco::NotFoundException thrown if the param is not found

	bool tryGetInfo(SQLSMALLINT type, const std::string& param, Dynamic::Var& result) const;
		/// Returns information about specified data type as specified by parameter 'type' in param result.
		/// The requested information is specified by parameter 'param'.
		/// Will return false if the param is not found. The value of result will be not changed in this case.


	void print(std::ostream& ostr);
		/// Prints all the types (as reported by the underlying database)
		/// to the supplied output stream.

private:
	void fillCTypes();
	void fillSQLTypes();

	DataTypeMap _cDataTypes;
	DataTypeMap _sqlDataTypes;
	TypeInfoVec _typeInfo;
	const SQLHDBC* _pHDBC;
};


} } } // namespace Poco::Data::ODBC


#endif
