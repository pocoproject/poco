//
// Placeholder.h
//
// Library: Foundation
// Package: Core
// Module:  Placeholder
//
// Definition of the Placeholder union, which stores the content of
// Poco::Any and Poco::Dynamic::Var, and of the traits it uses.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Placeholder_INCLUDED
#define Foundation_Placeholder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Bugcheck.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <new>
#include <type_traits>
#include <utility>


namespace Poco {


#ifndef POCO_DOC
template <class T, std::size_t S>
struct TypeSizeLE:
	std::integral_constant<bool, (sizeof(T) <= S)>{};


template <class T, std::size_t S>
struct TypeSizeGT:
	std::integral_constant<bool, (sizeof(T) > S)>{};
#endif


template <typename PlaceholderT, unsigned int SizeV = POCO_SMALL_OBJECT_SIZE>
union Placeholder
	/// ValueHolder union (used by Poco::Any and Poco::Dynamic::Var for small
	/// object optimization, when enabled).
	///
	/// If Holder<Type> fits into POCO_SMALL_OBJECT_SIZE bytes of storage,
	/// it will be placement-new-allocated into the local buffer
	/// (i.e. there will be no heap-allocation). The local buffer size is one byte
	/// larger - [POCO_SMALL_OBJECT_SIZE + 1], additional byte value indicating
	/// where the object was allocated. See enum Allocation.
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

	Placeholder(): pHolder(nullptr)
	{
		// Forces to use optimised memset internally
		// https://travisdowns.github.io/blog/2020/01/20/zero.html
		std::fill(std::begin(holder), std::end(holder), static_cast<char>(0));
		setAllocation(Allocation::POCO_ANY_EMPTY);
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

	[[nodiscard]] bool isEmpty() const
	{
		return holder[SizeV] == Allocation::POCO_ANY_EMPTY;
	}

	[[nodiscard]] bool isLocal() const
	{
		return holder[SizeV] == Allocation::POCO_ANY_LOCAL;
	}

	template<typename T, typename V,
		typename std::enable_if_t<TypeSizeLE<T, Placeholder::Size::value>::value>* = nullptr>
	PlaceholderT* assign(const V& value)
	{
		erase();
		new (reinterpret_cast<PlaceholderT*>(holder)) T(value);
		setAllocation(Allocation::POCO_ANY_LOCAL);
		return reinterpret_cast<PlaceholderT*>(holder);
	}

	template<typename T, typename V,
		typename std::enable_if_t<TypeSizeGT<T, Placeholder::Size::value>::value>* = nullptr>
	PlaceholderT* assign(const V& value)
	{
		erase();
		pHolder = new T(value);
		setAllocation(Allocation::POCO_ANY_EXTERNAL);
		return pHolder;
	}

	[[nodiscard]] PlaceholderT* content() const
	{
		if (isLocal())
			return reinterpret_cast<PlaceholderT*>(holder);
		else
			return pHolder;
	}

private:
	using AlignerType = std::max_align_t;
#ifndef POCO_DOC
	static_assert(
		sizeof(AlignerType) < SizeV,
		"Aligner type is bigger than the actual storage, so SizeV should be made bigger otherwise you simply waste unused memory."
	);
#endif

	enum Allocation : unsigned char
	{
		POCO_ANY_EMPTY = 0,
		POCO_ANY_LOCAL = 1,
		POCO_ANY_EXTERNAL = 2
	};

	void setAllocation(Allocation alloc) const
	{
		holder[SizeV] = alloc;
	}

	void destruct(bool clear)
	{
		const auto allocation {holder[SizeV]};
		switch (allocation)
		{
		case Allocation::POCO_ANY_EMPTY:
			break;
		case Allocation::POCO_ANY_LOCAL:
			{
				// Do not deallocate, just explicitly call destructor
				auto* h { reinterpret_cast<PlaceholderT*>(holder) };
				h->~PlaceholderT();
			}
			break;
		case Allocation::POCO_ANY_EXTERNAL:
			{
				auto* h { pHolder };
				pHolder = nullptr;
				delete h;
			}
			break;
		default:
			poco_bugcheck();
			break;
		}
		setAllocation(Allocation::POCO_ANY_EMPTY);
		if (clear)
		{
			// Force to use optimised memset internally
			std::fill(std::begin(holder), std::end(holder), static_cast<char>(0));
		}
	}

	mutable unsigned char holder[SizeV+1];
	AlignerType           aligner;

#else // POCO_NO_SOO

	Placeholder(): pHolder(nullptr)
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
		pHolder = nullptr;
	}

	[[nodiscard]] bool isEmpty() const
	{
		return nullptr == pHolder;
	}

	[[nodiscard]] bool isLocal() const
	{
		return false;
	}

	template <typename T, typename V>
	PlaceholderT* assign(const V& value)
	{
		erase();
		return pHolder = new T(value);
	}

	[[nodiscard]] PlaceholderT* content() const
	{
		return pHolder;
	}

private:
#endif // POCO_NO_SOO
	PlaceholderT* pHolder;
};


} // namespace Poco


#endif // Foundation_Placeholder_INCLUDED
