//
// GetTracksResponseItemDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GetTracksResponseItem_INCLUDED
#define TypeDeserializer_ONVIF_GetTracksResponseItem_INCLUDED


#include "ONVIF/GetTracksResponseItem.h"
#include "ONVIF/TrackConfigurationDeserializer.h"
#include "ONVIF/TrackConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GetTracksResponseItem>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GetTracksResponseItem& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::GetTracksResponseItem& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TrackToken"s,"Configuration"s};
		bool ret = false;
		std::string gen_trackToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_trackToken);
		if (ret) value.setTrackToken(gen_trackToken);
		ONVIF::TrackConfiguration gen_configuration;
		ret = TypeDeserializer<ONVIF::TrackConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_configuration);
		if (ret) value.setConfiguration(gen_configuration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GetTracksResponseItem_INCLUDED

