//
// Deserializer.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Deserializer
//
// Definition of the Deserializer class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_Deserializer_INCLUDED
#define RemotingNG_JSONRPC_Deserializer_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include "Poco/RemotingNG/Deserializer.h"
#include <vector>


namespace Poco::RemotingNG::JSONRPC {


class RemotingNGJSONRPC_API Deserializer: public Poco::RemotingNG::Deserializer
	/// The Deserializer implementation for the JSONRPC Transport.
	///
	/// For more information, please see the Serializer documentation.
{
public:
	Deserializer();
		/// Creates the Deserializer.

	~Deserializer() override;
		/// Destroys the Deserializer.

	Deserializer(const Deserializer&) = delete;
	Deserializer& operator=(const Deserializer&) = delete;

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
	[[nodiscard]] int id() const;
	void associateSerializer(Serializer& ser);

protected:
	void resetImpl() override;
	void setupImpl(std::istream& inStream) override;

private:
	[[nodiscard]] JSValue::Ptr findValue(const std::string& name, bool isMandatory, bool& found);

private:
	std::vector<JSValue::Ptr> _objectStack;
	std::vector<std::size_t> _indexStack;
	int _id;
	Serializer* _pAssociatedSerializer = nullptr;
};


//
// inlines
//
inline int Deserializer::id() const
{
	return _id;
}


} // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_Deserializer_INCLUDED
