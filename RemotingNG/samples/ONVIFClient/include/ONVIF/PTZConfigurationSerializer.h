//
// PTZConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZConfiguration_INCLUDED
#define TypeSerializer_ONVIF_PTZConfiguration_INCLUDED


#include "ONVIF/PTZConfiguration.h"
#include "ONVIF/PTZConfigurationExtensionDeserializer.h"
#include "ONVIF/PTZConfigurationExtensionSerializer.h"
#include "ONVIF/PTZSpeedDeserializer.h"
#include "ONVIF/PTZSpeedSerializer.h"
#include "ONVIF/PanTiltLimitsDeserializer.h"
#include "ONVIF/PanTiltLimitsSerializer.h"
#include "ONVIF/ZoomLimitsDeserializer.h"
#include "ONVIF/ZoomLimitsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZConfiguration>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0]);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MoveRamp"s,"PresetRamp"s,"PresetTourRamp"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2]);
	}

	static void serialize(const std::string& name, const ONVIF::PTZConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::PTZConfiguration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MoveRamp"s,"PresetRamp"s,"PresetTourRamp"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getMoveRamp(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getPresetRamp(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getPresetTourRamp(), ser);
	}

	static void serializeImpl(const ONVIF::PTZConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"NodeToken"s,"DefaultAbsolutePantTiltPositionSpace"s,"DefaultAbsoluteZoomPositionSpace"s,"DefaultRelativePanTiltTranslationSpace"s,"DefaultRelativeZoomTranslationSpace"s,"DefaultContinuousPanTiltVelocitySpace"s,"DefaultContinuousZoomVelocitySpace"s,"DefaultPTZSpeed"s,"DefaultPTZTimeout"s,"PanTiltLimits"s,"ZoomLimits"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getNodeToken(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[1], value.getDefaultAbsolutePantTiltPositionSpace(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[2], value.getDefaultAbsoluteZoomPositionSpace(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[3], value.getDefaultRelativePanTiltTranslationSpace(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[4], value.getDefaultRelativeZoomTranslationSpace(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[5], value.getDefaultContinuousPanTiltVelocitySpace(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[6], value.getDefaultContinuousZoomVelocitySpace(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZSpeed>>::serialize(REMOTING__NAMES[7], value.getDefaultPTZSpeed(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[8], value.getDefaultPTZTimeout(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PanTiltLimits>>::serialize(REMOTING__NAMES[9], value.getPanTiltLimits(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ZoomLimits>>::serialize(REMOTING__NAMES[10], value.getZoomLimits(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZConfigurationExtension>>::serialize(REMOTING__NAMES[11], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZConfiguration_INCLUDED

