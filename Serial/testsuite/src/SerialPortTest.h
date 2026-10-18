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
	~SerialPortTest() override;

	void setUp() override;
	void tearDown() override;

	void testConstructDefaults();
	void testConstructBufferSize();
	void testConstructLogger();
	void testConstructZeroBufferSize();
	void testOpenBadParameterLength();
	void testOpenEmptyDevice();
	void testOpenNonexistentDevice();
	void testOpenNotATerminal();
	void testClosedPortThrows();
	void testCloseWhenNotOpen();
	void testRS485Flags();
	void testRS485ParamsDefaults();

#if defined(POCO_OS_FAMILY_UNIX)
	void testOpenClose();
	void testIsOpen();
	void testMove();
	void testOpenDefaultSpeed();
	void testOpenSetsBlockingRead();
	void testOpenTwice();
	void testOpenCharacterSize5();
	void testOpenParityEven();
	void testOpenClearsInheritedParityFlags();
	void testBadCharacterSize();
	void testBadParity();
	void testBadStopBits();
	void testBadSpeed();
	void testWrite();
	void testWriteEmptyString();
	void testReadBuffer();
	void testReadString();
	void testReadChar();
	void testReadAfterHangup();
	void testReadZeroLength();
	void testAvailable();
	void testPollTimeout();
	void testPollNegativeTimeout();
	void testPollDoesNotConsume();
	void testReadWithTimeout();
	void testDrain();
#endif

	static CppUnit::Test* suite();

private:
	void assertInitialState(const Poco::Serial::SerialPort& port);
		/// Asserts the state every constructor leaves the port in.

	void assertClosed(const Poco::Serial::SerialPort& port);
		/// Asserts that the port is closed, without writing to it: a port
		/// that is wrongly left open is the defect such a check looks for,
		/// and a write would then put a byte on the wire.

#if defined(POCO_OS_FAMILY_UNIX)
	class PseudoTerminal;

	bool skipUnavailable(const PseudoTerminal& pty);
		/// Warns and returns true when the platform provides no pseudo
		/// terminal, so that a container without /dev/ptmx skips the case
		/// instead of failing it.
#endif
};


#endif // SerialPortTest_INCLUDED
