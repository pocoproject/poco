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
#include "Poco/Bugcheck.h"
#include "Poco/Placeholder.h"
#include <typeinfo>


namespace Poco {

class Any;

using namespace std::string_literals;

namespace Dynamic {

class Var;
class VarHolder;
template <class T> class VarHolderImpl;

}


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

	Any() = default;
		/// Creates an empty any type.

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

	Any(Any&& other)
		/// Move constructor, works with both empty and initialized Any values.
	{
		construct(other);
	}

	~Any() = default;
		/// Destructor.
		/// Small Object Optimization mode behavior:
		/// Invokes the Placeholder destructor, which calls the
		/// ValueHolder destructor explicitly (locally held Any), or
		/// deletes the ValueHolder heap memory (heap-allocated Any).

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
		if (this != &rhs)
		{
			if (!rhs.empty())
				construct(rhs);
			else
				_valueHolder.erase();
		}
		return *this;
	}

	Any& operator = (Any&& rhs)
		/// Move operator for Any.
	{
		if (!rhs.empty())
			construct(rhs);
		else
			_valueHolder.erase();
		return *this;
	}

	[[nodiscard]] bool empty() const
		/// Returns true if the Any is empty.
	{
		return _valueHolder.isEmpty();
	}

	[[nodiscard]] const std::type_info& type() const
		/// Returns the type information of the stored content.
		/// If the Any is empty typeid(void) is returned.
		/// It is recommended to always query an Any for its type info before
		/// trying to extract data via an AnyCast/RefAnyCast.
	{
		return empty() ? typeid(void) : content()->type();
	}

	[[nodiscard]] bool local() const
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

		[[nodiscard]] virtual const std::type_info & type() const = 0;
		virtual void clone(Placeholder<ValueHolder>*) const = 0;
	};

	template<typename ValueType>
	class Holder : public ValueHolder
	{
	public:
		Holder(const ValueType & value) : _held(value)
		{
		}

		Holder & operator = (const Holder &) = delete;

		[[nodiscard]] const std::type_info& type() const override
		{
			return typeid(ValueType);
		}

		void clone(Placeholder<ValueHolder>* pPlaceholder) const override
		{
			pPlaceholder->assign<Holder<ValueType>, ValueType>(_held);
		}

		ValueType _held;
	};

	[[nodiscard]] ValueHolder* content() const
	{
		return _valueHolder.content();
	}

	template <typename ValueType>
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
	friend const ValueType* AnyCast(const Any*);

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
[[nodiscard]] ValueType* AnyCast(Any* operand)
	/// AnyCast operator used to extract the ValueType from an Any*. Will return a pointer
	/// to the stored value.
	///
	/// Example Usage:
	///	 MyType* pTmp = AnyCast<MyType>(pAny).
	/// Returns nullptr if the types don't match.
{
	return operand && operand->type() == typeid(ValueType)
				? &static_cast<Any::Holder<ValueType>*>(operand->content())->_held
				: nullptr;
}


template <typename ValueType>
[[nodiscard]] const ValueType* AnyCast(const Any* operand)
	/// AnyCast operator used to extract a const ValueType pointer from an const Any*. Will return a const pointer
	/// to the stored value.
	///
	/// Example Usage:
	///	 const MyType* pTmp = AnyCast<MyType>(pAny).
	/// Returns nullptr if the types don't match.
{
	return operand && operand->type() == typeid(ValueType)
				? &static_cast<const Any::Holder<ValueType>*>(operand->content())->_held
				: nullptr;
}


template <typename ValueType>
[[nodiscard]] ValueType AnyCast(Any& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an Any&.
	///
	/// Example Usage:
	///	 MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Do not use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& tmp = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
{
	using NonRef = typename TypeWrapper<ValueType>::TYPE;

	NonRef* result = AnyCast<NonRef>(&operand);
	if (!result)
	{
		std::string s(__func__);
		s.append(": Failed to convert between Any types "s);
		if (operand.content())
		{
			s.append(1, '(');
			s.append(Poco::demangle(operand.content()->type().name()));
			s.append(" => ");
			s.append(Poco::demangle<ValueType>());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
[[nodiscard]] ValueType AnyCast(const Any& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an const Any&.
	///
	/// Example Usage:
	///	 MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Do not use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
{
	using NonRef = typename TypeWrapper<ValueType>::TYPE;

	return AnyCast<NonRef&>(const_cast<Any&>(operand));
}


template <typename ValueType>
[[nodiscard]] const ValueType& RefAnyCast(const Any & operand)
	/// AnyCast operator used to return a const reference to the internal data.
	///
	/// Example Usage:
	///	 const MyType& tmp = RefAnyCast<MyType>(anAny);
{
	ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
	if (!result)
	{
		std::string s(__func__);
		s.append(": Failed to convert between Any types "s);
		if (operand.content())
		{
			s.append(1, '(');
			s.append(Poco::demangle(operand.content()->type().name()));
			s.append(" => ");
			s.append(Poco::demangle<ValueType>());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
[[nodiscard]] ValueType& RefAnyCast(Any& operand)
	/// AnyCast operator used to return a reference to the internal data.
	///
	/// Example Usage:
	///	 MyType& tmp = RefAnyCast<MyType>(anAny);
{
	ValueType* result = AnyCast<ValueType>(&operand);
	if (!result)
	{
		std::string s(__func__);
		s.append(": Failed to convert between Any types "s);
		if (operand.content())
		{
			s.append(1, '(');
			s.append(Poco::demangle(operand.content()->type().name()));
			s.append(" => ");
			s.append(Poco::demangle<ValueType>());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
[[nodiscard]] ValueType* UnsafeAnyCast(Any* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at any time. They are
	/// required where we know what type is stored in the any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
{
	return &static_cast<Any::Holder<ValueType>*>(operand->content())->_held;
}


template <typename ValueType>
[[nodiscard]] const ValueType* UnsafeAnyCast(const Any* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at any time. They are
	/// required where we know what type is stored in the any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
{
	return AnyCast<ValueType>(const_cast<Any*>(operand));
}


template <typename ValueType>
[[nodiscard]] bool AnyHoldsNullPtr(const Any& any)
	/// Returns true if any holds a null pointer.
	/// Fails to compile if `ValueType` is not a pointer.
{
	poco_static_assert_ptr(ValueType);
	return (AnyCast<ValueType>(any) == nullptr);
}


template <typename ValueType>
[[nodiscard]] bool AnyHoldsNullPtr(const Any* pAny)
	/// Returns true if the Any pointed to holds a null pointer.
	/// Returns false if `pAny` is a null pointer.
{
	if (!pAny) return false;
	return (AnyHoldsNullPtr<ValueType>(*pAny));
}


} // namespace Poco


#endif
