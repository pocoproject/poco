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


#include "Poco/SQL/RecordSet.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/SQL/SQLException.h"
#include "Poco/DateTime.h"
#include "Poco/UTFString.h"


using namespace Poco::SQL::Keywords;
using Poco::DateTime;
using Poco::UTF16String;


namespace Poco {
namespace SQL {


RecordSet::RecordSet(const Statement& rStatement,
	RowFormatter::Ptr pRowFormatter):
	Statement(rStatement),
	_currentRow(0),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true))
{
	if (pRowFormatter) setRowFormatter(pRowFormatter);
}


RecordSet::RecordSet(Session& rSession,
	const std::string& query,
	RowFormatter::Ptr pRowFormatter):
	Statement((rSession << query, now)),
	_currentRow(0),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true))
{
	if (pRowFormatter) setRowFormatter(pRowFormatter);
}


RecordSet::RecordSet(const RecordSet& other): Statement(other),
	_currentRow(other._currentRow),
	_pBegin(new RowIterator(this, 0 == rowsExtracted())),
	_pEnd(new RowIterator(this, true)),
	_pFilter(other._pFilter)
{
}


RecordSet::RecordSet(RecordSet&& other): Statement(std::move(other))
{
	_currentRow = other._currentRow; other._currentRow = 0;
	_pBegin = other._pBegin; other._pBegin = nullptr;
	_pEnd = other._pEnd; other._pEnd = nullptr;
	_pFilter = other._pFilter; other._pFilter = nullptr;
	_rowMap = std::move(other._rowMap); other._rowMap.clear();
}


RecordSet::~RecordSet()
{
	try
	{
		delete _pBegin;
		delete _pEnd;

		RowMap::iterator it = _rowMap.begin();
		RowMap::iterator itEnd = _rowMap.end();
		for (; it != itEnd; ++it) delete it->second;
	}
	catch (...)
	{
		poco_unexpected();
	}
}


RecordSet& RecordSet::reset(const Statement& stmt)
{
	delete _pBegin;
	_pBegin = 0;
	delete _pEnd;
	_pEnd = 0;
	_currentRow = 0;
	Statement::setTotalRowCount(StatementImpl::UNKNOWN_TOTAL_ROW_COUNT);
	
	RowMap::iterator it = _rowMap.begin();
	RowMap::iterator end = _rowMap.end();
	for (; it != end; ++it) delete it->second;
	_rowMap.clear();

	Statement::operator = (stmt);

	_pBegin = new RowIterator(this, 0 == rowsExtracted());
	_pEnd = new RowIterator(this, true);

	return *this;
}


Poco::Dynamic::Var RecordSet::value(std::size_t col, std::size_t dataRow, bool useFilter) const
{
	if (useFilter && isFiltered() && !isAllowed(dataRow))
		throw InvalidAccessException("Row not allowed");

	if (isNull(col, dataRow)) return Poco::Dynamic::Var();

	switch (columnType(col))
	{
	case MetaColumn::FDT_BOOL:      return value<bool>(col, dataRow, useFilter);
	case MetaColumn::FDT_INT8:      return value<Int8>(col, dataRow, useFilter);
	case MetaColumn::FDT_UINT8:     return value<UInt8>(col, dataRow, useFilter);
	case MetaColumn::FDT_INT16:     return value<Int16>(col, dataRow, useFilter);
	case MetaColumn::FDT_UINT16:    return value<UInt16>(col, dataRow, useFilter);
	case MetaColumn::FDT_INT32:     return value<Int32>(col, dataRow, useFilter);
	case MetaColumn::FDT_UINT32:    return value<UInt32>(col, dataRow, useFilter);
	case MetaColumn::FDT_INT64:     return value<Int64>(col, dataRow, useFilter);
	case MetaColumn::FDT_UINT64:    return value<UInt64>(col, dataRow, useFilter);
	case MetaColumn::FDT_FLOAT:     return value<float>(col, dataRow, useFilter);
	case MetaColumn::FDT_DOUBLE:    return value<double>(col, dataRow, useFilter);
	case MetaColumn::FDT_STRING:    return value<std::string>(col, dataRow, useFilter);
	case MetaColumn::FDT_WSTRING:   return value<UTF16String>(col, dataRow, useFilter);
	case MetaColumn::FDT_BLOB:      return value<BLOB>(col, dataRow, useFilter);
	case MetaColumn::FDT_CLOB:      return value<CLOB>(col, dataRow, useFilter);
	case MetaColumn::FDT_DATE:      return value<Date>(col, dataRow, useFilter);
	case MetaColumn::FDT_TIME:      return value<Time>(col, dataRow, useFilter);
	case MetaColumn::FDT_TIMESTAMP: return value<DateTime>(col, dataRow);
	default:
		throw UnknownTypeException("Data type not supported.");
	}
}


Poco::Dynamic::Var RecordSet::value(const std::string& name, std::size_t dataRow, bool useFilter) const
{
	if (useFilter && isFiltered() && !isAllowed(dataRow))
		throw InvalidAccessException("Row not allowed");

	if (isNull(metaColumn(name).position(), dataRow)) return Poco::Dynamic::Var();

	switch (columnType(name))
	{
	case MetaColumn::FDT_BOOL:      return value<bool>(name, dataRow, useFilter);
	case MetaColumn::FDT_INT8:      return value<Int8>(name, dataRow, useFilter);
	case MetaColumn::FDT_UINT8:     return value<UInt8>(name, dataRow, useFilter);
	case MetaColumn::FDT_INT16:     return value<Int16>(name, dataRow, useFilter);
	case MetaColumn::FDT_UINT16:    return value<UInt16>(name, dataRow, useFilter);
	case MetaColumn::FDT_INT32:	    return value<Int32>(name, dataRow, useFilter);
	case MetaColumn::FDT_UINT32:    return value<UInt32>(name, dataRow, useFilter);
	case MetaColumn::FDT_INT64:     return value<Int64>(name, dataRow, useFilter);
	case MetaColumn::FDT_UINT64:    return value<UInt64>(name, dataRow, useFilter);
	case MetaColumn::FDT_FLOAT:     return value<float>(name, dataRow, useFilter);
	case MetaColumn::FDT_DOUBLE:    return value<double>(name, dataRow, useFilter);
	case MetaColumn::FDT_STRING:    return value<std::string>(name, dataRow, useFilter);
	case MetaColumn::FDT_WSTRING:   return value<UTF16String>(name, dataRow, useFilter);
	case MetaColumn::FDT_BLOB:      return value<BLOB>(name, dataRow, useFilter);
	case MetaColumn::FDT_DATE:      return value<Date>(name, dataRow, useFilter);
	case MetaColumn::FDT_TIME:      return value<Time>(name, dataRow, useFilter);
	case MetaColumn::FDT_TIMESTAMP: return value<DateTime>(name, dataRow, useFilter);
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
	if (0 == extractions().size() && 0 == columnsExtracted())
		return 0;
	poco_assert (extractions().size());
	std::size_t rc = storageRowCount();
	if (!isFiltered()) return rc;

	std::size_t counter = 0;
	for (int dataRow = 0; dataRow < rc; ++dataRow)
	{
		if (isAllowed(dataRow)) ++counter;
	}

	return counter;
}


bool RecordSet::isAllowed(std::size_t dataRow) const
{
	if (!isFiltered()) return true;
	return _pFilter->isAllowed(dataRow);
}


bool RecordSet::moveFirst()
{
	const size_t rc = storageRowCount();
	if (rc > 0)
	{
		if (!isFiltered())
		{
			_currentRow = 0;
			return true;
		}

		std::size_t currentRow = 0;
		while (!isAllowed(currentRow))
		{
			if (currentRow >= rc - 1) return false;
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
		if (currentRow >= storageRowCount() -1) return false;
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
	if (storageRowCount() > 0)
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
	if (pRowFormatter)
	{
		if (pRowFormatter->getTotalRowCount() == RowFormatter::INVALID_ROW_COUNT)
			pRowFormatter->setTotalRowCount(static_cast<int>(getTotalRowCount()));

		Statement::setRowFormatter(pRowFormatter);
		RowMap::iterator it = _rowMap.begin();
		RowMap::iterator itEnd = _rowMap.end();
		for (; it != itEnd; ++it) it->second->setFormatter(getRowFormatter());
	}
	else
		throw NullPointerException("Null RowFormatter in RecordSet.");
}


std::ostream& RecordSet::copyNames(std::ostream& os) const
{
	if (begin() == end()) return os;

	std::string names = (*_pBegin)->namesToString();
	if (!names.empty()) os << names;
	return os;
}


std::ostream& RecordSet::copyValues(std::ostream& os, std::size_t offset, std::size_t length) const
{
	if (begin() == end()) return os;

	RowIterator it = *_pBegin + offset;
	RowIterator itEnd = (RowIterator::POSITION_END != length) ? it + length : *_pEnd;
	std::copy(it, itEnd, std::ostream_iterator<Row>(os));
	return os;
}


void RecordSet::formatValues(std::size_t offset, std::size_t length) const
{
	if (begin() == end()) return;

	RowIterator it = *_pBegin + offset;
	RowIterator itEnd = (RowIterator::POSITION_END != length) ? it + length : *_pEnd;
	std::string val;
	for (; it != itEnd; ++it) it->formatValues();
}


std::ostream& RecordSet::copy(std::ostream& os, std::size_t offset, std::size_t length) const
{
	if (begin() == end()) return os;

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


} } // namespace Poco::SQL
