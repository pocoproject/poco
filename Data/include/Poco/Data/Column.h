//
// Column.h
//
// Library: Data
// Package: DataCore
// Module:  Column
//
// Definition of the Column class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Column_INCLUDED
#define Data_Column_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/SharedPtr.h"
#include "Poco/RefCountedObject.h"
#include <vector>
#include <list>
#include <deque>


namespace Poco {
namespace Data {


template <class C>
class Column
	/// Column class is column data container.
	/// Data (a pointer to underlying STL container) is assigned to the class
	/// at construction time. Construction with null pointer is not allowed.
	/// This class owns the data assigned to it and deletes the storage on destruction.
{
public:
	using Container = C;
	using ContainerPtr = Poco::SharedPtr<C>;
	using Iterator = typename C::const_iterator;
	using RIterator = typename C::const_reverse_iterator;
	using Size = typename C::size_type;
	using Type = typename C::value_type;

	Column(const MetaColumn& metaColumn, Container* pData):
		_metaColumn(metaColumn),
		_pData(pData)
		/// Creates the Column.
	{
		if (!_pData)
			throw NullPointerException("Container pointer must point to valid storage.");
	}

	Column(const Column& col):
		_metaColumn(col._metaColumn),
		_pData(col._pData)
		/// Creates the Column.
	{
	}

	Column(Column&& col) noexcept:
		_metaColumn(std::move(col._metaColumn)),
		_pData(std::move(col._pData))
		/// Creates the Column.
	{
	}

	~Column()
		/// Destroys the Column.
	{
	}

	Column& operator = (const Column& col)
		/// Assignment operator.
	{
		Column tmp(col);
		swap(tmp);
		return *this;
	}

	Column& operator = (Column&& col) noexcept
		/// Assignment operator.
	{
		_metaColumn = std::move(col._metaColumn);
		_pData = std::move(col._pData);
		return *this;
	}

	void swap(Column& other) noexcept
		/// Swaps the column with another one.
	{
		using std::swap;
		swap(_metaColumn, other._metaColumn);
		swap(_pData, other._pData);
	}

	Container& data()
		/// Returns reference to contained data.
	{
		return *_pData;
	}

	const Type& value(std::size_t row) const
		/// Returns the field value in specified row.
	{
		try
		{
			return _pData->at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	const Type& operator [] (std::size_t row) const
		/// Returns the field value in specified row.
	{
		return value(row);
	}

	Size rowCount() const
		/// Returns number of rows.
	{
		return _pData->size();
	}

	void reset()
		/// Clears and shrinks the storage.
	{
		Container().swap(*_pData);
	}

	const std::string& name() const
		/// Returns column name.
	{
		return _metaColumn.name();
	}

	std::size_t length() const
		/// Returns column maximum length.
	{
		return _metaColumn.length();
	}

	std::size_t precision() const
		/// Returns column precision.
		/// Valid for floating point fields only (zero for other data types).
	{
		return _metaColumn.precision();
	}

	std::size_t position() const
		/// Returns column position.
	{
		return _metaColumn.position();
	}

	MetaColumn::ColumnDataType type() const
		/// Returns column type.
	{
		return _metaColumn.type();
	}

	Iterator begin() const
		/// Returns iterator pointing to the beginning of data storage vector.
	{
		return _pData->begin();
	}

	Iterator end() const
		/// Returns iterator pointing to the end of data storage vector.
	{
		return _pData->end();
	}

private:
	Column();

	MetaColumn   _metaColumn;
	ContainerPtr _pData;
};


template <>
class Column<std::vector<bool>>
	/// The std::vector<bool> specialization for the Column class.
	///
	/// This specialization is necessary due to the nature of std::vector<bool>.
	/// For details, see the standard library implementation of vector<bool>
	/// or
	/// S. Meyers: "Effective STL" (Copyright Addison-Wesley 2001),
	/// Item 18: "Avoid using vector<bool>."
	///
	/// The workaround employed here is using deque<bool> as an
	/// internal "companion" container kept in sync with the vector<bool>
	/// column data.
{
public:
	using Container = std::vector<bool>;
	using ContainerPtr = Poco::SharedPtr<Container>;
	using Iterator = Container::const_iterator;
	using RIterator = Container::const_reverse_iterator;
	using Size = Container::size_type;

	Column(const MetaColumn& metaColumn, Container* pData):
		_metaColumn(metaColumn),
		_pData(pData)
		/// Creates the Column.
	{
		poco_check_ptr (_pData);
		_deque.assign(_pData->begin(), _pData->end());
	}

	Column(const Column& col):
		_metaColumn(col._metaColumn),
		_pData(col._pData)
		/// Creates the Column.
	{
		_deque.assign(_pData->begin(), _pData->end());
	}

	~Column()
		/// Destroys the Column.
	{
	}

	Column& operator = (const Column& col)
		/// Assignment operator.
	{
		Column tmp(col);
		swap(tmp);
		return *this;
	}

	void swap(Column& other) noexcept
		/// Swaps the column with another one.
	{
		using std::swap;
		swap(_metaColumn, other._metaColumn);
		swap(_pData, other._pData);
		swap(_deque, other._deque);
	}

	Container& data()
		/// Returns reference to contained data.
	{
		return *_pData;
	}

	const bool& value(std::size_t row) const
		/// Returns the field value in specified row.
	{
		if (_deque.size() < _pData->size())
			_deque.resize(_pData->size());

		try
		{
			return _deque.at(row) = _pData->at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	const bool& operator [] (std::size_t row) const
		/// Returns the field value in specified row.
	{
		return value(row);
	}

	Size rowCount() const
		/// Returns number of rows.
	{
		return _pData->size();
	}

	void reset()
		/// Clears and shrinks the storage.
	{
		Container().swap(*_pData);
		_deque.clear();
	}

	const std::string& name() const
		/// Returns column name.
	{
		return _metaColumn.name();
	}

	std::size_t length() const
		/// Returns column maximum length.
	{
		return _metaColumn.length();
	}

	std::size_t precision() const
		/// Returns column precision.
		/// Valid for floating point fields only (zero for other data types).
	{
		return _metaColumn.precision();
	}

	std::size_t position() const
		/// Returns column position.
	{
		return _metaColumn.position();
	}

	MetaColumn::ColumnDataType type() const
		/// Returns column type.
	{
		return _metaColumn.type();
	}

	Iterator begin() const
		/// Returns iterator pointing to the beginning of data storage vector.
	{
		return _pData->begin();
	}

	Iterator end() const
		/// Returns iterator pointing to the end of data storage vector.
	{
		return _pData->end();
	}

private:
	Column();

	MetaColumn               _metaColumn;
	ContainerPtr             _pData;
	mutable std::deque<bool> _deque;
};


template <class T>
class Column<std::list<T>>
	/// Column specialization for std::list
{
public:
	using Container = std::list<T>;
	using ContainerPtr = Poco::SharedPtr<Container>;
	using Iterator = typename Container::const_iterator;
	using RIterator = typename Container::const_reverse_iterator;
	using Size = typename Container::size_type;

	Column(const MetaColumn& metaColumn, std::list<T>* pData):
		_metaColumn(metaColumn),
		_pData(pData)
		/// Creates the Column.
	{
		poco_check_ptr (_pData);
	}

	Column(const Column& col):
		_metaColumn(col._metaColumn),
		_pData(col._pData)
		/// Creates the Column.
	{
	}

	~Column()
		/// Destroys the Column.
	{
	}

	Column& operator = (const Column& col)
		/// Assignment operator.
	{
		Column tmp(col);
		swap(tmp);
		return *this;
	}

	void swap(Column& other) noexcept
		/// Swaps the column with another one.
	{
		using std::swap;
		swap(_metaColumn, other._metaColumn);
		swap(_pData, other._pData);
	}

	Container& data()
		/// Returns reference to contained data.
	{
		return *_pData;
	}

	const T& value(std::size_t row) const
		/// Returns the field value in specified row.
		/// This is the std::list specialization and std::list
		/// is not the optimal solution for cases where random
		/// access is needed.
		/// However, to allow for compatibility with other
		/// containers, this functionality is provided here.
		/// To alleviate the problem, an effort is made
		/// to start iteration from beginning or end,
		/// depending on the position requested.
	{
		if (row <= (_pData->size() / 2))
		{
			Iterator it = _pData->begin();
			Iterator end = _pData->end();
			for (std::size_t i = 0; it != end; ++it, ++i)
				if (i == row) return *it;
		}
		else
		{
			row = _pData->size() - row;
			RIterator it = _pData->rbegin();
			const RIterator end = _pData->rend();
			for (std::size_t i = 1; it != end; ++it, ++i)
				if (i == row) return *it;
		}

		throw RangeException("Invalid row number.");
	}

	const T& operator [] (std::size_t row) const
		/// Returns the field value in specified row.
	{
		return value(row);
	}

	Size rowCount() const
		/// Returns number of rows.
	{
		return _pData->size();
	}

	void reset()
		/// Clears the storage.
	{
		_pData->clear();
	}

	const std::string& name() const
		/// Returns column name.
	{
		return _metaColumn.name();
	}

	std::size_t length() const
		/// Returns column maximum length.
	{
		return _metaColumn.length();
	}

	std::size_t precision() const
		/// Returns column precision.
		/// Valid for floating point fields only (zero for other data types).
	{
		return _metaColumn.precision();
	}

	std::size_t position() const
		/// Returns column position.
	{
		return _metaColumn.position();
	}

	MetaColumn::ColumnDataType type() const
		/// Returns column type.
	{
		return _metaColumn.type();
	}

	Iterator begin() const
		/// Returns iterator pointing to the beginning of data storage vector.
	{
		return _pData->begin();
	}

	Iterator end() const
		/// Returns iterator pointing to the end of data storage vector.
	{
		return _pData->end();
	}

private:
	Column();

	MetaColumn   _metaColumn;
	ContainerPtr _pData;
};


template <typename C>
inline void swap(Column<C>& c1, Column<C>& c2) noexcept
{
	c1.swap(c2);
}


} } // namespace Poco::Data


#endif // Data_Column_INCLUDED

