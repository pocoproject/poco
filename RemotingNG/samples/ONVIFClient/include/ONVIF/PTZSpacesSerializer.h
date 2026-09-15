//
// PTZSpacesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZSpaces_INCLUDED
#define TypeSerializer_ONVIF_PTZSpaces_INCLUDED


#include "ONVIF/PTZSpaces.h"
#include "ONVIF/PTZSpacesExtensionDeserializer.h"
#include "ONVIF/PTZSpacesExtensionSerializer.h"
#include "ONVIF/Space1DDescriptionDeserializer.h"
#include "ONVIF/Space1DDescriptionSerializer.h"
#include "ONVIF/Space2DDescriptionDeserializer.h"
#include "ONVIF/Space2DDescriptionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZSpaces>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZSpaces& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZSpaces& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AbsolutePanTiltPositionSpace"s,"AbsoluteZoomPositionSpace"s,"RelativePanTiltTranslationSpace"s,"RelativeZoomTranslationSpace"s,"ContinuousPanTiltVelocitySpace"s,"ContinuousZoomVelocitySpace"s,"PanTiltSpeedSpace"s,"ZoomSpeedSpace"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Space2DDescription>>::serialize(REMOTING__NAMES[0], value.getAbsolutePanTiltPositionSpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[1], value.getAbsoluteZoomPositionSpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space2DDescription>>::serialize(REMOTING__NAMES[2], value.getRelativePanTiltTranslationSpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[3], value.getRelativeZoomTranslationSpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space2DDescription>>::serialize(REMOTING__NAMES[4], value.getContinuousPanTiltVelocitySpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[5], value.getContinuousZoomVelocitySpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[6], value.getPanTiltSpeedSpace(), ser);
		TypeSerializer<std::vector<ONVIF::Space1DDescription>>::serialize(REMOTING__NAMES[7], value.getZoomSpeedSpace(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZSpacesExtension>>::serialize(REMOTING__NAMES[8], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZSpaces_INCLUDED

