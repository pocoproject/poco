//
// MakeUnique.h
//
// Library: Foundation
// Package: Core
// Module:  MakeUnique
//
// Definition of the MakeUnique template class. This is essentially std::make_unique
// for pre-C++14 compilers.
//
// Code adapted for naming convention from https://isocpp.org/files/papers/N3656.txt
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MakeUnique_INCLUDED
#define Foundation_MakeUnique_INCLUDED


#include "Poco/Foundation.h"
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>


namespace Poco {


	template<class T> struct UniqueIf
	{
		typedef std::unique_ptr<T> SingleObject;
	};

	template<class T> struct UniqueIf<T[]>
	{
		typedef std::unique_ptr<T[]> UnknownBound;
	};

	template<class T, size_t N> struct UniqueIf<T[N]>
	{
		typedef void KnownBound;
	};

	template<class T, class... Args>
	typename UniqueIf<T>::SingleObject makeUnique(Args&&... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template<class T>
	typename UniqueIf<T>::UnknownBound makeUnique(size_t n)
	{
		typedef typename std::remove_extent<T>::type U;
		return std::unique_ptr<T>(new U[n]());
	}

	template<class T, class... Args>
	typename UniqueIf<T>::KnownBound makeUnique(Args&&...) = delete;


} // namespace Poco


#endif // Foundation_MakeUnique_INCLUDED
