//
// SerialPortTest.h
//
// Definition of the SerialPortTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SerialPortTest_INCLUDED
#define SerialPortTest_INCLUDED


#include "Poco/Serial/SerialPort.h"
#include "CppUnit/TestCase.h"


class SerialPortTest: public CppUnit::TestCase
{
public:
	SerialPortTest(const std::string& name);
	~SerialPortTest();

	void setUp();
	void tearDown();

	void testConstructDefaults();
	void testConstructBufferSize();
	void testConstructLogger();
	void testOpenBadParameterLength();
	void testOpenNonexistentDevice();
	void testOpenNotATerminal();
	void testClosedPortThrows();
	void testCloseWhenNotOpen();
	void testRS485Flags();

#if defined(POCO_OS_FAMILY_UNIX)
	void testOpenClose();
	void testOpenDefaultSpeed();
	void testOpenTwice();
	void testBadCharacterSize();
	void testBadParity();
	void testBadStopBits();
	void testBadSpeed();
	void testClosedAfterFailedOpen();
	void testWrite();
	void testReadBuffer();
	void testReadString();
	void testReadChar();
	void testAvailable();
	void testPollTimeout();
	void testPollDoesNotConsume();
	void testReadWithTimeout();
	void testDrain();
#endif

	static CppUnit::Test* suite();

private:
	void assertInitialState(const Poco::Serial::SerialPort& port);
		/// Asserts the state every constructor leaves the port in.

#if defined(POCO_OS_FAMILY_UNIX)
	class PseudoTerminal;

	bool skipUnavailable(const PseudoTerminal& pty);
		/// Warns and returns true when the platform provides no pseudo
		/// terminal, so that a container without /dev/ptmx skips the case
		/// instead of failing it.
#endif
};


#endif // SerialPortTest_INCLUDED
