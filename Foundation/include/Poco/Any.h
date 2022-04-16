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


#ifndef POCO_NO_SOO


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
{
public:
	struct Size
	{
		static const unsigned int value = SizeV;
	};

	Placeholder(const Placeholder&) = delete;
	Placeholder(Placeholder&&) = delete;
	Placeholder& operator=(const Placeholder&) = delete;
	Placeholder& operator=(Placeholder&&) = delete;

	Placeholder(): pHolder(0)
	{
		std::memset(holder, 0, sizeof(Placeholder));
	}

	~Placeholder()
	{
		destruct(false);
	}

	void swap(Placeholder& other)
	{
		if (!isLocal() && !other.isLocal())
			std::swap(pHolder, other.pHolder);
		else
			throw Poco::InvalidAccessException("Placeholder::swap()");
	}

	void erase()
	{
		destruct(true);
	}

	bool isEmpty() const
	{
		char buf[POCO_SMALL_OBJECT_SIZE] = {};
		return 0 == std::memcmp(holder, buf, POCO_SMALL_OBJECT_SIZE);
	}

	bool isLocal() const
	{
		return holder[SizeV] != 0;
	}

	void setLocal(bool local) const
	{
		holder[SizeV] = local ? 1 : 0;
	}

	template <typename T, typename V>
	PlaceholderT* assignStack(const V& value)
	{
		erase();
		new (reinterpret_cast<PlaceholderT*>(holder)) T(value);
		setLocal(true);
		return reinterpret_cast<PlaceholderT*>(holder);
	}

	template <typename T, typename V>
	PlaceholderT* assignHeap(const V& value)
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

	PlaceholderT*         pHolder;
	mutable unsigned char holder[SizeV+1];
	AlignerType           aligner;
};


#else // !POCO_NO_SOO


template <typename PlaceholderT>
union Placeholder
	/// ValueHolder union (used by Poco::Any and Poco::Dynamic::Var for small
	/// object optimization, when enabled).
	///
	/// If Holder<Type> fits into POCO_SMALL_OBJECT_SIZE bytes of storage,
	/// it will be placement-new-allocated into the local buffer
	/// (i.e. there will be no heap-allocation). The local buffer size is one byte
	/// larger - [POCO_SMALL_OBJECT_SIZE + 1], additional byte value indicating
	/// where the object was allocated (0 => heap, 1 => local).
{
public:
	Placeholder ()
	{
	}

	PlaceholderT* content() const
	{
		return pHolder;
	}

// MSVC71,80 won't extend friendship to nested class (Any::Holder)
#if !defined(POCO_MSVC_VERSION) || (defined(POCO_MSVC_VERSION) && (POCO_MSVC_VERSION > 80))
private:
#endif

	PlaceholderT* pHolder;

	friend class Any;
	friend class Dynamic::Var;
	friend class Dynamic::VarHolder;
	template <class> friend class Dynamic::VarHolderImpl;
};


#endif // POCO_NO_SOO


class Any
	/// An Any class represents a general type and is capable of storing any type, supporting type-safe extraction
	/// of the internally stored data.
	///
	/// Code taken from the Boost 1.33.1 library. Original copyright by Kevlin Henney. Modified for Poco
	/// by Applied Informatics.
	///
	/// Modified for small object optimization support (optionally supported through conditional compilation)
	/// by Alex Fabijanic.
{
public:

#ifndef POCO_NO_SOO

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

	Any& swap(Any& other)
		/// Swaps the content of the two Anys.
		///
		/// When small object optimization is enabled, swap only
		/// has no-throw guarantee when both (*this and other)
		/// objects are allocated on the heap.
	{
		if (this == &other) return *this;

		if (!_valueHolder.isLocal() && !other._valueHolder.isLocal())
		{
			_valueHolder.swap(other._valueHolder);
		}
		else
		{
			Any tmp(*this);
			try
			{
				construct(other);
				other = tmp;
			}
			catch (...)
			{
				construct(tmp);
				throw;
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

	const std::type_info & type() const
		/// Returns the type information of the stored content.
		/// If the Any is empty typeid(void) is returned.
		/// It is recommended to always query an Any for its type info before
		/// trying to extract data via an AnyCast/RefAnyCast.
	{
		return empty() ? typeid(void) : content()->type();
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

		virtual const std::type_info & type() const
		{
			return typeid(ValueType);
		}

		virtual void clone(Placeholder<ValueHolder>* pPlaceholder) const
		{
			cloneSOO(pPlaceholder, _held);
		}

		ValueType _held;

	private:

		template<typename VT,
				 typename std::enable_if<TypeSizeLE<Holder<VT>, Placeholder<VT>::Size::value>::value>::type* = nullptr>
		static void cloneSOO(Placeholder<ValueHolder>* pPlaceholder, VT& held)
		{
			pPlaceholder->assignStack<Holder<ValueType>, ValueType>(held);
		}

		template<typename VT,
				 typename std::enable_if<TypeSizeGT<Holder<VT>, Placeholder<VT>::Size::value>::value>::type* = nullptr>
		static void cloneSOO(Placeholder<ValueHolder>* pPlaceholder, VT& held)
		{
			pPlaceholder->assignHeap<Holder<ValueType>, ValueType>(held);
		}

		Holder & operator = (const Holder &);
	};

	ValueHolder* content() const
	{
		return _valueHolder.content();
	}

	template<typename ValueType,
		 typename std::enable_if<TypeSizeLE<Holder<ValueType>, Placeholder<ValueType>::Size::value>::value>::type* = nullptr>
	void construct(const ValueType& value)
	{
		_valueHolder.assignStack<Holder<ValueType>, ValueType>(value);
	}

	template<typename ValueType,
		 typename std::enable_if<TypeSizeGT<Holder<ValueType>, Placeholder<ValueType>::Size::value>::value>::type* = nullptr>
	void construct(const ValueType& value)
	{
		_valueHolder.assignHeap<Holder<ValueType>, ValueType>(value);
	}

	void construct(const Any& other)
	{
		if (!other.empty())
			other.content()->clone(&_valueHolder);
		else
			_valueHolder.erase();
	}

	Placeholder<ValueHolder> _valueHolder;


#else // if POCO_NO_SOO


	Any(): _pHolder(0)
		/// Creates an empty any type.
	{
	}

	template <typename ValueType>
	Any(const ValueType& value):
		_pHolder(new Holder<ValueType>(value))
		/// Creates an any which stores the init parameter inside.
		///
		/// Example:
		///	 Any a(13);
		///	 Any a(string("12345"));
	{
	}

	Any(const Any& other):
		_pHolder(other._pHolder ? other._pHolder->clone() : 0)
		/// Copy constructor, works with both empty and initialized Any values.
	{
	}

	~Any()
	{
		delete _pHolder;
	}

	Any& swap(Any& rhs)
		/// Swaps the content of the two Anys.
	{
		std::swap(_pHolder, rhs._pHolder);
		return *this;
	}

	template <typename ValueType>
	Any& operator = (const ValueType& rhs)
		/// Assignment operator for all types != Any.
		///
		/// Example:
		///   Any a = 13;
		///   Any a = string("12345");
	{
		Any(rhs).swap(*this);
		return *this;
	}

	Any& operator = (const Any& rhs)
		/// Assignment operator for Any.
	{
		Any(rhs).swap(*this);
		return *this;
	}

	bool empty() const
		/// Returns true if the Any is empty.
	{
		return !_pHolder;
	}

	const std::type_info& type() const
		/// Returns the type information of the stored content.
		/// If the Any is empty typeid(void) is returned.
		/// It is recommended to always query an Any for its type info before
		/// trying to extract data via an AnyCast/RefAnyCast.
	{
		return _pHolder ? _pHolder->type() : typeid(void);
	}

private:
	class ValueHolder
	{
	public:
		virtual ~ValueHolder() = default;

		virtual const std::type_info& type() const = 0;
		virtual ValueHolder* clone() const = 0;
	};

	template <typename ValueType>
	class Holder: public ValueHolder
	{
	public:
		Holder(const ValueType& value):
			_held(value)
		{
		}

		virtual const std::type_info& type() const
		{
			return typeid(ValueType);
		}

		virtual ValueHolder* clone() const
		{
			return new Holder(_held);
		}

		ValueType _held;

	private:
		Holder & operator = (const Holder &);
	};

	ValueHolder* content() const
	{
		return _pHolder;
	}

private:
	ValueHolder* _pHolder;

#endif // POCO_NO_SOO

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
