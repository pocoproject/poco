//
// JSONDeserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  JSONDeserializer
//
// Definition of the JSONDeserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_JSONDeserializer_INCLUDED
#define RemotingNG_REST_JSONDeserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/REST/JSValue.h"
#include "Poco/RemotingNG/Deserializer.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API JSONDeserializer: public Poco::RemotingNG::Deserializer
	/// The Deserializer implementation for the REST Transport.
	///
	/// For more information, please see the Serializer documentation.
{
public:
	JSONDeserializer(std::istream& stream);
		/// Creates the JSONDeserializer.

	~JSONDeserializer();
		/// Destroys the JSONDeserializer.

	void deserializeError(const std::string& reason, int status);

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
	bool deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified);
	void deserializeOptionalEnd(const std::string& name);
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
	void associateSerializer(Serializer& ser);

protected:
	void resetImpl();
	void setupImpl(std::istream& inStream);

private:
	JSONDeserializer(const JSONDeserializer&);
	JSONDeserializer& operator = (const JSONDeserializer&);

	JSValue::Ptr findValue(const std::string& name, bool isMandatory, bool& found);

private:
	std::vector<JSValue::Ptr> _objectStack;
	std::vector<std::size_t> _indexStack;
	int _objectDepth;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_JSONDeserializer_INCLUDED
