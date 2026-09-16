//
// PTZConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_PTZConfiguration_INCLUDED


#include "ONVIF/PTZConfiguration.h"
#include "ONVIF/PTZConfigurationExtensionDeserializer.h"
#include "ONVIF/PTZConfigurationExtensionSerializer.h"
#include "ONVIF/PTZSpeedDeserializer.h"
#include "ONVIF/PTZSpeedSerializer.h"
#include "ONVIF/PanTiltLimitsDeserializer.h"
#include "ONVIF/PanTiltLimitsSerializer.h"
#include "ONVIF/ZoomLimitsDeserializer.h"
#include "ONVIF/ZoomLimitsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::PTZConfiguration& value)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MoveRamp"s,"PresetRamp"s,"PresetTourRamp"s};
		bool ret = false;
		Poco::Optional<int> gen_moveRamp;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_moveRamp);
		if (ret) value.setMoveRamp(gen_moveRamp);
		Poco::Optional<int> gen_presetRamp;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_presetRamp);
		if (ret) value.setPresetRamp(gen_presetRamp);
		Poco::Optional<int> gen_presetTourRamp;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_presetTourRamp);
		if (ret) value.setPresetTourRamp(gen_presetTourRamp);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZConfiguration& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		int gen_useCount;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], true, deser, gen_useCount);
		if (ret) value.setUseCount(gen_useCount);
		}
		
		static const std::string REMOTING__NAMES[] = {"NodeToken"s,"DefaultAbsolutePantTiltPositionSpace"s,"DefaultAbsoluteZoomPositionSpace"s,"DefaultRelativePanTiltTranslationSpace"s,"DefaultRelativeZoomTranslationSpace"s,"DefaultContinuousPanTiltVelocitySpace"s,"DefaultContinuousZoomVelocitySpace"s,"DefaultPTZSpeed"s,"DefaultPTZTimeout"s,"PanTiltLimits"s,"ZoomLimits"s,"Extension"s};
		bool ret = false;
		std::string gen_nodeToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_nodeToken);
		if (ret) value.setNodeToken(gen_nodeToken);
		Poco::Optional<Poco::URI> gen_defaultAbsolutePantTiltPositionSpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[1], false, deser, gen_defaultAbsolutePantTiltPositionSpace);
		if (ret) value.setDefaultAbsolutePantTiltPositionSpace(gen_defaultAbsolutePantTiltPositionSpace);
		Poco::Optional<Poco::URI> gen_defaultAbsoluteZoomPositionSpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[2], false, deser, gen_defaultAbsoluteZoomPositionSpace);
		if (ret) value.setDefaultAbsoluteZoomPositionSpace(gen_defaultAbsoluteZoomPositionSpace);
		Poco::Optional<Poco::URI> gen_defaultRelativePanTiltTranslationSpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[3], false, deser, gen_defaultRelativePanTiltTranslationSpace);
		if (ret) value.setDefaultRelativePanTiltTranslationSpace(gen_defaultRelativePanTiltTranslationSpace);
		Poco::Optional<Poco::URI> gen_defaultRelativeZoomTranslationSpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[4], false, deser, gen_defaultRelativeZoomTranslationSpace);
		if (ret) value.setDefaultRelativeZoomTranslationSpace(gen_defaultRelativeZoomTranslationSpace);
		Poco::Optional<Poco::URI> gen_defaultContinuousPanTiltVelocitySpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[5], false, deser, gen_defaultContinuousPanTiltVelocitySpace);
		if (ret) value.setDefaultContinuousPanTiltVelocitySpace(gen_defaultContinuousPanTiltVelocitySpace);
		Poco::Optional<Poco::URI> gen_defaultContinuousZoomVelocitySpace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[6], false, deser, gen_defaultContinuousZoomVelocitySpace);
		if (ret) value.setDefaultContinuousZoomVelocitySpace(gen_defaultContinuousZoomVelocitySpace);
		Poco::SharedPtr<ONVIF::PTZSpeed> gen_defaultPTZSpeed;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZSpeed>>::deserialize(REMOTING__NAMES[7], false, deser, gen_defaultPTZSpeed);
		if (ret) value.setDefaultPTZSpeed(gen_defaultPTZSpeed);
		Poco::Optional<std::string> gen_defaultPTZTimeout;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[8], false, deser, gen_defaultPTZTimeout);
		if (ret) value.setDefaultPTZTimeout(gen_defaultPTZTimeout);
		Poco::SharedPtr<ONVIF::PanTiltLimits> gen_panTiltLimits;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PanTiltLimits>>::deserialize(REMOTING__NAMES[9], false, deser, gen_panTiltLimits);
		if (ret) value.setPanTiltLimits(gen_panTiltLimits);
		Poco::SharedPtr<ONVIF::ZoomLimits> gen_zoomLimits;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ZoomLimits>>::deserialize(REMOTING__NAMES[10], false, deser, gen_zoomLimits);
		if (ret) value.setZoomLimits(gen_zoomLimits);
		Poco::SharedPtr<ONVIF::PTZConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZConfigurationExtension>>::deserialize(REMOTING__NAMES[11], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MoveRamp"s,"PresetRamp"s,"PresetTourRamp"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZConfiguration_INCLUDED

