/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Filesystem.h"
#include "quill/core/LogLevel.h"
#include "quill/core/QuillError.h"
#include "quill/sinks/Sink.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#if defined(_WIN32)
  #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif

  #if !defined(NOMINMAX)
    // Mingw already defines this, so no need to redefine
    #define NOMINMAX
  #endif

  #include <io.h>
  #include <windows.h>
#endif

QUILL_BEGIN_NAMESPACE

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
  #pragma warning(push)
  #pragma warning(disable : 4996)
#endif

/** Forward Declaration **/
class MacroMetadata;

/**
 * @brief Notifies on file events by calling the appropriate callback, the callback is executed on
 * the backend worker thread
 */
struct FileEventNotifier
{
  std::function<void(fs::path const& file_path)> before_open;
  std::function<void(fs::path const& file_path, FILE* f)> after_open;
  std::function<void(fs::path const& file_path, FILE* f)> before_close;
  std::function<void(fs::path const& file_path)> after_close;
  std::function<std::string(std::string_view message)> before_write;
};

/**
 * @brief StreamSink class for handling log messages
 */
class StreamSink : public Sink
{
public:
  /**
   * @brief Constructor for StreamSink
   * @param stream The stream type (stdout, stderr, or file)
   * @param override_pattern_formatter_options override the logger pattern formatter
   * @param file File pointer for file-based stream
   * @param file_event_notifier Notifies on file events
   * @throws QuillError if an invalid parameter is provided
   */
  explicit StreamSink(fs::path stream, FILE* file = nullptr,
                      std::optional<PatternFormatterOptions> const& override_pattern_formatter_options = std::nullopt,
                      FileEventNotifier file_event_notifier = FileEventNotifier{})
    : Sink(override_pattern_formatter_options),
      _filename(std::move(stream)),
      _file(file),
      _file_event_notifier(std::move(file_event_notifier))
  {
    // reserve stdout and stderr as filenames
    if (_filename == std::string{"stdout"})
    {
      _file = stdout;
    }
    else if (_filename == std::string{"stderr"})
    {
      _file = stderr;
    }
    else if (_filename == std::string{"/dev/null"})
    {
      _is_null = true;
    }
    else
    {
      // first attempt to create any non-existing directories
      std::error_code ec;
      fs::path parent_path;

      if (!_filename.parent_path().empty())
      {
        parent_path = _filename.parent_path();

        // The call to fs::status is necessary due to a known issue in GCC versions 8.3.0 to 9.4.0.
        // In these versions, fs::create_directories(path, ec) internally uses
        // fs::symlink_status(path, ec) instead of fs::status(path, ec) for checking the path.
        // This causes a problem because fs::symlink_status does not follow the symlink to the
        // target directory. As a result,  it fails the is_directory() check but still indicates
        // that the path exists, leading to a not_a_directory exception being set in the error code
        auto const st = fs::status(parent_path, ec);
        if (!is_directory(st))
        {
          fs::create_directories(parent_path, ec);
          if (ec)
          {
            // use .string() to also support experimental fs
            QUILL_THROW(QuillError{std::string{"cannot create directories for path "} +
                                   parent_path.string() + std::string{" - error: "} + ec.message()});
          }
        }
      }
      else
      {
        parent_path = fs::current_path();
      }

      // convert the parent path to an absolute path
      fs::path const canonical_path = fs::canonical(parent_path, ec);

      if (ec)
      {
        // use .string() to also support experimental fs
        QUILL_THROW(QuillError{std::string{"cannot create canonical path for path "} +
                               parent_path.string() + std::string{" - error: "} + ec.message()});
      }

      // finally replace the given filename's parent_path with the equivalent canonical path
      _filename = canonical_path / _filename.filename();
    }
  }

  ~StreamSink() override = default;

  /**
   * @brief Writes a formatted log message to the stream
   */
  QUILL_ATTRIBUTE_HOT void write_log(MacroMetadata const* /* log_metadata */,
                                     uint64_t /* log_timestamp */, std::string_view /* thread_id */,
                                     std::string_view /* thread_name */, std::string const& /* process_id */,
                                     std::string_view /* logger_name */, LogLevel /* log_level */,
                                     std::string_view /* log_level_description */,
                                     std::string_view /* log_level_short_code */,
                                     std::vector<std::pair<std::string, std::string>> const* /* named_args */,
                                     std::string_view /* log_message */, std::string_view log_statement) override
  {
    if (QUILL_UNLIKELY(!_file))
    {
      // FileSink::flush() tries to re-open a deleted file and if it fails _file can be null
      return;
    }

    if (_file_event_notifier.before_write)
    {
      std::string const user_log_statement = _file_event_notifier.before_write(log_statement);
      safe_fwrite(user_log_statement.data(), sizeof(char), user_log_statement.size(), _file);
      _file_size += user_log_statement.size();
    }
    else
    {
      safe_fwrite(log_statement.data(), sizeof(char), log_statement.size(), _file);
      _file_size += log_statement.size();
    }

    _write_occurred = true;
  }

  /**
   * Flushes the stream
   */
  QUILL_ATTRIBUTE_HOT void flush_sink() override
  {
    if (!_write_occurred || !_file)
    {
      return;
    }

    flush();
  }

  /**
   * @brief Returns the name of the file
   * @return The name of the file
   */
  QUILL_NODISCARD virtual fs::path const& get_filename() const noexcept { return _filename; }

  /**
   * @brief Checks if the stream is null
   * @return True if the stream is null, false otherwise
   */
  QUILL_NODISCARD bool is_null() const noexcept { return _is_null; }

  /**
   * @brief Writes data safely to the stream
   * @param ptr Pointer to the data to be written
   * @param size Size of each element to be written
   * @param count Number of elements to write
   * @param stream The stream to write to
   */
  QUILL_ATTRIBUTE_HOT static void safe_fwrite(void const* ptr, size_t size, size_t count, FILE* stream)
  {
    size_t const total_bytes = size * count;
    size_t bytes_written = 0;

    while (bytes_written < total_bytes)
    {
      auto const* current_ptr = static_cast<char const*>(ptr) + bytes_written;
      size_t const remaining = total_bytes - bytes_written;

#if defined(_WIN32)
      // On Windows, using fwrite to non-binary stream (stdout/stderr) results in \r\r\n
      // Instead, use WriteFile for console streams
      if ((stream == stdout) || (stream == stderr))
      {
        HANDLE const handle = reinterpret_cast<HANDLE>(::_get_osfhandle(::_fileno(stream)));

        if (QUILL_LIKELY(handle != INVALID_HANDLE_VALUE))
        {
          auto const total_bytes_remaining = static_cast<DWORD>(remaining);
          DWORD bytes_written_this_call = 0;

          if (QUILL_UNLIKELY((::WriteFile(handle, current_ptr, total_bytes_remaining,
                                          &bytes_written_this_call, nullptr) == 0) ||
                             (bytes_written_this_call != total_bytes_remaining)))
          {
            QUILL_THROW(QuillError{std::string{"WriteFile failed. GetLastError: "} +
                                   std::to_string(::GetLastError())});
          }

          bytes_written += bytes_written_this_call;
          continue;
        }

        // Fall through to fwrite if the handle is invalid (e.g., no console attached)
      }
#endif

      size_t const written = std::fwrite(current_ptr, 1, remaining, stream);

      if (QUILL_UNLIKELY(written < remaining))
      {
        // Partial write or error
        if (ferror(stream))
        {
          int const saved_errno = errno;
          std::clearerr(stream); // Reset error state
          QUILL_THROW(QuillError{std::string{"fwrite failed errno: "} + std::to_string(saved_errno) +
                                 " error: " + std::strerror(saved_errno)});
        }

        if (written == 0)
        {
          // Zero bytes written without error is unusual - treat as fatal to avoid infinite loop
          QUILL_THROW(
            QuillError{std::string{"fwrite returned 0 bytes written without error - stream may be "
                                   "at EOF or in invalid state"}});
        }

        // Partial write succeeded - continue with remaining bytes
      }

      bytes_written += written;
    }
  }

protected:
  /**
   * Flushes the stream
   */
  QUILL_ATTRIBUTE_HOT void flush()
  {
    int const result = std::fflush(_file);

    if (QUILL_LIKELY(result == 0))
    {
      _write_occurred = false;
    }
    else
    {
      int const saved_errno = errno;
      std::clearerr(_file); // Reset error state
      QUILL_THROW(QuillError{std::string{"fflush failed errno: "} + std::to_string(saved_errno) +
                             " error: " + std::strerror(saved_errno)});
    }
  }

protected:
  fs::path _filename;
  FILE* _file{nullptr};
  size_t _file_size{0}; /**< The current file size */
  FileEventNotifier _file_event_notifier;
  bool _is_null{false};
  bool _write_occurred{false};
};

#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
  #pragma warning(pop)
#endif

QUILL_END_NAMESPACE