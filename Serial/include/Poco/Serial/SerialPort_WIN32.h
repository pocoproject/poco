//
// SerialPort_WIN32.h
//
// Library: Serial
// Package: Serial
// Module:  SerialPort
//
// Definition of the SerialPortImpl class for Windows (CE).
//
// Copyright (c) 2009-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Poco_Serial_SerialPort_WIN32_INCLUDED
#define Poco_Serial_SerialPort_WIN32_INCLUDED


#include "Poco/Serial/Serial.h"
#include "Poco/Timespan.h"
#include "Poco/UnWindows.h"
#include <cstddef>


namespace Poco::Serial {


class Serial_API SerialPortImpl
	/// Serial port implementation for Windows.
{
public:
	enum FlowControlImpl
	{
		FLOW_NONE,
		FLOW_RTSCTS
	};

	struct RS485ParamsImpl
	{
		enum RS485Flags
		{
			RS485_ENABLED         = (1 << 0),
				/// Enable RS-485 mode for serial port.
			RS485_RTS_ON_SEND     = (1 << 1),
				/// Set logical level for RTS pin equal to 1 when sending,
				/// enabling the RS-485 driver.
			RS485_RTS_AFTER_SEND  = (1 << 2),
				/// Set logical level for RTS pin equal to 1 after sending,
				/// disabling the RS-485 driver.
			RS485_USE_GPIO        = (1 << 5)
				/// Use GPIO pin instead of RTS to enable/disable the
				/// RS-485 driver.
		};

		int flags = 0;
			/// Flags to configure RS-485 mode.
		int delayRTSBeforeSend = 0;
			/// RTS delay before send.
		int delayRTSAfterSend = 0;
			/// RTS delay after send.
		int gpioPin = 0;
			/// GPIO pin to use if RS485_USE_GPIO flag is set.
	};

	SerialPortImpl(const SerialPortImpl&) = delete;
	SerialPortImpl& operator = (const SerialPortImpl&) = delete;

protected:
	SerialPortImpl() = default;
	~SerialPortImpl() = default;

	SerialPortImpl(SerialPortImpl&& other) noexcept;
	SerialPortImpl& operator = (SerialPortImpl&& other) noexcept;

	void openImpl(const std::string& device, int baudRate, const std::string& parameters, FlowControlImpl flowControl);
	void closeImpl();
	[[nodiscard]] bool isOpenImpl() const;

	void drainImpl();
	void configureRS485Impl(const RS485ParamsImpl& rs485Params);
	void setRTSImpl(bool status);
	[[nodiscard]] bool getRTSImpl() const;
	[[nodiscard]] int pollImpl(char* data, std::size_t size, const Poco::Timespan& timeout);
	int readImpl(char* data, std::size_t size);
	int writeImpl(const char* data, std::size_t size);

private:
	static constexpr DWORD CHARACTER_TIMEOUT = 2; // timeout for single characters in milliseconds

	HANDLE _handle = INVALID_HANDLE_VALUE;
	bool _rtsState = false;
};


//
// inlines
//
inline bool SerialPortImpl::isOpenImpl() const
{
	return _handle != INVALID_HANDLE_VALUE;
}


} // namespace Poco::Serial


#endif // Poco_Serial_SerialPort_WIN32_INCLUDED
