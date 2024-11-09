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

		// TODO: Only delete and create table when creating demo messages?
		logger().information("Database connector: %s, cs: %s",
							 _inserter.connector(), _inserter.connectionString());
		logger().information("Directory: %s", _inserter.directory());
		logger().information("Number of workers: %z", _inserter.numWorkers());

		Poco::Data::Session session (_inserter.connector(), _inserter.connectionString());

		session << ("DROP TABLE IF EXISTS "s + _tableName), now;
		const auto create {
			"CREATE TABLE "s + _tableName +
			" (Source VARCHAR, Name VARCHAR, ProcessId INTEGER, Thread VARCHAR,"s +
			" ThreadId INTEGER, Priority INTEGER, Text VARCHAR, DateTime DATE)"s
		};

		session << create, now;


		_active = true;
		_startTime.update();

		_inserter.start();

		logger().information("Started directory scanning.");

		if (_demoMessagesRequested)
		{
			// SQL channel to generate SQL files
			_sqlChannel = new Poco::Data::SQLChannel();
			_sqlChannel->setProperty(Poco::Data::SQLChannel::PROP_DIRECTORY, _inserter.directory());
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
		_inserter.stop();

		logger().information(
			"Created %z messages, processed %z messages in %Ld ms.",
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

		_inserter.setDirectory(value);
	}

	void handleDbConnector(const std::string& name, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected database connector name");

		_inserter.setConnector(value);
	}

	void handleDbConnectionString(const std::string& name, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected database connection string");

		_inserter.setConnectionString(value);
	}

	void handleNumberOfWorkers(const std::string& name, const std::string& value)
	{
		const auto num = Poco::NumberParser::parseUnsigned(value);
		_inserter.setNumWorkers( std::min(1U, num) );
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
	const std::string _tableName{"T_POCO_LOG"};
	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
};


POCO_SERVER_MAIN(DBLogger)
