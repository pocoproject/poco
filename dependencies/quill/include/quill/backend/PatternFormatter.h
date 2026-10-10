/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/TimestampFormatter.h"
#include "quill/bundled/fmt/base.h"
#include "quill/bundled/fmt/format.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/MacroMetadata.h"
#include "quill/core/PatternFormatterOptions.h"
#include "quill/core/QuillError.h"

#include <array>
#include <bitset>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

class PatternFormatter
{
  /** Public classes **/
public:
  /**
   * Stores the precision of the timestamp
   */
  enum class TimestampPrecision : uint8_t
  {
    None,
    MilliSeconds,
    MicroSeconds,
    NanoSeconds
  };

  enum Attribute : uint8_t
  {
    Time = 0,
    FileName,
    CallerFunction,
    LogLevel,
    LogLevelShortCode,
    LineNumber,
    Logger,
    FullPath,
    ThreadId,
    ThreadName,
    ProcessId,
    SourceLocation,
    ShortSourceLocation,
    Message,
    Tags,
    NamedArgs,
    ATTR_NR_ITEMS
  };

  /** Main PatternFormatter class **/
public:
  /**
   * Constructor for a PatternFormatter with custom formatting options.
   *
   * @param options The PatternFormatterOptions object containing the formatting configuration.
   *                @see PatternFormatterOptions for detailed information on available options.
   *
   * @throws std::invalid_argument if the format string in options is invalid
   */
  explicit PatternFormatter(PatternFormatterOptions options)
    : _options(std::move(options)),
      _timestamp_formatter(_options.timestamp_pattern, _options.timestamp_timezone)
  {
    _set_pattern();
  }

  /***/
  PatternFormatter(PatternFormatter const& other) = delete;
  PatternFormatter& operator=(PatternFormatter const& other) = delete;

  /***/
  PatternFormatter& operator=(PatternFormatter&& other) noexcept = default;
  PatternFormatter(PatternFormatter&& other) noexcept = default;

  /***/
  ~PatternFormatter() = default;

  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::string_view format(
    uint64_t timestamp, std::string_view thread_id, std::string_view thread_name,
    std::string_view process_id, std::string_view logger, std::string_view log_level_description,
    std::string_view log_level_short_code, MacroMetadata const& log_statement_metadata,
    std::vector<std::pair<std::string, std::string>> const* named_args, std::string_view log_msg)
  {
    if (_options.format_pattern.empty())
    {
      // No formatting is needed when the format pattern is empty.
      // For example, in JsonFileSink, we can retrieve the MacroMetadata and the named arguments as
      // key-value pairs, but we do not need to format the log statement.
      return std::string_view{};
    }

    // clear out the existing buffer
    _formatted_log_message_buffer.clear();

    if (QUILL_UNLIKELY(log_msg.empty()))
    {
      // Process an empty message
      return _format(timestamp, thread_id, thread_name, process_id, logger, log_level_description,
                     log_level_short_code, log_statement_metadata, named_args, log_msg);
    }

    std::string_view formatted_log_msg;

    // Check if we need to handle multi-line formatting
    if (_options.add_metadata_to_multi_line_logs && (!named_args || named_args->empty()))
    {
      // multi line metadata only supported when named_args are not used
      size_t start = 0;

      while (start < log_msg.size())
      {
        size_t const end = log_msg.find_first_of('\n', start);

        if (end == std::string_view::npos)
        {
          // Handle the last line or a single line without a newline
          formatted_log_msg =
            _format(timestamp, thread_id, thread_name, process_id, logger, log_level_description,
                    log_level_short_code, log_statement_metadata, named_args,
                    std::string_view(log_msg.data() + start, log_msg.size() - start));
          break;
        }

        // Write the current line
        size_t line_length = end - start;
        if (_options.pattern_suffix != '\n')
        {
          // When suffix is not '\n', include the newline character in the message
          line_length++;
        }
        
        formatted_log_msg = _format(timestamp, thread_id, thread_name, process_id, logger,
                                    log_level_description, log_level_short_code, log_statement_metadata,
                                    named_args, std::string_view(log_msg.data() + start, line_length));
        start = end + 1;
      }
    }
    else
    {
      // Use the regular format method for single-line messages
      QUILL_ASSERT(
        !log_msg.empty(),
        "log_msg should not be empty, already checked earlier in PatternFormatter::format()");
      size_t log_message_size = log_msg.size();

      if (_options.pattern_suffix == '\n' && log_msg[log_msg.size() - 1] == '\n')
      {
        // if the log_message ends with \n we exclude it (only when using newline suffix)
        log_message_size = log_msg.size() - 1;
      }

      formatted_log_msg = _format(timestamp, thread_id, thread_name, process_id, logger,
                                  log_level_description, log_level_short_code, log_statement_metadata,
                                  named_args, std::string_view{log_msg.data(), log_message_size});
    }

    return formatted_log_msg;
  }

  /***/
  QUILL_NODISCARD PatternFormatterOptions const& get_options() const noexcept { return _options; }

protected:
  /***/
  QUILL_NODISCARD static std::string_view _process_source_location_path(std::string_view source_location,
                                                                        std::string const& strip_prefix,
                                                                        bool remove_relative_paths)
  {
    std::string_view result = source_location;

    // First, handle removal of relative paths if requested
    if (remove_relative_paths)
    {
      // Remove any relative paths (e.g., relative paths can appear when using a mounted volume under docker)

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
      static constexpr std::string_view relative_path = "..\\";
#else
      static constexpr std::string_view relative_path = "../";
#endif

      if (size_t n = result.rfind(relative_path); n != std::string_view::npos)
      {
        result = result.substr(n + relative_path.size());
      }
    }

    // Then handle prefix stripping
    if (!strip_prefix.empty())
    {
      // Find the last occurrence of the prefix in the path
      size_t prefix_pos = result.rfind(strip_prefix);

      if (prefix_pos != std::string_view::npos)
      {
        size_t after_prefix_pos = prefix_pos + strip_prefix.size();

        // If the prefix doesn't end with a separator and there is a character after the prefix
        // and that character is a separator, skip it as well
        if (after_prefix_pos < result.length() && result[after_prefix_pos] == detail::PATH_PREFERRED_SEPARATOR)
        {
          after_prefix_pos++;
        }

        return result.substr(after_prefix_pos);
      }
      // Prefix not found, use the full path
    }

    // No prefix set or prefix not found, use the full path
    return result;
  }

private:
  void _set_pattern()
  {
    // the order we pass the arguments here must match with the order of Attribute enum
    using namespace fmtquill::literals;
    std::tie(_fmt_format, _order_index) = _generate_fmt_format_string(
      _is_set_in_pattern, _options.format_pattern, "time"_a = "", "file_name"_a = "",
      "caller_function"_a = "", "log_level"_a = "", "log_level_short_code"_a = "",
      "line_number"_a = "", "logger"_a = "", "full_path"_a = "", "thread_id"_a = "",
      "thread_name"_a = "", "process_id"_a = "", "source_location"_a = "",
      "short_source_location"_a = "", "message"_a = "", "tags"_a = "", "named_args"_a = "");

    _set_arg<Attribute::Time>(std::string_view("time"));
    _set_arg<Attribute::FileName>(std::string_view("file_name"));
    _set_arg<Attribute::CallerFunction>(std::string_view("caller_function"));
    _set_arg<Attribute::LogLevel>(std::string_view("log_level"));
    _set_arg<Attribute::LogLevelShortCode>(std::string_view("log_level_short_code"));
    _set_arg<Attribute::LineNumber>("line_number");
    _set_arg<Attribute::Logger>(std::string_view("logger"));
    _set_arg<Attribute::FullPath>(std::string_view("full_path"));
    _set_arg<Attribute::ThreadId>(std::string_view("thread_id"));
    _set_arg<Attribute::ThreadName>(std::string_view("thread_name"));
    _set_arg<Attribute::ProcessId>(std::string_view("process_id"));
    _set_arg<Attribute::SourceLocation>("source_location");
    _set_arg<Attribute::ShortSourceLocation>("short_source_location");
    _set_arg<Attribute::Message>(std::string_view("message"));
    _set_arg<Attribute::Tags>(std::string_view("tags"));
    _set_arg<Attribute::NamedArgs>(std::string_view("named_args"));
  }

  /***/
  template <size_t I, typename T>
  void _set_arg(T const& arg)
  {
    _args[_order_index[I]] = arg;
  }

  template <size_t I, typename T>
  void _set_arg_val(T const& arg)
  {
    fmtquill::detail::value<fmtquill::format_context>& value_ =
      *(reinterpret_cast<fmtquill::detail::value<fmtquill::format_context>*>(
        std::addressof(_args[_order_index[I]])));

    value_ = fmtquill::detail::value<fmtquill::format_context>(arg);
  }

  /***/
  PatternFormatter::Attribute static _attribute_from_string(std::string const& attribute_name)
  {
    // don't make this static as it breaks on windows with atexit when backend worker stops
    std::unordered_map<std::string, PatternFormatter::Attribute> const attr_map = {
      {"time", PatternFormatter::Attribute::Time},
      {"file_name", PatternFormatter::Attribute::FileName},
      {"caller_function", PatternFormatter::Attribute::CallerFunction},
      {"log_level", PatternFormatter::Attribute::LogLevel},
      {"log_level_short_code", PatternFormatter::Attribute::LogLevelShortCode},
      {"line_number", PatternFormatter::Attribute::LineNumber},
      {"logger", PatternFormatter::Attribute::Logger},
      {"full_path", PatternFormatter::Attribute::FullPath},
      {"thread_id", PatternFormatter::Attribute::ThreadId},
      {"thread_name", PatternFormatter::Attribute::ThreadName},
      {"process_id", PatternFormatter::Attribute::ProcessId},
      {"source_location", PatternFormatter::Attribute::SourceLocation},
      {"short_source_location", PatternFormatter::Attribute::ShortSourceLocation},
      {"message", PatternFormatter::Attribute::Message},
      {"tags", PatternFormatter::Attribute::Tags},
      {"named_args", PatternFormatter::Attribute::NamedArgs}};

    auto const search = attr_map.find(attribute_name);

    if (QUILL_UNLIKELY(search == attr_map.cend()))
    {
      QUILL_THROW(QuillError{
        std::string{"Attribute enum value does not exist for attribute with name " + attribute_name}});
    }

    return search->second;
  }

  /***/
  template <size_t, size_t>
  constexpr void _store_named_args(std::array<fmtquill::detail::named_arg_info<char>, PatternFormatter::Attribute::ATTR_NR_ITEMS>&)
  {
  }

  /***/
  template <size_t Idx, size_t NamedIdx, typename Arg, typename... Args>
  constexpr void _store_named_args(
    std::array<fmtquill::detail::named_arg_info<char>, PatternFormatter::Attribute::ATTR_NR_ITEMS>& named_args_store,
    Arg const& arg, Args const&... args)
  {
    named_args_store[NamedIdx] = {arg.name, Idx};
    _store_named_args<Idx + 1, NamedIdx + 1>(named_args_store, args...);
  }

  /**
   * Convert the pattern to fmt format string and also populate the order index array
   * e.g. given :
   *   "%(time) [%(thread_id)] %(file_name):%(line_number) %(log_level:<12) %(logger) - "
   *
   * is changed to :
   *  {} [{}] {}:{} {:<12} {} -
   *
   *  with a order index of :
   *  i: 0 order idx[i] is: 0 - %(time)
   *  i: 1 order idx[i] is: 2 - %(file_name)
   *  i: 2 order idx[i] is: 10 - empty
   *  i: 3 order idx[i] is: 4 - %(log_level)
   *  i: 4 order idx[i] is: 10 - empty
   *  i: 5 order idx[i] is: 3 - %(line_number)
   *  i: 6 order idx[i] is: 5 - %(logger)
   *  i: 7 order idx[i] is: 10 - empty
   *  i: 8 order idx[i] is: 1 - %(thread_id)
   *  i: 9 order idx[i] is: 10 - empty
   *  i: 10 order idx[i] is: 10 - empty
   * @tparam Args Args
   * @param is_set_in_pattern is set in pattern
   * @param pattern pattern
   * @param args args
   * @return process_id pattern
   */
  template <typename... Args>
  QUILL_NODISCARD std::pair<std::string, std::array<size_t, PatternFormatter::Attribute::ATTR_NR_ITEMS>> _generate_fmt_format_string(
    std::bitset<PatternFormatter::Attribute::ATTR_NR_ITEMS>& is_set_in_pattern, std::string pattern,
    Args const&... args)
  {
    // Attribute enum and the args we are passing here must be in sync
    static_assert(PatternFormatter::Attribute::ATTR_NR_ITEMS == sizeof...(Args));

    if (_options.pattern_suffix != PatternFormatterOptions::NO_SUFFIX)
    {
      pattern += _options.pattern_suffix;
    }

    std::array<size_t, PatternFormatter::Attribute::ATTR_NR_ITEMS> order_index{};
    order_index.fill(PatternFormatter::Attribute::ATTR_NR_ITEMS - 1);

    std::array<fmtquill::detail::named_arg_info<char>, PatternFormatter::Attribute::ATTR_NR_ITEMS> named_args{};
    _store_named_args<0, 0>(named_args, args...);
    uint8_t arg_idx = 0;

    // we will replace all %(....) with {} to construct a string to pass to fmt library
    size_t arg_identifier_pos = pattern.find_first_of('%');
    while (arg_identifier_pos != std::string::npos)
    {
      if (size_t const open_paren_pos = pattern.find_first_of('(', arg_identifier_pos);
          open_paren_pos != std::string::npos && (open_paren_pos - arg_identifier_pos) == 1)
      {
        // if we found '%(' we have a matching pattern and we now need to get the closed paren
        size_t const closed_paren_pos = pattern.find_first_of(')', open_paren_pos);

        if (closed_paren_pos == std::string::npos)
        {
          QUILL_THROW(QuillError{"Invalid format pattern"});
        }

        // We have everything, get the substring, this time including '%( )'
        std::string attr = pattern.substr(arg_identifier_pos, (closed_paren_pos + 1) - arg_identifier_pos);

        // find any user format specifiers
        size_t const pos = attr.find(':');
        std::string attr_name;

        if (pos != std::string::npos)
        {
          // we found user format specifiers that we want to keep.
          // e.g. %(short_source_location:<32)

          // Get only the format specifier
          // e.g. :<32
          std::string custom_format_specifier = attr.substr(pos);
          custom_format_specifier.pop_back(); // remove the ")"

          // replace with the pattern with the correct value
          std::string value;
          value += "{";
          value += custom_format_specifier;
          value += "}";

          // e.g. {:<32}
          pattern.replace(arg_identifier_pos, attr.length(), value);

          // Get the part that is the named argument
          // e.g. short_source_location
          attr_name = attr.substr(2, pos - 2);
        }
        else
        {
          // Make the replacement.
          pattern.replace(arg_identifier_pos, attr.length(), "{}");

          // Get the part that is the named argument
          // e.g. short_source_location
          attr.pop_back(); // remove the ")"

          attr_name = attr.substr(2, attr.size());
        }

        // reorder
        int id = -1;

        for (size_t i = 0; i < PatternFormatter::Attribute::ATTR_NR_ITEMS; ++i)
        {
          if (named_args[i].name == attr_name)
          {
            id = named_args[i].id;
            break;
          }
        }

        if (id < 0)
        {
          QUILL_THROW(QuillError{"Invalid format pattern, attribute with name \"" + attr_name + "\" is invalid"});
        }

        order_index[static_cast<size_t>(id)] = arg_idx++;

        // Also set the value as used in the pattern in our bitset for lazy evaluation
        PatternFormatter::Attribute const attr_enum_value = _attribute_from_string(attr_name);
        is_set_in_pattern.set(attr_enum_value);

        // Look for the next pattern to replace
        arg_identifier_pos = pattern.find_first_of('%');
      }
      else
      {
        // search for the next '%'
        arg_identifier_pos = pattern.find_first_of('%', arg_identifier_pos + 1);
      }
    }

    return std::make_pair(pattern, order_index);
  }

  /***/
  QUILL_ATTRIBUTE_HOT std::string_view _format(
    uint64_t timestamp, std::string_view thread_id, std::string_view thread_name,
    std::string_view process_id, std::string_view logger, std::string_view log_level_description,
    std::string_view log_level_short_code, MacroMetadata const& log_statement_metadata,
    std::vector<std::pair<std::string, std::string>> const* named_args, std::string_view log_msg)
  {
    if (_is_set_in_pattern[Attribute::Time])
    {
      _set_arg_val<Attribute::Time>(_timestamp_formatter.format_timestamp(std::chrono::nanoseconds{timestamp}));
    }

    if (_is_set_in_pattern[Attribute::FileName])
    {
      _set_arg_val<Attribute::FileName>(log_statement_metadata.file_name());
    }

    if (_is_set_in_pattern[Attribute::CallerFunction])
    {
      std::string_view const function_name = _options.process_function_name
        ? _options.process_function_name(log_statement_metadata.caller_function())
        : std::string_view{log_statement_metadata.caller_function()};

      _set_arg_val<Attribute::CallerFunction>(function_name);
    }

    if (_is_set_in_pattern[Attribute::LogLevel])
    {
      _set_arg_val<Attribute::LogLevel>(log_level_description);
    }

    if (_is_set_in_pattern[Attribute::LogLevelShortCode])
    {
      _set_arg_val<Attribute::LogLevelShortCode>(log_level_short_code);
    }

    if (_is_set_in_pattern[Attribute::LineNumber])
    {
      _set_arg_val<Attribute::LineNumber>(log_statement_metadata.line());
    }

    if (_is_set_in_pattern[Attribute::Logger])
    {
      _set_arg_val<Attribute::Logger>(logger);
    }

    if (_is_set_in_pattern[Attribute::FullPath])
    {
      _set_arg_val<Attribute::FullPath>(log_statement_metadata.full_path());
    }

    if (_is_set_in_pattern[Attribute::ThreadId])
    {
      _set_arg_val<Attribute::ThreadId>(thread_id);
    }

    if (_is_set_in_pattern[Attribute::ThreadName])
    {
      _set_arg_val<Attribute::ThreadName>(thread_name);
    }

    if (_is_set_in_pattern[Attribute::ProcessId])
    {
      _set_arg_val<Attribute::ProcessId>(process_id);
    }

    if (_is_set_in_pattern[Attribute::SourceLocation])
    {
      _set_arg_val<Attribute::SourceLocation>(_process_source_location_path(
        log_statement_metadata.source_location(), _options.source_location_path_strip_prefix,
        _options.source_location_remove_relative_paths));
      ;
    }

    if (_is_set_in_pattern[Attribute::ShortSourceLocation])
    {
      _set_arg_val<Attribute::ShortSourceLocation>(log_statement_metadata.short_source_location());
    }

    if (_is_set_in_pattern[Attribute::NamedArgs])
    {
      _formatted_named_args_buffer.clear();

      if (named_args)
      {
        for (size_t i = 0; i < named_args->size(); ++i)
        {
          _formatted_named_args_buffer.append((*named_args)[i].first);
          _formatted_named_args_buffer.append(std::string_view{": "});
          _formatted_named_args_buffer.append((*named_args)[i].second);

          if (i != named_args->size() - 1)
          {
            _formatted_named_args_buffer.append(std::string_view{", "});
          }
        }
      }

      _set_arg_val<Attribute::NamedArgs>(
        std::string_view{_formatted_named_args_buffer.data(), _formatted_named_args_buffer.size()});
    }

    if (_is_set_in_pattern[Attribute::Tags])
    {
      if (log_statement_metadata.tags())
      {
        _set_arg_val<Attribute::Tags>(std::string_view{log_statement_metadata.tags()});
      }
      else
      {
        _set_arg_val<Attribute::Tags>(std::string_view{});
      }
    }

    _set_arg_val<Attribute::Message>(log_msg);

    fmtquill::vformat_to(std::back_inserter(_formatted_log_message_buffer), _fmt_format,
                         fmtquill::basic_format_args(_args.data(), static_cast<int>(_args.size())));

    return std::string_view{_formatted_log_message_buffer.data(), _formatted_log_message_buffer.size()};
  }

private:
  PatternFormatterOptions _options;
  std::string _fmt_format;

  /** Each named argument in the format_pattern is mapped in order to this array **/
  std::array<size_t, Attribute::ATTR_NR_ITEMS> _order_index{};
  std::array<fmtquill::basic_format_arg<fmtquill::format_context>, Attribute::ATTR_NR_ITEMS> _args{};
  std::bitset<Attribute::ATTR_NR_ITEMS> _is_set_in_pattern;

  /** class responsible for formatting the timestamp */
  detail::TimestampFormatter _timestamp_formatter;

  /** The buffer where we store each formatted string, also stored as class member to avoid
   * re-allocations **/
  fmtquill::basic_memory_buffer<char, 512> _formatted_log_message_buffer;
  fmtquill::basic_memory_buffer<char, 512> _formatted_named_args_buffer;
};

QUILL_END_NAMESPACE