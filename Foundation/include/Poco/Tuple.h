//
// Tuple.h
//
// Library: Foundation
// Package: Core
// Module:  Tuple
//
// Definition of the Tuple class.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Tuple_INCLUDED
#define Foundation_Tuple_INCLUDED


#include "Poco/Foundation.h"
#define POCO_TYPELIST_INTERNAL
#include "Poco/TypeList.h"
#undef POCO_TYPELIST_INTERNAL
#include <tuple>
#include <type_traits>


namespace Poco {


#if defined(POCO_COMPILER_MSVC)
#define POCO_TYPEWRAPPER_DEFAULTVALUE(T) Detail::DefaultValue<T>::get()
#else
#define POCO_TYPEWRAPPER_DEFAULTVALUE(T) Detail::DefaultValue<T>::get()
#endif


namespace Detail {


/// Helper to provide default values, handling NullTypeList specially
template <typename T>
struct DefaultValue
{
	static T get() { return T(); }
};

template <>
struct DefaultValue<NullTypeList>
{
	static NullTypeList get() { return NullTypeList(); }
};


/// Convert TypeList to std::tuple
template <typename List>
struct TypeListToTuple;

template <typename... Ts>
struct TypeListToTuple<TypeList<Ts...>>
{
	using Type = std::tuple<Ts...>;
};

template <>
struct TypeListToTuple<NullTypeList>
{
	using Type = std::tuple<>;
};


/// Helper to compare tuples element by element for less-than
template <std::size_t I, std::size_t N>
struct TupleLess
{
	template <typename T>
	static bool compare(const T& a, const T& b)
	{
		if (std::get<I>(a) < std::get<I>(b)) return true;
		if (std::get<I>(b) < std::get<I>(a)) return false;
		return TupleLess<I + 1, N>::compare(a, b);
	}
};

template <std::size_t N>
struct TupleLess<N, N>
{
	template <typename T>
	static bool compare(const T&, const T&)
	{
		return false;
	}
};


} // namespace Detail


/// Tuple - a fixed-size collection of heterogeneous values.
///
/// This implementation uses std::tuple internally while maintaining
/// backward compatibility with the original Poco::Tuple API.
///
/// Usage:
///   Tuple<int, std::string, double> t(42, "hello", 3.14);
///   int i = t.get<0>();
///   t.set<1>("world");
///
template <
	typename T0,
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
struct Tuple
{
	using Type = typename TypeListType<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9,
		T10, T11, T12, T13, T14, T15, T16, T17, T18, T19,
		T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
		T30, T31, T32, T33, T34, T35, T36, T37, T38, T39>::HeadType;

	using TupleType = typename Detail::TypeListToTuple<Type>::Type;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple() : _data()
	{
	}

	Tuple(
		const T0& t0,
		const T1& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const T2& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		const T3& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const T4& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		const T5& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const T6& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		const T7& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const T8& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		const T9& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const T10& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const T11& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const T12& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const T13& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const T14& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const T15& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const T16& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const T17& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const T18& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const T19& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const T20& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		const T21& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const T22& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		const T23& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const T24& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		const T25& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const T26& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		const T27& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const T28& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		const T29& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const T30& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const T31& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const T32& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const T33& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const T34& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const T35& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		const T36& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		const T37& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37),
		const T38& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38),
		const T39& t39 = POCO_TYPEWRAPPER_DEFAULTVALUE(T39))
	{
		init<0>(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
			t10, t11, t12, t13, t14, t15, t16, t17, t18, t19,
			t20, t21, t22, t23, t24, t25, t26, t27, t28, t29,
			t30, t31, t32, t33, t34, t35, t36, t37, t38, t39);
	}

	Tuple(const Tuple& other) : _data(other._data)
	{
	}

	Tuple& operator=(const Tuple& other)
	{
		if (this != &other)
		{
			_data = other._data;
		}
		return *this;
	}

	template <std::size_t N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return std::get<N>(_data);
	}

	template <std::size_t N>
	const typename TypeGetter<N, Type>::HeadType& get() const
	{
		return std::get<N>(_data);
	}

	template <std::size_t N>
	void set(const typename TypeGetter<N, Type>::HeadType& val)
	{
		std::get<N>(_data) = val;
	}

	bool operator==(const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator!=(const Tuple& other) const
	{
		return _data != other._data;
	}

	bool operator<(const Tuple& other) const
	{
		return Detail::TupleLess<0, length>::compare(_data, other._data);
	}

	explicit operator TupleType&() { return _data; }

	explicit operator const TupleType&() const { return _data; }

private:
	template <std::size_t I, typename U, typename... Us>
	void init(const U& u, const Us&... us)
	{
		if constexpr (I < length)
		{
			if constexpr (!std::is_same_v<U, NullTypeList>)
			{
				std::get<I>(_data) = u;
			}
			if constexpr (sizeof...(Us) > 0 && I + 1 < length)
			{
				init<I + 1>(us...);
			}
		}
	}

	template <std::size_t I>
	void init()
	{
		// Base case - nothing to do
	}

	TupleType _data;

};


} // namespace Poco


#endif // Foundation_Tuple_INCLUDED
