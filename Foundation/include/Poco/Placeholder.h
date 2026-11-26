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
#include "Poco/ValueTypeId.h"
#include <cstddef>
#include <new>
#include <string>
#include <type_traits>
#include <utility>


namespace Poco {


#ifndef POCO_DOC
template <class T, std::size_t S>
struct TypeSizeLE:
	std::integral_constant<bool, (sizeof(T) <= S)>{};
#endif


namespace Impl {


template <typename V>
inline constexpr bool IS_LEAF_VALUE = std::is_trivially_copyable_v<V>;
	/// True for a value type that cannot own a Poco::Any or a
	/// Poco::Dynamic::Var, so that neither can live inside such a value.


template <typename C, typename T, typename A>
inline constexpr bool IS_LEAF_VALUE<std::basic_string<C, T, A>> = true;


template <typename V>
inline constexpr bool IS_STRING = false;
	/// True for a std::basic_string.


template <typename C, typename T, typename A>
inline constexpr bool IS_STRING<std::basic_string<C, T, A>> = true;


} // namespace Impl


template <typename PlaceholderT, unsigned int SizeV = POCO_SMALL_OBJECT_SIZE>
union Placeholder
	/// ValueHolder union (used by Poco::Any and Poco::Dynamic::Var for small
	/// object optimization, when enabled).
	///
	/// If Holder<Type> fits into POCO_SMALL_OBJECT_SIZE bytes of storage,
	/// needs no stronger alignment than std::max_align_t and can be moved
	/// without an exception, it will be placement-new-allocated into the local buffer
	/// (i.e. there will be no heap-allocation). The local buffer size is two bytes
	/// larger - [POCO_SMALL_OBJECT_SIZE + 2]: the first additional byte indicates
	/// where the object was allocated (see enum Allocation), the second one holds
	/// the type id of the content (see typeId()).
	///
	/// Important: for SOO builds, swap() exchanges only contents that are not
	/// stored locally; moveFrom() also moves a locally stored content.
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
		setAllocation(Allocation::POCO_ANY_EMPTY);
	}

	~Placeholder()
	{
		destruct();
	}

	void swap(Placeholder& other) noexcept
		/// Exchanges the contents, which must not be stored locally.
	{
		poco_assert_dbg (!isLocal() && !other.isLocal());
		std::swap(pHolder, other.pHolder);
		std::swap(holder[TAG_BYTE], other.holder[TAG_BYTE]);
		std::swap(holder[TYPE_ID_BYTE], other.holder[TYPE_ID_BYTE]);
	}

	void erase()
	{
		destruct();
	}

	[[nodiscard]] bool isEmpty() const
	{
		return allocation() == Allocation::POCO_ANY_EMPTY;
	}

	[[nodiscard]] bool isLocal() const
	{
		return allocation() == Allocation::POCO_ANY_LOCAL;
	}

	[[nodiscard]] bool holdsLeaf() const noexcept
		/// Returns true if the placeholder is empty or holds a value for
		/// which Impl::IS_LEAF_VALUE is true, so that no Any or Var can
		/// live inside the content. Without small object optimization
		/// (POCO_NO_SOO), it returns true only for an empty placeholder.
	{
		return isEmpty() || (holder[TAG_BYTE] & LEAF_FLAG) != 0;
	}

	[[nodiscard]] bool hasNothrowClone() const noexcept
		/// Returns true if the placeholder is empty or the content is stored
		/// locally and its value is nothrow copy constructible, so that
		/// cloning the content cannot throw. Without small object
		/// optimization (POCO_NO_SOO), it returns true only for an empty
		/// placeholder.
	{
		return isEmpty() || (holder[TAG_BYTE] & NOTHROW_CLONE_FLAG) != 0;
	}

	[[nodiscard]] bool canTakeCloneOf(const Placeholder& source) const noexcept
		/// Returns true if the clone of the content of source can replace
		/// the content directly: holdsLeaf() && source.hasNothrowClone().
	{
		return holdsLeaf() && source.hasNothrowClone();
	}

	[[nodiscard]] unsigned char typeId() const noexcept
		/// Returns the type id that emplace() stored with the content (see
		/// holderTypeId()), or Impl::TYPE_ID_OTHER if the placeholder is empty.
		/// Without small object optimization (POCO_NO_SOO), it always returns
		/// Impl::TYPE_ID_OTHER.
	{
		return holder[TYPE_ID_BYTE];
	}

	[[nodiscard]] bool holdsSameTypeAs(const Placeholder& other) const
		/// Returns true if both contents have the same type id other than
		/// Impl::TYPE_ID_OTHER, or if both have the type id Impl::TYPE_ID_OTHER,
		/// neither placeholder is empty and both contents have the same type().
	{
		if (typeId() != Impl::TYPE_ID_OTHER)
			return typeId() == other.typeId();
		return other.typeId() == Impl::TYPE_ID_OTHER && !isEmpty() && !other.isEmpty() && content()->type() == other.content()->type();
	}

	template <typename T, typename V>
	[[nodiscard]] static constexpr unsigned char holderTypeId() noexcept
		/// Returns the type id that emplace<T, V>() stores: T::TYPE_ID if the
		/// holder type T declares "static constexpr unsigned char TYPE_ID" and
		/// "using TypeIdHolder = T;", otherwise the id of V in PlaceholderT::ValueTypes
		/// (see Impl::TYPE_ID_IN), or Impl::TYPE_ID_OTHER if PlaceholderT declares
		/// no such TypeList. The alias names T itself, so that the TYPE_ID of such a
		/// holder is not used for a class derived from it. Without small object
		/// optimization (POCO_NO_SOO), it always returns Impl::TYPE_ID_OTHER.
	{
		if constexpr (requires { T::TYPE_ID; requires std::is_same_v<typename T::TypeIdHolder, T>; })
			return T::TYPE_ID;
		else if constexpr (requires { typename PlaceholderT::ValueTypes; })
			return Impl::TYPE_ID_IN<V, typename PlaceholderT::ValueTypes, Impl::FOUNDATION_TYPE_ID_BASE>;
		else
			return Impl::TYPE_ID_OTHER;
	}

	template <typename T, typename V>
	PlaceholderT* assign(const V& value)
		/// Destroys the content and stores a holder of type T
		/// that holds a copy of value.
	{
		return emplace<T, V>(value);
	}

	template <typename T, typename V, typename... Args>
	PlaceholderT* emplace(Args&&... args)
		/// Destroys the content and stores a holder of type T for
		/// a value of type V, constructed from args. The holder is
		/// stored locally if storesLocally<T, V>() is true, otherwise
		/// on the heap. If the construction throws, the placeholder is empty.
	{
		erase();
		if constexpr (storesLocally<T, V>())
		{
			PlaceholderT* pContent = new (holder) T(std::forward<Args>(args)...);
			setAllocation(Allocation::POCO_ANY_LOCAL, leafFlag<V>() | nothrowCloneFlag<V>() | trivialDestructionFlag<T, V>(), holderTypeId<T, V>());
			return pContent;
		}
		else
		{
			pHolder = new T(std::forward<Args>(args)...);
			setAllocation(Allocation::POCO_ANY_EXTERNAL, leafFlag<V>(), holderTypeId<T, V>());
			return pHolder;
		}
	}

	void moveFrom(Placeholder& other) noexcept
		/// Destroys the content and takes over the content of other,
		/// which is left empty. A locally stored content is moved
		/// with PlaceholderT::moveTo(), a heap-allocated one by its pointer.
	{
		poco_assert_dbg (this != &other);
		erase();
		if (other.isLocal())
		{
			other.content()->moveTo(this);
			other.erase();
		}
		else swap(other);
	}

	[[nodiscard]] PlaceholderT* content() const
	{
		if (isLocal())
			return reinterpret_cast<PlaceholderT*>(holder);
		else if (isEmpty())
			return nullptr;
		else
			return pHolder;
	}

private:
	template <typename T, typename V>
	[[nodiscard]] static constexpr bool storesLocally()
		/// Returns true if a holder of type T for a value of type V is stored
		/// in the local buffer: it fits into the buffer, needs no stronger
		/// alignment than std::max_align_t, and moving it cannot throw, because
		/// V is nothrow copy constructible or T declares moveTo() noexcept.
		/// Any other holder is stored on the heap, where a move takes the pointer.
	{
		return TypeSizeLE<T, SizeV>::value && alignof(T) <= alignof(AlignerType) &&
			(std::is_nothrow_copy_constructible_v<V> ||
				requires (T& holderRef, Placeholder* pPlaceholder) { { holderRef.moveTo(pPlaceholder) } noexcept; });
	}

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

	static constexpr unsigned int TAG_BYTE = SizeV;
		/// The index of the tag byte, which holds the Allocation and the flags,
		/// behind the local buffer.

	static constexpr unsigned int TYPE_ID_BYTE = SizeV + 1;
		/// The index of the byte that holds the type id of the content.

	static constexpr unsigned char LEAF_FLAG = 0x80;
		/// Set in the tag byte, next to the Allocation, if Impl::IS_LEAF_VALUE
		/// is true for the value.

	static constexpr unsigned char NOTHROW_CLONE_FLAG = 0x40;
		/// Set in the tag byte, next to the Allocation, if the content is
		/// stored locally and the value is nothrow copy constructible.

	static constexpr unsigned char TRIVIAL_DESTRUCTION_FLAG = 0x20;
		/// Set in the tag byte, next to the Allocation, if the content is
		/// stored locally and its destructor has no effect (see trivialDestructionFlag()).

	static constexpr unsigned char ALLOCATION_MASK = 0x1F;
		/// Selects the Allocation in the tag byte.

	static_assert((ALLOCATION_MASK & (LEAF_FLAG | NOTHROW_CLONE_FLAG | TRIVIAL_DESTRUCTION_FLAG)) == 0 &&
		(Allocation::POCO_ANY_EXTERNAL & ~ALLOCATION_MASK) == 0, "the flags and the Allocation overlap in the tag byte");

	template <typename V>
	[[nodiscard]] static constexpr unsigned char leafFlag()
	{
		return Impl::IS_LEAF_VALUE<V> ? LEAF_FLAG : 0;
	}

	template <typename V>
	[[nodiscard]] static constexpr unsigned char nothrowCloneFlag()
	{
		return std::is_nothrow_copy_constructible_v<V> ? NOTHROW_CLONE_FLAG : 0;
	}

	template <typename T, typename V>
	[[nodiscard]] static constexpr unsigned char trivialDestructionFlag()
		/// A holder type T declares "using DestroysOnlyValue = T;" if its
		/// destructor does nothing but destroy the value. If V is also trivially
		/// destructible, the destructor has no effect and is not called: the
		/// storage is reused without it. The alias names T itself, so that a
		/// class derived from such a holder does not inherit the property.
	{
		if constexpr (requires { typename T::DestroysOnlyValue; })
			return (std::is_same_v<typename T::DestroysOnlyValue, T> && std::is_trivially_destructible_v<V>) ? TRIVIAL_DESTRUCTION_FLAG : 0;
		else
			return 0;
	}

	[[nodiscard]] Allocation allocation() const
	{
		return static_cast<Allocation>(holder[TAG_BYTE] & ALLOCATION_MASK);
	}

	void setAllocation(Allocation alloc, unsigned char flags = 0, unsigned char typeId = Impl::TYPE_ID_OTHER) const
	{
		holder[TAG_BYTE] = static_cast<unsigned char>(alloc | flags);
		holder[TYPE_ID_BYTE] = typeId;
	}

	void destruct()
	{
		switch (allocation())
		{
		case Allocation::POCO_ANY_EMPTY:
			break;
		case Allocation::POCO_ANY_LOCAL:
			if ((holder[TAG_BYTE] & TRIVIAL_DESTRUCTION_FLAG) == 0)
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
	}

	mutable unsigned char holder[TYPE_ID_BYTE + 1];
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

	[[nodiscard]] bool holdsLeaf() const noexcept
	{
		return isEmpty();
	}

	[[nodiscard]] bool hasNothrowClone() const noexcept
	{
		return isEmpty();
	}

	[[nodiscard]] bool canTakeCloneOf(const Placeholder& source) const noexcept
	{
		return holdsLeaf() && source.hasNothrowClone();
	}

	[[nodiscard]] unsigned char typeId() const noexcept
	{
		return Impl::TYPE_ID_OTHER;
	}

	[[nodiscard]] bool holdsSameTypeAs(const Placeholder& other) const
	{
		return !isEmpty() && !other.isEmpty() && pHolder->type() == other.pHolder->type();
	}

	template <typename T, typename V>
	[[nodiscard]] static constexpr unsigned char holderTypeId() noexcept
	{
		return Impl::TYPE_ID_OTHER;
	}

	template <typename T, typename V>
	PlaceholderT* assign(const V& value)
	{
		return emplace<T, V>(value);
	}

	template <typename T, typename V, typename... Args>
	PlaceholderT* emplace(Args&&... args)
	{
		erase();
		pHolder = new T(std::forward<Args>(args)...);
		return pHolder;
	}

	void moveFrom(Placeholder& other) noexcept
	{
		poco_assert_dbg (this != &other);
		erase();
		swap(other);
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
