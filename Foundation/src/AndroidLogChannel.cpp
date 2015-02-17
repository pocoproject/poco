//
// AndroidLogChannel.cpp
//
// $Id: //poco/1.4/Foundation/src/AndroidLogChannel.cpp#2 $
//
// Library: Foundation
// Package: Logging
// Module:  AndroidLogChannel
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AndroidLogChannel.h"
#include "Poco/Message.h"

#include <android/log.h>


namespace Poco {


AndroidLogChannel::AndroidLogChannel(const std::string &tag) :
	_tag(tag)
{
}


AndroidLogChannel::~AndroidLogChannel()
{
}


void AndroidLogChannel::log(const Message& msg)
{
	int prio = ANDROID_LOG_DEBUG;
	switch (msg.getPriority())
	{
	case Message::PRIO_FATAL:
		prio = ANDROID_LOG_FATAL;
		break;
	case Message::PRIO_CRITICAL:
	case Message::PRIO_ERROR:
		prio = ANDROID_LOG_ERROR;
		break;
	case Message::PRIO_WARNING:
		prio = ANDROID_LOG_WARN;
		break;
	case Message::PRIO_NOTICE:
	case Message::PRIO_INFORMATION:
		prio = ANDROID_LOG_INFO;
		break;
	case Message::PRIO_TRACE:
		prio = ANDROID_LOG_VERBOSE;
		break;
	}
	__android_log_print(prio, _tag.c_str(), msg.getText().c_str());
}


} // namespace Poco

