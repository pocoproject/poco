//
// Deserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Deserializer
//
// Definition of the Deserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_Deserializer_INCLUDED
#define RemotingNG_REST_Deserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/JSONDeserializer.h"
#include "Poco/RemotingNG/REST/FormDeserializer.h"
#include "Poco/RemotingNG/REST/HeaderDeserializer.h"
#include "Poco/RemotingNG/REST/PathDeserializer.h"
#include "Poco/RemotingNG/REST/RawDeserializer.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API Deserializer: public Poco::RemotingNG::Deserializer
	/// The Deserializer implementation for the REST Transport.
	///
	/// For more information, please see the Serializer documentation.
{
public:
	Deserializer();
		/// Creates the Deserializer.

	~Deserializer();
		/// Destroys the Deserializer.

	void setRequest(Poco::Net::HTTPRequest& request);
		/// Sets the HTTP request object.

	void setResponse(Poco::Net::HTTPResponse& response);
		/// Sets the HTTP response object.

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
	Poco::SharedPtr<Poco::RemotingNG::Deserializer> subDeserializer();

private:
	Deserializer(const Deserializer&);
	Deserializer& operator = (const Deserializer&);

private:
	std::istream* _pStream;
	Poco::Net::HTTPRequest* _pRequest;
	Poco::Net::HTTPResponse* _pResponse;
	Poco::SharedPtr<PathDeserializer> _pPathDeserializer;
	Poco::SharedPtr<FormDeserializer> _pQueryDeserializer;
	Poco::SharedPtr<HeaderDeserializer> _pHeaderDeserializer;
	Poco::SharedPtr<JSONDeserializer> _pJSONDeserializer;
	Poco::SharedPtr<RawDeserializer> _pRawDeserializer;
	Poco::SharedPtr<FormDeserializer> _pFormDeserializer;
};


//
// inlines
//
inline void Deserializer::setRequest(Poco::Net::HTTPRequest& request)
{
	_pRequest = &request;
}


inline void Deserializer::setResponse(Poco::Net::HTTPResponse& response)
{
	_pResponse = &response;
}


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_Deserializer_INCLUDED
