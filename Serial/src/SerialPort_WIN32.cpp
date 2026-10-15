//
// SerialPort_WIN32.cpp
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


#include "Poco/Serial/SerialPort_WIN32.h"
#include "Poco/Format.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Exception.h"
#include "Poco/Error.h"


using Poco::format;


namespace Poco::Serial {


SerialPortImpl::SerialPortImpl(SerialPortImpl&& other) noexcept:
	_handle(other._handle),
	_rtsState(other._rtsState)
{
	other._handle = INVALID_HANDLE_VALUE;
	other._rtsState = false;
}


SerialPortImpl& SerialPortImpl::operator = (SerialPortImpl&& other) noexcept
{
	if (this != &other)
	{
		closeImpl();
		_handle = other._handle;
		_rtsState = other._rtsState;
		other._handle = INVALID_HANDLE_VALUE;
		other._rtsState = false;
	}
	return *this;
}


void SerialPortImpl::openImpl(const std::string& device, int baudRate, const std::string& parameters, FlowControlImpl flowControl)
{
	if (device.empty()) throw Poco::InvalidArgumentException("no device specified");

	std::string fixedDeviceName = (device[0] == '\\')? device: "\\\\.\\" + device;
	std::wstring wdevice;
	Poco::UnicodeConverter::toUTF16(fixedDeviceName, wdevice);

	_handle = CreateFileW(wdevice.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (_handle == INVALID_HANDLE_VALUE)
	{
		const DWORD err = GetLastError();
		throw Poco::IOException("cannot open serial port " + device, Poco::Error::getMessage(err));
	}

	try
	{
		DCB dcb = {0};
		dcb.DCBlength = sizeof(dcb);
		if (!GetCommState(_handle, &dcb))
		{
			const DWORD err = GetLastError();
			throw Poco::IOException("cannot get serial port configuration for " + device, Poco::Error::getMessage(err));
		}

		switch (parameters[0])
		{
		case '5': dcb.ByteSize = 5; break;
		case '6': dcb.ByteSize = 6; break;
		case '7': dcb.ByteSize = 7; break;
		case '8': dcb.ByteSize = 8; break;
		default: throw Poco::InvalidArgumentException("invalid character size", parameters);
		}

		switch (parameters[1])
		{
		case 'N':
		case 'n':
			dcb.Parity = NOPARITY;
			break;
		case 'E':
		case 'e':
			dcb.Parity = EVENPARITY;
			break;
		case 'o':
		case 'O':
			dcb.Parity = ODDPARITY;
			break;
		case 'm':
		case 'M':
			dcb.Parity = MARKPARITY;
			break;
		case 's':
		case 'S':
			dcb.Parity = SPACEPARITY;
			break;
		default: throw Poco::InvalidArgumentException("invalid parity setting", parameters);
		}

		// Parity selects the bit; fParity is what makes the driver check it on
		// reception, and it is inherited like every other DCB flag.
		dcb.fParity = dcb.Parity != NOPARITY;

		switch (parameters[2])
		{
		case '1':
			dcb.StopBits = ONESTOPBIT;
			break;
		case '2':
			dcb.StopBits = TWOSTOPBITS;
			break;
		case '5':
			dcb.StopBits = ONE5STOPBITS;
			break;
		default: throw Poco::InvalidArgumentException("invalid number of stop bits", parameters);
		}

		// GetCommState() reports whatever the previous user of the port left
		// behind, so the flags that would silently drop or stop data are set
		// here rather than inherited: with fNull set, received zero bytes are
		// discarded, with fDsrSensitivity set, every byte arriving while DSR is
		// low is discarded, and with fAbortOnError set, the first line error
		// makes every later call fail until ClearCommError() is called. The
		// POSIX implementation never gates reception on DSR.
		dcb.fNull            = FALSE;
		dcb.fDsrSensitivity  = FALSE;
		dcb.fAbortOnError    = FALSE;

		// Output flow control waits for CTS only, which is what the POSIX
		// implementation does with CRTSCTS; waiting for DSR as well would block
		// every write on a cable that does not wire it.
		switch (flowControl)
		{
		case FLOW_NONE:
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fOutX        = FALSE;
			dcb.fInX         = FALSE;
			dcb.fRtsControl  = RTS_CONTROL_DISABLE;
			break;
		case FLOW_RTSCTS:
			dcb.fOutxCtsFlow = TRUE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fOutX        = FALSE;
			dcb.fInX         = FALSE;
			dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;
			break;
		}

		DWORD speed = dcb.BaudRate;
		switch (baudRate)
		{
		case     -1:   break;
		case    110:   speed =    CBR_110; break;
		case    300:   speed =    CBR_300; break;
		case    600:   speed =    CBR_600; break;
		case   1200:   speed =   CBR_1200; break;
		case   2400:   speed =   CBR_2400; break;
		case   4800:   speed =   CBR_4800; break;
		case   9600:   speed =   CBR_9600; break;
		case  14400:   speed =  CBR_14400; break;
		case  19200:   speed =  CBR_19200; break;
		case  38400:   speed =  CBR_38400; break;
		case  57600:   speed =  CBR_57600; break;
		case 115200:   speed = CBR_115200; break;
		default: throw Poco::InvalidArgumentException(format("unsupported serial line speed: %d", baudRate));
		}
		dcb.BaudRate = speed;
		if (!SetCommState(_handle, &dcb))
		{
			const DWORD err = GetLastError();
			throw Poco::IOException("error setting serial port parameters on serial port " + device,
				Poco::Error::getMessage(err));
		}

		// The driver raises RTS for handshaking and lowers it otherwise, so the
		// cached state that getRTSImpl() reports starts from what was set here.
		_rtsState = dcb.fRtsControl != RTS_CONTROL_DISABLE;

		COMMTIMEOUTS cto;
		cto.ReadIntervalTimeout         = CHARACTER_TIMEOUT;
		cto.ReadTotalTimeoutConstant    = MAXDWORD;
		cto.ReadTotalTimeoutMultiplier  = 0;
		cto.WriteTotalTimeoutConstant   = MAXDWORD;
		cto.WriteTotalTimeoutMultiplier = 0;
		if (!SetCommTimeouts(_handle, &cto))
		{
			const DWORD err = GetLastError();
			throw Poco::IOException("error setting serial port timeouts on serial port " + device,
				Poco::Error::getMessage(err));
		}
	}
	catch (...)
	{
		closeImpl();
		throw;
	}
}


void SerialPortImpl::closeImpl()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
		_rtsState = false;
	}
}


void SerialPortImpl::drainImpl()
{
	if (!FlushFileBuffers(_handle))
	{
		const DWORD err = GetLastError();
		// A driver that does not implement a flush reports one of these two and
		// has nothing to wait for.
		if (err == ERROR_INVALID_FUNCTION || err == ERROR_NOT_SUPPORTED) return;
		throw Poco::IOException("error draining serial port", Poco::Error::getMessage(err));
	}
}


void SerialPortImpl::configureRS485Impl(const RS485ParamsImpl& /*rs485Params*/)
{
	throw Poco::NotImplementedException("RS-485 mode");
}


void SerialPortImpl::setRTSImpl(bool status)
{
	DWORD func = status ? SETRTS : CLRRTS;
	if (!EscapeCommFunction(_handle, func))
	{
		const DWORD err = GetLastError();
		throw Poco::IOException("error setting RTS", Poco::Error::getMessage(err));
	}
	_rtsState = status;
}


bool SerialPortImpl::getRTSImpl() const
{
	return _rtsState;
}


int SerialPortImpl::pollImpl(char* data, std::size_t size, const Poco::Timespan& timeout)
{
	COMMTIMEOUTS prevCTO;
	if (!GetCommTimeouts(_handle, &prevCTO))
	{
		const DWORD err = GetLastError();
		throw Poco::IOException("error getting serial port timeouts", Poco::Error::getMessage(err));
	}

	COMMTIMEOUTS cto;
	cto.ReadTotalTimeoutMultiplier  = 0;
	cto.WriteTotalTimeoutConstant   = MAXDWORD;
	cto.WriteTotalTimeoutMultiplier = 0;
	if (timeout.totalMicroseconds() > 0)
	{
		// A read timeout constant of zero means, together with a zero
		// multiplier, that total timeouts are not used, so ReadFile() would
		// wait for the first character indefinitely. A wait shorter than a
		// millisecond therefore becomes one millisecond, and one longer than
		// the field can hold is capped.
		const Poco::Timespan::TimeDiff millis = timeout.totalMilliseconds();
		cto.ReadIntervalTimeout = CHARACTER_TIMEOUT;
		if (millis < 1)
			cto.ReadTotalTimeoutConstant = 1;
		else if (millis > static_cast<Poco::Timespan::TimeDiff>(MAXDWORD))
			cto.ReadTotalTimeoutConstant = MAXDWORD;
		else
			cto.ReadTotalTimeoutConstant = static_cast<DWORD>(millis);
	}
	else
	{
		// This is the combination that makes ReadFile() return at once with
		// whatever has already arrived, which is what a wait of zero asks for.
		cto.ReadIntervalTimeout      = MAXDWORD;
		cto.ReadTotalTimeoutConstant = 0;
	}
	if (!SetCommTimeouts(_handle, &cto))
	{
		const DWORD err = GetLastError();
		throw Poco::IOException("error setting serial port timeouts on serial port", Poco::Error::getMessage(err));
	}

	try
	{
		DWORD bytesRead = 0;
		if (!ReadFile(_handle, data, static_cast<DWORD>(size), &bytesRead, nullptr))
		{
			const DWORD err = GetLastError();
			throw Poco::IOException("failed to read from serial port", Poco::Error::getMessage(err));
		}
		SetCommTimeouts(_handle, &prevCTO);
		return (bytesRead == 0) ? -1 : bytesRead;
	}
	catch (...)
	{
		SetCommTimeouts(_handle, &prevCTO);
		throw;
	}
}


int SerialPortImpl::writeImpl(const char* data, std::size_t size)
{
	DWORD bytesWritten = 0;
	if (!WriteFile(_handle, data, static_cast<DWORD>(size), &bytesWritten, nullptr))
		return -1;
	return bytesWritten;
}


int SerialPortImpl::readImpl(char* data, std::size_t size)
{
	DWORD bytesRead = 0;
	if (!ReadFile(_handle, data, static_cast<DWORD>(size), &bytesRead, nullptr))
		return -1;
	return bytesRead;
}


} // namespace Poco::Serial
