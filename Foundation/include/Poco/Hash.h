//
// Hash.h
//
// Library: Foundation
// Package: Hashing
// Module:  Hash
//
// Definition of the Hash class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// hashRange Copyright 2005-2014 Daniel James.
// (Extracted from Boost 1.75.0 lib and adapted for poco on 2021-03-31)
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Hash_INCLUDED
#define Foundation_Hash_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Types.h"
#include <cstddef>


#if defined(_MSC_VER)
#   define POCO_HASH_ROTL32(x, r) _rotl(x,r)
#else
#   define POCO_HASH_ROTL32(x, r) (x << r) | (x >> (32 - r))
#endif


namespace Poco {


std::size_t Foundation_API hash(Int8 n);
std::size_t Foundation_API hash(UInt8 n);
std::size_t Foundation_API hash(Int16 n);
std::size_t Foundation_API hash(UInt16 n);
std::size_t Foundation_API hash(Int32 n);
std::size_t Foundation_API hash(UInt32 n);
std::size_t Foundation_API hash(Int64 n);
std::size_t Foundation_API hash(UInt64 n);
std::size_t Foundation_API hash(const std::string& str);


template <class T>
struct Hash
	/// A generic hash function.
{
	std::size_t operator () (T value) const
		/// Returns the hash for the given value.
	{
		return Poco::hash(value);
	}
};


//
// inlines
//
inline std::size_t hash(Int8 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(UInt8 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(Int16 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(UInt16 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(Int32 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(UInt32 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(Int64 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


inline std::size_t hash(UInt64 n)
{
	return static_cast<std::size_t>(n)*2654435761U;
}


namespace Impl {


template <typename SizeT>
inline void hashCombine(SizeT& seed, SizeT value)
{
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}


inline void hashCombine(Poco::UInt32& h1, Poco::UInt32 k1)
{
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    k1 *= c1;
    k1 = POCO_HASH_ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = POCO_HASH_ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
}


#if defined(POCO_PTR_IS_64_BIT) && !(defined(__GNUC__) && ULONG_MAX == 0xffffffff)


inline void hashCombine(Poco::UInt64& h, Poco::UInt64 k)
{
    const Poco::UInt64 m = UINT64_C(0xc6a4a7935bd1e995);
    const int r = 47;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;

    // Completely arbitrary number, to
    // prevent zeros from hashing to 0.
    h += 0xe6546b64;
}


#endif // POCO_PTR_IS_64_BIT


} // namespace Impl


template <class T>
inline void hashCombine(std::size_t& seed, T const& v)
{
    Hash<T> hasher;
    Impl::hashCombine(seed, hasher(v));
}


template <class It>
inline std::size_t hashRange(It first, It last)
{
    std::size_t seed = 0;

    for(; first != last; ++first)
    {
        hashCombine<typename std::iterator_traits<It>::value_type>(seed, *first);
    }

    return seed;
}


template <class It>
inline void hashRange(std::size_t& seed, It first, It last)
{
    for(; first != last; ++first)
    {
        hashCombine<typename std::iterator_traits<It>::value_type>(seed, *first);
    }
}


} // namespace Poco


#endif // Foundation_Hash_INCLUDED
