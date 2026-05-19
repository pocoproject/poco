//
// Database.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Database
//
// Definition of the Database class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Database_INCLUDED
#define MongoDB_Database_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/OpMsgCursor.h"


namespace Poco::MongoDB {


class MongoDB_API Database
	/// Database is a helper class for creating requests. MongoDB works with
	/// collection names and uses the part before the first dot as the name of
	/// the database.
{
public:

	enum IndexOptions {
		INDEX_UNIQUE        = 1 << 0,
		INDEX_SPARSE        = 1 << 1,
			/// Not deprecated, but for new code partialFilterExpression
			/// (see the createIndex() overload taking extraOptions) is preferred
			/// over INDEX_SPARSE for indexes with conditional inclusion semantics.
			/// Partial indexes are available since MongoDB 3.2.
		INDEX_BACKGROUND POCO_DEPRECATED("Deprecated since MongoDB 4.2; index builds are online by default") = 1 << 2,
			/// Background index builds were deprecated in MongoDB 4.2. The option
			/// is a server-side no-op on 4.2+ deployments since all index builds
			/// became hybrid (online) by default.
		INDEX_HIDDEN        = 1 << 3
			/// Marks the index hidden from the query planner. Available since
			/// MongoDB 4.4. The index is still maintained on writes and can be
			/// unhidden without rebuilding.
	};

	using FieldIndex = std::tuple<std::string, bool>;
		/// name of the field to index, ascending order (true), descending order (false)

	using IndexedFields = std::vector<FieldIndex>;
		/// Vector of fields to create index on

	explicit Database(const std::string& name);
		/// Creates a Database for the database with the given name.

	virtual ~Database();
		/// Destroys the Database.

	[[nodiscard]] const std::string& name() const;
		/// Database name

	bool authenticate(Connection& connection, const std::string& username, const std::string& password, const std::string& method = AUTH_SCRAM_SHA256);
		/// Authenticates against the database using the given connection,
		/// username and password, as well as authentication method.
		///
		/// Supported methods:
		///   - "SCRAM-SHA-256" (default, MongoDB 4.0+ default for new users)
		///   - "SCRAM-SHA-1"   (MongoDB 3.0+; pass AUTH_SCRAM_SHA1 explicitly)
		///
		/// "MONGODB-CR" is no longer supported as it requires the legacy wire
		/// protocol.
		///
		/// For SCRAM-SHA-256 the password is used directly after SASLprep
		/// (RFC 4013). Currently only ASCII passwords are accepted on the
		/// SCRAM-SHA-256 path; passing a password containing non-ASCII bytes
		/// throws Poco::NotImplementedException. Use SCRAM-SHA-1 for non-ASCII
		/// passwords until full SASLprep support is implemented.
		///
		/// Returns true if authentication was successful, otherwise false.
		///
		/// May throw a Poco::ProtocolException if authentication fails for a
		/// reason other than invalid credentials.

	[[nodiscard]] Document::Ptr queryBuildInfo(Connection& connection) const;
		/// Queries server build info using OP_MSG protocol.

	[[nodiscard]] Document::Ptr queryServerHello(Connection& connection) const;
		/// Queries hello response from server using OP_MSG protocol.

	[[nodiscard]] Int64 count(Connection& connection, const std::string& collectionName) const;
		/// Counts documents in the given collection using the aggregation
		/// framework ([{$count: "n"}]) over OP_MSG. Aggregation-based counting
		/// is preferred over the legacy "count" command because it is part of
		/// the Stable API v1 (since MongoDB 5.0), is accurate on sharded
		/// clusters (the legacy command can over-report due to orphaned
		/// documents), and is permitted in multi-document transactions.
		///
		/// If the command fails, -1 is returned.

	[[nodiscard]] SharedPtr<OpMsgMessage> createOpMsgMessage(const std::string& collectionName) const;
		/// Creates OpMsgMessage for the given collection.

	[[nodiscard]] SharedPtr<OpMsgMessage> createOpMsgMessage() const;
		/// Creates OpMsgMessage for database commands that do not require collection as an argument.

	[[nodiscard]] SharedPtr<OpMsgCursor> createOpMsgCursor(const std::string& collectionName) const;
		/// Creates OpMsgCursor for the given collection.

	Document::Ptr createIndex(
		Connection& connection,
		const std::string& collection,
		const IndexedFields& indexedFields,
		const std::string &indexName,
		unsigned long options = 0,
		int expirationSeconds = 0,
		int version = 0);
		/// Creates an index. The document returned is the response body.
		/// For more info look at the createIndex information on the MongoDB
		/// website. (new wire protocol)
		///
		/// Leave version at 0 to use the server default (v=2 since MongoDB 3.4).
		/// Setting v=1 is only for compatibility with pre-3.4 servers and is
		/// incompatible with text, wildcard, and several other modern index
		/// types.

	Document::Ptr createIndex(
		Connection& connection,
		const std::string& collection,
		const IndexedFields& indexedFields,
		const std::string& indexName,
		Document::Ptr extraOptions,
		unsigned long options = 0,
		int expirationSeconds = 0,
		int version = 0);
		/// Creates an index, allowing arbitrary additional fields in the
		/// per-index spec via extraOptions. Every element of extraOptions
		/// is added to the index spec document on top of the fields derived
		/// from indexedFields, indexName, options, expirationSeconds and
		/// version. Typical keys to pass in extraOptions include:
		///
		///   - "partialFilterExpression" (Document): partial indexes,
		///     MongoDB 3.2+. Preferred over INDEX_SPARSE for new code.
		///   - "collation" (Document): per-index collation, MongoDB 3.4+.
		///   - "wildcardProjection" (Document): wildcard / compound-wildcard
		///     indexes (the wildcard field itself is specified as "$**" or
		///     "path.$**" in indexedFields).
		///   - "weights", "default_language", "language_override" (text indexes).
		///   - "2dsphereIndexVersion", "bits", "min", "max" (geo indexes).
		///
		/// The document returned is the createIndexes response body.

	static const std::string AUTH_SCRAM_SHA1;
		/// SCRAM-SHA-1 authentication mechanism (MongoDB 3.0 and later).

	static const std::string AUTH_SCRAM_SHA256;
		/// SCRAM-SHA-256 authentication mechanism. MongoDB 4.0 and later;
		/// also the default mechanism for users created in 4.0+ without an
		/// explicit mechanism specification. Default for Database::authenticate.

	enum WireVersion
		/// Wire version as reported by the command hello.
		/// See details in MongoDB github, repository specifications.
		/// @see queryServerHello
	{
		VER_26		= 1,
		VER_26_2	= 2,
		VER_30		= 3,
		VER_32		= 4,
		VER_34		= 5,
		VER_36		= 6, ///< First wire version that supports OP_MSG
		VER_40		= 7,
		VER_42		= 8,
		VER_44		= 9,
		VER_50		= 13,
		VER_51		= 14, ///< First wire version that supports *only* OP_MSG
		VER_52		= 15,
		VER_53		= 16,
		VER_60		= 17,
		VER_61		= 18,
		VER_62		= 19,
		VER_70		= 21,
		VER_71		= 22,
		VER_72		= 23,
		VER_73		= 24,
		VER_80		= 25
	};

protected:
	bool authSCRAM(Connection& connection, const std::string& username, const std::string& password);
		/// Performs SCRAM-SHA-1 authentication.

	bool authSCRAM256(Connection& connection, const std::string& username, const std::string& password);
		/// Performs SCRAM-SHA-256 authentication. ASCII-only password fast path.

private:
	std::string _dbname;
};


//
// inlines
//
inline const std::string& Database::name() const
{
	return _dbname;
}


inline SharedPtr<OpMsgMessage>
Database::createOpMsgMessage(const std::string& collectionName) const
{
	return new OpMsgMessage(_dbname, collectionName);
}


inline SharedPtr<OpMsgMessage>
Database::createOpMsgMessage() const
{
	// Collection name for database commands is not needed.
	return createOpMsgMessage("");
}


inline SharedPtr<OpMsgCursor>
Database::createOpMsgCursor(const std::string& collectionName) const
{
	return new OpMsgCursor(_dbname, collectionName);
}


} // namespace Poco::MongoDB


#endif // MongoDB_Database_INCLUDED
