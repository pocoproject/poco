/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/backend/BackendOptions.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"

#include <memory>
#include <string>
#include <utility>

QUILL_BEGIN_NAMESPACE

/**
 * @brief Convenience function for trivial programs and simple use cases.
 *
 * Example usage:
 * @code
 * #include "quill/SimpleSetup.h"
 * #include "quill/LogMacros.h"
 *
 * int main() {
 *   // Log to console (stdout) - defaults to stdout
 *   auto* logger = quill::simple_logger();
 *   LOG_INFO(logger, "Hello from {}!", "Quill");
 *
 *   // Or log to a file
 *   auto* logger2 = quill::simple_logger("test.log");
 *   LOG_INFO(logger2, "This message goes to a file");
 * }
 * @endcode
 *
 * @param output The output destination. Use "stdout" (default) or "stderr" for console output,
 *               or provide a filename for file output. The output name is also used as the logger name.
 *
 * @return A pointer to the created or retrieved logger instance.
 */
inline Logger* simple_logger(std::string const& output = "stdout")
{
  Logger* logger = Frontend::get_logger(output);

  if (!logger)
  {
    std::shared_ptr<Sink> sink;

    if (output == "stdout" || output == "stderr")
    {
      sink = Frontend::create_or_get_sink<ConsoleSink>(output);
    }
    else
    {
      sink = Frontend::create_or_get_sink<FileSink>(output);
    }

    logger = Frontend::create_or_get_logger(
      output, std::move(sink),
      PatternFormatterOptions{
        "%(time) [%(thread_id)] %(short_source_location:<28) LOG_%(log_level:<9) %(message)"});

    Backend::start<FrontendOptions>(BackendOptions{}, SignalHandlerOptions{});
  }

  return logger;
}

QUILL_END_NAMESPACE