//
// WinDriver.cpp
//
// Windows test driver for Poco RemotingNG SOAP.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "WinTestRunner/WinTestRunner.h"
#include "SOAPTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(SOAPTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
