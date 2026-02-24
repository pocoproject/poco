//
// FastLoggerConfigurator.h
//
// Library: Util
// Package: Configuration
// Module:  FastLoggerConfigurator
//
// Definition of the FastLoggerConfigurator class.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_FastLoggerConfigurator_INCLUDED
#define Util_FastLoggerConfigurator_INCLUDED


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"


namespace Poco {
namespace Util {


class Util_API FastLoggerConfigurator
	/// This utility class uses a configuration object to configure
	/// the FastLogger logging subsystem.
	///
	/// FastLoggerConfigurator configures FastLogger instances using properties
	/// under "logging.fastloggers" in the configuration.
	///
	/// Unlike regular Logger configuration, FastLogger does not use channels
	/// or formatters - these are handled internally by the Quill backend.
	/// The configuration focuses on logger hierarchy and log levels.
	///
	/// Configuring FastLoggers
	///
	/// A FastLogger is configured using the "logging.fastloggers" property.
	/// Each logger requires a "name" property (empty string for root) and
	/// supports a "level" property.
	///
	/// Additional sink configuration:
	///   - logging.fastloggers.sinks.console = true (default)
	///   - logging.fastloggers.sinks.file = /path/to/logfile.log
	///
	/// Examples:
	///     logging.fastloggers.root.level = warning
	///     logging.fastloggers.l1.name = HTTP.Server
	///     logging.fastloggers.l1.level = information
	///     logging.fastloggers.l2.name = Database
	///     logging.fastloggers.l2.level = debug
	///
	/// The FastLoggerConfigurator can be used alongside the regular
	/// LoggingConfigurator - they configure separate logger hierarchies.
{
public:
	FastLoggerConfigurator() = default;
		/// Creates the FastLoggerConfigurator.

	FastLoggerConfigurator(const FastLoggerConfigurator&) = delete;

	FastLoggerConfigurator& operator = (const FastLoggerConfigurator&) = delete;

	~FastLoggerConfigurator() = default;
		/// Destroys the FastLoggerConfigurator.

	void configure(AbstractConfiguration::Ptr pConfig);
		/// Configures the FastLogger subsystem based on
		/// the given configuration.
		///
		/// A ConfigurationView can be used to pass only
		/// a part of a larger configuration.

private:
	void configureSinks(AbstractConfiguration::Ptr pConfig);
	void configureFastLoggers(AbstractConfiguration::Ptr pConfig);
	void configureFastLogger(AbstractConfiguration::Ptr pConfig);
};


} } // namespace Poco::Util


#endif // POCO_ENABLE_FASTLOGGER


#endif // Util_FastLoggerConfigurator_INCLUDED
