//
// BenchmarkApp.cpp
//
// This application runs performance benchmarks for Poco components.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;


class BenchmarkApp: public Application
	/// Poco Benchmark application that wraps Google Benchmark.
	///
	/// Usage:
	///   benchmark [options]
	///
	/// Options are translated to Google Benchmark equivalents.
{
public:
	BenchmarkApp():
		_helpRequested(false),
		_listTests(false),
		_reportAggregatesOnly(false),
		_countersTabular(false),
		_repetitions(0),
		_format("console"),
		_outFormat("console")
	{
	}

protected:
	void initialize(Application& self) override
	{
		Application::initialize(self);
	}

	void uninitialize() override
	{
		Application::uninitialize();
	}

	void defineOptions(OptionSet& options) override
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleHelp)));

		options.addOption(
			Option("list", "l", "List all available benchmarks and exit.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleList)));

		options.addOption(
			Option("filter", "f", "Run only benchmarks matching <regex>.")
				.required(false)
				.repeatable(false)
				.argument("regex")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleFilter)));

		options.addOption(
			Option("min-time", "t", "Minimum time (in seconds) to run each benchmark.")
				.required(false)
				.repeatable(false)
				.argument("seconds")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleMinTime)));

		options.addOption(
			Option("repetitions", "r", "Number of times to repeat each benchmark.")
				.required(false)
				.repeatable(false)
				.argument("num")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleRepetitions)));

		options.addOption(
			Option("aggregates-only", "a", "Report only aggregates (mean, median, stddev) for repeated benchmarks.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleAggregatesOnly)));

		options.addOption(
			Option("format", "", "Output format: console, json, or csv.")
				.required(false)
				.repeatable(false)
				.argument("fmt")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleFormat)));

		options.addOption(
			Option("output", "o", "Write results to <file>.")
				.required(false)
				.repeatable(false)
				.argument("file")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleOutput)));

		options.addOption(
			Option("output-format", "", "Format for output file: console, json, or csv.")
				.required(false)
				.repeatable(false)
				.argument("fmt")
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleOutFormat)));

		options.addOption(
			Option("tabular", "", "Display counters in tabular format.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<BenchmarkApp>(this, &BenchmarkApp::handleTabular)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleList(const std::string& name, const std::string& value)
	{
		_listTests = true;
	}

	void handleFilter(const std::string& name, const std::string& value)
	{
		_filter = value;
	}

	void handleMinTime(const std::string& name, const std::string& value)
	{
		_minTime = value;
	}

	void handleRepetitions(const std::string& name, const std::string& value)
	{
		_repetitions = std::stoi(value);
	}

	void handleAggregatesOnly(const std::string& name, const std::string& value)
	{
		_reportAggregatesOnly = true;
	}

	void handleFormat(const std::string& name, const std::string& value)
	{
		_format = value;
	}

	void handleOutput(const std::string& name, const std::string& value)
	{
		_outputFile = value;
	}

	void handleOutFormat(const std::string& name, const std::string& value)
	{
		_outFormat = value;
	}

	void handleTabular(const std::string& name, const std::string& value)
	{
		_countersTabular = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[OPTIONS]");
		helpFormatter.setHeader(
			"Poco Benchmark Application\n\n"
			"Runs performance benchmarks for Poco components.\n");
		helpFormatter.setFooter(
			"\nExamples:\n"
			"  benchmark --list\n"
			"  benchmark --filter=\"PatternFormatter.*\"\n"
			"  benchmark --format=json --output=results.json\n"
			"  benchmark --repetitions=5 --aggregates-only\n");
		helpFormatter.format(std::cout);
	}

	int main(const ArgVec& args) override
	{
		if (_helpRequested)
			return Application::EXIT_OK;

		// Check for unprocessed args that look like -- options (common mistake on Windows)
		for (const auto& arg : args)
		{
			if (arg.substr(0, 2) == "--")
			{
				std::cerr << "Error: Unknown option '" << arg << "'\n"
				          << "Note: On Windows, use '/' prefix (e.g., /filter) or single dash (e.g., -f).\n"
				          << "Use /help or -h for usage information.\n";
				return Application::EXIT_USAGE;
			}
		}

		// Build argc/argv for Google Benchmark
		std::vector<std::string> argStorage;
		std::vector<char*> gbArgs;

		std::string progName = commandName();
		gbArgs.push_back(const_cast<char*>(progName.c_str()));

		// Translate our options to Google Benchmark options
		if (_listTests)
		{
			argStorage.push_back("--benchmark_list_tests=true");
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (!_filter.empty())
		{
			argStorage.push_back("--benchmark_filter=" + _filter);
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (!_minTime.empty())
		{
			argStorage.push_back("--benchmark_min_time=" + _minTime);
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (_repetitions > 0)
		{
			argStorage.push_back("--benchmark_repetitions=" + std::to_string(_repetitions));
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (_reportAggregatesOnly)
		{
			argStorage.push_back("--benchmark_report_aggregates_only=true");
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (_format != "console")
		{
			argStorage.push_back("--benchmark_format=" + _format);
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (!_outputFile.empty())
		{
			argStorage.push_back("--benchmark_out=" + _outputFile);
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (_outFormat != "console")
		{
			argStorage.push_back("--benchmark_out_format=" + _outFormat);
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		if (_countersTabular)
		{
			argStorage.push_back("--benchmark_counters_tabular=true");
			gbArgs.push_back(const_cast<char*>(argStorage.back().c_str()));
		}

		int argc = static_cast<int>(gbArgs.size());
		char** argv = gbArgs.data();

		benchmark::Initialize(&argc, argv);

		if (benchmark::ReportUnrecognizedArguments(argc, argv))
			return Application::EXIT_USAGE;

		benchmark::RunSpecifiedBenchmarks();
		benchmark::Shutdown();

		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
	bool _listTests;
	bool _reportAggregatesOnly;
	bool _countersTabular;
	int _repetitions;
	std::string _filter;
	std::string _minTime;
	std::string _format;
	std::string _outputFile;
	std::string _outFormat;
};


POCO_APP_MAIN(BenchmarkApp)
