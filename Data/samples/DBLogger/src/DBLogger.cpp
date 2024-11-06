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
#include "Poco/Delegate.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DirectoryWatcher.h"
#include "Poco/FileStream.h"
#include "Poco/Glob.h"
#include "Poco/Thread.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>

using namespace Poco::Data::Keywords;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;

using namespace std::string_literals;

class DBLogger: public Application
{
public:
	DBLogger():
		_helpRequested(false),
		_sqlNameGlob("*.sql")
	{
	}

	~DBLogger()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);

		Poco::Data::SQLite::Connector::registerConnector();

		logger().information("conector: %s, cs: %s", _connector, _connectionString);

		_dataSession = std::make_shared<Poco::Data::Session>(_connector, _connectionString);

		(*_dataSession) << ("DROP TABLE IF EXISTS "s + _tableName), now;
		const auto create {
			"CREATE TABLE "s + _tableName +
			" (Source VARCHAR, Name VARCHAR, ProcessId INTEGER, Thread VARCHAR,"s +
			" ThreadId INTEGER, Priority INTEGER, Text VARCHAR, DateTime DATE)"s
		};

		(*_dataSession) << create, now;

		// Initial scan of the directory to pick existing files
		scanDirectory();

		// Watcher to process files as they are put in the directory
		_dirWatcher = std::make_shared<Poco::DirectoryWatcher>(_directory, Poco::DirectoryWatcher::DW_FILTER_ENABLE_ALL);

		_dirWatcher->itemAdded += Poco::delegate(this, &DBLogger::onItemAdded);
		_dirWatcher->itemRemoved += Poco::delegate(this, &DBLogger::onItemRemoved);
		_dirWatcher->itemModified += Poco::delegate(this, &DBLogger::onItemModified);
		_dirWatcher->itemMovedFrom += Poco::delegate(this, &DBLogger::onItemMovedFrom);
		_dirWatcher->itemMovedTo += Poco::delegate(this, &DBLogger::onItemMovedTo);

		// SQL channel to generate SQL files
		_sqlChannel = new Poco::Data::SQLChannel();
		_sqlChannel->setProperty(Poco::Data::SQLChannel::PROP_DIRECTORY, _directory);
		_sqlChannel->setProperty(Poco::Data::SQLChannel::PROP_TABLE, _tableName);

		_active = true;
		_startTime.update();

		_sqlSourceThread.startFunc(
			[this]() { this->createMessages(); }
		);

		logger().information("Scanning started: %s", _dirWatcher->directory().absolutePath());
	}

	void uninitialize()
	{
		_sqlSourceThread.join();
		_dirWatcher->suspendEvents();

		logger().information(
			"Created %z messages, processed %z messages in %Ld ms.",
			_created, _processed, (_startTime.elapsed() / 1000)
		);

		Application::uninitialize();
	}

	void reinitialize(Application& self)
	{
		Application::reinitialize(self);
	}

	void defineOptions(OptionSet& options)
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
				.argument("connector")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleDbConnector)));

		options.addOption(
			Option("constring", "s", "database connection string")
				.required(true)
				.repeatable(false)
				.argument("cstr")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleDbConnectionString)));

	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
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

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.format(std::cout);
	}

	void onItemAdded(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().trace("Added: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemModified(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().information("Modified: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemMovedFrom(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().trace("Moved from: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemMovedTo(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().trace("Moved to: %s", ev.item.path());
	}

	void onItemRemoved(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().trace("Removed: %s", ev.item.path());
	}

	void processFile(const Poco::File& file)
	{
		// TODO: What to do with symbolic links?
		// TODO: What to do if the file content is modified with correct content after it is added?
		//       Ignore if empty and process on modified?

		if (_dataSession == nullptr || !_dataSession->isGood())
		{
			return;
		}

		if (!file.isFile())
		{
			logger().trace("Not a file: %s", file.absolutePath());
			return;
		}
		if (!_sqlNameGlob.match(file.absolutePath()))
		{
			logger().trace("Not an SQL file: %s", file.absolutePath());
			return;
		}
		logger().debug("Will process: %s", file.absolutePath());

		Poco::File f(file);
		if (!f.exists())
		{
			logger().information("File does not exist: %s", file.absolutePath());
			return;
		}
		try
		{
			Poco::FileInputStream is(f.absolutePath());
			std::stringstream buffer;
			buffer << is.rdbuf();
			const auto& sql { buffer.str() };
			if (!sql.empty())
			{
				auto& s = (*_dataSession);

				s << sql, now;

				s.isGood();

				f.remove();
				++_processed;
			}
			// TODO: What if sql is empty?
		}
		catch (const Poco::Exception& e)
		{
			logger().warning("Failed to process %s: %s", f.absolutePath(), e.displayText());
		}
	}

	void scanDirectory()
	{
		logger().information("Performing initial full scan of directory.");
		Poco::DirectoryIterator diriter(_directory);
		Poco::DirectoryIterator end;
		while (diriter != end)
		{
			processFile(*diriter);
			++diriter;
		}
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
			Poco::Thread::sleep(50);
		}
	}

	int main(const std::vector<std::string>& args)
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
	bool _helpRequested;

	std::string _directory;
	std::string _connector;
	std::string _connectionString;
	const std::string _tableName{"T_POCO_LOG"};

	bool _active {false};

	std::size_t _created{0};
	std::size_t _processed{0};

	Poco::Timestamp _startTime;

	Poco::Glob _sqlNameGlob;
	std::shared_ptr<Poco::DirectoryWatcher> _dirWatcher;
	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
	Poco::Thread _sqlSourceThread;
	std::shared_ptr<Poco::Data::Session> _dataSession;
};


POCO_APP_MAIN(DBLogger)
