//
// Tuple.h
//
// Library: Foundation
// Package: Core
// Module:  Tuple
//
// Definition of the Tuple class.
//
// Copyright (c) 2004-2006,Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Tuple_INCLUDED
#define Foundation_Tuple_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/TypeList.h"


namespace Poco {


#if defined(POCO_COMPILER_MSVC)
#define POCO_TYPEWRAPPER_DEFAULTVALUE(T) TypeWrapper<T>::TYPE()
#else
#define POCO_TYPEWRAPPER_DEFAULTVALUE(T) typename TypeWrapper<T>::TYPE()
#endif


template <class T0,
	class T1 = NullTypeList,
	class T2 = NullTypeList,
	class T3 = NullTypeList,
	class T4 = NullTypeList,
	class T5 = NullTypeList,
	class T6 = NullTypeList,
	class T7 = NullTypeList,
	class T8 = NullTypeList,
	class T9 = NullTypeList,
	class T10 = NullTypeList,
	class T11 = NullTypeList,
	class T12 = NullTypeList,
	class T13 = NullTypeList,
	class T14 = NullTypeList,
	class T15 = NullTypeList,
	class T16 = NullTypeList,
	class T17 = NullTypeList,
	class T18 = NullTypeList,
	class T19 = NullTypeList,
	class T20 = NullTypeList,
	class T21 = NullTypeList,
	class T22 = NullTypeList,
	class T23 = NullTypeList,
	class T24 = NullTypeList,
	class T25 = NullTypeList,
	class T26 = NullTypeList,
	class T27 = NullTypeList,
	class T28 = NullTypeList,
	class T29 = NullTypeList,
	class T30 = NullTypeList,
	class T31 = NullTypeList,
	class T32 = NullTypeList,
	class T33 = NullTypeList,
	class T34 = NullTypeList,
	class T35 = NullTypeList,
	class T36 = NullTypeList,
	class T37 = NullTypeList,
	class T38 = NullTypeList,
	class T39 = NullTypeList>
struct Tuple
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37),
		typename TypeWrapper<T38>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38),
		typename TypeWrapper<T39>::CONSTTYPE& t39 = POCO_TYPEWRAPPER_DEFAULTVALUE(T39)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t31, typename TypeListType<T32,T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t32, typename TypeListType<T33,T34,T35,T36,T37,T38,T39>::HeadType
			(t33, typename TypeListType<T34,T35,T36,T37,T38,T39>::HeadType
			(t34, typename TypeListType<T35,T36,T37,T38,T39>::HeadType
			(t35, typename TypeListType<T36,T37,T38,T39>::HeadType
			(t36, typename TypeListType<T37,T38,T39>::HeadType
			(t37, typename TypeListType<T38,T39>::HeadType
			(t38, typename TypeListType<T39>::HeadType
			(t39, NullTypeList()))))))))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33,
	class T34,
	class T35,
	class T36,
	class T37,
	class T38>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37),
		typename TypeWrapper<T38>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t31, typename TypeListType<T32,T33,T34,T35,T36,T37,T38>::HeadType
			(t32, typename TypeListType<T33,T34,T35,T36,T37,T38>::HeadType
			(t33, typename TypeListType<T34,T35,T36,T37,T38>::HeadType
			(t34, typename TypeListType<T35,T36,T37,T38>::HeadType
			(t35, typename TypeListType<T36,T37,T38>::HeadType
			(t36, typename TypeListType<T37,T38>::HeadType
			(t37, typename TypeListType<T38>::HeadType
			(t38, NullTypeList())))))))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33,
	class T34,
	class T35,
	class T36,
	class T37>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34,T35,T36,T37>::HeadType
			(t31, typename TypeListType<T32,T33,T34,T35,T36,T37>::HeadType
			(t32, typename TypeListType<T33,T34,T35,T36,T37>::HeadType
			(t33, typename TypeListType<T34,T35,T36,T37>::HeadType
			(t34, typename TypeListType<T35,T36,T37>::HeadType
			(t35, typename TypeListType<T36,T37>::HeadType
			(t36, typename TypeListType<T37>::HeadType
			(t37, NullTypeList()))))))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33,
	class T34,
	class T35,
	class T36>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34,T35,T36>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34,T35,T36>::HeadType
			(t31, typename TypeListType<T32,T33,T34,T35,T36>::HeadType
			(t32, typename TypeListType<T33,T34,T35,T36>::HeadType
			(t33, typename TypeListType<T34,T35,T36>::HeadType
			(t34, typename TypeListType<T35,T36>::HeadType
			(t35, typename TypeListType<T36>::HeadType
			(t36, NullTypeList())))))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33,
	class T34,
	class T35>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34,T35>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34,T35>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34,T35>::HeadType
			(t31, typename TypeListType<T32,T33,T34,T35>::HeadType
			(t32, typename TypeListType<T33,T34,T35>::HeadType
			(t33, typename TypeListType<T34,T35>::HeadType
			(t34, typename TypeListType<T35>::HeadType
			(t35, NullTypeList()))))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33,
	class T34>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33,T34>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33,T34>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33,T34>::HeadType
			(t30, typename TypeListType<T31,T32,T33,T34>::HeadType
			(t31, typename TypeListType<T32,T33,T34>::HeadType
			(t32, typename TypeListType<T33,T34>::HeadType
			(t33, typename TypeListType<T34>::HeadType
			(t34, NullTypeList())))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32,
	class T33>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32,T33>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32,T33>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32,T33>::HeadType
			(t29, typename TypeListType<T30,T31,T32,T33>::HeadType
			(t30, typename TypeListType<T31,T32,T33>::HeadType
			(t31, typename TypeListType<T32,T33>::HeadType
			(t32, typename TypeListType<T33>::HeadType
			(t33, NullTypeList()))))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31,
	class T32>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31,T32>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31,T32>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31,T32>::HeadType
			(t28, typename TypeListType<T29,T30,T31,T32>::HeadType
			(t29, typename TypeListType<T30,T31,T32>::HeadType
			(t30, typename TypeListType<T31,T32>::HeadType
			(t31, typename TypeListType<T32>::HeadType
			(t32, NullTypeList())))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30,
	class T31>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30,T31>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30,T31>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30,T31>::HeadType
			(t27, typename TypeListType<T28,T29,T30,T31>::HeadType
			(t28, typename TypeListType<T29,T30,T31>::HeadType
			(t29, typename TypeListType<T30,T31>::HeadType
			(t30, typename TypeListType<T31>::HeadType
			(t31, NullTypeList()))))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29,
	class T30>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29,T30>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29,T30>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29,T30>::HeadType
			(t26, typename TypeListType<T27,T28,T29,T30>::HeadType
			(t27, typename TypeListType<T28,T29,T30>::HeadType
			(t28, typename TypeListType<T29,T30>::HeadType
			(t29, typename TypeListType<T30>::HeadType
			(t30, NullTypeList())))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28,
	class T29>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28),
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28,T29>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28,T29>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28,T29>::HeadType
			(t25, typename TypeListType<T26,T27,T28,T29>::HeadType
			(t26, typename TypeListType<T27,T28,T29>::HeadType
			(t27, typename TypeListType<T28,T29>::HeadType
			(t28, typename TypeListType<T29>::HeadType
			(t29, NullTypeList()))))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27,
	class T28>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27,T28>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27,T28>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27,T28>::HeadType
			(t24, typename TypeListType<T25,T26,T27,T28>::HeadType
			(t25, typename TypeListType<T26,T27,T28>::HeadType
			(t26, typename TypeListType<T27,T28>::HeadType
			(t27, typename TypeListType<T28>::HeadType
			(t28, NullTypeList())))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26,
	class T27>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26,T27>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26,T27>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26,T27>::HeadType
			(t23, typename TypeListType<T24,T25,T26,T27>::HeadType
			(t24, typename TypeListType<T25,T26,T27>::HeadType
			(t25, typename TypeListType<T26,T27>::HeadType
			(t26, typename TypeListType<T27>::HeadType
			(t27, NullTypeList()))))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25,
	class T26>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25,T26>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25,T26>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25,T26>::HeadType
			(t22, typename TypeListType<T23,T24,T25,T26>::HeadType
			(t23, typename TypeListType<T24,T25,T26>::HeadType
			(t24, typename TypeListType<T25,T26>::HeadType
			(t25, typename TypeListType<T26>::HeadType
			(t26, NullTypeList())))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24,
	class T25>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24,T25>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24,T25>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24,T25>::HeadType
			(t21, typename TypeListType<T22,T23,T24,T25>::HeadType
			(t22, typename TypeListType<T23,T24,T25>::HeadType
			(t23, typename TypeListType<T24,T25>::HeadType
			(t24, typename TypeListType<T25>::HeadType
			(t25, NullTypeList()))))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23,
	class T24>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23,T24>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23,T24>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23,T24>::HeadType
			(t20, typename TypeListType<T21,T22,T23,T24>::HeadType
			(t21, typename TypeListType<T22,T23,T24>::HeadType
			(t22, typename TypeListType<T23,T24>::HeadType
			(t23, typename TypeListType<T24>::HeadType
			(t24, NullTypeList())))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22,
	class T23>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22),
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22,T23>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22,T23>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22,T23>::HeadType
			(t19, typename TypeListType<T20,T21,T22,T23>::HeadType
			(t20, typename TypeListType<T21,T22,T23>::HeadType
			(t21, typename TypeListType<T22,T23>::HeadType
			(t22, typename TypeListType<T23>::HeadType
			(t23, NullTypeList()))))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21,
	class T22>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21,T22>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21,T22>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21,T22>::HeadType
			(t18, typename TypeListType<T19,T20,T21,T22>::HeadType
			(t19, typename TypeListType<T20,T21,T22>::HeadType
			(t20, typename TypeListType<T21,T22>::HeadType
			(t21, typename TypeListType<T22>::HeadType
			(t22, NullTypeList())))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20,
	class T21>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20,T21>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20,T21>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20,T21>::HeadType
			(t17, typename TypeListType<T18,T19,T20,T21>::HeadType
			(t18, typename TypeListType<T19,T20,T21>::HeadType
			(t19, typename TypeListType<T20,T21>::HeadType
			(t20, typename TypeListType<T21>::HeadType
			(t21, NullTypeList()))))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19,
	class T20>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19,T20>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19,T20>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19,T20>::HeadType
			(t16, typename TypeListType<T17,T18,T19,T20>::HeadType
			(t17, typename TypeListType<T18,T19,T20>::HeadType
			(t18, typename TypeListType<T19,T20>::HeadType
			(t19, typename TypeListType<T20>::HeadType
			(t20, NullTypeList())))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18,
	class T19>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18,T19>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18,T19>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18,T19>::HeadType
			(t15, typename TypeListType<T16,T17,T18,T19>::HeadType
			(t16, typename TypeListType<T17,T18,T19>::HeadType
			(t17, typename TypeListType<T18,T19>::HeadType
			(t18, typename TypeListType<T19>::HeadType
			(t19, NullTypeList()))))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17,
	class T18>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17,T18>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17,T18>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17,T18>::HeadType
			(t14, typename TypeListType<T15,T16,T17,T18>::HeadType
			(t15, typename TypeListType<T16,T17,T18>::HeadType
			(t16, typename TypeListType<T17,T18>::HeadType
			(t17, typename TypeListType<T18>::HeadType
			(t18, NullTypeList())))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16,
	class T17>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16,T17>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16,T17>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16,T17>::HeadType
			(t13, typename TypeListType<T14,T15,T16,T17>::HeadType
			(t14, typename TypeListType<T15,T16,T17>::HeadType
			(t15, typename TypeListType<T16,T17>::HeadType
			(t16, typename TypeListType<T17>::HeadType
			(t17, NullTypeList()))))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15,
	class T16>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15,T16>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15,T16>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15,T16>::HeadType
			(t12, typename TypeListType<T13,T14,T15,T16>::HeadType
			(t13, typename TypeListType<T14,T15,T16>::HeadType
			(t14, typename TypeListType<T15,T16>::HeadType
			(t15, typename TypeListType<T16>::HeadType
			(t16, NullTypeList())))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14,
	class T15>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14,T15>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14,T15>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14,T15>::HeadType
			(t11, typename TypeListType<T12,T13,T14,T15>::HeadType
			(t12, typename TypeListType<T13,T14,T15>::HeadType
			(t13, typename TypeListType<T14,T15>::HeadType
			(t14, typename TypeListType<T15>::HeadType
			(t15, NullTypeList()))))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13,
	class T14>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13,T14>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13,T14>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13,T14>::HeadType
			(t10, typename TypeListType<T11,T12,T13,T14>::HeadType
			(t11, typename TypeListType<T12,T13,T14>::HeadType
			(t12, typename TypeListType<T13,T14>::HeadType
			(t13, typename TypeListType<T14>::HeadType
			(t14, NullTypeList())))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12,
	class T13>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12,T13>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12,T13>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12,T13>::HeadType
			(t9, typename TypeListType<T10,T11,T12,T13>::HeadType
			(t10, typename TypeListType<T11,T12,T13>::HeadType
			(t11, typename TypeListType<T12,T13>::HeadType
			(t12, typename TypeListType<T13>::HeadType
			(t13, NullTypeList()))))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11,
	class T12>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11,T12>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11,T12>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11,T12>::HeadType
			(t8, typename TypeListType<T9,T10,T11,T12>::HeadType
			(t9, typename TypeListType<T10,T11,T12>::HeadType
			(t10, typename TypeListType<T11,T12>::HeadType
			(t11, typename TypeListType<T12>::HeadType
			(t12, NullTypeList())))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10,
	class T11>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10,T11>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10,T11>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10,T11>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10,T11>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10,T11>::HeadType
			(t7, typename TypeListType<T8,T9,T10,T11>::HeadType
			(t8, typename TypeListType<T9,T10,T11>::HeadType
			(t9, typename TypeListType<T10,T11>::HeadType
			(t10, typename TypeListType<T11>::HeadType
			(t11, NullTypeList()))))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9,
	class T10>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9,T10>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9,T10>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9,T10>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9,T10>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9,T10>::HeadType
			(t6, typename TypeListType<T7,T8,T9,T10>::HeadType
			(t7, typename TypeListType<T8,T9,T10>::HeadType
			(t8, typename TypeListType<T9,T10>::HeadType
			(t9, typename TypeListType<T10>::HeadType
			(t10, NullTypeList())))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8,
	class T9>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8),
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8,T9>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8,T9>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8,T9>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8,T9>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8,T9>::HeadType
			(t5, typename TypeListType<T6,T7,T8,T9>::HeadType
			(t6, typename TypeListType<T7,T8,T9>::HeadType
			(t7, typename TypeListType<T8,T9>::HeadType
			(t8, typename TypeListType<T9>::HeadType
			(t9, NullTypeList()))))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7,
	class T8>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7,T8>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7,T8>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7,T8>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7,T8>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7,T8>::HeadType
			(t4, typename TypeListType<T5,T6,T7,T8>::HeadType
			(t5, typename TypeListType<T6,T7,T8>::HeadType
			(t6, typename TypeListType<T7,T8>::HeadType
			(t7, typename TypeListType<T8>::HeadType
			(t8, NullTypeList())))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7>
struct Tuple<T0,T1,T2,T3,T4,T5,T6,T7, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6,T7>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6,T7>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6,T7>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6,T7>::HeadType
			(t3, typename TypeListType<T4,T5,T6,T7>::HeadType
			(t4, typename TypeListType<T5,T6,T7>::HeadType
			(t5, typename TypeListType<T6,T7>::HeadType
			(t6, typename TypeListType<T7>::HeadType
			(t7, NullTypeList()))))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6>
struct Tuple<T0,T1,T2,T3,T4,T5,T6, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5,T6>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5,T6>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5,T6>::HeadType
			(t2, typename TypeListType<T3,T4,T5,T6>::HeadType
			(t3, typename TypeListType<T4,T5,T6>::HeadType
			(t4, typename TypeListType<T5,T6>::HeadType
			(t5, typename TypeListType<T6>::HeadType
			(t6, NullTypeList())))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4,
	class T5>
struct Tuple<T0,T1,T2,T3,T4,T5, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4,T5>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5)):
		_data(t0, typename TypeListType<T1,T2,T3,T4,T5>::HeadType
			(t1, typename TypeListType<T2,T3,T4,T5>::HeadType
			(t2, typename TypeListType<T3,T4,T5>::HeadType
			(t3, typename TypeListType<T4,T5>::HeadType
			(t4, typename TypeListType<T5>::HeadType
			(t5, NullTypeList()))))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3,
	class T4>
struct Tuple<T0,T1,T2,T3,T4, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3,T4>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4)):
		_data(t0, typename TypeListType<T1,T2,T3,T4>::HeadType
			(t1, typename TypeListType<T2,T3,T4>::HeadType
			(t2, typename TypeListType<T3,T4>::HeadType
			(t3, typename TypeListType<T4>::HeadType
			(t4, NullTypeList())))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2,
	class T3>
struct Tuple<T0,T1,T2,T3, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2,T3>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2),
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3)):
		_data(t0, typename TypeListType<T1,T2,T3>::HeadType
			(t1, typename TypeListType<T2,T3>::HeadType
			(t2, typename TypeListType<T3>::HeadType
			(t3, NullTypeList()))))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1,
	class T2>
struct Tuple<T0,T1,T2, NullTypeList>
{
	using Type = typename TypeListType<T0,T1,T2>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2)):
		_data(t0, typename TypeListType<T1,T2>::HeadType
			(t1, typename TypeListType<T2>::HeadType
			(t2, NullTypeList())))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0,
	class T1>
struct Tuple<T0,T1, NullTypeList>
{
	using Type = typename TypeListType<T0,T1>::HeadType;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1)):
		_data(t0, typename TypeListType<T1>::HeadType(t1, NullTypeList()))
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


template <class T0>
struct Tuple<T0, NullTypeList>
{
	using Type = TypeList<T0, NullTypeList>;

	enum TupleLengthType
	{
		length = Type::length
	};

	Tuple():_data()
	{
	}

	Tuple(typename TypeWrapper<T0>::CONSTTYPE& t0):
		_data(t0, NullTypeList())
	{
	}

	template <int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data);
	}

	template <int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		Getter<N>::template get<typename TypeGetter<N, Type>::HeadType, typename Type::HeadType, typename Type::TailType>(_data) = val;
	}

	bool operator == (const Tuple& other) const
	{
		return _data == other._data;
	}

	bool operator != (const Tuple& other) const
	{
		return !(_data == other._data);
	}

	bool operator < (const Tuple& other) const
	{
		return _data < other._data;
	}

private:
	Type _data;
};


} // namespace Poco


#endif // Foundation_Tuple_INCLUDED
