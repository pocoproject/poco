//
// ReadPreference.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReadPreference
//
// Definition of the ReadPreference class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ReadPreference_INCLUDED
#define MongoDB_ReadPreference_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/ServerDescription.h"
#include <vector>


namespace Poco {
namespace MongoDB {


class TopologyDescription;  // Forward declaration


class MongoDB_API ReadPreference
	/// Configures read preference mode and constraints for MongoDB operations.
	///
	/// Read preferences determine which replica set members receive read
	/// operations. The five read preference modes are:
	///
	/// - Primary: Read from the primary only (default, strongest consistency)
	/// - PrimaryPreferred: Read from primary, fallback to secondary
	/// - Secondary: Read from secondary only (distributes load)
	/// - SecondaryPreferred: Read from secondary, fallback to primary
	/// - Nearest: Read from any available member (primary or secondary)
	///
	/// Additional constraints:
	/// - Tag sets: Target specific replica set members by tags
	/// - Max staleness: Limit how stale secondary data can be
	///
	/// LIMITATIONS:
	/// The Nearest mode does NOT measure actual network round-trip time (RTT).
	/// It simply allows selection of any available member. True latency-based
	/// server selection is not implemented.
	///
	/// Examples:
	///   ReadPreference primary(ReadPreference::Primary);
	///   ReadPreference secondary(ReadPreference::Secondary);
	///   ReadPreference nearest(ReadPreference::Nearest);
	///
	///   // With tag set (e.g., datacenter-aware routing)
	///   Document tags;
	///   tags.add("dc", "east");
	///   tags.add("rack", "1");
	///   ReadPreference geoPreference(ReadPreference::Nearest, tags);
	///
	/// THREAD SAFETY:
	/// This class is immutable after construction and therefore thread-safe.
{
public:
	enum Mode
		/// Read preference mode enumeration
	{
		Primary,              /// Read from primary only
		PrimaryPreferred,     /// Read from primary, fallback to secondary
		Secondary,            /// Read from secondary only
		SecondaryPreferred,   /// Read from secondary, fallback to primary
		Nearest               /// Read from any available member (no RTT measurement)
	};

	static const Poco::Int64 NO_MAX_STALENESS = -1;
		/// Constant indicating no max staleness constraint

	explicit ReadPreference(Mode mode = Primary);
		/// Creates a ReadPreference with the specified mode.

	ReadPreference(Mode mode, const Document& tags, Poco::Int64 maxStalenessSeconds = NO_MAX_STALENESS);
		/// Creates a ReadPreference with mode, tag set, and optional max staleness.
		/// maxStalenessSeconds: maximum replication lag in seconds (-1 for no limit)

	ReadPreference(const ReadPreference& other);
		/// Copy constructor.

	ReadPreference(ReadPreference&& other) noexcept;
		/// Move constructor.

	~ReadPreference();
		/// Destroys the ReadPreference.

	ReadPreference& operator=(const ReadPreference& other);
		/// Assignment operator.

	ReadPreference& operator=(ReadPreference&& other) noexcept;
		/// Move assignment operator.

	[[nodiscard]] Mode mode() const;
		/// Returns the read preference mode.

	[[nodiscard]] const Document& tags() const;
		/// Returns the tag set for server selection.

	[[nodiscard]] Poco::Int64 maxStalenessSeconds() const;
		/// Returns the max staleness in seconds, or NO_MAX_STALENESS if not set.

	[[nodiscard]] std::vector<ServerDescription> selectServers(const TopologyDescription& topology) const;
		/// Selects eligible servers from the topology based on this read preference.
		/// Returns a vector of eligible servers.
		/// If no servers match, returns an empty vector.

	[[nodiscard]] std::string toString() const;
		/// Returns a string representation of the read preference.

	static ReadPreference primary();
		/// Factory method for Primary read preference.

	static ReadPreference primaryPreferred();
		/// Factory method for PrimaryPreferred read preference.

	static ReadPreference secondary();
		/// Factory method for Secondary read preference.

	static ReadPreference secondaryPreferred();
		/// Factory method for SecondaryPreferred read preference.

	static ReadPreference nearest();
		/// Factory method for Nearest read preference.

private:
	bool matchesTags(const ServerDescription& server) const;
	std::vector<ServerDescription> filterByTags(const std::vector<ServerDescription>& servers) const;
	std::vector<ServerDescription> filterByMaxStaleness(const std::vector<ServerDescription>& servers, const ServerDescription& primary) const;
	std::vector<ServerDescription> selectByNearest(const std::vector<ServerDescription>& servers) const;

	Mode _mode{Primary};
	Document _tags;
	Poco::Int64 _maxStalenessSeconds{NO_MAX_STALENESS};
};


//
// inlines
//


inline ReadPreference::Mode ReadPreference::mode() const
{
	return _mode;
}


inline const Document& ReadPreference::tags() const
{
	return _tags;
}


inline Poco::Int64 ReadPreference::maxStalenessSeconds() const
{
	return _maxStalenessSeconds;
}


inline ReadPreference ReadPreference::primary()
{
	return ReadPreference(Primary);
}


inline ReadPreference ReadPreference::primaryPreferred()
{
	return ReadPreference(PrimaryPreferred);
}


inline ReadPreference ReadPreference::secondary()
{
	return ReadPreference(Secondary);
}


inline ReadPreference ReadPreference::secondaryPreferred()
{
	return ReadPreference(SecondaryPreferred);
}


inline ReadPreference ReadPreference::nearest()
{
	return ReadPreference(Nearest);
}


} } // namespace Poco::MongoDB


#endif // MongoDB_ReadPreference_INCLUDED
