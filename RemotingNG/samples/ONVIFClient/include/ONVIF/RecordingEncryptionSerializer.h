//
// RecordingEncryptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingEncryption_INCLUDED
#define TypeSerializer_ONVIF_RecordingEncryption_INCLUDED


#include "ONVIF/RecordingEncryption.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingEncryption>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::RecordingEncryption& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::RecordingEncryption& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
	}

	static void serializeImpl(const ONVIF::RecordingEncryption& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"KID"s,"Key"s,"Track"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getKID(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getKey(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getTrack(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingEncryption_INCLUDED

