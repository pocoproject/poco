//
// OpMsgCursor.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  OpMsgCursor
//
// Definition of the OpMsgCursor class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_OpMsgCursor_INCLUDED
#define MongoDB_OpMsgCursor_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/OpMsgMessage.h"


namespace Poco {
namespace MongoDB {


class Connection;
class ReplicaSetConnection;

class MongoDB_API OpMsgCursor: public Document
	/// OpMsgCursor is a helper class for querying multiple documents using OpMsgMessage.
	/// Once all of the data is read with the cursor (see isActive()) it can't be reused.
	///
	/// USAGE:
	/// Supports both Connection and ReplicaSetConnection. When using ReplicaSetConnection,
	/// cursor operations benefit from automatic retry and failover on retriable errors.
	///
	/// RESOURCE MANAGEMENT:
	/// When a cursor is no longer needed, you should call kill() to release server-side
	/// resources. If kill() is not called explicitly, the server will keep the cursor
	/// open until it times out.
	///
	/// THREAD SAFETY:
	/// This class is NOT thread-safe. A cursor must not be used concurrently from
	/// multiple threads. Each thread should have its own cursor instances.
	/// The next() method modifies internal state and is not safe for concurrent access.
{
public:
	OpMsgCursor(const std::string& dbname, const std::string& collectionName);
		/// Creates a OpMsgCursor for the given database and collection.

	virtual ~OpMsgCursor();
		/// Destroys the OpMsgCursor.

	void setEmptyFirstBatch(bool empty) noexcept;
		/// Empty first batch is used to get error response faster with little server processing

	[[nodiscard]] bool emptyFirstBatch() const noexcept;

	void setBatchSize(Int32 batchSize) noexcept;
		/// Set non-default batch size

	[[nodiscard]] Int32 batchSize() const noexcept;
		/// Current batch size (zero or negative number indicates default batch size)

	[[nodiscard]] Int64 cursorID() const noexcept;

	[[nodiscard]] bool isActive() const noexcept;
		/// Is there more data to acquire with this cursor?

	OpMsgMessage& next(Connection& connection);
		/// Tries to get the next documents. As long as response message has a
		/// cursor ID next can be called to retrieve the next bunch of documents.
		///
		/// The cursor must be killed (see kill()) when not all documents are needed.

	OpMsgMessage& next(ReplicaSetConnection& connection);
		/// Tries to get the next documents. As long as response message has a
		/// cursor ID next can be called to retrieve the next bunch of documents.
		///
		/// The cursor must be killed (see kill()) when not all documents are needed.
		///
		/// This overload provides automatic retry and failover for replica set deployments.

	OpMsgMessage& query();
		/// Returns the associated query.

	void kill(Connection& connection);
		/// Kills the cursor and resets its internal state.
		/// Call this method when you don't need all documents to release server resources.

	void kill(ReplicaSetConnection& connection);
		/// Kills the cursor and resets its internal state.
		/// Call this method when you don't need all documents to release server resources.
		///
		/// This overload provides automatic retry and failover for replica set deployments.

private:
	template<typename ConnType>
	OpMsgMessage& nextImpl(ConnType& connection);
		/// Template implementation for next() to avoid code duplication.

	template<typename ConnType>
	void killImpl(ConnType& connection);
		/// Template implementation for kill() to avoid code duplication.

	OpMsgMessage    _query;
	OpMsgMessage 	_response;

	bool			_emptyFirstBatch { false };
	Int32			_batchSize { -1 };
		/// Batch size used in the cursor. Zero or negative value means that default shall be used.

	Int64			_cursorID { 0 };
};


//
// inlines
//
inline OpMsgMessage& OpMsgCursor::query()
{
	return _query;
}

inline Int64 OpMsgCursor::cursorID() const noexcept
{
	return _cursorID;
}


} } // namespace Poco::MongoDB


#endif // MongoDB_OpMsgCursor_INCLUDED
