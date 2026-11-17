//
// Preparator.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Preparator
//
// Definition of the Preparator class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_Preparator_INCLUDED
#define Data_ODBC_Preparator_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ODBCMetaColumn.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/AbstractPreparator.h"
#include "Poco/Data/Constants.h"
#include "Poco/Data/LOB.h"
#include "Poco/Types.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/DateTime.h"
#include "Poco/SharedPtr.h"
#include "Poco/UTFString.h"
#include <vector>
#include <limits>
#ifdef POCO_OS_FAMILY_WINDOWS
#include "Poco/UnWindows.h"
#endif
#include <sqlext.h>


namespace Poco::Data {


class Date;
class Time;


namespace ODBC {


class ODBC_API Preparator : public AbstractPreparator
	/// Class used for database preparation where we first have to register all data types
	/// with respective memory output locations before extracting data.
	/// Extraction works in two-phases: first prepare is called once, then extract n-times.
	/// In ODBC, SQLBindCol/SQLFetch is the preferred method of data retrieval (SQLGetData is available,
	/// however with numerous driver implementation dependent limitations and inferior performance).
	/// In order to fit this functionality into Poco DataConnectors framework, every ODBC SQL statement
	/// instantiates its own Preparator object.
	/// This is done once per statement execution (from StatementImpl::bindImpl()).
	///
	/// Preparator object is used to :
	///
	///   1) Prepare SQL statement.
	///   2) Provide and contain the memory locations where retrieved values are placed during recordset iteration.
	///   3) Keep count of returned number of columns with their respective datatypes and sizes.
	///
	/// Notes:
	///
	/// - Value datatypes in this interface prepare() calls serve only for the purpose of type distinction.
	/// - Preparator keeps its own std::vector<Any> buffer for fetched data to be later retrieved by Extractor.
	/// - prepare() methods should not be called when extraction mode is DE_MANUAL
	///
{
public:
	typedef std::vector<char*> CharArray;
	typedef SharedPtr<Preparator> Ptr;

	enum DataExtraction
	{
		DE_MANUAL,
		DE_BOUND
	};

	enum DataType
	{
		DT_BOOL,
		DT_BOOL_ARRAY,
		DT_CHAR,
		DT_WCHAR,
		DT_UCHAR,
		DT_CHAR_ARRAY,
		DT_WCHAR_ARRAY,
		DT_UCHAR_ARRAY,
		DT_DATE,
		DT_TIME,
		DT_DATETIME
	};

	Preparator(const StatementHandle& rStmt,
		const std::string& statement,
		std::size_t maxFieldSize,
		DataExtraction dataExtraction = DE_BOUND);
		/// Creates the Preparator.

	Preparator(const Preparator& other);
		/// Copy constructs the Preparator.

	~Preparator() override;
		/// Destroys the Preparator.

	void prepare(std::size_t pos, const Poco::Int8& val) override;
		/// Prepares an Int8.

	void prepare(std::size_t pos, const std::vector<Poco::Int8>& val) override;
		/// Prepares an Int8 vector.

	void prepare(std::size_t pos, const std::deque<Poco::Int8>& val) override;
		/// Prepares an Int8 deque.

	void prepare(std::size_t pos, const std::list<Poco::Int8>& val) override;
		/// Prepares an Int8 list.

	void prepare(std::size_t pos, const Poco::UInt8& val) override;
		/// Prepares an UInt8.

	void prepare(std::size_t pos, const std::vector<Poco::UInt8>& val) override;
		/// Prepares an UInt8 vector.

	void prepare(std::size_t pos, const std::deque<Poco::UInt8>& val) override;
		/// Prepares an UInt8 deque.

	void prepare(std::size_t pos, const std::list<Poco::UInt8>& val) override;
		/// Prepares an UInt8 list.

	void prepare(std::size_t pos, const Poco::Int16& val) override;
		/// Prepares an Int16.

	void prepare(std::size_t pos, const std::vector<Poco::Int16>& val) override;
		/// Prepares an Int16 vector.

	void prepare(std::size_t pos, const std::deque<Poco::Int16>& val) override;
		/// Prepares an Int16 deque.

	void prepare(std::size_t pos, const std::list<Poco::Int16>& val) override;
		/// Prepares an Int16 list.

	void prepare(std::size_t pos, const Poco::UInt16& val) override;
		/// Prepares an UInt16.

	void prepare(std::size_t pos, const std::vector<Poco::UInt16>& val) override;
		/// Prepares an UInt16 vector.

	void prepare(std::size_t pos, const std::deque<Poco::UInt16>& val) override;
		/// Prepares an UInt16 deque.

	void prepare(std::size_t pos, const std::list<Poco::UInt16>& val) override;
		/// Prepares an UInt16 list.

	void prepare(std::size_t pos, const Poco::Int32& val) override;
		/// Prepares an Int32.

	void prepare(std::size_t pos, const std::vector<Poco::Int32>& val) override;
		/// Prepares an Int32 vector.

	void prepare(std::size_t pos, const std::deque<Poco::Int32>& val) override;
		/// Prepares an Int32 deque.

	void prepare(std::size_t pos, const std::list<Poco::Int32>& val) override;
		/// Prepares an Int32 list.

	void prepare(std::size_t pos, const Poco::UInt32& val) override;
		/// Prepares an UInt32.

	void prepare(std::size_t pos, const std::vector<Poco::UInt32>& val) override;
		/// Prepares an UInt32 vector.

	void prepare(std::size_t pos, const std::deque<Poco::UInt32>& val) override;
		/// Prepares an UInt32 deque.

	void prepare(std::size_t pos, const std::list<Poco::UInt32>& val) override;
		/// Prepares an UInt32 list.

	void prepare(std::size_t pos, const Poco::Int64& val) override;
		/// Prepares an Int64.

	void prepare(std::size_t pos, const std::vector<Poco::Int64>& val) override;
		/// Prepares an Int64 vector.

	void prepare(std::size_t pos, const std::deque<Poco::Int64>& val) override;
		/// Prepares an Int64 deque.

	void prepare(std::size_t pos, const std::list<Poco::Int64>& val) override;
		/// Prepares an Int64 list.

	void prepare(std::size_t pos, const Poco::UInt64& val) override;
		/// Prepares an UInt64.

	void prepare(std::size_t pos, const std::vector<Poco::UInt64>& val) override;
		/// Prepares an UInt64 vector.

	void prepare(std::size_t pos, const std::deque<Poco::UInt64>& val) override;
		/// Prepares an UInt64 deque.

	void prepare(std::size_t pos, const std::list<Poco::UInt64>& val) override;
		/// Prepares an UInt64 list.

#ifndef POCO_INT64_IS_LONG
	void prepare(std::size_t pos, const long& val) override;
		/// Prepares a long.

	void prepare(std::size_t pos, const unsigned long& val) override;
		/// Prepares an unsigned long.

	void prepare(std::size_t pos, const std::vector<long>& val) override;
		/// Prepares a long vector.

	void prepare(std::size_t pos, const std::deque<long>& val) override;
		/// Prepares a long deque.

	void prepare(std::size_t pos, const std::list<long>& val) override;
		/// Prepares a long list.
#endif

	void prepare(std::size_t pos, const bool& val) override;
		/// Prepares a boolean.

	void prepare(std::size_t pos, const std::vector<bool>& val) override;
		/// Prepares a boolean vector.

	void prepare(std::size_t pos, const std::deque<bool>& val) override;
		/// Prepares a boolean deque.

	void prepare(std::size_t pos, const std::list<bool>& val) override;
		/// Prepares a boolean list.

	void prepare(std::size_t pos, const float& val) override;
		/// Prepares a float.

	void prepare(std::size_t pos, const std::vector<float>& val) override;
		/// Prepares a float vector.

	void prepare(std::size_t pos, const std::deque<float>& val) override;
		/// Prepares a float deque.

	void prepare(std::size_t pos, const std::list<float>& val) override;
		/// Prepares a float list.

	void prepare(std::size_t pos, const double& val) override;
		/// Prepares a double.

	void prepare(std::size_t pos, const std::vector<double>& val) override;
		/// Prepares a double vector.

	void prepare(std::size_t pos, const std::deque<double>& val) override;
		/// Prepares a double deque.

	void prepare(std::size_t pos, const std::list<double>& val) override;
		/// Prepares a double list.

	void prepare(std::size_t pos, const char& val) override;
		/// Prepares a single character.

	void prepare(std::size_t pos, const std::vector<char>& val) override;
		/// Prepares a single character vector.

	void prepare(std::size_t pos, const std::deque<char>& val) override;
		/// Prepares a single character deque.

	void prepare(std::size_t pos, const std::list<char>& val) override;
		/// Prepares a single character list.

	void prepare(std::size_t pos, const std::string& val) override;
		/// Prepares a string.

	void prepare(std::size_t pos, const std::vector<std::string>& val) override;
		/// Prepares a string vector.

	void prepare(std::size_t pos, const std::deque<std::string>& val) override;
		/// Prepares a string deque.

	void prepare(std::size_t pos, const std::list<std::string>& val) override;
		/// Prepares a string list.

	void prepare(std::size_t pos, const UTF16String& val) override;
	/// Prepares a string.

	void prepare(std::size_t pos, const std::vector<UTF16String>& val) override;
	/// Prepares a string vector.

	void prepare(std::size_t pos, const std::deque<UTF16String>& val) override;
	/// Prepares a string deque.

	void prepare(std::size_t pos, const std::list<UTF16String>& val) override;
	/// Prepares a string list.

	void prepare(std::size_t pos, const Poco::Data::BLOB& val) override;
		/// Prepares a BLOB.

	void prepare(std::size_t pos, const std::vector<Poco::Data::BLOB>& val) override;
		/// Prepares a BLOB vector.

	void prepare(std::size_t pos, const std::deque<Poco::Data::BLOB>& val) override;
		/// Prepares a BLOB deque.

	void prepare(std::size_t pos, const std::list<Poco::Data::BLOB>& val) override;
		/// Prepares a BLOB list.

	void prepare(std::size_t pos, const Poco::Data::CLOB& val) override;
		/// Prepares a CLOB.

	void prepare(std::size_t pos, const std::vector<Poco::Data::CLOB>& val) override;
		/// Prepares a CLOB vector.

	void prepare(std::size_t pos, const std::deque<Poco::Data::CLOB>& val) override;
		/// Prepares a CLOB deque.

	void prepare(std::size_t pos, const std::list<Poco::Data::CLOB>& val) override;
		/// Prepares a CLOB list.

	void prepare(std::size_t pos, const Poco::Data::Date& val) override;
		/// Prepares a Date.

	void prepare(std::size_t pos, const std::vector<Poco::Data::Date>& val) override;
		/// Prepares a Date vector.

	void prepare(std::size_t pos, const std::deque<Poco::Data::Date>& val) override;
		/// Prepares a Date deque.

	void prepare(std::size_t pos, const std::list<Poco::Data::Date>& val) override;
		/// Prepares a Date list.

	void prepare(std::size_t pos, const Poco::Data::Time& val) override;
		/// Prepares a Time.

	void prepare(std::size_t pos, const std::vector<Poco::Data::Time>& val) override;
		/// Prepares a Time vector.

	void prepare(std::size_t pos, const std::deque<Poco::Data::Time>& val) override;
		/// Prepares a Time deque.

	void prepare(std::size_t pos, const std::list<Poco::Data::Time>& val) override;
		/// Prepares a Time list.

	void prepare(std::size_t pos, const Poco::DateTime& val) override;
		/// Prepares a DateTime.

	void prepare(std::size_t pos, const std::vector<Poco::DateTime>& val) override;
		/// Prepares a DateTime vector.

	void prepare(std::size_t pos, const std::deque<Poco::DateTime>& val) override;
		/// Prepares a DateTime deque.

	void prepare(std::size_t pos, const std::list<Poco::DateTime>& val) override;
		/// Prepares a DateTime list.

	void prepare(std::size_t pos, const Poco::UUID& val) override;
		/// Prepares a UUID.

	void prepare(std::size_t pos, const Poco::Any& val) override;
		/// Prepares an Any.

	void prepare(std::size_t pos, const std::vector<Poco::Any>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into an Any vector is not supported.

	void prepare(std::size_t pos, const std::deque<Poco::Any>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into an Any deque is not supported.

	void prepare(std::size_t pos, const std::list<Poco::Any>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into an Any list is not supported.

	void prepare(std::size_t pos, const Poco::Dynamic::Var& val) override;
		/// Prepares a Dynamic::Var.

	void prepare(std::size_t pos, const std::vector<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into a Dynamic::Var vector is not supported.

	void prepare(std::size_t pos, const std::deque<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into a Dynamic::Var deque is not supported.

	void prepare(std::size_t pos, const std::list<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException: bulk extraction
		/// into a Dynamic::Var list is not supported.

	[[nodiscard]] std::size_t columns() const;
		/// Returns the number of columns.
		/// Resizes the internal storage iff the size is zero.
		/// Once the connection of the statement has been closed, returns the number known by then.

	[[nodiscard]] Poco::Any& operator [] (std::size_t pos);
		/// Returns reference to column data.

	[[nodiscard]] Poco::Any& at(std::size_t pos);
		/// Returns reference to column data.

	void setMaxFieldSize(std::size_t size);
		/// Sets maximum supported field size.

	[[nodiscard]] std::size_t getMaxFieldSize() const;
		// Returns maximum supported field size.

	[[nodiscard]] std::size_t maxDataSize(std::size_t pos) const;
		/// Returns max supported size for column at position pos.
		/// Returned length for variable length fields is the one
		/// supported by this implementation, not the underlying DB.

	[[nodiscard]] std::size_t actualDataSize(std::size_t col, std::size_t row = POCO_DATA_INVALID_ROW) const;
		/// Returns the returned length for the column and row specified.
		/// This is usually equal to the column size, except for
		/// variable length fields (BLOB and variable length strings).
		/// For null values, the return value is (std::size_t) SQL_NULL_DATA;
		/// if the driver could not determine the length of a truncated
		/// value, it is (std::size_t) SQL_NO_TOTAL.

	[[nodiscard]] std::size_t boundBufferSize(std::size_t pos) const;
		/// Returns the length in bytes of the buffer bound with SQLBindCol()
		/// to the variable length column at position pos (for bulk columns,
		/// the length of one row), or 0 if no variable length buffer is bound
		/// to the column. No value fetched into the buffer can be longer.
		/// Unlike maxDataSize(), the length is recorded when the buffer is
		/// bound and does not depend on the current column metadata.

	[[nodiscard]] MetaColumn::ColumnDataType boundType(std::size_t pos) const;
		/// For a column prepared for extraction into Poco::Any or
		/// Poco::Dynamic::Var, returns the column type the driver described
		/// when the column was prepared, which determines the type of the
		/// bound buffer. Returns MetaColumn::FDT_UNKNOWN for other columns.
		/// After a fetch, a driver can describe the column differently.

	[[nodiscard]] std::size_t bulkSize(std::size_t col = 0) const;
		/// Returns bulk size. Column argument is optional
		/// since all columns must be the same size.

	void setDataExtraction(DataExtraction ext);
		/// Set data extraction mode.

	[[nodiscard]] DataExtraction getDataExtraction() const;
		/// Returns data extraction mode.

private:
	typedef std::vector<Poco::Any> ValueVec;
	typedef std::vector<SQLLEN>    LengthVec;
	typedef std::vector<LengthVec> LengthLengthVec;
	typedef std::map<std::size_t, DataType> IndexMap;

	Preparator();
	Preparator& operator = (const Preparator&);

	void prepareImpl(std::size_t pos)
		/// Utility function to prepare Any and Dynamic::Var.
		/// Binds the buffer the typed prepare() binds for the column type
		/// the driver describes, and records that type for boundType().
	{
		ODBCMetaColumn col(_rStmt, pos);
		poco_assert (pos < _boundTypes.size());
		_boundTypes[pos] = col.type();

		switch (col.type())
		{
			case MetaColumn::FDT_INT8:
				return prepareFixedSize<Poco::Int8>(pos, SQL_C_STINYINT);

			case MetaColumn::FDT_UINT8:
				return prepareFixedSize<Poco::UInt8>(pos, SQL_C_UTINYINT);

			case MetaColumn::FDT_INT16:
				return prepareFixedSize<Poco::Int16>(pos, SQL_C_SSHORT);

			case MetaColumn::FDT_UINT16:
				return prepareFixedSize<Poco::UInt16>(pos, SQL_C_USHORT);

			case MetaColumn::FDT_INT32:
				return prepareFixedSize<Poco::Int32>(pos, SQL_C_SLONG);

			case MetaColumn::FDT_UINT32:
				return prepareFixedSize<Poco::UInt32>(pos, SQL_C_ULONG);

			case MetaColumn::FDT_INT64:
				return prepareFixedSize<Poco::Int64>(pos, SQL_C_SBIGINT);

			case MetaColumn::FDT_UINT64:
				return prepareFixedSize<Poco::UInt64>(pos, SQL_C_UBIGINT);

			case MetaColumn::FDT_BOOL:
				return prepareFixedSize<bool>(pos, SQL_C_BIT);

			case MetaColumn::FDT_FLOAT:
				return prepareFixedSize<float>(pos, SQL_C_FLOAT);

			case MetaColumn::FDT_DOUBLE:
				return prepareFixedSize<double>(pos, SQL_C_DOUBLE);

			case MetaColumn::FDT_STRING:
				return prepareVariableLen<char>(pos, SQL_C_CHAR, maxDataSize(pos), DT_CHAR);

			case MetaColumn::FDT_WSTRING:
			{
				typedef UTF16String::value_type CharType;
				return prepareVariableLen<CharType>(pos, SQL_C_WCHAR, maxDataSize(pos), DT_WCHAR);
			}

			case MetaColumn::FDT_BLOB:
			{
				typedef Poco::Data::BLOB::ValueType CharType;
				return prepareVariableLen<CharType>(pos, SQL_C_BINARY, maxDataSize(pos), DT_UCHAR);
			}

			case MetaColumn::FDT_CLOB:
			{
				typedef Poco::Data::CLOB::ValueType CharType;
				return prepareVariableLen<CharType>(pos, SQL_C_BINARY, maxDataSize(pos), DT_CHAR);
			}

			// as prepare() for Date, Time, DateTime and UUID: the extraction
			// of these types reads the ODBC structures and the 16-byte buffer
			case MetaColumn::FDT_DATE:
				return prepareFixedSize<SQL_DATE_STRUCT>(pos, SQL_C_TYPE_DATE);

			case MetaColumn::FDT_TIME:
				return prepareFixedSize<SQL_TIME_STRUCT>(pos, SQL_C_TYPE_TIME);

			case MetaColumn::FDT_TIMESTAMP:
				return prepareFixedSize<SQL_TIMESTAMP_STRUCT>(pos, SQL_C_TYPE_TIMESTAMP);

			case MetaColumn::FDT_UUID:
				return prepareVariableLen<char>(pos, SQL_C_BINARY, 16, DT_CHAR);

			default:
				throw DataFormatException("Unsupported data type.");
		}
	}

	void resize() const;
		/// Resize the values and lengths vectors.
		/// Does nothing once the connection of the statement has been closed.

	template <typename T>
	void prepareFixedSize(std::size_t pos, SQLSMALLINT valueType)
		/// Utility function for preparation of fixed length columns.
	{
		poco_assert (DE_BOUND == _dataExtraction);
		std::size_t dataSize = sizeof(T);

		poco_assert (pos < _values.size());
		_values[pos] = Poco::Any(T());

		T* pVal = AnyCast<T>(&_values[pos]);
		if (Utility::isError(SQLBindCol(_rStmt,
			(SQLUSMALLINT) pos + 1,
			valueType,
			(SQLPOINTER) pVal,
			(SQLINTEGER) dataSize,
			&_lengths[pos])))
		{
			throw StatementException(_rStmt, "ODBC::Preparator::prepareFixedSize():SQLBindCol()");
		}
	}

	template <typename T>
	void prepareFixedSize(std::size_t pos, SQLSMALLINT valueType, std::size_t length)
		/// Utility function for preparation of fixed length columns that are
		/// bound to a std::vector.
	{
		poco_assert (DE_BOUND == _dataExtraction);
		std::size_t dataSize = sizeof(T);

		poco_assert (pos < _values.size());
		poco_assert (length);
		_values[pos] = Poco::Any(std::vector<T>());
		_lengths[pos] = 0;
		poco_assert (0 == _lenLengths[pos].size());
		_lenLengths[pos].resize(length);

		std::vector<T>& cache = RefAnyCast<std::vector<T>>(_values[pos]);
		cache.resize(length);

		if (Utility::isError(SQLBindCol(_rStmt,
			(SQLUSMALLINT) pos + 1,
			valueType,
			(SQLPOINTER) &cache[0],
			(SQLINTEGER) dataSize,
			&_lenLengths[pos][0])))
		{
			throw StatementException(_rStmt, "ODBC::Preparator::prepareFixedSize():SQLBindCol()");
		}
	}

	template <typename T>
	void prepareVariableLen(std::size_t pos, SQLSMALLINT valueType, std::size_t size, DataType dt)
		/// Utility function for preparation of variable length columns.
	{
		poco_assert (DE_BOUND == _dataExtraction);
		poco_assert (pos < _values.size());

		T* pCache = new T[size];
		std::memset(pCache, 0, size * sizeof(T));

		_values[pos] = Any(pCache);
		_lengths[pos] = (SQLLEN) size;
		_bufferSizes[pos] = size * sizeof(T);
		_varLengthArrays.insert(IndexMap::value_type(pos, dt));

		if (Utility::isError(SQLBindCol(_rStmt,
			(SQLUSMALLINT) pos + 1,
			valueType,
			(SQLPOINTER) pCache,
			(SQLLEN) _bufferSizes[pos],
			&_lengths[pos])))
		{
			throw StatementException(_rStmt, "ODBC::Preparator::prepareVariableLen():SQLBindCol()");
		}
	}

	template <typename T, DataType DT>
	void prepareCharArray(std::size_t pos, SQLSMALLINT valueType, std::size_t size, std::size_t length)
		/// Utility function for preparation of bulk variable length character and LOB columns.
		/// Binds length rows of size elements of type T; the BufferLength passed to
		/// SQLBindCol(), and with it the row stride, is size * sizeof(T) bytes.
	{
		poco_assert_dbg (DE_BOUND == _dataExtraction);
		// release checks, as in prepareVariableLen(): for a column past the result
		// set the writes below would go past the per-column vectors
		poco_assert (pos < _values.size());
		poco_assert (pos < _lengths.size());
		poco_assert (pos < _lenLengths.size());
		poco_assert (pos < _bufferSizes.size());
		if (size != 0 && length > std::numeric_limits<std::size_t>::max() / size / sizeof(T))
			throw InvalidArgumentException("ODBC::Preparator::prepareCharArray(): bulk buffer too large");

		T* pArray = (T*) std::calloc(length * size, sizeof(T));
		if (pArray == nullptr && length * size != 0)
			throw OutOfMemoryException("ODBC::Preparator::prepareCharArray()");

		_values[pos] = Any(pArray);
		_lengths[pos] = 0;
		_lenLengths[pos].resize(length);
		_bufferSizes[pos] = size * sizeof(T);
		_varLengthArrays.insert(IndexMap::value_type(pos, DT));

		if (Utility::isError(SQLBindCol(_rStmt,
			(SQLUSMALLINT) pos + 1,
			valueType,
			(SQLPOINTER) pArray,
			(SQLLEN) _bufferSizes[pos],
			&_lenLengths[pos][0])))
		{
			throw StatementException(_rStmt, "ODBC::Preparator::prepareCharArray():SQLBindCol()");
		}
	}

	void prepareBoolArray(std::size_t pos, SQLSMALLINT valueType, std::size_t length);
		/// Utility function for preparation of bulk bool columns.

	void freeMemory() const;
		/// Utility function. Releases memory allocated for variable length columns.

	template <typename T>
	void deleteCachedArray(std::size_t pos) const
	{
		T** p = Poco::AnyCast<T*>(&_values[pos]);
		if (p) delete [] *p;
	}

	const StatementHandle&  _rStmt;
	mutable ValueVec        _values;
	mutable LengthVec       _lengths;
	mutable LengthLengthVec _lenLengths;
	mutable std::vector<std::size_t> _bufferSizes;
	mutable std::vector<MetaColumn::ColumnDataType> _boundTypes;
	mutable IndexMap        _varLengthArrays;
	std::size_t             _maxFieldSize;
	DataExtraction          _dataExtraction;
};


//
// inlines
//
inline void Preparator::prepare(std::size_t pos, const Poco::Int8&)
{
	prepareFixedSize<Poco::Int8>(pos, SQL_C_STINYINT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Int8>& val)
{
	prepareFixedSize<Poco::Int8>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Int8>& val)
{
	prepareFixedSize<Poco::Int8>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Int8>& val)
{
	prepareFixedSize<Poco::Int8>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::UInt8&)
{
	prepareFixedSize<Poco::UInt8>(pos, SQL_C_UTINYINT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::UInt8>& val)
{
	prepareFixedSize<Poco::UInt8>(pos, SQL_C_UTINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::UInt8>& val)
{
	prepareFixedSize<Poco::UInt8>(pos, SQL_C_UTINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::UInt8>& val)
{
	prepareFixedSize<Poco::UInt8>(pos, SQL_C_UTINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Int16&)
{
	prepareFixedSize<Poco::Int16>(pos, SQL_C_SSHORT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Int16>& val)
{
	prepareFixedSize<Poco::Int16>(pos, SQL_C_SSHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Int16>& val)
{
	prepareFixedSize<Poco::Int16>(pos, SQL_C_SSHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Int16>& val)
{
	prepareFixedSize<Poco::Int16>(pos, SQL_C_SSHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::UInt16&)
{
	prepareFixedSize<Poco::UInt16>(pos, SQL_C_USHORT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::UInt16>& val)
{
	prepareFixedSize<Poco::UInt16>(pos, SQL_C_USHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::UInt16>& val)
{
	prepareFixedSize<Poco::UInt16>(pos, SQL_C_USHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::UInt16>& val)
{
	prepareFixedSize<Poco::UInt16>(pos, SQL_C_USHORT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Int32&)
{
	prepareFixedSize<Poco::Int32>(pos, SQL_C_SLONG);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Int32>& val)
{
	prepareFixedSize<Poco::Int32>(pos, SQL_C_SLONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Int32>& val)
{
	prepareFixedSize<Poco::Int32>(pos, SQL_C_SLONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Int32>& val)
{
	prepareFixedSize<Poco::Int32>(pos, SQL_C_SLONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::UInt32&)
{
	prepareFixedSize<Poco::UInt32>(pos, SQL_C_ULONG);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::UInt32>& val)
{
	prepareFixedSize<Poco::UInt32>(pos, SQL_C_ULONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::UInt32>& val)
{
	prepareFixedSize<Poco::UInt32>(pos, SQL_C_ULONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::UInt32>& val)
{
	prepareFixedSize<Poco::UInt32>(pos, SQL_C_ULONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Int64&)
{
	prepareFixedSize<Poco::Int64>(pos, SQL_C_SBIGINT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Int64>& val)
{
	prepareFixedSize<Poco::Int64>(pos, SQL_C_SBIGINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Int64>& val)
{
	prepareFixedSize<Poco::Int64>(pos, SQL_C_SBIGINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Int64>& val)
{
	prepareFixedSize<Poco::Int64>(pos, SQL_C_SBIGINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::UInt64&)
{
	prepareFixedSize<Poco::UInt64>(pos, SQL_C_UBIGINT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::UInt64>& val)
{
	prepareFixedSize<Poco::UInt64>(pos, SQL_C_UBIGINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::UInt64>& val)
{
	prepareFixedSize<Poco::UInt64>(pos, SQL_C_UBIGINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::UInt64>& val)
{
	prepareFixedSize<Poco::UInt64>(pos, SQL_C_UBIGINT, val.size());
}


#ifndef POCO_INT64_IS_LONG
inline void Preparator::prepare(std::size_t pos, const long&)
{
	prepareFixedSize<long>(pos, SQL_C_SLONG);
}


inline void Preparator::prepare(std::size_t pos, const unsigned long&)
{
	prepareFixedSize<unsigned long>(pos, SQL_C_ULONG);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<long>& val)
{
	prepareFixedSize<long>(pos, SQL_C_SLONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<long>& val)
{
	prepareFixedSize<long>(pos, SQL_C_SLONG, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<long>& val)
{
	prepareFixedSize<long>(pos, SQL_C_SLONG, val.size());
}
#endif


inline void Preparator::prepare(std::size_t pos, const bool&)
{
	prepareFixedSize<bool>(pos, SQL_C_BIT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<bool>& val)
{
	prepareBoolArray(pos, SQL_C_BIT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<bool>& val)
{
	prepareBoolArray(pos, SQL_C_BIT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<bool>& val)
{
	prepareBoolArray(pos, SQL_C_BIT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const float&)
{
	prepareFixedSize<float>(pos, SQL_C_FLOAT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<float>& val)
{
	prepareFixedSize<float>(pos, SQL_C_FLOAT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<float>& val)
{
	prepareFixedSize<float>(pos, SQL_C_FLOAT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<float>& val)
{
	prepareFixedSize<float>(pos, SQL_C_FLOAT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const double&)
{
	prepareFixedSize<double>(pos, SQL_C_DOUBLE);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<double>& val)
{
	prepareFixedSize<double>(pos, SQL_C_DOUBLE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<double>& val)
{
	prepareFixedSize<double>(pos, SQL_C_DOUBLE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<double>& val)
{
	prepareFixedSize<double>(pos, SQL_C_DOUBLE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const char&)
{
	prepareFixedSize<char>(pos, SQL_C_STINYINT);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<char>& val)
{
	prepareFixedSize<char>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<char>& val)
{
	prepareFixedSize<char>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<char>& val)
{
	prepareFixedSize<char>(pos, SQL_C_STINYINT, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::string&)
{
	prepareVariableLen<char>(pos, SQL_C_CHAR, maxDataSize(pos), DT_CHAR);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<std::string>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_CHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<std::string>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_CHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<std::string>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_CHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const UTF16String&)
{
	prepareVariableLen<UTF16String::value_type>(pos, SQL_C_WCHAR, maxDataSize(pos), DT_WCHAR);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<UTF16String>& val)
{
	prepareCharArray<UTF16String::value_type, DT_WCHAR_ARRAY>(pos, SQL_C_WCHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<UTF16String>& val)
{
	prepareCharArray<UTF16String::value_type, DT_WCHAR_ARRAY>(pos, SQL_C_WCHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<UTF16String>& val)
{
	prepareCharArray<UTF16String::value_type, DT_WCHAR_ARRAY>(pos, SQL_C_WCHAR, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Data::BLOB&)
{
	prepareVariableLen<Poco::Data::BLOB::ValueType>(pos, SQL_C_BINARY, maxDataSize(pos), DT_UCHAR);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Data::BLOB>& val)
{
	prepareCharArray<Poco::Data::BLOB::ValueType, DT_UCHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Data::BLOB>& val)
{
	prepareCharArray<Poco::Data::BLOB::ValueType, DT_UCHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Data::BLOB>& val)
{
	prepareCharArray<Poco::Data::BLOB::ValueType, DT_UCHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Data::CLOB&)
{
	prepareVariableLen<Poco::Data::CLOB::ValueType>(pos, SQL_C_BINARY, maxDataSize(pos), DT_CHAR);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Data::CLOB>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Data::CLOB>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Data::CLOB>& val)
{
	prepareCharArray<char, DT_CHAR_ARRAY>(pos, SQL_C_BINARY, maxDataSize(pos), val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Data::Date&)
{
	prepareFixedSize<SQL_DATE_STRUCT>(pos, SQL_C_TYPE_DATE);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Data::Date>& val)
{
	prepareFixedSize<SQL_DATE_STRUCT>(pos, SQL_C_TYPE_DATE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Data::Date>& val)
{
	prepareFixedSize<SQL_DATE_STRUCT>(pos, SQL_C_TYPE_DATE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Data::Date>& val)
{
	prepareFixedSize<SQL_DATE_STRUCT>(pos, SQL_C_TYPE_DATE, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::Data::Time&)
{
	prepareFixedSize<SQL_TIME_STRUCT>(pos, SQL_C_TYPE_TIME);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::Data::Time>& val)
{
	prepareFixedSize<SQL_TIME_STRUCT>(pos, SQL_C_TYPE_TIME, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::Data::Time>& val)
{
	prepareFixedSize<SQL_TIME_STRUCT>(pos, SQL_C_TYPE_TIME, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::Data::Time>& val)
{
	prepareFixedSize<SQL_TIME_STRUCT>(pos, SQL_C_TYPE_TIME, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::DateTime&)
{
	prepareFixedSize<SQL_TIMESTAMP_STRUCT>(pos, SQL_C_TYPE_TIMESTAMP);
}


inline void Preparator::prepare(std::size_t pos, const std::vector<Poco::DateTime>& val)
{
	prepareFixedSize<SQL_TIMESTAMP_STRUCT>(pos, SQL_C_TYPE_TIMESTAMP, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::deque<Poco::DateTime>& val)
{
	prepareFixedSize<SQL_TIMESTAMP_STRUCT>(pos, SQL_C_TYPE_TIMESTAMP, val.size());
}


inline void Preparator::prepare(std::size_t pos, const std::list<Poco::DateTime>& val)
{
	prepareFixedSize<SQL_TIMESTAMP_STRUCT>(pos, SQL_C_TYPE_TIMESTAMP, val.size());
}


inline void Preparator::prepare(std::size_t pos, const Poco::UUID&)
{
	prepareVariableLen<char>(pos, SQL_C_BINARY, 16, DT_CHAR);
}


inline void Preparator::prepare(std::size_t pos, const Poco::Any& val)
{
	prepareImpl(pos);
}


inline void Preparator::prepare(std::size_t, const std::vector<Poco::Any>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::vector<Poco::Any> is not supported.");
}


inline void Preparator::prepare(std::size_t, const std::deque<Poco::Any>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::deque<Poco::Any> is not supported.");
}


inline void Preparator::prepare(std::size_t, const std::list<Poco::Any>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::list<Poco::Any> is not supported.");
}


inline void Preparator::prepare(std::size_t pos, const Poco::Dynamic::Var& val)
{
	prepareImpl(pos);
}


inline void Preparator::prepare(std::size_t, const std::vector<Poco::Dynamic::Var>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::vector<Poco::Dynamic::Var> is not supported.");
}


inline void Preparator::prepare(std::size_t, const std::deque<Poco::Dynamic::Var>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::deque<Poco::Dynamic::Var> is not supported.");
}


inline void Preparator::prepare(std::size_t, const std::list<Poco::Dynamic::Var>&)
{
	throw NotImplementedException("ODBC::Preparator::prepare(): bulk extraction into std::list<Poco::Dynamic::Var> is not supported.");
}


inline std::size_t Preparator::bulkSize(std::size_t col) const
{
	poco_assert (col < _lenLengths.size());

	return _lenLengths[col].size();
}


inline std::size_t Preparator::boundBufferSize(std::size_t pos) const
{
	return _bufferSizes.at(pos);
}


inline MetaColumn::ColumnDataType Preparator::boundType(std::size_t pos) const
{
	return _boundTypes.at(pos);
}


inline void Preparator::setMaxFieldSize(std::size_t size)
{
	_maxFieldSize = size;
}


inline std::size_t Preparator::getMaxFieldSize() const
{
	return _maxFieldSize;
}


inline void Preparator::setDataExtraction(Preparator::DataExtraction ext)
{
	_dataExtraction = ext;
}


inline Preparator::DataExtraction Preparator::getDataExtraction() const
{
	return _dataExtraction;
}


inline Poco::Any& Preparator::operator [] (std::size_t pos)
{
	return at(pos);
}


inline Poco::Any& Preparator::at(std::size_t pos)
{
	return _values.at(pos);
}


} } // namespace Poco::Data


#endif // Data_ODBC_Preparator_INCLUDED
