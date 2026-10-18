//
// SerialPort_POSIX.cpp
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


#include "Poco/Serial/SerialPort_POSIX.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Error.h"
#include "Poco/Clock.h"
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <cerrno>
#include <cstring>
#include <limits>
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
#include <linux/serial.h>
#endif


using Poco::format;


namespace Poco::Serial {


SerialPortImpl::SerialPortImpl(SerialPortImpl&& other) noexcept:
	_fd(other._fd)
{
	other._fd = -1;
}


SerialPortImpl& SerialPortImpl::operator = (SerialPortImpl&& other) noexcept
{
	if (this != &other)
	{
		closeImpl();
		_fd = other._fd;
		other._fd = -1;
	}
	return *this;
}



namespace
{
	void setRawMode(struct termios& term)
		/// Configures the line discipline to pass every byte through unchanged.
	{
		// IEXTEN is cleared because the BSD line discipline acts on VLNEXT and
		// VDISCARD outside canonical mode, which loses those two bytes of a
		// binary frame and can stop all output.
		term.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB | CRTSCTS);
		term.c_cflag |= CLOCAL | CREAD;
		term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY |
			IGNPAR | INPCK);
		term.c_oflag &= ~(OPOST | ONLCR | OCRNL | ONOCR | ONLRET);
		term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);
		term.c_cc[VMIN]  = 1;
		term.c_cc[VTIME] = 0;
	}


	void applyFrame(struct termios& term, const std::string& parameters)
		/// Applies the character size, the parity and the number of stop bits.
	{
		switch (parameters[0])
		{
		case '5': term.c_cflag |= CS5; break;
		case '6': term.c_cflag |= CS6; break;
		case '7': term.c_cflag |= CS7; break;
		case '8': term.c_cflag |= CS8; break;
		default: throw Poco::InvalidArgumentException("invalid character size", parameters);
		}

		switch (parameters[1])
		{
		case 'N':
		case 'n':
			term.c_iflag |= IGNPAR;
			break;
		case 'E':
		case 'e':
			term.c_cflag |= PARENB;
			term.c_iflag |= INPCK;
			break;
		case 'o':
		case 'O':
			term.c_cflag |= PARENB | PARODD;
			term.c_iflag |= INPCK;
			break;
		default: throw Poco::InvalidArgumentException("invalid parity setting", parameters);
		}

		switch (parameters[2])
		{
		case '1':
			break;
		case '2':
			term.c_cflag |= CSTOPB;
			break;
		default: throw Poco::InvalidArgumentException("invalid number of stop bits", parameters);
		}
	}


	void applyFlowControl(struct termios& term, SerialPortImpl::FlowControlImpl flowControl)
	{
		switch (flowControl)
		{
		case SerialPortImpl::FLOW_NONE:
			break;
		case SerialPortImpl::FLOW_RTSCTS:
			term.c_cflag |= CRTSCTS;
			break;
		}
	}


	speed_t toSpeed(int baudRate)
		/// Maps a line speed in bits per second to its termios constant. A speed
		/// of -1 is handled by the caller and must not reach this function.
	{
		switch (baudRate)
		{
		case      50:   return      B50;
		case      75:   return      B75;
		case     110:   return     B110;
		case     134:   return     B134;
		case     150:   return     B150;
		case     200:   return     B200;
		case     300:   return     B300;
		case     600:   return     B600;
		case    1200:   return    B1200;
		case    1800:   return    B1800;
		case    2400:   return    B2400;
		case    4800:   return    B4800;
		case    9600:   return    B9600;
		case   19200:   return   B19200;
		case   38400:   return   B38400;
		case   57600:   return   B57600;
		case  115200:   return  B115200;
		case  230400:   return  B230400;
	#ifdef B460800
		case  460800:   return  B460800;
	#endif
	#ifdef B500000
		case  500000:   return  B500000;
	#endif
	#ifdef B576000
		case  576000:   return  B576000;
	#endif
	#ifdef B921600
		case  921600:   return  B921600;
	#endif
	#ifdef B1000000
		case 1000000:   return B1000000;
	#endif
	#ifdef B1152000
		case 1152000:   return B1152000;
	#endif
	#ifdef B1500000
		case 1500000:   return B1500000;
	#endif
	#ifdef B2000000
		case 2000000:   return B2000000;
	#endif
	#ifdef B2500000
		case 2500000:   return B2500000;
	#endif
	#ifdef B3000000
		case 3000000:   return B3000000;
	#endif
	#ifdef B3500000
		case 3500000:   return B3500000;
	#endif
	#ifdef B4000000
		case 4000000:   return B4000000;
	#endif
		default: throw Poco::InvalidArgumentException(format("unsupported serial line speed: %d", baudRate));
		}
	}


	void verifyFrame(int fd, const struct termios& requested, const std::string& device)
	{
		// tcsetattr() reports success when it has applied only part of the
		// request, so the settings that make up the character frame are read
		// back and compared with what was asked for. The speed is not compared:
		// a device may legitimately round it, and cfsetspeed() already fails for
		// a speed the platform cannot represent.
		struct termios actual;
		if (tcgetattr(fd, &actual) == -1)
		{
			const int err = errno;
			throw Poco::IOException("cannot verify serial port configuration for " + device,
				Poco::Error::getMessage(err));
		}
		if ((actual.c_cflag & CSIZE) != (requested.c_cflag & CSIZE))
			throw Poco::IOException("serial port does not support the requested character size", device);
		if ((actual.c_cflag & (PARENB | PARODD)) != (requested.c_cflag & (PARENB | PARODD)))
			throw Poco::IOException("serial port does not support the requested parity", device);
		if ((actual.c_cflag & CSTOPB) != (requested.c_cflag & CSTOPB))
			throw Poco::IOException("serial port does not support the requested number of stop bits", device);
	}
}


void SerialPortImpl::openImpl(const std::string& device, int baudRate, const std::string& parameters, FlowControlImpl flowControl)
{
	_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (_fd == -1)
	{
		const int err = errno;
		throw Poco::IOException("cannot open serial port " + device, Poco::Error::getMessage(err));
	}

	try
	{
		// Clearing the file status flags clears O_NDELAY with them, so that a
		// read blocks until at least VMIN bytes have arrived.
		if (fcntl(_fd, F_SETFL, 0) == -1)
		{
			const int err = errno;
			throw Poco::IOException("error calling fcntl() on " + device, Poco::Error::getMessage(err));
		}

		struct termios term;
		if (tcgetattr(_fd, &term) == -1)
		{
			const int err = errno;
			throw Poco::IOException("cannot get serial port configuration for " + device, Poco::Error::getMessage(err));
		}

		setRawMode(term);
		applyFrame(term, parameters);
		applyFlowControl(term, flowControl);

		// A speed of -1, the default, leaves the device at its configured speed.
		if (baudRate != -1 && cfsetspeed(&term, toSpeed(baudRate)) == -1)
		{
			const int err = errno;
			throw Poco::IOException("error setting baud rate on serial port " + device,
				Poco::Error::getMessage(err));
		}

		if (tcflush(_fd, TCIOFLUSH) == -1)
		{
			const int err = errno;
			throw Poco::IOException("error flushing serial port " + device, Poco::Error::getMessage(err));
		}

		if (tcsetattr(_fd, TCSANOW, &term) == -1)
		{
			const int err = errno;
			throw Poco::IOException("error configuring serial port " + device, Poco::Error::getMessage(err));
		}

		verifyFrame(_fd, term, device);
	}
	catch (...)
	{
		closeImpl();
		throw;
	}
}


void SerialPortImpl::closeImpl()
{
	if (_fd != -1)
	{
		::close(_fd);
		_fd = -1;
	}
}


void SerialPortImpl::drainImpl()
{
	int rc;
	do
	{
		rc = tcdrain(_fd);
	}
	while (rc == -1 && errno == EINTR);
	if (rc == -1)
	{
		const int err = errno;
		throw Poco::IOException("error draining serial port", Poco::Error::getMessage(err));
	}
}


void SerialPortImpl::configureRS485Impl(const RS485ParamsImpl& rs485Params)
{
#if defined(TIOCSRS485)
	struct serial_rs485 rs485conf;
	std::memset(&rs485conf, 0, sizeof(rs485conf));
	rs485conf.flags = rs485Params.flags;
	rs485conf.delay_rts_before_send = rs485Params.delayRTSBeforeSend;
	rs485conf.delay_rts_after_send  = rs485Params.delayRTSAfterSend;
	// BeagleBone needs a hack to enable RTS via GPIO pin
	#if defined(SER_RS485_USE_GPIO) // patched <linux/serial.h> header available
		rs485conf.gpio_pin = rs485Params.gpioPin;
	#elif defined(POCO_SERIAL_ENABLE_BEAGLEBONE_RS485_HACK) // unpatched <linux/serial.h> header: gpioPin is padding[0]
		rs485conf.padding[0] = rs485Params.gpioPin;
	#endif
	if (ioctl (_fd, TIOCSRS485, &rs485conf) < 0)
	{
		const int err = errno;
		throw Poco::IOException("error configuring RS-485 mode for serial port", Poco::Error::getMessage(err));
	}
#else
	throw Poco::NotImplementedException("RS-485 mode");
#endif
}


void SerialPortImpl::setRTSImpl(bool status)
{
	int flags;
	if (ioctl(_fd, TIOCMGET, &flags) == -1)
	{
		const int err = errno;
		throw Poco::IOException("error getting serial port flags", Poco::Error::getMessage(err));
	}

	if (status)
		flags |= TIOCM_RTS;
	else
		flags &= ~TIOCM_RTS;

	if (ioctl(_fd, TIOCMSET, &flags) == -1)
	{
		const int err = errno;
		throw Poco::IOException("error setting serial port flags", Poco::Error::getMessage(err));
	}
}


bool SerialPortImpl::getRTSImpl() const
{
	int flags;
	if (ioctl(_fd, TIOCMGET, &flags) == -1)
	{
		const int err = errno;
		throw Poco::IOException("error getting serial port flags", Poco::Error::getMessage(err));
	}

	return (flags & TIOCM_RTS) != 0;
}


int SerialPortImpl::pollImpl(char* /*data*/, std::size_t /*size*/, const Poco::Timespan& timeout)
{
	// ::poll() is used instead of ::select(), which cannot wait on a
	// descriptor at or above FD_SETSIZE.
	pollfd pollBuf;
	std::memset(&pollBuf, 0, sizeof(pollBuf));
	pollBuf.fd = _fd;
	pollBuf.events = POLLIN;

	Poco::Timespan remainingTime(timeout);
	int rc;
	do
	{
		// ::poll() takes the wait as an int number of milliseconds, where a
		// negative value means no timeout at all, so a longer wait is capped
		// at the largest value that argument can hold.
		const Poco::Timespan::TimeDiff ms = remainingTime.totalMilliseconds();
		const int timeoutMillis =
			ms < std::numeric_limits<int>::max() ? static_cast<int>(ms) : std::numeric_limits<int>::max();
		Poco::Clock start;
		rc = ::poll(&pollBuf, 1, timeoutMillis);
		if (rc < 0 && errno == EINTR)
		{
			const Poco::Timespan waited(start.elapsed());
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	}
	while (rc < 0 && errno == EINTR);
	if (rc < 0)
	{
		const int err = errno;
		throw Poco::IOException("serial port poll error", Poco::Error::getMessage(err));
	}
	return rc > 0 ? 0 : -1;
}


int SerialPortImpl::writeImpl(const char* data, std::size_t size)
{
	// The caller limits a transfer to what an int can hold.
	ssize_t n;
	do
	{
		n = ::write(_fd, data, size);
	}
	while (n < 0 && errno == EINTR);
	return static_cast<int>(n);
}


int SerialPortImpl::readImpl(char* data, std::size_t size)
{
	// The caller limits a transfer to what an int can hold.
	ssize_t n;
	do
	{
		n = ::read(_fd, data, size);
	}
	while (n < 0 && errno == EINTR);
	return static_cast<int>(n);
}


} // namespace Poco::Serial
