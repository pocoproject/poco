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


#ifndef POCO_DATA_NO_SQL_PARSER
#include "SQLParser.h"
#endif
#include "Poco/Data/Statement.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/Extraction.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Bulk.h"
#include "Poco/Any.h"
#include "Poco/Tuple.h"
#include "Poco/ActiveMethod.h"
#include <algorithm>


namespace Poco {
namespace Data {


Statement::Statement(StatementImpl::Ptr pImpl):
#ifndef POCO_DATA_NO_SQL_PARSER
	_pParseResult(new Parser::SQLParserResult()),
#endif
	_pImpl(pImpl),
	_async(false)
{
	poco_check_ptr (pImpl);
}


Statement::Statement(Session& session):
	_async(false)
{
	reset(session);
}


Statement::Statement(const Statement& stmt):
#ifndef POCO_DATA_NO_SQL_PARSER
	_pParseResult(stmt._pParseResult),
	_parseError(stmt._parseError),
#endif
	_pImpl(stmt._pImpl),
	_async(stmt._async),
	_pResult(stmt._pResult),
	_pAsyncExec(stmt._pAsyncExec),
	_arguments(stmt._arguments),
	_pRowFormatter(stmt._pRowFormatter),
	_stmtString(stmt._stmtString)
{
}


Statement::Statement(Statement&& stmt) noexcept:
#ifndef POCO_DATA_NO_SQL_PARSER
	_pParseResult(std::move(stmt._pParseResult)),
	_parseError(std::move(stmt._parseError)),
#endif
	_pImpl(std::move(stmt._pImpl)),
	_async(std::move(stmt._async)),
	_pResult(std::move(stmt._pResult)),
	_pAsyncExec(std::move(stmt._pAsyncExec)),
	_arguments(std::move(stmt._arguments)),
	_pRowFormatter(std::move(stmt._pRowFormatter)),
	_stmtString(std::move(stmt._stmtString))
{
	stmt._pImpl = nullptr;
	stmt._async = false;
	stmt._pResult = nullptr;
	stmt._pAsyncExec = nullptr;
	stmt._arguments.clear();
	stmt._pRowFormatter = nullptr;
	_stmtString.clear();
#ifndef POCO_DATA_NO_SQL_PARSER
	_parseError.clear();
#endif
}


Statement::~Statement()
{
}


Statement& Statement::operator = (const Statement& stmt)
{
	Statement tmp(stmt);
	swap(tmp);
	return *this;
}


Statement& Statement::operator = (Statement&& stmt) noexcept
{
#ifndef POCO_DATA_NO_SQL_PARSER
	_pParseResult = std::move(stmt._pParseResult);
	_parseError = std::move(stmt._parseError);
	_parseError.clear();
#endif
	_pImpl = std::move(stmt._pImpl);
	stmt._pImpl = nullptr;
	_async = std::move(stmt._async);
	stmt._async = false;
	_pResult = std::move(stmt._pResult);
	stmt._pResult = nullptr;
	_pAsyncExec = std::move(stmt._pAsyncExec);
	stmt._pAsyncExec = nullptr;
	_arguments = std::move(stmt._arguments);
	stmt._arguments.clear();
	_pRowFormatter = std::move(stmt._pRowFormatter);
	stmt._pRowFormatter = nullptr;
	_stmtString = std::move(stmt._stmtString);
	_stmtString.clear();

	return *this;
}

void Statement::swap(Statement& other) noexcept
{
	using std::swap;
#ifndef POCO_DATA_NO_SQL_PARSER
	swap(_pParseResult, other._pParseResult);
	swap(_parseError, other._parseError);
#endif
	swap(_pImpl, other._pImpl);
	swap(_async, other._async);
	swap(_pResult, other._pResult);
	swap(_pAsyncExec, other._pAsyncExec);
	_arguments.swap(other._arguments);
	swap(_pRowFormatter, other._pRowFormatter);
	swap(_stmtString, other._stmtString);
}


Statement& Statement::reset(Session& session)
{
	Statement stmt(session.createStatementImpl());
	swap(stmt);
	return *this;
}


Statement& Statement::reset()
{
	Statement stmt(_pImpl->session().createStatementImpl());
	swap(stmt);
	return *this;
}

Optional<std::size_t> Statement::statementsCount() const
{
	Optional<std::size_t> ret;
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		ret = _pParseResult->size();
#endif
	return ret;
}


Optional<bool> Statement::parse()
{
	Optional<bool> result;
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_stmtString.empty()) toString();
	if (!_stmtString.empty())
	{
		_pParseResult->reset();
		Parser::SQLParser::parse(_stmtString, _pParseResult.get());
		result = _pParseResult->isValid();
		if (!result.value())
		{
			Poco::format(_parseError, "%s (line %d, pos %d)",
				std::string(_pParseResult->errorMsg()),
				_pParseResult->errorLine(),
				_pParseResult->errorColumn());
		}
	}
#endif
	return result;
}


#ifndef POCO_DATA_NO_SQL_PARSER

bool Statement::isType(unsigned int type) const
{
	const auto st = static_cast<Parser::StatementType>(type);
	std::size_t sz = _pParseResult->size();
	if (sz)
	{
		for (int i = 0; i < sz; ++i)
		{
			if (_pParseResult->getStatement(i)->type() != st)
				return false;
		}
		return true;
	}
	return false;
}


bool Statement::hasType(unsigned int type) const
{
	const auto st = static_cast<Parser::StatementType>(type);
	for (int i = 0; i < _pParseResult->size(); ++i)
	{
		if (_pParseResult->getStatement(i)->type() == st)
			return true;
	}
	return false;
}


#endif //  POCO_DATA_NO_SQL_PARSER


const std::string& Statement::parseError()
{
#ifdef POCO_DATA_NO_SQL_PARSER
	static std::string empty;
	return empty;
#else
	return _parseError;
#endif
}


Optional<bool> Statement::isSelect() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return isType(Parser::StatementType::kStmtSelect);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::isInsert() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return isType(Parser::StatementType::kStmtInsert);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::isUpdate() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return isType(Parser::StatementType::kStmtUpdate);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::isDelete() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return isType(Parser::StatementType::kStmtDelete);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::hasSelect() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return hasType(Parser::StatementType::kStmtSelect);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::hasInsert() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return hasType(Parser::StatementType::kStmtInsert);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::hasUpdate() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return hasType(Parser::StatementType::kStmtUpdate);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


Optional<bool> Statement::hasDelete() const
{
#ifndef POCO_DATA_NO_SQL_PARSER
	if (_pImpl->session().shouldParse())
		return hasType(Parser::StatementType::kStmtDelete);
	else return Optional<bool>();
#else
	return Optional<bool>();
#endif
}


void Statement::formatQuery()
{
	if (_arguments.size())
	{
		_pImpl->formatSQL(_arguments);
		_arguments.clear();
	}
}


void Statement::checkBeginTransaction()
{
	SessionImpl& session = _pImpl->session();
	if (!session.isTransaction() && !session.isAutocommit()) {
		if (session.shouldParse())
		{
			auto result = parse();
			if (result.isSpecified() && result.value() && !isSelect().value())
				session.begin();
		} else
		{
			session.begin();
		}
	}
}


std::size_t Statement::execute(bool reset)
{
	Mutex::ScopedLock lock(_mutex);
	bool isDone = done();
	if (initialized() || paused() || isDone)
	{
		formatQuery();
		checkBeginTransaction();

		if (!isAsync())
		{
			if (isDone) _pImpl->reset();
			return _pImpl->execute(reset);
		}
		else
		{
			doAsyncExec();
			return 0;
		}
	}
	else throw InvalidAccessException("Statement still executing.");
}


void Statement::executeDirect(const std::string& query)
{
	Mutex::ScopedLock lock(_mutex);
	bool isDone = done();
	if (initialized() || paused() || isDone)
	{
		formatQuery();
		checkBeginTransaction();

		if (!isAsync())
		{
			if (isDone) _pImpl->reset();
			return _pImpl->executeDirect(query);
		}
		else throw InvalidAccessException("Cannot be executed async.");
	}
	else throw InvalidAccessException("Statement still executing.");
}


const Statement::Result& Statement::executeAsync(bool reset)
{
	Mutex::ScopedLock lock(_mutex);
	if (initialized() || paused() || done())
		return doAsyncExec(reset);
	else
		throw InvalidAccessException("Statement still executing.");
}


const Statement::Result& Statement::doAsyncExec(bool reset)
{
	if (done()) _pImpl->reset();
	if (!_pAsyncExec)
		_pAsyncExec = new AsyncExecMethod(_pImpl, &StatementImpl::execute);
	_pResult = new Result((*_pAsyncExec)(reset));
	return *_pResult;
}


void Statement::setAsync(bool async)
{
	_async = async;
	if (_async && !_pAsyncExec)
		_pAsyncExec = new AsyncExecMethod(_pImpl, &StatementImpl::execute);
}


std::size_t Statement::wait(long milliseconds)
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


} } // namespace Poco::Data
