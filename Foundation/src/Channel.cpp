//
// Channel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  Channel
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Channel.h"
#include "Poco/Message.h"


namespace Poco {


Channel::Channel()
{
}


Channel::~Channel()
{
}


void Channel::open()
{
}


void Channel::close()
{
}


void Channel::log(Message&& msg)
{
	// Default implementation forwards to log(const Message&).
	// Subclasses that buffer messages (e.g., AsyncChannel) should
	// override to take advantage of move semantics.
	log(static_cast<const Message&>(msg));
}


void Channel::setProperty(const std::string& name, const std::string& /*value*/)
{
	throw PropertyNotSupportedException(name);
}


std::string Channel::getProperty(const std::string& name) const
{
	throw PropertyNotSupportedException(name);
}


} // namespace Poco
