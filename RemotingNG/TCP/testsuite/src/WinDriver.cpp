//
// WinDriver.cpp
//
// Windows test driver for Poco RemotingNG SOAP.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "WinTestRunner/WinTestRunner.h"
#include "TCPTestSuite.h"
#include "Poco/Logger.h"
#include "Poco/WindowsConsoleChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
#if _DEBUG
		Poco::Logger::root().setLevel(Poco::Message::PRIO_DEBUG);
		Poco::Logger::root().setChannel(new Poco::FormattingChannel(
			new Poco::PatternFormatter("[%q] %s<%I>: %t"),
			new Poco::WindowsConsoleChannel)
		);
#endif

		CppUnit::WinTestRunner runner;
		runner.addTest(TCPTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
