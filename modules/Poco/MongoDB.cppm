module;

#ifdef ENABLE_MONGODB
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/BSONReader.h"
#include "Poco/MongoDB/BSONWriter.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/DeleteRequest.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/MongoDB/GetMoreRequest.h"
#include "Poco/MongoDB/InsertRequest.h"
#include "Poco/MongoDB/JavaScriptCode.h"
#include "Poco/MongoDB/KillCursorsRequest.h"
#include "Poco/MongoDB/Message.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/QueryRequest.h"
#include "Poco/MongoDB/RegularExpression.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/RequestMessage.h"
#include "Poco/MongoDB/ResponseMessage.h"
#include "Poco/MongoDB/UpdateRequest.h"
#endif

export module Poco.MongoDB;

export namespace Poco::MongoDB {
	#ifdef ENABLE_MONGODB
	using Poco::MongoDB::Array;
	using Poco::MongoDB::BSONReader;
	using Poco::MongoDB::BSONTimestamp;
	using Poco::MongoDB::BSONWriter;
	using Poco::MongoDB::Binary;
	using Poco::MongoDB::ConcreteElement;
	using Poco::MongoDB::Connection;
	using Poco::MongoDB::Cursor;
	using Poco::MongoDB::Database;
	using Poco::MongoDB::DeleteRequest;
	using Poco::MongoDB::Document;
	using Poco::MongoDB::Element;
	using Poco::MongoDB::ElementFindByName;
	using Poco::MongoDB::ElementTraits;
	using Poco::MongoDB::GetMoreRequest;
	using Poco::MongoDB::InsertRequest;
	using Poco::MongoDB::JavaScriptCode;
	using Poco::MongoDB::KillCursorsRequest;
	using Poco::MongoDB::Message;
	using Poco::MongoDB::MessageHeader;
	using Poco::MongoDB::ObjectId;
	using Poco::MongoDB::OpMsgCursor;
	using Poco::MongoDB::OpMsgMessage;
	using Poco::MongoDB::PooledConnection;
	using Poco::MongoDB::QueryRequest;
	using Poco::MongoDB::RegularExpression;
	using Poco::MongoDB::ReplicaSet;
	using Poco::MongoDB::RequestMessage;
	using Poco::MongoDB::ResponseMessage;
	using Poco::MongoDB::UpdateRequest;

	using Poco::MongoDB::ElementSet;
	using Poco::MongoDB::NullValue;
	#endif
}
