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

		logger().information("Scanning started: %s", _dirWatcher->directory().absolutePath());
	}

	void uninitialize()
	{
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
		logger().information("Added: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemModified(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().information("Modified: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemMovedFrom(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().information("Moved from: %s", ev.item.path());
		processFile(ev.item);
	}

	void onItemMovedTo(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().information("Moved to: %s", ev.item.path());
	}

	void onItemRemoved(const Poco::DirectoryWatcher::DirectoryEvent& ev)
	{
		logger().information("Removed: %s", ev.item.path());
	}

	void processFile(const Poco::File& file)
	{
		// TODO: What to do with symbolic links?
		// TODO: What to do if the file content is modified with correct content after it is added?
		//       Ignore if empty and process on modified?

		if (!file.isFile())
		{
			logger().information("Not a file: %s", file.absolutePath());
			return;
		}
		if (!_sqlNameGlob.match(file.absolutePath()))
		{
			logger().information("Not an SQL file: %s", file.absolutePath());
			return;
		}
		logger().information("Will process: %s", file.absolutePath());

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

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			logger().information("Press any key to stop scanning.");
			std::cin.get();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
	std::string _directory;
	Poco::Glob _sqlNameGlob;
	std::shared_ptr<Poco::DirectoryWatcher> _dirWatcher;
	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
};


POCO_APP_MAIN(DBLogger)
