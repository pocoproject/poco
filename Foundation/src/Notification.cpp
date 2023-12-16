//
// Notification.cpp
//
// Library: Foundation
// Package: Notifications
// Module:  Notification
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Notification.h"
#include <typeinfo>


namespace Poco {


Notification::Notification()
= default;


Notification::~Notification()
= default;


std::string Notification::name() const
{
	return typeid(*this).name();
}


} // namespace Poco
