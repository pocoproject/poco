//
// Statement.cpp
//
// Library: Data
// Package: DataCore
// Module:  Statement
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/Statement.h"
#include "Poco/SQL/SQLException.h"
#include "Poco/SQL/Extraction.h"
#include "Poco/SQL/Session.h"
#include "Poco/SQL/Bulk.h"
#include "Poco/Any.h"
#include "Poco/Tuple.h"
#include "Poco/ActiveMethod.h"
#include <algorithm>


namespace Poco {
namespace SQL {


Statement::Statement(StatementImpl::Ptr pImpl):
	_pImpl(pImpl),
	_async(false)
{
	poco_check_ptr (pImpl);
}


Statement::Statement(Session& rSession):
	_async(false)
{
	reset(rSession);
}


Statement::Statement(const Statement& stmt)
{
	if (stmt.isAsync() && !stmt.done()) wait();
	_pImpl = stmt._pImpl;
	_async = stmt._async;
	_pResult = stmt._pResult;
	_pAsyncExec = stmt._pAsyncExec;
	_arguments = stmt._arguments;
	_pRowFormatter = stmt._pRowFormatter;
}


Statement::Statement(Statement&& stmt)
{
	this->move(std::move(stmt));
}


Statement::~Statement()
{
}


Statement& Statement::operator = (const Statement& stmt)
{
	if (stmt.isAsync() && !stmt.done()) stmt.wait();
	if (this != &stmt)
	{
		Statement tmp(stmt);
		swap(tmp);
	}
	return *this;
}


void Statement::swap(Statement& other)
{
	using std::swap;
	if (this != &other)
	{
		swap(_pImpl, other._pImpl);
		swap(_async, other._async);
		swap(_pAsyncExec, other._pAsyncExec);
		swap(_pResult, other._pResult);
		_arguments.swap(other._arguments);
		swap(_pRowFormatter, other._pRowFormatter);
	}
}


Statement& Statement::operator = (Statement&& stmt)
{
	this->move(std::move(stmt));
	return *this;
}


void Statement::move(Statement&& stmt)
{
	if (stmt.isAsync() && !stmt.done()) stmt.wait();
	_pImpl = stmt._pImpl; stmt._pImpl = nullptr;
	_async = stmt._async; stmt._async = false;
	_pResult = stmt._pResult; stmt._pResult = nullptr;
	_pAsyncExec = stmt._pAsyncExec; stmt._pAsyncExec = nullptr;
	_arguments = std::move(stmt._arguments); stmt._arguments.clear();
	_pRowFormatter = stmt._pRowFormatter; stmt._pRowFormatter = nullptr;
}


Statement& Statement::reset(Session& rSession)
{
	Statement stmt(rSession.createStatementImpl());
	swap(stmt);
	return *this;
}


std::size_t Statement::execute(bool doReset)
{
	Mutex::ScopedLock lock(_mutex);
	bool isDone = done();
	if (initialized() || paused() || isDone)
	{
		if (_arguments.size())
		{
			_pImpl->formatSQL(_arguments);
			_arguments.clear();
		}

		if (!isAsync())
		{
			if (isDone) _pImpl->reset();
			return _pImpl->execute(doReset);
		}
		else
		{
			doAsyncExec();
			return 0;
		}
	}
	else throw InvalidAccessException("Statement still executing.");
}


const Statement::Result& Statement::executeAsync(bool doReset)
{
	Mutex::ScopedLock lock(_mutex);
	if (initialized() || paused() || done())
		return doAsyncExec(doReset);
	else
		throw InvalidAccessException("Statement still executing.");
}


const Statement::Result& Statement::doAsyncExec(bool doReset)
{
	if (done()) _pImpl->reset();
	if (!_pAsyncExec)
		_pAsyncExec = new AsyncExecMethod(_pImpl, &StatementImpl::execute);
	_pResult = new Result((*_pAsyncExec)(doReset));
	return *_pResult;
}


void Statement::setAsync(bool async)
{
	_async = async;
	if (_async && !_pAsyncExec)
		_pAsyncExec = new AsyncExecMethod(_pImpl, &StatementImpl::execute);
}


std::size_t Statement::wait(long milliseconds) const
{
	if (!_pResult) return 0;
	bool success = true;
	if (WAIT_FOREVER != milliseconds)
		success = _pResult->tryWait(milliseconds);
	else
		_pResult->wait();

	if (_pResult->exception())
		throw *_pResult->exception();
	else if (!success)
		throw TimeoutException("Statement timed out.");

	return _pResult->data();
}


const std::string& Statement::getStorage() const
{
	switch (storage())
	{
	case STORAGE_VECTOR:
		return StatementImpl::VECTOR;
	case STORAGE_LIST:
		return StatementImpl::LIST;
	case STORAGE_DEQUE:
		return StatementImpl::DEQUE;
	case STORAGE_UNKNOWN:
		return StatementImpl::UNKNOWN;
	}

	throw IllegalStateException("Invalid storage setting.");
}


Statement& Statement::operator , (Manipulator manip)
{
	manip(*this);
	return *this;
}


Statement& Statement::addBind(AbstractBinding::Ptr pBind)
{
	if (pBind->isBulk())
	{
		if (!_pImpl->isBulkSupported())
			throw InvalidAccessException("Bulk not supported by this session.");

		if(_pImpl->bulkBindingAllowed())
			_pImpl->setBulkBinding();
		else
			throw InvalidAccessException("Bulk and non-bulk binding modes can not be mixed.");
	}
	else _pImpl->forbidBulk();

	_pImpl->addBind(pBind);
	return *this;
}


Statement& Statement::addExtract(AbstractExtraction::Ptr pExtract)
{
	if (pExtract->isBulk())
	{
		if (!_pImpl->isBulkSupported())
			throw InvalidAccessException("Bulk not supported by this session.");

		if(_pImpl->bulkExtractionAllowed())
		{
			Bulk b(pExtract->getLimit());
			_pImpl->setBulkExtraction(b);
		}
		else
			throw InvalidAccessException("Bulk and non-bulk extraction modes can not be mixed.");
	}
	else _pImpl->forbidBulk();

	_pImpl->addExtract(pExtract);
	return *this;
}


Statement& Statement::operator , (const Limit& extrLimit)
{
	if (_pImpl->isBulkExtraction() && _pImpl->extractionLimit() != extrLimit)
		throw InvalidArgumentException("Limit for bulk extraction already set.");

	_pImpl->setExtractionLimit(extrLimit);
	return *this;
}


Statement& Statement::operator , (const Range& extrRange)
{
	if (_pImpl->isBulkExtraction())
		throw InvalidAccessException("Can not set range for bulk extraction.");

	_pImpl->setExtractionLimit(extrRange.lower());
	_pImpl->setExtractionLimit(extrRange.upper());
	return *this;
}


Statement& Statement::operator , (const Bulk& bulk)
{
	if (!_pImpl->isBulkSupported())
			throw InvalidAccessException("Bulk not supported by this session.");

	if (0 == _pImpl->extractions().size() &&
		0 == _pImpl->bindings().size() &&
		_pImpl->bulkExtractionAllowed() &&
		_pImpl->bulkBindingAllowed())
	{
		_pImpl->setBulkExtraction(bulk);
		_pImpl->setBulkBinding();
	}
	else
		throw InvalidAccessException("Can not set bulk operations.");

	return *this;
}


Statement& Statement::operator , (BulkFnType)
{
	const Limit& limit(_pImpl->extractionLimit());
	if (limit.isHardLimit() ||
		limit.isLowerLimit() ||
		Limit::LIMIT_UNLIMITED == limit.value())
	{
		throw InvalidAccessException("Bulk is only allowed with limited extraction,"
			"non-hard and zero-based limits.");
	}

	Bulk bulk(limit);
	_pImpl->setBulkExtraction(bulk);
	_pImpl->setBulkBinding();

	return *this;
}


Session Statement::session()
{
	Poco::AutoPtr<SessionImpl> ps(&impl()->session(), true);
	return Session(ps);
}


void Statement::setTotalRowCount(const std::string& sql)
{
	std::size_t count;
	session() << sql,
		Poco::SQL::Keywords::into(count),
		Poco::SQL::Keywords::now;
	_pImpl->setTotalRowCount(count);
}


} } // namespace Poco::SQL
