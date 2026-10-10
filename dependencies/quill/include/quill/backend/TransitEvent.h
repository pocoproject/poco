/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/LogLevel.h"
#include "quill/core/MacroMetadata.h"

#include "quill/bundled/fmt/format.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/** Forward declaration */
class LoggerBase;

/***/
struct TransitEvent
{
  using FormatBuffer = fmtquill::basic_memory_buffer<char, 88>;

  /***/
  TransitEvent() = default;

  /***/
  ~TransitEvent() = default;

  /**
   * Copy constructor and assignment operator are deleted to prevent accidental copying.
   */
  TransitEvent(TransitEvent const& other) = delete;
  TransitEvent& operator=(TransitEvent const& other) = delete;

  /***/
  TransitEvent(TransitEvent&& other) noexcept
    : timestamp(other.timestamp),
      macro_metadata(other.macro_metadata),
      logger_base(other.logger_base),
      formatted_msg(std::move(other.formatted_msg)),
      extra_data(std::move(other.extra_data)),
      flush_flag(other.flush_flag)
  {
    // Update macro_metadata pointer if it was pointing to runtime_metadata
    if (extra_data && extra_data->runtime_metadata.has_runtime_metadata)
    {
      // Point to our own runtime_metadata instead of the moved-from object's
      macro_metadata = &extra_data->runtime_metadata.macro_metadata;
    }
  }

  /***/
  TransitEvent& operator=(TransitEvent&& other) noexcept
  {
    if (this != &other)
    {
      timestamp = other.timestamp;
      macro_metadata = other.macro_metadata;
      logger_base = other.logger_base;
      formatted_msg = std::move(other.formatted_msg);
      extra_data = std::move(other.extra_data);
      flush_flag = other.flush_flag;
    }

    if (extra_data && extra_data->runtime_metadata.has_runtime_metadata)
    {
      // Point to our own runtime_metadata instead of the moved-from object's
      macro_metadata = &extra_data->runtime_metadata.macro_metadata;
    }

    return *this;
  }

  /***/
  void copy_to(TransitEvent& other) const
  {
    other.timestamp = timestamp;
    other.macro_metadata = macro_metadata;
    other.logger_base = logger_base;
    other.flush_flag = flush_flag;

    // manually copy the fmt::buffer
    other.formatted_msg->clear();
    other.formatted_msg->reserve(formatted_msg->size());
    other.formatted_msg->append(*formatted_msg);

    if (extra_data)
    {
      other.extra_data = std::make_unique<ExtraData>(*extra_data);

      if (other.extra_data->runtime_metadata.has_runtime_metadata)
      {
        // then point macro_metadata to the correct object
        other.macro_metadata = &other.extra_data->runtime_metadata.macro_metadata;
      }
    }
  }

  /***/
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT LogLevel log_level() const noexcept
  {
    return macro_metadata->log_level();
  }

  /***/
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::vector<std::pair<std::string, std::string>>* get_named_args() const noexcept
  {
    return extra_data ? &extra_data->named_args : nullptr;
  }

  /***/
  QUILL_ATTRIBUTE_HOT void ensure_extra_data() noexcept
  {
    if (extra_data)
    {
      return;
    }

    extra_data = std::make_unique<ExtraData>();
  }

  struct RuntimeMetadata
  {
    RuntimeMetadata() = default;

    RuntimeMetadata(char const* file, uint32_t line, char const* function, char const* in_tags,
                    char const* in_fmt, LogLevel log_level)
      : fmt(in_fmt),
        source_location(_format_file_location(file, line)),
        function_name(function),
        tags(in_tags),
        macro_metadata(source_location.data(), function_name.data(), fmt.data(),
                       tags.empty() ? nullptr : tags.data(), log_level, MacroMetadata::Event::Log),
        has_runtime_metadata(true)
    {
    }

    RuntimeMetadata(RuntimeMetadata const& other)
      : fmt(other.fmt),
        source_location(other.source_location),
        function_name(other.function_name),
        tags(other.tags),
        macro_metadata(source_location.data(), function_name.data(), fmt.data(),
                       tags.empty() ? nullptr : tags.data(), other.macro_metadata.log_level(),
                       MacroMetadata::Event::Log),
        has_runtime_metadata(other.has_runtime_metadata)
    {
      // Recreate macro_metadata to point to our own strings
    }

    RuntimeMetadata& operator=(RuntimeMetadata const& other)
    {
      if (this != &other)
      {
        fmt = other.fmt;
        source_location = other.source_location;
        function_name = other.function_name;
        tags = other.tags;
        has_runtime_metadata = other.has_runtime_metadata;

        // Recreate macro_metadata to point to our own strings
        macro_metadata = MacroMetadata(source_location.data(), function_name.data(), fmt.data(),
                                       tags.empty() ? nullptr : tags.data(),
                                       other.macro_metadata.log_level(), MacroMetadata::Event::Log);
      }
      return *this;
    }

    RuntimeMetadata(RuntimeMetadata&& other) noexcept = delete;
    RuntimeMetadata& operator=(RuntimeMetadata&& other) noexcept = delete;

    std::string fmt;
    std::string source_location;
    std::string function_name;
    std::string tags;
    MacroMetadata macro_metadata;
    bool has_runtime_metadata{false};

  private:
    QUILL_NODISCARD static std::string _format_file_location(char const* file, uint32_t line)
    {
      if (!file || (file[0] == '\0' && line == 0))
      {
        return std::string{};
      }

      // Format as "file:line"
      return std::string{file} + ":" + std::to_string(line);
    }
  };

  struct ExtraData
  {
    // Additional fields that are used for some features as a separate structure to keep
    // TransitEvent size smaller for the main scenarios
    std::vector<std::pair<std::string, std::string>> named_args;
    RuntimeMetadata runtime_metadata;
  };

  uint64_t timestamp{0};
  MacroMetadata const* macro_metadata{nullptr};
  LoggerBase* logger_base{nullptr};
  std::unique_ptr<FormatBuffer> formatted_msg{std::make_unique<FormatBuffer>()}; /** buffer for message **/
  std::unique_ptr<ExtraData> extra_data; /** A unique ptr to save space as these fields not always used */
  std::atomic<bool>* flush_flag{nullptr}; /** This is only used in the case of Event::Flush **/
};
} // namespace detail

QUILL_END_NAMESPACE