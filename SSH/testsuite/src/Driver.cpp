//
// Driver.cpp
//
// Console-based test driver for Poco SSH.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "CppUnit/TestRunner.h"
#include "SSHTestSuite.h"


int main(int ac, char **av)
{
	std::vector<std::string> args;
	for (int i = 0; i < ac; ++i)
		args.push_back(std::string(av[i]));
	CppUnit::TestRunner runner;
	runner.addTest("SSHTestSuite", SSHTestSuite::suite());
	CppUnitPocoExceptionText (exc);
	return runner.run(args, exc) ? 0 : 1;
}
