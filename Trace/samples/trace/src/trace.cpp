//
// Benchmark.cpp
//
// This sample shows a benchmark of the JSON parser.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Trace/StackTrace.h"
#include "Poco/Trace/StackTraceException.h"
#include <iostream>
#include <sstream>
#include "cpptrace/cpptrace.hpp"

using Poco::Trace::StackTrace;


void functionC()
{
	std::ostringstream ostr;
	StackTrace().printWithSnippets(ostr);
	throw Poco::Exception(ostr.str());
}


void functionB()
{
	std::cout << "Stack trace from functionB:" << std::endl;
	//StackTrace().print(std::cout, false);
	cpptrace::generate_trace().print();
	functionC();
}


void functionA()
{
	functionB();
}

int main()
{
	try
	{
		functionA();
	}
	catch (const Poco::Exception& e)
	{
		std::cerr << "Poco::Exception caught: " << e.displayText() << std::endl;
		//std::cerr << "Stack trace:" << std::endl;
		//e.printStackTrace(std::cerr);
		//Poco::Trace::StackTrace::printExceptionTrace(std::cout, e);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	return 0;
}
