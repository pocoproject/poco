//
// ActiveRecord.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  ActiveRecord
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"


using namespace Poco::Data::Keywords;
using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {


void ActiveRecordBase::attach(Context::Ptr pContext)
{
	if (_pContext) throw Poco::IllegalStateException("ActiveRecord already has a Context");
	if (!pContext) throw Poco::InvalidArgumentException("Cannot attach to a null Context");

	_pContext = pContext;
}


void ActiveRecordBase::detach()
{
	_pContext.reset();
}


void ActiveRecordBase::create(Context::Ptr pContext)
{
	attach(pContext);
	insert();
}


bool ActiveRecordBase::isValid() const
{
	return true;
}


std::string KeylessActiveRecord::toString() const
{
	return ""s;
}


} } // namespace Poco::ActiveRecord
