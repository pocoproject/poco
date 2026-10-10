//
// PTZPresetTourPresetDetailOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED


#include "ONVIF/PTZPresetTourPresetDetailOptions.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsExtensionSerializer.h"
#include "ONVIF/Space1DDescriptionDeserializer.h"
#include "ONVIF/Space1DDescriptionSerializer.h"
#include "ONVIF/Space2DDescriptionDeserializer.h"
#include "ONVIF/Space2DDescriptionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourPresetDetailOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourPresetDetailOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourPresetDetailOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetToken"s,"Home"s,"PanTiltPositionSpace"s,"ZoomPositionSpace"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getPresetToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getHome(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Space2DDescription>>::serialize(REMOTING__NAMES[2], value.getPanTiltPositionSpace(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[3], value.getZoomPositionSpace(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourPresetDetailOptionsExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED

