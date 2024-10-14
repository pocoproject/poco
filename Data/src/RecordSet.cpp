//
// RecordSet.cpp
//
// Library: Data
// Package: DataCore
// Module:  RecordSet
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Extraction.h"
#include "Poco/Data/RowFilter.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/DataException.h"
#include "Poco/DateTime.h"
#include "Poco/UTFString.h"


using namespace Poco::Data::Keywords;
using Poco::DateTime;
using Poco::UTF16String;


namespace Poco {
namespace Data {


const std::size_t RecordSet::UNKNOWN_TOTAL_ROW_COUNT = std::numeric_limits<std::size_t>::max();


RecordSet::RecordSet(const Statement& rStatement,
	RowFormatter::Ptr pRowFormatter):
	Statement(rStatement),
	_currentRow(0),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true)),
	_totalRowCount(UNKNOWN_TOTAL_ROW_COUNT)
{
	if (pRowFormatter) setRowFormatter(pRowFormatter);
}


RecordSet::RecordSet(Session& rSession,
	const std::string& query,
	RowFormatter::Ptr pRowFormatter):
	Statement((rSession << query, now)),
	_currentRow(0),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true)),
	_totalRowCount(UNKNOWN_TOTAL_ROW_COUNT)
{
	if (pRowFormatter) setRowFormatter(pRowFormatter);
}


RecordSet::RecordSet(const RecordSet& other):
	Statement(other.impl()),
	_currentRow(other._currentRow),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true)),
	_pFilter(other._pFilter),
	_totalRowCount(other._totalRowCount)
{
}


RecordSet::RecordSet(RecordSet&& other) noexcept:
	Statement(std::move(other)),
	_currentRow(std::move(other._currentRow)),
	_pBegin(std::move(other._pBegin)),
	_pEnd(std::move(other._pEnd)),
	_pFilter(std::move(other._pFilter)),
	_totalRowCount(std::move(other._totalRowCount))
{
}


RecordSet::~RecordSet()
{
	try
	{
		delete _pBegin;
		delete _pEnd;

		RowMap::iterator it = _rowMap.begin();
		RowMap::iterator end = _rowMap.end();
		for (; it != end; ++it) delete it->second;
	}
	catch (...)
	{
		poco_unexpected();
	}
}


RecordSet& RecordSet::operator = (RecordSet&& other) noexcept
{
	Statement::operator = (std::move(other));
	_currentRow = std::move(other._currentRow);
	_pBegin = std::move(other._pBegin);
	_pEnd = std::move(other._pEnd);
	_pFilter = std::move(other._pFilter);
	_totalRowCount = std::move(other._totalRowCount);

	return *this;
}


void RecordSet::reset(const Statement& stmt)
{
	delete _pBegin;
	_pBegin = 0;
	delete _pEnd;
	_pEnd = 0;
	_currentRow = 0;
	_totalRowCount = UNKNOWN_TOTAL_ROW_COUNT;

	RowMap::iterator it = _rowMap.begin();
	RowMap::iterator end = _rowMap.end();
	for (; it != end; ++it) delete it->second;
	_rowMap.clear();

	Statement::operator = (stmt);

	_pBegin = new RowIterator(this, 0 == rowsExtracted());
	_pEnd = new RowIterator(this, true);
}


template <class C>
const Column<C>& RecordSet::column(const std::string& name) const
	/// Returns the reference to the first Column with the specified name.
{
	if (isBulkExtraction())
	{
		using E = InternalBulkExtraction<C>;
		return columnImpl<C,E>(name);
	}
	else
	{
		using E = InternalExtraction<C>;
		return columnImpl<C,E>(name);
	}
}


template Data_API const Column<std::vector<bool>>& RecordSet::column<std::vector<bool>>(const std::string& name) const;
template Data_API const Column<std::vector<UInt8>>& RecordSet::column<std::vector<UInt8>>(const std::string& name) const;
template Data_API const Column<std::vector<Int16>>& RecordSet::column<std::vector<Int16>>(const std::string& name) const;
template Data_API const Column<std::vector<UInt16>>& RecordSet::column<std::vector<UInt16>>(const std::string& name) const;
template Data_API const Column<std::vector<Int32>>& RecordSet::column<std::vector<Int32>>(const std::string& name) const;
template Data_API const Column<std::vector<UInt32>>& RecordSet::column<std::vector<UInt32>>(const std::string& name) const;
template Data_API const Column<std::vector<Int64>>& RecordSet::column<std::vector<Int64>>(const std::string& name) const;
template Data_API const Column<std::vector<UInt64>>& RecordSet::column<std::vector<UInt64>>(const std::string& name) const;
template Data_API const Column<std::vector<float>>& RecordSet::column<std::vector<float>>(const std::string& name) const;
template Data_API const Column<std::vector<double>>& RecordSet::column<std::vector<double>>(const std::string& name) const;
template Data_API const Column<std::vector<std::string>>& RecordSet::column<std::vector<std::string>>(const std::string& name) const;
template Data_API const Column<std::vector<UTF16String>>& RecordSet::column<std::vector<UTF16String>>(const std::string& name) const;
template Data_API const Column<std::vector<BLOB>>& RecordSet::column<std::vector<BLOB>>(const std::string& name) const;
template Data_API const Column<std::vector<CLOB>>& RecordSet::column<std::vector<CLOB>>(const std::string& name) const;
template Data_API const Column<std::vector<Date>>& RecordSet::column<std::vector<Date>>(const std::string& name) const;
template Data_API const Column<std::vector<Time>>& RecordSet::column<std::vector<Time>>(const std::string& name) const;
template Data_API const Column<std::vector<DateTime>>& RecordSet::column<std::vector<DateTime>>(const std::string& name) const;
template Data_API const Column<std::vector<UUID>>& RecordSet::column<std::vector<UUID>>(const std::string& name) const;

template Data_API const Column<std::list<bool>>& RecordSet::column<std::list<bool>>(const std::string& name) const;
template Data_API const Column<std::list<UInt8>>& RecordSet::column<std::list<UInt8>>(const std::string& name) const;
template Data_API const Column<std::list<Int16>>& RecordSet::column<std::list<Int16>>(const std::string& name) const;
template Data_API const Column<std::list<UInt16>>& RecordSet::column<std::list<UInt16>>(const std::string& name) const;
template Data_API const Column<std::list<Int32>>& RecordSet::column<std::list<Int32>>(const std::string& name) const;
template Data_API const Column<std::list<UInt32>>& RecordSet::column<std::list<UInt32>>(const std::string& name) const;
template Data_API const Column<std::list<Int64>>& RecordSet::column<std::list<Int64>>(const std::string& name) const;
template Data_API const Column<std::list<UInt64>>& RecordSet::column<std::list<UInt64>>(const std::string& name) const;
template Data_API const Column<std::list<float>>& RecordSet::column<std::list<float>>(const std::string& name) const;
template Data_API const Column<std::list<double>>& RecordSet::column<std::list<double>>(const std::string& name) const;
template Data_API const Column<std::list<std::string>>& RecordSet::column<std::list<std::string>>(const std::string& name) const;
template Data_API const Column<std::list<UTF16String>>& RecordSet::column<std::list<UTF16String>>(const std::string& name) const;
template Data_API const Column<std::list<BLOB>>& RecordSet::column<std::list<BLOB>>(const std::string& name) const;
template Data_API const Column<std::list<CLOB>>& RecordSet::column<std::list<CLOB>>(const std::string& name) const;
template Data_API const Column<std::list<Date>>& RecordSet::column<std::list<Date>>(const std::string& name) const;
template Data_API const Column<std::list<Time>>& RecordSet::column<std::list<Time>>(const std::string& name) const;
template Data_API const Column<std::list<DateTime>>& RecordSet::column<std::list<DateTime>>(const std::string& name) const;
template Data_API const Column<std::list<UUID>>& RecordSet::column<std::list<UUID>>(const std::string& name) const;

template Data_API const Column<std::deque<bool>>& RecordSet::column<std::deque<bool>>(const std::string& name) const;
template Data_API const Column<std::deque<UInt8>>& RecordSet::column<std::deque<UInt8>>(const std::string& name) const;
template Data_API const Column<std::deque<Int16>>& RecordSet::column<std::deque<Int16>>(const std::string& name) const;
template Data_API const Column<std::deque<UInt16>>& RecordSet::column<std::deque<UInt16>>(const std::string& name) const;
template Data_API const Column<std::deque<Int32>>& RecordSet::column<std::deque<Int32>>(const std::string& name) const;
template Data_API const Column<std::deque<UInt32>>& RecordSet::column<std::deque<UInt32>>(const std::string& name) const;
template Data_API const Column<std::deque<Int64>>& RecordSet::column<std::deque<Int64>>(const std::string& name) const;
template Data_API const Column<std::deque<UInt64>>& RecordSet::column<std::deque<UInt64>>(const std::string& name) const;
template Data_API const Column<std::deque<float>>& RecordSet::column<std::deque<float>>(const std::string& name) const;
template Data_API const Column<std::deque<double>>& RecordSet::column<std::deque<double>>(const std::string& name) const;
template Data_API const Column<std::deque<std::string>>& RecordSet::column<std::deque<std::string>>(const std::string& name) const;
template Data_API const Column<std::deque<UTF16String>>& RecordSet::column<std::deque<UTF16String>>(const std::string& name) const;
template Data_API const Column<std::deque<BLOB>>& RecordSet::column<std::deque<BLOB>>(const std::string& name) const;
template Data_API const Column<std::deque<CLOB>>& RecordSet::column<std::deque<CLOB>>(const std::string& name) const;
template Data_API const Column<std::deque<Date>>& RecordSet::column<std::deque<Date>>(const std::string& name) const;
template Data_API const Column<std::deque<Time>>& RecordSet::column<std::deque<Time>>(const std::string& name) const;
template Data_API const Column<std::deque<DateTime>>& RecordSet::column<std::deque<DateTime>>(const std::string& name) const;
template Data_API const Column<std::deque<UUID>>& RecordSet::column<std::deque<UUID>>(const std::string& name) const;


template <class C>
const Column<C>& RecordSet::column(std::size_t pos) const
	/// Returns the reference to column at specified position.
{
	if (isBulkExtraction())
	{
		using E = InternalBulkExtraction<C>;
		return columnImpl<C,E>(pos);
	}
	else
	{
		using E = InternalExtraction<C>;
		return columnImpl<C,E>(pos);
	}
}


template Data_API const Column<std::vector<bool>>& RecordSet::column<std::vector<bool>>(std::size_t pos) const;
template Data_API const Column<std::vector<UInt8>>& RecordSet::column<std::vector<UInt8>>(std::size_t pos) const;
template Data_API const Column<std::vector<Int16>>& RecordSet::column<std::vector<Int16>>(std::size_t pos) const;
template Data_API const Column<std::vector<UInt16>>& RecordSet::column<std::vector<UInt16>>(std::size_t pos) const;
template Data_API const Column<std::vector<Int32>>& RecordSet::column<std::vector<Int32>>(std::size_t pos) const;
template Data_API const Column<std::vector<UInt32>>& RecordSet::column<std::vector<UInt32>>(std::size_t pos) const;
template Data_API const Column<std::vector<Int64>>& RecordSet::column<std::vector<Int64>>(std::size_t pos) const;
template Data_API const Column<std::vector<UInt64>>& RecordSet::column<std::vector<UInt64>>(std::size_t pos) const;
template Data_API const Column<std::vector<float>>& RecordSet::column<std::vector<float>>(std::size_t pos) const;
template Data_API const Column<std::vector<double>>& RecordSet::column<std::vector<double>>(std::size_t pos) const;
template Data_API const Column<std::vector<std::string>>& RecordSet::column<std::vector<std::string>>(std::size_t pos) const;
template Data_API const Column<std::vector<UTF16String>>& RecordSet::column<std::vector<UTF16String>>(std::size_t pos) const;
template Data_API const Column<std::vector<BLOB>>& RecordSet::column<std::vector<BLOB>>(std::size_t pos) const;
template Data_API const Column<std::vector<CLOB>>& RecordSet::column<std::vector<CLOB>>(std::size_t pos) const;
template Data_API const Column<std::vector<Date>>& RecordSet::column<std::vector<Date>>(std::size_t pos) const;
template Data_API const Column<std::vector<Time>>& RecordSet::column<std::vector<Time>>(std::size_t pos) const;
template Data_API const Column<std::vector<DateTime>>& RecordSet::column<std::vector<DateTime>>(std::size_t pos) const;
template Data_API const Column<std::vector<UUID>>& RecordSet::column<std::vector<UUID>>(std::size_t pos) const;

template Data_API const Column<std::list<bool>>& RecordSet::column<std::list<bool>>(std::size_t pos) const;
template Data_API const Column<std::list<UInt8>>& RecordSet::column<std::list<UInt8>>(std::size_t pos) const;
template Data_API const Column<std::list<Int16>>& RecordSet::column<std::list<Int16>>(std::size_t pos) const;
template Data_API const Column<std::list<UInt16>>& RecordSet::column<std::list<UInt16>>(std::size_t pos) const;
template Data_API const Column<std::list<Int32>>& RecordSet::column<std::list<Int32>>(std::size_t pos) const;
template Data_API const Column<std::list<UInt32>>& RecordSet::column<std::list<UInt32>>(std::size_t pos) const;
template Data_API const Column<std::list<Int64>>& RecordSet::column<std::list<Int64>>(std::size_t pos) const;
template Data_API const Column<std::list<UInt64>>& RecordSet::column<std::list<UInt64>>(std::size_t pos) const;
template Data_API const Column<std::list<float>>& RecordSet::column<std::list<float>>(std::size_t pos) const;
template Data_API const Column<std::list<double>>& RecordSet::column<std::list<double>>(std::size_t pos) const;
template Data_API const Column<std::list<std::string>>& RecordSet::column<std::list<std::string>>(std::size_t pos) const;
template Data_API const Column<std::list<UTF16String>>& RecordSet::column<std::list<UTF16String>>(std::size_t pos) const;
template Data_API const Column<std::list<BLOB>>& RecordSet::column<std::list<BLOB>>(std::size_t pos) const;
template Data_API const Column<std::list<CLOB>>& RecordSet::column<std::list<CLOB>>(std::size_t pos) const;
template Data_API const Column<std::list<Date>>& RecordSet::column<std::list<Date>>(std::size_t pos) const;
template Data_API const Column<std::list<Time>>& RecordSet::column<std::list<Time>>(std::size_t pos) const;
template Data_API const Column<std::list<DateTime>>& RecordSet::column<std::list<DateTime>>(std::size_t pos) const;
template Data_API const Column<std::list<UUID>>& RecordSet::column<std::list<UUID>>(std::size_t pos) const;

template Data_API const Column<std::deque<bool>>& RecordSet::column<std::deque<bool>>(std::size_t pos) const;
template Data_API const Column<std::deque<UInt8>>& RecordSet::column<std::deque<UInt8>>(std::size_t pos) const;
template Data_API const Column<std::deque<Int16>>& RecordSet::column<std::deque<Int16>>(std::size_t pos) const;
template Data_API const Column<std::deque<UInt16>>& RecordSet::column<std::deque<UInt16>>(std::size_t pos) const;
template Data_API const Column<std::deque<Int32>>& RecordSet::column<std::deque<Int32>>(std::size_t pos) const;
template Data_API const Column<std::deque<UInt32>>& RecordSet::column<std::deque<UInt32>>(std::size_t pos) const;
template Data_API const Column<std::deque<Int64>>& RecordSet::column<std::deque<Int64>>(std::size_t pos) const;
template Data_API const Column<std::deque<UInt64>>& RecordSet::column<std::deque<UInt64>>(std::size_t pos) const;
template Data_API const Column<std::deque<float>>& RecordSet::column<std::deque<float>>(std::size_t pos) const;
template Data_API const Column<std::deque<double>>& RecordSet::column<std::deque<double>>(std::size_t pos) const;
template Data_API const Column<std::deque<std::string>>& RecordSet::column<std::deque<std::string>>(std::size_t pos) const;
template Data_API const Column<std::deque<UTF16String>>& RecordSet::column<std::deque<UTF16String>>(std::size_t pos) const;
template Data_API const Column<std::deque<BLOB>>& RecordSet::column<std::deque<BLOB>>(std::size_t pos) const;
template Data_API const Column<std::deque<CLOB>>& RecordSet::column<std::deque<CLOB>>(std::size_t pos) const;
template Data_API const Column<std::deque<Date>>& RecordSet::column<std::deque<Date>>(std::size_t pos) const;
template Data_API const Column<std::deque<Time>>& RecordSet::column<std::deque<Time>>(std::size_t pos) const;
template Data_API const Column<std::deque<DateTime>>& RecordSet::column<std::deque<DateTime>>(std::size_t pos) const;
template Data_API const Column<std::deque<UUID>>& RecordSet::column<std::deque<UUID>>(std::size_t pos) const;


template <class T>
const T& RecordSet::value(std::size_t col, std::size_t row, bool useFilter) const
	/// Returns the reference to data value at [col, row] location.
{
	if (useFilter && isFiltered() && !isAllowed(row))
		throw InvalidAccessException("Row not allowed");

	switch (storage())
	{
		case STORAGE_VECTOR:
		{
			using C = typename std::vector<T>;
			return column<C>(col).value(row);
		}
		case STORAGE_LIST:
		{
			using C = typename std::list<T>;
			return column<C>(col).value(row);
		}
		case STORAGE_DEQUE:
		case STORAGE_UNKNOWN:
		{
			using C = typename std::deque<T>;
			return column<C>(col).value(row);
		}
		default:
			throw IllegalStateException("Invalid storage setting.");
	}
}


template Data_API const bool& RecordSet::value<bool>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UInt8& RecordSet::value<UInt8>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const Int16& RecordSet::value<Int16>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UInt16& RecordSet::value<UInt16>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const Int32& RecordSet::value<Int32>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UInt32& RecordSet::value<UInt32>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const Int64& RecordSet::value<Int64>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UInt64& RecordSet::value<UInt64>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const float& RecordSet::value<float>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const double& RecordSet::value<double>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const std::string& RecordSet::value<std::string>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UTF16String& RecordSet::value<UTF16String>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const BLOB& RecordSet::value<BLOB>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const CLOB& RecordSet::value<CLOB>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const Date& RecordSet::value<Date>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const Time& RecordSet::value<Time>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const DateTime& RecordSet::value<DateTime>(std::size_t col, std::size_t row, bool useFilter) const;
template Data_API const UUID& RecordSet::value<UUID>(std::size_t col, std::size_t row, bool useFilter) const;


template <class T>
const T& RecordSet::value(const std::string& name, std::size_t row, bool useFilter) const
	/// Returns the reference to data value at named column, row location.
{
	if (useFilter && isFiltered() && !isAllowed(row))
		throw InvalidAccessException("Row not allowed");

	switch (storage())
	{
		case STORAGE_VECTOR:
		{
			using C = typename std::vector<T>;
			return column<C>(name).value(row);
		}
		case STORAGE_LIST:
		{
			using C = typename std::list<T>;
			return column<C>(name).value(row);
		}
		case STORAGE_DEQUE:
		case STORAGE_UNKNOWN:
		{
			using C = typename std::deque<T>;
			return column<C>(name).value(row);
		}
		default:
			throw IllegalStateException("Invalid storage setting.");
	}
}


template Data_API const bool& RecordSet::value<bool>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UInt8& RecordSet::value<UInt8>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const Int16& RecordSet::value<Int16>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UInt16& RecordSet::value<UInt16>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const Int32& RecordSet::value<Int32>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UInt32& RecordSet::value<UInt32>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const Int64& RecordSet::value<Int64>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UInt64& RecordSet::value<UInt64>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const float& RecordSet::value<float>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const double& RecordSet::value<double>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const std::string& RecordSet::value<std::string>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UTF16String& RecordSet::value<UTF16String>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const BLOB& RecordSet::value<BLOB>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const CLOB& RecordSet::value<CLOB>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const Date& RecordSet::value<Date>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const Time& RecordSet::value<Time>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const DateTime& RecordSet::value<DateTime>(const std::string& name, std::size_t row, bool useFilter) const;
template Data_API const UUID& RecordSet::value<UUID>(const std::string& name, std::size_t row, bool useFilter) const;


Poco::Dynamic::Var RecordSet::value(std::size_t col, std::size_t row, bool useFilter) const
{
	if (useFilter && isFiltered() && !isAllowed(row))
		throw InvalidAccessException("Row not allowed");

	if (isNull(col, row)) return Poco::Dynamic::Var();

	switch (columnType(col))
	{
	case MetaColumn::FDT_BOOL:      return value<bool>(col, row, useFilter);
	case MetaColumn::FDT_INT8:      return value<Int8>(col, row, useFilter);
	case MetaColumn::FDT_UINT8:     return value<UInt8>(col, row, useFilter);
	case MetaColumn::FDT_INT16:     return value<Int16>(col, row, useFilter);
	case MetaColumn::FDT_UINT16:    return value<UInt16>(col, row, useFilter);
	case MetaColumn::FDT_INT32:	    return value<Int32>(col, row, useFilter);
	case MetaColumn::FDT_UINT32:    return value<UInt32>(col, row, useFilter);
	case MetaColumn::FDT_INT64:     return value<Int64>(col, row, useFilter);
	case MetaColumn::FDT_UINT64:    return value<UInt64>(col, row, useFilter);
	case MetaColumn::FDT_FLOAT:     return value<float>(col, row, useFilter);
	case MetaColumn::FDT_DOUBLE:    return value<double>(col, row, useFilter);
	case MetaColumn::FDT_STRING:    return value<std::string>(col, row, useFilter);
	case MetaColumn::FDT_WSTRING:   return value<UTF16String>(col, row, useFilter);
	case MetaColumn::FDT_BLOB:      return value<BLOB>(col, row, useFilter);
	case MetaColumn::FDT_CLOB:      return value<CLOB>(col, row, useFilter);
	case MetaColumn::FDT_DATE:      return value<Date>(col, row, useFilter);
	case MetaColumn::FDT_TIME:      return value<Time>(col, row, useFilter);
	case MetaColumn::FDT_TIMESTAMP: return value<DateTime>(col, row);
	case MetaColumn::FDT_UUID:      return value<UUID>(col, row);
	case MetaColumn::FDT_JSON:      return value<std::string>(col, row, useFilter);
	default:
		throw UnknownTypeException("Data type not supported.");
	}
}


Poco::Dynamic::Var RecordSet::value(const std::string& name, std::size_t row, bool useFilter) const
{
	if (useFilter && isFiltered() && !isAllowed(row))
		throw InvalidAccessException("Row not allowed");

	if (isNull(metaColumn(name).position(), row)) return Poco::Dynamic::Var();

	switch (columnType(name))
	{
	case MetaColumn::FDT_BOOL:      return value<bool>(name, row, useFilter);
	case MetaColumn::FDT_INT8:      return value<Int8>(name, row, useFilter);
	case MetaColumn::FDT_UINT8:     return value<UInt8>(name, row, useFilter);
	case MetaColumn::FDT_INT16:     return value<Int16>(name, row, useFilter);
	case MetaColumn::FDT_UINT16:    return value<UInt16>(name, row, useFilter);
	case MetaColumn::FDT_INT32:	    return value<Int32>(name, row, useFilter);
	case MetaColumn::FDT_UINT32:    return value<UInt32>(name, row, useFilter);
	case MetaColumn::FDT_INT64:     return value<Int64>(name, row, useFilter);
	case MetaColumn::FDT_UINT64:    return value<UInt64>(name, row, useFilter);
	case MetaColumn::FDT_FLOAT:     return value<float>(name, row, useFilter);
	case MetaColumn::FDT_DOUBLE:    return value<double>(name, row, useFilter);
	case MetaColumn::FDT_STRING:    return value<std::string>(name, row, useFilter);
	case MetaColumn::FDT_WSTRING:   return value<UTF16String>(name, row, useFilter);
	case MetaColumn::FDT_BLOB:      return value<BLOB>(name, row, useFilter);
	case MetaColumn::FDT_CLOB:      return value<CLOB>(name, row, useFilter);
	case MetaColumn::FDT_DATE:      return value<Date>(name, row, useFilter);
	case MetaColumn::FDT_TIME:      return value<Time>(name, row, useFilter);
	case MetaColumn::FDT_TIMESTAMP: return value<DateTime>(name, row, useFilter);
	case MetaColumn::FDT_UUID:      return value<UUID>(name, row, useFilter);
	default:
		throw UnknownTypeException("Data type not supported.");
	}
}


Row& RecordSet::row(std::size_t pos)
{
	std::size_t rowCnt = rowCount();
	if (0 == rowCnt || pos > rowCnt - 1)
		throw RangeException("Invalid recordset row requested.");

	RowMap::const_iterator it = _rowMap.find(pos);
	Row* pRow = 0;
	std::size_t columns = columnCount();
	if (it == _rowMap.end())
	{
		if (_rowMap.size())
		{
			//reuse first row column names and sorting fields to save some memory
			pRow = new Row(_rowMap.begin()->second->names(),
				_rowMap.begin()->second->getSortMap(),
				getRowFormatter());

			for (std::size_t col = 0; col < columns; ++col)
				pRow->set(col, value(col, pos));
		}
		else
		{
			pRow = new Row;
			pRow->setFormatter(getRowFormatter());
			for (std::size_t col = 0; col < columns; ++col)
				pRow->append(metaColumn(static_cast<UInt32>(col)).name(), value(col, pos));
		}

		_rowMap.insert(RowMap::value_type(pos, pRow));
	}
	else
	{
		pRow = it->second;
		poco_check_ptr (pRow);
	}

	return *pRow;
}


std::size_t RecordSet::rowCount() const
{
	if (extractions().size() == 0) return 0;

	std::size_t rc = subTotalRowCount();
	if (!isFiltered()) return rc;

	std::size_t counter = 0;
	for (int row = 0; row < rc; ++row)
	{
		if (isAllowed(row)) ++counter;
	}

	return counter;
}


std::size_t RecordSet::affectedRowCount() const
{
	return Statement::affectedRowCount();
}


bool RecordSet::isAllowed(std::size_t row) const
{
	if (!isFiltered()) return true;
	return _pFilter->isAllowed(row);
}


bool RecordSet::moveFirst()
{
	if (subTotalRowCount() > 0)
	{
		if (!isFiltered())
		{
			_currentRow = 0;
			return true;
		}

		std::size_t currentRow = 0;
		while (!isAllowed(currentRow))
		{
			if (currentRow >= subTotalRowCount() - 1) return false;
			++currentRow;
		}

		_currentRow = currentRow;
		return true;
	}
	else return false;
}


bool RecordSet::moveNext()
{
	std::size_t currentRow = _currentRow;
	do
	{
		if (currentRow >= subTotalRowCount() - 1) return false;
		++currentRow;
	} while (isFiltered() && !isAllowed(currentRow));

	_currentRow = currentRow;
	return true;
}


bool RecordSet::movePrevious()
{
	std::size_t currentRow = _currentRow;
	do
	{
		if (currentRow <= 0) return false;
		--currentRow;
	} while (isFiltered() && !isAllowed(currentRow));

	_currentRow = currentRow;
	return true;
}


bool RecordSet::moveLast()
{
	if (subTotalRowCount() > 0)
	{
		std::size_t currentRow = subTotalRowCount() - 1;
		if (!isFiltered())
		{
			_currentRow = currentRow;
			return true;
		}

		while (!isAllowed(currentRow))
		{
			if (currentRow <= 0) return false;
			--currentRow;
		}

		_currentRow = currentRow;
		return true;
	}
	else return false;
}


void RecordSet::setRowFormatter(RowFormatter::Ptr pRowFormatter)
{
	pRowFormatter->setTotalRowCount(static_cast<int>(getTotalRowCount()));
	Statement::setRowFormatter(pRowFormatter);
	RowMap::iterator it = _rowMap.begin();
	RowMap::iterator end = _rowMap.end();
	for (; it != end; ++it) it->second->setFormatter(getRowFormatter());
}


std::ostream& RecordSet::copyNames(std::ostream& os) const
{
	std::string names = (*_pBegin)->namesToString();
	if (!names.empty()) os << names;
	return os;
}


std::ostream& RecordSet::copyValues(std::ostream& os, std::size_t offset, std::size_t length) const
{
	RowIterator it = *_pBegin + offset;
	RowIterator end = (RowIterator::POSITION_END != length) ? it + length : *_pEnd;
	std::copy(it, end, std::ostream_iterator<Row>(os));
	return os;
}


void RecordSet::formatValues(std::size_t offset, std::size_t length) const
{
	RowIterator it = *_pBegin + offset;
	RowIterator end = (RowIterator::POSITION_END != length) ? it + length : *_pEnd;
	std::string val;
	for (; it != end; ++it) it->formatValues();
}


std::ostream& RecordSet::copy(std::ostream& os, std::size_t offset, std::size_t length) const
{
	RowFormatter& rf = const_cast<RowFormatter&>((*_pBegin)->getFormatter());
	rf.setTotalRowCount(static_cast<int>(getTotalRowCount()));
	if (RowFormatter::FORMAT_PROGRESSIVE == rf.getMode())
	{
		os << rf.prefix();
		copyNames(os);
		copyValues(os, offset, length);
		os << rf.postfix();
	}
	else
	{
		formatNames();
		formatValues(offset, length);
		os << rf.toString();
	}

	return os;
}


void RecordSet::filter(const Poco::AutoPtr<RowFilter>& pFilter)
{
	_pFilter = pFilter;
}


bool RecordSet::isFiltered() const
{
	return _pFilter && !_pFilter->isEmpty();
}


void RecordSet::setTotalRowCount(const std::string& sql)
{
	session() << sql, into(_totalRowCount), now;
}


} } // namespace Poco::Data
