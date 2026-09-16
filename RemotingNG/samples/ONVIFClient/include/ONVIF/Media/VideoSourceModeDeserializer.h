//
// VideoSourceModeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_VideoSourceMode_INCLUDED
#define TypeDeserializer_ONVIF_Media_VideoSourceMode_INCLUDED


#include "ONVIF/Media/VideoSourceMode.h"
#include "ONVIF/Media/VideoSourceModeExtensionDeserializer.h"
#include "ONVIF/Media/VideoSourceModeExtensionSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::VideoSourceMode>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::VideoSourceMode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Media::VideoSourceMode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"Enabled"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		Poco::Optional<bool> gen_enabled;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::VideoSourceMode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaxFramerate"s,"MaxResolution"s,"Encodings"s,"Reboot"s,"Description"s,"Extension"s};
		bool ret = false;
		float gen_maxFramerate;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_maxFramerate);
		if (ret) value.setMaxFramerate(gen_maxFramerate);
		ONVIF::VideoResolution gen_maxResolution;
		ret = TypeDeserializer<ONVIF::VideoResolution>::deserialize(REMOTING__NAMES[1], true, deser, gen_maxResolution);
		if (ret) value.setMaxResolution(gen_maxResolution);
		std::vector<std::string> gen_encodings;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], true, deser, gen_encodings);
		if (ret) value.setEncodings(gen_encodings);
		bool gen_reboot;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_reboot);
		if (ret) value.setReboot(gen_reboot);
		Poco::Optional<std::string> gen_description;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		Poco::SharedPtr<ONVIF::Media::VideoSourceModeExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Media::VideoSourceModeExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"Enabled"s,"http://www.onvif.org/ver10/media/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_VideoSourceMode_INCLUDED

