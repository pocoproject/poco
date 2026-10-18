//
// SerialPort.cpp
//
// Library: Serial
// Package: Serial
// Module:  SerialPort
//
// Copyright (c) 2009-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Serial/SerialPort.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <utility>


using Poco::format;


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "SerialPort_WIN32.cpp"
#else
#include "SerialPort_POSIX.cpp"
#endif


namespace Poco::Serial {


const std::string SerialPort::LOGGER_NAME("IO.SerialPort");


namespace
{
	// The implementation layer reports the number of bytes transferred as an
	// int, so no single transfer can be larger than an int can represent.
	constexpr std::size_t MAX_TRANSFER_SIZE = static_cast<std::size_t>(std::numeric_limits<int>::max());
}


SerialPort::SerialPort():
	_baudRate(-1),
	_flowControl(FLOW_NONE),
	_logger(Poco::Logger::get(LOGGER_NAME)),
	_buffer(DEFAULT_BUFFER_SIZE),
	_end(_buffer.begin()),
	_cur(_buffer.begin())
{
}


SerialPort::SerialPort(std::size_t bufferSize):
	_baudRate(-1),
	_flowControl(FLOW_NONE),
	_logger(Poco::Logger::get(LOGGER_NAME)),
	_buffer(bufferSize),
	_end(_buffer.begin()),
	_cur(_buffer.begin())
{
	if (bufferSize == 0) throw Poco::InvalidArgumentException("buffer size must not be zero");
}


SerialPort::SerialPort(Poco::Logger& logger, std::size_t bufferSize):
	_baudRate(-1),
	_flowControl(FLOW_NONE),
	_logger(logger),
	_buffer(bufferSize),
	_end(_buffer.begin()),
	_cur(_buffer.begin())
{
	if (bufferSize == 0) throw Poco::InvalidArgumentException("buffer size must not be zero");
}


SerialPort::SerialPort(const std::string& device, int baudRate, const std::string& parameters, FlowControl flowControl, std::size_t bufferSize):
	_baudRate(-1),
	_flowControl(FLOW_NONE),
	_logger(Poco::Logger::get(LOGGER_NAME)),
	_buffer(bufferSize),
	_end(_buffer.begin()),
	_cur(_buffer.begin())
{
	if (bufferSize == 0) throw Poco::InvalidArgumentException("buffer size must not be zero");

	open(device, baudRate, parameters, flowControl);
}


SerialPort::SerialPort(SerialPort&& other) noexcept:
	SerialPortImpl(std::move(static_cast<SerialPortImpl&>(other))),
	_device(std::move(other._device)),
	_baudRate(other._baudRate),
	_parameters(std::move(other._parameters)),
	_flowControl(other._flowControl),
	_logger(other._logger),
	_buffer(std::move(other._buffer)),
	_end(other._end),
	_cur(other._cur)
{
	// The buffer keeps its block when it is moved, so the two cursors stay
	// valid; the moved-from port is left closed and without a buffer. The two
	// strings are cleared because a moved-from string is valid but unspecified.
	other._device.clear();
	other._parameters.clear();
	other._baudRate = -1;
	other._flowControl = FLOW_NONE;
	other._end = nullptr;
	other._cur = nullptr;
}


SerialPort& SerialPort::operator = (SerialPort&& other) noexcept
{
	if (this != &other)
	{
		SerialPortImpl::operator = (std::move(static_cast<SerialPortImpl&>(other)));
		_device = std::move(other._device);
		_baudRate = other._baudRate;
		_parameters = std::move(other._parameters);
		_flowControl = other._flowControl;
		_buffer = std::move(other._buffer);
		_end = other._end;
		_cur = other._cur;
		other._device.clear();
		other._parameters.clear();
		other._baudRate = -1;
		other._flowControl = FLOW_NONE;
		other._end = nullptr;
		other._cur = nullptr;
	}
	return *this;
}


SerialPort::~SerialPort()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void SerialPort::open(const std::string& device, int baudRate, const std::string& parameters, FlowControl flowControl)
{
	if (isOpenImpl()) throw Poco::IllegalStateException("Port is already open", device);
	if (device.empty()) throw Poco::InvalidArgumentException("no device specified");
	if (parameters.size() != 3) throw Poco::InvalidArgumentException("Parameters string must be three characters long", parameters);
	// A port that has been moved from has no buffer left to read into.
	if (_buffer.empty()) throw Poco::IllegalStateException("Port has been moved from", device);

	if (_logger.debug())
	{
		_logger.debug(format("Opening serial port %s, speed %d, parameters %s, flow control %s", device, baudRate, parameters, std::string(flowControl == FLOW_NONE ? "none" : "RTS/CTS")));
	}

	_cur = _end = _buffer.begin();
	openImpl(device, baudRate, parameters, static_cast<FlowControlImpl>(flowControl));

	_device = device;
	_baudRate = baudRate;
	_parameters = parameters;
	_flowControl = flowControl;
}


void SerialPort::close()
{
	if (isOpenImpl())
	{
		if (_logger.debug())
		{
			_logger.debug("Closing serial port");
		}
		closeImpl();
		_cur = _end = _buffer.begin();
	}
}


void SerialPort::drain()
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	drainImpl();
}


void SerialPort::configureRS485(const RS485Params& rs485Params)
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	configureRS485Impl(rs485Params);
}


void SerialPort::setRTS(bool status)
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	setRTSImpl(status);
}


bool SerialPort::getRTS() const
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	return getRTSImpl();
}


bool SerialPort::poll(const Poco::Timespan& timeout)
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	if (available())
	{
		return true;
	}
	else
	{
		_cur = _end = _buffer.begin();
		const std::size_t size = std::min(_buffer.size(), MAX_TRANSFER_SIZE);
		// A negative timeout waits forever on some platforms, so it is treated
		// as no wait at all.
		Poco::Timespan waitTime(timeout);
		if (waitTime < 0) waitTime = 0;
		const int n = pollImpl(_cur, size, waitTime);
		if (n >= 0)
		{
			_end += n;
			return true;
		}
		else return false;
	}
}


std::size_t SerialPort::write(const char* data, std::size_t size)
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");
	if (size > MAX_TRANSFER_SIZE)
		throw Poco::InvalidArgumentException("size exceeds the maximum of a single write");

	if (_logger.debug())
	{
		_logger.dump(format("Sending %z byte(s)", size), data, size, Poco::Message::PRIO_DEBUG);
	}
	// A write can report fewer bytes than it was given, for example when a
	// signal interrupts it after it has transmitted part of the data. Those
	// bytes are gone from the caller's point of view, so the rest is written
	// here rather than reported as an error.
	std::size_t written = 0;
	while (written < size)
	{
		const int n = writeImpl(data + written, size - written);
		if (n <= 0)
			throw Poco::IOException(format("serial port write error after %z of %z bytes", written, size));
		written += static_cast<std::size_t>(n);
	}
	return written;
}


std::size_t SerialPort::write(const std::string& data)
{
	return write(data.data(), data.size());
}


std::size_t SerialPort::read(char* data, std::size_t size)
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");
	// Asking for nothing must not wait for a byte to arrive.
	if (size == 0) return 0;

	if (_cur == _end)
	{
		readBuffer();
	}
	std::size_t n = _end - _cur;
	if (n > size) n = size;
	std::memcpy(data, _cur, n);
	_cur += n;
	return n;
}


std::size_t SerialPort::read(char* data, std::size_t size, const Poco::Timespan& timeout)
{
	std::size_t n = 0;
	while (n < size)
	{
		if (available() || poll(timeout))
		{
			std::size_t rd = read(data + n, size - n);
			n += rd;
		}
		else break;
	}
	return n;
}


void SerialPort::read(std::string& data)
{
	if (_cur == _end)
	{
		readBuffer();
	}
	data.assign(_cur, _end);
	_cur = _end;
}


int SerialPort::read()
{
	if (_cur == _end)
	{
		readBuffer();
	}
	if (_cur < _end)
		return static_cast<int>(static_cast<unsigned char>(*_cur++));
	else
		return -1;
}


void SerialPort::readBuffer()
{
	if (!isOpenImpl()) throw Poco::IllegalStateException("Port is not open");

	_cur = _end = _buffer.begin();
	const std::size_t size = std::min(_buffer.size(), MAX_TRANSFER_SIZE);
	const int n = readImpl(_cur, size);
	if (n < 0) throw Poco::IOException("serial port read error");
	// open() asks the device for at least one character per read, so a read
	// of zero bytes on an open port means that the peer has hung up.
	if (n == 0) throw Poco::IOException("serial port was disconnected");

	_end += n;
	if (_logger.debug())
	{
		_logger.dump(format("Received %d byte(s)", n), _cur, n, Poco::Message::PRIO_DEBUG);
	}
}


} // namespace Poco::Serial
