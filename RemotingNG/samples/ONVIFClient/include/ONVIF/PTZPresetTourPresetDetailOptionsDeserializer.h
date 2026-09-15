//
// PTZPresetTourPresetDetailOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED


#include "ONVIF/PTZPresetTourPresetDetailOptions.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsExtensionSerializer.h"
#include "ONVIF/Space1DDescriptionDeserializer.h"
#include "ONVIF/Space1DDescriptionSerializer.h"
#include "ONVIF/Space2DDescriptionDeserializer.h"
#include "ONVIF/Space2DDescriptionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourPresetDetailOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourPresetDetailOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourPresetDetailOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetToken"s,"Home"s,"PanTiltPositionSpace"s,"ZoomPositionSpace"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_presetToken;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_presetToken);
		if (ret) value.setPresetToken(gen_presetToken);
		Poco::Optional<bool> gen_home;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_home);
		if (ret) value.setHome(gen_home);
		Poco::SharedPtr<ONVIF::Space2DDescription> gen_panTiltPositionSpace;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Space2DDescription>>::deserialize(REMOTING__NAMES[2], false, deser, gen_panTiltPositionSpace);
		if (ret) value.setPanTiltPositionSpace(gen_panTiltPositionSpace);
		Poco::SharedPtr<ONVIF::Space1DDescription> gen_zoomPositionSpace;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Space1DDescription>>::deserialize(REMOTING__NAMES[3], false, deser, gen_zoomPositionSpace);
		if (ret) value.setZoomPositionSpace(gen_zoomPositionSpace);
		Poco::SharedPtr<ONVIF::PTZPresetTourPresetDetailOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourPresetDetailOptionsExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED

