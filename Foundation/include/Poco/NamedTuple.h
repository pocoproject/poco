//
// NamedTuple.h
//
// Library: Foundation
// Package: Core
// Module:  NamedTuple
//
// Definition of the NamedTuple class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NamedTuple_INCLUDED
#define Foundation_NamedTuple_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Tuple.h"
#include "Poco/TypeList.h"
#include "Poco/DynamicAny.h"
#include "Poco/SharedPtr.h"
#include "Poco/Format.h"


namespace Poco {


template<class T0,
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
struct NamedTuple: public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34), 
		typename TypeWrapper<T15>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35), 
		typename TypeWrapper<T16>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36), 
		typename TypeWrapper<T17>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37), 
		typename TypeWrapper<T18>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38), 
		typename TypeWrapper<T19>::CONSTTYPE& t39 = POCO_TYPEWRAPPER_DEFAULTVALUE(T39)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const std::string& n35 = "J1",
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		const std::string& n36 = "K1",
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		const std::string& n37 = "L1",
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37),
		const std::string& n38 = "M1",
		typename TypeWrapper<T38>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38),
		const std::string& n39 = "N1",
		typename TypeWrapper<T39>::CONSTTYPE& t39 = POCO_TYPEWRAPPER_DEFAULTVALUE(T39)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34,n35,n36,n37,n38,n39);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					case 35: return TupleType::template get<35>();
					case 36: return TupleType::template get<36>();
					case 37: return TupleType::template get<37>();
					case 38: return TupleType::template get<38>();
					case 39: return TupleType::template get<39>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1",
		const std::string& n35 = "J1",
		const std::string& n36 = "K1",
		const std::string& n37 = "L1",
		const std::string& n38 = "M1",
		const std::string& n39 = "N1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
			_pNames->push_back(n35);
			_pNames->push_back(n36);
			_pNames->push_back(n37);
			_pNames->push_back(n38);
			_pNames->push_back(n39);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34), 
		typename TypeWrapper<T15>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35), 
		typename TypeWrapper<T16>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36), 
		typename TypeWrapper<T17>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37), 
		typename TypeWrapper<T18>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const std::string& n35 = "J1",
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		const std::string& n36 = "K1",
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		const std::string& n37 = "L1",
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37),
		const std::string& n38 = "M1",
		typename TypeWrapper<T38>::CONSTTYPE& t38 = POCO_TYPEWRAPPER_DEFAULTVALUE(T38)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34,n35,n36,n37,n38);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					case 35: return TupleType::template get<35>();
					case 36: return TupleType::template get<36>();
					case 37: return TupleType::template get<37>();
					case 38: return TupleType::template get<38>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1",
		const std::string& n35 = "J1",
		const std::string& n36 = "K1",
		const std::string& n37 = "L1",
		const std::string& n38 = "M1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
			_pNames->push_back(n35);
			_pNames->push_back(n36);
			_pNames->push_back(n37);
			_pNames->push_back(n38);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34), 
		typename TypeWrapper<T15>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35), 
		typename TypeWrapper<T16>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36), 
		typename TypeWrapper<T17>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const std::string& n35 = "J1",
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		const std::string& n36 = "K1",
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36),
		const std::string& n37 = "L1",
		typename TypeWrapper<T37>::CONSTTYPE& t37 = POCO_TYPEWRAPPER_DEFAULTVALUE(T37)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34,n35,n36,n37);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					case 35: return TupleType::template get<35>();
					case 36: return TupleType::template get<36>();
					case 37: return TupleType::template get<37>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1",
		const std::string& n35 = "J1",
		const std::string& n36 = "K1",
		const std::string& n37 = "L1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
			_pNames->push_back(n35);
			_pNames->push_back(n36);
			_pNames->push_back(n37);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34), 
		typename TypeWrapper<T15>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35), 
		typename TypeWrapper<T16>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const std::string& n35 = "J1",
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35),
		const std::string& n36 = "K1",
		typename TypeWrapper<T36>::CONSTTYPE& t36 = POCO_TYPEWRAPPER_DEFAULTVALUE(T36)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34,n35,n36);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					case 35: return TupleType::template get<35>();
					case 36: return TupleType::template get<36>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1",
		const std::string& n35 = "J1",
		const std::string& n36 = "K1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
			_pNames->push_back(n35);
			_pNames->push_back(n36);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34), 
		typename TypeWrapper<T15>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34),
		const std::string& n35 = "J1",
		typename TypeWrapper<T35>::CONSTTYPE& t35 = POCO_TYPEWRAPPER_DEFAULTVALUE(T35)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34,n35);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					case 35: return TupleType::template get<35>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1",
		const std::string& n35 = "J1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
			_pNames->push_back(n35);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33), 
		typename TypeWrapper<T14>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33),
		const std::string& n34 = "I1",
		typename TypeWrapper<T34>::CONSTTYPE& t34 = POCO_TYPEWRAPPER_DEFAULTVALUE(T34)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					case 34: return TupleType::template get<34>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1",
		const std::string& n34 = "I1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
			_pNames->push_back(n34);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32), 
		typename TypeWrapper<T13>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32),
		const std::string& n33 = "H1",
		typename TypeWrapper<T33>::CONSTTYPE& t33 = POCO_TYPEWRAPPER_DEFAULTVALUE(T33)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					case 33: return TupleType::template get<33>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1",
		const std::string& n33 = "H1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
			_pNames->push_back(n33);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31), 
		typename TypeWrapper<T12>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31),
		const std::string& n32 = "G1",
		typename TypeWrapper<T32>::CONSTTYPE& t32 = POCO_TYPEWRAPPER_DEFAULTVALUE(T32)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					case 32: return TupleType::template get<32>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1",
		const std::string& n32 = "G1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
			_pNames->push_back(n32);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30), 
		typename TypeWrapper<T11>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30),
		const std::string& n31 = "F1",
		typename TypeWrapper<T31>::CONSTTYPE& t31 = POCO_TYPEWRAPPER_DEFAULTVALUE(T31)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					case 31: return TupleType::template get<31>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1",
		const std::string& n31 = "F1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
			_pNames->push_back(n31);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29), 
		typename TypeWrapper<T10>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29),
		const std::string& n30 = "E1",
		typename TypeWrapper<T30>::CONSTTYPE& t30 = POCO_TYPEWRAPPER_DEFAULTVALUE(T30)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					case 30: return TupleType::template get<30>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1",
		const std::string& n30 = "E1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
			_pNames->push_back(n30);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		typename TypeWrapper<T9>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28), 
		const std::string& n29 = "D1",
		typename TypeWrapper<T29>::CONSTTYPE& t29 = POCO_TYPEWRAPPER_DEFAULTVALUE(T29)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					case 29: return TupleType::template get<29>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1",
		const std::string& n29 = "D1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
			_pNames->push_back(n29);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		typename TypeWrapper<T8>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27),
		const std::string& n28 = "C1",
		typename TypeWrapper<T28>::CONSTTYPE& t28 = POCO_TYPEWRAPPER_DEFAULTVALUE(T28)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					case 28: return TupleType::template get<28>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1",
		const std::string& n28 = "C1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
			_pNames->push_back(n28);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26),
		typename TypeWrapper<T7>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26), 
		const std::string& n27 = "B1",
		typename TypeWrapper<T27>::CONSTTYPE& t27 = POCO_TYPEWRAPPER_DEFAULTVALUE(T27)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					case 27: return TupleType::template get<27>(); 
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1",
		const std::string& n27 = "B1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
			_pNames->push_back(n27);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25), 
		typename TypeWrapper<T6>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25),
		const std::string& n26 = "A1",
		typename TypeWrapper<T26>::CONSTTYPE& t26 = POCO_TYPEWRAPPER_DEFAULTVALUE(T26)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					case 26: return TupleType::template get<26>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z", 
		const std::string& n26 = "A1")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
			_pNames->push_back(n26);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24),
		typename TypeWrapper<T5>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24), 
		const std::string& n25 = "Z",
		typename TypeWrapper<T25>::CONSTTYPE& t25 = POCO_TYPEWRAPPER_DEFAULTVALUE(T25)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					case 25: return TupleType::template get<25>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y",
		const std::string& n25 = "Z")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
			_pNames->push_back(n25);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		typename TypeWrapper<T4>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23),
		const std::string& n24 = "Y",
		typename TypeWrapper<T24>::CONSTTYPE& t24 = POCO_TYPEWRAPPER_DEFAULTVALUE(T24)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					case 24: return TupleType::template get<24>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X",
		const std::string& n24 = "Y")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
			_pNames->push_back(n24);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23),
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		typename TypeWrapper<T3>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22), 
		const std::string& n23 = "X",
		typename TypeWrapper<T23>::CONSTTYPE& t23 = POCO_TYPEWRAPPER_DEFAULTVALUE(T23)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					case 23: return TupleType::template get<23>(); 
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W",
		const std::string& n23 = "X")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
			_pNames->push_back(n23);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		typename TypeWrapper<T2>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21),
		const std::string& n22 = "W",
		typename TypeWrapper<T22>::CONSTTYPE& t22 = POCO_TYPEWRAPPER_DEFAULTVALUE(T22)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					case 22: return TupleType::template get<22>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V",
		const std::string& n22 = "W")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
			_pNames->push_back(n22);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20),
		typename TypeWrapper<T1>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20), 
		const std::string& n21 = "V",
		typename TypeWrapper<T21>::CONSTTYPE& t21 = POCO_TYPEWRAPPER_DEFAULTVALUE(T21)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					case 21: return TupleType::template get<21>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U",
		const std::string& n21 = "V")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
			_pNames->push_back(n21);
		}
	}

	NameVecPtr _pNames;
};


template<class T0,
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		typename TypeWrapper<T0>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19),
		const std::string& n20 = "U",
		typename TypeWrapper<T20>::CONSTTYPE& t20 = POCO_TYPEWRAPPER_DEFAULTVALUE(T20)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					case 20: return TupleType::template get<20>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T",
		const std::string& n20 = "U")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3);
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9);
			_pNames->push_back(n10);
			_pNames->push_back(n11);
			_pNames->push_back(n12);
			_pNames->push_back(n13);
			_pNames->push_back(n14);
			_pNames->push_back(n15);
			_pNames->push_back(n16);
			_pNames->push_back(n17);
			_pNames->push_back(n18);
			_pNames->push_back(n19);
			_pNames->push_back(n20);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18),
		const std::string& n19 = "T",
		typename TypeWrapper<T19>::CONSTTYPE& t19 = POCO_TYPEWRAPPER_DEFAULTVALUE(T19)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					case 19: return TupleType::template get<19>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S",
		const std::string& n19 = "T")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
			_pNames->push_back(n15); 
			_pNames->push_back(n16); 
			_pNames->push_back(n17); 
			_pNames->push_back(n18); 
			_pNames->push_back(n19); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17),
		const std::string& n18 = "S",
		typename TypeWrapper<T18>::CONSTTYPE& t18 = POCO_TYPEWRAPPER_DEFAULTVALUE(T18)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					case 18: return TupleType::template get<18>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R",
		const std::string& n18 = "S")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
			_pNames->push_back(n15); 
			_pNames->push_back(n16); 
			_pNames->push_back(n17); 
			_pNames->push_back(n18); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16),
		const std::string& n17 = "R",
		typename TypeWrapper<T17>::CONSTTYPE& t17 = POCO_TYPEWRAPPER_DEFAULTVALUE(T17)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					case 17: return TupleType::template get<17>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q",
		const std::string& n17 = "R")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
			_pNames->push_back(n15); 
			_pNames->push_back(n16); 
			_pNames->push_back(n17); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16>::Type Type;
	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15),
		const std::string& n16 = "Q",
		typename TypeWrapper<T16>::CONSTTYPE& t16 = POCO_TYPEWRAPPER_DEFAULTVALUE(T16)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					case 16: return TupleType::template get<16>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P",
		const std::string& n16 = "Q")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
			_pNames->push_back(n15); 
			_pNames->push_back(n16); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14),
		const std::string& n15 = "P",
		typename TypeWrapper<T15>::CONSTTYPE& t15 = POCO_TYPEWRAPPER_DEFAULTVALUE(T15)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					case 15: return TupleType::template get<15>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O",
		const std::string& n15 = "P")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
			_pNames->push_back(n15); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13),
		const std::string& n14 = "O",
		typename TypeWrapper<T14>::CONSTTYPE& t14 = POCO_TYPEWRAPPER_DEFAULTVALUE(T14)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					case 14: return TupleType::template get<14>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N",
		const std::string& n14 = "O")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
			_pNames->push_back(n14); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12),
		const std::string& n13 = "N",
		typename TypeWrapper<T13>::CONSTTYPE& t13 = POCO_TYPEWRAPPER_DEFAULTVALUE(T13)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					case 13: return TupleType::template get<13>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M",
		const std::string& n13 = "N")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
			_pNames->push_back(n13); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11),
		const std::string& n12 = "M",
		typename TypeWrapper<T12>::CONSTTYPE& t12 = POCO_TYPEWRAPPER_DEFAULTVALUE(T12)): 
	TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12), _pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					case 12: return TupleType::template get<12>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L",
		const std::string& n12 = "M")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
			_pNames->push_back(n12); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10),
		const std::string& n11 = "L",
		typename TypeWrapper<T11>::CONSTTYPE& t11 = POCO_TYPEWRAPPER_DEFAULTVALUE(T11)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					case 11: return TupleType::template get<11>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K",
		const std::string& n11 = "L")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
			_pNames->push_back(n11); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
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
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
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
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9),
		const std::string& n10 = "K",
		typename TypeWrapper<T10>::CONSTTYPE& t10 = POCO_TYPEWRAPPER_DEFAULTVALUE(T10)): 
	TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10), _pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					case 10: return TupleType::template get<10>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J",
		const std::string& n10 = "K")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
			_pNames->push_back(n10); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7, 
	class T8,
	class T9>
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8), 
		const std::string& n9 = "J",
		typename TypeWrapper<T9>::CONSTTYPE& t9 = POCO_TYPEWRAPPER_DEFAULTVALUE(T9)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					case 9: return TupleType::template get<9>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I",
		const std::string& n9 = "J")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
			_pNames->push_back(n9); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7, 
	class T8>
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7),
		const std::string& n8 = "I",
		typename TypeWrapper<T8>::CONSTTYPE& t8 = POCO_TYPEWRAPPER_DEFAULTVALUE(T8)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7,t8), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7,n8);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					case 8: return TupleType::template get<8>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H",
		const std::string& n8 = "I")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
			_pNames->push_back(n8);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6,
	class T7>
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,T7,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6,T7>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6,T7> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6,T7>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6),
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6), 
		const std::string& n7 = "H",
		typename TypeWrapper<T7>::CONSTTYPE& t7 = POCO_TYPEWRAPPER_DEFAULTVALUE(T7)): 
		TupleType(t0,t1,t2,t3,t4,t5,t6,t7), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6,n7);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					case 7: return TupleType::template get<7>(); 
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G",
		const std::string& n7 = "H")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
			_pNames->push_back(n7);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4,
	class T5,
	class T6>
struct NamedTuple<T0,T1,T2,T3,T4,T5,T6,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5,T6>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5,T6> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5,T6>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6)): 
	TupleType(t0,t1,t2,t3,t4,t5,t6), _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5), 
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6)): 
	TupleType(t0,t1,t2,t3,t4,t5,t6)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5),
		const std::string& n6 = "G",
		typename TypeWrapper<T6>::CONSTTYPE& t6 = POCO_TYPEWRAPPER_DEFAULTVALUE(T6)): 
	TupleType(t0,t1,t2,t3,t4,t5,t6), _pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5,n6);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					case 6: return TupleType::template get<6>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F", 
		const std::string& n6 = "G")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
			_pNames->push_back(n6);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4,
	class T5>
struct NamedTuple<T0,T1,T2,T3,T4,T5,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4,T5>
{
	typedef Tuple<T0,T1,T2,T3,T4,T5> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4,T5>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5)): 
	TupleType(t0,t1,t2,t3,t4,t5), _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4),
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5)): 
	TupleType(t0,t1,t2,t3,t4,t5)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4), 
		const std::string& n5 = "F",
		typename TypeWrapper<T5>::CONSTTYPE& t5 = POCO_TYPEWRAPPER_DEFAULTVALUE(T5)): 
	TupleType(t0,t1,t2,t3,t4,t5), _pNames(0) 
	{
		init(n0,n1,n2,n3,n4,n5);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					case 5: return TupleType::template get<5>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E",
		const std::string& n5 = "F")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
			_pNames->push_back(n5);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3,
	class T4>
struct NamedTuple<T0,T1,T2,T3,T4,NullTypeList>:
	public Tuple<T0,T1,T2,T3,T4>
{
	typedef Tuple<T0,T1,T2,T3,T4> TupleType;
	typedef typename Tuple<T0,T1,T2,T3,T4>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4)): 
		TupleType(t0,t1,t2,t3,t4), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4)): 
		TupleType(t0,t1,t2,t3,t4)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3),
		const std::string& n4 = "E",
		typename TypeWrapper<T4>::CONSTTYPE& t4 = POCO_TYPEWRAPPER_DEFAULTVALUE(T4)): 
		TupleType(t0,t1,t2,t3,t4), 
		_pNames(0) 
	{
		init(n0,n1,n2,n3,n4);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					case 4: return TupleType::template get<4>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D",
		const std::string& n4 = "E")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
			_pNames->push_back(n4);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2,
	class T3>
struct NamedTuple<T0,T1,T2,T3,NullTypeList>:
	public Tuple<T0,T1,T2,T3>
{
	typedef Tuple<T0,T1,T2,T3> TupleType;
	typedef typename Tuple<T0,T1,T2,T3>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3)): 
		TupleType(t0,t1,t2,t3), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3)): 
		TupleType(t0,t1,t2,t3)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2), 
		const std::string& n3 = "D",
		typename TypeWrapper<T3>::CONSTTYPE& t3 = POCO_TYPEWRAPPER_DEFAULTVALUE(T3)): 
	TupleType(t0,t1,t2,t3), _pNames(0) 
	{
		init(n0,n1,n2,n3);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					case 3: return TupleType::template get<3>(); 
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C",
		const std::string& n3 = "D")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
			_pNames->push_back(n3); 
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1,
	class T2>
struct NamedTuple<T0,T1,T2,NullTypeList>:
	public Tuple<T0,T1,T2>
{
	typedef Tuple<T0,T1,T2> TupleType;
	typedef typename Tuple<T0,T1,T2>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1), 
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2)): 
		TupleType(t0,t1,t2), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2)): 
		TupleType(t0,t1,t2)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1),
		const std::string& n2 = "C",
		typename TypeWrapper<T2>::CONSTTYPE& t2 = POCO_TYPEWRAPPER_DEFAULTVALUE(T2)): 
		TupleType(t0,t1,t2), 
		_pNames(0) 
	{
		init(n0,n1,n2);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					case 2: return TupleType::template get<2>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B",
		const std::string& n2 = "C")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
			_pNames->push_back(n2);
		}
	}

	NameVecPtr _pNames;
};


template<class T0, 
	class T1>
struct NamedTuple<T0,T1,NullTypeList>:
	public Tuple<T0,T1>
{
	typedef Tuple<T0,T1> TupleType;
	typedef typename Tuple<T0,T1>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1)): 
		TupleType(t0,t1), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0,
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1)): 
		TupleType(t0,t1)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0,
		typename TypeWrapper<T0>::CONSTTYPE& t0, 
		const std::string& n1 = "B",
		typename TypeWrapper<T1>::CONSTTYPE& t1 = POCO_TYPEWRAPPER_DEFAULTVALUE(T1)): 
		TupleType(t0,t1), 
		_pNames(0) 
	{
		init(n0,n1);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					case 1: return TupleType::template get<1>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A",
		const std::string& n1 = "B")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
			_pNames->push_back(n1);
		}
	}

	NameVecPtr _pNames;
};


template<class T0>
struct NamedTuple<T0,NullTypeList>:
	public Tuple<T0>
{
	typedef Tuple<T0> TupleType;
	typedef typename Tuple<T0>::Type Type;

	typedef std::vector<std::string> NameVec; 
	typedef SharedPtr<NameVec> NameVecPtr; 

	NamedTuple(): _pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames) 
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(typename TypeWrapper<T0>::CONSTTYPE& t0): 
		TupleType(t0), 
		_pNames(0)
	{
		init();
	}

	NamedTuple(const NameVecPtr& rNames, 
		typename TypeWrapper<T0>::CONSTTYPE& t0): 
		TupleType(t0)
	{
		if (rNames->size() != TupleType::length)
			throw InvalidArgumentException("Wrong names vector length."); 

		_pNames = rNames;
	}

	NamedTuple(const std::string& n0, typename TypeWrapper<T0>::CONSTTYPE& t0): 
		TupleType(t0), 
		_pNames(0) 
	{
		init(n0);
	}

	const DynamicAny get(const std::string& name) const
	{
		NameVec::const_iterator it = _pNames->begin(); 
		NameVec::const_iterator itEnd = _pNames->end();

		for(std::size_t counter = 0; it != itEnd; ++it, ++counter)
		{
			if (name == *it)
			{
				switch (counter)
				{ 
					case 0: return TupleType::template get<0>();
					default: throw RangeException();
				}
			}
		} 

		throw NotFoundException("Name not found: " + name);
	}

	const DynamicAny operator [] (const std::string& name) const
	{
		return get(name);
	}

	template<int N>
	typename TypeGetter<N, Type>::ConstHeadType& get() const 
	{
		return TupleType::template get<N>();
	}

	template<int N>
	typename TypeGetter<N, Type>::HeadType& get()
	{
		return TupleType::template get<N>();
	}

	template<int N>
	void set(typename TypeGetter<N, Type>::ConstHeadType& val)
	{
		return TupleType::template set<N>(val);
	}

	const NameVecPtr& names()
	{
		return _pNames; 
	}

	void setName(std::size_t index, const std::string& name)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		(*_pNames)[index] = name;
	}

	const std::string& getName(std::size_t index)
	{
		if (index >= _pNames->size())
			throw InvalidArgumentException(format("Invalid index: %z", index));

		return (*_pNames)[index];
	}

	bool operator == (const NamedTuple& other) const
	{
		return TupleType(*this) == TupleType(other) && _pNames == other._pNames;
	}

	bool operator != (const NamedTuple& other) const 
	{
		return !(*this == other);
	}

	bool operator < (const NamedTuple& other) const
	{
		TupleType th(*this);
		TupleType oth(other); 

		return (th < oth && _pNames == other._pNames) || 
			(th == oth && _pNames < other._pNames) ||
			(th < oth && _pNames < other._pNames);
	}

private:
	void init(const std::string& n0 = "A")
	{ 
		if (!_pNames)
		{
			_pNames = new NameVec;
			_pNames->push_back(n0);
		}
	}

	NameVecPtr _pNames;
};


} // namespace Poco


#endif // Foundation_Tuple_INCLUDED
