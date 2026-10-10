#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/MathUtilities.h"
#include "quill/core/QuillError.h"

#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#if defined(_WIN32)
  #include <malloc.h>
#else
  #include <sys/mman.h>
  #include <unistd.h>
#endif

#if defined(QUILL_X86ARCH)
  #if defined(_WIN32)
    #include <intrin.h>
  #else
    #if __has_include(<x86gprintrin.h>)
      #if defined(__GNUC__) && __GNUC__ > 10
        #include <emmintrin.h>
        #include <x86gprintrin.h>
      #elif defined(__clang_major__)
      // clang needs immintrin for _mm_clflushopt
        #include <immintrin.h>
      #endif
    #else
      #include <immintrin.h>
      #include <x86intrin.h>
    #endif
  #endif
#endif

QUILL_BEGIN_NAMESPACE

namespace detail
{

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

/**
 * A bounded single producer single consumer ring buffer.
 */
template <typename T>
class BoundedSPSCQueueImpl
{
public:
  using integer_type = T;

  QUILL_ATTRIBUTE_HOT explicit BoundedSPSCQueueImpl(integer_type capacity,
                                                    HugePagesPolicy huge_pages_policy = HugePagesPolicy::Never,
                                                    integer_type reader_store_percent = 5)
    : _capacity(next_power_of_two(capacity)),
      _mask(_capacity - 1),
      _bytes_per_batch(static_cast<integer_type>(static_cast<double>(_capacity * reader_store_percent) / 100.0)),
      _storage(static_cast<std::byte*>(_alloc_aligned(
        2ull * static_cast<uint64_t>(_capacity), QUILL_CACHE_LINE_ALIGNED, huge_pages_policy))),
      _huge_pages_policy(huge_pages_policy)
  {
    std::memset(_storage, 0, 2ull * static_cast<uint64_t>(_capacity));

    _atomic_writer_pos.store(0);
    _atomic_reader_pos.store(0);

#if defined(QUILL_X86ARCH)
    // remove log memory from cache
    for (uint64_t i = 0; i < (2ull * static_cast<uint64_t>(_capacity)); i += QUILL_CACHE_LINE_SIZE)
    {
      _mm_clflush(_storage + i);
    }

    // load cache lines into memory
    if (_capacity < 1024)
    {
      QUILL_THROW(QuillError{"Capacity must be at least 1024"});
    }

    uint64_t const cache_lines = (_capacity >= 2048) ? 32 : 16;

    for (uint64_t i = 0; i < cache_lines; ++i)
    {
      _mm_prefetch(reinterpret_cast<char const*>(_storage + (QUILL_CACHE_LINE_SIZE * i)), _MM_HINT_T0);
    }
#endif
  }

  ~BoundedSPSCQueueImpl() { _free_aligned(_storage); }

  /**
   * Deleted
   */
  BoundedSPSCQueueImpl(BoundedSPSCQueueImpl const&) = delete;
  BoundedSPSCQueueImpl& operator=(BoundedSPSCQueueImpl const&) = delete;

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::byte* prepare_write(integer_type n) noexcept
  {
    if ((_capacity - static_cast<integer_type>(_writer_pos - _reader_pos_cache)) < n)
    {
      // not enough space, we need to load reader and re-check
      _reader_pos_cache = _atomic_reader_pos.load(std::memory_order_acquire);

      if ((_capacity - static_cast<integer_type>(_writer_pos - _reader_pos_cache)) < n)
      {
        return nullptr;
      }
    }

    return _storage + (_writer_pos & _mask);
  }

  QUILL_ATTRIBUTE_HOT void finish_write(integer_type n) noexcept { _writer_pos += n; }

  QUILL_ATTRIBUTE_HOT void commit_write() noexcept
  {
    // set the atomic flag so the reader can see write
    _atomic_writer_pos.store(_writer_pos, std::memory_order_release);

#if defined(QUILL_X86ARCH)
    // flush writen cache lines
    _flush_cachelines(_last_flushed_writer_pos, _writer_pos);

    // prefetch a future cache line
    _mm_prefetch(
      reinterpret_cast<char const*>(_storage + (_writer_pos & _mask) + (QUILL_CACHE_LINE_SIZE * 10)), _MM_HINT_T0);
#endif
  }

  /**
   * Finish and commit write as a single function
   */
  QUILL_ATTRIBUTE_HOT void finish_and_commit_write(integer_type n) noexcept
  {
    finish_write(n);
    commit_write();
  }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::byte* prepare_read() noexcept
  {
    if (empty())
    {
      return nullptr;
    }

    return _storage + (_reader_pos & _mask);
  }

  QUILL_ATTRIBUTE_HOT void finish_read(integer_type n) noexcept { _reader_pos += n; }

  QUILL_ATTRIBUTE_HOT void commit_read() noexcept
  {
    if (static_cast<integer_type>(_reader_pos - _atomic_reader_pos.load(std::memory_order_relaxed)) >= _bytes_per_batch)
    {
      _atomic_reader_pos.store(_reader_pos, std::memory_order_release);

#if defined(QUILL_X86ARCH)
      _flush_cachelines(_last_flushed_reader_pos, _reader_pos);
#endif
    }
  }

  /**
   * Only meant to be called by the reader
   * @return true if the queue is empty
   */
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT bool empty() const noexcept
  {
    if (_writer_pos_cache == _reader_pos)
    {
      // if we think the queue is empty we also load the atomic variable to check further
      _writer_pos_cache = _atomic_writer_pos.load(std::memory_order_acquire);

      if (_writer_pos_cache == _reader_pos)
      {
        return true;
      }
    }

    return false;
  }

  QUILL_NODISCARD integer_type capacity() const noexcept
  {
    return static_cast<integer_type>(_capacity);
  }

  QUILL_NODISCARD HugePagesPolicy huge_pages_policy() const noexcept { return _huge_pages_policy; }

private:
#if defined(QUILL_X86ARCH)
  QUILL_ATTRIBUTE_HOT void _flush_cachelines(integer_type& last, integer_type offset)
  {
    integer_type last_diff = last - (last & QUILL_CACHE_LINE_MASK);
    integer_type const cur_diff = offset - (offset & QUILL_CACHE_LINE_MASK);

    if (cur_diff > last_diff)
    {
      // Compute masked base pointer once before loop to avoid repeated mask operations
      std::byte* ptr = _storage + (last_diff & _mask);

      do
      {
        _mm_clflushopt(ptr);
        ptr += QUILL_CACHE_LINE_SIZE;
        last_diff += QUILL_CACHE_LINE_SIZE;
      } while (cur_diff > last_diff);

      last = last_diff;
    }
  }
#endif

  /**
   * align a pointer to the given alignment
   * @param pointer a pointer the object
   * @param alignment a pointer the object
   * @return an aligned pointer for the given object
   */
  QUILL_NODISCARD static std::byte* _align_pointer(void* pointer, size_t alignment) noexcept
  {
    QUILL_ASSERT(is_power_of_two(alignment),
                 "alignment must be a power of two in BoundedSPSCQueue::_align_pointer()");
    return reinterpret_cast<std::byte*>((reinterpret_cast<uintptr_t>(pointer) + (alignment - 1ul)) &
                                        ~(alignment - 1ul));
  }

  /**
   * Aligned alloc
   * @param size number of bytes to allocate. An integral multiple of alignment
   * @param alignment specifies the alignment. Must be a valid alignment supported by the implementation.
   * @param huge_pages_policy allocate huge pages, only supported on linux
   * @return On success, returns the pointer to the beginning of newly allocated memory.
   * To avoid a memory leak, the returned pointer must be deallocated with _free_aligned().
   * @throws QuillError on failure
   */

  QUILL_NODISCARD static void* _alloc_aligned(size_t size, size_t alignment,
                                              QUILL_MAYBE_UNUSED HugePagesPolicy huge_pages_policy)
  {
#if defined(_WIN32)
    void* p = _aligned_malloc(size, alignment);

    if (!p)
    {
      QUILL_THROW(QuillError{std::string{"_alloc_aligned failed with error message errno: "} +
                             std::to_string(errno)});
    }

    return p;
#else
    // Calculate the total size including the metadata and alignment
    constexpr size_t metadata_size{2u * sizeof(size_t)};
    size_t const total_size{size + metadata_size + alignment};

    // Allocate the memory
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;

  #if defined(__linux__)
    if (huge_pages_policy != HugePagesPolicy::Never)
    {
      flags |= MAP_HUGETLB;
    }
  #endif

    void* mem = ::mmap(nullptr, total_size, PROT_READ | PROT_WRITE, flags, -1, 0);

  #if defined(__linux__)
    if ((mem == MAP_FAILED) && (huge_pages_policy == HugePagesPolicy::Try))
    {
      // we tried but failed allocating huge pages, try normal pages instead
      flags &= ~MAP_HUGETLB;
      mem = ::mmap(nullptr, total_size, PROT_READ | PROT_WRITE, flags, -1, 0);
    }
  #endif

    if (mem == MAP_FAILED)
    {
      QUILL_THROW(QuillError{std::string{"mmap failed. errno: "} + std::to_string(errno) +
                             " error: " + std::strerror(errno)});
    }

    // Calculate the aligned address after the metadata
    std::byte* aligned_address = _align_pointer(static_cast<std::byte*>(mem) + metadata_size, alignment);

    // Calculate the offset from the original memory location
    auto const offset = static_cast<size_t>(aligned_address - static_cast<std::byte*>(mem));

    // Store the size and offset information in the metadata
    std::memcpy(aligned_address - sizeof(size_t), &total_size, sizeof(total_size));
    std::memcpy(aligned_address - (2u * sizeof(size_t)), &offset, sizeof(offset));

    return aligned_address;
#endif
  }

  /**
   * Free aligned memory allocated with _alloc_aligned
   * @param ptr address to aligned memory
   */
  void static _free_aligned(void* ptr) noexcept
  {
#if defined(_WIN32)
    _aligned_free(ptr);
#else
    // Retrieve the size and offset information from the metadata
    size_t offset;
    std::memcpy(&offset, static_cast<std::byte*>(ptr) - (2u * sizeof(size_t)), sizeof(offset));

    size_t total_size;
    std::memcpy(&total_size, static_cast<std::byte*>(ptr) - sizeof(size_t), sizeof(total_size));

    // Calculate the original memory block address
    void* mem = static_cast<std::byte*>(ptr) - offset;

    ::munmap(mem, total_size);
#endif
  }

private:
  static constexpr integer_type QUILL_CACHE_LINE_MASK{QUILL_CACHE_LINE_SIZE - 1};

  integer_type const _capacity;
  integer_type const _mask;
  integer_type const _bytes_per_batch;
  std::byte* const _storage{nullptr};
  HugePagesPolicy const _huge_pages_policy;

  alignas(QUILL_CACHE_LINE_ALIGNED) std::atomic<integer_type> _atomic_writer_pos{0};
  alignas(QUILL_CACHE_LINE_ALIGNED) integer_type _writer_pos{0};
  integer_type _reader_pos_cache{0};
  integer_type _last_flushed_writer_pos{0};

  alignas(QUILL_CACHE_LINE_ALIGNED) std::atomic<integer_type> _atomic_reader_pos{0};
  alignas(QUILL_CACHE_LINE_ALIGNED) integer_type _reader_pos{0};
  mutable integer_type _writer_pos_cache{0};
  integer_type _last_flushed_reader_pos{0};
};

using BoundedSPSCQueue = BoundedSPSCQueueImpl<size_t>;

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
#pragma warning(pop)
#endif

} // namespace detail

QUILL_END_NAMESPACE