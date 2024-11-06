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
#include "Poco/Delegate.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DirectoryWatcher.h"
#include "Poco/Glob.h"
#include "Poco/Thread.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;

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
		_sqlChannel->setProperty("directory", _directory);

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
				.argument("filebasename")
				.callback(OptionCallback<DBLogger>(this, &DBLogger::handleLogDirectory)));

	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleLogDirectory(const std::string& directory, const std::string& value)
	{
		if (value.empty())
			throw Poco::Util::IncompatibleOptionsException("Expected directory name");
		_directory = value;
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
		f.remove();
		++_processed;
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
	bool _active {false};

	std::size_t _created{0};
	std::size_t _processed{0};

	Poco::Timestamp _startTime;

	std::string _directory;
	Poco::Glob _sqlNameGlob;
	std::shared_ptr<Poco::DirectoryWatcher> _dirWatcher;
	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
	Poco::Thread _sqlSourceThread;
};


POCO_APP_MAIN(DBLogger)
