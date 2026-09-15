//
// BinarySerializer.h
//
// Library: RemotingNG
// Package: Serialization
// Module:  BinarySerializer
//
// Definition of the BinarySerializer class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_BinarySerializer_INCLUDED
#define RemotingNG_BinarySerializer_INCLUDED


#include "Poco/RemotingNG/Serializer.h"
#include "Poco/BinaryWriter.h"
#include <memory>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API BinarySerializer: public Serializer
	/// A generic Serializer using a proprietary binary
	/// serialization format based on the Poco::BinaryWriter
	/// class.
	///
	/// This class can be used by all Transport implementations
	/// using a binary serialization format that does not need
	/// to be interoperable with other network protocols.
	///
	/// This serializer uses a Poco::BinaryWriter for
	/// serializing data in binary form. Data is always
	/// written in the host's native endian format. To
	/// allow the deserializer finding out the endianess
	/// used by the serializer, a byte order mark is
	/// written at the beginning of the message.
	///
	/// Messages created by the BinarySerializer are not
	/// self describing and contain only minimal measures
	/// for detecting deserialization failures or message
	/// version incompatibilities.
{
public:
	BinarySerializer();
		/// Creates a BinarySerializer.

	~BinarySerializer();
		/// Destroys the BinarySerializer.

	void serializeEndPoint(const std::string& oid, const std::string& tid);
		/// Serializes the object and type ID of the service object.

	template <typename T>
	void serializeToken(T t)
		/// Serializes the given value, which must be a type directly supported
		/// by Poco::BinaryWriter.
	{
		(*this->_pWriter) << t;
	}

	// Serializer
	void serializeMessageBegin(const std::string& name, SerializerBase::MessageType type);
	void serializeMessageEnd(const std::string& name, SerializerBase::MessageType type);
	void serializeFaultMessage(const std::string& name, const Poco::Exception& exc);
	void serializeStructBegin(const std::string& name);
	void serializeStructEnd(const std::string& name);
	void serializeSequenceBegin(const std::string& name, Poco::UInt32 length);
	void serializeSequenceEnd(const std::string& name);
	void serializeNullableBegin(const std::string& name, bool isNull);
	void serializeNullableEnd(const std::string& name);
	void serialize(const std::string& name, Poco::Int8 value);
	void serialize(const std::string& name, Poco::UInt8 value);
	void serialize(const std::string& name, Poco::Int16 value);
	void serialize(const std::string& name, Poco::UInt16 value);
	void serialize(const std::string& name, Poco::Int32 value);
	void serialize(const std::string& name, Poco::UInt32 value);
	void serialize(const std::string& name, long value);
	void serialize(const std::string& name, unsigned long value);
#ifndef POCO_INT64_IS_LONG
	void serialize(const std::string& name, Poco::Int64 value);
	void serialize(const std::string& name, Poco::UInt64 value);
#endif
	void serialize(const std::string& name, float value);
	void serialize(const std::string& name, double value);
	void serialize(const std::string& name, bool value);
	void serialize(const std::string& name, char value);
	void serialize(const std::string& name, const std::string& value);
	void serialize(const std::string& name, const std::vector<char>& value);

protected:
	void resetImpl();
	void setupImpl(std::ostream& ostr);

	static const std::string MESSAGE_START_TAG;
	static const std::string MESSAGE_END_TAG;

	enum MessageCode
	{
		MESSAGE_CODE_REQUEST      = 0x01,
		MESSAGE_CODE_EVENT        = 0x02,
		MESSAGE_CODE_REPLY        = 0x11,
		MESSAGE_CODE_EVENT_REPLY  = 0x12,
		MESSAGE_CODE_FAULT        = 0x18
	};

private:
	using BinaryWriterPtr = std::unique_ptr<Poco::BinaryWriter>;

	BinaryWriterPtr _pWriter;

	friend class BinaryDeserializer;
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_BinarySerializer_INCLUDED
