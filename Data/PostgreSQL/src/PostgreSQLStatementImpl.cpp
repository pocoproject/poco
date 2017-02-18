//
// PostgreSQLStatementImpl.cpp
//
// $Id: //poco/1.4/Data/PostgreSQL/src/PostgreSQLStatementImpl.cpp#1 $
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQLStatementImpl
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PostgreSQL/PostgreSQLStatementImpl.h"

namespace Poco {
namespace Data {
namespace PostgreSQL {


PostgreSQLStatementImpl::PostgreSQLStatementImpl(SessionImpl& aSessionImpl)
:	Poco::Data::StatementImpl	(aSessionImpl),
	_statementExecutor			(aSessionImpl.handle()),
	_pBinder					(new Binder),
	_pExtractor					(new Extractor (_statementExecutor)),
	_hasNext					(NEXT_DONTKNOW)
{
}


PostgreSQLStatementImpl::~PostgreSQLStatementImpl()
{
}


std::size_t
PostgreSQLStatementImpl::columnsReturned() const
{
	return _statementExecutor.columnsReturned();
}


int
PostgreSQLStatementImpl::affectedRowCount() const
{
	return _statementExecutor.getAffectedRowCount();
}


const MetaColumn&
PostgreSQLStatementImpl::metaColumn(std::size_t aPosition, std::size_t aDataset) const
{
	// PostgreSql doesn't support multiple result sets
	poco_assert_dbg(aDataset == 0);
	
	return _statementExecutor.metaColumn(aPosition);
}


bool
PostgreSQLStatementImpl::hasNext()
{
	if (NEXT_DONTKNOW == _hasNext)
	{
		if (columnsReturned() == 0)
		{
			return false;
		}

		if (_statementExecutor.fetch())
		{
			_hasNext = NEXT_TRUE;
			return true;
		}

		_hasNext = NEXT_FALSE;
		return false;
	}
	else
		if (NEXT_TRUE == _hasNext)
		{
			return true;
		}

	return false;
}


std::size_t
PostgreSQLStatementImpl::next()
{
	if (! hasNext())
	{
		throw StatementException("No data received");
	}

	Poco::Data::AbstractExtractionVec::iterator it= extractions().begin();
	Poco::Data::AbstractExtractionVec::iterator itEnd = extractions().end();
	
	std::size_t position = 0;

	for (; it != itEnd; ++it)
	{
		(*it)->extract(position);
		position += (*it)->numOfColumnsHandled();
	}

	_hasNext = NEXT_DONTKNOW;

	return 1;
}


bool
PostgreSQLStatementImpl::canBind() const
{
	bool ret = false;

	if ((_statementExecutor.state() >= StatementExecutor::STMT_COMPILED)
		 && ! bindings().empty())
	{
		ret = (*bindings().begin())->canBind();
	}

	return ret;
}


bool
PostgreSQLStatementImpl::canCompile() const
{
	return (_statementExecutor.state() < StatementExecutor::STMT_COMPILED);
}


void
PostgreSQLStatementImpl::compileImpl()
{
	_statementExecutor.prepare(toString());
}


void
PostgreSQLStatementImpl::bindImpl()
{
	Poco::Data::AbstractBindingVec& binds = bindings();

	std::size_t position = 0;
	Poco::Data::AbstractBindingVec::iterator it= binds.begin();
	Poco::Data::AbstractBindingVec::iterator itEnd = binds.end();

	for (; it != itEnd && (*it)->canBind(); ++it)
	{
		(*it)->bind(position);
		position += (*it)->numOfColumnsHandled();
	}

	_pBinder->updateBindVectorToCurrentValues();

	_statementExecutor.bindParams(_pBinder->bindVector());

	_statementExecutor.execute();

	_hasNext = NEXT_DONTKNOW;
}


Poco::Data::AbstractExtractor::Ptr PostgreSQLStatementImpl::extractor()
{
	return _pExtractor;
}


Poco::Data::AbstractBinder::Ptr PostgreSQLStatementImpl::binder()
{
	return _pBinder;
}


} } } // namespace Poco::Data::PostgreSQL
