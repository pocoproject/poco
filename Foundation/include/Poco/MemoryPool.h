//
// MemoryPool.h
//
// Library: Foundation
// Package: Core
// Module:  MemoryPool
//
// Definition of the MemoryPool class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MemoryPool_INCLUDED
#define Foundation_MemoryPool_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AtomicCounter.h"
#include "Poco/Mutex.h"
#include <vector>
#include <cstddef>


namespace Poco {


class Foundation_API MemoryPool
	/// A simple pool for fixed-size memory blocks.
	///
	/// The main purpose of this class is to speed-up
	/// memory allocations, as well as to reduce memory
	/// fragmentation in situations where the same blocks
	/// are allocated all over again, such as in server
	/// applications.
	///
	/// All allocated blocks are retained for future use.
	/// A limit on the number of blocks can be specified.
	/// Blocks can be preallocated.
{
public:
	MemoryPool(std::size_t blockSize, int preAlloc = 0, int maxAlloc = 0);
		/// Creates a MemoryPool for blocks with the given blockSize.
		/// The number of blocks given in preAlloc are preallocated.

	~MemoryPool();

	void* get();
		/// Returns a memory block. If there are no more blocks
		/// in the pool, a new block will be allocated.
		///
		/// If maxAlloc blocks are already allocated, an
		/// OutOfMemoryException is thrown.

	void release(void* ptr);
		/// Releases a memory block and returns it to the pool.

	std::size_t blockSize() const;
		/// Returns the block size.

	int allocated() const;
		/// Returns the number of allocated blocks.

	int available() const;
		/// Returns the number of available blocks in the pool.

private:
	MemoryPool();
	MemoryPool(const MemoryPool&);
	MemoryPool& operator = (const MemoryPool&);

	void clear();

	enum
	{
		BLOCK_RESERVE = 128
	};

	typedef std::vector<char*> BlockVec;

	std::size_t _blockSize;
	int         _maxAlloc;
	int         _allocated;
	BlockVec    _blocks;
	FastMutex   _mutex;
};


//
// FastMemoryPool
//

// Macro defining the default initial size of any
// FastMemoryPool; can be overridden by specifying
// FastMemoryPool pre-alloc at runtime.
#define POCO_FAST_MEMORY_POOL_PREALLOC 1000


template <typename T, typename M = FastMutex>
class FastMemoryPool
	/// FastMemoryPool is a class for pooling fixed-size blocks of memory.
	///
	/// The main purpose of this class is to speed-up memory allocations,
	/// as well as to reduce memory fragmentation in situations where the
	/// same blocks are allocated all over again, such as in server
	/// applications. It differs from the MemoryPool in the way the block
	/// size is determined - it is inferred form the held type size and
	/// applied statically. It is also, as its name implies, faster than
	/// Poco::MemoryPool. It is likely to be significantly faster than
	/// the runtime platform generic memory allocation functionality
	/// as well, but it has certain limitations (aside from only giving
	/// blocks of fixed size) - see more below.
	///
	/// An object using memory from the pool should be created using
	/// in-place new operator; once released back to the pool, its
	/// destructor will be called by the pool. The returned pointer
	/// must be a valid pointer to the type for which it was obtained.
	///
	/// Example use:
	///
	///   using std::vector;
	///   using std:string;
	///   using std::to_string;
	///   using Poco::FastMemoryPool;
	///
	///   int blocks = 10;
	///   FastMemoryPool<int> fastIntPool(blocks);
	///   FastMemoryPool<string> fastStringPool(blocks);
	///
	///   vector<int*> intVec(blocks, 0);
	///   vector<string*> strVec(blocks);
	///
	///   for (int i = 0; i < blocks; ++i)
	///   {
	///     intVec[i] = new (fastIntPool.get()) int(i);
	///     strVec[i] = new (fastStringPool.get()) string(to_string(i));
	///   }
	///
	///   for (int i = 0; i < blocks; ++i)
	///   {
	///     fastIntPool.release(intVec[i]);
	///     fastStringPool.release(strVec[i]);
	///   }
	///
	/// Pool keeps memory blocks in "buckets". A bucket is an array of
	/// blocks; it is always allocated with a single `new[]`, and its blocks
	/// are initialized at creation time. Whenever the current capacity
	/// of the pool is reached, a new bucket is allocated and its blocks
	/// initialized for internal use. If the new bucket allocation would
	/// exceed allowed maximum size, std::bad_alloc() exception is thrown,
	/// with object itself left intact.
	///
	/// Pool internally keeps track of available blocks through a linked-list
	/// and utilizes unused memory blocks for that purpose. This means that,
	/// for types smaller than pointer the size of a block will be greater
	/// than the size of the type. The implications are following:
	///
	///   - FastMemoryPool can not be used for arrays of types smaller
	///     than pointer
	///
	///   - if FastMemoryPool is used to store variable-size arrays, it
	///     must not have multiple buckets; the way to achieve this is by
	///     specifying proper argument values at construction.
	///
	/// Neither of the above are primarily intended or recommended modes
	/// of use. It is recommended to use a FastMemoryPool for creation of
	/// many objects of the same type. Furthermore, it is perfectly fine
	/// to have arrays or STL containers of pointers to objects created
	/// in blocks of memory obtained from the FastMemoryPool.
	///
	/// Before a block is given to the user, it is removed from the list;
	/// when a block is returned to the pool, it is re-inserted in the
	/// list. Pool will return held memory to the system at destruction,
	/// and will not leak memory after destruction; this means that after
	/// pool destruction, any memory that was taken, but not returned to
	/// it becomes invalid.
	///
	/// FastMemoryPool is thread safe; it uses Poco::FastMutex by
	/// default, but other mutexes can be specified through the template
	/// parameter, if needed. Poco::NullMutex can be specified as template
	/// parameter to avoid locking and improve speed in single-threaded
	/// scenarios.
{
private:
	class Block
		/// A block of memory. This class represents a memory
		/// block. It has dual use, the primary one being
		/// obvious - memory provided to the user of the pool.
		/// The secondary use is for internal "housekeeping"
		/// purposes.
		///
		/// It works like this:
		///
		///    - when initially created, a Block is properly
		///      constructed and positioned into the internal
		///      linked list of blocks
		///
		///    - when given to the user, the Block is removed
		///      from the internal linked list of blocks
		///
		///    - when returned back to the pool, the Block
		///      is again in-place constructed and inserted
		///      as next available block in the linked list
		///      of blocks
	{
	public:

		Block()
			/// Creates a Block and sets its next pointer.
			/// This constructor should ony be used to initialize
			/// a block sequence (an array of blocks) in a newly
			/// allocated bucket.
			///
			/// After the construction, the last block's `next`
			/// pointer points outside the allocated memory and
			/// must be set to zero. This design improves performance,
			/// because otherwise the block array would require an
			/// initialization loop after the allocation.
		{
			_memory.next = this + 1;
		}

		explicit Block(Block* next)
			/// Creates a Block and sets its next pointer.
		{
			_memory.next = next;
		}

#ifndef POCO_DOC
		union
			/// Memory block storage.
			///
			/// Note that this storage is properly aligned
			/// for the datatypes it holds. It will not work
			/// for arrays of types smaller than pointer size.
			/// Furthermore, the pool  itself will not work for
			/// a variable-size array of any type after it is
			/// resized.
		{
			char buffer[sizeof(T)];
			Block* next;
		} _memory;
#endif

	private:
		Block(const Block&);
		Block& operator = (const Block&);
	};

public:
	typedef M MutexType;
	typedef typename M::ScopedLock ScopedLock;

	typedef Block* Bucket;
	typedef std::vector<Bucket> BucketVec;

	FastMemoryPool(std::size_t blocksPerBucket = POCO_FAST_MEMORY_POOL_PREALLOC, std::size_t bucketPreAlloc = 10, std::size_t maxAlloc = 0):
			_blocksPerBucket(blocksPerBucket),
			_maxAlloc(maxAlloc),
			_available(0)
		/// Creates the FastMemoryPool.
		///
		/// The size of a block is inferred from the type size. Number of blocks
		/// per bucket, pre-allocated bucket pointer storage and maximum allowed
		/// total size of the pool can be customized by overriding default
		/// parameter value:
		///
		///   - blocksPerBucket specifies how many blocks each bucket contains
		///                     defaults to POCO_FAST_MEMORY_POOL_PREALLOC
		///
		///   - bucketPreAlloc specifies how much space for bucket pointers
		///                    (buckets themselves are not pre-allocated) will
		///                    be pre-alocated.
		///
		///   - maxAlloc specifies maximum allowed total pool size in bytes.
	{
		if (_blocksPerBucket < 2)
			throw std::invalid_argument("FastMemoryPool: blocksPerBucket must be >=2");
		_buckets.reserve(bucketPreAlloc);
		resize();
	}

	~FastMemoryPool()
		/// Destroys the FastMemoryPool and releases all memory.
		/// Any memory taken from, but not returned to, the pool
		/// becomes invalid.
	{
		clear();
	}

	void* get()
		/// Returns pointer to the next available
		/// memory block. If the pool is exhausted,
		/// it will be resized by allocating a new
		/// bucket.
	{
		Block* ret;
		{
			ScopedLock l(_mutex);
			if(_firstBlock == 0) resize();
			ret = _firstBlock;
			_firstBlock = _firstBlock->_memory.next;
		}
		--_available;
		return ret;
	}

	template <typename P>
	void release(P* ptr)
		/// Recycles the released memory by initializing it for
		/// internal use and setting it as next available block;
		/// previously next block becomes this block's next.
		/// Releasing of null pointers is silently ignored.
		/// Destructor is called for the returned pointer.
	{
		if (!ptr) return;
		reinterpret_cast<P*>(ptr)->~P();
		++_available;
		ScopedLock l(_mutex);
		_firstBlock = new (ptr) Block(_firstBlock);
	}

	std::size_t blockSize() const
		/// Returns the block size in bytes.
	{
		return sizeof(Block);
	}

	std::size_t allocated() const
		/// Returns the total amount of memory allocated, in bytes.
	{
		return _buckets.size() * _blocksPerBucket;
	}

	std::size_t available() const
		/// Returns currently available amount of memory in bytes.
	{
		return _available;
	}

private:
	FastMemoryPool(const FastMemoryPool&);
	FastMemoryPool& operator = (const FastMemoryPool&);

	void resize()
		/// Creates new bucket and initializes it for internal use.
		/// Sets the previously next block to point to the new bucket's
		/// first block and the new bucket's last block becomes the
		/// last block.
	{
		if (_buckets.size() == _buckets.capacity())
		{
			std::size_t newSize = _buckets.capacity() * 2;
			if (_maxAlloc != 0 && newSize > _maxAlloc) throw std::bad_alloc();
			_buckets.reserve(newSize);
		}
		_buckets.push_back(new Block[_blocksPerBucket]);
		_firstBlock = _buckets.back();
		// terminate last block
		_firstBlock[_blocksPerBucket-1]._memory.next = 0;
		_available = _available.value() + static_cast<AtomicCounter::ValueType>(_blocksPerBucket);
	}

	void clear()
	{
		typename BucketVec::iterator it = _buckets.begin();
		typename BucketVec::iterator end = _buckets.end();
		for (; it != end; ++it) delete[] *it;
	}

	typedef Poco::AtomicCounter Counter;

	const
	std::size_t _blocksPerBucket;
	BucketVec   _buckets;
	Block*      _firstBlock;
	std::size_t _maxAlloc;
	Counter     _available;
	mutable M   _mutex;
};


//
// inlines
//
inline std::size_t MemoryPool::blockSize() const
{
	return _blockSize;
}


inline int MemoryPool::allocated() const
{
	return _allocated;
}


inline int MemoryPool::available() const
{
	return (int) _blocks.size();
}


} // namespace Poco


#endif // Foundation_MemoryPool_INCLUDED
