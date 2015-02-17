//
// Benchmark.cpp
//
// $Id$
//
// This sample shows a benchmark of various mutex implementations.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex.h"
#include "Poco/Stopwatch.h"
#include <iostream>
#include <iomanip>

template<typename Mtx>
void Benchmark(Mtx& mtx, std::string const& label)
{
	Poco::Stopwatch sw;
	sw.start();

	const int LOOP_COUNT = 1000000000;

	mtx.lock();

	for (int i = 0 ; i < LOOP_COUNT ; ++i)
	{
		mtx.unlock();
		mtx.lock();
	}

	mtx.unlock();

	sw.stop();

	std::cout << label << ' ' << sw.elapsed() << " [us]" << std::endl;
}


int main(int argc, char** argv)
{
	{
		Poco::NullMutex mtx;
		Benchmark(mtx, "NullMutex");
	}

	{
		Poco::Mutex mtx(Poco::Mutex::MUTEX_RECURSIVE);
		Benchmark(mtx, "Mutex(MUTEX_RECURSIVE)");
	}

	{
		Poco::Mutex mtx(Poco::Mutex::MUTEX_NONRECURSIVE);
		Benchmark(mtx, "Mutex(MUTEX_NONRECURSIVE)");
	}

	{
		Poco::FastMutex mtx;
		Benchmark(mtx, "FastMutex");
	}

	return 0;
}
