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
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Poco_Serial_SerialPort_INCLUDED
#define Poco_Serial_SerialPort_INCLUDED


#include "Poco/Serial/Serial.h"
#include "Poco/Logger.h"
#include "Poco/Timespan.h"
#include "Poco/Buffer.h"
#include <cctype>


#if defined(_WIN32)
#include "Poco/Serial/SerialPort_WIN32.h"
#else
#include "Poco/Serial/SerialPort_POSIX.h"
#endif


namespace Poco {
namespace Serial {


class Serial_API SerialPort: private SerialPortImpl
	/// This class provides an interface to a serial port.
{
public:
	enum FlowControl
	{
		FLOW_NONE,   /// No flow control
		FLOW_RTSCTS  /// Hardware (RTS/CTS) flow control
	};
	
	enum
	{
		DEFAULT_BUFFER_SIZE = 4096 /// Default size for the internal buffer.
	};
	
	typedef RS485ParamsImpl RS485Params;
	
	SerialPort();
		/// Creates the SerialPort.

	explicit SerialPort(std::size_t bufferSize);
		/// Creates the SerialPort, using the given buffer size.
		
	SerialPort(Poco::Logger& logger, std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
		/// Creates the SerialPort, using the given logger and buffer size.
		
	SerialPort(const std::string& device, int baudRate = -1, const std::string& parameters = "8N1", FlowControl flowControl = FLOW_NONE, std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
		/// Creates and opens the SerialPort, using the given parameters.
		///
		/// See open() for a description of the parameters.
		
	~SerialPort();
		/// Closes, if necessary, and destroys the SerialPort.
		
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
		///
		/// Specify -1 for baudRate to use the currently configured speed.
		/// Note that not all platforms support all baud rates.		
		///
		/// To specify character size, parity and stop bits, use the
		/// parameters argument. The parameters argument must be a three
		/// character string. The first character specifies the serial line
		/// character width in bits (5, 6, 7 or 8). The second char specifies
		/// the parity (N for none, E for even, O for odd) and the third
		/// character specifies the number of stop bits (1 or 2).
		/// Example: "8N1" for 8 bit characters, no parity, 1 stop bit.
		///
		/// The flowControl argument can be used to enable or disable
		/// hardware (CTS/RTS) flow control.
		
	void close();
		/// Closes the serial port.
	
	bool poll(const Poco::Timespan& timeout);
		/// Waits for data to arrive at the port.
		///
		/// Returns true immediately if data is already in 
		/// the internal buffer, or if data arrives during the
		/// specified time interval, otherwise false.
		
	void drain();
		/// Waits until all output written to the serial port has been transmitted.

	void configureRS485(const RS485Params& rs485Params);
		/// Enable and configure RS-485 mode, if supported.
		///
		/// Throws a Poco::NotImplementedException if the hardware does not support RS-485 mode.

	void setRTS(bool status);
		/// Manually sets or clears RTS.
		
	bool getRTS() const;
		/// Returns the RTS status.
				
	std::size_t write(const std::string& data);
		/// Writes the given data to the port.
		/// Returns the number of characters written.
		///
		/// Throws a Poco::IOException if an error occurs.
		
	std::size_t write(const char* data, std::size_t size);
		/// Writes the given data to the port.
		/// Returns the number of characters written.
		///
		/// Throws a Poco::IOException if an error occurs.

	std::size_t read(char* data, std::size_t size);
		/// Reads up to size characters from the port (or
		/// internal buffer) and places them in the given buffer.
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port.
		///
		/// Returns the number of characters read.
		///
		/// Throws a Poco::IOException if an error occurs.

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
		/// Throws a Poco::IOException if an error occurs.
		
	void read(std::string& data);
		/// Reads what's currently available from the serial port 
		/// or internal buffer and returns it in data.
		///
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port.
	
	int read();
		/// Reads a single character from the port.
		///
		/// If the internal buffer is empty, blocks until data
		/// arrives at the port.
		
	std::size_t available() const;
		/// Returns the number of characters available from
		/// the internal buffer.

	const std::string& device() const;
		/// Returns the device configured for this SerialPort instance.
		
	int baudRate() const;
		/// Returns the baud rate configured for this SerialPort instance.
		
	const std::string& parameters() const;
		/// Returns the parameters configured for this SerialPort instance.
		
	FlowControl flowControl() const;
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
	return static_cast<std::size_t>(_end - _cur);
}


} } // namespace Poco::Serial


#endif // Poco_Serial_SerialPort_INCLUDED
