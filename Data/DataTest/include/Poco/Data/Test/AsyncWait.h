//
// AsyncWait.h
//
// Library: DataTest
// Package: DataTestCore
// Module:  AsyncWait
//
// Definition of the AsyncWait class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataTest_AsyncWait_INCLUDED
#define DataTest_AsyncWait_INCLUDED


#include "Poco/Data/Test/DataTest.h"
#include "Poco/Data/Statement.h"


namespace Poco::Data::Test {


class DataTest_API AsyncWait
	/// Waits for the asynchronous execution of a statement when it goes out
	/// of scope, so that a failed check does not return while an execution
	/// still uses the session and the variables of the test.
{
public:
	POCO_NODISCARD_CTOR explicit AsyncWait(Statement& stmt);
		/// Creates the AsyncWait for the given statement.

	~AsyncWait();
		/// Waits for the pending execution of the statement and reports
		/// an exception thrown by it with poco_unexpected().

	AsyncWait() = delete;
	AsyncWait(const AsyncWait&) = delete;
	AsyncWait(AsyncWait&&) = delete;
	AsyncWait& operator = (const AsyncWait&) = delete;
	AsyncWait& operator = (AsyncWait&&) = delete;

private:
	Statement& _stmt;
};


} // namespace Poco::Data::Test


#endif // DataTest_AsyncWait_INCLUDED
