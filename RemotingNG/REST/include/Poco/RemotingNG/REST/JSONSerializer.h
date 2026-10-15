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
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_JSONSerializer_INCLUDED
#define RemotingNG_REST_JSONSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco::RemotingNG::REST {


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

	~JSONSerializer() override;
		/// Destroys the JSONSerializer.

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
	void serializeOptionalBegin(const std::string& name, bool isSpecified) override;
	void serializeOptionalEnd(const std::string& name) override;
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

protected:
	void resetImpl() override;
	void setupImpl(std::ostream& stream) override;

private:
	void serializeData(const std::string& name, const std::string& val);
	void writeSeparator();
	[[nodiscard]] std::string escapeString(const std::string& val);

private:
	std::ostream* _pStream;
	int _indent = 0;
	std::vector<int> _state;
};


} // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_JSONSerializer_INCLUDED
