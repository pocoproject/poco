//
// Benchmark.cpp
//
// This sample shows printing of stack trace captured by Poco::Exception.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <iostream>
#include <sstream>
#include "Poco/Exception.h"


void functionC(int& i)
{
	std::ostringstream ostr;
	if (i>0)
		functionC(--i);
	throw Poco::Exception("thrown from functionC()");
}


void functionB()
{
	int i = 5;
	functionC(i);
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
		std::cerr << e.displayText() << std::endl;
	}

	return 0;
}
