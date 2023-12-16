//
// AbstractObserver.cpp
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AbstractObserver.h"


namespace Poco {


AbstractObserver::AbstractObserver()
= default;


AbstractObserver::AbstractObserver(const AbstractObserver& /*observer*/)
= default;


AbstractObserver::~AbstractObserver()
= default;


AbstractObserver& AbstractObserver::operator = (const AbstractObserver& /*observer*/)
= default;


} // namespace Poco
