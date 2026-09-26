//
// XSDVector.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  XSDVector
//
// Definition of the XSDVector class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_XSDVector_INCLUDED
#define XSDTypes_XSDVector_INCLUDED


#include <vector>
#include "Poco/Types.h"


namespace Poco {
namespace XSD {
namespace Types {


template <typename T>
class XSDVector
	/// Class XSDVector is the root class for all simple list types.
{
public:
	using Iterator = typename std::vector<T>::iterator;
	using ConstIterator = typename std::vector<T>::const_iterator;
	using ReverseIterator = typename std::vector<T>::reverse_iterator;
	using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;
	using SizeType = typename std::vector<T>::size_type;
	using Reference = typename std::vector<T>::reference;
	using ConstReference = typename std::vector<T>::const_reference;

	XSDVector():_data()
		/// Creates the XSDVector.
	{
	}

	XSDVector(SizeType n):_data(n)
		/// Creates the XSDVector with n elements
	{
	}

	XSDVector(SizeType n, const T& val):_data(n, val)
		/// Creates the XSDVector with n copies of val
	{
	}

	template <typename InputIterator>
	XSDVector(InputIterator beg, InputIterator end): _data(beg, end)
		/// Creates the XSDVector with a copy of a range.
	{
	}

	virtual ~XSDVector()
		/// Destroys the XSDVector.
	{
	}

	Iterator begin()
		/// Returns the begin iterator
	{
		return _data.begin();
	}

	Iterator end()
		/// Returns the end iterator
	{
		return _data.end();
	}

	ConstIterator begin() const
		/// Returns the begin iterator
	{
		return _data.begin();
	}

	const Iterator end() const
		/// Returns the end iterator
	{
		return _data.end();
	}

	ReverseIterator rbegin()
		/// Returns the begin iterator
	{
		return _data.rbegin();
	}

	ReverseIterator rend()
		/// Returns the end iterator
	{
		return _data.rend();
	}

	ConstReverseIterator rbegin() const
		/// Returns the begin iterator
	{
		return _data.rbegin();
	}

	ConstReverseIterator rend() const
		/// Returns the end iterator
	{
		return _data.rend();
	}

	SizeType size() const
	{
		return _data.size();
	}

	SizeType maxSize() const
	{
		return _data.max_size();
	}

	SizeType capacity() const
	{
		return _data.capacity();
	}

	bool empty() const
	{
		return _data.empty();
	}
	
	Reference operator[](const SizeType n)
	{
		return _data.operator[](n);
	}

	ConstReference operator[](const SizeType n) const
	{
		return _data.operator[](n);
	}

	void reserve(SizeType n)
	{
		_data.reserve(n);
	}

	Reference front()
	{
		return _data.front();
	}

	ConstReference front() const
	{
		return _data.front();
	}

	Reference back()
	{
		return _data.back();
	}

	ConstReference back() const
	{
		return _data.back();
	}

	void pushBack(const T& t)
	{
		_data.push_back(t);
	}

	void popBack()
	{
		_data.pop_back();
	}

	Iterator insert(Iterator pos, const T& val)
	{
		return _data.insert(pos, val);
	}

	template <typename InputIterator>
	void insert(Iterator pos, InputIterator first, InputIterator last)
	{
		_data.insert(pos, first, last);
	}

	void insert(Iterator pos, SizeType n, const T& val)
	{
		_data.insert(pos, n, val);
	}

	Iterator erase(Iterator pos)
	{
		_data.erase(pos);
	}

	Iterator erase(Iterator first, Iterator last)
	{
		_data.erase(first, last);
	}

	void clear()
	{
		_data.clear();
	}

	void resize(SizeType n, const T& def = T())
	{
		_data.resize(n, def);
	}

	bool operator== (const XSDVector<T>& other) const
	{
		return _data == other._data;
	}

	bool operator< (const XSDVector<T>& other) const
	{
		return _data < other._data;
	}

private:
	std::vector<T> _data;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Documentation_INCLUDED
