//
// FastLogger.cpp
//
// This sample demonstrates the FastLogger class, a high-performance
// asynchronous logger built on the Quill logging library.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/FastLogger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"


using Poco::FastLogger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;


int main(int argc, char** argv)
{
	// set up two channel chains - one to the
	// console and the other one to a log file.
	FormattingChannel* pFCConsole = new FormattingChannel(new PatternFormatter("[%O] %s: %p: %t"));
	pFCConsole->setChannel(new ConsoleChannel);
	pFCConsole->open();

	FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	pFCFile->setChannel(new FileChannel("sample.log"));
	pFCFile->open();

	// create two FastLogger objects - one for
	// each channel chain.
	FastLogger& consoleLogger = FastLogger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);
	FastLogger& fileLogger    = FastLogger::create("FileLogger", pFCFile, Message::PRIO_WARNING);

	// log some messages
	consoleLogger.error("An error message");
	fileLogger.error("An error message");

	consoleLogger.warning("A warning message");
	fileLogger.error("A warning message");

	consoleLogger.information("An information message");
	fileLogger.information("An information message");

	consoleLogger.information("Another informational message");
	consoleLogger.warning("A warning message with arguments: %d, %d", 1, 2);

	FastLogger::get("ConsoleLogger").error("Another error message");

	// Flush all pending messages before exit
	consoleLogger.flush();
	fileLogger.flush();

	// Shutdown the FastLogger backend
	FastLogger::shutdown();

	return 0;
}
