/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/LogLevel.h"
#include "quill/sinks/StreamSink.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
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
#else
  #include <cstdlib>
  #include <unistd.h>
#endif

QUILL_BEGIN_NAMESPACE

/** Forward Declaration **/
class MacroMetadata;

class ConsoleSinkConfig
{
public:
  enum class ColourMode
  {
    Always,
    Automatic,
    Never
  };

  /**
   * Represents console colours
   */
  class Colours
  {
  public:
    Colours() { apply_default_colours(); }

    ~Colours() = default;

    /**
     * Sets some default colours for terminal
     */
    void apply_default_colours() noexcept
    {
      assign_colour_to_log_level(LogLevel::TraceL3, white);
      assign_colour_to_log_level(LogLevel::TraceL2, white);
      assign_colour_to_log_level(LogLevel::TraceL1, white);
      assign_colour_to_log_level(LogLevel::Debug, cyan);
      assign_colour_to_log_level(LogLevel::Info, green);
      assign_colour_to_log_level(LogLevel::Notice, white_bold);
      assign_colour_to_log_level(LogLevel::Warning, yellow_bold);
      assign_colour_to_log_level(LogLevel::Error, red_bold);
      assign_colour_to_log_level(LogLevel::Critical, bold_on_red);
      assign_colour_to_log_level(LogLevel::Backtrace, magenta);
    }

    /**
     * Sets a custom colour per log level
     * @param log_level the log level
     * @param colour the colour
     */
    void assign_colour_to_log_level(LogLevel log_level, std::string_view colour) noexcept
    {
      auto const log_lvl = static_cast<uint32_t>(log_level);
      _log_level_colours[log_lvl] = colour;
      _colours_enabled = true;
    }

    QUILL_ATTRIBUTE_COLD void set_colours_enabled(bool value) { _colours_enabled = value; }

    /**
     * @return true if we are in terminal and have also enabled colours
     */
    QUILL_NODISCARD bool colours_enabled() const noexcept
    {
      return _colour_output_supported && _colours_enabled;
    }

    /**
     * The colour for the given log level
     * @param log_level the message log level
     * @return the configured colour for this log level
     */
    QUILL_NODISCARD std::string_view log_level_colour(LogLevel log_level) const noexcept
    {
      auto const log_lvl = static_cast<uint32_t>(log_level);
      return _log_level_colours[log_lvl];
    }

    // Formatting codes
    static constexpr std::string_view reset{"\033[0m"};
    static constexpr std::string_view bold{"\033[1m"};
    static constexpr std::string_view dark{"\033[2m"};
    static constexpr std::string_view underline{"\033[4m"};
    static constexpr std::string_view blink{"\033[5m"};
    static constexpr std::string_view reverse{"\033[7m"};
    static constexpr std::string_view concealed{"\033[8m"};
    static constexpr std::string_view clear_line{"\033[K"};

    // Foreground colors
    static constexpr std::string_view black{"\033[30m"};
    static constexpr std::string_view red{"\033[31m"};
    static constexpr std::string_view green{"\033[32m"};
    static constexpr std::string_view yellow{"\033[33m"};
    static constexpr std::string_view blue{"\033[34m"};
    static constexpr std::string_view magenta{"\033[35m"};
    static constexpr std::string_view cyan{"\033[36m"};
    static constexpr std::string_view white{"\033[37m"};

    /// Background colors
    static constexpr std::string_view on_black{"\033[40m"};
    static constexpr std::string_view on_red{"\033[41m"};
    static constexpr std::string_view on_green{"\033[42m"};
    static constexpr std::string_view on_yellow{"\033[43m"};
    static constexpr std::string_view on_blue{"\033[44m"};
    static constexpr std::string_view on_magenta{"\033[45m"};
    static constexpr std::string_view on_cyan{"\033[46m"};
    static constexpr std::string_view on_white{"\033[47m"};

    /// Bold colors
    static constexpr std::string_view white_bold{"\033[97m\033[1m"};
    static constexpr std::string_view yellow_bold{"\033[33m\033[1m"};
    static constexpr std::string_view red_bold{"\033[31m\033[1m"};
    static constexpr std::string_view bold_on_red{"\033[1m\033[41m"};

  private:
    friend class ConsoleSink;

    /***/
    QUILL_NODISCARD QUILL_ATTRIBUTE_COLD static bool _supports_colour_output() noexcept
    {
#ifdef _WIN32
      // On Windows 10 and later, ANSI colors are supported
      return true;
#else
      // Get term from env
      auto* env_p = std::getenv("TERM");

      if (env_p == nullptr)
      {
        return false;
      }

      static constexpr char const* terms[] = {
        "ansi",          "color",     "console",        "cygwin",         "gnome",
        "konsole",       "kterm",     "linux",          "msys",           "putty",
        "rxvt",          "screen",    "vt100",          "xterm",          "tmux",
        "terminator",    "alacritty", "gnome-terminal", "xfce4-terminal", "lxterminal",
        "mate-terminal", "uxterm",    "eterm",          "tilix",          "rxvt-unicode",
        "kde-konsole"};

      // Loop through each term and check if it's found in env_p
      for (char const* term : terms)
      {
        if (std::strstr(env_p, term) != nullptr)
        {
          // term found
          return true;
        }
      }

      // none of the terms are found
      return false;
#endif
    }

    /***/
    QUILL_NODISCARD QUILL_ATTRIBUTE_COLD static bool _is_terminal_output(FILE* file) noexcept
    {
#ifdef _WIN32
      return _isatty(_fileno(file)) != 0;
#else
      return ::isatty(fileno(file)) != 0;
#endif
    }

#ifdef _WIN32
    /***/
    QUILL_ATTRIBUTE_COLD void _activate_ansi_support(FILE* file) const
    {
      if (!_colour_output_supported)
      {
        return;
      }

      // Try to enable ANSI support for Windows console
      auto const out_handle = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(file)));
      if (out_handle == INVALID_HANDLE_VALUE)
      {
        return;
      }

      DWORD dw_mode = 0;
      if (!GetConsoleMode(out_handle, &dw_mode))
      {
        return;
      }

      dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      dw_mode |= ENABLE_PROCESSED_OUTPUT;

      SetConsoleMode(out_handle, dw_mode);
    }
#endif

    /***/
    void _configure_colour_support(FILE* file, ColourMode colour_mode) noexcept
    {
      if (colour_mode == ColourMode::Always)
      {
        _colour_output_supported = true;
      }
      else if (colour_mode == ColourMode::Automatic)
      {
        _colour_output_supported = _is_terminal_output(file) && _supports_colour_output();
      }
      else
      {
        _colour_output_supported = false;
      }

#ifdef _WIN32
      // Enable ANSI color support on Windows
      _activate_ansi_support(file);
#endif
    }

  private:
    std::array<std::string_view, 10> _log_level_colours; /**< Colours per log level */
    bool _colours_enabled{true};
    bool _colour_output_supported{false};
  };

  /**
   * @brief Sets custom colours for each log level.
   *
   * This function allows specifying a custom Colours instance, enabling fine-grained control
   * over the colours used for different log levels in the console output.
   *
   * @param colours The Colours instance to use.
   */
  QUILL_ATTRIBUTE_COLD void set_colours(Colours colours) { _colours = colours; }

  /**
   * @brief Sets the colour mode for console output.
   *
   * This function determines when colours are applied in the console output.
   * Valid options for the colour mode are:
   *   - Always: Colours are always enabled.
   *   - Automatic: Colours are enabled automatically based on the environment
   *     (e.g., terminal support).
   *   - Never: Colours are never enabled.
   *
   * The default value is 'Automatic'.
   *
   * @param colour_mode The desired colour mode.
   */
  QUILL_ATTRIBUTE_COLD void set_colour_mode(ColourMode colour_mode) { _colour_mode = colour_mode; }

  /**
   * @brief Sets the output stream for console logging.
   *
   * This function allows selecting the output stream where log messages should be printed.
   * The valid options are:
   *   - "stdout": Logs are printed to the standard output.
   *   - "stderr": Logs are printed to the standard error.
   *
   * The default value is "stdout".
   *
   * @param stream The output stream to use.
   */
  QUILL_ATTRIBUTE_COLD void set_stream(std::string const& stream) { _stream = stream; }

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
  QUILL_NODISCARD Colours const& colours() noexcept { return _colours; }
  QUILL_NODISCARD ColourMode colour_mode() const noexcept { return _colour_mode; }
  QUILL_NODISCARD std::string const& stream() const noexcept { return _stream; }
  QUILL_NODISCARD std::optional<PatternFormatterOptions> const& override_pattern_formatter_options() const noexcept
  {
    return _override_pattern_formatter_options;
  }

private:
  friend class ConsoleSink;

  std::optional<PatternFormatterOptions> _override_pattern_formatter_options;
  std::string _stream = "stdout";
  Colours _colours;
  ColourMode _colour_mode{ColourMode::Automatic};
};

/***/
class ConsoleSink : public StreamSink
{
public:
  /**
   * @brief Constructor with custom ConsoleColours
   * config
   */
  explicit ConsoleSink(ConsoleSinkConfig const& config = ConsoleSinkConfig{},
                       FileEventNotifier file_event_notifier = FileEventNotifier{})
    : StreamSink{config.stream(), nullptr, config.override_pattern_formatter_options(),
                 std::move(file_event_notifier)},
      _config(config)
  {
    QUILL_ASSERT(_config.stream() == "stdout" || config.stream() == "stderr",
                 "Invalid stream name in ConsoleSink constructor, must be 'stdout' or 'stderr'");

    if (_config.colour_mode() == ConsoleSinkConfig::ColourMode::Never)
    {
      _config._colours.set_colours_enabled(false);
    }
    else
    {
      _config._colours._configure_colour_support(_file, _config.colour_mode());
      _config._colours.set_colours_enabled(true);
    }
  }

  ~ConsoleSink() override = default;

  /**
   * @brief Write a formatted log message to the stream
   * @param log_metadata log metadata
   * @param log_timestamp log timestamp
   * @param thread_id thread id
   * @param thread_name thread name
   * @param process_id Process Id
   * @param logger_name logger name
   * @param log_level Log level of the message.
   * @param log_level_description Description of the log level.
   * @param log_level_short_code Short code representing the log level.
   * @param named_args vector of key-value pairs of named args
   * @param log_message log message
   * @param log_statement log statement
   */
  QUILL_ATTRIBUTE_HOT void write_log(MacroMetadata const* log_metadata, uint64_t log_timestamp,
                                     std::string_view thread_id, std::string_view thread_name,
                                     std::string const& process_id, std::string_view logger_name,
                                     LogLevel log_level, std::string_view log_level_description,
                                     std::string_view log_level_short_code,
                                     std::vector<std::pair<std::string, std::string>> const* named_args,
                                     std::string_view log_message, std::string_view log_statement) override
  {
    if (_config.colours().colours_enabled())
    {
      // Write colour code
      std::string_view const colour_code = _config.colours().log_level_colour(log_level);
      safe_fwrite(colour_code.data(), sizeof(char), colour_code.size(), _file);

      // Write record to file
      StreamSink::write_log(log_metadata, log_timestamp, thread_id, thread_name, process_id,
                            logger_name, log_level, log_level_description, log_level_short_code,
                            named_args, log_message, log_statement);

      // Reset colour code
      safe_fwrite(ConsoleSinkConfig::Colours::reset.data(), sizeof(char),
                  ConsoleSinkConfig::Colours::reset.size(), _file);
    }
    else
    {
      // Write record to file
      StreamSink::write_log(log_metadata, log_timestamp, thread_id, thread_name, process_id,
                            logger_name, log_level, log_level_description, log_level_short_code,
                            named_args, log_message, log_statement);
    }
  }

protected:
  // protected in case someone wants to derive from this class and create a custom one, e.g. for json logging to stdout
  ConsoleSinkConfig _config;
};

QUILL_END_NAMESPACE