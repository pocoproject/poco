//
// Binder.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Binder
//
// Definition of the Binder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_ODBC_Binder_INCLUDED
#define SQL_ODBC_Binder_INCLUDED


#include "Poco/SQL/ODBC/ODBC.h"
#include "Poco/SQL/AbstractBinder.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/ODBC/Handle.h"
#include "Poco/SQL/ODBC/Parameter.h"
#include "Poco/SQL/ODBC/ODBCMetaColumn.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/TypeInfo.h"
#include "Poco/Exception.h"
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <sqlext.h>


namespace Poco {


class DateTime;


namespace SQL {


class Date;
class Time;


namespace ODBC {


class ODBC_API Binder: public Poco::SQL::AbstractBinder
	/// Binds placeholders in the sql query to the provided values. Performs data types mapping.
{

	struct ParamDescriptor
	{
		ParamDescriptor() : colSize(0), cDataType(0), decDigits(-1)
		{}

		ParamDescriptor(SQLINTEGER colSize_, SQLSMALLINT cDataType_, SQLSMALLINT decDigits_) : colSize(colSize_), cDataType(cDataType_), decDigits(decDigits_)
		{}

		bool defined() const { return cDataType != 0; }
		SQLINTEGER colSize;
		SQLSMALLINT cDataType;
		SQLSMALLINT decDigits;
	};

public:
	typedef AbstractBinder::Direction Direction;
	typedef std::map<SQLPOINTER, SQLLEN> ParamMap;

	static const size_t DEFAULT_PARAM_SIZE = 1024;


	enum ParameterBinding
	{
		PB_IMMEDIATE,
		PB_AT_EXEC
	};

	Binder(const StatementHandle& rStmt,
		std::size_t maxFieldSize,
		ParameterBinding dataBinding,
		TypeInfo* pDataTypes,
		bool insertOnly);
		/// Creates the Binder.

	~Binder();
		/// Destroys the Binder.

	void bind(std::size_t pos, const Poco::Int8& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int8.

	void bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir);
		/// Binds an Int8 vector.

	void bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir);
		/// Binds an Int8 deque.

	void bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir);
		/// Binds an Int8 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::Int8> >& val, Direction dir);
		/// Binds an Int8 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::Int8> >& val, Direction dir);
		/// Binds an Int8 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::Int8> >& val, Direction dir);
		/// Binds an Int8 list.

	void bind(std::size_t pos, const Poco::UInt8& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt8.

	void bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir);
		/// Binds an UInt8 vector.

	void bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir);
		/// Binds an UInt8 deque.

	void bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir);
		/// Binds an UInt8 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt8> >& val, Direction dir);
		/// Binds an UInt8 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt8> >& val, Direction dir);
		/// Binds an UInt8 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::UInt8> >& val, Direction dir);
		/// Binds an UInt8 list.

	void bind(std::size_t pos, const Poco::Int16& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int16.
	
	void bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir);
		/// Binds an Int16 vector.

	void bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir);
		/// Binds an Int16 deque.

	void bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir);
		/// Binds an Int16 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::Int16> >& val, Direction dir);
		/// Binds an Int16 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::Int16> >& val, Direction dir);
		/// Binds an Int16 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::Int16> >& val, Direction dir);
		/// Binds an Int16 list.

	void bind(std::size_t pos, const Poco::UInt16& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt16.

	void bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir);
		/// Binds an UInt16 vector.

	void bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir);
		/// Binds an UInt16 deque.

	void bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir);
		/// Binds an UInt16 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt16> >& val, Direction dir);
		/// Binds an UInt16 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt16> >& val, Direction dir);
		/// Binds an UInt16 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::UInt16> >& val, Direction dir);
		/// Binds an UInt16 list.

	void bind(std::size_t pos, const Poco::Int32& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int32.

	void bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir);
		/// Binds an Int32 vector.

	void bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir);
		/// Binds an Int32 deque.

	void bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir);
		/// Binds an Int32 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::Int32> >& val, Direction dir);
		/// Binds an Int32 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::Int32> >& val, Direction dir);
		/// Binds an Int32 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::Int32> >& val, Direction dir);
		/// Binds an Int32 list.

	void bind(std::size_t pos, const Poco::UInt32& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt32.

	void bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir);
		/// Binds an UInt32 vector.

	void bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir);
		/// Binds an UInt32 deque.

	void bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir);
		/// Binds an UInt32 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt32> >& val, Direction dir);
		/// Binds an UInt32 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt32> >& val, Direction dir);
		/// Binds an UInt32 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::UInt32> >& val, Direction dir);
		/// Binds an UInt32 list.

	void bind(std::size_t pos, const Poco::Int64& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int64.

	void bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir);
		/// Binds an Int64 vector.

	void bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir);
		/// Binds an Int64 deque.

	void bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir);
		/// Binds an Int64 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::Int64> >& val, Direction dir);
		/// Binds an Int64 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::Int64> >& val, Direction dir);
		/// Binds an Int64 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::Int64> >& val, Direction dir);
		/// Binds an Int64 list.

	void bind(std::size_t pos, const Poco::UInt64& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt64.

	void bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir);
		/// Binds an UInt64 vector.

	void bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir);
		/// Binds an UInt64 deque.

	void bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir);
		/// Binds an UInt64 list.

	void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt64> >& val, Direction dir);
		/// Binds an UInt64 vector.

	void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt64> >& val, Direction dir);
		/// Binds an UInt64 deque.

	void bind(std::size_t pos, const std::list<Nullable<Poco::UInt64> >& val, Direction dir);
		/// Binds an UInt64 list.

#ifndef POCO_LONG_IS_64_BIT
	void bind(std::size_t pos, const long& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a long.

	void bind(std::size_t pos, const unsigned long& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an unsigned long.

	void bind(std::size_t pos, const std::vector<long>& val, Direction dir);
		/// Binds a long vector.

	void bind(std::size_t pos, const std::deque<long>& val, Direction dir);
		/// Binds a long deque.

	void bind(std::size_t pos, const std::list<long>& val, Direction dir);
		/// Binds a long list.

	void bind(std::size_t pos, const std::vector<Nullable<long> >& val, Direction dir);
		/// Binds a long vector.

	void bind(std::size_t pos, const std::deque<Nullable<long> >& val, Direction dir);
		/// Binds a long deque.

	void bind(std::size_t pos, const std::list<Nullable<long> >& val, Direction dir);
		/// Binds a long list.
#endif

	void bind(std::size_t pos, const bool& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a boolean.

	void bind(std::size_t pos, const std::vector<bool>& val, Direction dir);
		/// Binds a boolean vector.

	void bind(std::size_t pos, const std::deque<bool>& val, Direction dir);
		/// Binds a boolean deque.

	void bind(std::size_t pos, const std::list<bool>& val, Direction dir);
		/// Binds a boolean list.

	void bind(std::size_t pos, const std::vector<Nullable<bool> >& val, Direction dir);
		/// Binds a boolean vector.

	void bind(std::size_t pos, const std::deque<Nullable<bool> >& val, Direction dir);
		/// Binds a boolean deque.

	void bind(std::size_t pos, const std::list<Nullable<bool> >& val, Direction dir);
		/// Binds a boolean list.

	void bind(std::size_t pos, const float& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a float.

	void bind(std::size_t pos, const std::vector<float>& val, Direction dir);
		/// Binds a float vector.

	void bind(std::size_t pos, const std::deque<float>& val, Direction dir);
		/// Binds a float deque.

	void bind(std::size_t pos, const std::list<float>& val, Direction dir);
		/// Binds a float list.

	void bind(std::size_t pos, const std::vector<Nullable<float> >& val, Direction dir);
		/// Binds a float vector.

	void bind(std::size_t pos, const std::deque<Nullable<float> >& val, Direction dir);
		/// Binds a float deque.

	void bind(std::size_t pos, const std::list<Nullable<float> >& val, Direction dir);
		/// Binds a float list.

	void bind(std::size_t pos, const std::vector<Nullable<double> >& val, Direction dir);
		/// Binds a double vector.

	void bind(std::size_t pos, const std::deque<Nullable<double> >& val, Direction dir);
		/// Binds a double deque.

	void bind(std::size_t pos, const std::list<Nullable<double> >& val, Direction dir);
		/// Binds a double list.

	void bind(std::size_t pos, const double& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a double.

	void bind(std::size_t pos, const std::vector<double>& val, Direction dir);
		/// Binds a double vector.

	void bind(std::size_t pos, const std::deque<double>& val, Direction dir);
		/// Binds a double deque.

	void bind(std::size_t pos, const std::list<double>& val, Direction dir);
		/// Binds a double list.

	void bind(std::size_t pos, const char& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a single character.

	void bind(std::size_t pos, const std::vector<char>& val, Direction dir);
		/// Binds a character vector.

	void bind(std::size_t pos, const std::deque<char>& val, Direction dir);
		/// Binds a character deque.

	void bind(std::size_t pos, const std::list<char>& val, Direction dir);
		/// Binds a character list.

	void bind(std::size_t pos, const std::vector<Nullable<char> >& val, Direction dir);
		/// Binds a character vector.

	void bind(std::size_t pos, const std::deque<Nullable<char> >& val, Direction dir);
		/// Binds a character deque.

	void bind(std::size_t pos, const std::list<Nullable<char> >& val, Direction dir);
		/// Binds a character list.

	void bind(std::size_t pos, const std::string& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a string.

	void bind(std::size_t pos, const std::vector<std::string>& val, Direction dir);
		/// Binds a string vector.

	void bind(std::size_t pos, const std::deque<std::string>& val, Direction dir);
		/// Binds a string deque.

	void bind(std::size_t pos, const std::list<std::string>& val, Direction dir);
		/// Binds a string list.

	void bind(std::size_t pos, const std::vector<Nullable<std::string> >& val, Direction dir);
		/// Binds a string vector.

	void bind(std::size_t pos, const std::deque<Nullable<std::string> >& val, Direction dir);
		/// Binds a string deque.

	void bind(std::size_t pos, const std::list<Nullable<std::string> >& val, Direction dir);
		/// Binds a string list.

	void bind(std::size_t pos, const UTF16String& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a UTF-16 Unicode string.

	void bind(std::size_t pos, const std::vector<UTF16String>& val, Direction dir);
		/// Binds a UTF-16 Unicode string vector.

	void bind(std::size_t pos, const std::deque<UTF16String>& val, Direction dir);
		/// Binds a UTF-16 Unicode string deque.

	void bind(std::size_t pos, const std::list<UTF16String>& val, Direction dir);
		/// Binds a UTF-16 Unicode string list.

	void bind(std::size_t pos, const std::vector<Nullable<UTF16String> >& val, Direction dir);
		/// Binds a UTF-16 Unicode string vector.

	void bind(std::size_t pos, const std::deque<Nullable<UTF16String> >& val, Direction dir);
		/// Binds a UTF-16 Unicode string deque.

	void bind(std::size_t pos, const std::list<Nullable<UTF16String> >& val, Direction dir);
		/// Binds a UTF-16 Unicode string list.

	void bind(std::size_t pos, const BLOB& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a BLOB. In-bound only.

	void bind(std::size_t pos, const CLOB& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a CLOB. In-bound only.

	void bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir);
		/// Binds a BLOB vector.

	void bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir);
		/// Binds a BLOB deque.

	void bind(std::size_t pos, const std::list<BLOB>& val, Direction dir);
		/// Binds a BLOB list.

	void bind(std::size_t pos, const std::vector<Nullable<BLOB> >& val, Direction dir);
		/// Binds a BLOB vector.

	void bind(std::size_t pos, const std::deque<Nullable<BLOB> >& val, Direction dir);
		/// Binds a BLOB deque.

	void bind(std::size_t pos, const std::list<Nullable<BLOB> >& val, Direction dir);
		/// Binds a BLOB list.

	void bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir);
		/// Binds a CLOB vector.

	void bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir);
		/// Binds a CLOB deque.

	void bind(std::size_t pos, const std::list<CLOB>& val, Direction dir);
		/// Binds a CLOB list.

	void bind(std::size_t pos, const std::vector<Nullable<CLOB> >& val, Direction dir);
		/// Binds a CLOB vector.

	void bind(std::size_t pos, const std::deque<Nullable<CLOB> >& val, Direction dir);
		/// Binds a CLOB deque.

	void bind(std::size_t pos, const std::list<Nullable<CLOB> >& val, Direction dir);
		/// Binds a CLOB list.

	void bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a Date.

	void bind(std::size_t pos, const std::vector<Date>& val, Direction dir);
		/// Binds a Date vector.

	void bind(std::size_t pos, const std::deque<Date>& val, Direction dir);
		/// Binds a Date deque.

	void bind(std::size_t pos, const std::list<Date>& val, Direction dir);
		/// Binds a Date list.

	void bind(std::size_t pos, const std::vector<Nullable<Date> >& val, Direction dir);
		/// Binds a Date vector.

	void bind(std::size_t pos, const std::deque<Nullable<Date> >& val, Direction dir);
		/// Binds a Date deque.

	void bind(std::size_t pos, const std::list<Nullable<Date> >& val, Direction dir);
		/// Binds a Date list.

	void bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a Time.

	void bind(std::size_t pos, const std::vector<Time>& val, Direction dir);
		/// Binds a Time vector.

	void bind(std::size_t pos, const std::deque<Time>& val, Direction dir);
		/// Binds a Time deque.

	void bind(std::size_t pos, const std::list<Time>& val, Direction dir);
		/// Binds a Time list.

	void bind(std::size_t pos, const std::vector<Nullable<Time> >& val, Direction dir);
		/// Binds a Time vector.

	void bind(std::size_t pos, const std::deque<Nullable<Time> >& val, Direction dir);
		/// Binds a Time deque.

	void bind(std::size_t pos, const std::list<Nullable<Time> >& val, Direction dir);
		/// Binds a Time list.

	void bind(std::size_t pos, const DateTime& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a DateTime.

	void bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir);
		/// Binds a DateTime vector.

	void bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir);
		/// Binds a DateTime deque.

	void bind(std::size_t pos, const std::list<DateTime>& val, Direction dir);
		/// Binds a DateTime list.

	void bind(std::size_t pos, const std::vector<Nullable<DateTime> >& val, Direction dir);
		/// Binds a DateTime vector.

	void bind(std::size_t pos, const std::deque<Nullable<DateTime> >& val, Direction dir);
		/// Binds a DateTime deque.

	void bind(std::size_t pos, const std::list<Nullable<DateTime> >& val, Direction dir);
		/// Binds a DateTime list.

	void bind(std::size_t pos, const NullData& val, Direction dir, const std::type_info& bindType);
		/// Binds a null. In-bound only.

	void bind(std::size_t pos, const std::vector<NullData>& val, Direction dir, const std::type_info& bindType);
		/// Binds a null vector.

	void bind(std::size_t pos, const std::deque<NullData>& val, Direction dir, const std::type_info& bindType);
		/// Binds a null deque.

	void bind(std::size_t pos, const std::list<NullData>& val, Direction dir, const std::type_info& bindType);
		/// Binds a null list.

	void setDataBinding(ParameterBinding binding);
		/// Set data binding type.

	ParameterBinding getDataBinding() const;
		/// Return data binding type.

	std::size_t parameterSize(SQLPOINTER pAddr) const;
		/// Returns bound data size for parameter at specified position.

	void synchronize();
		/// Transfers the results of non-POD outbound parameters from internal
		/// holders back into the externally supplied buffers.

	void reset();
		/// Clears the cached storage.

private:
	typedef std::vector<ParamDescriptor>                     ParameterInfoVec;
	typedef std::vector<SQLLEN*>                             LengthPtrVec;
	typedef std::vector<SQLLEN>                              LengthVec;
	typedef std::vector<LengthVec*>                          LengthVecVec;
	typedef std::vector<char*>                               CharPtrVec;
	typedef std::vector<UTF16Char*>                          UTF16CharPtrVec;
	typedef std::vector<bool*>                               BoolPtrVec;
	typedef std::vector<SQL_DATE_STRUCT>                     DateVec;
	typedef std::vector<DateVec*>                            DateVecVec;
	typedef std::vector<SQL_TIME_STRUCT>                     TimeVec;
	typedef std::vector<TimeVec*>                            TimeVecVec;
	typedef std::vector<SQL_TIMESTAMP_STRUCT>                DateTimeVec;
	typedef std::vector<DateTimeVec*>                        DateTimeVecVec;
	typedef std::vector<Poco::Any*>                          AnyPtrVec;
	typedef std::vector<AnyPtrVec>                           AnyPtrVecVec;
	typedef std::map<char*, std::string*>                    StringMap;
	typedef std::map<UTF16String::value_type*, UTF16String*> UTF16StringMap;
	typedef std::map<SQL_DATE_STRUCT*, Date*>                DateMap;
	typedef std::map<SQL_TIME_STRUCT*, Time*>                TimeMap;
	typedef std::map<SQL_TIMESTAMP_STRUCT*, DateTime*>       TimestampMap;
	typedef std::map<SQLLEN*, WhenNullCb>                    NullCbMap;

	void describeParameter(std::size_t pos);
		/// Sets the description field for the parameter, if needed.

	void bind(std::size_t pos, const char* const& pVal, Direction dir, const WhenNullCb& nullCb);
		/// Binds a const char ptr.
		/// This is a private no-op in this implementation
		/// due to security risk.

	SQLSMALLINT toODBCDirection(Direction dir) const;
		/// Returns ODBC parameter direction based on the parameter binding direction
		/// specified by user.

	template <typename T>
	void bindImpl(std::size_t pos, T& val, SQLSMALLINT cDataType, Direction dir, const WhenNullCb& nullCb)
	{
		SQLINTEGER colSize = 0;
		SQLSMALLINT decDigits = 0;
		getColSizeAndPrecision(pos, cDataType, colSize, decDigits);
		SQLLEN* pLenIn = NULL;
		if (isOutBound(dir) && nullCb.defined())
		{
			pLenIn = new SQLLEN;
			*pLenIn = SQL_NTS; // microsoft example does that, otherwise no null indicator is returned
			_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));
		}
		_lengthIndicator.push_back(pLenIn);

		if (Utility::isError(SQLBindParameter(_rStmt,
			(SQLUSMALLINT) (pos + 1),
			toODBCDirection(dir),
			cDataType,
			static_cast<SQLSMALLINT>(Utility::sqlDataType(cDataType)),
			colSize,
			decDigits,
			(SQLPOINTER)&val, 0,
			_lengthIndicator.back())))
		{
			throw StatementException(_rStmt, "SQLBindParameter()");
		}
	}

	template <typename L>
	void bindImplLOB(std::size_t pos, const L& val, Direction dir, const WhenNullCb& nullCb)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("LOB parameter type can only be inbound.");

		SQLPOINTER pVal = (SQLPOINTER) val.rawContent();
		SQLINTEGER size = (SQLINTEGER) val.size();
			
		_inParams.insert(ParamMap::value_type(pVal, size));

		SQLLEN* pLenIn = new SQLLEN;
		*pLenIn  = size;

		if (PB_AT_EXEC == _paramBinding)
			*pLenIn  = SQL_LEN_DATA_AT_EXEC(size);

		if (isOutBound(dir) && nullCb.defined())
			_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));

		_lengthIndicator.push_back(pLenIn);
		SQLSMALLINT sqlType = (isInBound(dir) && size <= _maxVarBinColSize) ? SQL_VARBINARY : SQL_LONGVARBINARY;

		if (Utility::isError(SQLBindParameter(_rStmt,
			(SQLUSMALLINT) pos + 1,
			SQL_PARAM_INPUT,
			SQL_C_BINARY,
			sqlType,
			(SQLUINTEGER) size,
			0,
			pVal,
			(SQLINTEGER) size,
			_lengthIndicator.back())))
		{
			throw StatementException(_rStmt, "SQLBindParameter(LOB)");
		}
	}

	template<typename T>
	size_t getValueSize(const T& val)
	{
		return 0;
	}

	size_t getValueSize(const std::string& val)
	{
		return val.length();
	}

	size_t getValueSize(const UTF16String& val)
	{
		return val.length() * sizeof(UTF16Char);
	}
	
	template <typename T>
	size_t getValueSize(const LOB<T>& val)
	{
		return val.size();
	}

	template<typename T>
	size_t getValueSize(const Nullable<T>& val)
	{
		if (val.isNull())
			return 0;
			
		return getValueSize(val.value());
	}
	
	template <typename T>
	SQLLEN getLengthIndicator(const T& val)
	{
		return getValueSize(val);
	}

	SQLLEN getLengthIndicator(const NullData& val)
	{
		return SQL_NULL_DATA;
	}

	template <typename T>
	SQLLEN getLengthIndicator(const Nullable<T>& val)
	{
		if (val.isNull())
			return SQL_NULL_DATA;
		else
			return getLengthIndicator(val.value());
	}
	
	template <typename C>
	void initLengthIndicator(std::size_t pos, const C& val, size_t length)
	{
		setParamSetSize(length);

		if (_vecLengthIndicator.size() <= pos)
		{
			_vecLengthIndicator.resize(pos + 1);
			LengthVec* lenVec = new LengthVec(length);
			_vecLengthIndicator[pos] = lenVec;
			LengthVec::iterator itLen = lenVec->begin();
			for (typename C::const_iterator it = val.begin(); it != val.end(); ++it, ++itLen)
				*itLen = getLengthIndicator(*it);
		}
	}

	template <typename T>
	bool isVariableSizeType(const T*)
	{
		return false;
	}
	
	bool isVariableSizeType(const std::string*)
	{
		return true;
	}
	
	bool isVariableSizeType(const UTF16String*)
	{
		return true;
	}
	
	template <typename T>
	bool isVariableSizeType(const LOB<T>*)
	{
		return true;
	}
	
	template <typename T>
	bool isVariableSizeType(const Nullable<T>*)
	{
		return isVariableSizeType((T*)nullptr);
	}
	
	template <typename C>
	void getColSizeAndPrecision(std::size_t pos, 
		const C& val,
		SQLSMALLINT cDataType, 
		SQLINTEGER& colSize, 
		SQLSMALLINT& decDigits,
		SQLINTEGER& bufSize)
	{
		if (isVariableSizeType((typename C::value_type*)nullptr))
		{
			size_t maxSize = 0;
			for (typename C::const_iterator it = val.begin(); it != val.end(); ++it)
				maxSize = std::max(getValueSize(*it), maxSize);

			colSize = static_cast<SQLINTEGER>(maxSize);
			decDigits = 0;
			bufSize = static_cast<SQLINTEGER>(maxSize);
		}
		else
		{
			getColSizeAndPrecision(pos, cDataType, colSize, decDigits);
			bufSize = 0;
		}
	}
	
	template <typename T>
	void next(T& target, SQLINTEGER)
	{
		target++;
	}
	
	template <typename T>
	void next(T*& target, SQLINTEGER)
	{
		target++;
	}
	
	void next(char*& target, SQLINTEGER bufSize)
	{
		target += bufSize;
	}
	
	void next(UTF16Char*& target, SQLINTEGER bufSize)
	{
		target += bufSize / 2;
	}
	
	template <typename T>
	void copyValue(T& target, const T& val, SQLINTEGER)
	{
		target = val;
	}
	
	void copyValue(char& target, const std::string& str, SQLINTEGER bufSize)
	{
		std::size_t strSize = str.length();
		if (strSize > bufSize)
			throw LengthExceededException("copyValue(std::vector of strings)");
		std::memcpy(&target, str.c_str(), strSize);
	}

	void copyValue(UTF16Char& target, const UTF16String& str, SQLINTEGER bufSize)
	{
		std::size_t strSize = str.length();
		if (strSize > bufSize)
			throw LengthExceededException("copyValue(std::vector of strings)");
		std::memcpy(&target, str.c_str(), strSize * sizeof(UTF16Char));
	}

	template <typename T>
	void copyValue(char& target, const LOB<T>& lob, SQLINTEGER bufSize)
	{
		std::size_t lobSize = lob.size();
		if (lobSize > bufSize)
			throw LengthExceededException("copyValue(std::vector of LOB)");
		std::memcpy(&target, lob.rawContent(), lobSize * sizeof(typename LOB<T>::ValueType));
	}
	
	void copyValue(SQL_DATE_STRUCT& target, const Date& dt, SQLINTEGER)
	{
		Utility::dateSync(target, dt);
	}
	
	void copyValue(SQL_TIME_STRUCT& target, const Time& dt, SQLINTEGER)
	{
		Utility::timeSync(target, dt);
	}
	
	void copyValue(SQL_TIMESTAMP_STRUCT& target, const DateTime& dt, SQLINTEGER)
	{
		Utility::dateTimeSync(target, dt);
	}

	template <typename T, typename C>
	std::vector<T>* allocBuffer(size_t pos, size_t bufSize, const C& val, const T*)
	{
		if (_containers.size() <= pos)
			_containers.resize(pos + 1);
		
		_containers[pos].push_back(new Any(std::vector<T>(val.size())));
		std::vector<T>& cont = RefAnyCast<std::vector<T> >(*_containers[pos].back());
		return &cont;
	}
	
	template <typename C>
	char* allocBuffer(size_t pos, size_t bufSize, const C& val, const std::string*)
	{
		if (_charPtrs.size() <= pos)
			_charPtrs.resize(pos + 1, 0);

		_charPtrs[pos] = (char*)std::calloc(val.size() * bufSize, sizeof(char));
		return _charPtrs[pos];
	}
	
	template <typename C>
	UTF16Char* allocBuffer(size_t pos, size_t bufSize, const C& val, const UTF16String*)
	{
		if (_utf16CharPtrs.size() <= pos)
			_utf16CharPtrs.resize(pos + 1, 0);

		_utf16CharPtrs[pos] = (UTF16Char*)std::calloc(val.size() * bufSize, sizeof(UTF16Char));
		return _utf16CharPtrs[pos];
	}
	
	template <typename T, typename C>
	char* allocBuffer(size_t pos, size_t bufSize, const C& val, const LOB<T>*)
	{
		if (_charPtrs.size() <= pos)
			_charPtrs.resize(pos + 1, 0);

		_charPtrs[pos] = (char*)std::calloc(val.size() * bufSize, sizeof(typename LOB<T>::ValueType));
		return _charPtrs[pos];
	}
	
	template <typename C>
	DateVec* allocBuffer(size_t pos, size_t bufSize, const C& val, const Date*)
	{
		if (_dateVecVec.size() <= pos)
			_dateVecVec.resize(pos + 1, 0);

		_dateVecVec[pos] = new DateVec(val.size());
		return _dateVecVec[pos];
	}
	
	template <typename C>
	TimeVec* allocBuffer(size_t pos, size_t bufSize, const C& val, const Time*)
	{
		if (_timeVecVec.size() <= pos)
			_timeVecVec.resize(pos + 1, 0);

		_timeVecVec[pos] = new TimeVec(val.size());
		return _timeVecVec[pos];
	}
	
	template <typename C>
	DateTimeVec* allocBuffer(size_t pos, size_t bufSize, const C& val, const DateTime*)
	{
		if (_dateTimeVecVec.size() <= pos)
			_dateTimeVecVec.resize(pos + 1, 0);

		_dateTimeVecVec[pos] = new DateTimeVec(val.size());
		return _dateTimeVecVec[pos];
	}

	template <typename C>
	bool* allocBuffer(size_t pos, size_t bufSize, const C& val, const bool*)
	{
		if (_boolPtrs.size() <= pos)
			_boolPtrs.resize(pos + 1);

		_boolPtrs[pos] = new bool[val.size()];
		return _boolPtrs[pos];
	}
	
	template <typename T, typename C>
	auto* allocBuffer(size_t pos, size_t bufSize, const C& val, const Nullable<T>* dummy)
	{
		return allocBuffer(pos, bufSize, val, (const T*)nullptr);
	}

	template <typename C, typename OUTPUT, typename T>
	void copyContainerValues(OUTPUT out, size_t bufSize, const C& val, const T*)
	{
		for (typename C::const_iterator it = val.begin(); it != val.end(); ++it)
		{
			copyValue(*out, *it, static_cast<SQLINTEGER>(bufSize));
			next(out, static_cast<SQLINTEGER>(bufSize));
		}
	}

	template <typename C, typename OUTPUT, typename T>
	void copyContainerValues(OUTPUT out, size_t bufSize, const C& val, const Nullable<T>*)
	{
		for (typename C::const_iterator it = val.begin(); it != val.end(); ++it)
		{
			if (!it->isNull())
				copyValue(*out, it->value(), static_cast<SQLINTEGER>(bufSize));
			next(out, static_cast<SQLINTEGER>(bufSize));
		}
	}
	
	template <typename T>
	typename std::vector<T>::iterator getOutput(std::vector<T>* cont)
	{
		return cont->begin();
	}
	
	template <typename T>
	T* getOutput(T* cont)
	{
		return cont;
	}
	
	template <typename T>
	SQLPOINTER getContainerPointer(const std::vector<T>* cont)
	{
		return (SQLPOINTER)&((*cont)[0]);
	}
	
	template <typename T>
	SQLPOINTER getContainerPointer(const T* cont)
	{
		return (SQLPOINTER)cont;
	}
	
	template <typename C>
	SQLPOINTER copyContainer(size_t pos, size_t bufSize, const C& val)
	{
		auto* cont = allocBuffer(pos, bufSize, val, (const typename C::value_type*)nullptr);
		copyContainerValues(getOutput(cont), bufSize, val, (const typename C::value_type*)nullptr);
		return getContainerPointer(cont);
	}
	
	SQLPOINTER copyContainer(size_t pos, size_t bufSize, const std::vector<bool>& val)
	{
		bool* cont = allocBuffer(pos, bufSize, val, (const bool*)nullptr);
		copyContainerValues(getOutput(cont), bufSize, val, (const bool*)nullptr);
		return cont;
	}

	template <typename T>
	bool isNativeType(const T*)
	{
		return true;
	}
	
	bool isNativeType(const Date*)
	{
		return false;
	}
	
	bool isNativeType(const Time*)
	{
		return false;
	}
	
	bool isNativeType(const DateTime*)
	{
		return false;
	}
	
	template <typename T>
	SQLPOINTER copyContainer(size_t pos, size_t bufSize, const std::vector<T>& val)
	{
		if (isVariableSizeType((const T*)nullptr) || !isNativeType((const T*)nullptr))
			return copyContainer<std::vector<T>>(pos, bufSize, val);
		else
			return (SQLPOINTER)&val[0];
	}
	
	template <typename T>
	SQLPOINTER copyContainer(size_t pos, size_t bufSize, const std::vector<Nullable<T>>& val)
	{
		auto cont = allocBuffer(pos, bufSize, val, (const Nullable<T>*)nullptr);
		copyContainerValues(getOutput(cont), bufSize, val, (const Nullable<T>*)nullptr);
		return getContainerPointer(cont);
	}
	
	template <typename T>
	SQLSMALLINT getSqlType(const T*, SQLSMALLINT cDataType, size_t colSize)
	{
		return Utility::sqlDataType(cDataType);
	}
	
	SQLSMALLINT getSqlType(const std::string*, SQLSMALLINT cDataType, size_t colSize)
	{
		if (colSize > _maxCharColLength)
			return SQL_LONGVARCHAR;
		return SQL_VARCHAR;
	}
	
	SQLSMALLINT getSqlType(const UTF16String*, SQLSMALLINT cDataType, size_t colSize)
	{
		if (colSize > _maxWCharColLength)
			return SQL_WLONGVARCHAR;
		return SQL_WVARCHAR;
	}
	
	template <typename T>
	SQLSMALLINT getSqlType(const LOB<T>*, SQLSMALLINT cDataType, size_t colSize)
	{
		if (colSize > _maxVarBinColSize)
			return SQL_LONGVARBINARY;
		return SQL_VARBINARY;
	}
	
	template <typename T>
	SQLSMALLINT getSqlType(const Nullable<T>* val, SQLSMALLINT cDataType, size_t colSize)
	{
		return getSqlType((T*)nullptr, cDataType, colSize);
	}
	
	template <typename T>
	void validateDirection(Direction, const T*)
	{
	}
	
	void validateDirection(Direction dir, const std::string*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("String container parameter type can only be inbound.");
	}
	
	void validateDirection(Direction dir, const UTF16String*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("UTF16String container parameter type can only be inbound.");
	}
	
	template <typename T>
	void validateDirection(Direction dir, const LOB<T>*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("LOB container parameter type can only be inbound.");
	}
	
	void validateDirection(Direction dir, const Date*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("Date container parameter type can only be inbound.");
	}
	
	void validateDirection(Direction dir, const Time*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("Time container parameter type can only be inbound.");
	}
	
	void validateDirection(Direction dir, const DateTime*)
	{
		if (isOutBound(dir) || !isInBound(dir))
			throw NotImplementedException("DateTime container parameter type can only be inbound.");
	}
	
	template <typename T>
	void validateDirection(Direction dir, const Nullable<T>*)
	{
		validateDirection(dir, (T*)nullptr);
	}
	
	template <typename C>
	void bindImplContainer(std::size_t pos, const C& val, SQLSMALLINT cDataType, Direction dir)
	{
		if (val.empty())
			throw InvalidArgumentException("Empty container not allowed.");

		if (PB_IMMEDIATE != _paramBinding)
			throw InvalidAccessException("container can only be bound immediately.");

		validateDirection(dir, (typename C::value_type*)nullptr);
		
		const size_t length = val.size();
		initLengthIndicator(pos, val, length);
		
		SQLINTEGER colSize;
		SQLINTEGER bufSize;
		SQLSMALLINT decDigits;
		getColSizeAndPrecision(pos, val, cDataType, colSize, decDigits, bufSize);
		SQLPOINTER cont = copyContainer(pos, bufSize, val);

		if (Utility::isError(SQLBindParameter(_rStmt, 
			(SQLUSMALLINT) pos + 1, 
			toODBCDirection(dir),
			cDataType,
			getSqlType((typename C::value_type*)NULL, cDataType, colSize),
			colSize,
			decDigits,
			cont,
			bufSize,
			&(*_vecLengthIndicator[pos])[0])))
		{
			throw StatementException(_rStmt, "SQLBindParameter()");
		}
	}

	void getColSizeAndPrecision(std::size_t pos,
		SQLSMALLINT cDataType,
		SQLINTEGER& colSize,
		SQLSMALLINT& decDigits,
		std::size_t actualSize = 0);
		/// Used to retrieve column size and precision.
		/// Not all drivers cooperate with this inquiry under all circumstances
		/// This function runs for query and stored procedure parameters (in and
		/// out-bound). Some drivers, however, do not care about knowing this
		/// information to start with. For that reason, after all the attempts
		/// to discover the required values are unsuccessfully exhausted, the values
		/// are both set to zero and no exception is thrown.
		/// However, if the colSize is succesfully retrieved and it is greater than
		/// session-wide maximum allowed field size, LengthExceededException is thrown.

	void setParamSetSize(std::size_t length);
		/// Sets the parameter set size. Used for column-wise binding.

	void getColumnOrParameterSize(std::size_t pos, SQLINTEGER& size);
		/// Fills the column or parameter size into the 'size' argument.
		/// Does nothing if neither can be obtained from the driver, so
		/// size should be set to some default value prior to calling this
		/// function in order to avoid undefined size value.

	void freeMemory();
		/// Frees all dynamically allocated memory resources.

	const StatementHandle& _rStmt;

	LengthPtrVec     _lengthIndicator;
	LengthVecVec     _vecLengthIndicator;

	ParamMap         _inParams;
	ParamMap         _outParams;
	ParameterBinding _paramBinding;
	ParameterInfoVec _parameters;
	
	DateMap          _dates;
	TimeMap          _times;
	TimestampMap     _timestamps;
	StringMap        _strings;
	UTF16StringMap   _utf16Strings;

	DateVecVec       _dateVecVec;
	TimeVecVec       _timeVecVec;
	DateTimeVecVec   _dateTimeVecVec;
	CharPtrVec       _charPtrs;
	UTF16CharPtrVec  _utf16CharPtrs;
	BoolPtrVec       _boolPtrs;
	const TypeInfo*  _pTypeInfo;
	SQLINTEGER       _paramSetSize;
	std::size_t      _maxFieldSize;
	AnyPtrVecVec     _containers;
	std::size_t      _maxCharColLength;
	std::size_t      _maxWCharColLength;
	std::size_t      _maxVarBinColSize;
	NullCbMap        _nullCbMap;
	bool             _insertOnly;
};


//
// inlines
//
inline void Binder::bind(std::size_t pos, const Poco::Int8& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_STINYINT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::Int8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::Int8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::Int8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt8& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_UTINYINT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::UInt8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::UInt8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::UInt8> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UTINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::Int16& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_SSHORT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::Int16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::Int16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::Int16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SSHORT, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt16& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_USHORT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::UInt16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::UInt16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::UInt16> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_USHORT, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::Int32& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_SLONG, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::Int32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::Int32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::Int32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt32& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_ULONG, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::UInt32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::UInt32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::UInt32> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_ULONG, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::Int64& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_SBIGINT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::Int64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::Int64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::Int64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt64& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_UBIGINT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Poco::UInt64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Poco::UInt64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Poco::UInt64> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_UBIGINT, dir);
}


#ifndef POCO_LONG_IS_64_BIT
inline void Binder::bind(std::size_t pos, const long& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_SLONG, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const unsigned long& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_SLONG, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<long>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<long>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<long>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<long> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<long> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<long> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_SLONG, dir);
}
#endif


inline void Binder::bind(std::size_t pos, const float& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_FLOAT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<float>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<float>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<float>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<float> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<float> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<float> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_FLOAT, dir);
}


inline void Binder::bind(std::size_t pos, const double& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_DOUBLE, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<double>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<double>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<double>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<double> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<double> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<double> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_DOUBLE, dir);
}


inline void Binder::bind(std::size_t pos, const bool& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_BIT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<bool>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<bool>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<bool>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<bool> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<bool> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<bool> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BIT, dir);
}


inline void Binder::bind(std::size_t pos, const char& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImpl(pos, val, SQL_C_STINYINT, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<char>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<char>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<char>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<char> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<char> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<char> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<std::string>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<std::string>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<std::string>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<std::string> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<std::string> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<std::string> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_CHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<UTF16String>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<UTF16String>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<UTF16String>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<UTF16String> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<UTF16String> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<UTF16String> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_WCHAR, dir);
}


inline void Binder::bind(std::size_t pos, const BLOB& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImplLOB<BLOB>(pos, val, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const CLOB& val, Direction dir, const WhenNullCb& nullCb)
{
	bindImplLOB<CLOB>(pos, val, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<BLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<BLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<BLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<BLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<CLOB>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<CLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<CLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<CLOB> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_BINARY, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Date>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Date>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Date>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Date> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Date> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Date> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_DATE, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Time>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Time>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Time>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<Time> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<Time> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<Time> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIME, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<DateTime>& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<Nullable<DateTime> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<Nullable<DateTime> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<Nullable<DateTime> >& val, Direction dir)
{
	bindImplContainer(pos, val, SQL_C_TYPE_TIMESTAMP, dir);
}


inline void Binder::bind(std::size_t pos, const std::vector<NullData>& val, Direction dir, const std::type_info& bindElemType)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::deque<NullData>& val, Direction dir, const std::type_info& bindElemType)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::bind(std::size_t pos, const std::list<NullData>& val, Direction dir, const std::type_info& bindElemType)
{
	bindImplContainer(pos, val, SQL_C_STINYINT, dir);
}


inline void Binder::setDataBinding(Binder::ParameterBinding binding)
{
	_paramBinding = binding;
}


inline Binder::ParameterBinding Binder::getDataBinding() const
{
	return _paramBinding;
}


} } } // namespace Poco::SQL::ODBC


#endif // Data_ODBC_Binder_INCLUDED
