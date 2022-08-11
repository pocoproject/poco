//
// SocketAcceptorTest.h
//
// Definition of the SocketAcceptorTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef POCO_SOCKETACCEPTORTEST_H
#define POCO_SOCKETACCEPTORTEST_H


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SocketAcceptorTest: public CppUnit::TestCase
{
public:
	SocketAcceptorTest(const std::string& name);
	~SocketAcceptorTest();

	void testSocketAcceptor();
	void testParallelSocketAcceptor();

	static CppUnit::Test* suite();

private:
};


#endif //POCO_SOCKETACCEPTORTEST_H
