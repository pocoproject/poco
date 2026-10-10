//
// PTZStatusFilterOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZStatusFilterOptions_INCLUDED
#define TypeSerializer_ONVIF_PTZStatusFilterOptions_INCLUDED


#include "ONVIF/PTZStatusFilterOptions.h"
#include "ONVIF/PTZStatusFilterOptionsExtensionDeserializer.h"
#include "ONVIF/PTZStatusFilterOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZStatusFilterOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZStatusFilterOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZStatusFilterOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PanTiltStatusSupported"s,"ZoomStatusSupported"s,"PanTiltPositionSupported"s,"ZoomPositionSupported"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getPanTiltStatusSupported(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getZoomStatusSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getPanTiltPositionSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getZoomPositionSupported(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZStatusFilterOptionsExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZStatusFilterOptions_INCLUDED

