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

#ifndef SQLLogInserter_INCLUDED
#define SQLLogInserter_INCLUDED

#include "Poco/Data/SQLChannel.h"

#include <unordered_set>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace Poco::Data::Keywords;
using namespace std::string_literals;

class SQLLogInserter
{
public:

	SQLLogInserter() = default;
	~SQLLogInserter() = default;

	void start();
	void stop();

	void setDirectory(const std::string& directory);
	std::string directory() const;

	void setConnector(const std::string& connector);
	std::string connector() const;

	void setConnectionString(const std::string& cs);
	std::string connectionString() const;

	void setNumWorkers(std::size_t number);
	std::size_t numWorkers() const;

	std::size_t totalProcessed() const;

private:

	using WorkSet = std::unordered_set<std::string>;

	std::size_t insertEntries(std::vector<std::string>& entries);
	std::string popEntry();
    void removeEntry(const std::string &entry);
    void processFile(const std::string& entry);
	std::size_t scanDirectory();

	void runDirectoryScan();
	void runProcessingEntries();

	static constexpr std::size_t MAX_WORKSET_SIZE {1000};

	std::string _directory;
	std::string _connector;
	std::string _connectionString;
	std::size_t	_numWorkers {2};

	bool _active {false};
	bool _doneProcessing {false};

	std::size_t _processed{0};

	std::thread _dirScanThread;

	WorkSet _workSet;
	WorkSet _processingSet;
	std::mutex _workMutex;
	std::condition_variable _workCondition;
	std::condition_variable _underflowCondition;
	std::condition_variable _completedCondition;

	std::vector<std::thread> _workers;

	Poco::AutoPtr<Poco::Data::SQLChannel> _sqlChannel;
	std::shared_ptr<Poco::Data::Session> _dataSession;
};

inline void SQLLogInserter::setDirectory(const std::string& directory)
{
	_directory = directory;
}

inline std::string SQLLogInserter::directory() const
{
	return _directory;
}

inline void SQLLogInserter::setConnector(const std::string& connector)
{
	_connector = connector;
}

inline std::string SQLLogInserter::connector() const
{
	return _connector;
}

inline void SQLLogInserter::setConnectionString(const std::string& cs)
{
	_connectionString = cs;
}

inline std::string SQLLogInserter::connectionString() const
{
	return _connectionString;
}

inline void SQLLogInserter::setNumWorkers(std::size_t number)
{
	_numWorkers = number;
}

inline std::size_t SQLLogInserter::numWorkers() const
{
	return _numWorkers;
}

inline std::size_t SQLLogInserter::totalProcessed() const
{
	return _processed;
}

#endif // SQLLogInserter_INCLUDED
