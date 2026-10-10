/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/Filesystem.h"
#include "quill/core/QuillError.h"
#include "quill/core/TimeUtilities.h"
#include "quill/sinks/StreamSink.h"

#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <thread>

#if defined(_WIN32)
  #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif

  #if !defined(NOMINMAX)
    // Mingw already defines this, so no need to redefine
    #define NOMINMAX
  #endif

  #include <io.h>
  #include <share.h>
  #include <windows.h>
#else
  #include <fcntl.h>
  #include <unistd.h>
#endif

QUILL_BEGIN_NAMESPACE

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
  #pragma warning(push)
  #pragma warning(disable : 4996)
#endif

enum class FilenameAppendOption : uint8_t
{
  None,
  StartDate,
  StartDateTime,
  StartCustomTimestampFormat
};

/**
 * The FileSinkConfig class holds the configuration options for the FileSink
 */
class FileSinkConfig
{
public:
  /**
   * @brief Sets the append type for the file name.
   * Possible append types are: StartDate, StartDateTime or None.
   * When this option is set, the file name will be appended with the start date or date and time
   * timestamp of when the process started.
   *
   * For example:
   * application.log -> application_20230101.log  (StartDate)
   * application.log -> application_20230101_121020.log  (StartDateTime)
   *
   * @param value The append type to set. Valid options are Date and DateAndTime.
   * @param append_filename_format_pattern Specifies a custom `strftime` format pattern to use for the filename. This parameter is
   *                                       only applicable when `FilenameAppendOption::CustomDateTimeFormat` is selected
   */
  QUILL_ATTRIBUTE_COLD void set_filename_append_option(
    FilenameAppendOption value, std::string_view append_filename_format_pattern = std::string_view{})
  {
    _filename_append_option = value;

    if (_filename_append_option == FilenameAppendOption::StartCustomTimestampFormat)
    {
      if (append_filename_format_pattern.empty())
      {
        QUILL_THROW(QuillError{
          "The 'CustomDateTimeFormat' option was specified, but no format pattern was provided. "
          "Please set a valid strftime format pattern"});
      }

      _append_filename_format_pattern = append_filename_format_pattern;
    }
    else if (_filename_append_option == FilenameAppendOption::StartDateTime)
    {
      _append_filename_format_pattern = "_%Y%m%d_%H%M%S";
    }
    else if (_filename_append_option == FilenameAppendOption::StartDate)
    {
      _append_filename_format_pattern = "_%Y%m%d";
    }
  }

  /**
   * @brief Sets the timezone to use for time-based operations e.g. when appending the date to the
   * get_filename or when setting the logging pattern.
   * Valid options for the timezone are 'LocalTime' or 'GmtTime'
   * The default value is 'LocalTime'
   * @param time_zone The timezone to use for time-based operations.
   */
  QUILL_ATTRIBUTE_COLD void set_timezone(Timezone time_zone) { _time_zone = time_zone; }

  /**
   * @brief Sets whether fsync should be performed when flushing.
   * The default value is false.
   * @param value True to perform fsync, false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_fsync_enabled(bool value) { _fsync_enabled = value; }

  /**
   * @brief Sets the open mode for the file.
   * @param open_mode open mode for the file.
   */
  QUILL_ATTRIBUTE_COLD void set_open_mode(char open_mode) { _open_mode = open_mode; }

  /**
   * @brief Sets the open mode for the file.
   * @param open_mode open mode for the file.
   */
  QUILL_ATTRIBUTE_COLD void set_open_mode(std::string_view open_mode) { _open_mode = open_mode; }

  /**
   * @brief Sets the user-defined buffer size for fwrite operations.
   *
   * This function allows you to specify a custom buffer size for fwrite, improving efficiency
   * for file write operations.
   *
   * To disable custom buffering and revert to the default size, pass a value of 0.
   *
   * @note By default, a buffer size of 64 KB is used.
   * @param value Size of the buffer in bytes. If set to 0, the default buffer size will be used.
   */
  QUILL_ATTRIBUTE_COLD void set_write_buffer_size(size_t value)
  {
    _write_buffer_size = (value == 0) ? 0 : ((value < 4096) ? 4096 : value);
  }

  /**
   * Sets the minimum interval between `fsync` calls. This specifies the minimum time between
   * consecutive `fsync` operations but does not guarantee that `fsync` will be called exactly
   * at that interval.
   *
   * For example, if some messages are flushed to the log and `fsync` is skipped because it
   * was previously called, and no further messages are written to the file, `fsync` will not
   * be called even if the minimum interval has passed. This is because the previous call was
   * skipped due to the interval, and no new messages necessitate another `fsync` call.
   *
   * This feature is intended to mitigate concerns about frequent `fsync` calls potentially causing
   * disk wear.
   *
   * Note: This option is only applicable when `fsync` is enabled. By default, the value is 0,
   * which means that `fsync` will be called periodically by the backend worker thread when
   * messages are written to the file, irrespective of the interval.
   *
   * @param value The minimum interval, in milliseconds, between `fsync` calls.
   */
  QUILL_ATTRIBUTE_COLD void set_minimum_fsync_interval(std::chrono::milliseconds value)
  {
    _minimum_fsync_interval = value;
  }

  /**
   * @brief Sets custom pattern formatter options for this sink.
   *
   * By default, the logger's pattern formatter is used to format log messages.
   * This function allows overriding the default formatter with custom options for this specific
   * sink. If a custom formatter is provided, it will be used instead of the logger's formatter.
   *
   * @param options The custom pattern formatter options to use
   */
  QUILL_ATTRIBUTE_COLD void set_override_pattern_formatter_options(std::optional<PatternFormatterOptions> const& options)
  {
    _override_pattern_formatter_options = options;
  }

  /** Getters **/
  QUILL_NODISCARD bool fsync_enabled() const noexcept { return _fsync_enabled; }
  QUILL_NODISCARD Timezone timezone() const noexcept { return _time_zone; }
  QUILL_NODISCARD FilenameAppendOption filename_append_option() const noexcept
  {
    return _filename_append_option;
  }
  QUILL_NODISCARD std::string const& append_filename_format_pattern() const noexcept
  {
    return _append_filename_format_pattern;
  }
  QUILL_NODISCARD std::string const& open_mode() const noexcept { return _open_mode; }
  QUILL_NODISCARD size_t write_buffer_size() const noexcept { return _write_buffer_size; }
  QUILL_NODISCARD std::chrono::milliseconds minimum_fsync_interval() const noexcept
  {
    return _minimum_fsync_interval;
  }
  QUILL_NODISCARD std::optional<PatternFormatterOptions> const& override_pattern_formatter_options() const noexcept
  {
    return _override_pattern_formatter_options;
  }

private:
  std::string _open_mode{'a'};
  std::string _append_filename_format_pattern;
  size_t _write_buffer_size{64 * 1024}; // Default size 64k
  std::chrono::milliseconds _minimum_fsync_interval{0};
  std::optional<PatternFormatterOptions> _override_pattern_formatter_options;
  Timezone _time_zone{Timezone::LocalTime};
  FilenameAppendOption _filename_append_option{FilenameAppendOption::None};
  bool _fsync_enabled{false};
};

/**
 * FileSink
 * Writes the log messages to a file
 */
class FileSink : public StreamSink
{
public:
  /**
   * Construct a FileSink object.
   * This constructor will always attempt to open the given file.
   * @param filename Path to the file to be opened.
   * @param config Configuration for the FileSink.
   * @param file_event_notifier Notifies on file events.
   * @param do_fopen If false, the file will not be opened.
   * @param start_time start time
   */
  explicit FileSink(fs::path const& filename, FileSinkConfig const& config = FileSinkConfig{},
                    FileEventNotifier file_event_notifier = FileEventNotifier{}, bool do_fopen = true,
                    std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now())
    : StreamSink(_get_updated_filename_with_appended_datetime(filename, config.filename_append_option(),
                                                              config.append_filename_format_pattern(),
                                                              config.timezone(), start_time),
                 nullptr, config.override_pattern_formatter_options(), std::move(file_event_notifier)),
      _config(config)
  {
    if (!_config.fsync_enabled() && (_config.minimum_fsync_interval().count() != 0))
    {
      QUILL_THROW(
        QuillError{"Cannot set a non-zero minimum fsync interval when fsync is disabled."});
    }

    if (do_fopen)
    {
      open_file(_filename, _config.open_mode());
    }
  }

  ~FileSink() override { close_file(); }

  /**
   * Flushes the stream and optionally fsyncs it.
   */
  QUILL_ATTRIBUTE_HOT void flush_sink() override
  {
    if (!_write_occurred || !_file)
    {
      // Check here because StreamSink::flush() will set _write_occurred to false
      return;
    }

    StreamSink::flush_sink();

    if (_config.fsync_enabled())
    {
      fsync_file();
    }

    if (!fs::exists(_filename))
    {
      // after flushing the file we can check if the file still exists. If not we reopen it.
      // This can happen if a user deletes a file while the application is running
      close_file();

      // now reopen the file for writing again, it will be a new file
      open_file(_filename, "w");
    }
  }

protected:
  /**
   * Format a datetime string.
   * @param timestamp_ns Timestamp in nanoseconds.
   * @param time_zone Timezone to use.
   * @param append_format_pattern strftime pattern
   * @return Formatted datetime string.
   */
  QUILL_NODISCARD static std::string format_datetime_string(uint64_t timestamp_ns, Timezone time_zone,
                                                            std::string const& append_format_pattern)
  {
    // convert to seconds
    auto const time_now = static_cast<time_t>(timestamp_ns / 1000000000);
    tm now_tm;

    if (time_zone == Timezone::GmtTime)
    {
      detail::gmtime_rs(&time_now, &now_tm);
    }
    else
    {
      detail::localtime_rs(&time_now, &now_tm);
    }

    // Construct the string
    static constexpr size_t buffer_size{128};
    char buffer[buffer_size];
    std::strftime(buffer, buffer_size, append_format_pattern.data(), &now_tm);

    return std::string{buffer};
  }

  /**
   * Extract stem and extension from a filename.
   * @param filename Path to the file.
   * @return Pair containing stem and extension.
   */
  QUILL_NODISCARD static std::pair<std::string, std::string> extract_stem_and_extension(fs::path const& filename) noexcept
  {
    // filename and extension
    return std::make_pair((filename.parent_path() / filename.stem()).string(), filename.extension().string());
  }

  /**
   * Append date and/or time to a filename.
   * @param filename Path to the file.
   * @param append_filename_format_pattern strftime pattern
   * @param time_zone Timezone to use.
   * @param timestamp Timestamp to use.
   * @return Updated filename.
   */
  QUILL_NODISCARD static fs::path append_datetime_to_filename(fs::path const& filename,
                                                              std::string const& append_filename_format_pattern,
                                                              Timezone time_zone,
                                                              std::chrono::system_clock::time_point timestamp) noexcept
  {
    // Get base file and extension
    auto const [stem, ext] = extract_stem_and_extension(filename);

    // Get the time now as tm from user or default to now
    uint64_t const timestamp_ns = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()).count());

    // Construct a filename
    return stem + format_datetime_string(timestamp_ns, time_zone, append_filename_format_pattern) + ext;
  }

  /**
   * Open a file.
   * @param filename Path to the file.
   * @param mode File open mode.
   */
  void open_file(fs::path const& filename, std::string const& mode)
  {
    if (_file_event_notifier.before_open)
    {
      _file_event_notifier.before_open(filename);
    }

    // Retry file open to handle transient failures (e.g., antivirus locking on Windows)
    constexpr int max_retries = 3;
    constexpr int retry_delay_ms = 200;

    for (int attempt = 0; attempt < max_retries; ++attempt)
    {
#if defined(_WIN32)
      // Use _fsopen with _SH_DENYNO to allow other processes to read the file
      // while we're writing (e.g., tail, monitoring tools)
      _file = ::_fsopen(filename.string().data(), mode.data(), _SH_DENYNO);

      if (_file)
      {
        // Prevent child processes from inheriting this file handle
        auto file_handle = reinterpret_cast<HANDLE>(::_get_osfhandle(::_fileno(_file)));
        if (!::SetHandleInformation(file_handle, HANDLE_FLAG_INHERIT, 0))
        {
          ::fclose(_file);
          _file = nullptr;
        }
      }
#else
      // Unix: use open() with O_CLOEXEC to prevent child processes from inheriting the FD
      int flags = O_CREAT | O_WRONLY | O_CLOEXEC;
      flags |= (mode == "w") ? O_TRUNC : O_APPEND;

      int fd = ::open(filename.string().data(), flags, 0644);
      if (fd != -1)
      {
        _file = ::fdopen(fd, mode.data());
        if (!_file)
        {
          ::close(fd);
        }
      }
#endif

      if (_file)
      {
        break; // Success
      }

      // Retry after delay if not the last attempt
      if (attempt < max_retries - 1)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds{retry_delay_ms});
      }
    }

    if (!_file)
    {
      QUILL_THROW(QuillError{std::string{"fopen failed after "} + std::to_string(max_retries) +
                             " attempts, path: " + filename.string() + " mode: " + mode +
                             " errno: " + std::to_string(errno) + " error: " + std::strerror(errno)});
    }

    if (_config.write_buffer_size() != 0)
    {
      _write_buffer = std::make_unique<char[]>(_config.write_buffer_size());

      if (setvbuf(_file, _write_buffer.get(), _IOFBF, _config.write_buffer_size()) != 0)
      {
        QUILL_THROW(QuillError{std::string{"setvbuf failed error: "} + std::strerror(errno)});
      }
    }

    if (_file_event_notifier.after_open)
    {
      _file_event_notifier.after_open(filename, _file);
    }
  }

  /**
   * Close the file.
   */
  void close_file()
  {
    if (!_file)
    {
      return;
    }

    if (_file_event_notifier.before_close)
    {
      _file_event_notifier.before_close(_filename, _file);
    }

    fclose(_file);
    _file = nullptr;

    if (_file_event_notifier.after_close)
    {
      _file_event_notifier.after_close(_filename);
    }
  }

  /**
   * Fsync the file descriptor.
   */
  void fsync_file(bool force_fsync = false) noexcept
  {
    if (!force_fsync)
    {
      auto const now = std::chrono::steady_clock::now();
      if ((now - _last_fsync_timestamp) < _config.minimum_fsync_interval())
      {
        return;
      }
      _last_fsync_timestamp = now;
    }

#ifdef _WIN32
    FlushFileBuffers(reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(_file))));
#else
    ::fsync(fileno(_file));
#endif
  }

private:
  /**
   * Get the filename with appended date and/or time.
   * @param filename Path to the file.
   * @param append_to_filename_option Append option.
   * @param append_filename_format_pattern Append filename format option.
   * @param time_zone Timezone to use.
   * @param timestamp timestamp
   * @return Updated filename.
   */
  QUILL_NODISCARD static fs::path _get_updated_filename_with_appended_datetime(
    fs::path const& filename, FilenameAppendOption append_to_filename_option,
    std::string const& append_filename_format_pattern, Timezone time_zone,
    std::chrono::system_clock::time_point timestamp)
  {
    if ((append_to_filename_option == FilenameAppendOption::None) || (filename == "/dev/null"))
    {
      return filename;
    }

    if ((append_to_filename_option == FilenameAppendOption::StartCustomTimestampFormat) ||
        (append_to_filename_option == FilenameAppendOption::StartDate) ||
        (append_to_filename_option == FilenameAppendOption::StartDateTime))
    {
      return append_datetime_to_filename(filename, append_filename_format_pattern, time_zone, timestamp);
    }

    return fs::path{};
  }

protected:
  FileSinkConfig _config;
  std::chrono::steady_clock::time_point _last_fsync_timestamp{};
  std::unique_ptr<char[]> _write_buffer;
};

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
#pragma warning(pop)
#endif

QUILL_END_NAMESPACE