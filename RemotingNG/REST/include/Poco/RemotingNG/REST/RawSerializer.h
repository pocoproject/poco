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


namespace Poco::RemotingNG::REST {


class RemotingNGREST_API RawSerializer: public Poco::RemotingNG::Serializer
	/// Raw text-based or binary serialization of scalar values.
{
public:
	RawSerializer(std::ostream& stream);
		/// Creates the RawSerializer using the given stream.

	~RawSerializer() override;
		/// Destroys the RawSerializer.

	// Poco::RemotingNG::Serializer
	void serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type) override;
	void serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type) override;
	void serializeFaultMessage(const std::string& name, const Poco::Exception& exc) override;
	void serializeStructBegin(const std::string& name) override;
	void serializeStructEnd(const std::string& name) override;
	void serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems) override;
	void serializeSequenceEnd(const std::string& name) override;
	void serializeNullableBegin(const std::string& name, bool isNull) override;
	void serializeNullableEnd(const std::string& name) override;
	void serialize(const std::string& name, Poco::Int8 val) override;
	void serialize(const std::string& name, Poco::UInt8 val) override;
	void serialize(const std::string& name, Poco::Int16 val) override;
	void serialize(const std::string& name, Poco::UInt16 val) override;
	void serialize(const std::string& name, Poco::Int32 val) override;
	void serialize(const std::string& name, Poco::UInt32 val) override;
	void serialize(const std::string& name, long val) override;
	void serialize(const std::string& name, unsigned long val) override;
#ifndef POCO_INT64_IS_LONG
	void serialize(const std::string& name, Poco::Int64 val) override;
	void serialize(const std::string& name, Poco::UInt64 val) override;
#endif
	void serialize(const std::string& name, float val) override;
	void serialize(const std::string& name, double val) override;
	void serialize(const std::string& name, bool val) override;
	void serialize(const std::string& name, char val) override;
	void serialize(const std::string& name, const std::string& val) override;
	void serialize(const std::string& name, const std::vector<char>& val) override;
	void resetImpl() override;
	void setupImpl(std::ostream&) override;

protected:
	void serializeData(const std::string& name, const std::string& val);

private:
	std::ostream* _pStream;

	static const std::string VAL_TRUE;
	static const std::string VAL_FALSE;

	friend class RawDeserializer;
};


} // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RawSerializer_INCLUDED
