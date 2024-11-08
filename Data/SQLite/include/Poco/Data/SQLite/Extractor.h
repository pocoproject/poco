//
// Extractor.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Extractor
//
// Definition of the Extractor class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SQLite_Extractor_INCLUDED
#define Data_SQLite_Extractor_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/AbstractExtractor.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/Constants.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Nullable.h"
#include "Poco/UUID.h"
#include "Poco/UTFString.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "sqlite3.h"
#include <vector>
#include <utility>


namespace Poco {
namespace Data {
namespace SQLite {


class SQLite_API Extractor: public Poco::Data::AbstractExtractor
	/// Extracts and converts data values form the result row returned by SQLite.
	/// If NULL is received, the incoming val value is not changed and false is returned
{
public:
	using NullIndVec = std::vector<std::pair<bool, bool> >;
		/// Type for null indicators container.

	Extractor(sqlite3_stmt* pStmt);
		/// Creates the Extractor.

	~Extractor();
		/// Destroys the Extractor.

	bool extract(std::size_t pos, Poco::Int8& val) override;
		/// Extracts an Int8.

	bool extract(std::size_t pos, Poco::UInt8& val) override;
		/// Extracts an UInt8.

	bool extract(std::size_t pos, Poco::Int16& val) override;
		/// Extracts an Int16.

	bool extract(std::size_t pos, Poco::UInt16& val) override;
		/// Extracts an UInt16.

	bool extract(std::size_t pos, Poco::Int32& val) override;
		/// Extracts an Int32.

	bool extract(std::size_t pos, Poco::UInt32& val) override;
		/// Extracts an UInt32.

	bool extract(std::size_t pos, Poco::Int64& val) override;
		/// Extracts an Int64.

	bool extract(std::size_t pos, Poco::UInt64& val) override;
		/// Extracts an UInt64.

	bool extract(std::size_t pos, bool& val) override;
		/// Extracts a boolean.

	bool extract(std::size_t pos, float& val) override;
		/// Extracts a float.

	bool extract(std::size_t pos, double& val) override;
		/// Extracts a double.

	bool extract(std::size_t pos, char& val) override;
		/// Extracts a single character.

	bool extract(std::size_t pos, std::string& val) override;
		/// Extracts a string.

	bool extract(std::size_t pos, UTF16String& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, Poco::Data::BLOB& val) override;
		/// Extracts a BLOB.

	bool extract(std::size_t pos, Poco::Data::CLOB& val) override;
		/// Extracts a CLOB.

	bool extract(std::size_t pos, Poco::Data::Date& val) override;
		/// Extracts a Date.

	bool extract(std::size_t pos, Poco::Data::Time& val) override;
		/// Extracts a Time.

	bool extract(std::size_t pos, Poco::DateTime& val) override;
		/// Extracts a DateTime.

	bool extract(std::size_t pos, Poco::UUID& val) override;
		/// Extracts a UUID.

	bool extract(std::size_t pos, Poco::Any& val) override;
		/// Extracts an Any.

	bool extract(std::size_t pos, Poco::Dynamic::Var& val) override;
		/// Extracts a Dynamic::Var.

	// Nullable extraction
	bool extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val) override;
		/// Extracts an Nullable<Int8>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val) override;
		/// Extracts an Nullable<UInt8>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val) override;
		/// Extracts an Nullable<Int16>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val) override;
		/// Extracts an Nullable<UInt16>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val) override;
		/// Extracts an Nullable<Int32>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val) override;
		/// Extracts an Nullable<UInt32>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val) override;
		/// Extracts an Nullable<Int64>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val) override;
		/// Extracts an Nullable<UInt64>.

#ifndef POCO_INT64_IS_LONG

	bool extract(std::size_t pos, long& val) override;
		/// Extracts a long.

	bool extract(std::size_t pos, unsigned long& val) override;
		/// Extracts an unsigned long.

	bool extract(std::size_t pos, std::vector<long>& val) override;
		/// Extracts a long vector.

	bool extract(std::size_t pos, std::deque<long>& val) override;
		/// Extracts a long deque.

	bool extract(std::size_t pos, std::list<long>& val) override;
		/// Extracts a long list.

	bool extract(std::size_t pos, std::vector<unsigned long>& val) override;
		/// Extracts an unsigned long vector.

	bool extract(std::size_t pos, std::deque<unsigned long>& val) override;
		/// Extracts an unsigned long deque.

	bool extract(std::size_t pos, std::list<unsigned long>& val) override;
		/// Extracts an unsigned long list.

	bool extract(std::size_t pos, Poco::Nullable<long>& val) override;
		/// Extracts a Nullable<long>.

	bool extract(std::size_t pos, Poco::Nullable<unsigned long>& val) override;
		/// Extracts a Nullable<unsigned long>.

#endif

	bool extract(std::size_t pos, Poco::Nullable<bool>& val) override;
		/// Extracts a Nullable<bool>.

	bool extract(std::size_t pos, Poco::Nullable<float>& val) override;
		/// Extracts a Nullable<float>.

	bool extract(std::size_t pos, Poco::Nullable<double>& val) override;
		/// Extracts a Nullable<double>.

	bool extract(std::size_t pos, Poco::Nullable<char>& val) override;
		/// Extracts a single Nullable character.

	bool extract(std::size_t pos, Poco::Nullable<std::string>& val) override;
		/// Extracts a Nullable<string>.

	bool extract(std::size_t pos, Poco::Nullable<UTF16String>& val) override;
		/// Extracts a Nullable<UTF16String>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Data::BLOB>& val) override;
		/// Extracts a Nullable<BLOB>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Data::CLOB>& val) override;
		/// Extracts a Nullable<BLOB>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Data::Date>& val) override;
		/// Extracts a Nullable<Date>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Data::Time>& val) override;
		/// Extracts a :Nullable<Time>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::DateTime>& val) override;
		/// Extracts Nullable<DateTime>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UUID>& val) override;
		/// Extracts a Nullable<UUID>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Any>& val) override;
		/// Extracts a Nullable<Any>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val) override;
		/// Extracts a Nullable<Var>.

	// Containers
	bool extract(std::size_t pos, std::vector<Poco::Int8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Int8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Int8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::UInt8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::UInt8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::UInt8>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Int16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Int16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Int16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::UInt16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::UInt16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::UInt16>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Int32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Int32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Int32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::UInt32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::UInt32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::UInt32>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Int64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Int64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Int64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::UInt64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::UInt64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::UInt64>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<bool>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<bool>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<bool>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<char>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<char>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<char>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<std::string>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<std::string>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<std::string>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<UTF16String>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<UTF16String>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<UTF16String>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<float>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<float>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<float>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<double>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<double>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<double>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Data::Date>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Data::Date>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Data::Date>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Data::Time>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Data::Time>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Data::Time>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Data::BLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Data::BLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Data::BLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Data::CLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Data::CLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Data::CLOB>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::DateTime>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::DateTime>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::DateTime>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::UUID>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::UUID>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::UUID>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Any>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Any>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Any>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException.

	bool extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val) override;
		/// Throws NotImplementedException.

	bool isNull(std::size_t pos, std::size_t row = POCO_DATA_INVALID_ROW) override;
		/// Returns true if the current row value at pos column is null.
		/// Because of the loss of information about null-ness of the
		/// underlying database values due to the nature of SQLite engine,
		/// (once null value is converted to default value, SQLite API
		/// treats it  as non-null), a null indicator container member
		/// variable is used to cache the indicators of the underlying nulls
		/// thus rendering this function idempotent.
		/// The container is a vector of [bool, bool] pairs.
		/// The vector index corresponds to the column position, the first
		/// bool value in the pair is true if the null indicator has
		/// been set and the second bool value in the pair is true if
		/// the column is actually null.
		/// The row argument, needed for connectors with bulk capabilities,
		/// is ignored in this implementation.

	void reset() override;
		/// Clears the cached nulls indicator vector.

private:
	template <typename F, typename T>
	bool extract(F func, std::size_t pos, T& val)
	{
		if (isNull(pos)) return false;
		val = func(_pStmt, (int) pos);
		return true;
	}

	template <typename T>
	bool extractImpl(std::size_t pos, T& val)
		/// Utility function for extraction of Any and Dynamic::Var.
	{
		if (isNull(pos)) return false;

		bool ret = false;

		switch (Utility::getColumnType(_pStmt, pos))
		{
		case MetaColumn::FDT_BOOL:
		{
			bool i = false;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_INT8:
		{
			Poco::Int8 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_UINT8:
		{
			Poco::UInt8 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_INT16:
		{
			Poco::Int16 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_UINT16:
		{
			Poco::UInt16 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_INT32:
		{
			Poco::Int32 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_UINT32:
		{
			Poco::UInt32 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_INT64:
		{
			Poco::Int64 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_UINT64:
		{
			Poco::UInt64 i = 0;
			ret = extract(pos, i);
			val = i;
			break;
		}
		case MetaColumn::FDT_STRING:
		{
			std::string s;
			ret = extract(pos, s);
			val = s;
			break;
		}
		case MetaColumn::FDT_DOUBLE:
		{
			double d(0.0);
			ret = extract(pos, d);
			val = d;
			break;
		}
		case MetaColumn::FDT_FLOAT:
		{
			float f(0.0);
			ret = extract(pos, f);
			val = f;
			break;
		}
		case MetaColumn::FDT_BLOB:
		{
			BLOB b;
			ret = extract(pos, b);
			val = b;
			break;
		}
		case MetaColumn::FDT_DATE:
		{
			Date d;
			ret = extract(pos, d);
			val = d;
			break;
		}
		case MetaColumn::FDT_TIME:
		{
			Time t;
			ret = extract(pos, t);
			val = t;
			break;
		}
		case MetaColumn::FDT_TIMESTAMP:
		{
			DateTime dt;
			ret = extract(pos, dt);
			val = dt;
			break;
		}
		case MetaColumn::FDT_UUID:
		{
			UUID uuid;
			ret = extract(pos, uuid);
			val = uuid;
			break;
		}
		default:
			throw Poco::Data::UnknownTypeException("Unknown type during extraction");
		}

		return ret;
	}

	template <typename T>
	bool extractLOB(std::size_t pos, Poco::Data::LOB<T>& val)
	{
		if (isNull(pos)) return false;
		int size = sqlite3_column_bytes(_pStmt, (int) pos);
		const T* pTmp = reinterpret_cast<const T*>(sqlite3_column_blob(_pStmt, (int) pos));
		val = Poco::Data::LOB<T>(pTmp, size);
		return true;
	}

	sqlite3_stmt* _pStmt;
	NullIndVec    _nulls;
};


///
/// inlines
///
inline void Extractor::reset()
{
	_nulls.clear();
}


inline bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	return extractLOB<Poco::Data::BLOB::ValueType>(pos, val);
}


inline bool Extractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	return extractLOB<Poco::Data::CLOB::ValueType>(pos, val);
}


} } } // namespace Poco::Data::SQLite


#endif // Data_SQLite_Extractor_INCLUDED
