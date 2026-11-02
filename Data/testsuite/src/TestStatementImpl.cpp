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


namespace Poco::Data::Test {


std::atomic<int> TestStatementImpl::_instances(0);
std::atomic<bool> TestStatementImpl::_holdNext(false);
Poco::Event TestStatementImpl::_held;
Poco::Event TestStatementImpl::_released;


TestStatementImpl::TestStatementImpl(SessionImpl& rSession, bool throwOnHasNext):
	Poco::Data::StatementImpl(rSession),
	_compiled(false),
	_throwOnHasNext(throwOnHasNext)
{
	++_instances;
}


TestStatementImpl::~TestStatementImpl()
{
	--_instances;
}


int TestStatementImpl::instances()
{
	return _instances;
}


void TestStatementImpl::holdNextExecution()
{
	_held.reset();
	_released.reset();
	_holdNext = true;
}


bool TestStatementImpl::waitUntilHeld(long milliseconds)
{
	return _held.tryWait(milliseconds);
}


void TestStatementImpl::releaseExecution()
{
	_holdNext = false;
	_released.set();
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
	using Bindings = Poco::Data::AbstractBindingVec;
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


const MetaColumn& TestStatementImpl::metaColumn(std::size_t pos) const
{
	static MetaColumn c(pos, "", MetaColumn::FDT_BOOL, 0);
	return c;
}


bool TestStatementImpl::hasNext()
{
	if (_holdNext.exchange(false))
	{
		_held.set();
		_released.wait();
	}
	if (_throwOnHasNext)
		throw Poco::Data::UnknownDataBaseException();
	return false;
}


std::size_t TestStatementImpl::next()
{
	Poco::Data::AbstractExtractionVec::iterator it    = extractions().begin();
	Poco::Data::AbstractExtractionVec::iterator itEnd = extractions().end();
	std::size_t pos = 0;
	for (; it != itEnd; ++it)
	{
		(*it)->extract(pos);
		pos += (*it)->numOfColumnsHandled();
	}

	return 1u;
}


} // namespace Poco::Data::Test
