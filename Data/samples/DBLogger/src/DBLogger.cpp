//
// DBLogger.cpp
//
// This sample demonstrates the Data library recordset row formatting
// and streaming capabilities.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0


#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/ServerApplication.h"

#include <unordered_set>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <fstream>

using namespace Poco::Data::Keywords;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;

using namespace std::string_literals;


class DBLogger: public ServerApplication
{
public:

	using WorkSet = std::unordered_set<std::string>;

	DBLogger():
		_helpRequested(false)
	{
	}

	~DBLogger() override
	{
	}

protected:
	void initialize(Application& self) override
	{
		if (_helpRequested)
		{
			return;
		}

		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);

		Poco::Data::SQLite::Connector::registerConnector();

		logger().information("Database connector: %s, cs: %s", _connector, _connectionString);
		logger().information("Directory: %s", _directory);
		logger().information("Number of workers: %z", _numWorkers);

		_dataSession = std::make_shared<Poco::Data::Session>(_connector, _connectionString);

		(*_dataSession) << ("DROP TABLE IF EXISTS "s + _tableName), now;
		const auto create {
			"CREATE TABLE "s + _tableName +
			" (Source VARCHAR, Name VARCHAR, ProcessId INTEGER, Thread VARCHAR,"s +
			" ThreadId INTEGER, Priority INTEGER, Text VARCHAR, DateTime DATE)"s
		};

		(*_dataSession) << create, now;

		_active = true;
		_startTime.update();

		_workSet.reserve(MAX_WORKSET_SIZE * 2);

		// TODO: Create worker threads
		for (std::size_t i = 0; i < _numWorkers; i++)
		{
			_workers.emplace_back(&DBLogger::processFiles, this);
		}

		// Thread to scan the directory
		_dirScanThread = std::move(std::thread(&DBLogger::runDirectoryScan, this));

		logger().information("Started directory scanning.");

		if (_demoMessagesRequested)
		{
			// SQL channel to generate SQL files
			_sqlChannel = new Poco::Data::SQLChannel();
			_sqlChannel->setProperty(Poco::Data::SQLChannel::PROP_DIRECTORY, _directory);
			_sqlChannel->setProperty(Poco::Data::SQLChannel::PROP_TABLE, _tableName);

			_sqlSourceThread = std::move(std::thread(&DBLogger::createMessages, this));
			logger().information("Started creating demo messages.");
		}
	}

	void uninitialize() override
	{
		if (_helpRequested)
		{
			return;
		}
		if (_demoMessagesRequested)
		{
			_sqlSourceThread.join();
		}
		_dirScanThread.join();

		while (!_workSet.empty())
		{
			logger().information("Waiting for workers to stop. Work %z", _workSet.size());
			Poco::Thread::sleep(200);
		}

		// stop all worker threads
		for (auto& w: _workers)
		{
			if (w.joinable()) {
				w.join();
			}
		}

		logger().information(
			"Created %z messages, processed %z messages in %Ld ms.",
			_created, _processed, (_startTime.elapsed() / 1000)
		);

		Application::uninitialize();
	}

	void reinitialize(Application& self) override
	{
		Application::reinitialize(self);
	}

	void defineOptions(OptionSet& options) override
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleHelp)));

		options.addOption(
			Option("?", "?", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleHelp)));

		options.addOption(
			Option("dir", "d", "directory path to scan for SQL log files")
				.required(true)
				.repeatable(false)
				.argument("dir")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleLogDirectory)));

		options.addOption(
			Option("connector", "c", "database connector")
				.required(true)
				.repeatable(false)
				.argument("type")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleDbConnector)));

		options.addOption(
			Option("constring", "s", "database connection string")
				.required(true)
				.repeatable(false)
				.argument("cstr")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleDbConnectionString)));

		options.addOption(
			Option("workers", "w", "number of workers inserting into database (default 2, max 100)")
				.required(false)
				.repeatable(false)
				.argument("N")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleNumberOfWorkers)));

		options.addOption(
			Option("demo", "m", "create demo messages")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleCreateDemoMessages)));

	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleCreateDemoMessages(const std::string& name, const std::string& value)
	{
		_demoMessagesRequested = true;
	}

	void handleLogDirectory(const std::string& name, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected directory name");
		_directory = value;
	}

	void handleDbConnector(const std::string& name, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected database connector name");
		_connector = value;
	}

	void handleDbConnectionString(const std::string& name, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected database connection string");
		_connectionString = value;
	}

	void handleNumberOfWorkers(const std::string& name, const std::string& value)
	{
		const auto num = Poco::NumberParser::parseUnsigned(value);
		this->_numWorkers = std::min(1U, num);
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.format(std::cout);
	}

	std::size_t insertEntries(std::vector<std::string>& entries)
	{
		std::unique_lock<std::mutex> l(_workMutex);

		while (_workSet.size() > MAX_WORKSET_SIZE && _active)
		{
			// Prevent creating too large work set
			_underflowCondition.wait_for(l, std::chrono::milliseconds(200));
		}

		const auto wss = _workSet.size();
		// Do not re-insert entries that are being processed.
		entries.erase(
			std::remove_if(
				entries.begin(),
				entries.end(),
				[this](const std::string& e) {
					return this->_processingSet.find(e) != this->_processingSet.end();
				}
			),
			entries.end()
		);

		logger().information("Enqueued new entries: %z", entries.size());
		_workSet.insert(entries.begin(), entries.end());
		_workCondition.notify_all();
		return _workSet.size() - wss;
	}

	std::string popEntry()
	{
		std::unique_lock<std::mutex> l(_workMutex);
		while (_workSet.empty() && _active)
		{
			_workCondition.wait_for(l, std::chrono::milliseconds(200));
		}
		if (_workSet.empty())
		{
			// Exited loop because of !_active
			return {};
		}
		auto entry = (*_workSet.begin());
		_processingSet.insert(entry);
		_workSet.erase(_workSet.begin());
		if (_workSet.size() < MAX_WORKSET_SIZE)
		{
			_underflowCondition.notify_all();
		}
		return entry;
	}

	void removeEntry(std::string entry)
	{
		std::unique_lock<std::mutex> l(_workMutex);
		auto i = _processingSet.find(entry);
		if (i != _processingSet.end())
		{
			_processingSet.erase(i);
		}
	}

	void processFile(std::string& entry)
	{
		if (entry.empty())
		{
			return;
		}
		if (!std::filesystem::exists(entry))
		{
			// Directory iterator can still pick up files that were already processed
			removeEntry(entry);
			return;
		}
		bool success {false};
		try
		{
			std::ifstream is(entry);
			std::stringstream buffer;
			buffer << is.rdbuf();
			const auto& sql { buffer.str() };

			if (!sql.empty())
			{
				auto& s = (*_dataSession);

				s << sql, now;

				++_processed;
				success = true;
			}
		}
		catch (const Poco::Exception& e)
		{
			logger().warning("Failed to insert to database %s: %s", entry, e.displayText());
		}
		if (success)
		{
			std::filesystem::remove(entry);
		}
		else
		{
			std::filesystem::path newPath {entry};
			newPath.replace_extension("err"s);
			std::filesystem::rename(entry, newPath);
		}
		removeEntry(entry);
	}

	std::size_t scanDirectory()
	{
		std::vector<std::string> newEntries;
		newEntries.reserve(1000);
		std::filesystem::directory_iterator diriter(_directory, std::filesystem::directory_options::skip_permission_denied);
		for (auto& entry: diriter)
		{
			if (!_active)
			{
				return 0;
			}
			if (_dataSession == nullptr || !_dataSession->isGood())
			{
				// Do not process files if database session is not healthy.
				// Files will be processed later.
				return 0;
			}

			if (!std::filesystem::exists(entry))
			{
				continue;
			}
			if (!entry.is_regular_file())
			{
				continue;
			}
			if (entry.path().extension() != ".sql"s)
			{
				continue;
			}
			newEntries.push_back(entry.path());
		}
		return insertEntries(newEntries);
	}

	void createMessages()
	{
		int i {0};
		while (_active)
		{
			for (int j = 0; j < 100 && _active; ++j)
			{
				Poco::Message msg("SQL Source", Poco::format("%d Informational message", i), Poco::Message::PRIO_INFORMATION);
				_sqlChannel->log(msg);
				++i;
				++_created;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void runDirectoryScan()
	{
		while (_active)
		{
			const auto scheduled = scanDirectory();
			if (scheduled == 0)
			{
				// No new files to be scheduled. Wait a bit.
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	}

	void processFiles()
	{
		while (!_workSet.empty() || _active)
		{
			auto entry = popEntry();
			processFile(entry);
		}
	}

	int main(const std::vector<std::string>& args) override
	{
		if (!_helpRequested)
		{
			logger().information("Press any key to stop scanning.");
			std::cin.get();

			_active = false;
		}
		return Application::EXIT_OK;
	}

private:
	static constexpr std::size_t MAX_WORKSET_SIZE {1000};
	bool _helpRequested {false};
	bool _demoMessagesRequested {false};

	std::string _directory;
	std::string _connector;
	std::string _connectionString;
	std::size_t	_numWorkers {2};

	const std::string _tableName{"T_POCO_LOG"};

	bool _active {false};

	std::size_t _created{0};
	std::size_t _processed{0};

	Poco::Timestamp _startTime;

	std::thread _sqlSourceThread;
	std::thread _dirScanThread;

	WorkSet _workSet;
	WorkSet _processingSet;
	std::mutex _workMutex;
	std::condition_variable _workCondition;
	std::condition_variable _underflowCondition;

	std::vector<std::thread> _workers;

	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
	std::shared_ptr<Poco::Data::Session> _dataSession;
};


POCO_SERVER_MAIN(DBLogger)
