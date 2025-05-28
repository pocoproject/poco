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
#include "Poco/Exception.h"


namespace Poco {

AbstractObserver::AbstractObserver() = default;
AbstractObserver::AbstractObserver(const AbstractObserver& /*observer*/) = default;
AbstractObserver::AbstractObserver(AbstractObserver&& /*observer*/) = default;
AbstractObserver::~AbstractObserver() = default;
AbstractObserver& AbstractObserver::operator = (const AbstractObserver& /*observer*/) = default;
AbstractObserver& AbstractObserver::operator = (AbstractObserver&& /*observer*/) = default;

NotificationResult AbstractObserver::notifySynchronously(Notification* pNf) const
{
	throw Poco::NotImplementedException("Synchronous notification not implemented.");
}

bool AbstractObserver::acceptsSynchronously() const
{
	return false;
}

} // namespace Poco
