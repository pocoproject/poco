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
#include <mutex>
#include "Poco/UnWindows.h"

#include <winsock2.h>
#include <iphlpapi.h>


namespace Poco {


namespace {


OSVERSIONINFOEXW cachedVersionInfo{};
std::once_flag versionInfoFlag;
bool versionInfoInitialized = false;


void initVersionInfo()
{
	cachedVersionInfo.dwOSVersionInfoSize = sizeof(cachedVersionInfo);

	// RtlGetVersion is always available in ntdll.dll (loaded in every process)
	if (HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll"))
	{
		using RtlGetVersionFunc = LONG (WINAPI *)(OSVERSIONINFOEXW*);
		if (auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(
				GetProcAddress(hNtdll, "RtlGetVersion")))
		{
			if (pRtlGetVersion(&cachedVersionInfo) == 0) // STATUS_SUCCESS
			{
				versionInfoInitialized = true;
			}
		}
	}
}


/// Returns cached OS version information.
/// Uses RtlGetVersion from ntdll.dll which returns accurate version info
/// regardless of application manifest (unlike deprecated GetVersionEx).
/// The version info is cached since it doesn't change during runtime.
/// Throws SystemException if version info cannot be obtained.
const OSVERSIONINFOEXW& getVersionInfo()
{
	std::call_once(versionInfoFlag, initVersionInfo);
	if (!versionInfoInitialized)
		throw SystemException("Cannot get OS version information");
	return cachedVersionInfo;
}


} // namespace


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
	const auto& vi = getVersionInfo();

	// All modern Windows versions are NT-based
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return "Windows NT";

	return "Unknown";
}


std::string EnvironmentImpl::osDisplayNameImpl()
{
	const auto& vi = getVersionInfo();
	const bool isWorkstation = (vi.wProductType == VER_NT_WORKSTATION);

	switch (vi.dwMajorVersion)
	{
	case 10:
		switch (vi.dwMinorVersion)
		{
		case 0:
			if (isWorkstation)
			{
				// Windows 10 vs Windows 11 (client)
				return (vi.dwBuildNumber >= 22000) ? "Windows 11" : "Windows 10";
			}
			else
			{
				// Server versions based on build number
				if (vi.dwBuildNumber >= 26100)
					return "Windows Server 2025";
				else if (vi.dwBuildNumber >= 20348)
					return "Windows Server 2022";
				else if (vi.dwBuildNumber >= 17763)
					return "Windows Server 2019";
				else
					return "Windows Server 2016";
			}
		default:
			return isWorkstation ? "Windows 10" : "Windows Server";
		}
	case 6:
		switch (vi.dwMinorVersion)
		{
		case 0:
			return isWorkstation ? "Windows Vista" : "Windows Server 2008";
		case 1:
			return isWorkstation ? "Windows 7" : "Windows Server 2008 R2";
		case 2:
			return isWorkstation ? "Windows 8" : "Windows Server 2012";
		case 3:
			return isWorkstation ? "Windows 8.1" : "Windows Server 2012 R2";
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
	const auto& vi = getVersionInfo();
	std::ostringstream str;
	str << vi.dwMajorVersion << "." << vi.dwMinorVersion << " (Build " << vi.dwBuildNumber;
	std::string version;
	UnicodeConverter::toUTF8(vi.szCSDVersion, version);
	if (!version.empty()) str << ": " << version;
	str << ")";
	return str.str();
}


std::string EnvironmentImpl::osArchitectureImpl()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return "IA32"s;
	case PROCESSOR_ARCHITECTURE_MIPS:
		return "MIPS"s;
	case PROCESSOR_ARCHITECTURE_ALPHA:
		return "ALPHA"s;
	case PROCESSOR_ARCHITECTURE_PPC:
		return "PPC"s;
	case PROCESSOR_ARCHITECTURE_SHX:
		return "SHX"s;
	case PROCESSOR_ARCHITECTURE_ARM:
		return "ARM"s;
	case PROCESSOR_ARCHITECTURE_IA64:
		return "IA64";
	case PROCESSOR_ARCHITECTURE_ALPHA64:
		return "ALPHA64"s;
	case PROCESSOR_ARCHITECTURE_MSIL:
		return "MSIL"s;
	case PROCESSOR_ARCHITECTURE_AMD64:
		return "AMD64"s;
	case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
		return "IA64/32"s;
	case PROCESSOR_ARCHITECTURE_NEUTRAL:
		return "NEUTRAL"s;
	case PROCESSOR_ARCHITECTURE_ARM64:
		return "ARM64"s;
	case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:
		return "IA64/ARM"s;
	case PROCESSOR_ARCHITECTURE_IA32_ON_ARM64:
		return "ARM64/IA32"s;
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
	// GetAdaptersAddresses multiple times.
	static constexpr int STARTING_BUFFER_SIZE = 20000;

	auto buffer = std::make_unique<unsigned char[]>(STARTING_BUFFER_SIZE);
	ULONG len = STARTING_BUFFER_SIZE;

	// use GAA_FLAG_SKIP_DNS_SERVER because we're only interested in the physical addresses of the interfaces
	const DWORD rc = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_DNS_SERVER, nullptr, reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get()), &len);

	if (rc == ERROR_BUFFER_OVERFLOW)
	{
		// Buffer is not large enough: reallocate and retry.
		buffer = std::make_unique<unsigned char[]>(len);

		if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_DNS_SERVER, nullptr, reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get()), &len) != ERROR_SUCCESS)
		{
			throw SystemException("cannot get network adapter list");
		}
	}
	else if (rc != ERROR_SUCCESS)
	{
		throw SystemException("cannot get network adapter list");
	}

	IP_ADAPTER_ADDRESSES* pAdapter = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get());
	while (pAdapter)
	{
		if (pAdapter->IfType == IF_TYPE_ETHERNET_CSMACD && pAdapter->PhysicalAddressLength == sizeof(id))
		{
			std::memcpy(&id, pAdapter->PhysicalAddress, pAdapter->PhysicalAddressLength);

			// found an ethernet adapter, we can return now
			return;
		}
		pAdapter = pAdapter->Next;
	}

	// if an ethernet adapter was not found, search for a wifi adapter
	pAdapter = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get());
	while (pAdapter)
	{
		if (pAdapter->IfType == IF_TYPE_IEEE80211 && pAdapter->PhysicalAddressLength == sizeof(id))
		{
			std::memcpy(&id, pAdapter->PhysicalAddress, pAdapter->PhysicalAddressLength);

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
	GetNativeSystemInfo(&si);
	return si.dwNumberOfProcessors;
}


} // namespace Poco
