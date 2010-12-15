//
// LogStreamTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/LogStreamTest.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules it contains.  It does not include source code for modules or
//    files that typically accompany the major components of the operating
//    system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//


#include "LogStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Logger.h"
#include "Poco/LogStream.h"
#include "Poco/AutoPtr.h"
#include "TestChannel.h"


using Poco::Logger;
using Poco::LogStream;
using Poco::Channel;
using Poco::Message;
using Poco::AutoPtr;


LogStreamTest::LogStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


LogStreamTest::~LogStreamTest()
{
}


void LogStreamTest::testLogStream()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel.get());

	LogStream ls(root);

	ls << "information" << ' ' << 1 << std::endl;
	assert (pChannel->list().begin()->getPriority() == Message::PRIO_INFORMATION);
	assert (pChannel->list().begin()->getText() == "information 1");
	pChannel->list().clear();

	ls.error() << "error" << std::endl;
	assert (pChannel->list().begin()->getPriority() == Message::PRIO_ERROR);
	assert (pChannel->list().begin()->getText() == "error");
	pChannel->list().clear();
}


void LogStreamTest::setUp()
{
}


void LogStreamTest::tearDown()
{
}


CppUnit::Test* LogStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LogStreamTest");

	CppUnit_addTest(pSuite, LogStreamTest, testLogStream);

	return pSuite;
}
