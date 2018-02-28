//
// Array.h
//
// Library: Foundation
// Package: Core
// Module:  Array
//
// Definition of the Array class
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
// ------------------------------------------------------------------------------
// (C) Copyright Nicolai M. Josuttis 2001.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
// ------------------------------------------------------------------------------


#ifndef Foundation_Array_INCLUDED
#define Foundation_Array_INCLUDED


#include "Poco/Exception.h"
#include "Poco/Bugcheck.h"
#include <algorithm>
#include <array>


namespace Poco {

template<class T, std::size_t N>
class Array : public std::array<T, N>
	/// STL container like C-style array replacement class.
	///
	/// This implementation is based on the idea of Nicolai Josuttis.
	/// His original implementation can be found at http://www.josuttis.com/cppcode/array.html .
{

public:

	Array() : std::array<T, N>()
	{}

	template <typename... X>
	Array(T t, X... xs) :
		std::array<T, N>::array{ {t, xs...} }
	{}

	Array(std::initializer_list<T> l) :
		std::array<T, N>::array()
	{
		std::copy(l.begin(), l.end(), this->begin());
	}

	enum { static_size = N };

	T* c_array()
	{
		/// Use array as C array (direct read/write access to data)
		return this->data();
	}

	template <typename Other>
	Array<T,N>& operator= (const Array<Other,N>& rhs)
		/// Assignment with type conversion
	{
		std::copy(rhs.begin(),rhs.end(), this->begin());
		return *this;
	}

	void assign (const T& value)
		/// Assign one value to all elements
	{
		std::fill_n(this->begin(),this->size(),value);
	}

};


} // namespace Poco

#endif // Foundation_Array_INCLUDED

