//
// ScalarSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  ScalarSerializer
//
// Definition of the ScalarSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_ScalarSerializer_INCLUDED
#define RemotingNG_REST_ScalarSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API ScalarSerializer: public Poco::RemotingNG::Serializer
	/// Serialization of scalar values for form, path or header.
{
public:
	ScalarSerializer();
		/// Creates the ScalarSerializer.

	~ScalarSerializer();
		/// Destroys the ScalarSerializer.

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
	virtual void serializeData(const std::string& name, const std::string& val) = 0;
	virtual void serializeBuffer(const std::string& name, const std::vector<char>& val);

private:
	static const std::string VAL_TRUE;
	static const std::string VAL_FALSE;

	friend class ScalarDeserializer;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_ScalarSerializer_INCLUDED
