//
// TypeHandler.h
//
// Library: Data
// Package: DataCore
// Module:  TypeHandler
//
// Definition of the TypeHandler class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_TypeHandler_INCLUDED
#define Data_TypeHandler_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/AbstractBinder.h"
#include "Poco/Data/AbstractExtractor.h"
#include "Poco/Data/AbstractPreparator.h"
#include "Poco/Nullable.h"
#include "Poco/Tuple.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include <cstddef>
#include <optional>

#if defined(POCO_COMPILER_GCC) && (__GNUC__ >= 12)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wuse-after-free"
#elif defined (POCO_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable : 4624)
#endif

namespace Poco {
namespace Data {


class AbstractTypeHandler
	/// Parent class for type handlers.
	/// The reason for this class is to prevent instantiations of type handlers.
	/// For documentation on type handlers, see TypeHandler class.
{
public:
	AbstractTypeHandler() = delete;
	~AbstractTypeHandler() = delete;
	AbstractTypeHandler(const AbstractTypeHandler&) = delete;
	AbstractTypeHandler& operator = (const AbstractTypeHandler&) = delete;
};


template <typename T>
class TypeHandler: public AbstractTypeHandler
	/// Converts Rows to a Type and the other way around. Provide template specializations to support your own complex types.
	///
	/// Take as example the following (simplified) class:
	///	class Person
	///	{
	///	private:
	///		std::string _lastName;
	///		std::string _firstName;
	///		int		 _age;
	///	public:
	///		const std::string& getLastName();
	///		[...] // other set/get methods (returning const reference), a default constructor,
	///		[...] // optional < operator (for set, multiset) or function operator (for map, multimap)
	///	};
	///
	/// The TypeHandler must provide a custom bind, size, prepare and extract method:
	///
	///	template <>
	///	class TypeHandler<struct Person>
	///	{
	///	public:
	///		static std::size_t size()
	///		{
	///			return 3; // lastName + firstname + age occupy three columns
	///		}
	///
	///		static void bind(std::size_t pos, const Person& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	///		{
	///			// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Age INTEGER(3))
	///			// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
	///			poco_assert_dbg (!pBinder.isNull());
	///			   TypeHandler<std::string>::bind(pos++, obj.getLastName(), pBinder, dir);
	///			   TypeHandler<std::string>::bind(pos++, obj.getFirstName(), pBinder, dir);
	///			   TypeHandler<int>::bind(pos++, obj.getAge(), pBinder, dir);
	///		}
	///
	///		static void prepare(std::size_t pos, const Person& obj, AbstractPreparator::Ptr pPreparator)
	///		{
	///			// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Age INTEGER(3))
	///			poco_assert_dbg (!pPreparator.isNull());
	///			   TypeHandler<std::string>::prepare(pos++, obj.getLastName(), pPreparator);
	///			   TypeHandler<std::string>::prepare(pos++, obj.getFirstName(), pPreparator);
	///			   TypeHandler<int>::prepare(pos++, obj.getAge(), pPreparator);
	///		}
	///
	///		static void extract(std::size_t pos, Person& obj, const Person& defVal, AbstractExtractor::Ptr pExt)
	///		{
	///			// defVal is the default person we should use if we encunter NULL entries, so we take the individual fields
	///			// as defaults. You can do more complex checking, ie return defVal if only one single entry of the fields is null etc...
	///			poco_assert_dbg (!pExt.isNull());
	///			std::string lastName;
	///			std::string firstName;
	///			int age = 0;
	///			// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Age INTEGER(3))
	///			   TypeHandler<std::string>::extract(pos++, lastName, defVal.getLastName(), pExt);
	///			   TypeHandler<std::string>::extract(pos++, firstName, defVal.getFirstName(), pExt);
	///			   TypeHandler<int>::extract(pos++, age, defVal.getAge(), pExt);
	///			obj.setLastName(lastName);
	///			obj.setFirstName(firstName);
	///			obj.setAge(age);
	///		}
	///	};
	///
	/// Note that the TypeHandler template specialization must always be declared in the namespace Poco::Data.
	/// Apart from that no further work is needed. One can now use Person with into and use clauses.
{
public:
	static void bind(std::size_t pos, const T& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos, obj, dir);
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, T& obj, const T& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		if (!pExt->extract(pos, obj)) obj = defVal;
	}

	static void prepare(std::size_t pos, const T& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		pPreparator->prepare(pos, obj);
	}
};


template <typename T>
class TypeHandler<std::deque<T>>: public AbstractTypeHandler
	/// Specialization of type handler for std::deque.
{
public:
	static void bind(std::size_t pos, const std::deque<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos, obj, dir);
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, std::deque<T>& obj, const T& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		if (!pExt->extract(pos, obj))
			obj.assign(obj.size(), defVal);
	}

	static void prepare(std::size_t pos, const std::deque<T>& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		pPreparator->prepare(pos, obj);
	}
};


template <typename T>
class TypeHandler<std::vector<T>>: public AbstractTypeHandler
	/// Specialization of type handler for std::vector.
{
public:
	static void bind(std::size_t pos, const std::vector<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos, obj, dir);
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, std::vector<T>& obj, const T& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		if (!pExt->extract(pos, obj))
			obj.assign(obj.size(), defVal);
	}

	static void prepare(std::size_t pos, const std::vector<T>& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		pPreparator->prepare(pos, obj);
	}
};


template <typename T>
class TypeHandler<std::list<T>>: public AbstractTypeHandler
	/// Specialization of type handler for std::list.
{
public:
	static void bind(std::size_t pos, const std::list<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos, obj, dir);
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, std::list<T>& obj, const T& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		if (!pExt->extract(pos, obj))
			obj.assign(obj.size(), defVal);
	}

	static void prepare(std::size_t pos, const std::list<T>& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		pPreparator->prepare(pos, obj);
	}
};

// define this macro to nothing for smaller code size
#define POCO_TUPLE_TYPE_HANDLER_INLINE inline


template <typename... T>
class TypeHandler<std::tuple<T...>>: public AbstractTypeHandler
	/// Specialization of type handler for std::tuple.
{
public:
	static void bind(std::size_t pos, const std::tuple<T...> & t, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		tupleBind<0>(pos, t, pBinder, dir);
	}

	static void prepare(std::size_t pos, const std::tuple<T...> & t, AbstractPreparator::Ptr pPrepare)
	{
		poco_assert_dbg (!pPrepare.isNull());
		tuplePrepare<0>(pos, t, pPrepare);
	}

	static std::size_t size()
	{
		std::size_t sz = 0;
		tupleSize<0>(sz);
		return sz;
	}

	static void extract(std::size_t pos, std::tuple<T...>& t, const std::tuple<T...>& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		tupleExtract<0>(pos, t, defVal, pExt);
	}

private:
	template <std::size_t N>
	static POCO_TUPLE_TYPE_HANDLER_INLINE void tupleBind(std::size_t& pos, const std::tuple<T...>& t, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		if constexpr (N < sizeof...(T))
		{
			using Type = std::tuple_element_t<N, std::tuple<T...>>;
			TypeHandler<Type>::bind(pos, std::get<N>(t), pBinder, dir);
			pos += TypeHandler<Type>::size();
			tupleBind<N+1>(pos, t, pBinder, dir);
		}
	}

	template <std::size_t N>
	static POCO_TUPLE_TYPE_HANDLER_INLINE void tuplePrepare(std::size_t& pos, const std::tuple<T...>& t, AbstractPreparator::Ptr pPreparator)
	{
		if constexpr (N < sizeof...(T))
		{
			using Type = std::tuple_element_t<N, std::tuple<T...>>;
			TypeHandler<Type>::prepare(pos, std::get<N>(t), pPreparator);
			pos += TypeHandler<Type>::size();
			tuplePrepare<N+1>(pos, t, pPreparator);
		}
	}

	template <std::size_t N>
	static POCO_TUPLE_TYPE_HANDLER_INLINE void tupleSize(std::size_t& sz)
	{
		if constexpr (N < sizeof...(T))
		{
			using Type = std::tuple_element_t<N, std::tuple<T...>>;
			sz += TypeHandler<Type>::size();
			tupleSize<N+1>(sz);
		}
	}

	template <std::size_t N>
	static POCO_TUPLE_TYPE_HANDLER_INLINE void tupleExtract(std::size_t& pos, std::tuple<T...>& t, const std::tuple<T...>& defVal, AbstractExtractor::Ptr pExt)
	{
		if constexpr (N < sizeof...(T))
		{
			using Type = std::tuple_element_t<N, std::tuple<T...>>;
			auto dVal = std::get<N>(defVal);
			TypeHandler<Type>::extract(pos, std::get<N>(t), dVal, pExt);
			pos += TypeHandler<Type>::size();
			tupleExtract<N+1>(pos, t, defVal, pExt);
		}
	}
};


template <typename T>
class TypeHandler<std::optional<T>>: public AbstractTypeHandler
	/// Specialization of type handler for std::optional.
{
public:
	static void bind(std::size_t pos, const std::optional<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		if (obj)
			pBinder->bind(pos++, *obj, dir);
		else
			pBinder->bind(pos++, Poco::Data::Keywords::null, dir);
	}

	static void prepare(std::size_t pos, const std::optional<T>& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		if (obj)
			pPreparator->prepare(pos++, *obj);
		else
			pPreparator->prepare(pos++, T()); //Poco::Data::Keywords::null);
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, std::optional<T>& obj, const std::optional<T>& , AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		T val;
		if (pExt->extract(pos++, val))
			obj = std::move(val);
		else
			obj.reset();
	}
};


template <typename T>
class TypeHandler<Nullable<T>>: public AbstractTypeHandler
	/// Specialization of type handler for Nullable.
{
public:
	static void bind(std::size_t pos, const Nullable<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg (!pBinder.isNull());
		if (obj.isNull())
			pBinder->bind(pos++, Poco::Data::Keywords::null, dir);
		else
			pBinder->bind(pos++, obj.value(), dir);
	}

	static void prepare(std::size_t pos, const Nullable<T>& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		if (obj.isNull())
			pPreparator->prepare(pos++, T());
		else
			pPreparator->prepare(pos++, obj.value());
	}

	static std::size_t size()
	{
		return 1u;
	}

	static void extract(std::size_t pos, Nullable<T>& obj, const Nullable<T>& , AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		pExt->extract(pos++, obj);
	}
};


template <typename... T>
class TypeHandler<Poco::Tuple<T...>>: public AbstractTypeHandler
	/// Specialization of type handler for Tuple.
{
public:
	using TupleConstRef = typename Poco::TypeWrapper<Poco::Tuple<T...>>::CONSTREFTYPE;
	using TupleRef = typename Poco::TypeWrapper<Poco::Tuple<T...>>::REFTYPE;
	using TupleType = typename Poco::Tuple<T...>::TupleType;

	static void bind(std::size_t pos, TupleConstRef tuple, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<TupleType>::bind(pos, static_cast<const TupleType&>(tuple), pBinder, dir);
	}

	static void prepare(std::size_t pos, TupleConstRef tuple, AbstractPreparator::Ptr pPreparator)
	{
		TypeHandler<TupleType>::prepare(pos, static_cast<const TupleType&>(tuple), pPreparator);
	}

	static std::size_t size()
	{
		return TypeHandler<TupleType>::size();
	}

	static void extract(std::size_t pos, TupleRef tuple, TupleConstRef defVal, AbstractExtractor::Ptr pExt)
	{
		TypeHandler<TupleType>::extract(pos, static_cast<TupleType&>(tuple), static_cast<const TupleType&>(defVal), pExt);
	}
};


template <typename K, typename V>
class TypeHandler<std::pair<K, V>>: public AbstractTypeHandler
{
public:
	static void bind(std::size_t pos, const std::pair<K, V>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<K>::bind(pos, obj.first, pBinder, dir);
		pos += TypeHandler<K>::size();
		TypeHandler<V>::bind(pos, obj.second, pBinder, dir);
	}

	static std::size_t size()
	{
		return TypeHandler<K>::size() + TypeHandler<V>::size();
	}

	static void extract(std::size_t pos, std::pair<K, V>& obj, const std::pair<K, V>& defVal, AbstractExtractor::Ptr pExt)
	{
		TypeHandler<K>::extract(pos, obj.first, defVal.first, pExt);
		pos += TypeHandler<K>::size();
		TypeHandler<V>::extract(pos, obj.second, defVal.second, pExt);
	}

	static void prepare(std::size_t pos, const std::pair<K, V>& obj, AbstractPreparator::Ptr pPreparator)
	{
		TypeHandler<K>::prepare(pos, obj.first, pPreparator);
		pos += TypeHandler<K>::size();
		TypeHandler<V>::prepare(pos, obj.second, pPreparator);
	}
};


template <typename T>
class TypeHandler<Poco::AutoPtr<T>>: public AbstractTypeHandler
	/// Specialization of type handler for Poco::AutoPtr
{
public:
	static void bind(std::size_t pos, const Poco::AutoPtr<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		// *obj will trigger a nullpointer exception if empty: this is on purpose
		TypeHandler<T>::bind(pos, *obj, pBinder, dir);
	}

	static std::size_t size()
	{
		return TypeHandler<T>::size();
	}

	static void extract(std::size_t pos, Poco::AutoPtr<T>& obj, const Poco::AutoPtr<T>& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());

		obj = Poco::AutoPtr<T>(new T());
		if (defVal)
			TypeHandler<T>::extract(pos, *obj, *defVal, pExt);
		else
			TypeHandler<T>::extract(pos, *obj, *obj, pExt);
	}

	static void prepare(std::size_t pos, const Poco::AutoPtr<T>&, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		TypeHandler<T>::prepare(pos, T(), pPreparator);
	}
};



template <typename T>
class TypeHandler<Poco::SharedPtr<T>>: public AbstractTypeHandler
	/// Specialization of type handler for Poco::SharedPtr
{
public:
	static void bind(std::size_t pos, const Poco::SharedPtr<T>& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		// *obj will trigger a nullpointer exception if empty
		TypeHandler<T>::bind(pos, *obj, pBinder, dir);
	}

	static std::size_t size()
	{
		return TypeHandler<T>::size();
	}

	static void extract(std::size_t pos, Poco::SharedPtr<T>& obj, const Poco::SharedPtr<T>& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());

		obj = Poco::SharedPtr<T>(new T());
		if (defVal)
			TypeHandler<T>::extract(pos, *obj, *defVal, pExt);
		else
			TypeHandler<T>::extract(pos, *obj, *obj, pExt);
	}

	static void prepare(std::size_t pos, const Poco::SharedPtr<T>&, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg (!pPreparator.isNull());
		TypeHandler<T>::prepare(pos, T(), pPreparator);
	}
};


} } // namespace Poco::Data

#if defined(POCO_COMPILER_GCC) && (__GNUC__ >= 12)
	#pragma GCC diagnostic pop
#elif defined (POCO_COMPILER_MSVC)
#pragma warning(pop)
#endif

#endif // Data_TypeHandler_INCLUDED
