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


namespace Poco::RemotingNG::REST {


class RemotingNGREST_API RawDeserializer: public Poco::RemotingNG::Deserializer
	/// Deserialization of text-based or binary scalar values from a request body.
{
public:
	RawDeserializer(std::istream& stream);
		/// Creates the RawDeserializer using the given input stream.

	~RawDeserializer() override;
		/// Destroys the RawDeserializer.

	// Poco::RemotingNG::Deserializer
	Poco::RemotingNG::SerializerBase::MessageType findMessage(std::string& name) override;
	void deserializeMessageBegin(const std::string& name, SerializerBase::MessageType type) override;
	void deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type) override;
	bool deserializeStructBegin(const std::string& name, bool isMandatory) override;
	void deserializeStructEnd(const std::string& name) override;
	bool deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& lengthHint) override;
	void deserializeSequenceEnd(const std::string& name) override;
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull) override;
	void deserializeNullableEnd(const std::string& name) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int8& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int16& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int32& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val) override;
	bool deserialize(const std::string& name, bool isMandatory, long& val) override;
	bool deserialize(const std::string& name, bool isMandatory, unsigned long& val) override;
#ifndef POCO_INT64_IS_LONG
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int64& val) override;
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val) override;
#endif
	bool deserialize(const std::string& name, bool isMandatory, float& val) override;
	bool deserialize(const std::string& name, bool isMandatory, double& val) override;
	bool deserialize(const std::string& name, bool isMandatory, bool& val) override;
	bool deserialize(const std::string& name, bool isMandatory, char& val) override;
	bool deserialize(const std::string& name, bool isMandatory, std::string& val) override;
	bool deserialize(const std::string& name, bool isMandatory, std::vector<char>& val) override;

protected:
	void resetImpl() override;
	void setupImpl(std::istream& inStream) override;
	[[nodiscard]] const std::string& deserializeData(const std::string& name, bool isMandatory, bool& found);

private:
	RawDeserializer(const RawDeserializer&) = delete;
	RawDeserializer& operator = (const RawDeserializer&) = delete;

	std::istream* _pStream;
	std::string _data;
};


} // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RawDeserializer_INCLUDED
