/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/TransitEvent.h"
#include "quill/core/Attributes.h"

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * Stores N max messages per logger name in a vector.
 * For simplicity this class is used ONLY by the backend worker thread.
 * We push to the queue a BacktraceCommand event to communicate this from the frontend caller threads
 */
class BacktraceStorage
{
public:
  BacktraceStorage() = default;

  /***/
  void store(TransitEvent transit_event, std::string_view const& thread_id, std::string_view const& thread_name)
  {
    if (_stored_events.size() < _capacity)
    {
      // We are still growing the vector to max capacity
      _stored_events.emplace_back(std::string{thread_id}, std::string{thread_name}, std::move(transit_event));
    }
    else
    {
      // Store the object in the vector, replacing the previous
      StoredTransitEvent& ste = _stored_events[_index];

      ste = StoredTransitEvent{std::string{thread_id}, std::string{thread_name}, std::move(transit_event)};

      // Update the index wrapping around the vector capacity
      if (_index < _capacity - 1)
      {
        _index += 1;
      }
      else
      {
        _index = 0;
      }
    }
  }

  /***/
  void process(std::function<void(TransitEvent const&, std::string_view thread_id, std::string_view thread_name)> const& callback)
  {
    // we found stored messages for this logger
    uint32_t index = _index;

    // Iterate retrieved records in order from first message using index
    for (uint32_t i = 0; i < _stored_events.size(); ++i)
    {
      // Give to the user callback the thread id and the RecordBase pointer
      callback(_stored_events[index].transit_event, _stored_events[index].thread_id,
               _stored_events[index].thread_name);

      // We wrap around to iterate all messages
      if (index < _stored_events.size() - 1)
      {
        index += 1;
      }
      else
      {
        index = 0;
      }
    }

    // finally, clean all messages
    _stored_events.clear();
    _index = 0;
  }

  /***/
  void set_capacity(uint32_t capacity)
  {
    if (_capacity != capacity)
    {
      _capacity = capacity;
      _index = 0;
      _stored_events.clear();
      _stored_events.reserve(_capacity);
    }
  }

private:
  struct StoredTransitEvent
  {
    StoredTransitEvent(std::string thread_id, std::string thread_name, TransitEvent transit_event)
      : thread_id(std::move(thread_id)),
        thread_name(std::move(thread_name)),
        transit_event(std::move(transit_event))
    {
    }

    /**
     * We use this to take o copy of some objects that are out of space when a thread finishes but
     * the transit events are still in the buffer
     */
    std::string thread_id;
    std::string thread_name;
    TransitEvent transit_event;
  };

private:
  uint32_t _capacity{0};
  uint32_t _index{0};
  std::vector<StoredTransitEvent> _stored_events;
};
} // namespace detail

QUILL_END_NAMESPACE