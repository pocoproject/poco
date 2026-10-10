//
// VideoSourceConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED


#include "ONVIF/IntRectangleRangeDeserializer.h"
#include "ONVIF/IntRectangleRangeSerializer.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSourceConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSourceConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoSourceConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfProfiles"s};
		bool ret = false;
		Poco::Optional<int> gen_maximumNumberOfProfiles;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_maximumNumberOfProfiles);
		if (ret) value.setMaximumNumberOfProfiles(gen_maximumNumberOfProfiles);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSourceConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundsRange"s,"VideoSourceTokensAvailable"s,"Extension"s};
		bool ret = false;
		ONVIF::IntRectangleRange gen_boundsRange;
		ret = TypeDeserializer<ONVIF::IntRectangleRange>::deserialize(REMOTING__NAMES[0], true, deser, gen_boundsRange);
		if (ret) value.setBoundsRange(gen_boundsRange);
		std::vector<std::string> gen_videoSourceTokensAvailable;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], true, deser, gen_videoSourceTokensAvailable);
		if (ret) value.setVideoSourceTokensAvailable(gen_videoSourceTokensAvailable);
		Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfProfiles"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED

