module;

#ifdef ENABLE_UTIL
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/ConfigurationMapper.h"
#include "Poco/Util/ConfigurationView.h"
#include "Poco/Util/FilesystemConfiguration.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/Util/IntValidator.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Util/LocalConfigurationView.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/LoggingSubsystem.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/Util/OptionCallback.h"
#include "Poco/Util/OptionException.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/RegExpValidator.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Util/SystemConfiguration.h"
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"
#include "Poco/Util/TimerTask.h"
#include "Poco/Util/Units.h"
#include "Poco/Util/Util.h"
#include "Poco/Util/Validator.h"
#ifdef _WIN32
#include "Poco/Util/WinRegistryConfiguration.h"
#include "Poco/Util/WinRegistryKey.h"
#include "Poco/Util/WinService.h"
#endif
#include "Poco/Util/XMLConfiguration.h"
#endif

export module Poco.Util;

export namespace Poco::Util {
	#ifdef ENABLE_UTIL
	using Poco::Util::AbstractConfiguration;
	using Poco::Util::AbstractOptionCallback;
	using Poco::Util::AmbiguousOptionException;
	using Poco::Util::Application;
	using Poco::Util::ConfigurationMapper;
	using Poco::Util::ConfigurationView;
	using Poco::Util::DuplicateOptionException;
	using Poco::Util::EmptyOptionException;
	using Poco::Util::FilesystemConfiguration;
	using Poco::Util::HelpFormatter;
	using Poco::Util::IncompatibleOptionsException;
	using Poco::Util::IniFileConfiguration;
	using Poco::Util::IntValidator;
	using Poco::Util::InvalidArgumentException;
	using Poco::Util::JSONConfiguration;
	using Poco::Util::LayeredConfiguration;
	using Poco::Util::LocalConfigurationView;
	using Poco::Util::LoggingConfigurator;
	using Poco::Util::LoggingSubsystem;
	using Poco::Util::MapConfiguration;
	using Poco::Util::MissingArgumentException;
	using Poco::Util::MissingOptionException;
	using Poco::Util::Option;
	using Poco::Util::OptionCallback;
	using Poco::Util::OptionException;
	using Poco::Util::OptionProcessor;
	using Poco::Util::OptionSet;
	using Poco::Util::PropertyFileConfiguration;
	using Poco::Util::RegExpValidator;
	using Poco::Util::ServerApplication;
	using Poco::Util::Subsystem;
	using Poco::Util::SystemConfiguration;
	using Poco::Util::Timer;
	using Poco::Util::TimerFunc;
	using Poco::Util::TimerTask;
	using Poco::Util::TimerTaskAdapter;
	using Poco::Util::UnexpectedArgumentException;
	using Poco::Util::UnknownOptionException;
	using Poco::Util::Validator;
	#ifdef _WIN32
	using Poco::Util::WinRegistryConfiguration;
	using Poco::Util::WinRegistryKey;
	using Poco::Util::WinService;
	#endif
	using Poco::Util::XMLConfiguration;
	#endif
}
