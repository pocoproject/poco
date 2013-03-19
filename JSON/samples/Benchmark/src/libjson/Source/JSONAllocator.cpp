#include "JSONAllocator.h"

#if defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
#include "JSONMemory.h"

#ifdef JSON_UNIT_TEST
    static size_t alloccount = 0;
    size_t JSONAllocatorRelayer::getAllocationCount(void){ return alloccount; }
    #define INC_ALLOC() ++alloccount;
#else
    #define INC_ALLOC() (void)0
#endif

void * JSONAllocatorRelayer::alloc(size_t bytes) json_nothrow {
    INC_ALLOC();
	return JSONMemory::json_malloc(bytes);
}

void JSONAllocatorRelayer::dealloc(void * ptr) json_nothrow {
	JSONMemory::json_free(ptr);
}
#endif
