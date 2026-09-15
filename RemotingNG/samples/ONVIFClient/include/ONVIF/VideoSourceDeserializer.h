//
// VideoSourceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSource_INCLUDED
#define TypeDeserializer_ONVIF_VideoSource_INCLUDED


#include "ONVIF/ImagingSettingsDeserializer.h"
#include "ONVIF/ImagingSettingsSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "ONVIF/VideoSource.h"
#include "ONVIF/VideoSourceExtensionDeserializer.h"
#include "ONVIF/VideoSourceExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSource>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSource& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoSource& value)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSource& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Framerate"s,"Resolution"s,"Imaging"s,"Extension"s};
		bool ret = false;
		float gen_framerate;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_framerate);
		if (ret) value.setFramerate(gen_framerate);
		ONVIF::VideoResolution gen_resolution;
		ret = TypeDeserializer<ONVIF::VideoResolution>::deserialize(REMOTING__NAMES[1], true, deser, gen_resolution);
		if (ret) value.setResolution(gen_resolution);
		Poco::SharedPtr<ONVIF::ImagingSettings> gen_imaging;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingSettings>>::deserialize(REMOTING__NAMES[2], false, deser, gen_imaging);
		if (ret) value.setImaging(gen_imaging);
		Poco::SharedPtr<ONVIF::VideoSourceExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoSourceExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSource_INCLUDED

