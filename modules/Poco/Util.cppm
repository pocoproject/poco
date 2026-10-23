//
// Util.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_UTIL
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/ConfigurationMapper.h"
#include "Poco/Util/ConfigurationView.h"
#include "Poco/Util/FastLoggerConfigurator.h"
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

export module Poco:Util;

export namespace Poco::Util {
	#ifdef ENABLE_UTIL
	using Poco::Util::AbstractConfiguration;
	using Poco::Util::AbstractConfigurationView;
	using Poco::Util::AbstractOptionCallback;
	using Poco::Util::AmbiguousOptionException;
	using Poco::Util::Application;
	using Poco::Util::ConfigurationMapper;
	using Poco::Util::ConfigurationView;
	using Poco::Util::DuplicateOptionException;
	using Poco::Util::EmptyOptionException;
	#ifdef POCO_ENABLE_FASTLOGGER
	using Poco::Util::FastLoggerConfigurator;
	#endif
	using Poco::Util::FilesystemConfiguration;
	using Poco::Util::HelpFormatter;
	using Poco::Util::IncompatibleOptionsException;
	using Poco::Util::IniFileConfiguration;
	using Poco::Util::IntValidator;
	using Poco::Util::InvalidArgumentException;
	#ifndef POCO_UTIL_NO_JSONCONFIGURATION
	using Poco::Util::JSONConfiguration;
	#endif
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
	#ifndef POCO_UTIL_NO_XMLCONFIGURATION
	using Poco::Util::XMLConfiguration;
	#endif

	namespace Units {
		using Poco::Util::Units::Compose;
		using Poco::Util::Units::OutputUnit;
		using Poco::Util::Units::Power;
		using Poco::Util::Units::Scale;
		using Poco::Util::Units::Translate;
		using Poco::Util::Units::Value;

		using Poco::Util::Units::Unit;

		using Poco::Util::Units::abs;
		using Poco::Util::Units::cos;
		using Poco::Util::Units::cube;
		using Poco::Util::Units::operator*;
		using Poco::Util::Units::operator/;
		using Poco::Util::Units::operator<<;
		using Poco::Util::Units::raise;
		using Poco::Util::Units::sin;
		using Poco::Util::Units::sqrt;
		using Poco::Util::Units::square;
		using Poco::Util::Units::streamOp;
		using Poco::Util::Units::tan;

		namespace Constants {
			using Poco::Util::Units::Constants::AU;
			using Poco::Util::Units::Constants::F;
			using Poco::Util::Units::Constants::G;
			using Poco::Util::Units::Constants::G0;
			using Poco::Util::Units::Constants::NA;
			using Poco::Util::Units::Constants::R;
			using Poco::Util::Units::Constants::Rinf;
			using Poco::Util::Units::Constants::alpha;
			using Poco::Util::Units::Constants::c;
			using Poco::Util::Units::Constants::e;
			using Poco::Util::Units::Constants::e0;
			using Poco::Util::Units::Constants::eV;
			using Poco::Util::Units::Constants::g;
			using Poco::Util::Units::Constants::h;
			using Poco::Util::Units::Constants::h_bar;
			using Poco::Util::Units::Constants::inv_alpha;
			using Poco::Util::Units::Constants::k;
			using Poco::Util::Units::Constants::lightyear;
			using Poco::Util::Units::Constants::me;
			using Poco::Util::Units::Constants::mp;
			using Poco::Util::Units::Constants::mpme;
			using Poco::Util::Units::Constants::mu;
			using Poco::Util::Units::Constants::phi0;
			using Poco::Util::Units::Constants::pi;
			using Poco::Util::Units::Constants::rho;
			using Poco::Util::Units::Constants::u0;
		}

		namespace Units {
			using Poco::Util::Units::Units::A;
			using Poco::Util::Units::Units::K;
			using Poco::Util::Units::Units::Prefix;
			using Poco::Util::Units::Units::atto;
			using Poco::Util::Units::Units::cd;
			using Poco::Util::Units::Units::centi;
			using Poco::Util::Units::Units::deca;
			using Poco::Util::Units::Units::deci;
			using Poco::Util::Units::Units::exa;
			using Poco::Util::Units::Units::femto;
			using Poco::Util::Units::Units::giga;
			using Poco::Util::Units::Units::hecto;
			using Poco::Util::Units::Units::kg;
			using Poco::Util::Units::Units::kilo;
			using Poco::Util::Units::Units::m;
			using Poco::Util::Units::Units::mega;
			using Poco::Util::Units::Units::micro;
			using Poco::Util::Units::Units::milli;
			using Poco::Util::Units::Units::mol;
			using Poco::Util::Units::Units::month;
			using Poco::Util::Units::Units::nano;
			using Poco::Util::Units::Units::peta;
			using Poco::Util::Units::Units::pico;
			using Poco::Util::Units::Units::s;
			using Poco::Util::Units::Units::tera;
			using Poco::Util::Units::Units::yocto;
			using Poco::Util::Units::Units::yotta;
			using Poco::Util::Units::Units::zepto;
			using Poco::Util::Units::Units::zetta;

			using Poco::Util::Units::Units::Bq;
			using Poco::Util::Units::Units::C;
			using Poco::Util::Units::Units::Celsius;
			using Poco::Util::Units::Units::F;
			using Poco::Util::Units::Units::Fahrenheit;
			using Poco::Util::Units::Units::Gy;
			using Poco::Util::Units::Units::H;
			using Poco::Util::Units::Units::Hz;
			using Poco::Util::Units::Units::J;
			using Poco::Util::Units::Units::N;
			using Poco::Util::Units::Units::Ohm;
			using Poco::Util::Units::Units::Pa;
			using Poco::Util::Units::Units::S;
			using Poco::Util::Units::Units::Sv;
			using Poco::Util::Units::Units::T;
			using Poco::Util::Units::Units::Unit;
			using Poco::Util::Units::Units::V;
			using Poco::Util::Units::Units::W;
			using Poco::Util::Units::Units::Wb;
			using Poco::Util::Units::Units::acre;
			using Poco::Util::Units::Units::are;
			using Poco::Util::Units::Units::bakers_dozen;
			using Poco::Util::Units::Units::century;
			using Poco::Util::Units::Units::cl;
			using Poco::Util::Units::Units::cm;
			using Poco::Util::Units::Units::cm3;
			using Poco::Util::Units::Units::day;
			using Poco::Util::Units::Units::degree;
			using Poco::Util::Units::Units::degree_minute;
			using Poco::Util::Units::Units::degree_second;
			using Poco::Util::Units::Units::dl;
			using Poco::Util::Units::Units::dozen;
			using Poco::Util::Units::Units::foot;
			using Poco::Util::Units::Units::g;
			using Poco::Util::Units::Units::grad;
			using Poco::Util::Units::Units::hectare;
			using Poco::Util::Units::Units::hour;
			using Poco::Util::Units::Units::inch;
			using Poco::Util::Units::Units::inch2;
			using Poco::Util::Units::Units::kPa;
			using Poco::Util::Units::Units::kat;
			using Poco::Util::Units::Units::km;
			using Poco::Util::Units::Units::knot;
			using Poco::Util::Units::Units::kph;
			using Poco::Util::Units::Units::lb;
			using Poco::Util::Units::Units::liter;
			using Poco::Util::Units::Units::lm;
			using Poco::Util::Units::Units::lx;
			using Poco::Util::Units::Units::m2;
			using Poco::Util::Units::Units::m3;
			using Poco::Util::Units::Units::mach;
			using Poco::Util::Units::Units::meters_per_second;
			using Poco::Util::Units::Units::mg;
			using Poco::Util::Units::Units::mile;
			using Poco::Util::Units::Units::millennium;
			using Poco::Util::Units::Units::millibar;
			using Poco::Util::Units::Units::minute;
			using Poco::Util::Units::Units::ml;
			using Poco::Util::Units::Units::mm;
			using Poco::Util::Units::Units::mm2;
			using Poco::Util::Units::Units::mph;
			using Poco::Util::Units::Units::ms;
			using Poco::Util::Units::Units::nautical_mile;
			using Poco::Util::Units::Units::oz;
			using Poco::Util::Units::Units::percent;
			using Poco::Util::Units::Units::psi;
			using Poco::Util::Units::Units::rad;
			using Poco::Util::Units::Units::rpm;
			using Poco::Util::Units::Units::sr;
			using Poco::Util::Units::Units::tonne;
			using Poco::Util::Units::Units::week;
			using Poco::Util::Units::Units::yard;
			using Poco::Util::Units::Units::year;
		}

		namespace Values {
			using Poco::Util::Units::Values::atto;
			using Poco::Util::Units::Values::centi;
			using Poco::Util::Units::Values::deca;
			using Poco::Util::Units::Values::deci;
			using Poco::Util::Units::Values::exa;
			using Poco::Util::Units::Values::femto;
			using Poco::Util::Units::Values::giga;
			using Poco::Util::Units::Values::hecto;
			using Poco::Util::Units::Values::kilo;
			using Poco::Util::Units::Values::mega;
			using Poco::Util::Units::Values::micro;
			using Poco::Util::Units::Values::milli;
			using Poco::Util::Units::Values::nano;
			using Poco::Util::Units::Values::peta;
			using Poco::Util::Units::Values::pico;
			using Poco::Util::Units::Values::tera;
			using Poco::Util::Units::Values::yocto;
			using Poco::Util::Units::Values::yotta;
			using Poco::Util::Units::Values::zepto;
			using Poco::Util::Units::Values::zetta;

			using Poco::Util::Units::Values::A;
			using Poco::Util::Units::Values::Bq;
			using Poco::Util::Units::Values::C;
			using Poco::Util::Units::Values::Celsius;
			using Poco::Util::Units::Values::F;
			using Poco::Util::Units::Values::Fahrenheit;
			using Poco::Util::Units::Values::Gy;
			using Poco::Util::Units::Values::H;
			using Poco::Util::Units::Values::Hz;
			using Poco::Util::Units::Values::J;
			using Poco::Util::Units::Values::K;
			using Poco::Util::Units::Values::N;
			using Poco::Util::Units::Values::Ohm;
			using Poco::Util::Units::Values::Pa;
			using Poco::Util::Units::Values::S;
			using Poco::Util::Units::Values::Sv;
			using Poco::Util::Units::Values::T;
			using Poco::Util::Units::Values::Unit;
			using Poco::Util::Units::Values::V;
			using Poco::Util::Units::Values::W;
			using Poco::Util::Units::Values::Wb;
			using Poco::Util::Units::Values::acre;
			using Poco::Util::Units::Values::are;
			using Poco::Util::Units::Values::bakers_dozen;
			using Poco::Util::Units::Values::cd;
			using Poco::Util::Units::Values::century;
			using Poco::Util::Units::Values::cl;
			using Poco::Util::Units::Values::cm;
			using Poco::Util::Units::Values::cm3;
			using Poco::Util::Units::Values::day;
			using Poco::Util::Units::Values::degree;
			using Poco::Util::Units::Values::degree_minute;
			using Poco::Util::Units::Values::degree_second;
			using Poco::Util::Units::Values::dl;
			using Poco::Util::Units::Values::dozen;
			using Poco::Util::Units::Values::foot;
			using Poco::Util::Units::Values::g;
			using Poco::Util::Units::Values::grad;
			using Poco::Util::Units::Values::hectare;
			using Poco::Util::Units::Values::hour;
			using Poco::Util::Units::Values::inch;
			using Poco::Util::Units::Values::inch2;
			using Poco::Util::Units::Values::kPa;
			using Poco::Util::Units::Values::kat;
			using Poco::Util::Units::Values::kg;
			using Poco::Util::Units::Values::km;
			using Poco::Util::Units::Values::knot;
			using Poco::Util::Units::Values::kph;
			using Poco::Util::Units::Values::lb;
			using Poco::Util::Units::Values::liter;
			using Poco::Util::Units::Values::lm;
			using Poco::Util::Units::Values::lx;
			using Poco::Util::Units::Values::m;
			using Poco::Util::Units::Values::m2;
			using Poco::Util::Units::Values::m3;
			using Poco::Util::Units::Values::mach;
			using Poco::Util::Units::Values::meters_per_second;
			using Poco::Util::Units::Values::mg;
			using Poco::Util::Units::Values::mile;
			using Poco::Util::Units::Values::millennium;
			using Poco::Util::Units::Values::millibar;
			using Poco::Util::Units::Values::minute;
			using Poco::Util::Units::Values::ml;
			using Poco::Util::Units::Values::mm;
			using Poco::Util::Units::Values::mm2;
			using Poco::Util::Units::Values::mol;
			using Poco::Util::Units::Values::month;
			using Poco::Util::Units::Values::mph;
			using Poco::Util::Units::Values::ms;
			using Poco::Util::Units::Values::nautical_mile;
			using Poco::Util::Units::Values::oz;
			using Poco::Util::Units::Values::percent;
			using Poco::Util::Units::Values::psi;
			using Poco::Util::Units::Values::rad;
			using Poco::Util::Units::Values::rpm;
			using Poco::Util::Units::Values::s;
			using Poco::Util::Units::Values::sr;
			using Poco::Util::Units::Values::tonne;
			using Poco::Util::Units::Values::week;
			using Poco::Util::Units::Values::yard;
			using Poco::Util::Units::Values::year;

			using Poco::Util::Units::Values::operator<<;
		}
	}
	#endif
}
