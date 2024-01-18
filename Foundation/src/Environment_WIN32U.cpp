//
// Environment_WIN32U.cpp
//
// Library: Foundation
// Package: Core
// Module:  Environment
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Environment_WIN32U.h"
#include "Poco/Exception.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Buffer.h"

#include <sstream>
#include <cstring>
#include <memory>
#include <winsock2.h>
#include <iphlpapi.h>

#if defined(_MSC_VER)
#pragma warning(disable:4996) // deprecation warnings
#endif

#if (POCO_ARCH == POCO_ARCH_IA32)
// It is expected that _USE_32BIT_TIME_T is defined when compiling on 32-bit Windows
// to have the structure IP_ADAPTER_INFO defined properly.
//
// https://learn.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_info
//
#if !defined(_USE_32BIT_TIME_T)
	#if defined(POCO_COMPILER_MSVC)
	#pragma message("WARNING: _USE_32BIT_TIME_T is expected to be defined for compiling " __FILE__ )
	#endif
#endif
static_assert( sizeof(__time32_t) == 4, "Unexpected size of __time32_t.");
static_assert( sizeof(time_t) == sizeof(__time32_t), "Size of time_t != __time32_t.");
#endif

namespace Poco {


std::string EnvironmentImpl::getImpl(const std::string& name)
{
	std::wstring uname;
	UnicodeConverter::toUTF16(name, uname);
	DWORD len = GetEnvironmentVariableW(uname.c_str(), 0, 0);
	if (len == 0) throw NotFoundException(name);
	Buffer<wchar_t> buffer(len);
	GetEnvironmentVariableW(uname.c_str(), buffer.begin(), len);
	std::string result;
	UnicodeConverter::toUTF8(buffer.begin(), len - 1, result);
	return result;
}


bool EnvironmentImpl::hasImpl(const std::string& name)
{
	std::wstring uname;
	UnicodeConverter::toUTF16(name, uname);
	DWORD len = GetEnvironmentVariableW(uname.c_str(), 0, 0);
	return len > 0;
}


void EnvironmentImpl::setImpl(const std::string& name, const std::string& value)
{
	std::wstring uname;
	std::wstring uvalue;
	UnicodeConverter::toUTF16(name, uname);
	UnicodeConverter::toUTF16(value, uvalue);
	if (SetEnvironmentVariableW(uname.c_str(), uvalue.c_str()) == 0)
	{
		std::string msg = "cannot set environment variable: ";
		msg.append(name);
		throw SystemException(msg);
	}
}


std::string EnvironmentImpl::osNameImpl()
{
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionEx(&vi) == 0) throw SystemException("Cannot get OS version information");
	switch (vi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		return "Windows 3.x";
	case VER_PLATFORM_WIN32_WINDOWS:
		return vi.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
	case VER_PLATFORM_WIN32_NT:
		return "Windows NT";
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::osDisplayNameImpl()
{
	OSVERSIONINFOEX vi;	// OSVERSIONINFOEX is supported starting at Windows 2000
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionEx((OSVERSIONINFO*) &vi) == 0) throw SystemException("Cannot get OS version information");
	switch (vi.dwMajorVersion)
	{
	case 10:
		switch (vi.dwMinorVersion)
		{
		case 0:
			if (vi.dwBuildNumber >= 22000)
				return "Windows 11";
			else if (vi.dwBuildNumber >= 20348 && vi.wProductType != VER_NT_WORKSTATION)
				return "Windows Server 2022";
			else if (vi.dwBuildNumber >= 17763 && vi.wProductType != VER_NT_WORKSTATION)
				return "Windows Server 2019";
			else
				return vi.wProductType == VER_NT_WORKSTATION ? "Windows 10" : "Windows Server 2016";
		}
	case 6:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows Vista" : "Windows Server 2008";
		case 1:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 7" : "Windows Server 2008 R2";
		case 2:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 8" : "Windows Server 2012";
		case 3:
			return vi.wProductType == VER_NT_WORKSTATION ? "Windows 8.1" : "Windows Server 2012 R2";
		default:
			return "Unknown";
		}
	case 5:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return "Windows 2000";
		case 1:
			return "Windows XP";
		case 2:
			return "Windows Server 2003/Windows Server 2003 R2";
		default:
			return "Unknown";
		}
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::osVersionImpl()
{
	OSVERSIONINFOW vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if (GetVersionExW(&vi) == 0) throw SystemException("Cannot get OS version information");
	std::ostringstream str;
	str << vi.dwMajorVersion << "." << vi.dwMinorVersion << " (Build " << (vi.dwBuildNumber & 0xFFFF);
	std::string version;
	UnicodeConverter::toUTF8(vi.szCSDVersion, version);
	if (!version.empty()) str << ": " << version;
	str << ")";
	return str.str();
}


std::string EnvironmentImpl::osArchitectureImpl()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return "IA32";
	case PROCESSOR_ARCHITECTURE_MIPS:
		return "MIPS";
	case PROCESSOR_ARCHITECTURE_ALPHA:
		return "ALPHA";
	case PROCESSOR_ARCHITECTURE_PPC:
		return "PPC";
	case PROCESSOR_ARCHITECTURE_IA64:
		return "IA64";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64
	case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
		return "IA64/32";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64
	case PROCESSOR_ARCHITECTURE_AMD64:
		return "AMD64";
#endif
	default:
		return "Unknown";
	}
}


std::string EnvironmentImpl::nodeNameImpl()
{
	wchar_t name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerNameW(name, &size) == 0) throw SystemException("Cannot get computer name");
	std::string result;
	UnicodeConverter::toUTF8(name, result);
	return result;
}


void EnvironmentImpl::nodeIdImpl(NodeId& id)
{
	std::memset(&id, 0, sizeof(id));

	// Preallocate buffer for some adapters to avoid calling
	// GetAdaptersInfo multiple times.
	static constexpr int IP_ADAPTER_BUFFER_SIZE = 10;

	auto pAdapterInfo = std::make_unique<IP_ADAPTER_INFO[]>(IP_ADAPTER_BUFFER_SIZE);
	ULONG len    = IP_ADAPTER_BUFFER_SIZE * sizeof(IP_ADAPTER_INFO);

	const DWORD rc = GetAdaptersInfo(pAdapterInfo.get(), &len);

	if (rc == ERROR_BUFFER_OVERFLOW)
	{
		// Buffer is not large enough: reallocate and retry.

		// len is expected to be a multiple of IP_ADAPTER_INFO.
		// If it is not, then this file most probably uses different struct then
		// the system library.
		poco_assert_msg( (len % sizeof(IP_ADAPTER_INFO)) == 0, "Unexpected size of IP_ADAPTER_INFO.");

		pAdapterInfo = std::make_unique<IP_ADAPTER_INFO[]>(len / sizeof(IP_ADAPTER_INFO));
		if (GetAdaptersInfo(pAdapterInfo.get(), &len) != ERROR_SUCCESS)
		{
			throw SystemException("cannot get network adapter list");
		}
	}
	else if (rc != ERROR_SUCCESS)
	{
		throw SystemException("cannot get network adapter list");
	}

	IP_ADAPTER_INFO* pAdapter = pAdapterInfo.get();
	while (pAdapter)
	{
		if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->AddressLength == sizeof(id))
		{
			std::memcpy(&id, pAdapter->Address, pAdapter->AddressLength);

			// found an ethernet adapter, we can return now
			return;
		}
		pAdapter = pAdapter->Next;
	}

	// if an ethernet adapter was not found, search for a wifi adapter
	pAdapter = pAdapterInfo.get();
	while (pAdapter)
	{
		if (pAdapter->Type == IF_TYPE_IEEE80211 && pAdapter->AddressLength == sizeof(id))
		{
			std::memcpy(&id, pAdapter->Address, pAdapter->AddressLength);

			// found a wifi adapter, we can return now
			return;
		}
		pAdapter = pAdapter->Next;
	}

	// ethernet and wifi adapters not found, fail the search
	throw SystemException("no ethernet or wifi adapter found");
}


unsigned EnvironmentImpl::processorCountImpl()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}


} // namespace Poco
