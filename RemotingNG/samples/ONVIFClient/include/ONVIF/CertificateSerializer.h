//
// CertificateSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Certificate_INCLUDED
#define TypeSerializer_ONVIF_Certificate_INCLUDED


#include "ONVIF/BinaryDataDeserializer.h"
#include "ONVIF/BinaryDataSerializer.h"
#include "ONVIF/Certificate.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Certificate>
{
public:
	static void serialize(const std::string& name, const ONVIF::Certificate& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Certificate& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"Certificate"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getCertificateID(), ser);
		TypeSerializer<ONVIF::BinaryData>::serialize(REMOTING__NAMES[1], value.getCertificate(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Certificate_INCLUDED

