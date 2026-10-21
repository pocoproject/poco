//
// Serial.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_SERIAL
#include "Poco/Serial/SerialPort.h"
#endif

export module Poco:Serial;

export namespace Poco::Serial {
	#ifdef ENABLE_SERIAL
	using Poco::Serial::SerialPort;
	#endif
}
