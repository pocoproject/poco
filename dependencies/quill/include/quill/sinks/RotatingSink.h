/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/Filesystem.h"
#include "quill/core/LogLevel.h"
#include "quill/core/QuillError.h"
#include "quill/core/TimeUtilities.h"
#include "quill/sinks/FileSink.h"
#include "quill/sinks/StreamSink.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <deque>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

/** Forward Declaration **/
class MacroMetadata;

/**
 * @brief The configuration options for the RotatingSink
 */
class RotatingFileSinkConfig : public FileSinkConfig
{
public:
  /**
   * @brief The frequency of log file rotation
   */
  enum class RotationFrequency : uint8_t
  {
    Disabled,
    Daily,
    Hourly,
    Minutely
  };

  /**
   * @brief The naming scheme for rotated log files
   */
  enum class RotationNamingScheme : uint8_t
  {
    Index,
    Date,
    DateAndTime
  };

  /**
   * @brief Constructs a new RotatingFileSinkConfig object.
   */
  RotatingFileSinkConfig() : _daily_rotation_time{_disabled_daily_rotation_time()} {}

  /**
   * @brief Sets the maximum file size for rotation.
   * @param value The maximum file size in bytes per file
   */
  QUILL_ATTRIBUTE_COLD void set_rotation_max_file_size(size_t value)
  {
    if (value < 512)
    {
      QUILL_THROW(QuillError{"rotation_max_file_size must be greater than or equal to 512 bytes"});
    }

    _rotation_max_file_size = value;
  }

  /**
   * @brief Sets the frequency and interval of file rotation.
   * @param frequency The frequency of file rotation ('M' for minutes, 'H' for hours)
   * @param interval The rotation interval
   */
  QUILL_ATTRIBUTE_COLD void set_rotation_frequency_and_interval(char frequency, uint32_t interval)
  {
    if (frequency == 'M' || frequency == 'm')
    {
      _rotation_frequency = RotationFrequency::Minutely;
    }
    else if (frequency == 'H' || frequency == 'h')
    {
      _rotation_frequency = RotationFrequency::Hourly;
    }
    else
    {
      QUILL_THROW(QuillError{
        "Invalid frequency. Valid values are 'M' or 'm' for minutes or 'H' or 'h' for hours"});
    }

    if (interval == 0)
    {
      QUILL_THROW(QuillError{"interval must be set to a value greater than 0"});
    }

    _rotation_interval = interval;
    _daily_rotation_time = _disabled_daily_rotation_time();
  }

  /**
   * @brief Sets the time of day for daily log file rotation.
   * @param daily_rotation_time_str The time of day for rotation (format: "HH:MM")
   */
  QUILL_ATTRIBUTE_COLD void set_rotation_time_daily(std::string const& daily_rotation_time_str)
  {
    _rotation_frequency = RotationFrequency::Daily;
    _rotation_interval = 0;
    _daily_rotation_time = _parse_daily_rotation_time(daily_rotation_time_str);
  }

  /**
   * @brief Sets the maximum number of log files to keep.
   * @param value The maximum number of log files
   */
  QUILL_ATTRIBUTE_COLD void set_max_backup_files(uint32_t value) { _max_backup_files = value; }

  /**
   * @brief Sets whether the oldest rolled logs should be overwritten when the maximum backup count
   * is reached. If set to false, the oldest logs will not be overwritten when the maximum backup
   * count is reached, and log file rotation will stop. The default value is true.
   * @param value True to overwrite the oldest logs, false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_overwrite_rolled_files(bool value)
  {
    _overwrite_rolled_files = value;
  }

  /**
   * @brief Sets whether previous rotated log files should be removed on process start up.
   * @note This option works only when using the mode="w"
   * This is useful to avoid conflicting file names when the process restarts and
   * FilenameAppend::DateTime was not set. The default value is true.
   * @param value True to remove old log files, false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_remove_old_files(bool value) { _remove_old_files = value; }

  /**
   * @brief Sets the naming scheme for the rotated files.
   * The default value is 'Index'.
   * @param value The naming scheme to set.
   */
  QUILL_ATTRIBUTE_COLD void set_rotation_naming_scheme(RotationNamingScheme value)
  {
    _rotation_naming_scheme = value;
  }

  /**
   * @brief Sets whether to force rotation on file sink creation/startup.
   * When enabled, if a log file with the same name exists on startup, it will be rotated
   * according to the RotationNamingScheme before starting to write new logs.
   * This allows creating a new log file for every program run. The default value is false.
   * @param value True to force rotation on creation, false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_rotation_on_creation(bool value)
  {
    _rotation_on_creation = value;
  }

  /** Getter methods **/
  QUILL_NODISCARD size_t rotation_max_file_size() const noexcept { return _rotation_max_file_size; }
  QUILL_NODISCARD uint32_t max_backup_files() const noexcept { return _max_backup_files; }
  QUILL_NODISCARD bool overwrite_rolled_files() const noexcept { return _overwrite_rolled_files; }
  QUILL_NODISCARD bool remove_old_files() const noexcept { return _remove_old_files; }
  QUILL_NODISCARD RotationFrequency rotation_frequency() const noexcept
  {
    return _rotation_frequency;
  }
  QUILL_NODISCARD uint32_t rotation_interval() const noexcept { return _rotation_interval; }
  QUILL_NODISCARD std::pair<std::chrono::hours, std::chrono::minutes> daily_rotation_time() const noexcept
  {
    return _daily_rotation_time;
  }
  QUILL_NODISCARD RotationNamingScheme rotation_naming_scheme() const noexcept
  {
    return _rotation_naming_scheme;
  }
  QUILL_NODISCARD bool rotation_on_creation() const noexcept { return _rotation_on_creation; }

private:
  /***/
  static std::pair<std::chrono::hours, std::chrono::minutes> _disabled_daily_rotation_time() noexcept
  {
    return std::make_pair(std::chrono::hours{std::numeric_limits<std::chrono::hours::rep>::max()},
                          std::chrono::minutes{std::numeric_limits<std::chrono::hours::rep>::max()});
  }

  /***/
  static std::pair<std::chrono::hours, std::chrono::minutes> _parse_daily_rotation_time(std::string const& daily_rotation_time_str)
  {
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0, end = 0;

    while ((end = daily_rotation_time_str.find(':', start)) != std::string::npos)
    {
      token = daily_rotation_time_str.substr(start, end - start);
      tokens.push_back(token);
      start = end + 1;
    }

    // Add the last token (or the only token if there's no delimiter)
    token = daily_rotation_time_str.substr(start);
    tokens.push_back(token);

    if (tokens.size() != 2)
    {
      QUILL_THROW(
        QuillError{"Invalid daily_rotation_time_str value format. The format should be `HH:MM`."});
    }

    for (auto const& parsed_token : tokens)
    {
      if (parsed_token.size() != 2)
      {
        QUILL_THROW(QuillError{
          "Invalid daily_rotation_time_str value format. Each component of the time (HH and MM) "
          "should be two digits."});
      }
    }

    auto const daily_rotation_time_str_tp = std::make_pair(
      std::chrono::hours{std::stoi(tokens[0])}, std::chrono::minutes{std::stoi(tokens[1])});

    if ((daily_rotation_time_str_tp.first > std::chrono::hours{23}) ||
        (daily_rotation_time_str_tp.second > std::chrono::minutes{59}))
    {
      QUILL_THROW(
        QuillError("Invalid rotation values. The hour value should be between 00 and 23, and the "
                   "minute value should be between 00 and 59."));
    }

    return daily_rotation_time_str_tp;
  }

private:
  std::pair<std::chrono::hours, std::chrono::minutes> _daily_rotation_time;
  size_t _rotation_max_file_size{0};                                // 0 means disabled
  uint32_t _max_backup_files{std::numeric_limits<uint32_t>::max()}; // max means disabled
  uint32_t _rotation_interval{0};                                   // 0 means disabled
  RotationFrequency _rotation_frequency{RotationFrequency::Disabled};
  RotationNamingScheme _rotation_naming_scheme{RotationNamingScheme::Index};
  bool _overwrite_rolled_files{true};
  bool _remove_old_files{true};
  bool _rotation_on_creation{false};
};

/**
 * @brief The RotatingSink class
 */
template <typename TBase>
class RotatingSink : public TBase
{
public:
  using base_type = TBase;

  /**
   * @brief Constructor.
   *
   * Creates a new instance of the RotatingSink class.
   *
   * @param filename The base file name to be used for logs.
   * @param config The sink configuration.
   * @param file_event_notifier file event notifier
   * @param start_time start time
   */
  RotatingSink(fs::path const& filename, RotatingFileSinkConfig const& config,
               FileEventNotifier file_event_notifier = FileEventNotifier{},
               std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now())
    : base_type(filename, static_cast<FileSinkConfig const&>(config),
                std::move(file_event_notifier), false, start_time),
      _config(config)
  {
    uint64_t const today_timestamp_ns = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(start_time.time_since_epoch()).count());

    _clean_and_recover_files(this->_filename, _config.open_mode(), today_timestamp_ns);

    if (_config.rotation_frequency() != RotatingFileSinkConfig::RotationFrequency::Disabled)
    {
      // Calculate next rotation time
      _next_rotation_time = _calculate_initial_rotation_tp(
        static_cast<uint64_t>(
          std::chrono::duration_cast<std::chrono::nanoseconds>(start_time.time_since_epoch()).count()),
        config);
    }

    // Open file for logging
    this->open_file(this->_filename, _config.open_mode());
    _open_file_timestamp = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(start_time.time_since_epoch()).count());

    _created_files.emplace_front(this->_filename, 0, std::string{});

    // Check if we need to rotate on creation
    if (_config.rotation_on_creation() && !this->is_null() && _get_file_size(this->_filename) > 0)
    {
      _rotate_files(today_timestamp_ns);
    }

    if (!this->is_null())
    {
      this->_file_size = _get_file_size(this->_filename);
    }
  }

  ~RotatingSink() override = default;

  /**
   * @brief Writes a formatted log message to the stream
   * @param log_metadata The metadata of the log message
   * @param log_timestamp The timestamp of the log message
   * @param thread_id The ID of the thread that generated the log message
   * @param thread_name The name of the thread that generated the log message
   * @param process_id Process Id
   * @param logger_name logger name
   * @param log_level Log level of the message.
   * @param log_level_description Description of the log level.
   * @param log_level_short_code Short code representing the log level.
   * @param named_args Structured key-value pairs associated with the log message
   * @param log_message The log message to write
   * @param log_statement The full log statement
   */
  QUILL_ATTRIBUTE_HOT void write_log(MacroMetadata const* log_metadata, uint64_t log_timestamp,
                                     std::string_view thread_id, std::string_view thread_name,
                                     std::string const& process_id, std::string_view logger_name,
                                     LogLevel log_level, std::string_view log_level_description,
                                     std::string_view log_level_short_code,
                                     std::vector<std::pair<std::string, std::string>> const* named_args,
                                     std::string_view log_message, std::string_view log_statement) override
  {
    if (this->is_null())
    {
      base_type::write_log(log_metadata, log_timestamp, thread_id, thread_name, process_id,
                           logger_name, log_level, log_level_description, log_level_short_code,
                           named_args, log_message, log_statement);
      return;
    }

    bool time_rotation = false;

    if (_config.rotation_frequency() != RotatingFileSinkConfig::RotationFrequency::Disabled)
    {
      // Check if we need to rotate based on time
      time_rotation = _time_rotation(log_timestamp);
    }

    if (!time_rotation && _config.rotation_max_file_size() != 0)
    {
      // Check if we need to rotate based on size
      _size_rotation(log_statement.size(), log_timestamp);
    }

    // write to file
    base_type::write_log(log_metadata, log_timestamp, thread_id, thread_name, process_id,
                         logger_name, log_level, log_level_description, log_level_short_code,
                         named_args, log_message, log_statement);
  }

private:
  /***/
  QUILL_NODISCARD bool _time_rotation(uint64_t record_timestamp_ns)
  {
    if (record_timestamp_ns >= _next_rotation_time)
    {
      _rotate_files(record_timestamp_ns);
      _next_rotation_time = _calculate_rotation_tp(record_timestamp_ns, _config);
      return true;
    }

    return false;
  }

  /***/
  void _size_rotation(size_t log_msg_size, uint64_t record_timestamp_ns)
  {
    // Calculate the new size of the file
    if ((this->_file_size + log_msg_size) > _config.rotation_max_file_size())
    {
      _rotate_files(record_timestamp_ns);
    }
  }

  /***/
  void _rotate_files(uint64_t record_timestamp_ns)
  {
    if ((_created_files.size() > _config.max_backup_files()) && !_config.overwrite_rolled_files())
    {
      // We have reached the max number of backup files, and we are not allowed to overwrite the
      // oldest file. We will stop rotating
      return;
    }

    // We need to flush and also fsync before actually getting the size of the file
    base_type::flush_sink();
    base_type::fsync_file(true);

    if (_get_file_size(this->_filename) <= 0)
    {
      // Also check the file size is > 0  to better deal with full disk
      return;
    }

    this->close_file();

    // datetime_suffix will be empty if we are using the default naming scheme
    std::string datetime_suffix;
    if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Date)
    {
      datetime_suffix =
        this->format_datetime_string(_open_file_timestamp, _config.timezone(), "%Y%m%d");
    }
    else if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::DateAndTime)
    {
      datetime_suffix =
        this->format_datetime_string(_open_file_timestamp, _config.timezone(), "%Y%m%d_%H%M%S");
    }

    // We need to rotate the files and rename them with an index
    for (auto it = _created_files.rbegin(); it != _created_files.rend(); ++it)
    {
      // Create each existing filename on disk with the existing index.
      // when the index is 0 we want to rename the latest file
      fs::path existing_file;
      fs::path renamed_file;

      existing_file = _get_filename(it->base_filename, it->index, it->date_time);

      // increment the index if needed and rename the file
      uint32_t index_to_use = it->index;

      if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Index ||
          it->date_time == datetime_suffix)
      {
        // we are rotating and incrementing the index, or we have another file with the same date_time suffix
        index_to_use += 1;

        renamed_file = _get_filename(it->base_filename, index_to_use, datetime_suffix);

        it->index = index_to_use;
        it->date_time = datetime_suffix;

        _rename_file(existing_file, renamed_file);
      }
      else if (it->date_time.empty())
      {
        // we are renaming the latest file
        index_to_use = it->index;

        renamed_file = _get_filename(it->base_filename, index_to_use, datetime_suffix);

        it->index = index_to_use;
        it->date_time = datetime_suffix;

        _rename_file(existing_file, renamed_file);
      }
    }

    // Check if we have too many files in the queue remove_file the oldest one
    if (_created_files.size() > _config.max_backup_files())
    {
      // remove_file that file from the system and also pop it from the queue
      fs::path const removed_file = _get_filename(
        _created_files.back().base_filename, _created_files.back().index, _created_files.back().date_time);
      _remove_file(removed_file);
      _created_files.pop_back();
    }

    // add the current file back to the list with index 0
    _created_files.emplace_front(this->_filename, 0, std::string{});

    // Open file for logging
    this->open_file(this->_filename, _config.open_mode());
    _open_file_timestamp = record_timestamp_ns;
    this->_file_size = 0;
  }

  /***/
  void _clean_and_recover_files(fs::path const& filename, std::string const& open_mode, uint64_t today_timestamp_ns)
  {
    if ((_config.rotation_naming_scheme() != RotatingFileSinkConfig::RotationNamingScheme::Index) &&
        (_config.rotation_naming_scheme() != RotatingFileSinkConfig::RotationNamingScheme::Date))
    {
      // clean and recover is only supported for index and date naming scheme, when using
      // DateAndTime there are no collisions in the filenames
      return;
    }

    // if we are starting in "w" mode, then we also should clean all previous log files of the previous run
    if (_config.remove_old_files() && (open_mode.find('w') != std::string::npos))
    {
      for (auto const& entry : fs::directory_iterator(fs::current_path() / filename.parent_path()))
      {
        if (entry.path().extension().string() != filename.extension().string())
        {
          // we only check for the files of the same extension to remove
          continue;
        }

        // is_directory() does not exist in std::experimental::filesystem
        if (entry.path().filename().string().find(filename.stem().string() + ".") != 0)
        {
          // expect to find filename.stem().string() exactly at the start of the filename
          continue;
        }

        if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Index)
        {
          fs::remove(entry);
        }
        else if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Date)
        {
          // Find the first dot in the filename
          // stem will be something like `logfile.1`
          if (size_t const pos = entry.path().stem().string().find_last_of('.'); pos != std::string::npos)
          {
            // Get the today's date, we won't remove the files of the previous dates as they won't collide
            std::string const today_date =
              this->format_datetime_string(today_timestamp_ns, _config.timezone(), "%Y%m%d");

            if (std::string const index_or_date =
                  entry.path().stem().string().substr(pos + 1, entry.path().stem().string().length());
                (index_or_date.length() >= 8) && (index_or_date == today_date))
            {
              // assume it is a date, no need to find the index
              if (index_or_date == today_date)
              {
                fs::remove(entry);
              }
            }
            else
            {
              // assume it is an index
              // Find the second last dot to get the date
              std::string const filename_with_date = entry.path().filename().string().substr(0, pos);

              if (size_t const second_last = filename_with_date.find_last_of('.'); second_last != std::string::npos)
              {
                if (std::string const date_part =
                      filename_with_date.substr(second_last + 1, filename_with_date.length());
                    date_part == today_date)
                {
                  fs::remove(entry);
                }
              }
            }
          }
        }
      }
    }
    else if (open_mode.find('a') != std::string::npos)
    {
      // we need to recover the index from the existing files
      for (auto const& entry : fs::directory_iterator(fs::current_path() / filename.parent_path()))
      {
        // is_directory() does not exist in std::experimental::filesystem
        if (entry.path().extension().string() != filename.extension().string())
        {
          // we only check for the files of the same extension to remove
          continue;
        }

        // is_directory() does not exist in std::experimental::filesystem
        if (entry.path().filename().string().find(filename.stem().string() + ".") != 0)
        {
          // expect to find filename.stem().string() exactly at the start of the filename
          continue;
        }

        std::string const extension = entry.path().extension().string(); // e.g. ".log"

        // stem will be something like `logfile.1`
        if (size_t const pos = entry.path().stem().string().find_last_of('.'); pos != std::string::npos)
        {
          if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Index)
          {
            std::string const index =
              entry.path().stem().string().substr(pos + 1, entry.path().stem().string().length());

            std::string const current_filename = entry.path().filename().string().substr(0, pos) + extension;
            fs::path current_file = entry.path().parent_path();
            current_file.append(current_filename);

            // Attempt to convert the index to a number
            QUILL_TRY
            {
              _created_files.emplace_front(current_file, static_cast<uint32_t>(std::stoul(index)),
                                           std::string{});
            }
            QUILL_CATCH_ALL() { continue; }
          }
          else if (_config.rotation_naming_scheme() == RotatingFileSinkConfig::RotationNamingScheme::Date)
          {
            // Get the today's date, we won't remove the files of the previous dates as they won't collide
            std::string const today_date =
              this->format_datetime_string(today_timestamp_ns, _config.timezone(), "%Y%m%d");

            if (std::string const index_or_date =
                  entry.path().stem().string().substr(pos + 1, entry.path().stem().string().length());
                (index_or_date.length() >= 8) && (index_or_date == today_date))
            {
              // assume it is a date, no need to find the index
              std::string const current_filename = entry.path().filename().string().substr(0, pos) + extension;
              fs::path current_file = entry.path().parent_path();
              current_file.append(current_filename);

              _created_files.emplace_front(current_file, 0, index_or_date);
            }
            else
            {
              // assume it is an index
              // Find the second last dot to get the date
              std::string const filename_with_date = entry.path().filename().string().substr(0, pos);

              if (size_t const second_last = filename_with_date.find_last_of('.'); second_last != std::string::npos)
              {
                if (std::string const date_part =
                      filename_with_date.substr(second_last + 1, filename_with_date.length());
                    date_part == today_date)
                {
                  std::string const current_filename = filename_with_date.substr(0, second_last) + extension;
                  fs::path current_file = entry.path().parent_path();
                  current_file.append(current_filename);

                  // Attempt to convert the index to a number
                  QUILL_TRY
                  {
                    _created_files.emplace_front(
                      current_file, static_cast<uint32_t>(std::stoul(index_or_date)), date_part);
                  }
                  QUILL_CATCH_ALL() { continue; }
                }
              }
            }
          }
        }
      }

      // finally we need to sort the deque
      std::sort(_created_files.begin(), _created_files.end(),
                [](FileInfo const& a, FileInfo const& b) { return a.index < b.index; });
    }
  }

  /***/
  QUILL_NODISCARD static size_t _get_file_size(fs::path const& filename)
  {
    return static_cast<size_t>(fs::file_size(filename));
  }

  /***/
  static void _remove_file(fs::path const& filename) noexcept
  {
    std::error_code ec;

    fs::file_status const status = fs::status(filename, ec);

    if (ec || status.type() != fs::file_type::regular)
    {
      // File doesn't exist or is not a regular file
      return;
    }

    fs::remove(filename, ec);
  }

  /***/
  bool static _rename_file(fs::path const& previous_file, fs::path const& new_file) noexcept
  {
    std::error_code ec;
    fs::rename(previous_file, new_file, ec);

    if (ec)
    {
      // Retry once after a delay - workaround for Windows antivirus locking files
      // This is a common issue where antivirus software temporarily locks files during scanning
      std::this_thread::sleep_for(std::chrono::milliseconds{250});

      ec.clear();
      fs::rename(previous_file, new_file, ec);

      if (ec)
      {
        return false;
      }
    }

    return true;
  }

  /***/
  QUILL_NODISCARD static fs::path _append_index_to_filename(fs::path const& filename, uint32_t index) noexcept
  {
    if (index == 0u)
    {
      return filename;
    }

    // Get base file and extension
    auto const [stem, ext] = base_type::extract_stem_and_extension(filename);
    return fs::path{stem + "." + std::to_string(index) + ext};
  }

  /***/
  QUILL_NODISCARD static fs::path _append_string_to_filename(fs::path const& filename, std::string const& text) noexcept
  {
    if (text.empty())
    {
      return filename;
    }

    // Get base file and extension
    auto const [stem, ext] = base_type::extract_stem_and_extension(filename);
    return fs::path{stem + "." + text + ext};
  }

  /***/
  static uint64_t _calculate_initial_rotation_tp(uint64_t start_time_ns, RotatingFileSinkConfig const& config)
  {
// time_t on i386 is 32 bits so casting out of range number results in zero
#if (defined(__i386))
    time_t const time_now = static_cast<time_t>(start_time_ns / 1000000000);
#else
    time_t const time_now = static_cast<time_t>(start_time_ns) / 1000000000;
#endif
    tm date;

    // here we do this because of `daily_rotation_time_str` that might have specified the time in UTC
    if (config.timezone() == Timezone::GmtTime)
    {
      detail::gmtime_rs(&time_now, &date);
    }
    else
    {
      detail::localtime_rs(&time_now, &date);
    }

    // update to the desired date
    if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Minutely)
    {
      date.tm_min += 1;
      date.tm_sec = 0;
    }
    else if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Hourly)
    {
      date.tm_hour += 1;
      date.tm_min = 0;
      date.tm_sec = 0;
    }
    else if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Daily)
    {
      date.tm_hour = static_cast<decltype(date.tm_hour)>(config.daily_rotation_time().first.count());
      date.tm_min = static_cast<decltype(date.tm_min)>(config.daily_rotation_time().second.count());
      date.tm_sec = 0;
    }
    else
    {
      QUILL_THROW(QuillError{"Invalid rotation frequency"});
    }

    // convert back to timestamp
    time_t const rotation_time =
      (config.timezone() == Timezone::GmtTime) ? detail::timegm(&date) : std::mktime(&date);

    uint64_t const rotation_time_seconds = (rotation_time > time_now)
      ? static_cast<uint64_t>(rotation_time)
      : static_cast<uint64_t>(rotation_time + std::chrono::seconds{std::chrono::hours{24}}.count());

    return static_cast<uint64_t>(
      std::chrono::nanoseconds{std::chrono::seconds{rotation_time_seconds}}.count());
  }

  /***/
  static uint64_t _calculate_rotation_tp(uint64_t rotation_timestamp_ns, RotatingFileSinkConfig const& config)
  {
    if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Minutely)
    {
      return rotation_timestamp_ns +
        static_cast<uint64_t>(
               std::chrono::nanoseconds{std::chrono::minutes{config.rotation_interval()}}.count());
    }

    if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Hourly)
    {
      return rotation_timestamp_ns +
        static_cast<uint64_t>(
               std::chrono::nanoseconds{std::chrono::hours{config.rotation_interval()}}.count());
    }

    if (config.rotation_frequency() == RotatingFileSinkConfig::RotationFrequency::Daily)
    {
      return rotation_timestamp_ns + std::chrono::nanoseconds{std::chrono::hours{24}}.count();
    }

    QUILL_THROW(QuillError{"Invalid rotation frequency"});
  }

  /***/
  static fs::path _get_filename(fs::path filename, uint32_t index, std::string const& date_time)
  {
    if (!date_time.empty())
    {
      filename = _append_string_to_filename(filename, date_time);
    }

    if (index > 0)
    {
      filename = _append_index_to_filename(filename, index);
    }

    return filename;
  }

protected:
  struct FileInfo
  {
    FileInfo(fs::path base_filename, uint32_t index, std::string date_time)
      : base_filename{std::move(base_filename)}, date_time{std::move(date_time)}, index{index}
    {
    }

    fs::path base_filename;
    std::string date_time;
    uint32_t index;
  };

  FileEventNotifier _file_event_notifier;
  std::deque<FileInfo> _created_files; /**< We store in a queue the filenames we created, first: index, second: date/datetime, third: base_filename */
  uint64_t _next_rotation_time;        /**< The next rotation time point */
  uint64_t _open_file_timestamp{0};    /**< The timestamp of the currently open file */
  RotatingFileSinkConfig _config;
};

QUILL_END_NAMESPACE