//
// MemoryPool.cpp
//
// $Id: //poco/1.4/Foundation/src/MemoryPool.cpp#1 $
//
// Library: Foundation
// Package: Core
// Module:  MemoryPool
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MemoryPool.h"
#include "Poco/Exception.h"


namespace Poco {


MemoryPool::MemoryPool(std::size_t blockSize, int preAlloc, int maxAlloc):
	_blockSize(blockSize),
	_maxAlloc(maxAlloc)
{
	poco_assert (maxAlloc == 0 || maxAlloc >= preAlloc);
	poco_assert (preAlloc >= 0 && maxAlloc >= 0);

	// FIXME: we'd like to make the size of chunks aligned to page boundaries, which
	// will speedup chunk allocations when maxAlloc was specified as 
	int r = BLOCK_RESERVE;
	if (preAlloc > r)
		r = preAlloc;
	if (maxAlloc > 0 && maxAlloc < r)
		r = maxAlloc;

	_allocated = r;
	_avaliable = r;
	_blocksPerChunk = r;
	_chunk = AllocateNewChunk();
	_freeBlock = (void *) _chunk->start;
}

MemoryPool::_MemoryChunk* MemoryPool::AllocateNewChunk(void)
{
	std::size_t blksz = std::max(_blockSize, sizeof(void *));
	std::size_t chunkSize = _blocksPerChunk * blksz + sizeof(_MemoryChunk);

	// allocate memory for this new chunk
	_MemoryChunk *chunk = (_MemoryChunk *) (new char[chunkSize]);
	chunk->nextChunk = NULL;

	// link all blocks in this chunk into a freelist
	char **curblk = (char **) chunk->start;
	for (std::size_t i = 0; i < _blocksPerChunk - 1; i++)
	{
		char *nextblk = ((char *) curblk) + blksz;
		*curblk = nextblk;
		curblk = (char **) nextblk;
	}
	*curblk = NULL;

	return chunk;				// done
}
	
MemoryPool::~MemoryPool()
{
	_MemoryChunk *curchk = _chunk;
	while (curchk) {
		_MemoryChunk *next = curchk->nextChunk;
		delete [] ((char *) curchk);
		curchk = next;
	}
}


void* MemoryPool::get()
{
	FastMutex::ScopedLock lock(_mutex);
	void *ptr;

	if (_freeBlock == NULL)
	{
		if (_maxAlloc == 0 || _allocated < _maxAlloc)
		{
			// allocate a new chunk
			_MemoryChunk *newChunk = AllocateNewChunk();
			_chunk->nextChunk = newChunk;
			_chunk = newChunk;
			_avaliable += _blocksPerChunk;
			_allocated += _blocksPerChunk;

			// return a block from the newly allocated chunk
			ptr = (void *) newChunk->start;
			_freeBlock = *(void **) ptr;
		}
		else throw OutOfMemoryException("MemoryPool exhausted");
	}
	else
	{
		ptr = _freeBlock;
		_freeBlock = *(void **)_freeBlock;
	}

	--_avaliable;
	return ptr;
}

	
void MemoryPool::release(void* ptr)
{
	FastMutex::ScopedLock lock(_mutex);

	*(void **) ptr = _freeBlock;
	_freeBlock = ptr;
	++_avaliable;
}


} // namespace Poco
