//
// MongoDB.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_MONGODB
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/BSONReader.h"
#include "Poco/MongoDB/BSONWriter.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/MongoDB/JavaScriptCode.h"
#include "Poco/MongoDB/Message.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/RegularExpression.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/ReplicaSetPoolableConnectionFactory.h"
#include "Poco/MongoDB/ReplicaSetURI.h"
#include "Poco/MongoDB/ServerDescription.h"
#include "Poco/MongoDB/TopologyChangeNotification.h"
#include "Poco/MongoDB/TopologyDescription.h"
#endif

export module Poco.MongoDB;

export namespace Poco::MongoDB {
	#ifdef ENABLE_MONGODB
	// Main classes
	using Poco::MongoDB::Array;
	using Poco::MongoDB::BSONReader;
	using Poco::MongoDB::BSONWriter;
	using Poco::MongoDB::Binary;
	using Poco::MongoDB::Connection;
	using Poco::MongoDB::Database;
	using Poco::MongoDB::Document;
	using Poco::MongoDB::JavaScriptCode;
	using Poco::MongoDB::ObjectId;
	using Poco::MongoDB::OpMsgCursor;
	using Poco::MongoDB::OpMsgMessage;
	using Poco::MongoDB::ReadPreference;
	using Poco::MongoDB::RegularExpression;
	using Poco::MongoDB::ReplicaSet;
	using Poco::MongoDB::ReplicaSetConnection;
	using Poco::MongoDB::ReplicaSetURI;
	using Poco::MongoDB::ServerDescription;
	using Poco::MongoDB::TopologyChangeNotification;
	using Poco::MongoDB::TopologyDescription;

	// Helper classes and structs
	using Poco::MongoDB::BSONTimestamp;
	using Poco::MongoDB::PooledConnection;
	using Poco::MongoDB::PooledReplicaSetConnection;

	// Type aliases
	using Poco::MongoDB::NullValue;

	#endif
}
