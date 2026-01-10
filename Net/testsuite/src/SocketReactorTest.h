//
// SocketReactorTest.h
//
// Definition of the SocketReactorTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SocketReactorTest_INCLUDED
#define SocketReactorTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"
#include "Poco/AutoPtr.h"

namespace Poco {
namespace Net {

class ReadableNotification;
class WritableNotification;

} }


class SocketReactorTest: public CppUnit::TestCase
{
public:
	SocketReactorTest(const std::string& name);
	~SocketReactorTest();

	void testSocketReactor();
	void testSetSocketReactor();
	void testParallelSocketReactor();
	void testSocketConnectorFail();
	void testSocketConnectorTimeout();
	void testDataCollection();
	void testSocketConnectorDeadlock();
	void testSocketReactorWakeup();
	void testSocketReactorRemove();
	void testConcurrentHandlerRemoval();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf);
	void onWritable(const Poco::AutoPtr<Poco::Net::WritableNotification>& pNf);
};


#endif // SocketReactorTest_INCLUDED
