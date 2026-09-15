//
// JSONSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  JSONSerializer
//
// Definition of the JSONSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_JSONSerializer_INCLUDED
#define RemotingNG_REST_JSONSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API JSONSerializer: public Poco::RemotingNG::Serializer
	/// The JSON Serializer implementation for the REST
	/// Transport.
{
public:
	static const std::string JSON_ERROR;
	static const std::string JSON_CODE;
	static const std::string JSON_DETAIL;
	static const std::string JSON_TRUE;
	static const std::string JSON_FALSE;
	static const std::string JSON_NULL;

	enum State
	{
		ST_OBJECT,
		ST_OBJECT_FIRST,
		ST_ARRAY,
		ST_ARRAY_FIRST
	};

	JSONSerializer(std::ostream& stream);
		/// Creates the JSONSerializer.

	~JSONSerializer();
		/// Destroys the JSONSerializer.

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
	void serializeOptionalBegin(const std::string& name, bool isSpecified);
	void serializeOptionalEnd(const std::string& name);
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

protected:
	void resetImpl();
	void setupImpl(std::ostream& stream);

private:
	void serializeData(const std::string& name, const std::string& val);
	void writeSeparator();
	std::string escapeString(const std::string& val);

private:
	std::ostream* _pStream;
	int _indent;
	std::vector<int> _state;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_JSONSerializer_INCLUDED
