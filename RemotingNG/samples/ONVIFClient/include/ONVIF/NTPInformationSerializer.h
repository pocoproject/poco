//
// NTPInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NTPInformation_INCLUDED
#define TypeSerializer_ONVIF_NTPInformation_INCLUDED


#include "ONVIF/NTPInformation.h"
#include "ONVIF/NTPInformationExtensionDeserializer.h"
#include "ONVIF/NTPInformationExtensionSerializer.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NTPInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::NTPInformation& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NTPInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"NTPFromDHCP"s,"NTPManual"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getFromDHCP(), ser);
		TypeSerializer<std::vector<ONVIF::NetworkHost>>::serialize(REMOTING__NAMES[1], value.getNTPFromDHCP(), ser);
		TypeSerializer<std::vector<ONVIF::NetworkHost>>::serialize(REMOTING__NAMES[2], value.getNTPManual(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NTPInformationExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NTPInformation_INCLUDED

