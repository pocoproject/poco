/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/TransitEvent.h"
#include "quill/bundled/fmt/format.h" // for assert_fail
#include "quill/core/Attributes.h"
#include "quill/core/MathUtilities.h"

QUILL_BEGIN_NAMESPACE

namespace detail
{

class TransitEventBuffer
{
public:
  explicit TransitEventBuffer(size_t initial_capacity)
    : _initial_capacity(next_power_of_two(initial_capacity)),
      _capacity(_initial_capacity),
      _storage(std::make_unique<TransitEvent[]>(_capacity)),
      _mask(_capacity - 1u)
  {
  }

  TransitEventBuffer(TransitEventBuffer const&) = delete;
  TransitEventBuffer& operator=(TransitEventBuffer const&) = delete;

  // Move constructor
  TransitEventBuffer(TransitEventBuffer&& other) noexcept
    : _initial_capacity(other._initial_capacity),
      _capacity(other._capacity),
      _storage(std::move(other._storage)),
      _mask(other._mask),
      _reader_pos(other._reader_pos),
      _writer_pos(other._writer_pos),
      _shrink_requested(other._shrink_requested)
  {
    other._capacity = 0;
    other._mask = 0;
    other._reader_pos = 0;
    other._writer_pos = 0;
    other._shrink_requested = false;
  }

  // Move assignment operator
  TransitEventBuffer& operator=(TransitEventBuffer&& other) noexcept
  {
    if (this != &other)
    {
      _initial_capacity = other._initial_capacity;
      _capacity = other._capacity;
      _storage = std::move(other._storage);
      _mask = other._mask;
      _reader_pos = other._reader_pos;
      _writer_pos = other._writer_pos;
      _shrink_requested = other._shrink_requested;

      other._capacity = 0;
      other._mask = 0;
      other._reader_pos = 0;
      other._writer_pos = 0;
      other._shrink_requested = false;
    }
    return *this;
  }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT TransitEvent* front() noexcept
  {
    if (_reader_pos == _writer_pos)
    {
      return nullptr;
    }
    return &_storage[_reader_pos & _mask];
  }

  QUILL_ATTRIBUTE_HOT void pop_front() noexcept { ++_reader_pos; }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT TransitEvent* back() noexcept
  {
    if (_capacity == size())
    {
      // Buffer is full, need to expand
      _expand();
    }
    return &_storage[_writer_pos & _mask];
  }

  QUILL_ATTRIBUTE_HOT void push_back() noexcept { ++_writer_pos; }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT size_t size() const noexcept
  {
    return _writer_pos - _reader_pos;
  }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT size_t capacity() const noexcept { return _capacity; }

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT bool empty() const noexcept
  {
    return _reader_pos == _writer_pos;
  }

  void request_shrink() noexcept { _shrink_requested = true; }

  void try_shrink()
  {
    // we only shrink empty buffers
    if (_shrink_requested && empty())
    {
      if (_capacity > _initial_capacity)
      {
        _storage = std::make_unique<TransitEvent[]>(_initial_capacity);
        _capacity = _initial_capacity;
        _mask = _capacity - 1;
        _writer_pos = 0;
        _reader_pos = 0;
      }

      _shrink_requested = false;
    }
  }

private:
  void _expand()
  {
    size_t const new_capacity = _capacity * 2;

    auto new_storage = std::make_unique<TransitEvent[]>(new_capacity);

    // Move existing elements from the old storage to the new storage.
    // Since the buffer is full, this moves all the previous TransitEvents, preserving their order.
    // The reader position and mask are used to handle the circular buffer's wraparound.
    size_t const current_size = size();
    for (size_t i = 0; i < current_size; ++i)
    {
      new_storage[i] = std::move(_storage[(_reader_pos + i) & _mask]);
    }

    _storage = std::move(new_storage);
    _capacity = new_capacity;
    _mask = _capacity - 1;
    _writer_pos = current_size;
    _reader_pos = 0;
  }

  size_t _initial_capacity;
  size_t _capacity;
  std::unique_ptr<TransitEvent[]> _storage;
  size_t _mask;
  size_t _reader_pos{0};
  size_t _writer_pos{0};
  bool _shrink_requested{false};
};

} // namespace detail

QUILL_END_NAMESPACE
