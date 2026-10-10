/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/StringFromTime.h"
#include "quill/bundled/fmt/format.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/QuillError.h"

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <utility>

QUILL_BEGIN_NAMESPACE

namespace detail
{

/**
 * Formats a timestamp given a format string as a pattern. The format pattern uses the
 * same format specifiers as strftime() but with the following additional specifiers :
 * 1) %Qms - Milliseconds
 * 2) %Qus - Microseconds
 * 3) %Qns - Nanoseconds
 * @note %Qms, %Qus, %Qns specifiers are mutually exclusive
 * e.g. given : "%I:%M.%Qms%p" the output would be "03:21.343PM"
 */
class TimestampFormatter
{
private:
  enum AdditionalSpecifier : uint8_t
  {
    None,
    Qms,
    Qus,
    Qns
  };

public:
  /***/
  explicit TimestampFormatter(std::string time_format, Timezone timestamp_timezone = Timezone::LocalTime)
    : _time_format(std::move(time_format)), _timestamp_timezone(timestamp_timezone)
  {
    QUILL_ASSERT(
      _timestamp_timezone == Timezone::LocalTime || _timestamp_timezone == Timezone::GmtTime,
      "Invalid timezone type in TimestampFormatter constructor, must be LocalTime or GmtTime");

    // store the beginning of the found specifier
    size_t specifier_begin{std::string::npos};

    // look for all three special specifiers

    if (size_t const search_qms = _time_format.find(specifier_name[AdditionalSpecifier::Qms]);
        search_qms != std::string::npos)
    {
      _additional_format_specifier = AdditionalSpecifier::Qms;
      specifier_begin = search_qms;
    }

    if (size_t const search_qus = _time_format.find(specifier_name[AdditionalSpecifier::Qus]);
        search_qus != std::string::npos)
    {
      if (specifier_begin != std::string::npos)
      {
        QUILL_THROW(QuillError{"format specifiers %Qms, %Qus and %Qns are mutually exclusive"});
      }

      _additional_format_specifier = AdditionalSpecifier::Qus;
      specifier_begin = search_qus;
    }

    if (size_t const search_qns = _time_format.find(specifier_name[AdditionalSpecifier::Qns]);
        search_qns != std::string::npos)
    {
      if (specifier_begin != std::string::npos)
      {
        QUILL_THROW(QuillError{"format specifiers %Qms, %Qus and %Qns are mutually exclusive"});
      }

      _additional_format_specifier = AdditionalSpecifier::Qns;
      specifier_begin = search_qns;
    }

    if (specifier_begin == std::string::npos)
    {
      // If no additional specifier was found then we can simply store the whole format string
      QUILL_ASSERT(_additional_format_specifier == AdditionalSpecifier::None,
                   "Unexpected specifier state in TimestampFormatter constructor, should be None "
                   "when no specifier found");
      _strftime_part_1.init(_time_format, _timestamp_timezone);
    }
    else
    {
      // We now the index where the specifier begins so copy everything until there from beginning
      std::string const format_part_1 = _time_format.substr(0, specifier_begin);
      _strftime_part_1.init(format_part_1, _timestamp_timezone);

      // Now copy the remaining format string, ignoring the specifier
      size_t const specifier_end = specifier_begin + specifier_length;
      std::string const format_part_2 =
        _time_format.substr(specifier_end, _time_format.length() - specifier_end);

      if (!format_part_2.empty())
      {
        _strftime_part_2.init(format_part_2, _timestamp_timezone);
        _has_format_part_2 = true;
      }
    }
  }

  /***/
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::string_view format_timestamp(std::chrono::nanoseconds time_since_epoch)
  {
    int64_t const timestamp_ns = time_since_epoch.count();

    // convert timestamp to seconds
    int64_t const timestamp_secs = timestamp_ns / 1'000'000'000;

    // First always clear our cached string
    _formatted_date.clear();

    // 1. we always format part 1
    _formatted_date.append(_strftime_part_1.format_timestamp(timestamp_secs));

    // 2. We add any special ms/us/ns specifier if any
    auto const extracted_ns = static_cast<uint32_t>(timestamp_ns - (timestamp_secs * 1'000'000'000));

    if (_additional_format_specifier == AdditionalSpecifier::Qms)
    {
      uint32_t const extracted_ms = extracted_ns / 1'000'000;

      // Add as many zeros as the extracted_fractional_seconds_length
      static constexpr std::string_view zeros{"000"};
      _formatted_date.append(zeros);

      _write_fractional_seconds(extracted_ms);
    }
    else if (_additional_format_specifier == AdditionalSpecifier::Qus)
    {
      uint32_t const extracted_us = extracted_ns / 1'000;

      // Add as many zeros as the extracted_fractional_seconds_length
      static constexpr std::string_view zeros{"000000"};
      _formatted_date.append(zeros);

      _write_fractional_seconds(extracted_us);
    }
    else if (_additional_format_specifier == AdditionalSpecifier::Qns)
    {
      // Add as many zeros as the extracted_fractional_seconds_length
      static constexpr std::string_view zeros{"000000000"};
      _formatted_date.append(zeros);

      _write_fractional_seconds(extracted_ns);
    }

    // 3. format part 2 after fractional seconds - if any
    if (_has_format_part_2)
    {
      _formatted_date.append(_strftime_part_2.format_timestamp(timestamp_secs));
    }

    return std::string_view{_formatted_date.data(), _formatted_date.size()};
  }

  /***/
  QUILL_NODISCARD std::string const& time_format() const noexcept { return _time_format; }

  /***/
  QUILL_NODISCARD Timezone timestamp_timezone() const noexcept { return _timestamp_timezone; }

private:
  /***/
  void _write_fractional_seconds(uint32_t extracted_fractional_seconds)
  {
    // Format the seconds and add them
    fmtquill::format_int const extracted_ms_string{extracted_fractional_seconds};

    // _formatted_date.size() - extracted_ms_string.size() is where we want to begin placing the fractional seconds
    std::memcpy(&_formatted_date[_formatted_date.size() - extracted_ms_string.size()],
                extracted_ms_string.data(), extracted_ms_string.size());
  }

private:
  /** Contains the additional specifier name, at the same index as the enum **/
  static constexpr std::array<char const*, 4> specifier_name{"", "%Qms", "%Qus", "%Qns"};

  /** All special specifiers have same length at the moment **/
  static constexpr size_t specifier_length = 4u;

  std::string _time_format;
  fmtquill::basic_memory_buffer<char, 32> _formatted_date;

  /** Strftime cache for both parts of the string */
  StringFromTime _strftime_part_1;
  StringFromTime _strftime_part_2;

  /** Timezone, Local time by default **/
  Timezone _timestamp_timezone;

  /** fractional seconds */
  AdditionalSpecifier _additional_format_specifier{AdditionalSpecifier::None};
  bool _has_format_part_2{false};
};

} // namespace detail

QUILL_END_NAMESPACE