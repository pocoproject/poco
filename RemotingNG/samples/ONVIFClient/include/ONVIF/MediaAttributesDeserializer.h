//
// MediaAttributesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MediaAttributes_INCLUDED
#define TypeDeserializer_ONVIF_MediaAttributes_INCLUDED


#include "ONVIF/MediaAttributes.h"
#include "ONVIF/TrackAttributesDeserializer.h"
#include "ONVIF/TrackAttributesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MediaAttributes>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MediaAttributes& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MediaAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"TrackAttributes"s,"From"s,"Until"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		std::vector<ONVIF::TrackAttributes> gen_trackAttributes;
		ret = TypeDeserializer<std::vector<ONVIF::TrackAttributes>>::deserialize(REMOTING__NAMES[1], false, deser, gen_trackAttributes);
		if (ret) value.setTrackAttributes(gen_trackAttributes);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_from;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[2], true, deser, gen_from);
		if (ret) value.setFrom(gen_from);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_until;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[3], true, deser, gen_until);
		if (ret) value.setUntil(gen_until);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MediaAttributes_INCLUDED

