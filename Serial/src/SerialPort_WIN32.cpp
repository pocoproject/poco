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
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/Serial/SerialPort_WIN32.h"
#include "Poco/Format.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Exception.h"


using Poco::format;


namespace Poco {
namespace Serial {


SerialPortImpl::SerialPortImpl():
	_handle(INVALID_HANDLE_VALUE),
	_rtsState(false)
{
}


SerialPortImpl::~SerialPortImpl()
{
}


void SerialPortImpl::openImpl(const std::string& device, int baudRate, const std::string& parameters, FlowControlImpl flowControl)
{
	if (device.empty()) throw Poco::InvalidArgumentException("no device specified");

	std::string fixedDeviceName = (device[0] == '\\')? device: "\\\\.\\" + device;
	std::wstring wdevice;
	Poco::UnicodeConverter::toUTF16(fixedDeviceName, wdevice);

	_handle = CreateFileW(wdevice.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (_handle == INVALID_HANDLE_VALUE) throw Poco::IOException("cannot open serial port " + device);

	try
	{
		DCB dcb = {0};
		dcb.DCBlength = sizeof(dcb);
		if (!GetCommState(_handle, &dcb)) throw Poco::IOException("cannot get serial port configuration for " + device);

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
			dcb.fOutxDsrFlow = TRUE;
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
			throw Poco::IOException("error setting serial port parameters on serial port " + device);

		COMMTIMEOUTS cto;
		cto.ReadIntervalTimeout         = CHARACTER_TIMEOUT;
		cto.ReadTotalTimeoutConstant    = MAXDWORD;
		cto.ReadTotalTimeoutMultiplier  = 0;
		cto.WriteTotalTimeoutConstant   = MAXDWORD;
		cto.WriteTotalTimeoutMultiplier = 0;
		if (!SetCommTimeouts(_handle, &cto))
			throw Poco::IOException("error setting serial port timeouts on serial port " + device);
	}
	catch (...)
	{
		closeImpl();
		throw;
	}
}


void SerialPortImpl::closeImpl()
{
	CloseHandle(_handle);
	_handle = INVALID_HANDLE_VALUE;
}


void SerialPortImpl::drainImpl()
{
}


void SerialPortImpl::configureRS485Impl(const RS485ParamsImpl& rs485Params)
{
	throw Poco::NotImplementedException("RS-485 mode");
}


void SerialPortImpl::setRTSImpl(bool status)
{
	DWORD func = status ? SETRTS : CLRRTS;
	if (!EscapeCommFunction(_handle, func))
		throw Poco::IOException("error setting RTS");
	_rtsState = status;
}


bool SerialPortImpl::getRTSImpl() const
{
	return _rtsState;
}


int SerialPortImpl::pollImpl(char* buffer, std::size_t size, const Poco::Timespan& timeout)
{
	COMMTIMEOUTS prevCTO;
	if (!GetCommTimeouts(_handle, &prevCTO))
		throw Poco::IOException("error getting serial port timeouts");

	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout         = CHARACTER_TIMEOUT;
	cto.ReadTotalTimeoutConstant    = static_cast<DWORD>(timeout.totalMilliseconds());
	cto.ReadTotalTimeoutMultiplier  = 0;
	cto.WriteTotalTimeoutConstant   = MAXDWORD;
	cto.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(_handle, &cto))
		throw Poco::IOException("error setting serial port timeouts on serial port");

	try
	{
		DWORD bytesRead = 0;
		if (!ReadFile(_handle, buffer, size, &bytesRead, NULL))
			throw Poco::IOException("failed to read from serial port");
		SetCommTimeouts(_handle, &prevCTO);
		return (bytesRead == 0) ? -1 : bytesRead;
	}
	catch (...)
	{
		SetCommTimeouts(_handle, &prevCTO);
		throw;
	}
}


int SerialPortImpl::writeImpl(const char* buffer, std::size_t size)
{
	DWORD bytesWritten = 0;
	if (!WriteFile(_handle, buffer, size, &bytesWritten, NULL))
		return -1;
	return bytesWritten;
}


int SerialPortImpl::readImpl(char* buffer, std::size_t size)
{
	DWORD bytesRead = 0;
	if (!ReadFile(_handle, buffer, size, &bytesRead, NULL))
		return -1;
	return bytesRead;
}


} } // namespace Poco::Serial
