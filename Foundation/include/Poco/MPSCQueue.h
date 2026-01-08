//
// MPSCQueue.h
//
// Library: Foundation
// Package: Core
// Module:  MPSCQueue
//
// Definition of the MPSCQueue class template.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MPSCQueue_INCLUDED
#define Foundation_MPSCQueue_INCLUDED


#include "Poco/Foundation.h"
#include <atomic>
#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>


namespace Poco {


template <typename T>
class MPSCQueue
	/// A lock-free multi-producer single-consumer (MPSC) bounded queue.
	///
	/// This queue is optimized for the case where multiple threads
	/// produce items and exactly one thread consumes them.
	///
	/// The queue uses a bounded ring buffer with atomic operations.
	/// Producers use compare-and-swap to reserve slots; the consumer
	/// reads committed entries in order.
	///
	/// The queue has a fixed capacity specified at construction time.
	/// When full, tryPush() returns false. When empty, tryPop() returns false.
	///
	/// Usage:
	///     MPSCQueue<Message> queue(1024);
	///
	///     // Producer threads (any number):
	///     if (queue.tryPush(msg)) { /* success */ }
	///
	///     // Consumer thread (exactly one):
	///     Message msg;
	///     if (queue.tryPop(msg)) { /* got message */ }
	///
	/// Thread safety:
	///   - Multiple threads may call tryPush/emplace concurrently (producers)
	///   - Exactly one thread may call tryPop (consumer)
	///   - size(), empty(), capacity() may be called from any thread
	///
	/// Note: This queue does not provide blocking operations. For blocking
	/// behavior, combine with a semaphore or condition variable.
{
public:
#ifdef __cpp_lib_hardware_interference_size
	static constexpr std::size_t CACHE_LINE_SIZE = std::hardware_destructive_interference_size;
#else
	static constexpr std::size_t CACHE_LINE_SIZE = 64;
#endif

	explicit MPSCQueue(std::size_t capacity):
		/// Creates the queue with the given capacity.
		/// Capacity must be at least 1 and will be rounded up
		/// to the next power of two for efficient indexing.
		_capacity(nextPowerOfTwo(capacity)),
		_mask(_capacity - 1),
		_slots(static_cast<Slot*>(::operator new(_capacity * sizeof(Slot))))
	{
		// Initialize sequence numbers
		for (std::size_t i = 0; i < _capacity; ++i)
		{
			new (&_slots[i].sequence) std::atomic<std::size_t>(i);
		}
	}

	~MPSCQueue()
		/// Destroys the queue.
	{
		// Destroy any remaining items
		std::size_t tail = _tail.load(std::memory_order_relaxed);
		std::size_t head = _head.load(std::memory_order_relaxed);
		while (tail != head)
		{
			std::size_t idx = index(tail);
			_slots[idx].ptr()->~T();
			++tail;
		}
		// Destroy sequence atomics and free memory
		for (std::size_t i = 0; i < _capacity; ++i)
		{
			_slots[i].sequence.~atomic();
		}
		::operator delete(_slots);
	}

	MPSCQueue(const MPSCQueue&) = delete;
	MPSCQueue& operator=(const MPSCQueue&) = delete;
	MPSCQueue(MPSCQueue&&) = delete;
	MPSCQueue& operator=(MPSCQueue&&) = delete;

	bool tryPush(const T& item)
		/// Attempts to push a copy of item onto the queue.
		/// Returns true if successful, false if the queue is full.
		/// Thread-safe for multiple concurrent producers.
	{
		return emplace(item);
	}

	bool tryPush(T&& item)
		/// Attempts to push item onto the queue using move semantics.
		/// Returns true if successful, false if the queue is full.
		/// Thread-safe for multiple concurrent producers.
	{
		return emplace(std::move(item));
	}

	template <typename... Args>
	bool emplace(Args&&... args)
		/// Attempts to construct an item in-place at the back of the queue.
		/// Returns true if successful, false if the queue is full.
		/// Thread-safe for multiple concurrent producers.
	{
		std::size_t head = _head.load(std::memory_order_relaxed);

		for (;;)
		{
			Slot& slot = _slots[index(head)];
			std::size_t seq = slot.sequence.load(std::memory_order_acquire);
			std::intptr_t diff = static_cast<std::intptr_t>(seq) - static_cast<std::intptr_t>(head);

			if (diff == 0)
			{
				// Slot is available for writing
				if (_head.compare_exchange_weak(head, head + 1, std::memory_order_relaxed))
				{
					// We claimed this slot, construct the item
					new (slot.ptr()) T(std::forward<Args>(args)...);
					// Publish: allow consumer to read this slot
					slot.sequence.store(head + 1, std::memory_order_release);
					return true;
				}
				// CAS failed, another producer claimed it, retry with updated head
			}
			else if (diff < 0)
			{
				// Queue is full (head has wrapped around and caught up with tail)
				return false;
			}
			else
			{
				// Another producer is writing to this slot, reload head and retry
				head = _head.load(std::memory_order_relaxed);
			}
		}
	}

	bool tryPop(T& item)
		/// Attempts to pop an item from the queue.
		/// If successful, moves the item into the provided reference
		/// and returns true. Returns false if the queue is empty.
		/// Must be called from exactly one thread (the consumer).
	{
		std::size_t tail = _tail.load(std::memory_order_relaxed);
		Slot& slot = _slots[index(tail)];
		std::size_t seq = slot.sequence.load(std::memory_order_acquire);
		std::intptr_t diff = static_cast<std::intptr_t>(seq) - static_cast<std::intptr_t>(tail + 1);

		if (diff == 0)
		{
			// Slot has been written and is ready to read
			item = std::move(*slot.ptr());
			slot.ptr()->~T();
			// Mark slot as available for producers (sequence = tail + capacity)
			slot.sequence.store(tail + _capacity, std::memory_order_release);
			_tail.store(tail + 1, std::memory_order_relaxed);
			return true;
		}
		else if (diff < 0)
		{
			// Queue is empty or producer hasn't finished writing yet
			return false;
		}

		// This shouldn't happen with a single consumer
		return false;
	}

	[[nodiscard]]
	std::size_t size() const
		/// Returns an approximate count of items in the queue.
		/// This is approximate because producers may be modifying
		/// the queue concurrently.
	{
		std::size_t head = _head.load(std::memory_order_acquire);
		std::size_t tail = _tail.load(std::memory_order_acquire);
		return head - tail;
	}

	[[nodiscard]]
	bool empty() const
		/// Returns true if the queue appears to be empty.
		/// This is approximate for the same reason as size().
	{
		return size() == 0;
	}

	[[nodiscard]]
	std::size_t capacity() const
		/// Returns the capacity of the queue.
	{
		return _capacity;
	}

private:
	struct Slot
	{
		std::atomic<std::size_t> sequence;
		typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;

		[[nodiscard]]
		T* ptr() { return reinterpret_cast<T*>(&storage); }
		[[nodiscard]]
		const T* ptr() const { return reinterpret_cast<const T*>(&storage); }
	};

	[[nodiscard]]
	static std::size_t nextPowerOfTwo(std::size_t n)
	{
		if (n == 0) return 1;
		--n;
		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		if constexpr (sizeof(std::size_t) > 4)
		{
			n |= n >> 32;
		}
		return n + 1;
	}

	[[nodiscard]]
	std::size_t index(std::size_t pos) const
	{
		return pos & _mask;
	}

	const std::size_t _capacity;
	const std::size_t _mask;
	Slot* const _slots;

	// Align head and tail to separate cache lines to avoid false sharing
	alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> _head{0}; // written by producers (CAS)
	alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> _tail{0}; // written by consumer only
};


} // namespace Poco


#endif // Foundation_MPSCQueue_INCLUDED
