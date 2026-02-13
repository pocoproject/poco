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


std::vector<ServerDescription> ReadPreference::selectServers(
	const TopologyDescription& topology,
	Poco::Int64 heartbeatFrequencyUs) const
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
					eligible = filterByMaxStaleness(eligible, primary, heartbeatFrequencyUs);
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
				eligible = filterByMaxStaleness(eligible, primary, heartbeatFrequencyUs);
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
				eligible = filterByMaxStaleness(eligible, primary, heartbeatFrequencyUs);
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
				eligible = filterByMaxStaleness(eligible, primary, heartbeatFrequencyUs);
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
	const ServerDescription& primary,
	Poco::Int64 heartbeatFrequencyUs) const
{
	if (_maxStalenessSeconds == NO_MAX_STALENESS)
	{
		return servers;  // No filtering needed
	}

	// MongoDB Server Selection Specification — maxStalenessSeconds
	// See: https://github.com/mongodb/specifications/blob/master/source/server-selection/server-selection.md#maxstalenessseconds
	//
	// When a primary is known:
	//   staleness = (S.lastUpdateTime - S.lastWriteDate) - (P.lastUpdateTime - P.lastWriteDate) + heartbeatFrequency
	//
	// When no primary is known:
	//   staleness = SMax.lastWriteDate - S.lastWriteDate + heartbeatFrequency
	//   where SMax is the secondary with the most recent lastWriteDate.

	std::vector<ServerDescription> result;
	result.reserve(servers.size());
	const Poco::Int64 maxStalenessUs = _maxStalenessSeconds * 1000000;  // Convert to microseconds

	if (primary.type() != ServerDescription::Unknown && primary.lastWriteDate() != 0)
	{
		// Primary is known — use the primary-based formula
		const Poco::Int64 primaryIdleUs =
			primary.lastUpdateTime().epochMicroseconds() - primary.lastWriteDate();

		for (const auto& server : servers)
		{
			if (server.lastWriteDate() == 0)
			{
				// Server didn't report lastWriteDate; include it to avoid
				// incorrectly filtering out servers that don't support the field.
				result.push_back(server);
				continue;
			}

			const Poco::Int64 serverIdleUs =
				server.lastUpdateTime().epochMicroseconds() - server.lastWriteDate();
			const Poco::Int64 stalenessUs = serverIdleUs - primaryIdleUs + heartbeatFrequencyUs;

			if (stalenessUs <= maxStalenessUs)
			{
				result.push_back(server);
			}
		}
	}
	else
	{
		// No primary known — use the secondary-max-based formula.
		// Find the secondary with the most recent lastWriteDate.
		Poco::Int64 maxWriteDateUs = 0;
		for (const auto& server : servers)
		{
			if (server.lastWriteDate() > maxWriteDateUs)
			{
				maxWriteDateUs = server.lastWriteDate();
			}
		}

		if (maxWriteDateUs == 0)
		{
			// No server reported lastWriteDate; include all servers.
			return servers;
		}

		for (const auto& server : servers)
		{
			if (server.lastWriteDate() == 0)
			{
				result.push_back(server);
				continue;
			}

			const Poco::Int64 stalenessUs = maxWriteDateUs - server.lastWriteDate() + heartbeatFrequencyUs;

			if (stalenessUs <= maxStalenessUs)
			{
				result.push_back(server);
			}
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

	// Select servers within the local threshold of minimum RTT (MongoDB spec)
	std::vector<ServerDescription> result;
	result.reserve(servers.size());

	for (const auto& server : servers)
	{
		if (server.roundTripTime() <= minRTT + DEFAULT_LOCAL_THRESHOLD_US)
		{
			result.emplace_back(server);
		}
	}

	return result;
}


} } // namespace Poco::MongoDB
