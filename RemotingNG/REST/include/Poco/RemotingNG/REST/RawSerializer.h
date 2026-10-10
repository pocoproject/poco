//
// RawSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RawSerializer
//
// Definition of the RawSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_RawSerializer_INCLUDED
#define RemotingNG_REST_RawSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API RawSerializer: public Poco::RemotingNG::Serializer
	/// Raw text-based or binary serialization of scalar values.
{
public:
	RawSerializer(std::ostream& stream);
		/// Creates the RawSerializer using the given stream.

	~RawSerializer();
		/// Destroys the RawSerializer.

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
	void resetImpl();
	void setupImpl(std::ostream&);

protected:
	void serializeData(const std::string& name, const std::string& val);

private:
	std::ostream* _pStream;

	static const std::string VAL_TRUE;
	static const std::string VAL_FALSE;

	friend class RawDeserializer;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RawSerializer_INCLUDED
