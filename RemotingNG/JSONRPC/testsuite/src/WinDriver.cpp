//
// WinDriver.cpp
//
// Windows test driver for Poco RemotingNG JSONRPC.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "WinTestRunner/WinTestRunner.h"
#include "JSONRPCTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(JSONRPCTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
