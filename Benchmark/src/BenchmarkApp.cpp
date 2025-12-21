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
#include <map>
#include <regex>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <set>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;


/// Custom reporter that collects benchmark results and prints a comparison summary.
/// Groups benchmarks by prefix (e.g., "NotificationQueues_") and compares pairs.
class SummaryReporter : public benchmark::ConsoleReporter
{
public:
	struct BenchmarkResult
	{
		std::string name;
		double realTimeSeconds = 0;  // Original time in seconds for comparison
		double displayTime = 0;      // Converted time for display (wall time)
		std::string timeUnit;        // Unit for wall time display
		double cpuTimeSeconds = 0;   // Original CPU time in seconds for comparison
		double cpuDisplayTime = 0;   // Converted CPU time for display
		std::string cpuTimeUnit;     // Unit for CPU time display
		double itemsPerSecond = 0;
	};

	void ReportRuns(const std::vector<Run>& reports) override
	{
		// First, let the console reporter do its normal output
		ConsoleReporter::ReportRuns(reports);

		// Collect results for summary
		for (const auto& run : reports)
		{
			if (run.skipped != benchmark::internal::NotSkipped)
				continue;

			// Skip aggregate runs (mean, median, stddev)
			if (!run.aggregate_name.empty())
				continue;

			BenchmarkResult result;
			result.name = run.benchmark_name();
			result.realTimeSeconds = run.real_accumulated_time / run.iterations;
			result.cpuTimeSeconds = run.cpu_accumulated_time / run.iterations;

			// Get items_per_second counter if available
			auto it = run.counters.find("items_per_second");
			if (it != run.counters.end())
			{
				result.itemsPerSecond = it->second;
			}

			// Determine wall time unit based on magnitude
			if (result.realTimeSeconds < 1e-6)
			{
				result.displayTime = result.realTimeSeconds * 1e9;
				result.timeUnit = "ns";
			}
			else if (result.realTimeSeconds < 1e-3)
			{
				result.displayTime = result.realTimeSeconds * 1e6;
				result.timeUnit = "us";
			}
			else if (result.realTimeSeconds < 1)
			{
				result.displayTime = result.realTimeSeconds * 1e3;
				result.timeUnit = "ms";
			}
			else
			{
				result.displayTime = result.realTimeSeconds;
				result.timeUnit = "s";
			}

			// Determine CPU time unit based on magnitude
			if (result.cpuTimeSeconds < 1e-6)
			{
				result.cpuDisplayTime = result.cpuTimeSeconds * 1e9;
				result.cpuTimeUnit = "ns";
			}
			else if (result.cpuTimeSeconds < 1e-3)
			{
				result.cpuDisplayTime = result.cpuTimeSeconds * 1e6;
				result.cpuTimeUnit = "us";
			}
			else if (result.cpuTimeSeconds < 1)
			{
				result.cpuDisplayTime = result.cpuTimeSeconds * 1e3;
				result.cpuTimeUnit = "ms";
			}
			else
			{
				result.cpuDisplayTime = result.cpuTimeSeconds;
				result.cpuTimeUnit = "s";
			}

			_results.push_back(result);
		}
	}

	void Finalize() override
	{
		ConsoleReporter::Finalize();
		printSummary();
	}

private:
	std::vector<BenchmarkResult> _results;

	void printSummary()
	{
		if (_results.size() < 2)
			return;

		// Group results by category prefix
		std::map<std::string, std::vector<BenchmarkResult>> groups;

		for (const auto& result : _results)
		{
			// Extract category (first part before underscore)
			auto pos = result.name.find('_');
			if (pos != std::string::npos)
			{
				std::string category = result.name.substr(0, pos);
				groups[category].push_back(result);
			}
		}

		// Print summary for each group that has comparison pairs
		for (const auto& [category, results] : groups)
		{
			printGroupSummary(category, results);
		}
	}

	void printGroupSummary(const std::string& category, const std::vector<BenchmarkResult>& results)
	{
		if (results.size() < 2)
			return;

		// Group results by test suffix (e.g., "File", "ShortMsg")
		// Map: suffix -> (implementation_name -> result)
		std::map<std::string, std::map<std::string, BenchmarkResult>> testGroups;

		for (const auto& result : results)
		{
			std::string impl = extractImplementation(result.name);
			std::string suffix = extractSuffix(result.name);
			if (!suffix.empty() && !impl.empty())
			{
				testGroups[suffix][impl] = result;
			}
		}

		// Need at least one test with multiple implementations
		bool hasComparisons = false;
		for (const auto& [suffix, impls] : testGroups)
		{
			if (impls.size() >= 2)
			{
				hasComparisons = true;
				break;
			}
		}
		if (!hasComparisons)
			return;

		// Collect all implementation names across all tests, sorted alphabetically
		// The first one alphabetically becomes the baseline
		std::set<std::string> allImpls;
		for (const auto& [suffix, impls] : testGroups)
		{
			for (const auto& [impl, result] : impls)
			{
				allImpls.insert(impl);
			}
		}

		if (allImpls.empty())
			return;

		std::string baselineImpl = *allImpls.begin();
		std::vector<std::string> otherImpls(std::next(allImpls.begin()), allImpls.end());

		// Print wall time summary
		printTimeSummary(category, "Wall Time", testGroups, baselineImpl, otherImpls, false);

		// Print CPU time summary
		printTimeSummary(category, "CPU Time", testGroups, baselineImpl, otherImpls, true);
	}

	void printTimeSummary(
		const std::string& category,
		const std::string& timeType,
		const std::map<std::string, std::map<std::string, BenchmarkResult>>& testGroups,
		const std::string& baselineImpl,
		const std::vector<std::string>& otherImpls,
		bool useCpuTime)
	{
		// Calculate column widths
		size_t testColWidth = 4;  // "Test"
		for (const auto& [suffix, impls] : testGroups)
		{
			if (impls.size() >= 2)
				testColWidth = std::max(testColWidth, suffix.length());
		}
		testColWidth += 2;  // Padding

		size_t baselineColWidth = std::max(baselineImpl.length(), size_t(12)) + 2;
		size_t implColWidth = 12;
		for (const auto& impl : otherImpls)
		{
			implColWidth = std::max(implColWidth, impl.length());
		}
		implColWidth += 14;  // Room for " (xx.xx x)" suffix

		size_t totalWidth = testColWidth + baselineColWidth + implColWidth * otherImpls.size();

		std::cout << "\n";
		std::cout << std::string(totalWidth, '=') << "\n";
		std::cout << "  " << category << " Summary (" << timeType << ")\n";
		std::cout << std::string(totalWidth, '=') << "\n";

		// Print header
		std::cout << std::left << std::setw(testColWidth) << "Test"
		          << std::right << std::setw(baselineColWidth) << baselineImpl;
		for (const auto& impl : otherImpls)
		{
			std::cout << std::setw(implColWidth) << impl;
		}
		std::cout << "\n";
		std::cout << std::string(totalWidth, '-') << "\n";

		// Print each test row
		for (const auto& [suffix, impls] : testGroups)
		{
			if (impls.size() < 2)
				continue;

			std::cout << std::left << std::setw(testColWidth) << suffix;

			// Print baseline time
			auto baselineIt = impls.find(baselineImpl);
			if (baselineIt != impls.end())
			{
				double dispTime = useCpuTime ? baselineIt->second.cpuDisplayTime : baselineIt->second.displayTime;
				const std::string& unit = useCpuTime ? baselineIt->second.cpuTimeUnit : baselineIt->second.timeUnit;
				std::ostringstream timeStr;
				timeStr << std::fixed << std::setprecision(1) << dispTime << " " << unit;
				std::cout << std::right << std::setw(baselineColWidth) << timeStr.str();
			}
			else
			{
				std::cout << std::right << std::setw(baselineColWidth) << "-";
			}

			// Print other implementations with speedup
			for (const auto& impl : otherImpls)
			{
				auto implIt = impls.find(impl);
				if (implIt != impls.end() && baselineIt != impls.end())
				{
					double baseTime = useCpuTime ? baselineIt->second.cpuTimeSeconds : baselineIt->second.realTimeSeconds;
					double implTime = useCpuTime ? implIt->second.cpuTimeSeconds : implIt->second.realTimeSeconds;
					double dispTime = useCpuTime ? implIt->second.cpuDisplayTime : implIt->second.displayTime;
					const std::string& unit = useCpuTime ? implIt->second.cpuTimeUnit : implIt->second.timeUnit;

					double speedup = baseTime / implTime;
					std::ostringstream str;
					str << std::fixed << std::setprecision(1) << dispTime << " " << unit << " (";
					if (speedup >= 1.0)
						str << std::setprecision(1) << speedup << "x)";
					else
						str << "-" << std::setprecision(2) << (1.0/speedup) << "x)";

					// Use green for faster, red for slower
					if (speedup >= 1.0)
						std::cout << "\033[32m";  // Green
					else
						std::cout << "\033[31m";  // Red
					std::cout << std::right << std::setw(implColWidth) << str.str();
					std::cout << "\033[0m";   // Reset
				}
				else if (implIt != impls.end())
				{
					double dispTime = useCpuTime ? implIt->second.cpuDisplayTime : implIt->second.displayTime;
					const std::string& unit = useCpuTime ? implIt->second.cpuTimeUnit : implIt->second.timeUnit;
					std::ostringstream timeStr;
					timeStr << std::fixed << std::setprecision(1) << dispTime << " " << unit;
					std::cout << std::right << std::setw(implColWidth) << timeStr.str();
				}
				else
				{
					std::cout << std::right << std::setw(implColWidth) << "-";
				}
			}
			std::cout << "\n";
		}

		std::cout << std::string(totalWidth, '=') << "\n";
	}

	std::string extractImplementation(const std::string& name)
	{
		// Extract the implementation name (second part between underscores)
		// e.g., "Loggers_AsyncChannel_File" -> "AsyncChannel"
		auto first = name.find('_');
		if (first == std::string::npos)
			return "";

		auto second = name.find('_', first + 1);
		if (second == std::string::npos)
			return name.substr(first + 1);

		return name.substr(first + 1, second - first - 1);
	}

	std::string extractSuffix(const std::string& name)
	{
		// Extract the part after the second underscore
		// e.g., "Loggers_AsyncChannel_File" -> "File"
		auto first = name.find('_');
		if (first == std::string::npos)
			return "";

		auto second = name.find('_', first + 1);
		if (second == std::string::npos)
			return "";

		return name.substr(second + 1);
	}
};


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

		// Use custom reporter that prints summary at end
		SummaryReporter reporter;
		benchmark::RunSpecifiedBenchmarks(&reporter);
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
