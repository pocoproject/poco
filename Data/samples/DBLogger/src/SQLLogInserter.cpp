//
// SQLLogInserter.cpp
//
// This sample demonstrates the Data library recordset row formatting
// and streaming capabilities.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0

#include "SQLLogInserter.h"

#include "Poco/Util/Application.h"

#include <iostream>
#include <filesystem>
#include <fstream>

using namespace Poco::Data::Keywords;

using namespace std::string_literals;

void SQLLogInserter::start()
{
	_dataSession = std::make_shared<Poco::Data::Session>(_connector, _connectionString);

	_active = true;

	_workSet.reserve(MAX_WORKSET_SIZE * 2);

	for (std::size_t i = 0; i < _numWorkers; i++)
	{
		_workers.emplace_back(&SQLLogInserter::runProcessingEntries, this);
	}

	// Thread to scan the directory
	_dirScanThread = std::move(std::thread(&SQLLogInserter::runDirectoryScan, this));
}

void SQLLogInserter::stop()
{
	_active = false;
	_workCondition.notify_all();
	_underflowCondition.notify_all();

	{
		std::unique_lock<std::mutex> l(_workMutex);
		while (!_workSet.empty() || !_processingSet.empty())
		{
			_completedCondition.wait_for(l, std::chrono::milliseconds(5000));
		}
	}

	if (_dirScanThread.joinable())
	{
		_dirScanThread.join();
	}

	// stop all worker threads
	for (auto& w: _workers)
	{
		if (w.joinable()) {
			w.join();
		}
	}
}


std::size_t SQLLogInserter::insertEntries(std::vector<std::string>& entries)
{
	std::unique_lock<std::mutex> l(_workMutex);

	while (_workSet.size() > MAX_WORKSET_SIZE && _active)
	{
		// Prevent creating too large work set
		_underflowCondition.wait_for(l, std::chrono::milliseconds(200));
	}
	if (!_active)
	{
		return 0;
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

	_workSet.insert(entries.begin(), entries.end());
	_workCondition.notify_all();
	return _workSet.size() - wss;
}


std::string SQLLogInserter::popEntry()
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


void SQLLogInserter::removeEntry(std::string entry)
{
	std::unique_lock<std::mutex> l(_workMutex);
	auto i = _processingSet.find(entry);
	if (i != _processingSet.end())
	{
		_processingSet.erase(i);
	}
}


void SQLLogInserter::processFile(std::string& entry)
{
	if (entry.empty())
	{
		return;
	}
	if (!std::filesystem::exists(entry))
	{
		// Directory iterator can pick up files that were already processed
		// Ignore such entries.
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
		// Failed inserting into the database
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


std::size_t SQLLogInserter::scanDirectory()
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


void SQLLogInserter::runDirectoryScan()
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


void SQLLogInserter::runProcessingEntries()
{
	while (!_workSet.empty() || _active)
	{
		auto entry = popEntry();
		processFile(entry);
	}
	_completedCondition.notify_all();
}


