//
// FindEventResultDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FindEventResult_INCLUDED
#define TypeDeserializer_ONVIF_FindEventResult_INCLUDED


#include "OASIS/WSN/B2/NotificationMessageHolderDeserializer.h"
#include "OASIS/WSN/B2/NotificationMessageHolderSerializer.h"
#include "ONVIF/FindEventResult.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FindEventResult>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FindEventResult& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FindEventResult& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"TrackToken"s,"Time"s,"Event"s,"StartStateEvent"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		std::string gen_trackToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_trackToken);
		if (ret) value.setTrackToken(gen_trackToken);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_time;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[2], true, deser, gen_time);
		if (ret) value.setTime(gen_time);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		OASIS::WSN::B2::NotificationMessageHolder gen_event;
		ret = TypeDeserializer<OASIS::WSN::B2::NotificationMessageHolder>::deserialize(REMOTING__NAMES[3], true, deser, gen_event);
		if (ret) value.setEvent(gen_event);
		bool gen_startStateEvent;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[4], true, deser, gen_startStateEvent);
		if (ret) value.setStartStateEvent(gen_startStateEvent);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FindEventResult_INCLUDED

