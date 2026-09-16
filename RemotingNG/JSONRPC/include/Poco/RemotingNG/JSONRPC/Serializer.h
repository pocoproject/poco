//
// Serializer.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Serializer
//
// Definition of the Serializer class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_Serializer_INCLUDED
#define RemotingNG_JSONRPC_Serializer_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API Serializer: public Poco::RemotingNG::Serializer
	/// The Serializer implementation for the JSONRPC
	/// Transport.
{
public:
	static const std::string JSON_JSONRPC;
	static const std::string JSON_VERSION;
	static const std::string JSON_METHOD;
	static const std::string JSON_PARAMS;
	static const std::string JSON_ID;
	static const std::string JSON_RESULT;
	static const std::string JSON_ERROR;
	static const std::string JSON_CODE;
	static const std::string JSON_MESSAGE;
	static const std::string JSON_TRUE;
	static const std::string JSON_FALSE;
	static const std::string JSON_NULL;

	enum State
	{
		ST_MESSAGE,
		ST_OBJECT,
		ST_OBJECT_FIRST,
		ST_ARRAY,
		ST_ARRAY_FIRST
	};

	Serializer();
		/// Creates the Serializer.

	~Serializer();
		/// Destroys the Serializer.

	// Poco::RemotingNG::Serializer
	void serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type);
	void serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type);
	void serializeFaultMessage(const std::string& name, const Poco::Exception& exc);
	void serializeStructBegin(const std::string& name);
	void serializeStructEnd(const std::string& name);
	void serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems);
	void serializeSequenceEnd(const std::string& name);
	void serializeNullableBegin(const std::string& name, bool isNull);
	void serializeNullableEnd(const std::string& name);
	void serialize(const std::string& name, Poco::Int8 val);
	void serialize(const std::string& name, Poco::UInt8 val);
	void serialize(const std::string& name, Poco::Int16 val);
	void serialize(const std::string& name, Poco::UInt16 val);
	void serialize(const std::string& name, Poco::Int32 val);
	void serialize(const std::string& name, Poco::UInt32 val);
	void serialize(const std::string& name, long val);
	void serialize(const std::string& name, unsigned long val);
#ifndef POCO_INT64_IS_LONG
	void serialize(const std::string& name, Poco::Int64 val);
	void serialize(const std::string& name, Poco::UInt64 val);
#endif
	void serialize(const std::string& name, float val);
	void serialize(const std::string& name, double val);
	void serialize(const std::string& name, bool val);
	void serialize(const std::string& name, char val);
	void serialize(const std::string& name, const std::string& val);
	void serialize(const std::string& name, const std::vector<char>& val);
	void resetId(int newId = 0);
	static std::string escapeString(const std::string& val);

protected:
	void resetImpl();
	void setupImpl(std::ostream& out);

private:
	Serializer(const Serializer&);
	Serializer& operator=(const Serializer&);

	void serializeMethodStart(const std::string& methodName);
		/// Indicates the begin of method serialization.

	void serializeMethodEnd(const std::string& methodName);
		/// Indicates the end of method serialization.

	void serializeData(const std::string& name, const std::string& val);
		/// Serializes string data.

	void writeSeparator();

private:
	std::ostream* _pStream;
	int _indent;
	int _id;
	std::vector<int> _state;
};


//
// inlines
//
inline void Serializer::serialize(const std::string& name, Poco::Int8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::UInt8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::Int16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::UInt16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::Int32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::UInt32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, unsigned long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


#ifndef POCO_INT64_IS_LONG
inline void Serializer::serialize(const std::string& name, Poco::Int64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, Poco::UInt64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}
#endif


inline void Serializer::serialize(const std::string& name, float val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, double val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


inline void Serializer::serialize(const std::string& name, bool val)
{
	serializeData(name, val? JSON_TRUE : JSON_FALSE);
}


inline void Serializer::resetId(int newId)
{
	_id = newId;
}


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_Serializer_INCLUDED
