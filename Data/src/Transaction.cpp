//
// Transaction.cpp
//
// Library: Data
// Package: DataCore
// Module:  Transaction
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/Transaction.h"
#include "Poco/Exception.h"


namespace Poco::Data {


Transaction::Transaction(Poco::Data::Session& rSession, Poco::Logger* pLogger):
	_rSession(rSession),
	_pLogger(pLogger),
	_autoCommit(_rSession.hasFeature("autoCommit") ? _rSession.getFeature("autoCommit") : false)
{
	begin();
}


Transaction::Transaction(Poco::Data::Session& rSession, bool start):
	_rSession(rSession),
	_pLogger(nullptr),
	_autoCommit(_rSession.hasFeature("autoCommit") ? _rSession.getFeature("autoCommit") : false)
{
	if (start) begin();
}


Transaction::~Transaction()
{
	try
	{
		if (_rSession.isTransaction())
		{
			try
			{
				if (_pLogger)
					_pLogger->debug("Rolling back transaction.");

				_rSession.rollback();
				if (_autoCommit)
					_rSession.setFeature("autoCommit", true);
			}
			catch (Poco::Exception& exc)
			{
				if (_pLogger)
					_pLogger->error("Error while rolling back database transaction: %s", exc.displayText());
			}
			catch (...)
			{
				if (_pLogger)
					_pLogger->error("Error while rolling back database transaction.");
			}
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void Transaction::begin()
{
	if (!_rSession.isTransaction())
	{
		// On an auto-commit session SessionImpl::begin() throws, so turn auto-commit
		// off for the duration of the transaction; it is restored (only if it was on
		// to begin with) by commit(), rollback() and the destructor. Captured per
		// session in the ctor, so sessions already in manual-commit mode are untouched.
		if (_autoCommit)
			_rSession.setFeature("autoCommit", false);
		_rSession.begin();
	}
	else
		throw InvalidAccessException("Transaction in progress.");
}


void Transaction::execute(const std::string& sql, bool doCommit)
{
	if (!_rSession.isTransaction()) begin();
	_rSession << sql, Keywords::now;
	if (doCommit) commit();
}


bool Transaction::execute(const std::vector<std::string>& sql)
{
	return execute(sql, nullptr);
}

bool Transaction::execute(const std::vector<std::string>& sql, std::string* info)
{
	try
	{
		std::vector<std::string>::const_iterator it = sql.begin();
		std::vector<std::string>::const_iterator end = sql.end();
		for (; it != end; ++it)	execute(*it, it + 1 == end ? true : false);
		return true;
	}
	catch (Exception& ex)
	{
		if (_pLogger) _pLogger->log(ex);
		if(info) *info = ex.displayText();
	}

	rollback();
	return false;
}


void Transaction::commit()
{
	if (_pLogger)
		_pLogger->debug("Committing transaction.");

	_rSession.commit();
	if (_autoCommit)
		_rSession.setFeature("autoCommit", true);
}


void Transaction::rollback()
{
	if (_pLogger)
		_pLogger->debug("Rolling back transaction.");

	_rSession.rollback();
	if (_autoCommit)
		_rSession.setFeature("autoCommit", true);
}


} // namespace Poco::Data
