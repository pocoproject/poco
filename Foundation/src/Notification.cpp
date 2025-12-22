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


Notification::Notification(const std::string& name):
	_pName(name.empty() ? nullptr : new std::string(name))
{
}


Notification::~Notification()
{
}


std::string Notification::name() const
{
	return _pName ? *_pName : typeid(*this).name();
}


} // namespace Poco
