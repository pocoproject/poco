//
// CapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_Capabilities_INCLUDED
#define TypeDeserializer_ONVIF_Media_Capabilities_INCLUDED


#include "ONVIF/Media/Capabilities.h"
#include "ONVIF/Media/ProfileCapabilitiesDeserializer.h"
#include "ONVIF/Media/ProfileCapabilitiesSerializer.h"
#include "ONVIF/Media/StreamingCapabilitiesDeserializer.h"
#include "ONVIF/Media/StreamingCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::Capabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::Capabilities& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Media::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SnapshotUri"s,"Rotation"s,"VideoSourceMode"s,"OSD"s,"TemporaryOSDText"s,"EXICompression"s};
		bool ret = false;
		Poco::Optional<bool> gen_snapshotUri;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_snapshotUri);
		if (ret) value.setSnapshotUri(gen_snapshotUri);
		Poco::Optional<bool> gen_rotation;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_rotation);
		if (ret) value.setRotation(gen_rotation);
		Poco::Optional<bool> gen_videoSourceMode;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_videoSourceMode);
		if (ret) value.setVideoSourceMode(gen_videoSourceMode);
		Poco::Optional<bool> gen_oSD;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_oSD);
		if (ret) value.setOSD(gen_oSD);
		Poco::Optional<bool> gen_temporaryOSDText;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_temporaryOSDText);
		if (ret) value.setTemporaryOSDText(gen_temporaryOSDText);
		Poco::Optional<bool> gen_eXICompression;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[5], false, deser, gen_eXICompression);
		if (ret) value.setEXICompression(gen_eXICompression);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ProfileCapabilities"s,"StreamingCapabilities"s};
		bool ret = false;
		ONVIF::Media::ProfileCapabilities gen_profileCapabilities;
		ret = TypeDeserializer<ONVIF::Media::ProfileCapabilities>::deserialize(REMOTING__NAMES[0], true, deser, gen_profileCapabilities);
		if (ret) value.setProfileCapabilities(gen_profileCapabilities);
		ONVIF::Media::StreamingCapabilities gen_streamingCapabilities;
		ret = TypeDeserializer<ONVIF::Media::StreamingCapabilities>::deserialize(REMOTING__NAMES[1], true, deser, gen_streamingCapabilities);
		if (ret) value.setStreamingCapabilities(gen_streamingCapabilities);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SnapshotUri"s,"Rotation"s,"VideoSourceMode"s,"OSD"s,"TemporaryOSDText"s,"EXICompression"s,"http://www.onvif.org/ver10/media/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[5], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_Capabilities_INCLUDED

