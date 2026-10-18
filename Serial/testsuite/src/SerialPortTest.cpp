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
#include "Poco/Stopwatch.h"
#include "Poco/Timespan.h"
#include <type_traits>
#include <utility>
#include <vector>
#if defined(POCO_OS_FAMILY_UNIX)
#include <termios.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#endif


using Poco::Serial::SerialPort;


static_assert(!std::is_copy_constructible_v<Poco::Serial::SerialPort>,
	"SerialPort owns an open port and must not be copy constructible");
static_assert(!std::is_copy_assignable_v<Poco::Serial::SerialPort>,
	"SerialPort owns an open port and must not be copy assignable");
static_assert(std::is_move_constructible_v<Poco::Serial::SerialPort>,
	"SerialPort must be move constructible, so that it can be returned by value and kept in a container");
static_assert(std::is_move_assignable_v<Poco::Serial::SerialPort>,
	"SerialPort must be move assignable, so that it can be returned by value and kept in a container");


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

		// readMaster() waits by looking for data again, so a read of the
		// master side must report that there is none instead of blocking.
		if (::fcntl(_master, F_SETFL, O_NONBLOCK) == -1)
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

	[[nodiscard]] bool valid() const
	{
		return _master != -1;
	}

	[[nodiscard]] const std::string& slave() const
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
		ssize_t n = -1;
		auto readOnce = [&]()
		{
			n = ::read(_master, buffer, size);
			return n >= 0;
		};
		const long timeoutMillis = static_cast<long>(timeout.totalMilliseconds());
		if (!CppUnit::waitForCondition(readOnce, timeoutMillis, POLL_INTERVAL_MS)) return 0;
		return static_cast<std::size_t>(n);
	}

	void hangUp()
		/// Closes the master side, which is what the slave side of the line
		/// sees when a serial adapter is unplugged.
	{
		closeMaster();
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

	static constexpr long POLL_INTERVAL_MS = 10;
		/// How often readMaster() looks for data from the slave side.

	int _master;
	int _slaveFd = -1;
	std::string _slave;
};


namespace
{
	SerialPort openPort(const std::string& device)
		/// Returns an open port by value, the pattern that needs the move
		/// constructor even where the compiler elides the move itself.
	{
		SerialPort port;
		port.open(device, 9600, "8N1");
		return port;
	}
}


#endif // POCO_OS_FAMILY_UNIX


SerialPortTest::SerialPortTest(const std::string& name): CppUnit::TestCase(name)
{
}


SerialPortTest::~SerialPortTest() = default;


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


void SerialPortTest::testConstructZeroBufferSize()
{
	try
	{
		SerialPort port(0);
		fail("the constructor must reject a buffer size of zero");
	}
	catch (const Poco::InvalidArgumentException&) { }

	try
	{
		SerialPort port(Poco::Logger::get("SerialPortTest"), 0);
		fail("the constructor taking a logger must reject a buffer size of zero");
	}
	catch (const Poco::InvalidArgumentException&) { }

	try
	{
		// The buffer size is checked before the device is touched, so the
		// device name below is never opened.
		SerialPort port(NON_SERIAL_DEVICE, -1, "8N1", SerialPort::FLOW_NONE, 0);
		fail("the constructor taking a device must reject a buffer size of zero");
	}
	catch (const Poco::InvalidArgumentException&) { }

	SerialPort port(1);
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

		assertClosed(port);
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

	assertClosed(port);
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

	assertClosed(port);
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

	// A failed open must leave no descriptor or handle behind. On Windows this
	// is the one case that reaches openImpl()'s cleanup with a live handle.
	assertClosed(port);
}


void SerialPortTest::testClosedPortThrows()
{
	SerialPort port;
	char buffer[16];
	assertClosed(port);

	try
	{
		(void) port.poll(Poco::Timespan(0, 1000));
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
		(void) port.getRTS();
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
		(void) port.poll(Poco::Timespan(0, 1000));
		fail("poll() must throw after close()");
	}
	catch (const Poco::IllegalStateException&) { }
}


void SerialPortTest::testIsOpen()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	assertTrue(!port.isOpen());

	port.open(pty.slave(), 9600, "8N1");
	assertTrue(port.isOpen());

	port.close();
	assertTrue(!port.isOpen());

	try
	{
		port.open(NONEXISTENT_DEVICE);
		fail("open() must throw for a device that does not exist");
	}
	catch (const Poco::IOException&) { }
	assertTrue(!port.isOpen());
}


void SerialPortTest::testMove()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort source;
	source.open(pty.slave(), 9600, "8N1");

	SerialPort constructed(std::move(source));
	assertTrue(constructed.isOpen());
	assertEqual(pty.slave(), constructed.device());
	assertEqual(9600, constructed.baudRate());
	assertEqual("8N1", constructed.parameters());
	assertClosed(source);
	assertInitialState(source);

	// The descriptor must have moved with the port instead of having been
	// closed together with the source, so the moved-to port can still read.
	assertEqual(1, pty.writeMaster("A"));
	assertTrue(constructed.poll(Poco::Timespan(2, 0)));
	assertEqual('A', constructed.read());

	SerialPort assigned;
	assigned = std::move(constructed);
	assertTrue(assigned.isOpen());
	assertEqual(pty.slave(), assigned.device());
	assertEqual(9600, assigned.baudRate());
	assertEqual("8N1", assigned.parameters());
	assertClosed(constructed);
	assertInitialState(constructed);

	assertEqual(1, pty.writeMaster("B"));
	assertTrue(assigned.poll(Poco::Timespan(2, 0)));
	assertEqual('B', assigned.read());

	try
	{
		source.open(pty.slave(), 9600, "8N1");
		fail("open() must throw on a port that has been moved from");
	}
	catch (const Poco::IllegalStateException&) { }

	// Returning a port by value and keeping ports in a vector, which moves
	// its elements when it grows, are the two patterns that need the move.
	SerialPort returned = openPort(pty.slave());
	assertTrue(returned.isOpen());
	assertEqual(pty.slave(), returned.device());

	std::vector<SerialPort> ports;
	ports.emplace_back(pty.slave(), 9600, "8N1");
	ports.emplace_back(64);
	assertTrue(ports[0].isOpen());
	assertEqual(pty.slave(), ports[0].device());
	assertTrue(!ports[1].isOpen());
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
	// device happened to carry.
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
		// A pseudo terminal need not support five bit characters. open()
		// reads the settings back and reports the device's refusal, which
		// is the other outcome this case accepts.
		assertClosed(port);
		return;
	}

	// The device accepted the request, so it must really carry five bit
	// characters. Termios settings belong to the device, not to the
	// descriptor, so this descriptor sees what the library configured.
	const int fd = pty.openSlave();
	assertTrue(fd != -1);
	struct termios term;
	assertTrue(::tcgetattr(fd, &term) == 0);
	assertTrue((term.c_cflag & CSIZE) == CS5);
}


void SerialPortTest::testOpenParityEven()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "8E1");
	}
	catch (const Poco::IOException&)
	{
		// A pseudo terminal need not support parity, which open() reports
		// the same way as a refused character size.
		assertClosed(port);
		return;
	}

	const int fd = pty.openSlave();
	assertTrue(fd != -1);
	struct termios term;
	assertTrue(::tcgetattr(fd, &term) == 0);
	assertTrue((term.c_cflag & PARENB) != 0);
	assertTrue((term.c_cflag & PARODD) == 0);
}


void SerialPortTest::testOpenClearsInheritedParityFlags()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	// Leave the device ignoring parity errors, as a previous user may have
	// done; open() must not inherit that when it is asked for even parity.
	const int previous = pty.openSlave();
	assertTrue(previous != -1);
	struct termios term;
	assertTrue(::tcgetattr(previous, &term) == 0);
	term.c_iflag |= IGNPAR;
	term.c_iflag &= ~INPCK;
	assertTrue(::tcsetattr(previous, TCSANOW, &term) == 0);

	SerialPort port;
	try
	{
		port.open(pty.slave(), 9600, "8E1");
	}
	catch (const Poco::IOException&)
	{
		// The pseudo terminal refused parity, which open() reports; nothing
		// was configured, so there is nothing to check.
		assertClosed(port);
		return;
	}

	const int fd = pty.openSlave();
	assertTrue(fd != -1);
	assertTrue(::tcgetattr(fd, &term) == 0);
	assertTrue((term.c_iflag & IGNPAR) == 0);
	assertTrue((term.c_iflag & INPCK) != 0);
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

	assertClosed(port);
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

	assertClosed(port);
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

	assertClosed(port);
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

	assertClosed(port);
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
	assertEqual('A', port.read());
}


void SerialPortTest::testReadAfterHangup()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	// Closing the master side hangs up the line, which is what unplugging a
	// serial adapter does to it. Every read must then report the loss of the
	// peer instead of returning nothing.
	pty.hangUp();

	char buffer[16];
	try
	{
		port.read(buffer, sizeof(buffer));
		fail("read(buffer) must throw after the peer has hung up");
	}
	catch (const Poco::IOException&) { }

	// The timeout overload is timed as well as checked, because what it did
	// wrong was to spin in a loop rather than to return a wrong value.
	Poco::Stopwatch sw;
	sw.start();
	try
	{
		port.read(buffer, sizeof(buffer), Poco::Timespan(0, 100000));
		fail("read(buffer, timeout) must throw after the peer has hung up");
	}
	catch (const Poco::IOException&) { }
	assertTrue(sw.elapsed() < Poco::Timespan(0, 500000).totalMicroseconds());

	try
	{
		std::string data;
		port.read(data);
		fail("read(string) must throw after the peer has hung up");
	}
	catch (const Poco::IOException&) { }

	try
	{
		port.read();
		fail("read() must throw after the peer has hung up");
	}
	catch (const Poco::IOException&) { }
}


void SerialPortTest::testReadZeroLength()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	// Nothing has arrived, so a read that asks for nothing must return at
	// once rather than wait for the first byte.
	char buffer[4];
	Poco::Stopwatch sw;
	sw.start();
	assertEqual(0, port.read(buffer, 0));
	assertTrue(sw.elapsed() < Poco::Timespan(0, 500000).totalMicroseconds());

	// Nor may it consume a byte that is waiting.
	assertEqual(1, pty.writeMaster("Z"));
	assertTrue(port.poll(Poco::Timespan(2, 0)));
	assertEqual(0, port.read(buffer, 0));
	assertEqual('Z', port.read());
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

	Poco::Stopwatch sw;
	sw.start();
	assertTrue(!port.poll(Poco::Timespan(0, 200000)));
	assertTrue(sw.elapsed() >= Poco::Timespan(0, 150000).totalMicroseconds());
	assertTrue(sw.elapsed() < Poco::Timespan(2, 0).totalMicroseconds());
}


void SerialPortTest::testPollNegativeTimeout()
{
	PseudoTerminal pty;
	if (skipUnavailable(pty)) return;

	SerialPort port;
	port.open(pty.slave(), 9600, "8N1");

	// A negative timeout is no wait at all, not an endless one.
	Poco::Stopwatch sw;
	sw.start();
	assertTrue(!port.poll(Poco::Timespan(-1, 0)));
	assertTrue(sw.elapsed() < Poco::Timespan(0, 500000).totalMicroseconds());
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


void SerialPortTest::assertClosed(const SerialPort& port)
{
	assertTrue(!port.isOpen());
}


CppUnit::Test* SerialPortTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerialPortTest");

	CppUnit_addTest(pSuite, SerialPortTest, testConstructDefaults);
	CppUnit_addTest(pSuite, SerialPortTest, testConstructBufferSize);
	CppUnit_addTest(pSuite, SerialPortTest, testConstructLogger);
	CppUnit_addTest(pSuite, SerialPortTest, testConstructZeroBufferSize);
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
	CppUnit_addTest(pSuite, SerialPortTest, testIsOpen);
	CppUnit_addTest(pSuite, SerialPortTest, testMove);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenDefaultSpeed);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenSetsBlockingRead);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenTwice);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenCharacterSize5);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenParityEven);
	CppUnit_addTest(pSuite, SerialPortTest, testOpenClearsInheritedParityFlags);
	CppUnit_addTest(pSuite, SerialPortTest, testBadCharacterSize);
	CppUnit_addTest(pSuite, SerialPortTest, testBadParity);
	CppUnit_addTest(pSuite, SerialPortTest, testBadStopBits);
	CppUnit_addTest(pSuite, SerialPortTest, testBadSpeed);
	CppUnit_addTest(pSuite, SerialPortTest, testWrite);
	CppUnit_addTest(pSuite, SerialPortTest, testWriteEmptyString);
	CppUnit_addTest(pSuite, SerialPortTest, testReadBuffer);
	CppUnit_addTest(pSuite, SerialPortTest, testReadString);
	CppUnit_addTest(pSuite, SerialPortTest, testReadChar);
	CppUnit_addTest(pSuite, SerialPortTest, testReadAfterHangup);
	CppUnit_addTest(pSuite, SerialPortTest, testReadZeroLength);
	CppUnit_addTest(pSuite, SerialPortTest, testAvailable);
	CppUnit_addTest(pSuite, SerialPortTest, testPollTimeout);
	CppUnit_addTest(pSuite, SerialPortTest, testPollNegativeTimeout);
	CppUnit_addTest(pSuite, SerialPortTest, testPollDoesNotConsume);
	CppUnit_addTest(pSuite, SerialPortTest, testReadWithTimeout);
	CppUnit_addTest(pSuite, SerialPortTest, testDrain);
#endif

	return pSuite;
}
