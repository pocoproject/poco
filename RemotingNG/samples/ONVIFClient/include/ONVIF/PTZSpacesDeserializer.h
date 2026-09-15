//
// PTZSpacesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZSpaces_INCLUDED
#define TypeDeserializer_ONVIF_PTZSpaces_INCLUDED


#include "ONVIF/PTZSpaces.h"
#include "ONVIF/PTZSpacesExtensionDeserializer.h"
#include "ONVIF/PTZSpacesExtensionSerializer.h"
#include "ONVIF/Space1DDescriptionDeserializer.h"
#include "ONVIF/Space1DDescriptionSerializer.h"
#include "ONVIF/Space2DDescriptionDeserializer.h"
#include "ONVIF/Space2DDescriptionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZSpaces>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZSpaces& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZSpaces& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AbsolutePanTiltPositionSpace"s,"AbsoluteZoomPositionSpace"s,"RelativePanTiltTranslationSpace"s,"RelativeZoomTranslationSpace"s,"ContinuousPanTiltVelocitySpace"s,"ContinuousZoomVelocitySpace"s,"PanTiltSpeedSpace"s,"ZoomSpeedSpace"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Space2DDescription> gen_absolutePanTiltPositionSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space2DDescription>>::deserialize(REMOTING__NAMES[0], false, deser, gen_absolutePanTiltPositionSpace);
		if (ret) value.setAbsolutePanTiltPositionSpace(gen_absolutePanTiltPositionSpace);
		std::vector<ONVIF::Space1DDescription> gen_absoluteZoomPositionSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[1], false, deser, gen_absoluteZoomPositionSpace);
		if (ret) value.setAbsoluteZoomPositionSpace(gen_absoluteZoomPositionSpace);
		std::vector<ONVIF::Space2DDescription> gen_relativePanTiltTranslationSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space2DDescription>>::deserialize(REMOTING__NAMES[2], false, deser, gen_relativePanTiltTranslationSpace);
		if (ret) value.setRelativePanTiltTranslationSpace(gen_relativePanTiltTranslationSpace);
		std::vector<ONVIF::Space1DDescription> gen_relativeZoomTranslationSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[3], false, deser, gen_relativeZoomTranslationSpace);
		if (ret) value.setRelativeZoomTranslationSpace(gen_relativeZoomTranslationSpace);
		std::vector<ONVIF::Space2DDescription> gen_continuousPanTiltVelocitySpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space2DDescription>>::deserialize(REMOTING__NAMES[4], false, deser, gen_continuousPanTiltVelocitySpace);
		if (ret) value.setContinuousPanTiltVelocitySpace(gen_continuousPanTiltVelocitySpace);
		std::vector<ONVIF::Space1DDescription> gen_continuousZoomVelocitySpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[5], false, deser, gen_continuousZoomVelocitySpace);
		if (ret) value.setContinuousZoomVelocitySpace(gen_continuousZoomVelocitySpace);
		std::vector<ONVIF::Space1DDescription> gen_panTiltSpeedSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[6], false, deser, gen_panTiltSpeedSpace);
		if (ret) value.setPanTiltSpeedSpace(gen_panTiltSpeedSpace);
		std::vector<ONVIF::Space1DDescription> gen_zoomSpeedSpace;
		ret = TypeDeserializer<std::vector<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[7], false, deser, gen_zoomSpeedSpace);
		if (ret) value.setZoomSpeedSpace(gen_zoomSpeedSpace);
		Poco::SharedPtr<ONVIF::PTZSpacesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZSpacesExtension>>::deserialize(REMOTING__NAMES[8], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZSpaces_INCLUDED

