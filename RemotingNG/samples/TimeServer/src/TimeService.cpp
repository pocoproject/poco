//
// TimeService.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TimeService.h"


namespace Services {


TimeService::TimeService()
{
}


TimeService::~TimeService()
{
}


Poco::DateTime TimeService::currentTime() const
{
	Poco::DateTime now;
	return now;
}


} // namespace Services
