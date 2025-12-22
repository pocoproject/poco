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


namespace Poco {
namespace MongoDB {


class MongoDB_API Database
	/// Database is a helper class for creating requests. MongoDB works with
	/// collection names and uses the part before the first dot as the name of
	/// the database.
{
public:

	enum IndexOptions {
		INDEX_UNIQUE        = 1 << 0,
		INDEX_SPARSE        = 1 << 1,
		INDEX_BACKGROUND    = 1 << 2
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

	bool authenticate(Connection& connection, const std::string& username, const std::string& password, const std::string& method = AUTH_SCRAM_SHA1);
		/// Authenticates against the database using the given connection,
		/// username and password, as well as authentication method.
		///
		/// "SCRAM-SHA-1" (default starting in MongoDB 3.0) is the only supported
		/// authentication method. "MONGODB-CR" is no longer supported as it
		/// requires the legacy wire protocol.
		///
		/// Returns true if authentication was successful, otherwise false.
		///
		/// May throw a Poco::ProtocolException if authentication fails for a reason other than
		/// invalid credentials.

	[[nodiscard]] Document::Ptr queryBuildInfo(Connection& connection) const;
		/// Queries server build info using OP_MSG protocol.

	[[nodiscard]] Document::Ptr queryServerHello(Connection& connection) const;
		/// Queries hello response from server using OP_MSG protocol.

	[[nodiscard]] Int64 count(Connection& connection, const std::string& collectionName) const;
		/// Sends a count request for the given collection to MongoDB using OP_MSG protocol.
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
		/// Creates an index. The document returned is the response body..
		/// For more info look at the createIndex information on the MongoDB website. (new wire protocol)

	static const std::string AUTH_SCRAM_SHA1;
		/// Default authentication mechanism for MongoDB 3.0 and later.

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
		VER_60		= 17
	};

protected:
	bool authSCRAM(Connection& connection, const std::string& username, const std::string& password);

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


} } // namespace Poco::MongoDB


#endif // MongoDB_Database_INCLUDED
