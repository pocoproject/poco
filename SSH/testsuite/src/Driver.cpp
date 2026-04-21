//
// Driver.cpp
//
// Console-based test driver for Poco SSH.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "CppUnit/TestRunner.h"
#include "SSHTestSuite.h"
#include <vector>
#include <string>


int main(int ac, char **av)
{
	std::vector<std::string> args;
	args.reserve(ac);
	for (int i = 0; i < ac; ++i)
		args.emplace_back(av[i]);
	CppUnit::TestRunner runner;
	runner.addTest("SSHTestSuite", SSHTestSuite::suite());
	CppUnitPocoExceptionText (exc);
	return runner.run(args, exc) ? 0 : 1;
}
