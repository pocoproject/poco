//
// SPSCQueue.h
//
// Library: Foundation
// Package: Core
// Module:  SPSCQueue
//
// Definition of the SPSCQueue class template.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SPSCQueue_INCLUDED
#define Foundation_SPSCQueue_INCLUDED


#include "Poco/Foundation.h"
#include <atomic>
#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>


namespace Poco {


template <typename T>
class SPSCQueue
	/// A lock-free single-producer single-consumer (SPSC) bounded queue.
	///
	/// This queue is optimized for the case where exactly one thread
	/// produces items and exactly one thread consumes them. It provides
	/// very low latency and high throughput by avoiding locks and
	/// minimizing cache line contention.
	///
	/// The queue has a fixed capacity specified at construction time.
	/// When full, tryPush() returns false. When empty, tryPop() returns false.
	///
	/// Usage:
	///     SPSCQueue<Message> queue(1024);
	///
	///     // Producer thread:
	///     if (queue.tryPush(msg)) { /* success */ }
	///     // or with move:
	///     if (queue.tryPush(std::move(msg))) { /* success */ }
	///
	///     // Consumer thread:
	///     Message msg;
	///     if (queue.tryPop(msg)) { /* got message */ }
	///
	/// Thread safety:
	///   - Exactly one thread may call tryPush/emplace (producer)
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

	explicit SPSCQueue(std::size_t capacity):
		/// Creates the queue with the given capacity.
		/// Capacity must be at least 1 and will be rounded up
		/// to the next power of two for efficient indexing.
		_capacity(nextPowerOfTwo(capacity)),
		_mask(_capacity - 1),
		_buffer(static_cast<T*>(::operator new(_capacity * sizeof(T))))
	{
	}

	~SPSCQueue()
		/// Destroys the queue.
	{
		// Destroy any remaining items
		std::size_t head = _head.load(std::memory_order_relaxed);
		std::size_t tail = _tail.load(std::memory_order_relaxed);
		while (tail != head)
		{
			_buffer[index(tail)].~T();
			++tail;
		}
		::operator delete(_buffer);
	}

	SPSCQueue(const SPSCQueue&) = delete;
	SPSCQueue& operator=(const SPSCQueue&) = delete;
	SPSCQueue(SPSCQueue&&) = delete;
	SPSCQueue& operator=(SPSCQueue&&) = delete;

	bool tryPush(const T& item)
		/// Attempts to push a copy of item onto the queue.
		/// Returns true if successful, false if the queue is full.
	{
		return emplace(item);
	}

	bool tryPush(T&& item)
		/// Attempts to push item onto the queue using move semantics.
		/// Returns true if successful, false if the queue is full.
	{
		return emplace(std::move(item));
	}

	template <typename... Args>
	bool emplace(Args&&... args)
		/// Attempts to construct an item in-place at the back of the queue.
		/// Returns true if successful, false if the queue is full.
	{
		const std::size_t head = _head.load(std::memory_order_relaxed);
		const std::size_t nextHead = head + 1;

		// Check if queue is full using cached tail
		if (index(nextHead) == index(_cachedTail))
		{
			// Cache miss - reload tail from atomic
			_cachedTail = _tail.load(std::memory_order_acquire);
			if (index(nextHead) == index(_cachedTail))
			{
				return false; // Queue is full
			}
		}

		// Construct item in place
		new (&_buffer[index(head)]) T(std::forward<Args>(args)...);

		// Publish the new head
		_head.store(nextHead, std::memory_order_release);
		return true;
	}

	bool tryPop(T& item)
		/// Attempts to pop an item from the queue.
		/// If successful, moves the item into the provided reference
		/// and returns true. Returns false if the queue is empty.
	{
		const std::size_t tail = _tail.load(std::memory_order_relaxed);

		// Check if queue is empty using cached head
		if (index(tail) == index(_cachedHead))
		{
			// Cache miss - reload head from atomic
			_cachedHead = _head.load(std::memory_order_acquire);
			if (index(tail) == index(_cachedHead))
			{
				return false; // Queue is empty
			}
		}

		// Move item out and destroy
		item = std::move(_buffer[index(tail)]);
		_buffer[index(tail)].~T();

		// Publish the new tail
		_tail.store(tail + 1, std::memory_order_release);
		return true;
	}

	std::size_t size() const
		/// Returns an approximate count of items in the queue.
		/// This is approximate because the producer and consumer
		/// may be modifying the queue concurrently.
	{
		const std::size_t head = _head.load(std::memory_order_acquire);
		const std::size_t tail = _tail.load(std::memory_order_acquire);
		return head - tail;
	}

	bool empty() const
		/// Returns true if the queue appears to be empty.
		/// This is approximate for the same reason as size().
	{
		return size() == 0;
	}

	std::size_t capacity() const
		/// Returns the capacity of the queue.
	{
		return _capacity;
	}

private:
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

	std::size_t index(std::size_t pos) const
	{
		return pos & _mask;
	}

	const std::size_t _capacity;
	const std::size_t _mask;
	T* const _buffer;

	// Align head and tail to separate cache lines to avoid false sharing
	alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> _head{0}; // written by producer
	alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> _tail{0}; // written by consumer

	// Cached positions to reduce atomic reads
	alignas(CACHE_LINE_SIZE) std::size_t _cachedTail{0}; // cached by producer
	alignas(CACHE_LINE_SIZE) std::size_t _cachedHead{0}; // cached by consumer
};


} // namespace Poco


#endif // Foundation_SPSCQueue_INCLUDED
