//
// AsyncWait.cpp
//
// Library: DataTest
// Package: DataTestCore
// Module:  AsyncWait
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/Test/AsyncWait.h"
#include "Poco/Data/Statement.h"
#include "Poco/Bugcheck.h"


namespace Poco::Data::Test {


AsyncWait::AsyncWait(Statement& stmt):
	_stmt(stmt)
{
}


AsyncWait::~AsyncWait()
{
	try
	{
		_stmt.wait();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} // namespace Poco::Data::Test
