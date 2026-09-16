//
// Serializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Serializer
//
// Definition of the Serializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_Serializer_INCLUDED
#define RemotingNG_REST_Serializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/REST/JSONSerializer.h"
#include "Poco/RemotingNG/REST/FormSerializer.h"
#include "Poco/RemotingNG/REST/HeaderSerializer.h"
#include "Poco/RemotingNG/REST/PathSerializer.h"
#include "Poco/RemotingNG/REST/RawSerializer.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class Deserializer;


class RemotingNGREST_API Serializer: public Poco::RemotingNG::Serializer
	/// The Serializer implementation for the REST
	/// Transport.
{
public:
	Serializer();
		/// Creates the Serializer.

	~Serializer();
		/// Destroys the Serializer.

	void setRequest(Poco::Net::HTTPRequest& request);
		/// Sets the HTTP request object.

	void setResponse(Poco::Net::HTTPResponse& response);
		/// Sets the HTTP response object.

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
	void setupImpl(std::ostream& out);
	Poco::SharedPtr<Poco::RemotingNG::Serializer> subSerializer();

	static const std::string LOCATION_PATH;
	static const std::string LOCATION_QUERY;
	static const std::string LOCATION_HEADER;
	static const std::string LOCATION_BODY;
	static const std::string LOCATION_FORM;
	static const std::string FORMAT_RAW;
	static const std::string FORMAT_BINARY;
	static const std::string FORMAT_JSON;

private:
	Serializer(const Serializer&);
	Serializer& operator = (const Serializer&);

private:
	std::ostream* _pStream;
	Poco::Net::HTTPRequest* _pRequest;
	Poco::Net::HTTPResponse* _pResponse;
	Poco::SharedPtr<PathSerializer> _pPathSerializer;
	Poco::SharedPtr<FormSerializer> _pQuerySerializer;
	Poco::SharedPtr<HeaderSerializer> _pHeaderSerializer;
	Poco::SharedPtr<RawSerializer> _pRawSerializer;
	Poco::SharedPtr<JSONSerializer> _pJSONSerializer;
	Poco::SharedPtr<FormSerializer> _pFormSerializer;

	friend class Deserializer;
};


//
// inlines
//
inline void Serializer::setRequest(Poco::Net::HTTPRequest& request)
{
	_pRequest = &request;
}


inline void Serializer::setResponse(Poco::Net::HTTPResponse& response)
{
	_pResponse = &response;
}


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_Serializer_INCLUDED
