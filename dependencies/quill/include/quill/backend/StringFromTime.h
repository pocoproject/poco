/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/bundled/fmt/format.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/QuillError.h"
#include "quill/core/TimeUtilities.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * A class that converts a timestamp to a string based on the given format.
 * It works exactly the same as strftime() but it caches the string on it's creation
 * and only modifies the parts of the string that need to change.
 * This is much more efficient than calling strftime as we just need to calculate the different
 * between the cached and the requested timestamp and modify our preformatted string.
 *
 * 1) We take a format string eg "%Y-%m-%dT%H:%M:%SZ".
 * 2) We split it into several parts (_initial_parts) where we separate the time modifiers.
 * e.g "%Y-%m-%dT", "%H", ":", "%M", ":", "%S", Z".
 * 3) We cache the current timestamp and current h/m/s in seconds and on each part above
 * we call strftime() and we create a preformatted string.
 * 4) While calling stftime in 3) we can manually check for any time modifiers and store
 * their index in the final pre-formatted string.
 * 5) Next time we want to calculate the timestamp we will just calculate the difference
 * in seconds between the current and the cache timestamp.
 * Then we add this value to the cached seconds and then convert the seconds
 * to hh::mm::ss and replace in our pre-formatted string using the stored indexes.
 * 6) We re-calculate the pre-formatted string every midnight and noon.
 */
class StringFromTime
{
public:
  /***/
  QUILL_ATTRIBUTE_COLD void init(std::string timestamp_format, Timezone timezone)
  {
    _timestamp_format = std::move(timestamp_format);
    _time_zone = timezone;

    if (_timestamp_format.find("%X") != std::string::npos)
    {
      QUILL_THROW(QuillError("`%X` as format modifier is not currently supported in format: " + _timestamp_format));
    }

    // We first look for some special format modifiers and replace them
    _replace_all(_timestamp_format, "%r", "%I:%M:%S %p");
    _replace_all(_timestamp_format, "%R", "%H:%M");
    _replace_all(_timestamp_format, "%T", "%H:%M:%S");

    // Populate the initial parts that we will use to generate a pre-formatted string
    _populate_initial_parts(_timestamp_format);
  }

  /***/
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::string const& format_timestamp(time_t timestamp)
  {
    // First we check for the edge case where the given timestamp is back in time. This is when
    // the timestamp provided is less than our cached_timestamp. We only expect to format timestamps
    // that are incrementing not those back in time. In this case we just fall back to calling strfime
    if (timestamp < _cached_timestamp)
    {
      _fallback_formatted = _safe_strftime(_timestamp_format.data(), timestamp, _time_zone).data();
      return _fallback_formatted;
    }

    // After this point we know that given timestamp is >= to the cache timestamp.

    // We check if the given timestamp greater than the _next_recalculation_timestamp to recalculate
    if (timestamp >= _next_recalculation_timestamp)
    {
      // in this case we have to populate our cached string again using strftime
      _pre_formatted_ts.clear();
      _cached_indexes.clear();

      // Now populate a pre-formatted string for the next rec
      _populate_pre_formatted_string_and_cached_indexes(timestamp);

      if (_time_zone == Timezone::LocalTime)
      {
        // If localtime is used, we will recalculate every 15 minutes. This approach accounts for
        // DST changes and simplifies handling transitions around midnight. Recalculating every 15
        // minutes ensures coverage for all possible timezones without additional computations.
        _next_recalculation_timestamp = _next_quarter_hour_timestamp(timestamp);
      }
      else if (_time_zone == Timezone::GmtTime)
      {
        // otherwise we will only recalculate every noon and midnight. the reason for this is in
        // case user is using PM, AM format etc
        _next_recalculation_timestamp = _next_noon_or_midnight_timestamp(timestamp);
      }
    }

    if (_cached_indexes.empty())
    {
      // if we don't have to format any hours minutes or seconds we can just return here
      return _pre_formatted_ts;
    }

    if (_cached_timestamp == timestamp)
    {
      // This has 2 usages:
      // 1. Any timestamps in seconds precision that are the same, we don't need to do anything.
      // 2. This checks happens after the _next_recalculation_timestamp calculation. The _next_recalculation_timestamp
      // will mutate _cached_timestamp and if they are similar we will just return here anyway
      return _pre_formatted_ts;
    }

    // Get the difference from our cached timestamp
    time_t const timestamp_diff = timestamp - _cached_timestamp;

    // cache this timestamp
    _cached_timestamp = timestamp;

    // Add the timestamp_diff to the cached seconds and calculate the new hours minutes seconds.
    // Note: cached_seconds could be in gmtime or localtime but we don't care as we are just
    // adding the difference.
    _cached_seconds += static_cast<uint32_t>(timestamp_diff);

    uint32_t total_seconds = _cached_seconds;
    uint32_t const hours = total_seconds / 3600;
    total_seconds -= hours * 3600;
    uint32_t const minutes = total_seconds / 60;
    total_seconds -= minutes * 60;
    uint32_t const seconds = total_seconds;

    // only update components that have changed
    bool const hours_changed = (hours != _prev_hours);
    bool const minutes_changed = (minutes != _prev_minutes);
    bool const seconds_changed = (seconds != _prev_seconds);

    // Update cached values
    _prev_hours = hours;
    _prev_minutes = minutes;
    _prev_seconds = seconds;

    for (auto const& index : _cached_indexes)
    {
      // for each cached index we have, replace it when the new value
      switch (index.second)
      {
      case format_type::H:
        if (hours_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:02}", hours);
        }
        break;
      case format_type::M:
        if (minutes_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:02}", minutes);
        }
        break;
      case format_type::S:
        if (seconds_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:02}", seconds);
        }
        break;
      case format_type::I:
        if (hours_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:02}",
                              (hours == 0 ? 12 : (hours > 12 ? hours - 12 : hours)));
        }
        break;
      case format_type::l:
        if (hours_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:2}",
                              (hours == 0 ? 12 : (hours > 12 ? hours - 12 : hours)));
        }
        break;
      case format_type::k:
        if (hours_changed)
        {
          fmtquill::format_to(&_pre_formatted_ts[index.first], "{:2}", hours);
        }
        break;
      case format_type::s:
        // Timestamp in seconds always changes
        fmtquill::format_to(&_pre_formatted_ts[index.first], "{:10}", _cached_timestamp);
        break;
      default:
        abort();
      }
    }

    return _pre_formatted_ts;
  }

protected:
  enum class format_type : uint8_t
  {
    H,
    M,
    S,
    I,
    k,
    l,
    s
  };

  /***/
  QUILL_ATTRIBUTE_COLD void _populate_initial_parts(std::string timestamp_format)
  {
    do
    {
      // we get part1 and part2 and keep looping on the new modified string without the part1 and
      // part2 until we find not %H, %M or %S at all
      auto const [part1, part2] = _split_timestamp_format_once(timestamp_format);

      if (!part1.empty())
      {
        _initial_parts.push_back(part1);
      }

      if (!part2.empty())
      {
        _initial_parts.push_back(part2);
      }

      if (part1.empty() && part2.empty())
      {
        // if both part_1 and part_2 are empty it means we have no more
        // format modifiers to add, we push back the remaining timestamp_format string
        // and break
        if (!timestamp_format.empty())
        {
          _initial_parts.push_back(timestamp_format);
        }
        break;
      }
    } while (true);
  }

  /***/
  void _populate_pre_formatted_string_and_cached_indexes(time_t timestamp)
  {
    _cached_timestamp = timestamp;

    tm time_info{};

    if (_time_zone == Timezone::LocalTime)
    {
      localtime_rs(reinterpret_cast<time_t const*>(std::addressof(_cached_timestamp)), std::addressof(time_info));
    }
    else if (_time_zone == Timezone::GmtTime)
    {
      gmtime_rs(reinterpret_cast<time_t const*>(std::addressof(_cached_timestamp)), std::addressof(time_info));
    }

    // also cache the seconds
    _cached_seconds =
      static_cast<uint32_t>((time_info.tm_hour * 3600) + (time_info.tm_min * 60) + time_info.tm_sec);

    // Initialize previous time component tracking
    uint32_t total_seconds = _cached_seconds;
    _prev_hours = total_seconds / 3600;
    total_seconds -= _prev_hours * 3600;
    _prev_minutes = total_seconds / 60;
    _prev_seconds = total_seconds - _prev_minutes * 60;

    // Now run through all parts and call strftime
    for (auto const& format_part : _initial_parts)
    {
      // We call strftime on each part of the timestamp to format it.
      _pre_formatted_ts += _safe_strftime(format_part.data(), _cached_timestamp, _time_zone).data();

      // If we formatted and appended to the string a time modifier also store the
      // current index in the string
      if (format_part == "%H")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::H);
      }
      else if (format_part == "%M")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::M);
      }
      else if (format_part == "%S")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::S);
      }
      else if (format_part == "%I")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::I);
      }
      else if (format_part == "%k")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::k);
      }
      else if (format_part == "%l")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 2, format_type::l);
      }
      else if (format_part == "%s")
      {
        _cached_indexes.emplace_back(_pre_formatted_ts.size() - 10, format_type::s);
      }
    }
  }

  /***/
  std::pair<std::string, std::string> static _split_timestamp_format_once(std::string& timestamp_format) noexcept
  {
    // don't make this static as it breaks on windows with atexit when backend worker stops
    std::array<std::string, 7> const modifiers{"%H", "%M", "%S", "%I", "%k", "%l", "%s"};

    // if we find any modifier in the timestamp format we store the index where we
    // found it.
    // We use a map to find the first modifier (with the lowest index) in the given string
    // Maps found_index -> modifier value
    std::map<size_t, std::string> found_format_modifiers;

    for (auto const& modifier : modifiers)
    {
      if (auto const search = timestamp_format.find(modifier); search != std::string::npos)
      {
        // Add the index and the modifier string to our map
        found_format_modifiers.emplace(search, modifier);
      }
    }

    if (found_format_modifiers.empty())
    {
      // We didn't find any modifiers in the given string, therefore we return
      // both parts as empty
      return std::make_pair(std::string{}, std::string{});
    }

    // we will split the formatted timestamp on the first modifier we found

    // Part 1 is the part before the modifier
    // Here we check that there is actually a part of before and the format doesn't start with the
    // modifier, otherwise we use an empty string
    std::string const part_1 = found_format_modifiers.begin()->first > 0
      ? std::string{timestamp_format.data(), found_format_modifiers.begin()->first}
      : "";

    // The actual value of the modifier string
    std::string const part_2 = found_format_modifiers.begin()->second;

    // We modify the given timestamp string to exclude part_1 and part_2.
    // part_2 length as the modifier value will always be 2
    timestamp_format = std::string{timestamp_format.data() + found_format_modifiers.begin()->first + 2};

    return std::make_pair(part_1, part_2);
  }

  /***/
  QUILL_NODISCARD static std::vector<char> _safe_strftime(char const* format_string, time_t timestamp, Timezone timezone)
  {
    if (format_string[0] == '\0')
    {
      std::vector<char> res;
      res.push_back('\0');
      return res;
    }

    // Convert timestamp to time_info
    tm time_info;
    if (timezone == Timezone::LocalTime)
    {
      localtime_rs(reinterpret_cast<time_t const*>(std::addressof(timestamp)), std::addressof(time_info));
    }
    else if (timezone == Timezone::GmtTime)
    {
      gmtime_rs(reinterpret_cast<time_t const*>(std::addressof(timestamp)), std::addressof(time_info));
    }

    // Create a buffer to call strftimex
    std::vector<char> buffer;
    buffer.resize(32);
    size_t res = strftime(&buffer[0], buffer.size(), format_string, std::addressof(time_info));

    while (res == 0)
    {
      // if strftime fails we will reserve more space
      buffer.resize(buffer.size() * 2);
      res = strftime(&buffer[0], buffer.size(), format_string, std::addressof(time_info));
    }

    return buffer;
  }

  /***/
  static void _replace_all(std::string& str, std::string const& old_value, std::string const& new_value) noexcept
  {
    std::string::size_type pos = 0u;
    while ((pos = str.find(old_value, pos)) != std::string::npos)
    {
      str.replace(pos, old_value.length(), new_value);
      pos += new_value.length();
    }
  }

  /***/
  QUILL_NODISCARD static time_t _nearest_quarter_hour_timestamp(time_t timestamp) noexcept
  {
    time_t const nearest_quarter_hour_ts = (timestamp / 900) * 900;
    return nearest_quarter_hour_ts;
  }

  /***/
  QUILL_NODISCARD static time_t _next_quarter_hour_timestamp(time_t timestamp) noexcept
  {
    time_t const next_quarter_hour_ts = _nearest_quarter_hour_timestamp(timestamp) + 900;
    return next_quarter_hour_ts;
  }

  /***/
  QUILL_NODISCARD static time_t _next_noon_or_midnight_timestamp(time_t timestamp) noexcept
  {
    // Get the current date and time now as time_info
    tm time_info;
    gmtime_rs(&timestamp, &time_info);

    if (time_info.tm_hour < 12)
    {
      // we are before noon, so calculate noon
      time_info.tm_hour = 11;
      time_info.tm_min = 59;
      time_info.tm_sec = 59; // we add 1 second later
    }
    else
    {
      // we are after noon so we calculate midnight
      time_info.tm_hour = 23;
      time_info.tm_min = 59;
      time_info.tm_sec = 59; // we add 1 second later
    }

    // convert back to time since epoch
    std::chrono::system_clock::time_point const next_midnight =
      std::chrono::system_clock::from_time_t(detail::timegm(&time_info));

    // returns seconds since epoch of the next midnight.
    return std::chrono::duration_cast<std::chrono::seconds>(next_midnight.time_since_epoch()).count() + 1;
  }

private:
  /** Contains the timestamp_format broken down into parts. We call use those parts to
   * create a pre-formatted string */
  std::vector<std::string> _initial_parts;

  /** Contains stored indexes of the _pre_formatted_ts for each format time modifier*/
  std::vector<std::pair<size_t, format_type>> _cached_indexes;

  /** The format request format of the timestamp. This can be slightly modified in constructor so we store it. */
  std::string _timestamp_format;

  /** The pre-formatted timestamp string */
  std::string _pre_formatted_ts;

  /** This is only used only when we fallback to strftime */
  std::string _fallback_formatted;

  /** The timestamp of the next noon, or midnight, we use this to resync */
  time_t _next_recalculation_timestamp{0};

  /** The timestamp of value of our pre-formated string */
  time_t _cached_timestamp{0};

  /** The seconds of hh:mm:ss of the pre-formatted string this is after using gmtime or localtime on cached_timestamp */
  uint32_t _cached_seconds{0};

  /** Previous time components for change detection */
  uint32_t _prev_hours{0};
  uint32_t _prev_minutes{0};
  uint32_t _prev_seconds{0};

  /** gmtime or localtime */
  Timezone _time_zone{Timezone::GmtTime};
};
} // namespace detail

QUILL_END_NAMESPACE