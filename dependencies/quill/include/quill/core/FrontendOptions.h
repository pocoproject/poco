/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"

#include <cstdint>

QUILL_BEGIN_NAMESPACE

struct FrontendOptions
{
  /**
   * Each frontend thread has its own queue, which can be configured with various options:
   * - UnboundedBlocking: Starts with initial_queue_capacity and reallocates up to unbounded_queue_max_capacity, then blocks.
   * - UnboundedDropping: Starts with initial_queue_capacity and reallocates up to unbounded_queue_max_capacity, then drops log messages.
   * - BoundedBlocking: Starts with initial_queue_capacity and never reallocates; blocks when the limit is reached.
   * - BoundedDropping: Starts with initial_queue_capacity and never reallocates; drops log messages when the limit is reached.
   *
   * By default, the library uses an UnboundedBlocking queue, which starts with initial_queue_capacity.
   */
  static constexpr QueueType queue_type = QueueType::UnboundedBlocking;

  /**
   * Initial capacity of the queue.
   */
  static constexpr size_t initial_queue_capacity = 128u * 1024u; // 128 KiB

  /**
   * Interval for retrying when using BoundedBlocking or UnboundedBlocking.
   * Applicable only when using BoundedBlocking or UnboundedBlocking.
   */
  static constexpr uint32_t blocking_queue_retry_interval_ns = 800;

  /**
   * Maximum capacity for unbounded queues (UnboundedBlocking, UnboundedDropping).
   * This defines the maximum size to which the queue can grow before blocking or dropping messages.
   */
  static constexpr size_t unbounded_queue_max_capacity = 2ull * 1024u * 1024u * 1024u; // 2 GiB

  /**
   * Enables huge pages on the frontend queues to reduce TLB misses. Available only for Linux.
   */
  static constexpr HugePagesPolicy huge_pages_policy = HugePagesPolicy::Never;
};

QUILL_END_NAMESPACE