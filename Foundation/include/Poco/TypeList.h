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

	bool operator == (const NullTypeList&) const { return true; }
	bool operator != (const NullTypeList&) const { return false; }
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


/// Find type T in a TypeList, returns index or -1 if not found
template <typename T, typename List>
struct TypeLocatorImpl;

template <typename T>
struct TypeLocatorImpl<T, TypeList<>>
{
	static constexpr int value = -1;
};

template <typename T, typename Head, typename... Tail>
struct TypeLocatorImpl<T, TypeList<Head, Tail...>>
{
private:
	static constexpr int tailResult = TypeLocatorImpl<T, TypeList<Tail...>>::value;
public:
	static constexpr int value = std::is_same_v<T, Head> ? 0 :
		(tailResult == -1 ? -1 : 1 + tailResult);
};


/// Concatenate two TypeLists
template <typename List1, typename List2>
struct TypeListConcat;

template <typename... Ts, typename... Us>
struct TypeListConcat<TypeList<Ts...>, TypeList<Us...>>
{
	using Type = TypeList<Ts..., Us...>;
};


/// Append type(s) to a TypeList
template <typename List, typename T>
struct TypeAppenderImpl;

template <typename... Ts, typename T>
struct TypeAppenderImpl<TypeList<Ts...>, T>
{
	using Type = std::conditional_t<
		std::is_same_v<T, NullTypeList>,
		TypeList<Ts...>,
		TypeList<Ts..., T>
	>;
};

template <typename... Ts, typename... Us>
struct TypeAppenderImpl<TypeList<Ts...>, TypeList<Us...>>
{
	using Type = TypeList<Ts..., Us...>;
};

template <typename T>
struct TypeAppenderImpl<NullTypeList, T>
{
	using Type = std::conditional_t<
		std::is_same_v<T, NullTypeList>,
		TypeList<>,
		TypeList<T>
	>;
};

template <typename... Us>
struct TypeAppenderImpl<NullTypeList, TypeList<Us...>>
{
	using Type = TypeList<Us...>;
};


/// Erase first occurrence of type T from a TypeList
template <typename List, typename T>
struct TypeOneEraserImpl;

template <typename T>
struct TypeOneEraserImpl<TypeList<>, T>
{
	using Type = TypeList<>;
};

template <typename T, typename... Tail>
struct TypeOneEraserImpl<TypeList<T, Tail...>, T>
{
	using Type = TypeList<Tail...>;
};

template <typename Head, typename... Tail, typename T>
struct TypeOneEraserImpl<TypeList<Head, Tail...>, T>
{
	using Type = typename TypeListConcat<
		TypeList<Head>,
		typename TypeOneEraserImpl<TypeList<Tail...>, T>::Type
	>::Type;
};


/// Erase all occurrences of type T from a TypeList
template <typename List, typename T>
struct TypeAllEraserImpl;

template <typename T>
struct TypeAllEraserImpl<TypeList<>, T>
{
	using Type = TypeList<>;
};

template <typename T, typename... Tail>
struct TypeAllEraserImpl<TypeList<T, Tail...>, T>
{
	using Type = typename TypeAllEraserImpl<TypeList<Tail...>, T>::Type;
};

template <typename Head, typename... Tail, typename T>
struct TypeAllEraserImpl<TypeList<Head, Tail...>, T>
{
	using Type = typename TypeListConcat<
		TypeList<Head>,
		typename TypeAllEraserImpl<TypeList<Tail...>, T>::Type
	>::Type;
};


/// Erase duplicate types from a TypeList (keep first occurrence)
template <typename List>
struct TypeDuplicateEraserImpl;

template <>
struct TypeDuplicateEraserImpl<TypeList<>>
{
	using Type = TypeList<>;
};

template <typename Head, typename... Tail>
struct TypeDuplicateEraserImpl<TypeList<Head, Tail...>>
{
private:
	using TailWithoutDuplicates = typename TypeDuplicateEraserImpl<TypeList<Tail...>>::Type;
	using TailWithoutHead = typename TypeOneEraserImpl<TailWithoutDuplicates, Head>::Type;
public:
	using Type = typename TypeListConcat<TypeList<Head>, TailWithoutHead>::Type;
};


/// Replace first occurrence of type T with type R
template <typename List, typename T, typename R>
struct TypeOneReplacerImpl;

template <typename T, typename R>
struct TypeOneReplacerImpl<TypeList<>, T, R>
{
	using Type = TypeList<>;
};

template <typename T, typename R, typename... Tail>
struct TypeOneReplacerImpl<TypeList<T, Tail...>, T, R>
{
	using Type = TypeList<R, Tail...>;
};

template <typename Head, typename... Tail, typename T, typename R>
struct TypeOneReplacerImpl<TypeList<Head, Tail...>, T, R>
{
	using Type = typename TypeListConcat<
		TypeList<Head>,
		typename TypeOneReplacerImpl<TypeList<Tail...>, T, R>::Type
	>::Type;
};


/// Replace all occurrences of type T with type R
template <typename List, typename T, typename R>
struct TypeAllReplacerImpl;

template <typename T, typename R>
struct TypeAllReplacerImpl<TypeList<>, T, R>
{
	using Type = TypeList<>;
};

template <typename Head, typename... Tail, typename T, typename R>
struct TypeAllReplacerImpl<TypeList<Head, Tail...>, T, R>
{
	using Type = typename TypeListConcat<
		TypeList<std::conditional_t<std::is_same_v<Head, T>, R, Head>>,
		typename TypeAllReplacerImpl<TypeList<Tail...>, T, R>::Type
	>::Type;
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


/// TypeLocator - find the index of type T in a TypeList. Returns -1 if not found.
template <typename List, typename T>
struct TypeLocator
{
	static constexpr int value = Detail::TypeLocatorImpl<T, List>::value;
};


/// TypeAppender - append a type or TypeList to another TypeList.
template <typename List, typename T>
struct TypeAppender
{
	using HeadType = typename Detail::TypeAppenderImpl<List, T>::Type;
};


/// TypeOneEraser - erase the first occurrence of type T from a TypeList.
template <typename List, typename T>
struct TypeOneEraser
{
	using HeadType = typename Detail::TypeOneEraserImpl<List, T>::Type;
};


/// TypeAllEraser - erase all occurrences of type T from a TypeList.
template <typename List, typename T>
struct TypeAllEraser
{
	using HeadType = typename Detail::TypeAllEraserImpl<List, T>::Type;
};


/// TypeDuplicateEraser - erase duplicate types (keep first occurrence).
template <typename List>
struct TypeDuplicateEraser
{
	using HeadType = typename Detail::TypeDuplicateEraserImpl<List>::Type;
};


/// TypeOneReplacer - replace first occurrence of type T with type R.
template <typename List, typename T, typename R>
struct TypeOneReplacer
{
	using HeadType = typename Detail::TypeOneReplacerImpl<List, T, R>::Type;
};


/// TypeAllReplacer - replace all occurrences of type T with type R.
template <typename List, typename T, typename R>
struct TypeAllReplacer
{
	using HeadType = typename Detail::TypeAllReplacerImpl<List, T, R>::Type;
};


} // namespace Poco


#endif // Foundation_TypeList_INCLUDED
