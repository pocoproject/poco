//
// TestStatementImpl.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TestStatementImpl.h"
#include "SessionImpl.h"


namespace Poco {
namespace SQL {
namespace Test {


TestStatementImpl::TestStatementImpl(SessionImpl& rSession):
	Poco::SQL::StatementImpl(rSession),
	_compiled(false)
{
}


TestStatementImpl::~TestStatementImpl()
{
}


void TestStatementImpl::compileImpl()
{
	// prepare binding
	_ptrBinder    = new Binder;
	_ptrExtractor = new Extractor;
	_ptrPreparation   = new Preparator;
	_compiled = true;
}


bool TestStatementImpl::canBind() const
{
	return false;
}


void TestStatementImpl::bindImpl()
{
	// bind
	typedef Poco::SQL::AbstractBindingVec Bindings;
	Bindings& binds = bindings();
	if (binds.empty())
		return;

	Bindings::iterator it    = binds.begin();
	Bindings::iterator itEnd = binds.end();
	std::size_t pos = 0;
	for (; it != itEnd && (*it)->canBind(); ++it)
	{
		(*it)->bind(pos);
		pos += (*it)->numOfColumnsHandled();
	}
}


std::size_t TestStatementImpl::columnsReturned() const
{
	return 0;
}


const MetaColumn& TestStatementImpl::metaColumn(std::size_t pos, std::size_t /*rsPos*/) const
{
	static MetaColumn c(pos, "", MetaColumn::FDT_BOOL, 0);
	return c;
}


bool TestStatementImpl::hasNext()
{
	return false;
}


std::size_t TestStatementImpl::next()
{
	Poco::SQL::AbstractExtractionVec::iterator it    = extractions().begin();
	Poco::SQL::AbstractExtractionVec::iterator itEnd = extractions().end();
	std::size_t pos = 0;
	for (; it != itEnd; ++it)
	{
		(*it)->extract(pos);
		pos += (*it)->numOfColumnsHandled();
	}

	return 1u;
}


} } } // namespace Poco::SQL::Test
