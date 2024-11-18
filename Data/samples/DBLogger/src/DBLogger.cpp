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
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/ServerApplication.h"

#include "SQLLogInserter.h"

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

	DBLogger()
	{
		Poco::Data::SQLite::Connector::registerConnector();
		Poco::Data::SQLChannel::registerChannel();
	}

	~DBLogger() override = default;

protected:
	void initialize(Application& self) override
	{
		if (_helpRequested)
		{
			return;
		}

		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);

		const auto& cfg { config() };

		if (!cfg.has("logging.channels.sql.directory"))
		{
			throw Poco::Util::MissingOptionException ("Missing scanning directory.");
		}
		_inserter.setDirectory(cfg.getString("logging.channels.sql.directory") );

		if (!cfg.has("DBLogger.connector"))
		{
			throw Poco::Util::MissingOptionException ("Missing database connector.");
		}
		_inserter.setConnector(cfg.getString("DBLogger.connector") );

		if (!cfg.has("DBLogger.constring"))
		{
			throw Poco::Util::MissingOptionException ("Missing database connection string.");
		}
		_inserter.setConnectionString(cfg.getString("DBLogger.constring") );

		_inserter.setNumWorkers(cfg.getUInt64("DBLogger.workers", 2) );
		_demoMessagesRequested = cfg.getBool("DBLogger.demo", false);

		logger().information("Directory: %s", _inserter.directory());
		logger().information("Database: %s, %s", _inserter.connector(), _inserter.connectionString());
		logger().information("Number of workers: %z", _inserter.numWorkers());

		if (_demoMessagesRequested)
		{
			auto& dl = Poco::Logger::get("SQLDemo");
			auto* sqlChannel = dynamic_cast<Poco::Data::SQLChannel*>(dl.getChannel().get());
			if (sqlChannel == nullptr)
			{
				throw Poco::Util::UnexpectedArgumentException ("SQLDemo logger does not have SQL channel.");
			}

			_tableName = cfg.getString("logging.channels.sql.table");

			// Only delete and create table when creating demo messages
			logger().information("Demo messages: Creating new table: %s", _tableName);

			Poco::Data::Session session (_inserter.connector(), _inserter.connectionString());

			if (!session.isGood() || !session.isConnected())
			{
				throw Poco::Data::NotConnectedException("Can't create demo database.");
			}

			session << ("DROP TABLE IF EXISTS "s + _tableName), now;
			const auto create {
				"CREATE TABLE "s + _tableName +
				" (Source VARCHAR, Name VARCHAR, ProcessId INTEGER, Thread VARCHAR,"s +
				" ThreadId INTEGER, Priority INTEGER, Text VARCHAR, DateTime DATE)"s
			};

			session << create, now;
		}

		_active = true;
		_startTime.update();

		_inserter.start();

		logger().information("Started directory scanning.");

		if (_demoMessagesRequested)
		{
			_sqlSourceThread = std::thread(&DBLogger::createMessages, this);
			logger().information("Started creating demo messages.");
		}
	}


	void uninitialize() override
	{
		if (_helpRequested)
		{
			return;
		}

		logger().information("Request to stop processing.");

		if (_demoMessagesRequested)
		{
			_sqlSourceThread.join();
		}
		_inserter.stop();

		logger().information(
			"Created %z messages, processed %z files in %Ld ms.",
			_created, _inserter.totalProcessed(), (_startTime.elapsed() / 1000)
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
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleHelp))
			);

		options.addOption(
			Option("?", "?", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleHelp))
			);

		options.addOption(
			Option("dir", "d", "directory path to scan for SQL log files")
				.repeatable(false)
				.argument("dir")
				.binding("logging.channels.sql.directory")
			);

		options.addOption(
			Option("connector", "c", "database connector")
				.repeatable(false)
				.argument("type")
				.binding("DBLogger.connector")
			);

		options.addOption(
			Option("constring", "s", "database connection string")
				.repeatable(false)
				.argument("cstr")
				.binding("DBLogger.constring")
			);

		options.addOption(
			Option("workers", "w", "number of workers inserting into database (default 2)")
				.repeatable(false)
				.argument("N")
				.binding("DBLogger.workers")
			);

		// Only command line option
		options.addOption(
			Option("demo", "m", "create demo messages")
				.required(false)
				.repeatable(false)
				.binding("DBLogger.demo")
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
		config().setBool("DBLogger.demo", true);
	}


	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.format(std::cout);
	}


	void createMessages()
	{
		int i {0};
		auto& dl = Poco::Logger::get("SQLDemo");

		while (_active)
		{
			for (int j = 0; j < 100 && _active; ++j)
			{
				dl.debug("%d Informational message", i);
				++i;
				++_created;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}


	int main(const std::vector<std::string>& args) override
	{
		if (!_helpRequested)
		{
			waitForTerminationRequest();

			_active = false;
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested {false};
	bool _demoMessagesRequested {false};

	SQLLogInserter _inserter;

	Poco::Timestamp _startTime;
	bool _active {false};

	std::size_t _created{0};
	std::thread _sqlSourceThread;
	std::string _tableName{"T_POCO_LOG"};
};


POCO_SERVER_MAIN(DBLogger)
