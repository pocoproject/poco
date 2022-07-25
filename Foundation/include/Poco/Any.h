//
// Any.h
//
// Library: Foundation
// Package: Core
// Module:	Any
//
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
// Extracted from Boost 1.33.1 lib and adapted for poco: Peter Schojer/AppliedInformatics 2006-02-02
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Any_INCLUDED
#define Foundation_Any_INCLUDED


#include "Poco/Exception.h"
#include "Poco/MetaProgramming.h"
#include <algorithm>
#include <typeinfo>
#include <cstring>


#define poco_any_assert(cond) do { if (!(cond)) std::abort(); } while (0)


namespace Poco {

class Any;

namespace Dynamic {

class Var;
class VarHolder;
template <class T> class VarHolderImpl;

}


template <class T, std::size_t S>
struct TypeSizeLE:
	std::integral_constant<bool, (sizeof(T) <= S)>{};


template <class T, std::size_t S>
struct TypeSizeGT:
	std::integral_constant<bool, (sizeof(T) > S)>{};


template <typename PlaceholderT, unsigned int SizeV = POCO_SMALL_OBJECT_SIZE>
union Placeholder
	/// ValueHolder union (used by Poco::Any and Poco::Dynamic::Var for small
	/// object optimization, when enabled).
	///
	/// If Holder<Type> fits into POCO_SMALL_OBJECT_SIZE bytes of storage,
	/// it will be placement-new-allocated into the local buffer
	/// (i.e. there will be no heap-allocation). The local buffer size is one byte
	/// larger - [POCO_SMALL_OBJECT_SIZE + 1], additional byte value indicating
	/// where the object was allocated (0 => heap, 1 => local).
	///
	/// Important: for SOO builds, only same-type (or trivial both-empty no-op)
	/// swap operation is allowed.
{
public:
	struct Size
	{
		enum { value = SizeV };
	};

	Placeholder(const Placeholder&) = delete;
	Placeholder(Placeholder&&) = delete;
	Placeholder& operator=(const Placeholder&) = delete;
	Placeholder& operator=(Placeholder&&) = delete;

#ifndef POCO_NO_SOO

	Placeholder(): pHolder(0)
	{
		std::memset(holder, 0, sizeof(Placeholder));
	}

	~Placeholder()
	{
		destruct(false);
	}

	void swap(Placeholder& other) noexcept
	{
		if (!isEmpty() || !other.isEmpty())
			std::swap(holder, other.holder);
	}

	void erase()
	{
		destruct(true);
	}

	bool isEmpty() const
	{
		static char buf[SizeV+1] = {};
		return 0 == std::memcmp(holder, buf, SizeV+1);
	}

	bool isLocal() const
	{
		return holder[SizeV] != 0;
	}

	template<typename T, typename V,
		typename std::enable_if<TypeSizeLE<T, Placeholder::Size::value>::value>::type* = nullptr>
	PlaceholderT* assign(const V& value)
	{
		erase();
		new (reinterpret_cast<PlaceholderT*>(holder)) T(value);
		setLocal(true);
		return reinterpret_cast<PlaceholderT*>(holder);
	}

	template<typename T, typename V,
		typename std::enable_if<TypeSizeGT<T, Placeholder::Size::value>::value>::type* = nullptr>
	PlaceholderT* assign(const V& value)
	{
		erase();
		pHolder = new T(value);
		setLocal(false);
		return pHolder;
	}

	PlaceholderT* content() const
	{
		if (isLocal())
			return reinterpret_cast<PlaceholderT*>(holder);
		else
			return pHolder;
	}

private:
	typedef typename std::aligned_storage<SizeV+1>::type AlignerType;

	void setLocal(bool local) const
	{
		holder[SizeV] = local ? 1 : 0;
	}

	void destruct(bool clear)
	{
		if (!isEmpty())
		{
			if (!isLocal())
				delete pHolder;
			else
				reinterpret_cast<PlaceholderT*>(holder)->~PlaceholderT();

			if (clear) std::memset(holder, 0, sizeof(Placeholder));
		}
	}

	mutable unsigned char holder[SizeV+1];
	AlignerType           aligner;

#else // POCO_NO_SOO

	Placeholder(): pHolder(0)
	{
	}

	~Placeholder()
	{
		delete pHolder;
	}

	void swap(Placeholder& other) noexcept
	{
		std::swap(pHolder, other.pHolder);
	}

	void erase()
	{
		delete pHolder;
		pHolder = 0;
	}

	bool isEmpty() const
	{
		return 0 == pHolder;
	}

	bool isLocal() const
	{
		return false;
	}

	template <typename T, typename V>
	PlaceholderT* assign(const V& value)
	{
		erase();
		return pHolder = new T(value);
	}

	PlaceholderT* content() const
	{
		return pHolder;
	}

private:
#endif // POCO_NO_SOO
	PlaceholderT* pHolder;
};


class Any
	/// Any class represents a general type and is capable of storing any type, supporting type-safe extraction
	/// of the internally stored data.
	///
	/// Code taken from the Boost 1.33.1 library. Original copyright by Kevlin Henney. Modified for Poco
	/// by Applied Informatics.
	///
	/// Modified for small object optimization support (optionally supported through conditional compilation)
	/// by Alex Fabijanic.
{
public:

	Any()
		/// Creates an empty any type.
	{
	}

	template<typename ValueType>
	Any(const ValueType & value)
		/// Creates an any which stores the init parameter inside.
		///
		/// Example:
		///   Any a(13);
		///   Any a(string("12345"));
	{
		construct(value);
	}

	Any(const Any& other)
		/// Copy constructor, works with both empty and initialized Any values.
	{
		if ((this != &other) && !other.empty())
			construct(other);
	}

	~Any()
		/// Destructor. If Any is locally held, calls ValueHolder destructor;
		/// otherwise, deletes the placeholder from the heap.
	{
	}

	Any& swap(Any& other) noexcept
		/// Swaps the content of the two Anys.
		///
		/// If an exception occurs during swapping, the program
		/// execution is aborted.
	{
		if (this == &other) return *this;

		if (!_valueHolder.isLocal() && !other._valueHolder.isLocal())
		{
			_valueHolder.swap(other._valueHolder);
		}
		else
		{
			try
			{
				Any tmp(*this);
				construct(other);
				other = tmp;
			}
			catch (...)
			{
				std::abort();
			}
		}

		return *this;
	}

	template<typename ValueType>
	Any& operator = (const ValueType& rhs)
		/// Assignment operator for all types != Any.
		///
		/// Example:
		///   Any a = 13;
		///   Any a = string("12345");
	{
		construct(rhs);
		return *this;
	}

	Any& operator = (const Any& rhs)
		/// Assignment operator for Any.
	{
		if ((this != &rhs) && !rhs.empty())
			construct(rhs);
		else if ((this != &rhs) && rhs.empty())
			_valueHolder.erase();

		return *this;
	}

	bool empty() const
		/// Returns true if the Any is empty.
	{
		return _valueHolder.isEmpty();
	}

	const std::type_info& type() const
		/// Returns the type information of the stored content.
		/// If the Any is empty typeid(void) is returned.
		/// It is recommended to always query an Any for its type info before
		/// trying to extract data via an AnyCast/RefAnyCast.
	{
		return empty() ? typeid(void) : content()->type();
	}

	bool local() const
		/// Returns true if data is held locally (ie. not allocated on the heap).
		/// If POCO_NO_SOO is defined, it always return false.
		/// The main purpose of this function is use for testing.
	{
		return _valueHolder.isLocal();
	}

private:
	class ValueHolder
	{
	public:
		virtual ~ValueHolder() = default;

		virtual const std::type_info & type() const = 0;
		virtual void clone(Placeholder<ValueHolder>*) const = 0;
	};

	template<typename ValueType>
	class Holder : public ValueHolder
	{
	public:
		Holder(const ValueType & value) : _held(value)
		{
		}

		virtual const std::type_info& type() const
		{
			return typeid(ValueType);
		}

		virtual void clone(Placeholder<ValueHolder>* pPlaceholder) const
		{
			pPlaceholder->assign<Holder<ValueType>, ValueType>(_held);
		}

		ValueType _held;

	private:

		Holder & operator = (const Holder &);
	};

	ValueHolder* content() const
	{
		return _valueHolder.content();
	}

	template<typename ValueType>
	void construct(const ValueType& value)
	{
		_valueHolder.assign<Holder<ValueType>, ValueType>(value);
	}

	void construct(const Any& other)
	{
		if (!other.empty())
			other.content()->clone(&_valueHolder);
		else
			_valueHolder.erase();
	}

	Placeholder<ValueHolder> _valueHolder;

	template <typename ValueType>
	friend ValueType* AnyCast(Any*);

	template <typename ValueType>
	friend ValueType* UnsafeAnyCast(Any*);

	template <typename ValueType>
	friend const ValueType& RefAnyCast(const Any&);

	template <typename ValueType>
	friend ValueType& RefAnyCast(Any&);

	template <typename ValueType>
	friend ValueType AnyCast(Any&);
};


template <typename ValueType>
ValueType* AnyCast(Any* operand)
	/// AnyCast operator used to extract the ValueType from an Any*. Will return a pointer
	/// to the stored value.
	///
	/// Example Usage:
	///	 MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
{
	return operand && operand->type() == typeid(ValueType)
				? &static_cast<Any::Holder<ValueType>*>(operand->content())->_held
				: 0;
}


template <typename ValueType>
const ValueType* AnyCast(const Any* operand)
	/// AnyCast operator used to extract a const ValueType pointer from an const Any*. Will return a const pointer
	/// to the stored value.
	///
	/// Example Usage:
	///	 const MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
{
	return AnyCast<ValueType>(const_cast<Any*>(operand));
}


template <typename ValueType>
ValueType AnyCast(Any& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an Any&.
	///
	/// Example Usage:
	///	 MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Do not use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& tmp = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
{
	typedef typename TypeWrapper<ValueType>::TYPE NonRef;

	NonRef* result = AnyCast<NonRef>(&operand);
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand.content())
		{
			s.append(1, '(');
			s.append(operand.content()->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType AnyCast(const Any& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an const Any&.
	///
	/// Example Usage:
	///	 MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Do not use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
{
	typedef typename TypeWrapper<ValueType>::TYPE NonRef;

	return AnyCast<NonRef&>(const_cast<Any&>(operand));
}


template <typename ValueType>
const ValueType& RefAnyCast(const Any & operand)
	/// AnyCast operator used to return a const reference to the internal data.
	///
	/// Example Usage:
	///	 const MyType& tmp = RefAnyCast<MyType>(anAny);
{
	ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand.content())
		{
			s.append(1, '(');
			s.append(operand.content()->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType& RefAnyCast(Any& operand)
	/// AnyCast operator used to return a reference to the internal data.
	///
	/// Example Usage:
	///	 MyType& tmp = RefAnyCast<MyType>(anAny);
{
	ValueType* result = AnyCast<ValueType>(&operand);
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand.content())
		{
			s.append(1, '(');
			s.append(operand.content()->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType* UnsafeAnyCast(Any* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at any time. They are
	/// required where we know what type is stored in the any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
{
	return &static_cast<Any::Holder<ValueType>*>(operand->content())->_held;
}


template <typename ValueType>
const ValueType* UnsafeAnyCast(const Any* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at any time. They are
	/// required where we know what type is stored in the any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
{
	return AnyCast<ValueType>(const_cast<Any*>(operand));
}


} // namespace Poco


#endif
