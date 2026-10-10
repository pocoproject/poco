//
// AnalyticsEngineInputInfoSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED
#define TypeSerializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED


#include "ONVIF/AnalyticsEngineInputInfo.h"
#include "ONVIF/AnalyticsEngineInputInfoExtensionDeserializer.h"
#include "ONVIF/AnalyticsEngineInputInfoExtensionSerializer.h"
#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AnalyticsEngineInputInfo>
{
public:
	static void serialize(const std::string& name, const ONVIF::AnalyticsEngineInputInfo& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AnalyticsEngineInputInfo& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InputInfo"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::Config>>::serialize(REMOTING__NAMES[0], value.getInputInfo(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsEngineInputInfoExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED

