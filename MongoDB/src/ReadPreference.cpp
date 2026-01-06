//
// ReadPreference.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReadPreference
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Format.h"
#include <cmath>
#include <limits>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


const Poco::Int64 ReadPreference::NO_MAX_STALENESS;


ReadPreference::ReadPreference(Mode mode):
	_mode(mode)
{
}


ReadPreference::ReadPreference(Mode mode, const Document& tags, Poco::Int64 maxStalenessSeconds):
	_mode(mode),
	_tags(tags),
	_maxStalenessSeconds(maxStalenessSeconds)
{
}


ReadPreference::ReadPreference(const ReadPreference& other) = default;


ReadPreference::ReadPreference(ReadPreference&& other) noexcept = default;


ReadPreference::~ReadPreference() = default;


ReadPreference& ReadPreference::operator=(const ReadPreference& other) = default;


ReadPreference& ReadPreference::operator=(ReadPreference&& other) noexcept = default;


std::vector<ServerDescription> ReadPreference::selectServers(const TopologyDescription& topology) const
{
	std::vector<ServerDescription> eligible;

	switch (_mode)
	{
	case Primary:
		{
			ServerDescription primary = topology.findPrimary();
			if (primary.type() != ServerDescription::Unknown)
			{
				eligible.push_back(primary);
			}
		}
		break;

	case PrimaryPreferred:
		{
			ServerDescription primary = topology.findPrimary();
			if (primary.type() != ServerDescription::Unknown)
			{
				eligible.push_back(primary);
			}
			else
			{
				// Fallback to secondaries
				std::vector<ServerDescription> secondaries = topology.findSecondaries();
				eligible = filterByTags(secondaries);
				if (_maxStalenessSeconds != NO_MAX_STALENESS)
				{
					eligible = filterByMaxStaleness(eligible, primary);
				}
			}
		}
		break;

	case Secondary:
		{
			std::vector<ServerDescription> secondaries = topology.findSecondaries();
			eligible = filterByTags(secondaries);
			if (_maxStalenessSeconds != NO_MAX_STALENESS)
			{
				ServerDescription primary = topology.findPrimary();
				eligible = filterByMaxStaleness(eligible, primary);
			}
		}
		break;

	case SecondaryPreferred:
		{
			std::vector<ServerDescription> secondaries = topology.findSecondaries();
			eligible = filterByTags(secondaries);
			if (_maxStalenessSeconds != NO_MAX_STALENESS)
			{
				ServerDescription primary = topology.findPrimary();
				eligible = filterByMaxStaleness(eligible, primary);
			}

			if (eligible.empty())
			{
				// Fallback to primary
				ServerDescription primary = topology.findPrimary();
				if (primary.type() != ServerDescription::Unknown)
				{
					eligible.push_back(primary);
				}
			}
		}
		break;

	case Nearest:
		{
			// Combine primary and secondaries
			std::vector<ServerDescription> all = topology.servers();
			std::vector<ServerDescription> candidates;

			for (const auto& server : all)
			{
				if (server.isPrimary() || server.isSecondary())
				{
					candidates.push_back(server);
				}
			}

			eligible = filterByTags(candidates);
			if (_maxStalenessSeconds != NO_MAX_STALENESS)
			{
				ServerDescription primary = topology.findPrimary();
				eligible = filterByMaxStaleness(eligible, primary);
			}

			// Select by nearest (lowest RTT)
			eligible = selectByNearest(eligible);
		}
		break;
	}

	return eligible;
}


std::string ReadPreference::toString() const
{
	std::string result;
	switch (_mode)
	{
	case Primary:
		result = "primary"s;
		break;
	case PrimaryPreferred:
		result = "primaryPreferred"s;
		break;
	case Secondary:
		result = "secondary"s;
		break;
	case SecondaryPreferred:
		result = "secondaryPreferred"s;
		break;
	case Nearest:
		result = "nearest"s;
		break;
	}

	if (!_tags.empty())
	{
		result += " (tags: "s + _tags.toString() + ')';
	}

	if (_maxStalenessSeconds != NO_MAX_STALENESS)
	{
		result += Poco::format(" (maxStaleness: %?d seconds)"s, _maxStalenessSeconds);
	}

	return result;
}


bool ReadPreference::matchesTags(const ServerDescription& server) const
{
	if (_tags.empty())
	{
		return true;  // No tag constraints
	}

	const Document& serverTags = server.tags();

	// Get tag names and check if all required tags match
	std::vector<std::string> tagNames;
	_tags.elementNames(tagNames);

	for (const auto& key : tagNames)
	{
		if (!serverTags.exists(key))
		{
			return false;
		}

		// Get both values as strings for comparison
		const auto& requiredValue = _tags.get<std::string>(key);
		const auto& serverValue = serverTags.get<std::string>(key);

		if (requiredValue != serverValue)
		{
			return false;
		}
	}

	return true;
}


std::vector<ServerDescription> ReadPreference::filterByTags(const std::vector<ServerDescription>& servers) const
{
	if (_tags.empty())
	{
		return servers;  // No filtering needed
	}

	std::vector<ServerDescription> result;
	result.reserve(servers.size());
	for (const auto& server : servers)
	{
		if (matchesTags(server))
		{
			result.push_back(server);
		}
	}
	return result;
}


std::vector<ServerDescription> ReadPreference::filterByMaxStaleness(
	const std::vector<ServerDescription>& servers,
	const ServerDescription& primary) const
{
	if (_maxStalenessSeconds == NO_MAX_STALENESS)
	{
		return servers;  // No filtering needed
	}

	// Note: Proper staleness calculation requires lastWriteDate from server responses
	// For now, we implement a simplified version based on lastUpdateTime
	// A full implementation would compare lastWriteDate timestamps

	std::vector<ServerDescription> result;
	result.reserve(servers.size());
	const Poco::Int64 maxStalenessMs = _maxStalenessSeconds * 1000000;  // Convert to microseconds

	for (const auto& server : servers)
	{
		// Calculate staleness as the difference between primary and secondary update times
		// This is a simplified approximation
		if (primary.type() != ServerDescription::Unknown)
		{
			Poco::Int64 staleness = std::abs(
				primary.lastUpdateTime().epochMicroseconds() -
				server.lastUpdateTime().epochMicroseconds()
			);

			if (staleness <= maxStalenessMs)
			{
				result.push_back(server);
			}
		}
		else
		{
			// No primary available, include all secondaries
			result.push_back(server);
		}
	}

	return result;
}


std::vector<ServerDescription> ReadPreference::selectByNearest(const std::vector<ServerDescription>& servers) const
{
	if (servers.empty())
	{
		return servers;
	}

	// Find minimum RTT
	Poco::Int64 minRTT = std::numeric_limits<Poco::Int64>::max();
	for (const auto& server : servers)
	{
		if (server.roundTripTime() < minRTT)
		{
			minRTT = server.roundTripTime();
		}
	}

	// Select servers within 15ms of minimum RTT (MongoDB spec)
	const Poco::Int64 localThresholdMs = 15000;  // 15ms in microseconds
	std::vector<ServerDescription> result;
	result.reserve(servers.size());

	for (const auto& server : servers)
	{
		if (server.roundTripTime() <= minRTT + localThresholdMs)
		{
			result.emplace_back(server);
		}
	}

	return result;
}


} } // namespace Poco::MongoDB
