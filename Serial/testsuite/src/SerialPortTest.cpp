//
// SerialPortTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SerialPortTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#if defined(POCO_OS_FAMILY_UNIX)
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <cerrno>
#endif


using Poco::Serial::SerialPort;


namespace
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	const std::string NON_SERIAL_DEVICE("NUL");
	const std::string NONEXISTENT_DEVICE("COM255");
#else
	const std::string NON_SERIAL_DEVICE("/dev/null");
	const std::string NONEXISTENT_DEVICE("/dev/poco-serial-no-such-device");
#endif
}


#if defined(POCO_OS_FAMILY_UNIX)


class SerialPortTest::PseudoTerminal
	/// A pseudo terminal pair. The slave side is a device with a termios line
	/// discipline and no hardware behind it, so it exercises the code paths a
	/// serial port would, on a machine that has no serial port.
{
public:
	PseudoTerminal():
		_master(::posix_openpt(O_RDWR | O_NOCTTY))
	{
		if (_master == -1) return;

		if (::grantpt(_master) == -1 || ::unlockpt(_master) == -1)
		{
			closeMaster();
			return;
		}

		const char* slave = ::ptsname(_master);
		if (slave == nullptr)
		{
			closeMaster();
			return;
		}
		_slave = slave;
	}

	~PseudoTerminal()
	{
		closeSlave();
		closeMaster();
	}

	PseudoTerminal(const PseudoTerminal&) = delete;
	PseudoTerminal& operator = (const PseudoTerminal&) = delete;

	bool valid() const
	{
		return _master != -1;
	}

	const std::string& slave() const
	{
		return _slave;
	}

	int openSlave()
		/// Opens the slave side with a plain descriptor and keeps it open for
		/// the lifetime of this object, so that a failing assertion cannot
		/// leak it. A pseudo terminal returns its termios settings to the
		/// default when the last slave descriptor is closed, so a test that
		/// prepares those settings must hold the descriptor open.
	{
		closeSlave();
		_slaveFd = ::open(_slave.c_str(), O_RDWR | O_NOCTTY);
		return _slaveFd;
	}

	std::size_t writeMaster(const std::string& data)
		/// Writes to the master side, which puts the data into the slave's
		/// input queue.
	{
		ssize_t n = ::write(_master, data.data(), data.size());
		return n < 0 ? 0 : static_cast<std::size_t>(n);
	}

	std::size_t readMaster(char* buffer, std::size_t size, const Poco::Timespan& timeout)
		/// Reads what the slave side has written, waiting at most timeout.
		/// Returns 0 on a timeout rather than blocking, so that a test which
		/// expects data fails instead of hanging.
	{
		Poco::Timespan remainingTime(timeout);
		int rc;
		do
		{
			fd_set readable;
			FD_ZERO(&readable);
			FD_SET(_master, &readable);
			struct timeval tv;
			tv.tv_sec = static_cast<long>(remainingTime.totalSeconds());
			tv.tv_usec = static_cast<long>(remainingTime.useconds());
			Poco::Timestamp start;
			rc = ::select(_master + 1, &readable, nullptr, nullptr, &tv);
			if (rc < 0 && errno == EINTR)
			{
				Poco::Timestamp end;
				Poco::Timespan waited = end - start;
				if (waited < remainingTime)
					remainingTime -= waited;
				else
					remainingTime = 0;
			}
		}
		while (rc < 0 && errno == EINTR);
		if (rc <= 0) return 0;

		ssize_t n = ::read(_master, buffer, size);
		return n < 0 ? 0 : static_cast<std::size_t>(n);
	}

private:
	void closeMaster()
	{
		if (_master != -1)
		{
			::close(_master);
			_master = -1;
		}
	}

	void closeSlave()
	{
		if (_slaveFd != -1)
		{
			::close(_slaveFd);
			_slaveFd = -1;
		}
	}

	int _master;
	int _slaveFd = -1;
	std::string _slave;
};


#endif // POCO_OS_FAMILY_UNIX


SerialPortTest::SerialPortTest(const std::string& name): CppUnit::TestCase(name)
{
}


SerialPortTest::~SerialPortTest()
{
}


void SerialPortTest::setUp()
{
}


void SerialPortTest::tearDown()
{
}


void SerialPortTest::testConstructDefaults()
{
	SerialPort port;
	assertInitialState(port);
}


void SerialPortTest::testConstructBufferSize()
{
	SerialPort port(64);
	assertInitialState(port);
}


void SerialPortTest::testConstructLogger()
{
	SerialPort port(Poco::Logger::get("SerialPortTest"), 64);
	assertInitialState(port);
}


void SerialPortTest::testOpenBadParameterLength()
{
	const std::string badParameters[] = { "", "8", "8N", "8N11" };
	for (const std::string& parameters: badParameters)
	{
		SerialPort port;
		try
		{
			// The length is checked before the device is touched, so the
			// device name below is never opened.
			port.open(NON_SERIAL_DEVICE, -1, parameters);
			fail("open() must reject a parameters string that is not three characters long");
		}
		catch (const Poco::InvalidArgumentException&) { }

		try
		{
			port.write(std::string("x"));
			fail("port must still be closed after a rejected parameters string");
		}
		catch (const Poco::IllegalStateException&) { }
	}
}


void SerialPortTest::testOpenEmptyDevice()
{
	SerialPort port;
	try
	{
		port.open("");
		fail("open() must reject an empty device name");
	}
	catch (const Poco::InvalidArgumentException&) { }

	try
	{
		port.write(std::string("x"));
		fail("port must still be closed after a rejected device name");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testOpenNonexistentDevice()
{
	SerialPort port;
	try
	{
		port.open(NONEXISTENT_DEVICE);
		fail("open() must throw for a device that does not exist");
	}
	catch (const Poco::IOException&) { }
}


void SerialPortTest::testOpenNotATerminal()
{
	SerialPort port;
	try
	{
		port.open(NON_SERIAL_DEVICE);
		fail("open() must throw for a device that is not a serial line");
	}
	catch (const Poco::IOException&) { }
}


void SerialPortTest::testClosedPortThrows()
{
	SerialPort port;
	char buffer[16];

	try
	{
		port.poll(Poco::Timespan(0, 1000));
		fail("poll() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.write(std::string("x"));
		fail("write(string) must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.write("x", 1);
		fail("write(buffer) must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.read(buffer, sizeof(buffer));
		fail("read(buffer) must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.read(buffer, sizeof(buffer), Poco::Timespan(0, 1000));
		fail("read(buffer, timeout) must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		std::string data;
		port.read(data);
		fail("read(string) must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.read();
		fail("read() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.drain();
		fail("drain() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.setRTS(true);
		fail("setRTS() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		port.getRTS();
		fail("getRTS() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }

	try
	{
		SerialPort::RS485Params params;
		port.configureRS485(params);
		fail("configureRS485() must throw on a port that is not open");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testCloseWhenNotOpen()
{
	SerialPort port;
	port.close();
	port.close();
	assertInitialState(port);
}


void SerialPortTest::testRS485Flags()
{
	assertEqual(1, static_cast<int>(SerialPort::RS485Params::RS485_ENABLED));
	assertEqual(2, static_cast<int>(SerialPort::RS485Params::RS485_RTS_ON_SEND));
	assertEqual(4, static_cast<int>(SerialPort::RS485Params::RS485_RTS_AFTER_SEND));
	assertEqual(32, static_cast<int>(SerialPort::RS485Params::RS485_USE_GPIO));
}


void SerialPortTest::testRS485ParamsDefaults()
{
	SerialPort::RS485Params params;
	assertEqual(0, params.flags);
	assertEqual(0, params.delayRTSBeforeSend);
	assertEqual(0, params.delayRTSAfterSend);
	assertEqual(0, params.gpioPin);
}


#if defined(POCO_OS_FAMILY_UNIX)


void SerialPortTest::testOpenClose()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(pty.slave(), port.device());
	assertEqual(9600, port.baudRate());
	assertEqual("8N1", port.parameters());
	assertTrue(port.flowControl() == SerialPort::FLOW_NONE);
	assertEqual(0, port.available());
	port.close();

	try
	{
		port.poll(Poco::Timespan(0, 1000));
		fail("poll() must throw after close()");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testOpenDefaultSpeed()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave());
	assertEqual(-1, port.baudRate());
}


void SerialPortTest::testOpenSetsBlockingRead()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	// Give the device the control characters of a non-blocking read, so that
	// what is asserted below is what open() established and not what the
	// device happened to carry. The descriptor stays open until the end of
	// the case because a pseudo terminal returns its settings to the default
	// when the last slave descriptor is closed.
	const int fd = pty.openSlave();
	assertTrue(fd != -1);
	struct termios term;
	assertTrue(::tcgetattr(fd, &term) == 0);
	term.c_cc[VMIN]  = 0;
	term.c_cc[VTIME] = 0;
	assertTrue(::tcsetattr(fd, TCSANOW, &term) == 0);

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	// Termios settings belong to the device, not to the descriptor, so this
	// descriptor sees what the library configured.
	assertTrue(::tcgetattr(fd, &term) == 0);
	assertEqual(1, static_cast<int>(term.c_cc[VMIN]));
	assertEqual(0, static_cast<int>(term.c_cc[VTIME]));
}


void SerialPortTest::testOpenTwice()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	try
	{
		port.open(pty.slave(), 9600, "8N1");
		fail("open() must throw on a port that is already open");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testOpenCharacterSize5()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "5N1");
	}
	catch (const Poco::InvalidArgumentException&)
	{
		fail("open() must accept a character size of five bits");
	}
	catch (const Poco::IOException&)
	{
		// A pseudo terminal need not support five bit characters; it
		// rejects them in tcsetattr(), which is not an argument error.
	}
}


void SerialPortTest::testBadCharacterSize()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "9N1");
		fail("open() must reject an invalid character size");
	}
	catch (const Poco::InvalidArgumentException&) { }
}


void SerialPortTest::testBadParity()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "8X1");
		fail("open() must reject an invalid parity setting");
	}
	catch (const Poco::InvalidArgumentException&) { }
}


void SerialPortTest::testBadStopBits()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "8N3");
		fail("open() must reject an invalid number of stop bits");
	}
	catch (const Poco::InvalidArgumentException&) { }
}


void SerialPortTest::testBadSpeed()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 12345, "8N1");
		fail("open() must reject an unsupported line speed");
	}
	catch (const Poco::InvalidArgumentException&) { }
}


void SerialPortTest::testClosedAfterFailedOpen()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "9N1");
		fail("open() must reject an invalid character size");
	}
	catch (const Poco::InvalidArgumentException&) { }

	try
	{
		port.write(std::string("x"));
		fail("port must be closed after a failed open()");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testWrite()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	char buffer[16];
	assertEqual(5, port.write(std::string("hello")));
	assertEqual(5, pty.readMaster(buffer, sizeof(buffer), Poco::Timespan(2, 0)));
	assertEqual("hello", std::string(buffer, 5));

	assertEqual(2, port.write("ab", 2));
	assertEqual(2, pty.readMaster(buffer, sizeof(buffer), Poco::Timespan(2, 0)));
	assertEqual("ab", std::string(buffer, 2));
}


void SerialPortTest::testWriteEmptyString()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(0, port.write(std::string()));
}


void SerialPortTest::testReadBuffer()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(5, pty.writeMaster("world"));

	// The read() overloads without a timeout block until data arrives, here
	// and in the cases below. poll() first, so that a pseudo terminal which
	// does not deliver the write fails the case instead of hanging it.
	assertTrue(port.poll(Poco::Timespan(2, 0)));
	char buffer[16];
	assertEqual(5, port.read(buffer, sizeof(buffer)));
	assertEqual("world", std::string(buffer, 5));
}


void SerialPortTest::testReadString()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(3, pty.writeMaster("abc"));

	assertTrue(port.poll(Poco::Timespan(2, 0)));
	std::string data;
	port.read(data);
	assertEqual("abc", data);
}


void SerialPortTest::testReadChar()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(1, pty.writeMaster("A"));

	assertTrue(port.poll(Poco::Timespan(2, 0)));
	assertEqual(65, port.read());
}


void SerialPortTest::testAvailable()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(4, pty.writeMaster("abcd"));

	assertTrue(port.poll(Poco::Timespan(2, 0)));
	assertEqual('a', port.read());
	assertEqual(3, port.available());
}


void SerialPortTest::testPollTimeout()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	Poco::Timestamp start;
	assertTrue(!port.poll(Poco::Timespan(0, 200000)));
	assertTrue(start.elapsed() >= Poco::Timespan(0, 150000).totalMicroseconds());
	assertTrue(start.elapsed() < Poco::Timespan(2, 0).totalMicroseconds());
}


void SerialPortTest::testPollDoesNotConsume()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(2, pty.writeMaster("xy"));

	assertTrue(port.poll(Poco::Timespan(2, 0)));
	assertEqual(0, port.available());

	char buffer[4];
	assertEqual(2, port.read(buffer, 2));
	assertEqual("xy", std::string(buffer, 2));
}


void SerialPortTest::testReadWithTimeout()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(3, pty.writeMaster("abc"));

	char buffer[16];
	assertEqual(3, port.read(buffer, 10, Poco::Timespan(0, 300000)));
	assertEqual("abc", std::string(buffer, 3));
}


void SerialPortTest::testDrain()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");
	assertEqual(2, port.write("hi", 2));

	// A pseudo terminal drains its output queue only when the master side
	// reads, so collect the bytes before draining: tcdrain() would otherwise
	// wait forever for a reader this test does not provide.
	char buffer[4];
	assertEqual(2, pty.readMaster(buffer, sizeof(buffer), Poco::Timespan(2, 0)));
	assertEqual("hi", std::string(buffer, 2));

	port.drain();
}


bool SerialPortTest::skipUnavailable(const PseudoTerminal& pty)
{
	if (pty.valid()) return false;

	warnmsg("no pseudo terminal available, skipping");
	return true;
}


#endif // POCO_OS_FAMILY_UNIX


void SerialPortTest::assertInitialState(const SerialPort& port)
{
	assertTrue(port.device().empty());
	assertEqual(-1, port.baudRate());
	assertTrue(port.parameters().empty());
	assertTrue(port.flowControl() == SerialPort::FLOW_NONE);
	assertEqual(0, port.available());
}


CppUnit::Test* SerialPortTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerialPortTest");

	CppUnit_addTest(pSuite, SerialPortTest, testConstructDefaults);
	CppUnit_addTest(pSuite, SerialPortTest, testConstructBufferSize);
	CppUnit_addTest(pSuite, SerialPortTest, testConstructLogger);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenBadParameterLength);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenEmptyDevice);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenNonexistentDevice);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenNotATerminal);
	CppUnit_addTest(pSuite, SerialPortTest, testClosedPortThrows);
	CppUnit_addTest(pSuite, SerialPortTest, testCloseWhenNotOpen);
	CppUnit_addTest(pSuite, SerialPortTest, testRS485Flags);
	CppUnit_addTest(pSuite, SerialPortTest, testRS485ParamsDefaults);

#if defined(POCO_OS_FAMILY_UNIX)
	CppUnit_addTest(pSuite, SerialPortTest, testOpenClose);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenDefaultSpeed);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenSetsBlockingRead);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenTwice);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenCharacterSize5);
	CppUnit_addTest(pSuite, SerialPortTest, testBadCharacterSize);
	CppUnit_addTest(pSuite, SerialPortTest, testBadParity);
	CppUnit_addTest(pSuite, SerialPortTest, testBadStopBits);
	CppUnit_addTest(pSuite, SerialPortTest, testBadSpeed);
	CppUnit_addTest(pSuite, SerialPortTest, testClosedAfterFailedOpen);
	CppUnit_addTest(pSuite, SerialPortTest, testWrite);
	CppUnit_addTest(pSuite, SerialPortTest, testWriteEmptyString);
	CppUnit_addTest(pSuite, SerialPortTest, testReadBuffer);
	CppUnit_addTest(pSuite, SerialPortTest, testReadString);
	CppUnit_addTest(pSuite, SerialPortTest, testReadChar);
	CppUnit_addTest(pSuite, SerialPortTest, testAvailable);
	CppUnit_addTest(pSuite, SerialPortTest, testPollTimeout);
	CppUnit_addTest(pSuite, SerialPortTest, testPollDoesNotConsume);
	CppUnit_addTest(pSuite, SerialPortTest, testReadWithTimeout);
	CppUnit_addTest(pSuite, SerialPortTest, testDrain);
#endif

	return pSuite;
}
