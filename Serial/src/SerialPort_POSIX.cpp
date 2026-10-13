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
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/Serial/SerialPort_POSIX.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
#include <linux/serial.h>
#endif


using Poco::format;


namespace Poco {
namespace Serial {


SerialPortImpl::SerialPortImpl():
	_fd(-1)
{
}


SerialPortImpl::~SerialPortImpl()
{
}


void SerialPortImpl::openImpl(const std::string& device, int baudRate, const std::string& parameters, FlowControlImpl flowControl)
{
	_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (_fd == -1) throw Poco::IOException("cannot open serial port " + device, strerror(errno));

	try
	{
		int rc = fcntl(_fd, F_SETFL, 0); // FNDELAY for non-blocking
		if (rc == -1) throw Poco::IOException("error calling fcntl() on " + device, strerror(errno));

		struct termios term;
		rc = tcgetattr(_fd, &term);
		if (rc == -1) throw Poco::IOException("cannot get serial port configuration for " + device, strerror(errno));

		term.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB | CRTSCTS);
		term.c_cflag |= CLOCAL | CREAD;
		term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		term.c_oflag &= ~(OPOST | ONLCR | OCRNL | ONOCR | ONLRET);
		term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

		switch (parameters[0])
		{
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

		switch (flowControl)
		{
		case FLOW_NONE:
			break;
		case FLOW_RTSCTS:
			term.c_cflag |= CRTSCTS;
			break;
		}

		speed_t speed(B0);
		switch (baudRate)
		{
		case      -1:   break;
		case      50:   speed =      B50; break;
		case      75:   speed =      B75; break;
		case     110:   speed =     B110; break;
		case     134:   speed =     B134; break;
		case     150:   speed =     B150; break;
		case     200:   speed =     B200; break;
		case     300:   speed =     B300; break;
		case     600:   speed =     B600; break;
		case    1200:   speed =    B1200; break;
		case    1800:   speed =    B1800; break;
		case    2400:   speed =    B2400; break;
		case    4800:   speed =    B4800; break;
		case    9600:   speed =    B9600; break;
		case   19200:   speed =   B19200; break;
		case   38400:   speed =   B38400; break;
		case   57600:   speed =   B57600; break;
		case  115200:   speed =  B115200; break;
		case  230400:   speed =  B230400; break;
	#ifdef B460800
		case  460800:   speed =  B460800; break;
	#endif
	#ifdef B500000
		case  500000:   speed =  B500000; break;
	#endif
	#ifdef B576000
		case  576000:   speed =  B576000; break;
	#endif
	#ifdef B921600
		case  921600:   speed =  B921600; break;
	#endif
	#ifdef B1000000
		case 1000000:   speed = B1000000; break;
	#endif
	#ifdef B1152000
		case 1152000:   speed = B1152000; break;
	#endif
	#ifdef B1500000
		case 1500000:   speed = B1500000; break;
	#endif
	#ifdef B2000000
		case 2000000:   speed = B2000000; break;
	#endif
	#ifdef B2500000
		case 2500000:   speed = B2500000; break;
	#endif
	#ifdef B3000000
		case 3000000:   speed = B3000000; break;
	#endif
	#ifdef B3500000
		case 3500000:   speed = B3500000; break;
	#endif
	#ifdef B4000000
		case 4000000:   speed = B4000000; break;
	#endif
		default: throw Poco::InvalidArgumentException(format("unsupported serial line speed: %d", baudRate));
		}

		if (baudRate != -1)
		{
			rc = cfsetspeed(&term, speed);
			if (rc == -1) throw Poco::IOException("error setting baud rate on serial port " + device, strerror(errno));
		}

		rc = tcflush(_fd, TCIOFLUSH);
		if (rc == -1) throw Poco::IOException("error flushing serial port " + device, strerror(errno));

		rc = tcsetattr(_fd, TCSANOW, &term);
		if (rc == -1) throw Poco::IOException("error configuring serial port " + device, strerror(errno));
	}
	catch (...)
	{
		closeImpl();
		throw;
	}
}


void SerialPortImpl::closeImpl()
{
	::close(_fd);
	_fd = -1;
}


void SerialPortImpl::drainImpl()
{
	poco_assert (_fd != -1);

	int rc = tcdrain(_fd);
 	if (rc == -1) throw Poco::IOException("error draining serial port", strerror(errno));
}


void SerialPortImpl::configureRS485Impl(const RS485ParamsImpl& rs485Params)
{
	poco_assert (_fd != -1);

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
		throw Poco::IOException("error configuring RS-485 mode for serial port", strerror(errno));
	}
#else
	throw Poco::NotImplementedException("RS-485 mode");
#endif
}


void SerialPortImpl::setRTSImpl(bool status)
{
	poco_assert (_fd != -1);

	int flags;
	if (ioctl(_fd, TIOCMGET, &flags) == -1)
		throw Poco::IOException("error getting serial port flags", strerror(errno));

	if (status)
		flags |= TIOCM_RTS;
	else
		flags &= ~TIOCM_RTS;

	if (ioctl(_fd, TIOCMSET, &flags) == -1)
		throw Poco::IOException("error setting serial port flags", strerror(errno));
}


bool SerialPortImpl::getRTSImpl() const
{
	poco_assert (_fd != -1);

	int flags;
	if (ioctl(_fd, TIOCMGET, &flags) == -1)
		throw Poco::IOException("error getting serial port flags", strerror(errno));

	return flags & TIOCM_RTS;
}


int SerialPortImpl::pollImpl(char* data, std::size_t size, const Poco::Timespan& timeout)
{
	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;
	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExcept);
	FD_SET(_fd, &fdRead);
	Poco::Timespan remainingTime(timeout);
	int rc;
	do
	{
		struct timeval tv;
		tv.tv_sec  = (long) remainingTime.totalSeconds();
		tv.tv_usec = (long) remainingTime.useconds();
		Poco::Timestamp start;
		rc = ::select(_fd + 1, &fdRead, &fdWrite, &fdExcept, &tv);
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
	if (rc < 0) throw Poco::IOException("serial port poll error", strerror(errno));
	return rc > 0 ? 0 : -1;
}


int SerialPortImpl::writeImpl(const char* buffer, std::size_t size)
{
	return ::write(_fd, buffer, size);
}


int SerialPortImpl::readImpl(char* buffer, std::size_t size)
{
	return ::read(_fd, buffer, size);
}


} } // namespace Poco::Serial
