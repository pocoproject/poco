//
// EventStreamFuzz.cpp
//
// Feeds arbitrary bytes to the server-sent event reader.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	std::istringstream in(std::string(reinterpret_cast<const char*>(data), size));
	// A limit well below the inputs' length, so that long lines and events
	// reach it.
	Poco::AI::MCP::HTTP::EventStreamReader reader(in, 256);
	Poco::AI::MCP::HTTP::EventStreamReader::Event event;
	try
	{
		while (reader.next(event))
		{
		}
	}
	catch (const Poco::DataFormatException&)
	{
		// What next() throws for a line or an event over the limit; anything
		// else is a finding.
	}
	return 0;
}
