//
// RecordingEventFilterDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingEventFilter_INCLUDED
#define TypeDeserializer_ONVIF_RecordingEventFilter_INCLUDED


#include "ONVIF/FilterDeserializer.h"
#include "ONVIF/FilterSerializer.h"
#include "ONVIF/RecordingEventFilter.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingEventFilter>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingEventFilter& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingEventFilter& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Filter"s,"Before"s,"After"s};
		bool ret = false;
		std::vector<ONVIF::Filter> gen_filter;
		ret = TypeDeserializer<std::vector<ONVIF::Filter>>::deserialize(REMOTING__NAMES[0], true, deser, gen_filter);
		if (ret) value.setFilter(gen_filter);
		Poco::Optional<std::string> gen_before;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_before);
		if (ret) value.setBefore(gen_before);
		Poco::Optional<std::string> gen_after;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_after);
		if (ret) value.setAfter(gen_after);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingEventFilter_INCLUDED

