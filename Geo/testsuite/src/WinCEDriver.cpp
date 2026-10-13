//
// WinCEDriver.cpp
//
// Console-based test driver for Windows CE.
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppUnit/TestRunner.h"
#include "GeoTestSuite.h"
#include <cstdlib>


int _tmain(int argc, wchar_t* argv[])
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i)
	{
		char buffer[1024];
		std::wcstombs(buffer, argv[i], sizeof(buffer));
		args.push_back(std::string(buffer));
	}
	CppUnit::TestRunner runner;	
	runner.addTest("GeoTestSuite", GeoTestSuite::suite());
	return runner.run(args) ? 0 : 1;
}
