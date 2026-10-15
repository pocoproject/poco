//
// SerialPort.h
//
// Library: Serial
// Package: Serial
// Module:  SerialPort
//
// Definition of the SerialPort class.
//
// Copyright (c) 2009-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Poco_Serial_SerialPort_INCLUDED
#define Poco_Serial_SerialPort_INCLUDED


#include "Poco/Serial/Serial.h"
#include "Poco/Logger.h"
#include "Poco/Timespan.h"
#include "Poco/Buffer.h"
#include <cstddef>


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Serial/SerialPort_WIN32.h"
#else
#include "Poco/Serial/SerialPort_POSIX.h"
#endif


namespace Poco::Serial {


class Serial_API SerialPort: private SerialPortImpl
	/// This class provides an interface to a serial port.
	///
	/// Two properties of a serial port differ between the platforms and
	/// cannot be hidden by this class.
	///
	/// On Windows the port is opened for exclusive use, so a second attempt
	/// to open the same device fails. POSIX does not lock the device, so two
	/// ports can be open on it at the same time, and the bytes arriving are
	/// then split between their readers.
	///
	/// On Windows the operations on one port are serialised: a thread waiting
	/// in read() also delays write(), poll(), drain() and setRTS() called
	/// from another thread. On POSIX a blocked read does not delay a write.
{
public:
	enum FlowControl
	{
		FLOW_NONE,   /// No flow control
		FLOW_RTSCTS  /// Hardware (RTS/CTS) flow control
	};

	static constexpr std::size_t DEFAULT_BUFFER_SIZE = 4096;
		/// Default size for the internal buffer.

	using RS485Params = RS485ParamsImpl;
		/// Parameters of RS-485 mode. The members are flags, a combination of
		/// RS485Params::RS485Flags; delayRTSBeforeSend and delayRTSAfterSend,
		/// the RTS delays in milliseconds; and gpioPin, the pin to use where
		/// RS485_USE_GPIO is set. All of them default to zero.

	SerialPort();
		/// Creates the SerialPort.

	explicit SerialPort(std::size_t bufferSize);
		/// Creates the SerialPort, using the given buffer size.
		///
		/// Throws a Poco::InvalidArgumentException if bufferSize is zero.

	SerialPort(Poco::Logger& logger, std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
		/// Creates the SerialPort, using the given logger and buffer size.
		///
		/// Throws a Poco::InvalidArgumentException if bufferSize is zero.

	SerialPort(const std::string& device, int baudRate = -1, const std::string& parameters = "8N1", FlowControl flowControl = FLOW_NONE, std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
		/// Creates and opens the SerialPort, using the given parameters.
		///
		/// See open() for a description of the parameters.
		///
		/// Throws a Poco::InvalidArgumentException if bufferSize is zero.

	~SerialPort();
		/// Closes, if necessary, and destroys the SerialPort.

	SerialPort(const SerialPort&) = delete;
	SerialPort& operator = (const SerialPort&) = delete;

	SerialPort(SerialPort&& other) noexcept;
		/// Creates the SerialPort by taking over the device, the buffered
		/// input and the logger of other.
		///
		/// The moved-from port is left closed, with an empty device name and
		/// without a buffer. Destruction and assignment are the only
		/// operations it supports.

	SerialPort& operator = (SerialPort&& other) noexcept;
		/// Closes this port, if necessary, and takes over the device and the
		/// buffered input of other. The logger of this port is kept, because a
		/// reference cannot be reseated.
		///
		/// See the move constructor for the state of the moved-from port.

	void open(const std::string& device, int baudRate = -1, const std::string& parameters = "8N1", FlowControl flowControl = FLOW_NONE);
		/// Opens the port.
		///
		/// Device sets the serial port device file name and is usually
		/// something like "/dev/ttyS0", "/dev/ttyUSB0",
		/// "/dev/tty.usbserial-XXX" or "COM1".
		///
		/// Baud rate must be one of the following:
		/// 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400,
		/// 4800, 9600, 19200, 38400, 57600, 115200 or 230400.
		/// POSIX platforms accept 460800, 500000, 576000, 921600, 1000000,
		/// 1152000, 1500000, 2000000, 2500000, 3000000, 3500000 and 4000000
		/// in addition, where the platform defines them.
		///
		/// Specify -1 for baudRate to use the currently configured speed.
		/// Note that not all platforms support all baud rates: Windows
		/// supports 14400 in addition, and none of 50, 75, 134, 150, 200,
		/// 1800 and 230400.
		///
		/// To specify character size, parity and stop bits, use the
		/// parameters argument. The parameters argument must be a three
		/// character string. The first character specifies the serial line
		/// character width in bits (5, 6, 7 or 8). The second char specifies
		/// the parity (N for none, E for even, O for odd; on Windows also
		/// M for mark and S for space) and the third character specifies
		/// the number of stop bits (1 or 2; on Windows also 5 for 1.5 stop
		/// bits).
		/// Example: "8N1" for 8 bit characters, no parity, 1 stop bit.
		/// Windows rejects 5 data bits with 2 stop bits, and 1.5 stop bits
		/// with 6, 7 or 8 data bits.
		///
		/// The flowControl argument can be used to enable or disable
		/// hardware (CTS/RTS) flow control.
		///
		/// Throws a Poco::IOException if the device cannot be opened or
		/// configured, which includes a device that accepts the request but
		/// does not apply the requested character size, parity or number of
		/// stop bits.

	void close();
		/// Closes the serial port.

	[[nodiscard]] bool isOpen() const;
		/// Returns true if the port is open, otherwise false.

	[[nodiscard]] bool poll(const Poco::Timespan& timeout);
		/// Waits for data to arrive at the port.
		///
		/// Returns true immediately if data is already in
		/// the internal buffer, or if data arrives during the
		/// specified time interval, otherwise false.
		///
		/// A negative timeout is treated as zero, so poll() returns at once.
		///
		/// A port whose peer has hung up is readable, so poll() returns true
		/// there as well and the following read throws.
		///
		/// On POSIX platforms the data is fetched by the following read, so
		/// available() may still be 0 after poll() has returned true. On
		/// Windows the data is already in the internal buffer.

	void drain();
		/// Waits until output written to the serial port has been transmitted.
		///
		/// On POSIX platforms this waits until the device has transmitted the
		/// pending output, and therefore blocks for as long as the receiver
		/// does not read it.
		///
		/// On Windows it waits for queued writes to complete, which does not
		/// guarantee that the transmit buffer of the device is empty, so
		/// switching the direction of an RS-485 transceiver immediately after
		/// drain() can truncate the end of the frame. A driver that does not
		/// implement a flush at all returns without doing anything.

	void configureRS485(const RS485Params& rs485Params);
		/// Enable and configure RS-485 mode, if supported.
		///
		/// Throws a Poco::NotImplementedException on platforms that have no
		/// RS-485 interface at all (macOS, Windows), and a Poco::IOException
		/// where the platform has the interface but the device refuses the
		/// request, which is what Linux does on hardware that does not support
		/// RS-485 mode.

	void setRTS(bool status);
		/// Manually sets or clears RTS.
		///
		/// Throws a Poco::IOException on Windows if the port was opened with
		/// FLOW_RTSCTS, where the driver owns the RTS line.

	[[nodiscard]] bool getRTS() const;
		/// Returns the RTS status.
		///
		/// On POSIX platforms the line is read from the device. On Windows the
		/// value that open() or the last setRTS() established is returned.

	std::size_t write(const std::string& data);
		/// Writes the given data to the port.
		/// Returns the number of characters written.
		///
		/// Throws a Poco::InvalidArgumentException if data is larger than the
		/// largest transfer the implementation can report, and a
		/// Poco::IOException if an error occurs.

	std::size_t write(const char* data, std::size_t size);
		/// Writes the given data to the port, continuing until all of it has
		/// been written.
		/// Returns the number of characters written, which is size.
		///
		/// Throws a Poco::InvalidArgumentException if size is larger than the
		/// largest transfer the implementation can report, and a
		/// Poco::IOException if an error occurs.

	std::size_t read(char* data, std::size_t size);
		/// Reads up to size characters from the port (or
		/// internal buffer) and places them in the given buffer.
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port, for as long as the port stays connected.
		/// A size of zero returns at once without waiting.
		///
		/// Returns the number of characters read.
		///
		/// Throws a Poco::IOException if an error occurs, and in particular if
		/// the peer has hung up, which is what ends the blocking wait.

	std::size_t read(char* data, std::size_t size, const Poco::Timespan& timeout);
		/// Attempts to read exactly size characters from
		/// the port. May read less than the requested number
		/// of characters if the given timeout is exceeded.
		///
		/// Returns the number of characters read.
		///
		/// Note: the given timeout is not an absolute timeout, but a timeout
		/// between blocks of data received. For example, if the timeout
		/// is 10 seconds, 10 characters are requested, and a character
		/// appears every 5 seconds, total time for reading will be about 50
		/// seconds.
		///
		/// Throws a Poco::IOException if an error occurs, and in particular if
		/// the peer has hung up, in which case characters already read are
		/// lost with the exception.

	void read(std::string& data);
		/// Reads what's currently available from the serial port
		/// or internal buffer and returns it in data.
		///
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port, for as long as the port stays connected.
		///
		/// Throws a Poco::IOException if an error occurs, and in particular if
		/// the peer has hung up, which is what ends the blocking wait.

	int read();
		/// Reads a single character from the port.
		///
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port, for as long as the port stays connected.
		///
		/// Returns the character read.
		///
		/// Throws a Poco::IOException if an error occurs, and in particular if
		/// the peer has hung up, which is what ends the blocking wait.

	[[nodiscard]] std::size_t available() const;
		/// Returns the number of characters available from
		/// the internal buffer.

	[[nodiscard]] const std::string& device() const;
		/// Returns the device configured for this SerialPort instance.

	[[nodiscard]] int baudRate() const;
		/// Returns the baud rate configured for this SerialPort instance.

	[[nodiscard]] const std::string& parameters() const;
		/// Returns the parameters configured for this SerialPort instance.

	[[nodiscard]] FlowControl flowControl() const;
		/// Returns the flow control setting configured for this SerialPort instance.

protected:
	void readBuffer();

	static const std::string LOGGER_NAME;

private:
	std::string _device;
	int _baudRate;
	std::string _parameters;
	FlowControl _flowControl;
	Poco::Logger& _logger;
	Poco::Buffer<char> _buffer;
	char* _end;
	char* _cur;
};


//
// inlines
//
inline bool SerialPort::isOpen() const
{
	return isOpenImpl();
}


inline const std::string& SerialPort::device() const
{
	return _device;
}


inline int SerialPort::baudRate() const
{
	return _baudRate;
}


inline const std::string& SerialPort::parameters() const
{
	return _parameters;
}


inline SerialPort::FlowControl SerialPort::flowControl() const
{
	return _flowControl;
}


inline std::size_t SerialPort::available() const
{
	// A moved-from port has no buffer and both cursors are null. Subtracting
	// two null pointers is only defined from C++20 on.
	if (_cur == nullptr) return 0;
	return static_cast<std::size_t>(_end - _cur);
}


} // namespace Poco::Serial


#endif // Poco_Serial_SerialPort_INCLUDED
