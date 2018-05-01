//
// RecordSet.h
//
// Library: Data
// Package: DataCore
// Module:  RecordSet
//
// Definition of the RecordSet class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_RecordSet_INCLUDED
#define SQL_RecordSet_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/Session.h"
#include "Poco/SQL/Extraction.h"
#include "Poco/SQL/BulkExtraction.h"
#include "Poco/SQL/Statement.h"
#include "Poco/SQL/RowIterator.h"
#include "Poco/SQL/RowFilter.h"
#include "Poco/SQL/LOB.h"
#include "Poco/String.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include <ostream>
#include <limits>


namespace Poco {
namespace SQL {


class Poco_SQL_API RecordSet: private Statement
	/// RecordSet provides access to data returned from a query.
	/// Data access indices (row and column) are 0-based, as usual in C++.
	///
	/// Recordset provides navigation methods to iterate through the
	/// recordset, retrieval methods to extract data, and methods
	/// to get metadata (type, etc.) about columns.
	///
	/// To work with a RecordSet, first create a Statement, execute it, and
	/// create the RecordSet from the Statement, as follows:
	///
	///     Statement select(session);
	///     select << "SELECT * FROM Person";
	///     select.execute();
	///     RecordSet rs(select);
	///
	/// The shorter way to do the above is following:
	///
	///     RecordSet rs(session, "SELECT * FROM Person"[, new SimpleRowFormatter]);
	///
	/// The third (optional) argument passed to the Recordset constructor is a RowFormatter
	/// implementation. The formatter is used in conjunction with << operator for recordset
	/// data formatting.
	///
	/// The number of rows in the RecordSet can be limited by specifying
	/// a limit for the Statement.
{
public:
	typedef std::map<std::size_t, Row*> RowMap;
	typedef const RowIterator           ConstIterator;
	typedef RowIterator                 Iterator;

	using Statement::isNull;
	using Statement::subTotalRowCount;
	using Statement::totalRowCount;

	explicit RecordSet(const Statement& rStatement,
		RowFormatter::Ptr pRowFormatter = 0);
		/// Creates the RecordSet.

	RecordSet(Session& rSession,
		const std::string& query,
		RowFormatter::Ptr pRowFormatter = 0);
		/// Creates the RecordSet.

	RecordSet(Session& rSession,
		const std::string& query,
		const RowFormatter& rowFormatter);
		/// Creates the RecordSet.

	template <class RF>
	RecordSet(Session& rSession, const std::string& query, const RF& rowFormatter):
		Statement((rSession << query, Keywords::now)),
		_currentRow(0),
		_pBegin(new RowIterator(this, 0 == rowsExtracted())),
		_pEnd(new RowIterator(this, true))
		/// Creates the RecordSet.
	{
		setRowFormatter(Keywords::format(rowFormatter));
	}

	RecordSet(const RecordSet& other);
		/// Copy-creates the recordset.

	RecordSet(RecordSet&& other);
	/// Move-creates the recordset.

	~RecordSet();
		/// Destroys the RecordSet.

	void setRowFormatter(RowFormatter::Ptr pRowFormatter);
		/// Assigns the row formatter to the statement and all recordset rows.

	Statement& operator = (const Statement& stmt);
		/// Assignment operator.

	std::size_t rowCount() const;
		/// Returns the number of rows in the RecordSet.
		/// The number of rows reported is dependent on filtering.
		/// Due to the need for filter conditions checking,
		/// this function may suffer significant performance penalty
		/// for large recordsets, so it should be used judiciously.
		/// Use totalRowCount() to obtain the total number of rows.

	std::size_t extractedRowCount() const;
		/// Returns the number of rows extracted during the last statement
		/// execution.
		/// The number of rows reported is independent of filtering.

	std::size_t columnCount() const;
		/// Returns the number of columns in the recordset.

	template <class C>
	const Column<C>& column(const std::string& name) const
		/// Returns the reference to the first Column with the specified name.
	{
		if (isBulkExtraction())
		{
			typedef InternalBulkExtraction<C> E;
			return columnImpl<C,E>(name);
		}
		else
		{
			typedef InternalExtraction<C> E;
			return columnImpl<C,E>(name);
		}
	}

	template <class C>
	const Column<C>& column(std::size_t pos) const
		/// Returns the reference to column at specified position.
	{
		if (isBulkExtraction())
		{
			typedef InternalBulkExtraction<C> E;
			return columnImpl<C,E>(pos);
		}
		else
		{
			typedef InternalExtraction<C> E;
			return columnImpl<C,E>(pos);
		}
	}

	Row& row(std::size_t pos);
		/// Returns reference to row at position pos.
		/// Rows are lazy-created and cached.

	template <class T>
	const T& value(std::size_t col) const;
		/// Returns the reference to data value at [col] location.

	template <class T>
	const T& value(std::size_t col, std::size_t dataRow, bool useFilter = true) const
		/// Returns the reference to data value at [col, row] location.
	{
		if (useFilter && isFiltered() && !isAllowed(dataRow))
			throw InvalidAccessException("Row not allowed");

		switch (storage())
		{
			case STORAGE_VECTOR:
			{
				typedef typename std::vector<T> C;
				return column<C>(col).value(dataRow);
			}
			case STORAGE_LIST:
			{
				typedef typename std::list<T> C;
				return column<C>(col).value(dataRow);
			}
			case STORAGE_DEQUE:
			case STORAGE_UNKNOWN:
			{
				typedef typename std::deque<T> C;
				return column<C>(col).value(dataRow);
			}
			default:
				throw IllegalStateException("Invalid storage setting.");
		}
	}

	template <class T>
	const T& value(const std::string& name, std::size_t dataRow, bool useFilter = true) const
		/// Returns the reference to data value at named column, row location.
	{
		if (useFilter && isFiltered() && !isAllowed(dataRow))
			throw InvalidAccessException("Row not allowed");

		switch (storage())
		{
			case STORAGE_VECTOR:
			{
				typedef typename std::vector<T> C;
				return column<C>(name).value(dataRow);
			}
			case STORAGE_LIST:
			{
				typedef typename std::list<T> C;
				return column<C>(name).value(dataRow);
			}
			case STORAGE_DEQUE:
			case STORAGE_UNKNOWN:
			{
				typedef typename std::deque<T> C;
				return column<C>(name).value(dataRow);
			}
			default:
				throw IllegalStateException("Invalid storage setting.");
		}
	}

	Poco::Dynamic::Var value(std::size_t col, std::size_t row, bool checkFiltering = true) const;
		/// Returns the data value at column, row location.

	Poco::Dynamic::Var value(const std::string& name, std::size_t row, bool checkFiltering = true) const;
		/// Returns the data value at named column, row location.

	template <typename T>
	Poco::Dynamic::Var nvl(const std::string& name, const T& deflt = T()) const
		/// Returns the value in the named column of the current row
		/// if the value is not NULL, or deflt otherwise.
	{
		if (isNull(name))
			return Poco::Dynamic::Var(deflt);
		else
			return value(name, _currentRow);
	}
	
	template <typename T>
	Poco::Dynamic::Var nvl(std::size_t index, const T& deflt = T()) const
		/// Returns the value in the given column of the current row
		/// if the value is not NULL, or deflt otherwise.
	{
		if (isNull(index, _currentRow))
			return Poco::Dynamic::Var(deflt);
		else
			return value(index, _currentRow);
	}

	ConstIterator& begin() const;
		/// Returns the const row iterator.

	ConstIterator& end() const;
		/// Returns the const row iterator.

	Iterator begin();
		/// Returns the row iterator.

	Iterator end();
		/// Returns the row iterator.

	bool moveFirst();
		/// Moves the row cursor to the first row.
		///
		/// Returns true if there is at least one row in the RecordSet,
		/// false otherwise.

	bool moveNext();
		/// Moves the row cursor to the next row.
		///
		/// Returns true if the row is available, or false
		/// if the end of the record set has been reached and
		/// no more rows are available.

	bool movePrevious();
		/// Moves the row cursor to the previous row.
		///
		/// Returns true if the row is available, or false
		/// if there are no more rows available.

	bool moveLast();
		/// Moves the row cursor to the last row.
		///
		/// Returns true if there is at least one row in the RecordSet,
		/// false otherwise.

	using Statement::reset;
		/// Don't hide base class method.

	RecordSet& reset(const Statement& stmt);
		/// Resets the RecordSet and assigns a new statement.
		/// Should be called after the given statement has been reset,
		/// assigned a new SQL statement, and executed.
		///
		/// Does not remove the associated RowFilter or RowFormatter.

	Poco::Dynamic::Var value(const std::string& name) const;
		/// Returns the value in the named column of the current row.

	Poco::Dynamic::Var value(std::size_t index) const;
		/// Returns the value in the given column of the current row.

	Poco::Dynamic::Var operator [] (const std::string& name) const;
		/// Returns the value in the named column of the current row.

	Poco::Dynamic::Var operator [] (std::size_t index) const;
		/// Returns the value in the named column of the current row.

	MetaColumn::ColumnDataType columnType(std::size_t pos) const;
		/// Returns the type for the column at specified position.

	MetaColumn::ColumnDataType columnType(const std::string& name) const;
		/// Returns the type for the column with specified name.

	const std::string& columnName(std::size_t pos) const;
		/// Returns column name for the column at specified position.

	std::size_t columnLength(std::size_t pos) const;
		/// Returns column maximum length for the column at specified position.

	std::size_t columnLength(const std::string& name) const;
		/// Returns column maximum length for the column with specified name.

	std::size_t columnPrecision(std::size_t pos) const;
		/// Returns column precision for the column at specified position.
		/// Valid for floating point fields only (zero for other data types).

	std::size_t columnPrecision(const std::string& name) const;
		/// Returns column precision for the column with specified name.
		/// Valid for floating point fields only (zero for other data types).

	bool isNull(const std::string& name) const;
		/// Returns true if column value of the current row is null.

	bool isNull(std::size_t& colNo) const;
	/// Returns true if column value of the current row is null.

	std::ostream& copyNames(std::ostream& os) const;
		/// Copies the column names to the target output stream.
		/// Copied string is formatted by the current RowFormatter.

	void formatNames() const;
		/// Formats names using the current RowFormatter.

	std::ostream& copyValues(std::ostream& os,
		std::size_t offset = 0,
		std::size_t length = RowIterator::POSITION_END) const;
		/// Copies the data values to the supplied output stream.
		/// The data set to be copied is starting at the specified offset
		/// from the recordset beginning. The number of rows to be copied
		/// is specified by length argument.
		/// An invalid combination of offset/length arguments shall
		/// cause RangeException to be thrown.
		/// Copied string is formatted by the current RowFormatter.

	void formatValues(std::size_t offset, std::size_t length) const;
		/// Formats values using the current RowFormatter.
		/// The data set to be formatted is starting at the specified offset
		/// from the recordset beginning. The number of rows to be copied
		/// is specified by length argument.
		/// An invalid combination of offset/length arguments shall
		/// cause RangeException to be thrown.

	std::ostream& copy(std::ostream& os,
		std::size_t offset = 0,
		std::size_t length = RowIterator::POSITION_END) const;
		/// Copies the column names and values to the target output stream.
		/// Copied strings are formatted by the current RowFormatter.

	bool isFiltered() const;
		/// Returns true if recordset is filtered.

private:
	RecordSet();

	template<class C, class E>
	std::size_t columnPosition(const std::string& name) const
		/// Returns the position of the column with specified name.
	{
		typedef typename C::value_type T;
		typedef const E* ExtractionVecPtr;

		bool typeFound = false;

		const AbstractExtractionVec& rExtractions = extractions();
		AbstractExtractionVec::const_iterator it = rExtractions.begin();
		AbstractExtractionVec::const_iterator itEnd = rExtractions.end();
		
		for (; it != itEnd; ++it)
		{
			ExtractionVecPtr pExtraction = dynamic_cast<ExtractionVecPtr>(it->get());

			if (pExtraction)
			{
				typeFound = true;
				const Column<C>& col = pExtraction->column();
				if (0 == Poco::icompare(name, col.name()))
					return col.position();
			}
		}

		if (typeFound)
			throw NotFoundException(Poco::format("Column name: %s", name));
		else
			throw NotFoundException(Poco::format("Column type: %s, Container type: %s, name: %s",
				std::string(typeid(T).name()), std::string(typeid(ExtractionVecPtr).name()), name));
	}

	template <class C, class E>
	const Column<C>& columnImpl(const std::string& name) const
		/// Returns the reference to the first Column with the specified name.
	{
		return columnImpl<C,E>(columnPosition<C,E>(name));
	}

	template <class C, class E>
	const Column<C>& columnImpl(std::size_t pos) const
		/// Returns the reference to column at specified position.
	{
		typedef typename C::value_type T;
		typedef const E* ExtractionVecPtr;

		const AbstractExtractionVec& rExtractions = extractions();

		std::size_t s = rExtractions.size();
		if (0 == s || pos >= s)
			throw RangeException(Poco::format("Invalid column index: %z", pos));

		ExtractionVecPtr pExtraction = dynamic_cast<ExtractionVecPtr>(rExtractions[pos].get());

		if (pExtraction)
		{
			return pExtraction->column();
		}
		else
		{
			throw Poco::BadCastException(Poco::format("RecordSet::columnImpl(%z) type cast failed!\nTarget type:\t%s"
				"\nTarget container type:\t%s\nSource container type:\t%s\nSource abstraction type:\t%s",
				pos,
				std::string(typeid(T).name()),
				std::string(typeid(ExtractionVecPtr).name()),
				rExtractions[pos]->type(),
				std::string(typeid(rExtractions[pos].get()).name())));
		}
	}

	size_t storageRowCount() const;

	bool isAllowed(std::size_t row) const;
		/// Returns true if the specified row is allowed by the
		/// currently active filter.

	void filter(const Poco::AutoPtr<RowFilter>& pFilter);
		/// Sets the filter for the RecordSet.

	const Poco::AutoPtr<RowFilter>& getFilter() const;
		/// Returns the filter associated with the RecordSet.

	std::size_t  _currentRow;
	RowIterator* _pBegin;
	RowIterator* _pEnd;
	RowMap       _rowMap;
	Poco::AutoPtr<RowFilter> _pFilter;

	friend class RowIterator;
	friend class RowFilter;
};


///
/// inlines
///


inline Poco_SQL_API std::ostream& operator << (std::ostream &os, const RecordSet& rs)
{
	return rs.copy(os);
}


inline std::size_t RecordSet::extractedRowCount() const
{
	return rowsExtracted();
}


inline std::size_t RecordSet::columnCount() const
{
	return static_cast<std::size_t>(extractions().size());
}


inline Statement& RecordSet::operator = (const Statement& stmt)
{
	reset(stmt);
	return *this;
}


inline Poco::Dynamic::Var RecordSet::value(const std::string& name)  const
{
	return value(name, _currentRow);
}


inline Poco::Dynamic::Var RecordSet::value(std::size_t index) const
{
	return value(index, _currentRow);
}


inline Poco::Dynamic::Var RecordSet::operator [] (const std::string& name)  const
{
	return value(name, _currentRow);
}


inline Poco::Dynamic::Var RecordSet::operator [] (std::size_t index) const
{
	return value(index, _currentRow);
}

template <class T>
inline const T& RecordSet::value(std::size_t col) const
{
	return value<T>(col, _currentRow);
}


inline MetaColumn::ColumnDataType RecordSet::columnType(std::size_t pos)const
{
	return metaColumn(static_cast<UInt32>(pos)).type();
}


inline MetaColumn::ColumnDataType RecordSet::columnType(const std::string& name)const
{
	return metaColumn(name).type();
}


inline const std::string& RecordSet::columnName(std::size_t pos) const
{
	return metaColumn(static_cast<UInt32>(pos)).name();
}


inline std::size_t RecordSet::columnLength(std::size_t pos) const
{
	return metaColumn(static_cast<UInt32>(pos)).length();
}


inline std::size_t RecordSet::columnLength(const std::string& name)const
{
	return metaColumn(name).length();
}


inline std::size_t RecordSet::columnPrecision(std::size_t pos) const
{
	return metaColumn(static_cast<UInt32>(pos)).precision();
}


inline std::size_t RecordSet::columnPrecision(const std::string& name)const
{
	return metaColumn(name).precision();
}


inline bool RecordSet::isNull(const std::string& name) const
{
	return isNull(metaColumn(name).position(), _currentRow);
}


inline bool RecordSet::isNull(std::size_t& colNo) const
{
    return isNull(colNo, _currentRow);
}

inline RecordSet::ConstIterator& RecordSet::begin() const
{
	return *_pBegin;
}


inline RecordSet::ConstIterator& RecordSet::end() const
{
	return *_pEnd;
}


inline RecordSet::Iterator RecordSet::begin()
{
	return *_pBegin;
}


inline RecordSet::Iterator RecordSet::end()
{
	return *_pEnd;
}


inline const Poco::AutoPtr<RowFilter>& RecordSet::getFilter() const
{
	return _pFilter;
}


inline void RecordSet::formatNames() const
{
	(*_pBegin)->formatNames();
}


inline size_t RecordSet::storageRowCount() const
{
	return impl()->rowsExtracted();
}


} } // namespace Poco::SQL


#endif // Data_RecordSet_INCLUDED
