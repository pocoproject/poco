//
// RawDeserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RawDeserializer
//
// Definition of the RawDeserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_RawDeserializer_INCLUDED
#define RemotingNG_REST_RawDeserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Deserializer.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API RawDeserializer: public Poco::RemotingNG::Deserializer
	/// Deserialization of text-based or binary scalar values from a request body.
{
public:
	RawDeserializer(std::istream& stream);
		/// Creates the RawDeserializer using the given input stream.

	~RawDeserializer();
		/// Destroys the RawDeserializer.

	// Poco::RemotingNG::Deserializer
	Poco::RemotingNG::SerializerBase::MessageType findMessage(std::string& name);
	void deserializeMessageBegin(const std::string& name, SerializerBase::MessageType type);
	void deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type);
	bool deserializeStructBegin(const std::string& name, bool isMandatory);
	void deserializeStructEnd(const std::string& name);
	bool deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& lengthHint);
	void deserializeSequenceEnd(const std::string& name);
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull);
	void deserializeNullableEnd(const std::string& name);
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int8& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int16& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int32& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val);
	bool deserialize(const std::string& name, bool isMandatory, long& val);
	bool deserialize(const std::string& name, bool isMandatory, unsigned long& val);
#ifndef POCO_INT64_IS_LONG
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int64& val);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val);
#endif
	bool deserialize(const std::string& name, bool isMandatory, float& val);
	bool deserialize(const std::string& name, bool isMandatory, double& val);
	bool deserialize(const std::string& name, bool isMandatory, bool& val);
	bool deserialize(const std::string& name, bool isMandatory, char& val);
	bool deserialize(const std::string& name, bool isMandatory, std::string& val);
	bool deserialize(const std::string& name, bool isMandatory, std::vector<char>& val);

protected:
	void resetImpl();
	void setupImpl(std::istream& inStream);
	const std::string& deserializeData(const std::string& name, bool isMandatory, bool& found);

private:
	RawDeserializer(const RawDeserializer&);
	RawDeserializer& operator = (const RawDeserializer&);

	std::istream* _pStream;
	std::string _data;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RawDeserializer_INCLUDED
