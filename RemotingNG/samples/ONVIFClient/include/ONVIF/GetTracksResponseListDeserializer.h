//
// GetTracksResponseListDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GetTracksResponseList_INCLUDED
#define TypeDeserializer_ONVIF_GetTracksResponseList_INCLUDED


#include "ONVIF/GetTracksResponseItemDeserializer.h"
#include "ONVIF/GetTracksResponseItemSerializer.h"
#include "ONVIF/GetTracksResponseList.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GetTracksResponseList>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GetTracksResponseList& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::GetTracksResponseList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Track"s};
		bool ret = false;
		std::vector<ONVIF::GetTracksResponseItem> gen_track;
		ret = TypeDeserializer<std::vector<ONVIF::GetTracksResponseItem>>::deserialize(REMOTING__NAMES[0], false, deser, gen_track);
		if (ret) value.setTrack(gen_track);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GetTracksResponseList_INCLUDED

