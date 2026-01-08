//
// TypeList.h
//
// Library: Foundation
// Package: Core
// Module:  TypeList
//
// Definition of the TypeList template.
//
// Copyright (c) 2006-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_TypeList_INCLUDED
#define Foundation_TypeList_INCLUDED


// This is an internal header file, included by other POCO headers.
// Do not attempt to use it directly.
#ifndef POCO_TYPELIST_INTERNAL
#error "TypeList.h is an internal header. Include Poco/Tuple.h instead."
#endif


#include "Poco/Foundation.h"
#include <tuple>
#include <type_traits>
#include <cstddef>


namespace Poco {


/// NullTypeList - marker type for unused template parameters.
struct NullTypeList
{
	static constexpr std::size_t length = 0;

	[[nodiscard]]
	bool operator == (const NullTypeList&) const { return true; }
	[[nodiscard]]
	bool operator != (const NullTypeList&) const { return false; }
	[[nodiscard]]
	bool operator < (const NullTypeList&) const { return false; }
};


/// TypeList - a compile-time list of types.
template <typename... Ts>
struct TypeList
{
	static constexpr std::size_t length = sizeof...(Ts);
};


namespace Detail {


/// Filter out NullTypeList from a parameter pack
template <typename... Ts>
struct FilterNullTypeList;

template <>
struct FilterNullTypeList<>
{
	using Type = TypeList<>;
};

template <typename T, typename... Rest>
struct FilterNullTypeList<T, Rest...>
{
private:
	using RestFiltered = typename FilterNullTypeList<Rest...>::Type;

	template <typename List>
	struct Prepend;

	template <typename... Us>
	struct Prepend<TypeList<Us...>>
	{
		using Type = TypeList<T, Us...>;
	};

public:
	using Type = std::conditional_t<
		std::is_same_v<T, NullTypeList>,
		RestFiltered,
		typename Prepend<RestFiltered>::Type
	>;
};


/// Get the type at index N from a TypeList
template <std::size_t N, typename List>
struct TypeAtImpl;

template <std::size_t N, typename... Ts>
struct TypeAtImpl<N, TypeList<Ts...>>
{
	static_assert(N < sizeof...(Ts), "TypeList index out of bounds");
	using Type = std::tuple_element_t<N, std::tuple<Ts...>>;
};


} // namespace Detail


/// TypeListType - creates a TypeList from up to 40 type parameters.
/// NullTypeList parameters are filtered out.
template <
	typename T0  = NullTypeList,
	typename T1  = NullTypeList,
	typename T2  = NullTypeList,
	typename T3  = NullTypeList,
	typename T4  = NullTypeList,
	typename T5  = NullTypeList,
	typename T6  = NullTypeList,
	typename T7  = NullTypeList,
	typename T8  = NullTypeList,
	typename T9  = NullTypeList,
	typename T10 = NullTypeList,
	typename T11 = NullTypeList,
	typename T12 = NullTypeList,
	typename T13 = NullTypeList,
	typename T14 = NullTypeList,
	typename T15 = NullTypeList,
	typename T16 = NullTypeList,
	typename T17 = NullTypeList,
	typename T18 = NullTypeList,
	typename T19 = NullTypeList,
	typename T20 = NullTypeList,
	typename T21 = NullTypeList,
	typename T22 = NullTypeList,
	typename T23 = NullTypeList,
	typename T24 = NullTypeList,
	typename T25 = NullTypeList,
	typename T26 = NullTypeList,
	typename T27 = NullTypeList,
	typename T28 = NullTypeList,
	typename T29 = NullTypeList,
	typename T30 = NullTypeList,
	typename T31 = NullTypeList,
	typename T32 = NullTypeList,
	typename T33 = NullTypeList,
	typename T34 = NullTypeList,
	typename T35 = NullTypeList,
	typename T36 = NullTypeList,
	typename T37 = NullTypeList,
	typename T38 = NullTypeList,
	typename T39 = NullTypeList>
struct TypeListType
{
	using HeadType = typename Detail::FilterNullTypeList<
		T0, T1, T2, T3, T4, T5, T6, T7, T8, T9,
		T10, T11, T12, T13, T14, T15, T16, T17, T18, T19,
		T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
		T30, T31, T32, T33, T34, T35, T36, T37, T38, T39
	>::Type;
};


/// TypeGetter - get the type at index N from a TypeList.
template <std::size_t N, typename List>
struct TypeGetter
{
	using HeadType = typename Detail::TypeAtImpl<N, List>::Type;
	using ConstHeadType = const HeadType;
};


} // namespace Poco


#endif // Foundation_TypeList_INCLUDED
