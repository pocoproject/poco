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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_JSONRPC_Deserializer_INCLUDED
#define RemotingNG_JSONRPC_Deserializer_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include "Poco/RemotingNG/Deserializer.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API Deserializer: public Poco::RemotingNG::Deserializer
	/// The Deserializer implementation for the JSONRPC Transport.
	///
	/// For more information, please see the Serializer documentation.
{
public:
	Deserializer();
		/// Creates the Deserializer.

	~Deserializer();
		/// Destroys the Deserializer.

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
	int id() const;
	void associateSerializer(Serializer& ser);

protected:
	void resetImpl();
	void setupImpl(std::istream& inStream);

private:
	Deserializer(const Deserializer&);
	Deserializer& operator=(const Deserializer&);

	JSValue::Ptr findValue(const std::string& name, bool isMandatory, bool& found);

private:
	std::vector<JSValue::Ptr> _objectStack;
	std::vector<std::size_t> _indexStack;
	int _id;
	Serializer* _pAssociatedSerializer;
};


//
// inlines
//
inline int Deserializer::id() const
{
	return _id;
}


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_Deserializer_INCLUDED
