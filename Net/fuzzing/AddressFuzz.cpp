//
// AddressFuzz.cpp
//
// Feeds an address string to IPAddress::tryParse and, when the host and port
// are numeric, to the SocketAddress string constructor.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/IPAddress.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/NumberParser.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


using Poco::NumberParser;
using Poco::Net::IPAddress;
using Poco::Net::SocketAddress;


namespace
{

constexpr std::size_t MAX_INPUT_SIZE = 256;
constexpr unsigned MAX_PORT = 65535;

	bool splitHostPort(const std::string& hostAndPort, std::string& host, std::string& port)
		/// Splits hostAndPort as SocketAddress::init does: [host]:port, else at
		/// the first colon. Returns false when the form has no port.
	{
		if (hostAndPort.empty()) return false;

		std::string::const_iterator it = hostAndPort.begin();
		const std::string::const_iterator end = hostAndPort.end();
		host.clear();
		port.clear();
		if (*it == '[')
		{
			++it;
			while (it != end && *it != ']') host += *it++;
			if (it == end) return false;
			++it;
		}
		else
		{
			while (it != end && *it != ':') host += *it++;
		}
		if (it != end && *it == ':')
		{
			++it;
			while (it != end) port += *it++;
			return true;
		}
		return false;
	}


	void checkRoundTrip(const IPAddress& parsed)
		/// Aborts if toString() does not parse back to the same address. When
		/// the address has a scope, only the address bytes are compared.
	{
		IPAddress again;
		if (!IPAddress::tryParse(parsed.toString(), again))
		{
			std::fputs("IPAddress round-trip parse failed\n", stderr);
			std::abort();
		}
		if (parsed.scope() != 0)
		{
			if (parsed.length() != again.length()
				|| std::memcmp(parsed.addr(), again.addr(), parsed.length()) != 0)
			{
				std::fputs("IPAddress round-trip address bytes differ\n", stderr);
				std::abort();
			}
		}
		else if (again != parsed)
		{
			std::fputs("IPAddress round-trip differs\n", stderr);
			std::abort();
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	IPAddress parsed;
	if (IPAddress::tryParse(input, parsed))
	{
		(void) parsed.toString();
		(void) parsed.family();
		(void) parsed.isWildcard();
		(void) parsed.isBroadcast();
		(void) parsed.isLoopback();
		(void) parsed.isMulticast();
		(void) parsed.isUnicast();
		(void) parsed.isLinkLocal();
		(void) parsed.isSiteLocal();
		(void) parsed.isGlobalMC();
		(void) parsed.prefixLength();
		(void) parsed.scope();
		(void) parsed.length();
		(void) parsed.addr();
		checkRoundTrip(parsed);
	}

	std::string host;
	std::string port;
	if (splitHostPort(input, host, port))
	{
		IPAddress hostAddr;
		unsigned portNumber = 0;
		if (IPAddress::tryParse(host, hostAddr)
			&& NumberParser::tryParseUnsigned(port, portNumber)
			&& portNumber <= MAX_PORT)
		{
			SocketAddress sa(input);
			(void) sa.toString();
			(void) sa.host();
			(void) sa.port();
			(void) sa.family();
		}
	}

	return 0;
}
